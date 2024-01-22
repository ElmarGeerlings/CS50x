import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# export API_KEY=pk_d551b134a0e342b79b12d8292a165405

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Search up data with sql
    data = db.execute("SELECT symbol, name, price, shares, SUM(shares), cash FROM transactions \
    WHERE userID = ? GROUP BY symbol HAVING SUM(shares) > 0", session["user_id"])

    total = 0
    for i in range(len(data)):
        newprice = lookup(data[i]["symbol"])["price"]
        value = newprice * data[i]["SUM(shares)"]
        total = total + value
        data[i]["shares"] = data[i]["SUM(shares)"]
        data[i]["price"] = newprice
        data[i]["cash"] = value

    currentcash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
    total = currentcash + total

    if not session['message'] == "empty":
        message = session['message']
        session['message'] = "empty"
        return render_template("index2.html", data=data, cash=currentcash, total=total, message=message)

    return render_template("index.html", data=data, cash=currentcash, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        '''
        # Show page with symbol inputted if coming from index page
        if not request.form.get("button") == "submit":
            return render_template("buy2.html", symbol=request.form.get("button"))
        '''
        # Ensure that there is input for the symbol
        if not request.form.get("symbol"):
            return apology("invalid symbol", 400)

        # Ensure that the stock exists
        stock = lookup(request.form.get("symbol"))
        if stock == None:
            return apology("invalid symbol", 400)

        # Ensure that user has input a number of shares
        if not request.form.get("shares"):
            return apology("missing shares", 400)

        # Make sure that number of shares is a positive integer
        if ("." in request.form.get("shares")) or ((request.form.get("shares")).isnumeric() == False):
            return apology("invalid shares", 400)

        # Make sure that the user has enough cash to buy stocks
        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]

        stock = lookup(request.form.get("symbol"))
        price = stock["price"]
        shares = int(request.form.get("shares"))

        newcash = cash - price * shares

        if newcash < 0:
            return apology("can't afford", 400)

        # Define data before loading into table
        userID = session["user_id"]
        symbol = stock["symbol"]
        name = stock["name"]
        shares = int(request.form.get("shares"))
        date_time = datetime.now().strftime("%d-%m-%Y %H:%M:%S")

        # Load data into table
        db.execute("INSERT INTO transactions (userID, cash, symbol, name, price, shares, datetime) \
        VALUES(?, ?, ?, ?, ?, ?, ?)", userID, newcash, symbol, name, price, shares, date_time)
        db.execute("UPDATE users SET cash = ? WHERE id = ?", newcash, userID)

        session['message'] = "Bought!"

        # Go back to homepage
        return redirect("/")
    else:
        # Show "buy" page
        return render_template("buy.html")


