#include "RectangleShape.h"

//constructor
RectangleShape::RectangleShape(int width, int height)
: entity(width, height)
{
    for(int row = 0;row < HEIGHT; row++)
    {
        for(int col = 0;col < WIDTH; col++)
        {
            array[row][col] = '*';
        }
    }

    REGION = ' ';
    BOUNDARY = '*';
    POS_X = POS_Y = 0;
}

//setBorder
void RectangleShape::setBorder(char border)
{
    BOUNDARY = border;
}

//setFillCharacter
void RectangleShape::setFillCharacter(char reg)
{
    REGION = reg;
}

//getLength
int RectangleShape::getLength()
{
   return WIDTH;
}

//getHeight
int RectangleShape::getHeight()
{
    return HEIGHT;
}

//draw
void RectangleShape::draw()
{
    for(int row = 0;row < HEIGHT; row++)
        for(int col = 0; col < WIDTH; col++)
        {
            if(row == 0 || row == (HEIGHT - 1) || col == 0 || col == (WIDTH - 1 ))
            {
                array[row][col] = BOUNDARY;
            }
            else
            {
                array[row][col] = REGION;
            }
        }
}

//setCharacter
void RectangleShape::setCharacter(char character)
{
    REGION = BOUNDARY = character;
}

//setPosition
void RectangleShape::setPosition(int x, int y)
{
    POS_X = x;
    POS_Y = y;
}

//Move
void RectangleShape::Move(int x, int y)
{
    POS_X += x;
    POS_Y += y;
}

//stayInside
void RectangleShape::stayInside(entity& ent)
{
    if(POS_X <= ent.POS_X){POS_X = ent.POS_X + 1;}
    if(POS_Y <= ent.POS_Y){POS_Y = ent.POS_Y + 1;}
    if((POS_X+WIDTH) >= ent.POS_X + ent.WIDTH - 1){POS_X = ent.POS_X + ent.WIDTH - 1 - WIDTH;}
    if((POS_Y+HEIGHT) >= ent.POS_Y + ent.HEIGHT - 1){POS_Y = ent.POS_Y + ent.HEIGHT - 1 - HEIGHT;}
}
