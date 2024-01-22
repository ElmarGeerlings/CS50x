// A program that finds out the grade reading level of a text

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    string text = get_string("Text: ");
    // printf("%s\n",text);

    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    // Calculating grade using the Coleman-Liau index
    double L = letters * 100.0 / words;   // L = letters / ( words / 100 ) = letters * 100 / words
    double S = sentences * 100.0 / words; // // S = letters / ( words / 100 ) = sentences * 100 / words
    double index = 0.0588 * L - 0.296 * S - 15.8; // formula for Coleman-Liau index

    int grade = round(index);   // rounding the index to the nearest integer to get the grade

    // Print the grade
    if (grade > 16)
    {
        printf("Grade 16+\n");
    }
    else if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}

int count_letters(string text)
{
    int letters = 0;

    for (int i = 0 ; i < strlen(text); i++) //iterate for the length of the text
    {
        // Add a counter to letters if there is either a lowercase or a uppercase letter
        if (islower(text[i]) || isupper(text[i]))
        {
            letters++;
        }
    }

    return letters;
}

int count_words(string text)
{
    int words = 1; // There is at least one word

    for (int i = 0 ; i < strlen(text); i++) //iterate for the length of the text
    {
        // Add a counter to words if there is a space
        if (text[i] == 32)  // ASCII number for space is 32
        {
            words++;
        }
    }

    return words;
}

int count_sentences(string text)
{
    int sentences = 0;

    for (int i = 0 ; i < strlen(text); i++) //iterate for the length of the text
    {
        // Add a counter to sentences if there is one of: ! . ?
        if (text[i] == 33 || text[i] == 46 || text[i] == 63) //ASCII correspondence: 33:!  46:.  63:?
        {
            sentences++;
        }
    }

    return sentences;
}