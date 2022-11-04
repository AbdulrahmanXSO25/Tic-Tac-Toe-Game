#include <iostream>
#include <string>
using namespace std;

// implement the stack data structure
class stack {

public:
    int Top = -1;
    char a[9] = {};
    bool push(char x);
    void pop();
    char top();
    bool isEmpty();
};

bool stack::push(char x)
{
    if (Top >= (9 - 1)) {
        return false;
    }
    else {
        a[++Top] = x;
        return true;
    }
}

void stack::pop()
{
    if (Top >= 0) {
        Top--;
    }
}

char stack::top()
{
    return a[Top];
}

bool stack::isEmpty()
{
    if (Top < 0) return true;
    else return false;
}


//implement a customized stacks for saving last positions to use it if the player will make undo
class lastPosStack {
public:
    int Top = -1;
    int Xs[9] = {};
    int Ys[9] = {};
    void push(int x, int y);
    int topX();
    int topY();
    void pop();
};

void lastPosStack::push(int x, int y)
{
    Xs[++Top] = x;
    Ys[Top] = y;
}

int lastPosStack::topX()
{
    return Xs[Top];
}

int lastPosStack::topY()
{
    return Ys[Top];
}

void lastPosStack::pop()
{
    --Top;
}

// two customized stacks, one for each player to save his last positions
lastPosStack P1, P2;

// an int to check the winner
//2: O win
//-2: x win
//0: tie
//1: no winner
int chekWinnerInt;

// two string variables to save the players'names
string p1, p2;

//declare 4 stacks for the two players //each player has a main stack and another stack to save undo
stack user1stack, user2stack, user1undo, user2undo;

// the function that draw the screen of the game
void draw(char arr[3][3])
{
    cout << endl;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (j != 2)
            {
                cout << arr[i][j] << " | ";
            }
            else cout << arr[i][j];
        }
        cout << endl;
    }
    cout << endl;
}

// the function that transfer x/o from the player's stack to show it in the array
void swap(char c, int x, int y, char arr[3][3])
{
    arr[x][y] = c;
}

// a function to check the poistion if it's invalid (=0,1,2) or not
bool chekPosition(int x, int y)
{
    if (x >= 0 && x <= 2 && y >= 0 && y <= 2) return true;
    else return false;
}

// a function to check the poistion if its free of 'X' or 'O' or not
bool chekPosition(char arr[3][3], int x, int y)
{
    if (arr[x][y] == '-') return true;
    else return false;
}

// the first function of three main functions // it make player select the position for putting x or o
void cellPoition(char arr[3][3], string cu, string p1)
{
    char c;
    if (cu == p1)
    {
        user1stack.push('O');
        c = user1stack.top();
    }
    else
    {
        user2stack.push('X');
        c = user2stack.top();
    }
    int x, y;
    bool poisition_taken = true;
    while (poisition_taken)
    {
        if (cu == p1)
        {
            cout << "\"" << cu << "\" please enter row and col to put 'O':";
        }
        else
        {
            cout << "\"" << cu << "\" please enter row and col to put 'X':";
        }
        cin >> x >> y;
        while (!chekPosition(x, y))
        {
            cout << "Invalid Poistion \n";
            if (cu == p1)
            {
                cout << "\"" << cu << "\" please enter row and col to put 'O':";
            }
            else
            {
                cout << "\"" << cu << "\" please enter row and col to put 'X':";
            }
            cin >> x >> y;
            chekPosition(x, y);
        }
        if (cu == p1)
        {
            P1.push(x, y);
        }
        else
        {
            P2.push(x, y);
        }
        if (arr[x][y] != 'X' && arr[x][y] != 'O')
        {
            swap(c, x, y, arr);
            poisition_taken = false;
        }
        else
        {
            cout << "The Position is Taken \n";
            poisition_taken = true;
        }
    }
}

// the second function for undo
void undo(char arr[3][3], string cu, string p1)
{
    if (cu == p1)
    {
        if (user1stack.isEmpty())
        {
            cout << "There is no movements yet! \n";
            cellPoition(arr, cu, p1);
        }
        else
        {
            char c = user1stack.top();
            user1undo.push(c);
            user1stack.pop();
            arr[P1.topX()][P1.topY()] = '-';
        }
    }
    else
    {
        if (user2stack.isEmpty())
        {
            cout << "There is no movements yet! \n";
            cellPoition(arr, cu, p1);
        }
        else
        {
            char c = user2stack.top();
            user2undo.push(c);
            user2stack.pop();
            arr[P2.topX()][P2.topY()] = '-';
        }
    }
}

// the third function for redo
void redo(char arr[3][3], string cu, string p1)
{
    char c;
    if (cu == p1)
    {
        if (user1undo.isEmpty())
        {
            cout << "you didn't undo a position yet \n";
            cellPoition(arr, cu, p1);
        }
        else
        {
            if (chekPosition(arr, P1.topX(), P1.topY()))
            {
                c = user1undo.top();
                swap(c, P1.topX(), P1.topY(), arr);
                P1.pop();
            }
            else
            {
                cout << "Your Friend Has Put X in this Poistion \n";
                cellPoition(arr, cu, p1);
            }
        }
    }
    else
    {
        if (user2undo.isEmpty())
        {
            cout << "you didn't undo a position yet \n";
            cellPoition(arr, cu, p1);
        }
        else
        {
            if (chekPosition(arr, P2.topX(), P2.topY()))
            {
                c = user2undo.top();
                swap(c, P2.topX(), P2.topY(), arr);
                P2.pop();
            }
            else
            {
                cout << "Your Friend Has Put O in this Poistion \n";
                cellPoition(arr, cu, p1);
            }
        }
    }
}

