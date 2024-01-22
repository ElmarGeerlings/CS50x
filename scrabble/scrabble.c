// A program that finds out which player's word is worth the most points on scrabblec

#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // Print winner (or tie)
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }

    if (score1 < score2)
    {
        printf("Player 2 wins!\n");
    }

    if (score1 == score2)
    {
        printf("Tie!\n");
    }
}

int compute_score(string word)
{
    int score = 0;

    for (int i = 0 ; i < strlen(word); i++)   //iterate for the amount of letters in the word
    {
        // Add the score of the letter word[i] to the total score of the word

        if (islower(word[i]))
        {
            score = score + POINTS[word[i] - 97];   // -97 for lowercase letters to convert a,b,c,... to 0,1,2,...
        }

        if (isupper(word[i]))
        {
            score = score + POINTS[word[i] - 65];   // -65 for uppercase letters to convert A,B,C,... to 0,1,2,...
        }
    }

    return score;
}
