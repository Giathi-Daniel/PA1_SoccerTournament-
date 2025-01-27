#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct player
{
    char *name; // dynamic allocation
    int totalGoals;
} Player;

typedef struct Team
{
    char *name;
    Player **players; // Array of player pointers
    int playerCount;  // Number of players
} Team;

typedef struct Goal
{
    Player *scorer;
    int time;
} Goal;

typedef struct Match
{
    Team *team1; // Pointer of Team 1
    Team *team2;
    Goal *goals;
    int goalCount;
} Match;

typedef struct MatchWinnerTeam
{
    Team *winnerTeam; // pointer to winner team
    int isDraw;
} MatchWinnerTeam;

// Prototypes
Player *createPlayer(char *name);
Team *createTeam(char *name, int playerCount);
Player *getPlayerByName(char *name, Player **playerList, int numPlayers);
Team *getTeamByTeamName(Team **teamList, int numTeams, char *tname);
void recordGoal(Match *match, Player *player, int time);
int printHatTrickPlayers(Match *match);
MatchWinnerTeam *getWinningTeam(Match *matches, int matchCount, char *team1Name, char *team2Name);
Team *getChampion(Team **teamList, int teamCount, Match *matchList, int matchCount);

Player *createPlayer(char *name)
{
    // memory allocation for the Player struct
    Player *newPlayer = (Player *)malloc(sizeof(Player));
    if (newPlayer == NULL)
    {
        fprintf(stderr, "Memory allocation failed for Player.\n");
        exit(1);
    }

    // Players' name memory allocation
    newPlayer->name = (char *)malloc((strlen(name) + 1) * sizeof(char));
    if (newPlayer->name == NULL)
    {
        fprintf(stderr, "Memory allocation failed for Player name.\n");
        exit(1);
    }
    strcpy(newPlayer->name, name);

    newPlayer->totalGoals = 0;

    return newPlayer;
}

Team *createTeam(char *name, int playerCount)
{
    // memory allocation for the Player struct
    Team *newTeam = (Team *)malloc(sizeof(Team));
    if (newTeam == NULL)
    {
        fprintf(stderr, "Memory allocation failed for Team.\n");
        exit(1);
    }

    // Teams' name memory allocation
    newTeam->name = (char *)malloc((strlen(name) + 1) * sizeof(char));
    if (newTeam->name == NULL)
    {
        fprintf(stderr, "Memory allocation failed for Team name.\n");
        exit(1);
    }
    strcpy(newTeam->name, name);

    newTeam->players = (Player **)malloc(playerCount * sizeof(Player *));
    if (newTeam->players == NULL)
    {
        fprintf(stderr, "Memory allocation failed for players array.\n");
        exit(1);
    }

    newTeam->playerCount = playerCount;

    return newTeam;
}

Player *getPlayerByName(char *name, Player **playerList, int numPlayers)
{
    int i;
    for (i = 0; i < numPlayers; i++)
    {
        if (strcmp(playerList[i]->name, name) == 0)
        { // compare names
            return playerList[i];
        }
    }

    return NULL; // PLAYER NOT FOUND
}

Team *getTeamByTeamName(Team **teamList, int numTeams, char *tname)
{
    int i;
    for (i = 0; i < numTeams; i++)
    {
        if (strcmp(teamList[i]->name, tname) == 0)
        {
            return teamList[i];
        }
    }
    return NULL;
}

void recordGoal(Match *match, Player *player, int time)
{
    // Increase the size of the goals array dynamically
    match->goals = (Goal *)realloc(match->goals, (match->goalCount + 1) * sizeof(Goal));
    if (match->goals == NULL)
    {
        fprintf(stderr, "Memory reallocation failed for goals array.\n");
        exit(1);
    }

    // add new goal to the array
    match->goals[match->goalCount].scorer = player;
    match->goals[match->goalCount].time = time;

    match->goalCount++;

    player->totalGoals;
}

