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

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
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

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // Set preference for candidate in rank i over candidates in rank j>i
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i wget https://cdn.cs50.net/2021/fall/labs/4/volume.zip+ 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;  // Preference of a voter of candidate i over j
        }
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++) // Start iterating at j=i+1 to avoid recording the same pair twice
        {
            // In case candidate i is preferred over candidate j
            if (preferences[i][j] > preferences[j][i])
            {
                pair_count++;
                pairs[pair_count - 1].winner = i;
                pairs[pair_count - 1].loser = j;
            }

            // In case candidate j is preferred over candidate i
            if (preferences[j][i] > preferences[i][j])
            {
                pair_count++;
                pairs[pair_count - 1].winner = j;
                pairs[pair_count - 1].loser = i;
            }
        }
    }

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int swapcount;  // Variable to check whether the array is fully sorted

    // Bubble sort algorithm
    do
    {
        swapcount = 0;  // Reset swapcount to zero

        // Go through every pair once (except the last one)
        // Swap if strength of victory of the next pair is smaller
        for (int i = 0; i < pair_count - 1; i++)
        {
            int winner1 = pairs[i].winner;
            int loser1 = pairs[i].loser;
            int winner2 = pairs[i + 1].winner;
            int loser2 = pairs[i + 1].loser;

            int dif1 = preferences[winner1][loser1] - preferences[loser1][winner1]; // Strenght of victory of candidate in pair i
            int dif2 = preferences[winner2][loser2] - preferences[loser2][winner2]; // Strenght of victory of candidate in pair i+1

            // In case the strength of victory of pair i is less than that of pair i+1
            if (dif1 < dif2)
            {
                pairs[i].winner = winner2;
                pairs[i].loser = loser2;
                pairs[i + 1].winner = winner1;
                pairs[i + 1].loser = loser1;
                swapcount++;    // Update swapcount to indicate that a swap has occured
            }
        }
    }
    while (swapcount > 0);   // Array is fully sorted if swapcount = 0, which means no swaps occured

    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        make_cycle(pairs[i].winner, pairs[i].loser, i);

        // If the new edge does not create a cycle, lock the pair
        if (cycle == false)
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }

    return;
}

// Check if edge creates a cycle
void make_cycle(int winner, int loser, int n)
{
    back = 0;   // Continue the function as long as back=0

    // The edge creates a cycle if this condition is true
    if (locked[loser][winner] == true)
    {
        cycle = true;
        back = 1;   // Set back to 1 to exit out of the function
        return;
    }

    // Check if there is a previous locked pair which has the candidate as winner
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

    // Arrive here when the previous loop is completed without finding a cycle
    if (loser == pairs[n].loser)
    {
        cycle = false;
        return;
    }
}

// Print the winner of the election
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