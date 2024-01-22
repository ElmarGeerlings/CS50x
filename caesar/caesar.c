// A program that encrypts messages using Caesar's cypher

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

bool only_digits(string input);
char rotate(char c, int key);

int main(int argc, string argv[])
{
    // Define user's input for key as a string
    string input = argv[1];

    // Check if there is one single command line argument that only consits of digits
    if (argc != 2 || !only_digits(input))
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // Convert the key from a string to a number
    int key = atoi(input);
    printf("key: %i\n", key);

    // Only use the key if it's a positive number
    if (key < 0)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // Ask user for text to encrypt
    string plaintext = get_string("plaintext:  ");

    int n = strlen(plaintext);
    char ciphertext[n];

    // Print the ciphertext
    printf("ciphertext: ");

    // Encrypt and print every character from the input text one by one using function "rotate"
    for (int j = 0; j < n; j++)
    {
        char c = plaintext[j];
        ciphertext[j] = rotate(c, key);
        printf("%c", ciphertext[j]);
    }

    printf("\n");
}

// Function that checks whether the input only consists of numbers
bool only_digits(string input)
{
    int len = strlen(input);

    // Return "false" if the string contains a non-digit
    for (int i = 0 ; i < len; i++)
    {
        if (!isdigit(input[i]))
        {
            return false;
        }
    }

    // Return "true" if the string only contains digits
    return true;
}

// Function that encrypts a character of the input text
char rotate(char c, int key)
{
    char r;

    // Encrypt lower case character
    // (c + key - 97) % 26 to get the letter number (a=1,b=2 etc.)
    // Sum with 97 to get the ASCII value of the letter
    if (islower(c))
    {
        r = 97 + ((c + key - 97) % 26);
    }

    // Encrypt upper case characters
    else if (isupper(c))
    {
        r = 65 + ((c + key - 65) % 26);
    }

    // If input character is not a letter, it will remain unchanged
    else
    {
        r = c;
    }

    // Return encrypted character
    return r;
}