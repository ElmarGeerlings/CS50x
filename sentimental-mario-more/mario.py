# A program that builds a right aligned pyramid and a left aligned pyramid

import cs50

# Repeat asking for height until an integer between 1 and 8 is given
while True:
    height = cs50.get_int("Height: ")
    if height > 0 and height < 9:
        break

# Create pyramid row by row
for i in range(1, height + 1):
    print(" " * (height - i), end="")
    print("#" * i, end="")
    print("  ", end="")
    print("#" * i)
