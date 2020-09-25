#include "ArrayWindow.h"
#include "RectangleShape.h"
//font: lucida console, size:12
//cosole screen: 120 x 40

int main()
{

    //create a 60x30 window with title "MY FIRST ARRAY WINDOW"
    ArrayWindow window(60, 30);
    window.setTitle("MY FIRST ARRAY WINDOW");
    window.setConsoleColor(BLACK, BRIGHT_WHITE);

    //create a 2x2 box
    RectangleShape box(2,2);
    box.setCharacter(254);//set fill and boundary characters equal to white box box

    //The Game Loop
    while(window.isOpen())
    {
    //update
        //box motion
        if(window.isKeyPressed('W'))//up
        {   box.Move(0, -1);    }
        if(window.isKeyPressed('S'))//down
        {   box.Move(0,  1);    }
        if(window.isKeyPressed('A'))//left
        {   box.Move(-1, 0);    }
        if(window.isKeyPressed('D'))//right
        {   box.Move( 1, 0);    }
    //bounds
        //keep box inside the window
        box.stayInside(window);

    //draw
        //clear screen
        window.clean();
        //draw box onto the screen
        window.draw(box);
        //Display Window
        window.Display();

    }
    return 0;
}
