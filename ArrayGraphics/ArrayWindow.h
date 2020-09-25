#pragma once
#include "entity.h"
#include "RectangleShape.h"

class ArrayWindow : public entity
{
    char offset;        //character between each window character
    char color[9];      //color to hold console color
    const char* m_Title;  //title of the window
    char m_titleBar;    //character for titleBar
    int m_titleLength;  //title length
    bool m_Open;
public:
//        /Array Window Constructor
    ArrayWindow(int width = 32, int height = 32, char boundary = '#', char Fill = ' ' );
//       /Prints Window's contents to the console
    void Display();
//       /set console background and text color
    void setConsoleColor(char bgColor, char fgColor);
//       /fill complete border with one single character
//       overtakes all corner commands
    void setBorder(char border);
//       /fill just upper border with a specific character
    void setUpperBorder(char border);
//       /fill just lower border with a specific character
    void setLowerBorder(char border);
//       /fill just left border with specific character
    void setLeftBorder(char border);
//       /fill just right border with specific character
    void setRightBorder(char border);
//       set all corner characters to 'corner'
    void setCorners(char corner);
//       set the top left chracter to 'corner'
    void setTopLeftCorner(char corner);
//       set top right character to 'corner'
    void setTopRightCorner(char corner);
//       set bottom left character to 'corner'
    void setBottomLeftCorner(char corner);
//       set bottom right character to 'corner'
    void setBottomRightCorner(char corner);
//       set the title to 'Title', titleLength for proper alignment
//                                 titleBar character to fill the titleBar
    void setTitle(const char* Title , char titleBar = '=');
//       copy rectangle array to window array at specifiied cordinates(position)
    void draw (RectangleShape&);
//       fill window space with 'region' character
    void clean(char region = ' ');
//       check if the capital key 'capKey' is pressed
    bool isKeyPressed(char capKey);
//       close window
    void close();
//       check if the window is open
    bool isOpen();
//       draw any array onto the window
    bool drawArray(char Array1D[], int rows=1, int columns=1,int x_coord = 1,int y_coord = 1);
//       rotate the array
    void Rotate(char arr[],int quarter, int width, int x, int y);
//       returns 2d window into passed 1d array
    void getWindow(char* screen);
};
