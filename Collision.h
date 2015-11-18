#include <windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <sstream>
#include <math.h>
#include <iostream>
#include <assert.h>
#include <fstream>
#include <time.h>

/**
    Checks if the ball collided with a bumper

    @param p Point from where the bumper is located
    @param type switch the different type of collitions,
    1 = bumper (500), 2 = target (1000 score) 3 = top wall
    4 = side walls ,  5 = upper side wall, 6 = outher wall
    7 = draw hole,   8 = circle collision

*/
void checkColission(Point &p, int type){

    switch (type) {
        case 1: { //bumper
             // Distance to bumper
            float d = sqrt((pow(p.x - currentPosition.x, 2)) + (pow(p.y - currentPosition.y, 2)));

            if(d <= 22)
            {
                // do this when there is a collision
                score += 500;
                vx = -vx;
                vy = 0.5;
                vy = -vy ;
            }
        }
        break;
        case 2: { //target
            if(p.y < currentPosition.y + 7 && (currentPosition.x > p.x && currentPosition.x < p.x + 20))
            {
                score += 1000;
                vy = 0.5;
                vy = -vy;
            }
        }
        break;
        case 3: { //top wall
            if(p.y < currentPosition.y + 7)
            {
                vy = -vy;
            }
        }
        break;
        case 4: { //side walls
            if((p.x + 10 < currentPosition.x - 7 || p.x > currentPosition.x + 7) && currentPosition.y < 110)
            {
                // do this when there is collision with side of wall
                vx = -vx;
            }
        }
        break;
        case 5: { //upper side wall
            if(p.x < currentPosition.x + 7 && p.y + 400 > currentPosition.y)
            {
                // collision with side of wall
                vx = -vx;
            }
        }
        break;
        case 6: { //outer sidewall
            if(p.x + 10 < currentPosition.x - 7 || p.x > currentPosition.x + 7)
            {
                // do this when there is collision with side of wall
                vx = -vx;
            }
            else if(p.y + 440 < currentPosition.y - 7 || p.y > currentPosition.y + 7)
            {
                // do this when there is collision with top of wall
                vy = 0.5;
                vy = -vy;
            }
        }
        break;

        case 7: {
             float d = sqrt((pow(p.x - currentPosition.x, 2)) + (pow(p.y - currentPosition.y, 2)));
             if (d <= 22 && ballCounter > 0) {
                // do this when there is a collision and there are no more balls left
                ballCounter--;
                cout << "Ball Counter: " << ballCounter << endl;
                currentPosition.x = 280;
                currentPosition.y = 77;
                vx = -vx;
                vy = -vy;
            }
        }
        break;
        case 8: { //circle collision
            if((p.y < 1 || p.x > 300 || p.y > 440) && ballCounter > 0)
            {
                ballCounter--;
                p.x = 280;
                p.y = 77;
                vx = -vx;
                vy = -vy;
            }
            // If the ball is in the launching platform
            if(p.x > 262)

            {
                // This is if the ball is still in the launching platform
                if(p.y >= 420)
                {
                    vx = vy;
                    p.x -= vx;
                }
                else
                {
                    // Launch the ball with that force depending on the launch
                    vy = 0;
                    vy += (launch/2000);
                    p.y += vy;
                }
            }
            else
            {
                acc = 0;
                p.x -= vx;
                // Caida
                if(vy > 0){
                    vy += g;
                    vy -= f;
                }
                // Subiendo
                else if(vy < 0){
                    vy -= g;
                    vy += f;
                }
                p.y -= vy;
            }
        }
        break;
        case 9: // flipper left


           if(50  > currentPosition.y - 7 && ( currentPosition.x > 50 && currentPosition.x < 115 ))
            {
                cout << "Colision con flipper left " << endl;
                // do this when there is collision with top of wall
                vy = -vy;
            }
        break;
        case 10: // flipper right



           if(50 > currentPosition.y - 7 && ( currentPosition.x > 155 && currentPosition.x < 215 ))
            {
                cout << "Colision con flipper right " << endl;
                // do this when there is collision with top of wall
                vy = -vy;
            }
        break;


    }

}