int printHatTrickPlayers(Match *match)
{
    int foundHatTrick = 0;

    // temporary memory allocation for tracking scorers and their goal counts
    Player **scorers = (Player **)calloc(match->goalCount, sizeof(Player *));
    int *goalCounts = (int *)calloc(match->goalCount, sizeof(int));
    int scorerCount = 0;

    if (scorers == NULL || goalCounts == NULL)
    {
        fprintf(stderr, "Memory allocation failed for hat trick processing.\n");
        exit(1);
    }

    // Counting goals for each player
    int i, j;
    for (i = 0; i < match->goalCount; i++)
    {
        Player *scorer = match->goals[i].scorer;

        // Checking if the scorer is already in the scorers list
        int index = -1;
        for (j = 0; j < scorerCount; j++)
        {
            if (scorers[j] == scorer)
            {
                index = j;
                break;
            }
        }

        if (index == -1)
        {
            // New scorer, add to the list
            scorers[scorerCount] = scorer;
            goalCounts[scorerCount] = 1;
            scorerCount++;
        }
        else
        {
            goalCounts[index]++;
        }
    }

    // Print players who achieved a hat trick
    int k;
    for (k = 0; k < scorerCount; k++)
    {
        if (goalCounts[k] >= 3)
        {
            if (!foundHatTrick)
            {
                printf("Hat trick found in %s vs %s:\n", match->team1->name, match->team2->name);
                foundHatTrick = 1;
            }
            printf("%s %d\n", scorers[k]->name, scorers[k]->totalGoals);
        }
    }

    if (!foundHatTrick)
    {
        printf("no hat trick found in the tournament\n");
    }

    // Free allocated memory
    free(scorers);
    free(goalCounts);

    return foundHatTrick;
}

MatchWinnerTeam *getWinningTeam(Match *matches, int matchCount, char *team1Name, char *team2Name)
{
    // Allocate memory for MatchWinnerTeam struct
    MatchWinnerTeam *result = (MatchWinnerTeam *)malloc(sizeof(MatchWinnerTeam));
    if (result == NULL)
    {
        fprintf(stderr, "Memory allocation failed for MatchWinnerTeam.\n");
        exit(1);
    }

    result->winnerTeam = NULL;
    result->isDraw = 0;

    // Search for the match between the given teams
    int i;
    for (i = 0; i < matchCount; i++)
    {
        Match *match = &matches[i];

        if ((strcmp(match->team1->name, team1Name) == 0) &&
            (strcmp(match->team2->name, team2Name) == 0))
        {

            // Count goals for each team
            int team1Goals = 0, team2Goals = 0;
            int j;
            for (j = 0; j < match->goalCount; j++)
            {
                if (match->goals[j].scorer != NULL)
                {
                    if (match->goals[j].scorer >= match->team1->players[0] &&
                        match->goals[j].scorer <= match->team1->players[match->team1->playerCount - 1])
                    {
                        team1Goals++;
                    }
                    else
                    {
                        team2Goals++;
                    }
                }
            }

            // Determine the winner or if it's a draw
            if (team1Goals > team2Goals)
            {
                result->winnerTeam = match->team1;
            }
            else if (team2Goals > team1Goals)
            {
                result->winnerTeam = match->team2;
            }
            else
            {
                result->isDraw = 1;
            }

            return result;
        }
    }

    // No match found
    return result;
}

Team *getChampion(Team **teamList, int teamCount, Match *matchList, int matchCount)
{
    int *points = (int *)calloc(teamCount, sizeof(int));
    if (points == NULL)
    {
        fprintf(stderr, "Memory allocation failed for points array.\n");
        exit(1);
    }

    // Process each match to update points
    int i;
    for (i = 0; i < matchCount; i++)
    {
        Match *match = &matchList[i];
        MatchWinnerTeam *result = getWinningTeam(matchList, matchCount, match->team1->name, match->team2->name);

        if (result->winnerTeam != NULL)
        {
            int j;
            for (j = 0; j < teamCount; j++)
            {
                if (teamList[j] == result->winnerTeam)
                {
                    points[j] += 3; // Winner gets 3 points
                    break;
                }
            }
        }
        else if (result->isDraw)
        {
            int j;
            for (j = 0; j < teamCount; j++)
            {
                if (teamList[j] == match->team1 || teamList[j] == match->team2)
                {
                    points[j] += 1; // Both teams get 1 point
                }
            }
        }

        free(result); // Free memory for the result
    }

    // Find the team with the maximum points
    int maxPoints = -1;
    Team *champion = NULL;

    int k;
    for (k = 0; k < teamCount; k++)
    {
        if (points[k] > maxPoints)
        {
            maxPoints = points[k];
            champion = teamList[k];
        }
    }

    free(points); // Free memory for points array
    return champion;
}

// FREE ALL DYNAMICALLY MEMORY TO PREVENT MEMORY LEAKS
void freePlayer(Player *player)
{
    if (player != NULL)
    {
        free(player->name);
        free(player);
    }
}

void freeTeam(Team *team)
{
    if (team != NULL)
    {
        free(team->name);
        int i;
        for (i = 0; i < team->playerCount; i++)
        {
            freePlayer(team->players[i]);
        }
        free(team->players);
        free(team);
    }
}

void freeMatch(Match *match)
{
    if (match != NULL)
    {
        free(match->goals);
        free(match);
    }
}

