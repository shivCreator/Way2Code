#include<SFML/Graphics.hpp>
#include<vector>
#include<iostream>
#include<math.h>

using namespace sf;

int main(){
RenderWindow app(VideoMode(800,600),"Circle");
app.setFramerateLimit(700);

RectangleShape bar(Vector2f(100,15));   //make the player-bar/paddle

bar.setFillColor(Color::Yellow);        //paint the player-bar yellow
bar.setOutlineColor(Color::Black);      //paint the player-bar outline black
bar.setOutlineThickness(-2);            //make the player-bar outline thick enough
bar.setPosition(Vector2f(400, 570));    //put the player-bar in the middle-bottom of the window
bar.setOrigin(Vector2f(50,7.5f));       //put the carry-handle at its middle point
float slide=0;                          //1D-speed of player-bar's motion
int playerScore = 0;

RectangleShape Computer(Vector2f(100,15));  //make AI-bar/paddle

Computer.setFillColor(Color::Green);        //paint AI-bar green
Computer.setOutlineThickness(-2);           //make the  AI-bar's outlines thick enough
Computer.setOutlineColor(Color::Black);     //paint the AI-bar's outlines black
Computer.setPosition(Vector2f(400,40));     //put the AI-bar at the top-middle corner
Computer.setOrigin(Vector2f(50,7.5f));           //put the AI-bar's carry-handle at its center point
float cSlide=0;                             //1D-speed of AI-bar's motion
int AIscore = 0;

CircleShape ball;                           //make the ball
float x=1.f,y=1.f;                          //dependent 2D ball speed

ball.setRadius(8);                          //set the radius of the ball
ball.setFillColor(Color::Red);              //paint the ball red
ball.setOutlineColor(Color::Black);         //paint the ball's outlines black
ball.setOutlineThickness(-2);               //make the outlines of the ball thick enough
ball.setOrigin(Vector2f(10,10));            //put the carry-handle at the center of the ball
ball.setPosition(Vector2f(app.getSize().x/2,app.getSize().y/2));//put the ball in the middle of the screen

float ballSpeed=0.5;                        //independent ball speed
float ballDirX=1;                           //which direction is the ball moving in Horizontally
float modDir;

    //when the window is open
    while(app.isOpen()){
        Event event;
        //if some event occurs
        while(app.pollEvent(event)){
            if(event.type == Event::Closed)//if the window needs to be closed
                app.close();               //close the window
        }
        //update
            //bar

            //if player pressed Left-Arrow key
            if(Keyboard::isKeyPressed(Keyboard::Left)){slide=-ballSpeed;}

            //if player pressed Right-Arrow key
            if(Keyboard::isKeyPressed(Keyboard::Right)){slide=ballSpeed;}

            //if the player-bar hits the left end of the screen/window
            if(bar.getPosition().x<50){slide=0;

                //deter bar from moving further left
                bar.setPosition(Vector2f(50,570));

            }

            //if the player-bar hits the right end of the screen/window
            if(bar.getPosition().x>(app.getSize().x-50)){slide=0;

                //deter bar from moving further right
                bar.setPosition(Vector2f((app.getSize().x-50),570));
            }

                //move or control the player-bar accordingly
                bar.move(Vector2f(slide,0));

                //stop the motion of the player-bar
                slide = 0;

//        Computer

             //if the AI-bar hits the left end of the screen/window
            if(Computer.getPosition().x<50){
                    cSlide = 0;

                //deter AI-bar from moving further left
                Computer.setPosition(Vector2f(50,40));
            }

            //if the AI-bar hits the right end of the screen/window
            if(Computer.getPosition().x>(app.getSize().x-50)){cSlide=0;

                //deter AI-bar from moving further right
                Computer.setPosition(Vector2f((app.getSize().x-50),40));
            }

            Computer.move(Vector2f(ballDirX*cSlide,0));
            cSlide = 0.44f;

        //if ball-windowBoundaries collide

            //if ball hits left screen boundary of the window
            if(ball.getPosition().x<10){x=ballSpeed;}

            //if ball hits upper boundary of the window
            if(ball.getPosition().y<10){y=ballSpeed; playerScore++;}

            //if ball hits the right boundary of the window
            if(ball.getPosition().x>(app.getSize().x-10)){x=-ballSpeed;}

            //if ball hits the bottom boundary of the window
            if(ball.getPosition().y>(app.getSize().y-10)){y=-ballSpeed;AIscore++;}

            //move ball accordingly
            ball.move(Vector2f(x,y));

            //direction of the ball's motion
            modDir = ball.getPosition().x-Computer.getPosition().x;
            ballDirX = modDir/(sqrt(pow(modDir,2)));

    //collision

                //if ball-bar collide with each other
                if(ball.getGlobalBounds().intersects(bar.getGlobalBounds())){

                    y=-ballSpeed;//reflect back upside

                    //if ball hits extreme left of the bar, reflect back left-ways swiftly
                    if(ball.getPosition().x<(bar.getPosition().x/2))x=-1.5*ballSpeed;

                    //otherwise if ball hits middle left of the bar, reflect back left-ways
                    else if(ball.getPosition().x<bar.getPosition().x)x=-ballSpeed;

                    //otherwise if ball hits extreme right of the bar, reflect back right-ways swiftly
                    else if(ball.getPosition().x>(bar.getPosition().x/2))x=1.5*ballSpeed;

                    //otherwise if ball hits middle right of the bar, reflect back right-ways
                    else if(ball.getPosition().x>bar.getPosition().x)x=ballSpeed;
                }

                //if ball-AI bar collide with each other
                if(ball.getGlobalBounds().intersects(Computer.getGlobalBounds())){
                    y=ballSpeed;

                    //if ball hits extreme left of the AI-bar, reflect back left-ways swiftly
                    if(ball.getPosition().x<(Computer.getPosition().x/2))x=-1.5*ballSpeed;

                    //otherwise if ball hits middle left of the bar, reflect back left-ways
                    else if(ball.getPosition().x<Computer.getPosition().x)x=-ballSpeed;

                    //otherwise if ball hits extreme right of the bar, reflect back right-ways swiftly
                    else if(ball.getPosition().x>(Computer.getPosition().x/2))x=1.5*ballSpeed;

                    //otherwise if ball hits middle right of the bar, reflect back right-ways
                    else if(ball.getPosition().x>Computer.getPosition().x)x=ballSpeed;
                }


        //update screen
        app.clear(Color::White);


        app.draw(Computer); //draw AI-Bar on the screen
        app.draw(ball);     //draw ball on the screen
        app.draw(bar);      //draw player-bar on the screen

        app.display();//display the screen with drawn objects
    }
//        update scores
//            system("cls");
            std::cout<<"Player : "<<playerScore<<std::endl;
            std::cout<<"AI : :"<<AIscore<<std::endl;

            if(playerScore>AIscore)
                std::cout<<"You Won!";
            else if(playerScore<AIscore)
                std::cout<<"You Lost!";
            else
                std::cout<<"Oww! It was a Tie!";
 return 0;//hand control over to the OS and return to IDE
}
