#include <iostream>
#include <chrono>
#include <conio.h>
#include <vector>
#include <utility>
using namespace std::chrono;

//fruits inside snake
//more fruits

//wall teleport in 2.0 (bad code here)

std::vector<std::pair<int, int>> allPos;

short currentX = 5;
short currentY = 5;
char dir = 'r';
char lastdir;
bool draw = false;
char helpdir;

inline void MoveCursor(int x, int y)
{
    printf("\033[%d;%dH", (y), (x));
}

inline void Clear()
{
    system("cls");
}

class Fruit
{
public:
    int posX, posY;
    void Spawn()
    {
        int randx;
        int randy;

        while(true)
        {
            randx = rand()%18+2;
            randy = rand()%8+2;

            for(int i = 0; i < allPos.size(); i++)
                if(allPos[i].first == randx && allPos[i].second == randy)
                    continue;

            break;
        }

        this->posX = randx;
        this->posY = randy;

        MoveCursor(this->posX, this->posY);
        std::cout << '^';
    }
};

void Draw()
{
    Clear();
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 20; j++)
        {
            if(j == 0 || i == 0 || j == 19 || i == 9)
                std::cout << "#";
            else
                std::cout << " ";
        }
        std::cout << "\n";
    }
    MoveCursor(0,11);
    std::cout << "Score: 1";
}

void ChangePos()
{
    MoveCursor(allPos[allPos.size()-1].first, allPos[allPos.size()-1].second);
    std::cout << ' ';

    if(allPos[allPos.size()-1].first > allPos[allPos.size()-2].first)
        lastdir = 'l';
    else if(allPos[allPos.size()-1].first < allPos[allPos.size()-2].first)
        lastdir = 'r';
    else if(allPos[allPos.size()-1].second > allPos[allPos.size()-2].second)
        lastdir = 'u';
    else
        lastdir = 'd';

    for(int i = allPos.size()-1; i > 0; i--)
    {
        allPos[i].first = allPos[i-1].first;
        allPos[i].second = allPos[i-1].second;
        MoveCursor(allPos[i].first, allPos[i].second);
        std::cout << '@';
    }
}

void MoveRight()
{
    MoveCursor(currentX++, currentY);
    std::cout << " @";

    if(allPos.size() > 1)
        ChangePos();
    allPos[0].first = currentX;
}

void MoveUp()
{
    MoveCursor(currentX, currentY--);
    std::cout << ' ';
    MoveCursor(currentX, currentY);
    std::cout << '@';
    if(allPos.size() > 1)
        ChangePos();
    allPos[0].second = currentY;
}

void MoveLeft()
{
    MoveCursor(--currentX, currentY);
    std::cout << "@ ";
    if(allPos.size() > 1)
        ChangePos();
    allPos[0].first = currentX;
}

void MoveDown()
{
    MoveCursor(currentX, currentY++);
    std::cout << ' ';
    MoveCursor(currentX, currentY);
    std::cout << '@';
    if(allPos.size() > 1)
        ChangePos();
    allPos[0].second = currentY;
}

inline void Input()
{
    if(_kbhit())
    {
        switch(_getch())
        {
        case 'w':
            if(dir != 'd')
                dir = 'u';
            break;
        case 's':
            if(dir != 'u')
                dir = 'd';
            break;
        case 'a':
            if(dir != 'r')
                dir = 'l';
            break;
        case 'd':
            if(dir != 'l')
                dir = 'r';
            break;
        }
    }
}

inline void AfterFruit(const char& here)
{
    if(here == 'u')
        MoveCursor(allPos[allPos.size()-1].first, allPos[allPos.size()-1].second+1);
    else if(here == 'd')
        MoveCursor(allPos[allPos.size()-1].first, allPos[allPos.size()-1].second-1);
    else if(here == 'l')
        MoveCursor(allPos[allPos.size()-1].first+1, allPos[allPos.size()-1].second);
    else if(here == 'r')
        MoveCursor(allPos[allPos.size()-1].first-1, allPos[allPos.size()-1].second);
}

void FruitCheck(Fruit* fruitt)
{
    if(currentX == fruitt->posX && currentY == fruitt->posY)
    {
        if(allPos.size()>1)
        {
            AfterFruit(lastdir);
            std::cout << '@';

            helpdir = lastdir;
            draw = true;

            allPos.push_back(std::make_pair(allPos[allPos.size() - 1].first, allPos[allPos.size() - 1].second));
        }
        else
        {
            if(dir == 'l')
                MoveCursor(allPos[0].first+1, allPos[0].second);
            else if(dir == 'r')
                MoveCursor(allPos[0].first-1, allPos[0].second);
            else if(dir == 'u')
                MoveCursor(allPos[0].first, allPos[0].second+1);
            else
                MoveCursor(allPos[0].first, allPos[0].second-1);

            std::cout << '@';

            draw = true;
            helpdir = dir;

            allPos.push_back(std::make_pair(allPos[0].first, allPos[0].second));
        }

        fruitt->Spawn();
    }
}

int main()
{
    std::srand(time(nullptr));
    allPos.push_back(std::make_pair(5, 5));
    Draw();
    MoveCursor(5,5);
    std::cout << "@";

    Fruit* fruit = new Fruit();
    fruit->Spawn();
    Fruit* fruit2 = new Fruit();
    fruit2->Spawn();

    auto start = high_resolution_clock::now();
    while(true)
    {
        if(high_resolution_clock::now() - start > std::chrono::milliseconds(500))
        {
            Input();

            if(dir == 'r')
                MoveRight();
            else if(dir == 'u')
                MoveUp();
            else if(dir == 'l')
                MoveLeft();
            else if(dir == 'd')
                MoveDown();

            start = high_resolution_clock::now();

            if(draw)
            {
                draw = false;
                AfterFruit(helpdir);
                std::cout << ' ';
            }
            
            FruitCheck(fruit);
            FruitCheck(fruit2);

            MoveCursor(0,11);
            std::cout << "Score: " << allPos.size();
        }

        if(currentX == 20 || currentX == 1 || currentY == 10 || currentY == 1)
        {
            MoveCursor(0,12);
            exit(0);
        }

        if(allPos.size()>2)
        {
            for(int i = allPos.size()-1; i > 0; i--)
            {
                if(allPos[i].first == currentX && allPos[i].second == currentY)
                    exit(0);
            }
        }
    }
}