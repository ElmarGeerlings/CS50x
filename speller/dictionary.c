// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <cs50.h>
#include <strings.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

bool compare(char *copy, node n);

// Number of buckets in hash table
const unsigned int N = 150000;

// Number of words loaded from the dictionary
int nwords;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Length of the input word
    int n = strlen(word);

    // An array to store a copy of the input word in
    char copy[LENGTH + 1];

    // Store lowercase version of the input word in the copy array
    for (int i = 0; i < n; i++)
    {
        copy[i] = tolower(word[i]);
    }
    // Add null to end of the string
    copy[n] = '\0';

    // Hash value of the input word
    int value = hash(copy);

    // Initialise value of cursor to the header of the hashtable
    node *cursor = table[value];

    // Go through the linked list to find the word
    while (cursor != NULL)
    {
        // Compare to input word
        if (strcasecmp(copy, cursor->word) == 0)
        {
            return true;
        }

        // Go to next element in the linked list
        cursor = cursor->next;
    }

    // Return false if the word is not found
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int n = strlen(word);
    int sum = 0;

    // Formula for generating hash number
    for (int i = 0; i < n; i++)
    {
        sum = sum + i * word[i];
    }
    int hash = (n * sum) % N;

    // Return hash number
    return hash;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open file
    FILE *dict = fopen(dictionary, "r");

    // Check whether file was successfully opened
    if (dict == NULL)
    {
        printf("Could not open file.\n");
        return false;
    }

    // Array to put store a word in
    char word1[LENGTH + 1];

    // Initialise counter for the amount of words in the dictionary
    nwords = 0;

    // Scan until the end of the dictionary (EOF)
    while (fscanf(dict, "%s", word1) != EOF)
    {
        // Get memory for a new node n
        node *n = malloc(sizeof(node));

        // Unload and return if unsuccessful
        if (n == NULL)
        {
            unload();
            return false;
        }

        // Copy the word into node n
        strcpy(n->word, word1);

        // Hash the word
        int value = hash(n->word);

        // Declare pointer towards the same node as the header
        n->next = table[value];

        // Let the header point towards the new node n
        table[value] = n;

        // Update counter for number of words in dictionary
        nwords++;
    }

    // Close the dictionary file
    fclose(dict);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // Return the value of the words that were counted while loading the dictionary
    return nwords;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        // Initialise value of cursor to the header of the hashtable
        node *cursor = table[i];

        // Free linked list
        while (cursor != NULL)
        {
            // Make a node that points towards in the same direction as cursor
            node *tmp = cursor;

            // Move cursor to the next element
            cursor = cursor->next;

            // Free element where tmp is pointing to
            free(tmp);
        }
    }

    return true;
}
