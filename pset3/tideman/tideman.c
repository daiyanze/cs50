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

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

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
    // TODO
    int i = 0;
    while (i < candidate_count)
    {
        if (!strcmp(candidates[i], name))
        {
            ranks[rank] = i;
            return true;
        }
        i++;
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    // Record the preferecens by a multi-dimensional array:
    // preferences[0][1] : 0 -> 1 (prefer 0 to 1)
    // preferences[0][1] = 3 : set strenth of "prefer 0 to 1" to 3
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i; j < candidate_count; j++)
        {
            if (ranks[i] != ranks[j])
            {
                preferences[ranks[i]][ranks[j]]++;
            }
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            // Check which preference is stronger: i->j or j->i
            // Skip if strenth is the same: i->j == j->i
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    // Bubble sort the pairs
    pair temp_pair[1];
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = 0; j < candidate_count - i - 1; j++)
        {
            // Check preferences[winner][loser] strentch and compare with its next neighbor
            // Switch the position if the current is stronger than its next neighbor
            if (preferences[pairs[i].winner][pairs[i].loser] > preferences[pairs[j].winner][pairs[j].loser])
            {
                temp_pair[0] = pairs[i];
                pairs[i].winner = pairs[j].winner;
                pairs[i].loser = pairs[j].loser;
                pairs[j].winner = temp_pair[0].winner;
                pairs[j].loser = temp_pair[0].loser;
            }
        }
    }
    return;
}

// For example, we have "pair chain":
// 0 -> 1
// 1 -> 0 doesn't exist, so we move onto the loop.
// Now we need to check if the winner 0 is also a loser.
// For instance 2 -> 0.
// If 2 -> 0 pair exists, then the chain would become:
// 2 -> 0 -> 1
// Then we need to know if 1 -> 2 exists with another round of recursion
// We reverse the locked array index: locked[2][1] to locked[1][2].
// 1 -> 2 doesn't exist (locked[1][2] is false), continue the loop to check if 2 is also a loser.
// ...
// After another loop, 2 is loser to 3.
// Now the "pair chain" is like this:
// 3 -> 2 -> 0 -> 1
// If 1 defeats 3, then there is a circle.
// This algorithm is to treat the winner as a loser and check if any of its winner is defeated by the initial loser.
bool is_circle(int winner, int loser)
{
    // If [loser] -> [winner] pair exists, then there must be a circle
    if (locked[loser][winner])
    {
        return true;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        // Check if winner is also a loser in the pair, if so continue to search if the next pair has circle
        if (locked[i][winner] == true)
        {
            return is_circle(i, loser);
        }
    }

    return false;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    for (int i = 0; i < pair_count; i++)
    {
        // Lock pair only when the current winner / loser does not form a circle
        if (!is_circle(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    // Assume the first pair with biggest strenth is the winner pair;
    for (int i = 0; i < candidate_count; i++)
    {
        int tally = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == true)
            {
                tally++;
                break;
            }
        }

        if (tally == 0)
        {
            printf("%s\n", candidates[i]);
        }
        else
        {
            continue;
        }
    }

    return;
}