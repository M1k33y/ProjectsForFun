#include <iostream>
#include <conio.h>
#include <windows.h>
#include <fstream>
using namespace std;
bool gameOver;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score,bestScore;
int gameMode;
int tailX[width * height], tailY[width * height];
int nTail;
enum eDirection
{
    STOP = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN
};
eDirection dir;


void LoadBestScore()
{
    ifstream fin("bestScore.txt");
    if(fin.is_open())
    {
        fin>>bestScore;
        fin.close();
    }
}
void SaveBestScore()
{
    ofstream fout("bestScore.txt");
    fout<<bestScore;
    fout.close();
}
void SetColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
void Setup()
{
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2; // capul sarpelui centrat pe mijloc
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
}
void DrawButton(const string&text,bool selected)
{
    if(selected)
    {
        SetColor(14); //ii dau highlight
        cout << "\t>> " << text << " <<\n";
    }
    else
    {
        SetColor(7); // Normal color
        cout << "\t   " << text << "   \n";
    }
}

int SelectGameMode()
{
    int option=0;
    const int optionNum=2;
    string options[]= {"Classic Mode (with walls)","Infinite Mode (no walls)"};
    bool choosing=true;
    while(choosing)
    {
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 0});
        SetColor(18);
        cout << "\n\n\t\t=== SNAKE GAME ===\n\n";
        SetColor(7);
        cout << "\tUse arrow keys to select mode:\n\n";


        for (int i = 0; i < optionNum; i++)
            DrawButton(options[i], option == i);


        cout << "\n\t\tPress ENTER to confirm";

        switch (_getch())
        {
        case 72: // Up arrow
            option = (option - 1 + optionNum) % optionNum;
            break;
        case 80: // Down arrow
            option= (option + 1) % optionNum;
            break;
        case 13: // Enter
            choosing = false;
            break;
        }
    }
    return option + 1;
}
void Draw()
{
    // system("cls"); //face curat in consola
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 0});
    /*Am inlocuit prima comanda care e standard cu a doua din windows.h sa nu mai flickuiasca ecranu*/
    SetConsoleOutputCP(CP_UTF8);
    for (int i = 0; i < width + 2; i++)
    {
        SetColor(7); //Alb
        cout << "█";

    }
    cout << endl;

    // aici fac mapa
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (j == 0)
                cout << "█";
            if (i == y && j == x)
            {
                SetColor(14);//Galben
                cout << "O";
            }
            else if (i == fruitY && j == fruitX)
            {
                SetColor(2);
                cout << "F";
            }
            else // desenam coada
            {
                bool print = false;
                for (int k = 0; k < nTail; k++)
                {

                    if (tailX[k] == j && tailY[k] == i)
                    {
                        SetColor(9);
                        cout << "o";
                        print = true;
                    }
                }
                if (!print)
                    cout << " ";
            }

            if (j == width - 1)
            {
                SetColor(7);
                cout << "█";
            }
        }
        cout << endl;
    }

    for (int i = 0; i < width + 2; i++)
        cout << "█";
    cout << endl;
    cout << "CURENT SCORE: " << score << endl;
    cout << "BEST SCORE: "<<bestScore<<endl;
}
void Input()
{
    if (_kbhit())
    {
        switch (_getch())
        {
        case 'a':
            dir = LEFT;
            break;
        case 'd':
            dir = RIGHT;
            break;
        case 'w':
            dir = UP;
            break;
        case 's':
            dir = DOWN;
            break;
        case 'x':
            gameOver = true;
            break;
        }
    }
}
void Logic()
{
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    for (int i = 1; i < nTail; i++)
    {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
    switch (dir)
    {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    default:
        break;
    }
    if(gameMode==1)
    {
        if(x >width || x<0||y>height||y<0)
            gameOver=true;
    }
    else if(gameMode==2)
    {
        if (x >= width)
            x = 0;
        else if (x < 0)
            x = width - 1;
        if (y >= height)
            y = 0;
        else if (y < 0)
            y = height - 1;
    }
    /*
    ------------------
    am 2 variante cu pereti sau fara
    sus e varianta 1
    jose varianta fara pereti
    ------------------
    */


    for (int i = 0; i < nTail; i++)
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;
    if (x == fruitX && y == fruitY)
    {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
    }
    if(score>bestScore)
    {
        bestScore=score;
        SaveBestScore();
    }

}
int main()
{
    gameMode=SelectGameMode();
    system("cls");
    Setup();
    LoadBestScore();
    while (!gameOver)
    {
        Draw();
        Input();
        Logic();
        _sleep(100);
    }

    return 0;
}
