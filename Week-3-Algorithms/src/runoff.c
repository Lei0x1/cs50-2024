#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <limits.h> // INT_MAX

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
} candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    // TODO
    // Iterate through the candidate names to find a match
    for (int i = 0; i < candidate_count; i++) {
        // Compare the candidate's name with the given name
        if (strcmp(candidates[i].name, name) == 0) {
            // Update preferences array to indicate the rank preference
            // Return true if preference is successfully recorded, false otherwise
            preferences[voter][rank] = i;
            return true;
        }
    }
    // No candidate found
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    // TODO
    // Update the number of votes each candidate has at this stage in the runoff

    // Iterate through each voter
    for (int i = 0; i < voter_count; i++) {
        // Get the voter's top-preferred candidate
        int top_choice = preferences[i][0];

        // Check if the top choice candidate is eliminated
        if (!candidates[top_choice].eliminated) {
            // Update the votes for the top choice candidate if not eliminated
            candidates[top_choice].votes++;
        }
        else {
            // If the top choice candidate is eliminated, find the next non-eliminated candidate in the voter's preferences
            for (int j = 0; j < candidate_count; j++) {
                int next_choice = preferences[i][j];
                if (!candidates[next_choice].eliminated) {
                    // Update the votes for the next non-eliminated candidate and break the loop
                    candidates[next_choice].votes++;
                    break; // Stop after updating the vote for the next choice
                }
            }
        }
    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    // Calculate the number of votes needed to win: half of the voters + 1
    int votes_needed = voter_count / 2;

    // Iterate through each candidate
    for (int i = 0; i < candidate_count; i++) {
        // Check if the candidate has more than half of the votes
        if (candidates[i].votes > votes_needed) {
            // Print the winner's name
            printf("Winner of the election is %s\n", candidates[i].name);
            // Return true indicating a winner is found
            return true;
        }
    }
    // Return false indicating no winner found
    return false;
}


// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    // TODO
    // Find the minimum vote total for any candidate still in the election

    int min_votes = INT_MAX; // Initialize a variable to store the minimum vote total, starting with a large value

    // Iterate through each candidate
    for (int i = 0; i < candidate_count; i++) {
        // Check if the candidate is still in the election and has fewer votes than the current minimum
        if (!candidates[i].eliminated && candidates[i].votes < min_votes) {
            // Update the minimum vote total if a candidate meets the criteria
            min_votes = candidates[i].votes;
        }
    }
    // Return the minimum vote total
    return min_votes;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    // TODO
    // Iterate through each candidate
    for (int i = 0; i < candidate_count; i++) {
        // Check if the candidate is still in the election and has votes different from the minimum
        if (!candidates[i].eliminated && candidates[i].votes != min) {
            // If any candidate has votes different from the minimum, return false (no tie)
            return false;
        }
    }
    // If all candidates have the same votes as the minimum, return true (tie)
    return true;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    // TODO
    // Iterate through each candidate
    for (int i = 0; i < candidate_count; i++) {
        // Check if the candidate is still in the election and has votes equal to the minimum
        if (!candidates[i].eliminated && candidates[i].votes == min) {
            // Eliminate the candidate by setting their eliminated flag to true
            candidates[i].eliminated = true;
        }
    }
    return;
}