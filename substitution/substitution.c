// A program that implements a substitution cipher

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

bool only_letters(string input);
char rotate(char c, string input);

int main(int argc, string argv[])
{
    string input = argv[1];

    // Check if there is one single command line argument and a corret cipher is given
    if (argc != 2 || !only_letters(input))
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // Check whether the cipher contains 26 characters
    int l =  strlen(input);
    if (l != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    // Check whether there are duplicate characters
    for (int i = 0; i < l; i++)
    {
        for (int j = i + 1; j < l; j++)
        {
            if (input[i] == input[j])
            {
                printf("Key must not contain repeated characters.\n");
                return 1;
            }
        }
    }

    // Ask user for text to encrypt
    string plaintext = get_string("plaintext:  ");

    int n = strlen(plaintext);
    char ciphertext[n];

    // Print the ciphertext
    printf("ciphertext: ");

    // Encrypt and print every character from the input text one by one using function "rotate"
    for (int k = 0; k < n; k++)
    {
        char c = plaintext[k];
        ciphertext[k] = rotate(c, input);
        printf("%c", ciphertext[k]);
    }

    printf("\n");
}

// Function that checks whether the input only consists of letters
bool only_letters(string input)
{
    int len = strlen(input);

    // Return "false" if the string contains a non-letter
    for (int i = 0 ; i < len; i++)
    {
        if (!isalpha(input[i]))
        {
            return false;
        }
    }

    // Return "true" if the string only contains letters
    return true;
}

// Function that encrypts a character of the input text
char rotate(char c, string input)
{
    char r;

    // Encrypt lower case letters
    // c - 97 to get the letter number (a=1,b=2 etc.) and thus array number in cipher
    if (islower(c))
    {
        r = input[c - 97];
        r = tolower(r);     // preserve lowercase
    }

    // Encrypt upper case characters
    else if (isupper(c))
    {
        r = input[c - 65];
        r = toupper(r);     // preserve uppercase
    }

    // If input character is not a letter, it will remain unchanged
    else
    {
        r = c;
    }

    // Return encrypted character
    return r;
}