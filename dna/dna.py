# A program that identifies a person based on their DNA

import csv
import sys


def main():

    # Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        exit(1)

    # Read database file into a variable
    database = sys.argv[1]
    with open(database) as file:
        reader = csv.reader(file)
        data = list(reader)

    # Read DNA sequence file into a variable
    sequence = sys.argv[2]
    sfile = open(sequence)
    seq = sfile.read()

    # Find longest match of each STR in DNA sequence
    longest = []        # Array to store longest match for each STR in
    col = len(data[0])  # Length of one column in the data

    for i in range(1, col):
        STR = data[0][i]
        longestmatch = longest_match(seq, STR)
        longest.append(longestmatch)

    # Check database for matching profiles
    found = 0       # Variable that defines whether match is found

    for i in range(1, len(data)):
        # Convert data to int
        for j in range(col-1):
            data[i][j + 1] = int(data[i][j + 1])
        # Set found=1 and break if a match is found
        if data[i][1:] == longest:
            found = 1
            break

    # Print name if a match is found, otherwise print "No match"
    if found == 1:
        print(data[i][0])
    else:
        print("No match")

    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
