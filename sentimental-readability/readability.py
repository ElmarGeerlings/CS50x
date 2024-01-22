# A program that finds out the grade reading level of a text

from cs50 import get_string

# Get text from user
text = get_string("Text: ")

# Count the number of letters
letters = 0
for i in text:
    if i.isalpha():
        letters += 1
print(letters)

# Count the number of words, which is the number of spaces plus one
words = 1
for i in text:
    if i.isspace():
        words += 1
print(words)

# Count the number of sentences, defined by the occurences of "!", "." and "?"
sentences = 0
for i in text:
    if i == "?" or i == "." or i == "!":
        sentences += 1
print(sentences)

# Apply the formule for the grade
l = letters * 100.0 / words
s = sentences * 100.0 / words
index = 0.0588 * l - 0.296 * s - 15.8
grade = round(index)

# Print grade
if (grade > 16):
    print("Grade 16+")
elif (grade < 1):
    print("Before Grade 1")
else:
    print("Grade " + str(grade))