// the function for recieve the player's option if he will enter a cell position, undo or redo
void playerOptions(char arr[3][3], int option, string cu, string p1)
{
    if (option == 1) cellPoition(arr, cu, p1);
    else if (option == 2) undo(arr, cu, p1);
    else if (option == 3) redo(arr, cu, p1);
}

// a function to check that three positions in the array are equal
bool eq(char x, char y, char z) {
    if (x == y && x == z && x != '-') {
        return true;
    }
    return false;
}

// a function to check if the two player are tied
bool chekTie(char arr[3][3])
{
    bool tie;
    for (int i = 0; i < 3; i++)
    {
        for (int g = 0; g < 3; g++)
        {
            if (arr[i][g] != '-')
            {
                tie = true;
            }
            else
            {
                tie = false;
                break;
            }
        }
        if (tie) return false;
    }
    if (tie) return true;
    else return false;
}

// a function to check if the two player are tied
void chekWinner(char arr[3][3])
{

    //1
    if (eq(arr[0][0], arr[0][1], arr[0][2]))
    {
        if (arr[0][0] == 'O') chekWinnerInt = 2;
        else chekWinnerInt = -2;
    }
    //2
    else if (eq(arr[1][0], arr[1][1], arr[1][2]))
    {
        if (arr[2][0] == 'O') chekWinnerInt = 2;
        else chekWinnerInt = -2;
    }
    //3
    else if (eq(arr[2][0], arr[2][1], arr[2][2]))
    {
        if (arr[2][0] == 'O') chekWinnerInt = 2;
        else chekWinnerInt = -2;
    }
    //4
    else if (eq(arr[0][0], arr[1][0], arr[2][0]))
    {
        if (arr[0][0] == 'O') chekWinnerInt = 2;
        else chekWinnerInt = -2;
    }
    //5
    else if (eq(arr[0][1], arr[1][1], arr[2][1]))
    {
        if (arr[0][1] == 'O') chekWinnerInt = 2;
        else chekWinnerInt = -2;
    }
    //6
    else if (eq(arr[0][2], arr[1][2], arr[2][2]))
    {
        if (arr[0][2] == 'O') chekWinnerInt = 2;
        else chekWinnerInt = -2;
    }
    //7
    else if (eq(arr[0][0], arr[1][1], arr[2][2]))
    {
        if (arr[0][0] == 'O') chekWinnerInt = 2;
        else chekWinnerInt = -2;
    }
    //8
    else if (eq(arr[0][2], arr[1][1], arr[2][0]))
    {
        if (arr[0][0] == 'O') chekWinnerInt = 2;
        else chekWinnerInt = -2;
    }
    //9
    else if (chekTie(arr)) chekWinnerInt = 0;
}

// The Game Function that starts game;
void game()
{
    system("cls");
    system("Color E1");

    char arr[3][3] = { {'-','-','-'}, {'-','-','-' }, {'-','-','-' } };
    string p1, p2;
    cout << "please enter player one name : ";
    cin >> p1;
    cout << "please enter player two name : ";
    cin >> p2;
    cout << "player one \"" << p1 << "\" will play with " << "\'O\', " << " player two \"" << p2 << "\" will play with " << "\'X\'" << endl;
    string currentUser = p1;
    chekWinnerInt = 1;
    while (chekWinnerInt == 1)
    {
        int option;
        if (currentUser == p1)
        {
            cout << "\"" << currentUser << "\" please enter 1- cell position , 2- undo , 3- redo :";
            cin >> option;
            while (option > 3 || option < 1)
            {
                cout << "Invalid Input \n";
                cout << "\"" << currentUser << "\" please enter 1- cell position , 2- undo , 3- redo :";
                cin >> option;
            }
            playerOptions(arr, option, currentUser, p1);
            draw(arr);
            currentUser = p2;
            chekWinner(arr);
            if (chekWinnerInt == 2)
            {
                cout << p1 << " is the winner \n";
            }
            else if (chekWinnerInt == 0)
            {
                cout << "There is no winner \n";
            }
            else { cout << currentUser + " turn \n"; }
        }
        else
        {
            cout << "\"" << currentUser << "\" please enter 1- cell position , 2- undo , 3- redo :";
            cin >> option;
            while (option > 3 || option < 1)
            {
                cout << "Invalid Input \n";
                cout << "\"" << currentUser << "\" please enter 1- cell position , 2- undo , 3- redo :";
                cin >> option;
            }
            playerOptions(arr, option, currentUser, p1);
            draw(arr);
            currentUser = p1;
            chekWinner(arr);
            if (chekWinnerInt == -2)
            {
                cout << p2 << " is the winner \n";
            }
            else if (chekWinnerInt == 0)
            {
                cout << "There is no winner \n";
            }
            else { cout << currentUser + " turn \n"; }
        }
        chekWinner(arr);
    }
}

int main() {

    // a boolean to make the player choose if he want to play again or no
    // it's true for the first time, after that the player decide if he want to play again or no
    // if he enterd "yes" the boolean will still true so the game function will be called again
    // if he eneterd "non" the boolean will change to false so the program will end
    bool again = true;
    while (again)
    {
        game();
        string yesno;
        system("color 04");
        cout << "Do You Want To Play Again?[yes or no] \n";
        cin >> yesno;
        if (yesno == "yes")
        {
            again = true;
        }
        else
        {
            cout << "Thank You For Your Time! \n";
            again = false;
        }
    }

    return 0;
}