 // Github CS50 Token: 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

#include <cs50.h>
#include <stdio.h>
#include <string.h>


// Max number of candidates ---------- limiter
#define MAX 9

// preferences[i][j] is NUMBER of voters who prefer i over j -- graph of preferred candidates (i) over other candidates (j)
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j ---- refers to a pair in the preferences graph
bool locked[MAX][MAX];

// Each pair has a winner, loser ------- this struct contains the index number for two candidates from candidates array
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX]; // array of candidate names --- will be passed the argv values
pair pairs[MAX * (MAX - 1) / 2]; // an array of all possible pairings of candidates

int pair_count;
int candidate_count; // stores number less than max candidates

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

bool is_circle(int winner, int loser); // additional program to trace the graph for loops


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
    // Clear graph of locked in pairs ---- runs through each i value x each j value and sets locked[i][j] to false
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }
    pair_count = 0;
    int voter_count = get_int("Number of voters: ");
    // Query for votes --------------------------------------------   "for each voter"
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count]; // ------------------- create array of ranks (e.g. 1st-9th preferences for that voter)
        // Query for each rank // ----------------------------------------- "for each rank"
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1); // ----------------- input candidate name

            if (vote(j, name, ranks) == false)
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }
        record_preferences(ranks);
        //takes current voter's array of ranked candidate names (ranks array), compares the relationships between
        printf("\n");
    }
    // AFTER each voter's ranking of each candidates is stored, and the number of preferences calculated and stored...
    add_pairs(); // populate the pairs array (remember pairs is a struct containing int winner & int loser) with each pairing of NET positive preference
    sort_pairs(); // SORT the pairs array into order, with most preferred first
    lock_pairs(); // one by one, starting at the first element of the sorted pairs array, update bool LOCKED[i][j] to lock in each pair UNLESS it causes a loop
    print_winner(); // identify the source of the graph and print name
    return 0;
}


// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i; // ------inputs to ranks array the index number of the chosen candidate from candidates array
            return true;
        }
    }
    return false;
}


//-----------------------takes current voter's array of ranked candidate indexes (ranks array), compares the relationships between
//-----------------------candidates and inputs data to the preferences graph (which NUMERATES the number of voters with each preference)
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (i < j) // --------------------- if one rank is above the other
            {
                preferences[ranks[i]][ranks[j]] += 1;
                //inputs the index of two candidates as found in the candidates array, via the candidate indexes stored in ranks array
            }
        }
    }
    return;
}


// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
    return;
}


// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int noswap = 0;
    int counter = ((candidate_count * (candidate_count - 1) / 2));
    pair sorted_pairs[candidate_count * (candidate_count - 1) / 2];
    while (noswap < counter)
    {
        for (int i = 0; i < counter; i++)
        {
            if (preferences[pairs[i].winner][pairs[i].loser] < preferences[pairs[i + 1].winner][pairs[i + 1].loser])
            {
                sorted_pairs[i] = pairs[i + 1];
                sorted_pairs[i + 1] = pairs[i];
                pairs[i] = sorted_pairs[i];
                pairs[i + 1] = sorted_pairs[i + 1];
                noswap = 0;
            }
            else
            {
                noswap++;
            }
        }
    }
    return;
}


bool is_circle(int winner, int loser)
{
    if (locked[loser][winner] == true)
    {
        return true;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[i][winner])
        {
            return is_circle(i, loser);
        }
    }
    return false;
}

void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        if (!is_circle(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}



// Print the winner of the election -----------------
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (!locked[j][i])
            {
                printf("%s\n", candidates[i]);
            }
        }
    }
}




