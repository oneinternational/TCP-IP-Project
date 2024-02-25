#include <iostream>
#include <cstring>
#include <windows.h>


typedef enum ColorType{
BLACK = 0,
darkBLUE = 1,
DarkGreen = 2, 
darkSkyBlue = 3, 
DarkRed = 4,
DarkPurple = 5,
DarkYellow = 6,
GRAY = 7,		
DarkGray = 8,	
BLUE = 9,		
GREEN = 10,		
SkyBlue = 11,	
RED = 12,		
PURPLE = 13,		
YELLOW = 14,		
WHITE = 15		
} COLOR;

class ImgArray
{
private:
    int logo[9][45]={
    {7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7},
    {7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
    {7,0,0,1,1,0,0,2,0,0,2,0,0,3,3,0,0,4,4,4,4,4,0,0,1,1,0,0,2,0,0,2,0,0,3,3,0,0,4,4,4,4,4,0,7},
    {7,0,1,0,0,1,0,2,0,0,2,0,3,0,0,3,0,0,0,4,0,0,0,1,0,0,1,0,2,0,0,2,0,3,0,0,3,0,0,0,4,0,0,0,7},
    {7,0,1,0,0,0,0,2,2,2,2,0,3,3,3,3,0,0,0,4,0,0,0,1,0,0,0,0,2,2,2,2,0,3,3,3,3,0,0,0,4,0,0,0,7},
    {7,0,1,0,0,1,0,2,0,0,2,0,3,0,0,3,0,0,0,4,0,0,0,1,0,0,1,0,2,0,0,2,0,3,0,0,3,0,0,0,4,0,0,0,7},
    {7,0,0,1,1,0,0,2,0,0,2,0,3,0,0,3,0,0,0,4,0,0,0,0,1,1,0,0,2,0,0,2,0,3,0,0,3,0,0,0,4,0,0,0,7},
    {7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
    {7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7}
    };
public:
    void printLogo()
    {
        for(int i = 0; i<9; i++)
    {
        for(int j=0; j<45; j++)
        {
            if(logo[i][j] == 0)
                std::cout << "  ";
            else if(logo[i][j] == 1)
            {
                textcolor(DarkYellow);
                std::cout << "бс ";
            }
            else if(logo[i][j] == 2)
            {
                textcolor(BLUE);
                std::cout <<"бс ";
            }
            else if(logo[i][j] == 3)
            {
                textcolor(RED);
                std::cout <<"бс ";
            }
            else if(logo[i][j] == 4)
            {
                textcolor(GREEN);
                std::cout <<"бс ";
            }
            else if(logo[i][j] == 7)
            {
                textcolor(PURPLE);
                std::cout <<"бр ";
            }
        }
        std::cout << std::endl;
    }
    }
    void textcolor(int colorNum)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNum);
    }
};



int main(void)
{

    ImgArray a;
    a.printLogo();
    return 0;
}

