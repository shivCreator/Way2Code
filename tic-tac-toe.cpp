#include <iostream>

using namespace std;

// utility function to print 40 empty lines by default
void clrscr(int n = 40)
{
    for(int i = 0; i < n; i++)
        cout << endl;
}

void print(char* map)
{
    cout << "Player 1 : X | Player 2 : O" << endl;
    cout << map[0] << " | " << map[1] << " | " << map[2] << endl;
    cout << "_________" << endl;
    cout << map[3] << " | " << map[4] << " | " << map[5] << endl;
    cout << "_________" << endl;
    cout << map[6] << " | " << map[7] << " | " << map[8] << endl;
}

int check(char* map)
{
    short winner = 0;
    
    // horizontals
    if(map[0] == map[1] && map[1] == map[2] && map[0] != ' ')
        winner = (map[0] == 'X') ? 1 : 2;
    else if(map[3] == map[4] && map[4] == map[5] && map[3] != ' ')
        winner = (map[3] == 'X') ? 1 : 2;
    else if(map[6] == map[7] && map[7] == map[8] && map[6] != ' ')
        winner = (map[6] == 'X') ? 1 : 2;
    
    // verticals
    else if(map[0] == map[3] && map[3] == map[6] && map[0] != ' ')
        winner = (map[0] == 'X') ? 1 : 2;
    else if(map[1] == map[4] && map[4] == map[7] && map[1] != ' ')
        winner = (map[1] == 'X') ? 1 : 2;
    else if(map[2] == map[5] && map[5] == map[8] && map[2] != ' ')
        winner = (map[2] == 'X') ? 1 : 2;
        
    // diagonals
    else if(map[0] == map[4] && map[4] == map[8] && map[0] != ' ')
        winner = (map[0] == 'X') ? 1 : 2;
    else if(map[2] == map[4] && map[4] == map[6] && map[2] != ' ')
        winner = (map[2] == 'X') ? 1 : 2;
    else 
        winner = 0;
    
    return winner;
}

int main()
{
    char map[] = "         ";
    //           |123456789|
    short turn = 1;
    short pos;
    short status;
    
    for(int i = 0; i < 9; i++)
    {
        clrscr();
        game:
            print(map);
            cout << "Player " << turn << " : ";
            cin >> pos;
            
            --pos;
            
            if(map[pos] == 'X' || map[pos] == 'O')
            {
                clrscr();
                cout << "Invalid Input" << endl;
                goto game;
            }
            else
                map[pos] = (turn == 1) ? 'X' : 'O';
            
            status = check(map);
            
            if(status != 0)
            {
                break;
            }
            
            turn = (turn == 1) ? 2 : 1;
    }
    
    clrscr();
    cout << "Game Over!";
    print(map);
    
    if(status == 0)
        cout << "It's a Draw!";
    else if(status == 1)
        cout << "Player 1 won!";
    else 
        cout << "Player 2 won!";
    
    return 0;
}
