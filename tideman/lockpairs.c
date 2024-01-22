// A program that runs a tideman election

#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

bool cycle;
int back;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

void make_cycle(int winner, int loser, int n);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 3;

    add_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)  // Check if vote name matches a candidate's name
        {
            ranks[rank] = i;   // Update preference to candidate i
            return true;
        }
    }

    return false;   // Return false if the given name does not match a candidate's name
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    pairs[0].winner = 0;
    pairs[0].loser = 1;

    pairs[1].winner = 2;
    pairs[1].loser = 0;

    pairs[2].winner = 1;
    pairs[2].loser = 2;

    lock_pairs();
    print_winner();
    return;
}


// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        printf("winner: %s\n", candidates[pairs[i].winner]);
        printf("loser: %s\n", candidates[pairs[i].loser]);

        make_cycle(pairs[i].winner, pairs[i].loser, i);

        if (cycle == false)
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }

    return;
}

// 3.abc 2.bca 4.cab

void make_cycle(int winner, int loser, int n)
{
    back = 0;

    if (locked[loser][winner] == true)
    {
        cycle = true;
        back = 1;
        return;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[loser][i] == true)
        {
            make_cycle(winner, i, n);
        }

        if (back == 1)
        {
            return;
        }
    }


    if (loser == pairs[n].loser)
    {
        cycle = false;
        return;
    }
}

void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        int arrows = 0;    // The amount of arrows pointing towards candidate i

        for (int j = 0; j < candidate_count; j++)
        {
            // Update arrow count if there is an arrow pointed from candidate j to candidate i
            if (locked[j][i] == true)
            {
                arrows++;
                // break;
            }
        }

        // If there are no arrows pointing towards candidate i;
        // he/she is the source of the graph, or in other words the winner of the election
        if (arrows == 0)
        {
            printf("%s\n", candidates[i]);   // Print winner
            return;
        }
    }

}