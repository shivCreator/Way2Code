#include "entity.h"
#include<iostream>
#include<process.h>
#include<stdlib.h>
#include<windows.h>

entity::entity(int width, int height, int posX, int posY)
:WIDTH(width), HEIGHT(height), POS_X(posX), POS_Y(posY)
{
    if(width>(MAX - 1)||height>(MAX - 1)||width<1||height<1){
        std::cout<<"width or height exceeds maximum resolution:"<<MAX-1<<" X "<<MAX-1;
        system("pause");
        exit(0);
    }
}

//collideWith
bool entity::collideWith(entity& ent)
{
    if(/*horizontal collision*/
     (ent.POS_X + ent.WIDTH - 1) >= this->POS_X && ent.POS_X <= (this->POS_X + this->WIDTH - 1)
       /*vertical collision*/               &&
     (ent.POS_Y + ent.HEIGHT - 1) >= this->POS_Y && ent.POS_Y <= (this->POS_Y + this->HEIGHT - 1)
     )
     return true;
     else return false;
}

//isAbove
bool entity::isBelow(entity& ent)
{
    if(ent.POS_Y <= this->POS_Y )
        return true;
    else return false;
}

//isBelow
bool entity::isAbove(entity& ent)
{
    if(ent.POS_Y > this->POS_Y)
        return true;
    else return false;
}

//isBefore
bool entity::isAfter(entity& ent)
{
    if(ent.POS_X <= this->POS_X)
        return true;
    else return false;
}

//isAfter
bool entity::isBefore(entity& ent)
{
    if(ent.POS_X > this->POS_X)
        return true;
    else return false;
}

//control
void entity::control(char up, char down, char left, char right)
{
    if(GetAsyncKeyState((unsigned short)up) & 0x8000)
    {
        this->POS_Y--;
    }
    if(GetAsyncKeyState((unsigned short)down) & 0x8000)
    {
        this->POS_Y++;
    }
    if(GetAsyncKeyState((unsigned short)left) & 0x8000)
    {
        this->POS_X--;
    }
    if(GetAsyncKeyState((unsigned short)right) & 0x8000)
    {
        this->POS_X++;
    }
}