void freeAll(Team **teamList, int teamCount, Match *matchList, int matchCount)
{
    int i;
    for (i = 0; i < teamCount; i++)
    {
        freeTeam(teamList[i]);
    }
    free(teamList);

    int j;
    for (j = 0; j < matchCount; j++)
    {
        freeMatch(&matchList[j]); // Free each match
    }
    free(matchList);
}

int main()
{
    int t, m, c; // Number of teams, matches, and commands
    scanf("%d %d %d", &t, &m, &c);

    // Dynamically allocate memory for the team list
    Team **teamList = (Team **)malloc(t * sizeof(Team *));
    if (teamList == NULL)
    {
        fprintf(stderr, "Memory allocation failed for team list.\n");
        exit(1);
    }

    // Read team data
    int i; // Declare i outside the loop
    for (i = 0; i < t; i++)
    {
        char teamName[21]; // At most 20 characters
        int playerCount;

        scanf("%s %d", teamName, &playerCount);
        teamList[i] = createTeam(teamName, playerCount);

        // Dynamically allocate and populate players for this team
        int j; // Declare j outside the loop
        for (j = 0; j < playerCount; j++)
        {
            char playerName[21];
            scanf("%s", playerName);

            Player *player = createPlayer(playerName);
            teamList[i]->players[j] = player; // Assign player to team
        }
    }

    // Dynamically allocate memory for the match list
    Match *matchList = (Match *)malloc(m * sizeof(Match));
    if (matchList == NULL)
    {
        fprintf(stderr, "Memory allocation failed for match list.\n");
        exit(1);
    }

    // Read match data
    int k;
    for (k = 0; k < m; k++) // Declare j outside the loop
    {
        char team1Name[21], team2Name[21];
        scanf("%s %s", team1Name, team2Name);

        Team *team1 = getTeamByTeamName(teamList, t, team1Name);
        Team *team2 = getTeamByTeamName(teamList, t, team2Name);

        matchList[k].team1 = team1;
        matchList[k].team2 = team2;
        matchList[k].goals = NULL; // Initialize the goals array to NULL
        matchList[k].goalCount = 0;

        // Read goals for this match
        while (1)
        {
            char playerName[21];
            int time;
            scanf("%s", playerName);

            if (strcmp(playerName, "end") == 0)
            {
                break; // End of goals for this match
            }

            scanf("%d", &time);

            Player *scorer = getPlayerByName(playerName, team1->players, team1->playerCount);
            if (scorer == NULL)
            {
                scorer = getPlayerByName(playerName, team2->players, team2->playerCount);
            }

            recordGoal(&matchList[k], scorer, time); // Use j instead of i
        }
    }

    // Process commands
    int l; // Declare l outside the loop
    for (l = 0; l < c; l++)
    {
        char command[21];
        scanf("%s", command);

        if (strcmp(command, "max_goal_player") == 0)
        {
            Player *maxPlayer = NULL;
            int maxGoals = -1;

            int m;
            for (m = 0; m < t; m++)
            {
                int n;
                for (n = 0; n < teamList[m]->playerCount; n++)
                {
                    if (teamList[m]->players[n]->totalGoals > maxGoals)
                    {
                        maxGoals = teamList[m]->players[n]->totalGoals;
                        maxPlayer = teamList[m]->players[n];
                    }
                }
            }

            if (maxPlayer != NULL)
            {
                printf("player with max goal %s %d\n", maxPlayer->name, maxGoals);
            }
        }
        else if (strcmp(command, "winner") == 0)
        {
            char team1Name[21], team2Name[21];
            scanf("%s %s", team1Name, team2Name);

            MatchWinnerTeam *result = getWinningTeam(matchList, m, team1Name, team2Name);
            if (result->winnerTeam != NULL)
            {
                printf("winner of %s vs %s is %s\n", team1Name, team2Name, result->winnerTeam->name);
            }
            else if (result->isDraw)
            {
                printf("%s vs %s match drawn\n", team1Name, team2Name);
            }
            else
            {
                printf("no match between %s and %s\n", team1Name, team2Name);
            }

            free(result);
        }
        else if (strcmp(command, "hattrick") == 0)
        {
            int hatTrickFound = 0;
            int j;
            for (j = 0; j < m; j++) // Declare j outside the loop
            {
                if (printHatTrickPlayers(&matchList[j]))
                {
                    hatTrickFound = 1;
                }
            }

            if (!hatTrickFound)
            {
                printf("no hat trick found in the tournament\n");
            }
        }
        else if (strcmp(command, "champion") == 0)
        {
            Team *champion = getChampion(teamList, t, matchList, m);
            if (champion != NULL)
            {
                printf("champion is %s\n", champion->name);
            }
        }
    }

    // Free all dynamically allocated memory
    freeAll(teamList, t, matchList, m);

    return 0;
}
