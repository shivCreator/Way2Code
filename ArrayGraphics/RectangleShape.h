#pragma once
#include"entity.h"

class RectangleShape : public entity
{
public:
    //Constructor- construct a rectangle width X height
    RectangleShape(int width=10, int height=10);
    //set complete border to 'border'
    void setBorder(char border);
    //set the space to region character
    void setFillCharacter(char region);
    //actually fill the rectangle
    void draw();
    //set the position
    void setPosition(int x, int y);
    //return rectangle length
    int getLength();
    //return rectangle height
    int getHeight();
    //increment posX and posY by x and y respectively
    //up    -> -ve
    //left  -> -ve
    //right -> +ve
    //down  -> +ve
    void Move(int x=1, int y=1);
    //sets the fill character to 'character'
    void setCharacter(char character);
    //limit movement and draw to window
    void stayInside(entity&);
};
