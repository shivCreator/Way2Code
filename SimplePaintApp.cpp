#include<iostream>
#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>

using namespace std;

int main()
{
    bool  box = false, circle = false, eraser = false, cb = false, red = false, green = false, blue = false;
    float cx=20,cy=20,ex=20,ey=20,bx=20,by=20,Cx=110,R=0,G=0,B=0;
    int r=10;

    sf::RenderWindow window(sf::VideoMode(800,600),"Test Window", sf::Style::Close);

    sf::RectangleShape Box, Eraser, UPanel, CPanel, Cline, Cbox, RBox, GBox, BBox;

    CPanel.setSize(sf::Vector2f(600,40));
    CPanel.setFillColor(sf::Color::White);
    CPanel.setPosition(sf::Vector2f(100,5));
    CPanel.setOutlineColor(sf::Color::Black);
    CPanel.setOutlineThickness(-1);

    UPanel.setSize(sf::Vector2f(800,50));
    UPanel.setPosition(sf::Vector2f(0,0));
    UPanel.setFillColor(sf::Color::Yellow);
    UPanel.setOutlineColor(sf::Color::Black);
    UPanel.setOutlineThickness(-1);

    Cline.setSize(sf::Vector2f(260,5));
    Cline.setPosition(sf::Vector2f(110,25));
    Cline.setFillColor(sf::Color::Transparent);
    Cline.setOutlineColor(sf::Color::Black);
    Cline.setOutlineThickness(-2);

    Cbox.setSize(sf::Vector2f(10,10));
    Cbox.setPosition(sf::Vector2f(Cx,23));
    Cbox.setFillColor(sf::Color::White);
    Cbox.setOutlineColor(sf::Color::Black);
    Cbox.setOutlineThickness(-2);

    RBox.setSize(sf::Vector2f(20,20));
    RBox.setPosition(sf::Vector2f(390,15));
    RBox.setFillColor(sf::Color::Red);
    RBox.setOutlineColor(sf::Color::Black);
    RBox.setOutlineThickness(-1);

    GBox.setSize(sf::Vector2f(20,20));
    GBox.setPosition(sf::Vector2f(420,15));
    GBox.setFillColor(sf::Color::Green);
    GBox.setOutlineColor(sf::Color::Black);
    GBox.setOutlineThickness(-1);

    BBox.setSize(sf::Vector2f(20,20));
    BBox.setPosition(sf::Vector2f(450,15));
    BBox.setFillColor(sf::Color::Blue);
    BBox.setOutlineColor(sf::Color::Black);
    BBox.setOutlineThickness(-1);


    Box.setFillColor(sf::Color::Red);
    Eraser.setFillColor(sf::Color::White);
    sf::CircleShape Circle(r);
    Circle.setFillColor(sf::Color(12,12,120));

    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();

sf::Vector2i pos = sf::Mouse::getPosition(window);

            if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                if(pos.x>=10 && pos.x<=30 && pos.y>=10 && pos.y<=30){  box = true;   }
                if(pos.x>=40 && pos.x<=60 && pos.y>=10 && pos.y<=30){  circle = true;  }
                if(pos.x>=760 && pos.x<=780 && pos.y>=10 && pos.y<=30){  eraser = true;   }
                if(pos.x>=110 && pos.x<=370 && pos.y>=20 && pos.y<=35){ cb = true;  }
                if(pos.x>=390 && pos.x<=410 && pos.y>=15 && pos.y<=35){ red = true;  }
                if(pos.x>=420 && pos.x<=440 && pos.y>=15 && pos.y<=35){ green = true;  }
                if(pos.x>=450 && pos.x<=470 && pos.y>=15 && pos.y<=35){ blue = true;  }

                if(box == true)
                {
                     circle = false;
                     eraser = false;
                     Box.setPosition(sf::Vector2f(pos.x-(0.5*bx),pos.y-(0.5*by)));
                }
                if(circle == true)
                {
                    box = false;
                    eraser = false;
                    Circle.setPosition(sf::Vector2f(pos.x-r,pos.y-r));
                }
                if(eraser == true)
                {
                     circle = false;
                     box = false;
                     Eraser.setPosition(sf::Vector2f(pos.x-(0.5*ex),pos.y-(0.5*ey)));
                }
                if((cb == true)&&(red == true || green == true || blue == true))
                {//color box position
                    if((pos.x-5)>=110 && (pos.x-5)<=365)
                    {
                         Cx = (pos.x-5);
                         if(red == true){R = Cx;}
                         if(green == true){G = Cx;}
                         if(blue == true){B = Cx;}
                         Cbox.setPosition(sf::Vector2f(Cx,23));
                    }

//                        cout<<(Cx-110)<<endl;Color box color
                    if((box == false)&&(circle == false)&&(eraser == false))//screen color
                    {
                        window.clear(sf::Color(R, G, B));
                    }
                }
            }

            if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
            {
                if(box == true){box = false;}
                if(circle == true){circle = false;}
                if(eraser == true){eraser = false;}
                if(cb == true){cb = false;}
                if(red == true){red = false;}
                if(green == true){green = false;}
                if(blue == true){blue = false;}
            }

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Add))
            {
                if(box == true)
                {
                    bx += 2;  by += 2;
                    Box.setSize(sf::Vector2f(bx,by));
                }
                if(circle == true)
                {
                    r += 2;
                    Circle.setRadius(r);
                }
                if(eraser == true)
                {
                    ex += 2;  ey += 2;
                    Eraser.setSize(sf::Vector2f(ex,ey));
                }
            }

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract))
            {
                if(box == true)
                {
                    if(bx>0 && by>0)
                    {
                        bx -= 2; by -= 2;
                    }
                    Box.setSize(sf::Vector2f(bx,by));
                }
                if(circle == true)
                {
                    if(r>=0)r -= 2;
                    Circle.setRadius(r);
                }
                if(eraser == true)
                {
                    if(ex>=0 && ey>=0)
                    {
                        ex -= 2; ey -= 2;
                    }
                    Eraser.setSize(sf::Vector2f(ex,ey));
                }
            }

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))window.clear(sf::Color::White);
        }

        if(box == false)
        {
            bx=20; by=20;
            Box.setSize(sf::Vector2f(20 ,20));
            Box.setPosition(sf::Vector2f(10,10));
        }if(circle == false)
        {
            r=10;
            Circle.setRadius(10);
            Circle.setPosition(sf::Vector2f(40, 10));
        }if(eraser == false)
        {
            ex=20; ey=20;
            Eraser.setSize(sf::Vector2f(20,20));
            Eraser.setPosition(sf::Vector2f(760,10));
        }
        if(cb == false)
        {
            Cx = 110;
            Cbox.setPosition(sf::Vector2f(Cx,23));
//            cout<<"Cx was false"<<endl;
        }
        window.draw(UPanel);
        window.draw(CPanel);
        window.draw(Cline);
        window.draw(Cbox);
        window.draw(RBox);
        window.draw(GBox);
        window.draw(BBox);

        window.draw(Box);
        window.draw(Eraser);
        window.draw(Circle);

        window.display();
    }
}
