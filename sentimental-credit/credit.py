# A program that finds out whether a credit card number is from VISA, AMEX or Mastercard or just invalid

from cs50 import get_string

# Get input for credit card number
number = get_string("Credit card number: ")

# Define length of input number
l = len(number)

# Initialise sum of Luhn's algorithm
sum = 0

# Apply Luhn's algorithm
for i in range(l):
    n = int(number[l-i-1])
    if i % 2 == 0:
        sum = sum + n
    else:
        sum = sum + (n * 2) // 10 + (n * 2) % 10

# First digit of credit card number
n1 = int(number[0])
# First two digits of credit card number
n12 = int(number[0:2])

# Check if last digit is 0
if sum % 10 != 0:
    print("INVALID")
else:
    # First digit of 4 and length of 16 or 13: VISA
    if n1 == 4 and (l == 16 or l == 13):
        print("VISA")
    # First digits of 34 or 37 and length of 15: AMEX
    elif (n12 == 34 or n12 == 37) and l == 15:
        print("AMEX")
    # First digits of 51 to 55 and length of 16: MASTERCARD
    elif n12 > 50 and n12 < 56 and l == 16:
        print("MASTERCARD")
    # Invalid if none of the above fit
    else:
        print("INVALID")