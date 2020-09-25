#include "ArrayWindow.h"
#include <iostream>
#include <windows.h>
#include <string.h>

//constructor
ArrayWindow::ArrayWindow(int width, int height, char boundary, char Fill)
: entity(width, height), offset('\0')
{
    REGION = Fill;
    BOUNDARY = boundary;
    for(int row = 0;row<HEIGHT; row++){
        for(int col = 0; col < WIDTH; col++)
        {
            if( row == 0 || row == (HEIGHT-1) || col == 0 || col == (WIDTH-1)  )
                array[row][col] = BOUNDARY;
            else
                array[row][col] = REGION;
        }
      }

    color[0] = 'c';
    color[1] = 'o';
    color[2] = 'l';
    color[3] = 'o';
    color[4] = 'r';
    color[5] = ' ';
    color[6] = '0';
    color[7] = '7';
    color[8] = '\0';
    color[9] = '\0';

    m_Title = " ";
    m_titleLength = 0;
    m_Open = true;

}

//display
void ArrayWindow::Display()
{
    if(m_titleLength)
    {
        for(int i=0;i < WIDTH - m_titleLength / 2; i++)
            std::cout << m_titleBar;
            std::cout << m_Title;
        for(int i=0;i < WIDTH - m_titleLength / 2; i++)
            std::cout << m_titleBar;
            std::cout << '\n';
    }

    for(int row = 0; row < HEIGHT ; row++)
    {
        for(int col = 0; col < WIDTH; col++)
        {
            std::cout << array[row][col] << offset;
        }
        std::cout << '\n';
    }
}

//setConsoleColor
void ArrayWindow::setConsoleColor(char bgColor, char fgColor)
{

    color[6] = bgColor;
    color[7] = fgColor;

    system(color);
}

//setTitle
void ArrayWindow::setTitle(const char* Title, char titleBar)
{
    m_Title = Title;
    m_titleBar = titleBar;
    m_titleLength = strlen(Title);
}

//setBorder
void ArrayWindow::setBorder(char border)
{
    for(int row = 0; row < HEIGHT; row++)
        for(int col = 0; col < WIDTH; col++)
          if( row == 0 || row == (HEIGHT-1) || col == 0 || col == (WIDTH-1)  )
                array[row][col] = border;
}

//setUpperBorder
void ArrayWindow::setUpperBorder(char border)
{
    for(int col = 1; col < WIDTH - 1; col++)
        array[0][col] = border;
}

//setLowerBorder
void ArrayWindow::setLowerBorder(char border)
{
    for(int col = 1; col < WIDTH - 1; col++)
        array[HEIGHT - 1][col] = border;
}

//setLeftBorder
void ArrayWindow::setLeftBorder(char border)
{
    for(int row = 1; row < HEIGHT - 1; row++)
        array[row][0] = border;
}

//setRightBorder
void ArrayWindow::setRightBorder(char border)
{
    for(int row = 1; row < HEIGHT - 1; row++)
        array[row][WIDTH-1] = border;
}

//setCorners
void ArrayWindow::setCorners(char corner)
{
    array[0][0] = array[0][WIDTH-1]= array[HEIGHT-1][WIDTH-1]= array[HEIGHT-1][0] = corner;
}

//setTopLeftCorner
void ArrayWindow::setTopLeftCorner(char corner)
{
    array[0][0] = corner;
}

//setTopRightCorner
void ArrayWindow::setTopRightCorner(char corner)
{
    array[0][WIDTH-1] = corner;
}

//setBottomLeftCorner
void ArrayWindow::setBottomLeftCorner(char corner)
{
    array[HEIGHT-1][0] = corner;
}

//setBottomRight Corner
void ArrayWindow::setBottomRightCorner(char corner)
{
    array[HEIGHT-1][WIDTH-1] = corner;
}

//draw(RectangleShape&)
void ArrayWindow::draw(RectangleShape& shape)
{
    shape.draw();
    if(shape.POS_X == 0){shape.POS_X = 1;}
    if(shape.POS_Y == 0){shape.POS_Y = 1;}

    for(int row = 0; row < shape.getHeight(); row++)
        for(int col = 0; col < shape.getLength();col++)
        {
//            if(shape.array[row][col] == ' '){continue;}
            this->array[row + shape.POS_Y][col + shape.POS_X] = shape.array[row][col];
        }
}

//clean
void ArrayWindow::clean(char region)
{
    system("cls");
    for(int i=1;i<HEIGHT-1;i++)
        for(int j=1;j<WIDTH-1;j++)
            array[i][j] = region;
}

//isKeyPressed
bool ArrayWindow::isKeyPressed(char capKey)
{
    return (GetAsyncKeyState((unsigned short)capKey) & 0x8000);
}

//close
void ArrayWindow::close()
{
    this->m_Open = false;
}
bool ArrayWindow::isOpen()
{
    return m_Open;
}

//drawArray
bool ArrayWindow::drawArray(char Array1D[], int rows, int columns,int x_coord,int y_coord)
{
    if(rows<=0 || columns<=0){return false;}
    if(x_coord<=0){x_coord = 1;}
    if((x_coord+columns)>=WIDTH){x_coord = WIDTH - columns - 1;}
    if(y_coord<=0){y_coord = 1;}
    if((y_coord+rows)>=HEIGHT){y_coord = HEIGHT - rows - 1;}

    for(int i=0;i<rows;i++)
    {
        for(int j=0; j<columns; j++)
        {
//            if(Array1D[columns * i + j] == ' '){continue;}
            array[y_coord + i][x_coord+j] = Array1D[columns * i + j];
        }
    }
    return true;
}

//Rotate
void ArrayWindow::Rotate(char arr[],int quarter, int width, int x, int y)
{
    if(x<=0){x=1;}
    if(x+width >= WIDTH){x = WIDTH - width - 1;}
    if(y<=0){y=1;}
    if(y+width >= HEIGHT){y = HEIGHT - width - 1;}
    if(quarter <0){quarter = 3;}
    if(quarter > 3){quarter = 0;}

    int temp = 0;;
    switch(quarter)
    {
    case 0: //no rotation
        for(int i = 0; i < width; i++)
        {
            for(int j=0;j<width;j++)
            {
//                if(arr[width*i + j] == ' '){continue;}
                array[i + y][j + x] = arr[width*i + j];
            }
        }
        break;
    case 1://90* rotation
        temp = width * (width - 1);
        for(int i = 0; i < width; i++)
        {
            for(int j=0;j<width;j++)
            {
//                if(arr[temp + i - width*j] == ' '){continue;}
                array[i + y][j + x] = arr[temp + i - width*j];
            }
        }
        break;
    case 2://180* rotation
        temp = width*(width - 1) + width - 1;
        for(int i = 0; i < width; i++)
        {
            for(int j=0;j<width;j++)
            {
//                if(arr[temp - width * i - j] = ' '){continue;}
                array[i + y][j + x] = arr[temp - width * i - j];
            }
        }
        break;
    case 3://270* rotation
        temp = (width-1);
        for(int i = 0; i < width; i++)
        {
            for(int j=0;j<width;j++)
            {
//                if(arr[temp - i + width*j] == ' '){continue;}
                array[i + y][j + x] = arr[temp - i + width*j];
            }
        }
    }
}

//getWindow
void ArrayWindow::getWindow(char* screen)
{
    for(int y=0;y<HEIGHT;y++){
        for(int x=0;x<WIDTH;x++){
            screen[y*WIDTH + x] = array[y][x];
        }
    }
}