'''
SQL table:

CREATE TABLE transactions
(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
userID NUMERIC NOT NULL,
cash NUMERIC NOT NULL,
symbol TEXT NOT NULL,
name TEXT NOT NULL,
price TEXT NOT NULL,
shares NUMERIC NOT NULL,
datetime TEXT NOT NULL);
'''


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # Load up data
    data = db.execute("SELECT symbol, price, shares, datetime FROM transactions \
    WHERE userID = ?", session["user_id"])

    # Convert "price" values from string to float
    for i in range(len(data)):
        data[i]["price"] = float(data[i]["price"])

    # Return "history" page
    return render_template("history.html", data=data)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        session['message'] = "empty"
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":

        # Ensure a symbol is submitted
        if not request.form.get("symbol"):
            return apology("missing symbol", 400)

        # Ensure the stock exists in the database
        stock = lookup(request.form.get("symbol"))
        if stock == None:
            return apology("invalid symbol", 400)

        # Display name and current price of stock associated with input symbol
        name = stock["name"]
        price = stock["price"]
        symbol = stock["symbol"]
        return render_template("quoted.html", name=name, price=price, symbol=symbol)
    else:
        # Display quote page
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)
        password = request.form.get("password")

        # Check if username already exists
        username = request.form.get("username")
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)
        if len(rows) == 1:
            return apology("username already exists", 400)

        # Check if password is long enough
        if len(password) < 4:
            return apology("password must contain at least 4 characters", 400)

        # Check if password contains both letters and numbers
        if (password.isalpha() == True) or (password.isdigit() == True):
            return apology("password must contain both letters and numbers", 400)

        # Check whether passwords match
        if request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords must match", 400)

        # Hash password
        hash = generate_password_hash(request.form.get("password"))

        # Insert data into table
        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, hash)

        # Log user in
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)
        session["user_id"] = rows[0]["id"]

        # Go back to homepage
        session['message'] = "empty"
        return redirect("/")

    else:
        # Display register page
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        '''
        # Show page with symbol inputted if coming from index page
        if not request.form.get("button") == "submit":
            return render_template("sell2.html", symbol=request.form.get("button"))
        '''
        # Ensure that there is input for the symbol
        if not request.form.get("symbol"):
            return apology("missing symbol", 400)

        # Ensure that user has input a number of shares
        if not request.form.get("shares"):
            return apology("missing shares", 400)

        # Create new variables for clarity
        userID = session["user_id"]
        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))
        minshares = shares * -1

        # Check if user has enough shars to sell
        totalshares = db.execute("SELECT SUM(shares) FROM transactions \
        WHERE userID = ? AND symbol = ?", userID, symbol)[0]["SUM(shares)"]

        if shares > totalshares:
            return apology("too many shares", 400)

        # Get price and name of stock
        price = lookup(symbol)["price"]
        name = lookup(symbol)["name"]

        # Calculate values of sold stocks and add to user cash
        cash = db.execute("SELECT cash FROM users WHERE id = ?", userID)[0]["cash"]
        value = price * shares
        newcash = cash + value

        # Time of transaction
        date_time = datetime.now().strftime("%d-%m-%Y %H:%M:%S")

        # Load data into table
        db.execute("INSERT INTO transactions (userID, cash, symbol, name, price, shares, datetime) \
        VALUES(?, ?, ?, ?, ?, ?, ?)", userID, newcash, symbol, name, price, minshares, date_time)
        db.execute("UPDATE users SET cash = ? WHERE id = ?", newcash, userID)

        # Go back to homepage
        session['message'] = "Sold!"
        return redirect("/")
    else:
        # Check which stocks the user owns
        stocks = db.execute("SELECT symbol FROM transactions WHERE userID = ? \
        GROUP BY symbol HAVING SUM(shares) > 0", session["user_id"])

        # Show "sell" page
        return render_template("sell.html", stocks=stocks)


@app.route("/change", methods=["GET", "POST"])
@login_required
def change():
    """Change password"""
    if request.method == "POST":

        # Ensure password was submitted
        if not request.form.get("password"):
            return apology("must provide password", 400)
        password = request.form.get("password")

        # Check if password is long enough
        if len(password) < 4:
            return apology("password must contain at least 4 characters", 400)

        # Check if password contains both letters and numbers
        if (password.isalpha() == True) or (password.isdigit() == True):
            return apology("password must contain both letters and numbers", 400)

        # Check whether passwords match
        if password != request.form.get("confirmation"):
            return apology("passwords must match", 400)

        # Hash password
        hash = generate_password_hash(request.form.get("password"))

        # Insert data into table
        db.execute("UPDATE users SET hash = ? WHERE id = ?", hash, session["user_id"])

        # Go back to homepage
        session['message'] = "Password change successful!"
        return redirect("/")
    else:
        # Show "Change Password" page
        return render_template("change.html")


@app.route("/addcash", methods=["GET", "POST"])
@login_required
def addcash():
    if request.method == "POST":

        # Ensure that user has input a number of shares
        if not request.form.get("amount"):
            return apology("amount missing", 400)

        # Make sure that number of shares is a positive integer
        if ((request.form.get("amount")).isnumeric() == False):
            return apology("invalid amount", 400)

        # Load in current cash
        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]

        # Increase cash and update database
        newcash = int(cash) + int(request.form.get("amount"))
        db.execute("UPDATE users SET cash = ? WHERE id = ?", newcash, session["user_id"])

        # Go back to homepage
        session['message'] = "Cash added!"
        return redirect("/")
    else:
        # Show "Add Cash" page
        return render_template("addcash.html")