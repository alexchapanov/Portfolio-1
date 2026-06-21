
#include <iostream>
#include <vector>
#include <string>
#include <cctype>

using namespace std;

struct Guard
{
    int row;
    int col;
    char direction;
};

string ToLower(string text)
{
    for (char& c : text)
        c = tolower(c);

    return text;
}

bool IsGuard(char c)
{
    return c == '<' || c == '>' || c == '^' || c == 'v';
}

void DisplayLevel(const vector<string>& level)
{
    for (const string& row : level)
        cout << row << endl;
}

void FindPlayer(const vector<string>& level, int& row, int& col)
{
    for (int r = 0; r < level.size(); r++)
    {
        for (int c = 0; c < level[r].size(); c++)
        {
            if (level[r][c] == '@')
            {
                row = r;
                col = c;
                return;
            }
        }
    }
}

vector<Guard> FindGuards(const vector<string>& level)
{
    vector<Guard> guards;

    for (int r = 0; r < level.size(); r++)
    {
        for (int c = 0; c < level[r].size(); c++)
        {
            if (IsGuard(level[r][c]))
            {
                Guard g;
                g.row = r;
                g.col = c;
                g.direction = level[r][c];
                guards.push_back(g);
            }
        }
    }

    return guards;
}

bool PlayerSeen(const vector<string>& level)
{
    for (int r = 0; r < level.size(); r++)
    {
        for (int c = 0; c < level[r].size(); c++)
        {
            char tile = level[r][c];

            if (!IsGuard(tile))
                continue;

            if (tile == '>')
            {
                for (int x = c + 1; x < level[r].size(); x++)
                {
                    if (level[r][x] == '@')
                        return true;

                    if (level[r][x] != ' ')
                        break;
                }
            }

            if (tile == '<')
            {
                for (int x = c - 1; x >= 0; x--)
                {
                    if (level[r][x] == '@')
                        return true;

                    if (level[r][x] != ' ')
                        break;
                }
            }

            if (tile == '^')
            {
                for (int y = r - 1; y >= 0; y--)
                {
                    if (level[y][c] == '@')
                        return true;

                    if (level[y][c] != ' ')
                        break;
                }
            }

            if (tile == 'v')
            {
                for (int y = r + 1; y < level.size(); y++)
                {
                    if (level[y][c] == '@')
                        return true;

                    if (level[y][c] != ' ')
                        break;
                }
            }
        }
    }

    return false;
}

void MoveGuards(vector<string>& level)
{
    vector<Guard> guards = FindGuards(level);

    for (Guard& g : guards)
    {
        int newRow = g.row;
        int newCol = g.col;

        switch (g.direction)
        {
        case '>': newCol++; break;
        case '<': newCol--; break;
        case '^': newRow--; break;
        case 'v': newRow++; break;
        }

        bool blocked =
            level[newRow][newCol] != ' ' &&
            level[newRow][newCol] != '@';

        if (blocked)
        {
            switch (g.direction)
            {
            case '>': g.direction = '<'; break;
            case '<': g.direction = '>'; break;
            case '^': g.direction = 'v'; break;
            case 'v': g.direction = '^'; break;
            }

            newRow = g.row;
            newCol = g.col;

            switch (g.direction)
            {
            case '>': newCol++; break;
            case '<': newCol--; break;
            case '^': newRow--; break;
            case 'v': newRow++; break;
            }
        }

        if (level[newRow][newCol] == '@')
            continue;

        level[g.row][g.col] = ' ';
        level[newRow][newCol] = g.direction;
    }
}

void PlayLevel(vector<string> level)
{
    while (true)
    {
        int playerRow, playerCol;
        FindPlayer(level, playerRow, playerCol);

        DisplayLevel(level);

        if (PlayerSeen(level))
        {
            cout << "\nMission Failed! You were spotted!\n";
            return;
        }

        cout << "\nMove (W/A/S/D): ";

        string input;
        getline(cin, input);

        input = ToLower(input);

        if (input.empty())
            continue;

        int newRow = playerRow;
        int newCol = playerCol;

        switch (input[0])
        {
        case 'w': newRow--; break;
        case 's': newRow++; break;
        case 'a': newCol--; break;
        case 'd': newCol++; break;
        default: continue;
        }

        char destination = level[newRow][newCol];

        if (destination == '#')
            continue;

        if (IsGuard(destination))
        {
            cout << "\nMission Failed!\n";
            return;
        }

        if (destination == '$')
        {
            cout << "\nMission Complete!\n";
            return;
        }

        level[playerRow][playerCol] = ' ';
        level[newRow][newCol] = '@';

        MoveGuards(level);

        if (PlayerSeen(level))
        {
            DisplayLevel(level);
            cout << "\nMission Failed! You were spotted!\n";
            return;
        }
    }
}

int main()
{
    vector<string> level1 =
    {
        "########",
        "#@     #",
        "# ###  #",
        "#   >$ #",
        "# ###  #",
        "#      #",
        "########"
    };

    vector<string> level2 =
    {
        "###########",
        "#@        #",
        "# ######  #",
        "#      v$ #",
        "# ######  #",
        "#         #",
        "###########"
    };

    cout << "Welcome to Ultra-Spy!\n\n";
    cout << "1) A New Enemy\n";
    cout << "2) The Truth of the Weapon\n\n";

    string choice;
    getline(cin, choice);

    choice = ToLower(choice);

    if (choice == "1" || choice == "a new enemy")
        PlayLevel(level1);
    else if (choice == "2" || choice == "the truth of the weapon")
        PlayLevel(level2);
    else
        cout << "Invalid selection.\n";

    return 0;
}

