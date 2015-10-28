
/**
    Graficos Computacionales
    main.cpp
    Purpose: Create a pinball game to be used by kids
    with the theme of pregnancy

    Much thanks to Github user Skatrud, whos pinball
    code gave us the base for this app.

    @author Emilio Flores A01035087
    @author Sergio Cordero A01191167
    @version 1.0 28/10/2015
*/
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

using namespace std;

struct Color {
	float r;
	float g;
	float b;
	float a;
};

struct Point {
	float x;
	float y;
	float z;
};

struct Vector {
	float angle;
	float speed;
};

struct Flipper {
    Point backLeft;
    Point backRight;
    Point frontLeft;
    Point frontRight;
    Point frontTop;
};

// Colors
Color red, grey, dRed, gold, black, blue, green, white;

// Points
Point origin, leftFlipper, rightFlipper, upperWall, leftWall,holeWallLeft;
Point rightWall, farRightWall, holeWallRight, insertWall, bumper1, bumper2, target1, target2, hole1, hole2;
Point leftLFlipperState, leftRFlipperState, rightLFlipperState, rightRFlipperState;
Point currentPosition, initialPosition, lastPosition;

// Vectors
Vector current;
Vector initial;

// Flippers
Flipper rFlipper;
Flipper lFlipper;

// physics variables
float g = .00002;
float f = .00001;
float initVelX = 0;
float currVelX = 0;
float initVelY = 0;
float currVelY = 0;
float vx = 0;
float vy = 0;
float ld, rd;
float acc = 0;
float launch = 0;


const float ANGLE_CHANGE = 45;
const float PI = 3.141592;
const int BALL_SIZE = 7;
const int ANIMATION_TIME = 20;
const int screenHeight = 500;
const int screenWidth = 300;
const int circleRadius = 7;

// mouse variables
int dragging = 0;
int mousePressed = 0;
int mouseX = 0;
int mouseY = 0;
int ballCounter = 5;
int score = 0;
//flipper values
int activateRFlipper=0;
int activateLFlipper=0;
int clockR = 0;
int clockL = 0;
int lockR = 0;
int lockL = 0;
float angleR = 25.0;
float angleL = 25.0;



/**
    Prints given string in the screen

    @param x the position of x
    @param y the position of y
    @param color the Color structure to be usded
    @param str character array that wants to be printed

*/
void output(int x, int y, Color color, const char *str)
{
  glColor3f( color.r, color.g, color.b );
  glRasterPos2f(x, y);
  int len, i;
  len = (int)strlen(str);
  for (i = 0; i < len; i++) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
  }
}


/**
    Function that draws the circle in the screen given its position

    @param p point in where the circle should be draw
    @param radius of the circle to draw
    @param c Color struct to paint the circle

*/
void drawCircle(Point& p, float radius, Color& c ) {
	float delta_theta = 0.03;
	glColor3f(c.r, c.g, c.b);
	glBegin( GL_POLYGON );
	for( float angle = 0; angle < 2*PI; angle += delta_theta )
		glVertex3f( radius*cos(angle)+p.x, radius*sin(angle)+p.y, 0 );
	glEnd();


	lastPosition = p;

}

/**
    Function that draws in the screen the bumper rectangle

    @param p Point in which the bumper is located
    @param radius of the bumper to draw
    @param c Color structure containing r,g,b
*/
void drawBumper(Point& p, float radius, Color& c ) {

	// draw circle
	float delta_theta = 0.03;
	glColor3f(c.r, c.g, c.b);
	glBegin( GL_POLYGON );
	for( float angle = 0; angle < 2*PI; angle += delta_theta )
		glVertex3f( radius*cos(angle)+p.x, radius*sin(angle)+p.y, 0 );
	glEnd();
}


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
        // Bumper
        case 1: {

             // Distance to bumper
            float d = sqrt((pow(p.x - currentPosition.x, 2)) + (pow(p.y - currentPosition.y, 2)));

            if(d <= 22)
            {

                // do this when there is a collision
                score += 500;
                vx = -vx;
                vy = -vy;

            }
        }
        break;
        // target
        case 2: {


            if(p.y < currentPosition.y + 7 && (currentPosition.x > p.x && currentPosition.x < p.x + 20))
            {
                score += 1000;
                vy = -vy;
            }
        }
        break;
        // top wall
        case 3: {
            if(p.y < currentPosition.y + 7)
            {
                vy = -vy;
            }
        }
        break;

        // side walls
        case 4: {
            if((p.x + 10 < currentPosition.x - 7 || p.x > currentPosition.x + 7) && currentPosition.y < 110)
            {
                // do this when there is collision with side of wall

                vx = -vx;
            }
        }
        break;

        // upper side wall
        case 5: {
            if(p.x < currentPosition.x + 7 && p.y + 400 > currentPosition.y)
            {
                // collision with side of wall
                vx = -vx;
            }
        }
        break;

        // outher side wall
        case 6: {
            if(p.x + 10 < currentPosition.x - 7 || p.x > currentPosition.x + 7)
            {
                // do this when there is collision with side of wall
                vx = -vx;
            }
            else if(p.y + 440 < currentPosition.y - 7 || p.y > currentPosition.y + 7)
            {
                // do this when there is collision with top of wall
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

        // circle collision
        case 8: {
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



    }

}

/**
    Draws the hole in which the player looses one life

    @param p Point in which hole is located
    @param radius of the hole to draw
    @param c Color structure containing r,g,b
*/
void drawHole(Point& p, float radius, Color& c ) {

	float delta_theta = 0.03;
	glColor3f(c.r, c.g, c.b);
	glBegin( GL_POLYGON );
	for( float angle = 0; angle < 2*PI; angle += delta_theta )
		glVertex3f( radius*cos(angle)+p.x, radius*sin(angle)+p.y, 0 );
	glEnd();
}

/**
    Function that draws the target that gives 1000 score

    @param p Point in which the target is defined
    @param height of the target
    @param width of the target
    @param c Color to draw the target
*/
void drawTarget(Point& p, float height, float width, Color& c ) {

	glColor3f(c.r, c.g, c.b);
	glBegin( GL_POLYGON );
	glVertex3f( p.x, p.y, 0 );
	glVertex3f( p.x+width, p.y, 0 );
	glVertex3f( p.x+width, p.y+height, 0 );
	glVertex3f( p.x, p.y+height, 0 );
	glEnd();
}

/**
    Method used to draw every triangle of the game

    @param p Point in which the rectangle should be
    @param height of the rectangle
    @param width of the rectangle
    @param c Color structure in the form of r,g,b
*/
void drawRectangle(Point& p, float height, float width, Color& c ) {

	glColor3f(c.r, c.g, c.b);
	glBegin( GL_POLYGON );
	glVertex3f( p.x, p.y, 0 );
	glVertex3f( p.x+width, p.y, 0 );
	glVertex3f( p.x+width, p.y+height, 0 );
	glVertex3f( p.x, p.y+height, 0 );
	glEnd();
}

/**
    Function used to draw any wall given point and sizes

    @param p Point in which the rectangle should be
    @param height of the rectangle
    @param width of the rectangle
    @param c Color structure in the form of r,g,b
*/
void drawWall(Point& p, float height, float width, Color& c ) {

	// draw rectangle
	glColor3f(c.r, c.g, c.b);
	glBegin( GL_POLYGON );
	glVertex3f( p.x, p.y, 0 );
	glVertex3f( p.x+width, p.y, 0 );
	glVertex3f( p.x+width, p.y+height, 0 );
	glVertex3f( p.x, p.y+height, 0 );
	glVertex3f( p.x, p.y, 0 );
	glEnd();
}

/**
    Function that draws a given flipper

    @param c Color structure containing r,g,b
    @param f Flipper structure containing the values of the flipper.
*/
void drawFlipper( Color& c, Flipper& f ) {

    //define flipper color
    glColor3f(c.r, c.g, c.b);

	glBegin(GL_POLYGON);
        //define vertices as the flipper's position
        glVertex2f(f.backLeft.x, f.backLeft.y);
        glVertex2f(f.backRight.x, f.backRight.y);
        glVertex2f(f.frontRight.x, f.frontRight.y);
        glVertex2f(f.frontTop.x, f.frontTop.y);
        glVertex2f(f.frontLeft.x, f.frontLeft.y);
    glEnd();

}

/**
    Function used as Display. Called every time there
    is a change that needs to be displayed
*/
void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT);




	drawRectangle(origin, 450, 300, dRed);
	//draw flippers
	drawFlipper(white, lFlipper);
	drawFlipper(white, rFlipper);

	// obstacles
	drawBumper(bumper1,15,gold); // bumper1
	drawBumper(bumper2,15,gold); // bumper2
	drawTarget(target1, 5, 20, green); // target1
	drawTarget(target2, 5, 20, green); // target2
	drawHole(hole1,10,black); // hole1
	drawHole(hole2,10,black); // hole2
	drawCircle(currentPosition,circleRadius,grey); // the pinball
	drawWall(upperWall,10,300,red); // top wall
	drawWall(leftWall,440,10,red); // left playspace wall
	drawWall(holeWallLeft,110,10,red); // left hole wall
	drawWall(holeWallRight,110,10,red); // right hole wall
	drawWall(rightWall,400,10,red); // right playspace wall
	drawWall(farRightWall,440,290,red); // far right wall
	drawRectangle(insertWall,10,50,red); // wall under ball in starting position

    checkColission(bumper1, 1);
	checkColission(bumper2, 1);
	checkColission(target1, 2);
	checkColission(target2, 2);
    checkColission(upperWall,3);
    checkColission(holeWallLeft,4);
	checkColission(holeWallRight,4);
    checkColission(rightWall, 5);
    checkColission(leftWall, 6);
    checkColission(farRightWall, 6);
    checkColission(hole1, 7);
    checkColission(hole2, 7);
    checkColission(currentPosition, 8);


	// plunger mechanism
	glBegin(GL_POLYGON);
		glColor3f(white.r,white.g, white.b);
		glVertex2f(270,60);
		glVertex2f(290,60);
		if(dragging)
		{
			launch = 0;
			acc += .5;
			glVertex2f(290, screenHeight - mouseY);
			glVertex2f(270, screenHeight - mouseY);
		}
		else
		{
			launch = acc;
			glVertex2f(290,40);
			glVertex2f(270,40);
		}
	glEnd();
	// angled wall for insertion
	glBegin(GL_POLYGON);
		glColor3f(red.r, red.g, red.b);
		glVertex2f(270,440);
		glVertex2f(290,440);
		glVertex2f(290,420);
	glEnd();

	stringstream ss;
	ss << "Score: " << score;
	output(10,460,white,ss.str().c_str());

	stringstream bs;
	bs << "Balls: " << ballCounter;
	output(200,460,white,bs.str().c_str());

	glutSwapBuffers();
}

/**
    Function to be used when there is nothing going on


*/
void idle (void)
{
    //  Call display function (draw the current frame)
    glutPostRedisplay ();
}
/**
    Function used to initialize all of the positions
    of the flippers
*/
void flipperPositions(void){
    //right flipper positions
        rFlipper.backLeft.x = 230;
        rFlipper.backLeft.y = 50;

        rFlipper.backRight.x = 230 - sin((0+angleR)*(PI/180.0))*10;
        rFlipper.backRight.y = 50 + cos((0+angleR)*(PI/180.0))*10;

        rFlipper.frontRight.x = 230 - sin((82.875+angleR)*(PI/180))*80.6226;
        rFlipper.frontRight.y = 50 + cos((82.875+angleR)*(PI/180))*80.6226;

        rFlipper.frontTop.x = 230 - sin((86.63+angleR)*(PI/180))*85.1469;
        rFlipper.frontTop.y = 50 + cos((86.63+angleR)*(PI/180))*85.1469;

        rFlipper.frontLeft.x = 230 - sin((90+angleR)*(PI/180.0))*80;
        rFlipper.frontLeft.y = 50 + cos((90+angleR)*(PI/180.0))*80;

    //left flipper positions
        lFlipper.backLeft.x = 40;
        lFlipper.backLeft.y = 50;

        lFlipper.backRight.x = 40 + sin((0+angleL)*(PI/180.0))*10;
        lFlipper.backRight.y = 50 + cos((0+angleL)*(PI/180.0))*10;

        lFlipper.frontRight.x = 40 + sin((82.875+angleL)*(PI/180))*80.6226;
        lFlipper.frontRight.y = 50 + cos((82.875+angleL)*(PI/180))*80.6226;

        lFlipper.frontTop.x = 40 + sin((86.63+angleL)*(PI/180))*85.1469;
        lFlipper.frontTop.y = 50 + cos((86.63+angleL)*(PI/180))*85.1469;

        lFlipper.frontLeft.x = 40 + sin((90+angleL)*(PI/180.0))*80;
        lFlipper.frontLeft.y = 50 + cos((90+angleL)*(PI/180.0))*80;
}

/**
    Function used to initialize OpenGL, GLU and
    global variables used in the code
*/
void init(void)
{
    red.r = 1.0f; red.g = 0.0f; red.b = 0.0f;
    grey.r = 0.827451; grey.g = 0.827451; grey.b = 0.827451;
    dRed.r = 0.545098; dRed.g = 0; dRed.b = 0;
    gold.r = 0.721569; gold.g = 0.52549; gold.b = 0.0431373;
    black.r = 0.0; black.g = 0.0; black.b = 0.0;
    blue.r = 0.372549; blue.g = 0.619608; blue.b = 0.627451;
    green.r = 0.180392; green.g = 0.545098; green.b = 0.341176;
    white.r = 1.0f; white.g = 1.0f; white.b = 1.0f;

    // POINT LOCATIONS
	origin.x = 0; origin.y = 0;

	// flippers
	leftFlipper.x = 0;	leftFlipper.y = 0;
	rightFlipper.x = 0; rightFlipper.y = 0;

	// walls
	upperWall.x = 0;	upperWall.y = 440;
	leftWall.x = 0;	leftWall.y = 0;
	holeWallLeft.x = 40; holeWallLeft.y = 0;
	holeWallRight.x = 222; holeWallRight.y = 0;
	rightWall.x = 260;	rightWall.y = 0;
	farRightWall.x = 290; farRightWall.y = 0;
	insertWall.x = 260; insertWall.y = 60;

	// obstacles
	bumper1.x = 65; bumper1.y = 270;
	bumper2.x = 210; bumper2.y = 270;
	target1.x = 110; target1.y = 435;
	target2.x = 145; target2.y = 435;
	hole1.x = 25; hole1.y = 10;
	hole2.x = 245; hole2.y = 10;


    glClearColor (0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, 0, screenHeight, 0, 1);
	initialPosition.x = 280;	initialPosition.y = 77;
	currentPosition = initialPosition;
	flipperPositions();
}

/**
    Function used to check if the mouse was pressed

    @param button left mouse button
    @param state
    @param x position of the click
    @param y position of the click
*/
void mousePress( int button, int state, int x, int y) {
	if (button != 0 ) {  // left mouse button pressed?
		mousePressed = 1;

	} else { mousePressed = 0;
		dragging = 0;
	}
}

/**
    Function used to check if the mouse is dragged


    @param x position of the drag
    @param y position of the drag
*/
void mouseDrag( int x, int y) {

	mousePressed = 1;
	dragging = 1;
	mouseX = x;
	mouseY = y;
}

/**
    Function to check the position of the mouse
    at any time.

    @param x position of mouse in x axis at any time
    @param y position of mouse in y axis at any time
*/
void mouseMovement( int x, int y) {

	mousePressed = 0;
	dragging = 0;
	mouseX = x;
	mouseY = y;
}


/**
    Function used to activate any of the flippers

    @param type flipper to check.
            1 = left, 2 = right
*/
void activateFlipper(int type){
    switch (type) {
        case 1: {
            if(lockL != 1){
                activateLFlipper = 1;
            }
        }
        break;

        case 2: {
            if(lockR != 1){
                activateRFlipper = 1;
            }
        }
        break;
    }
}


/**
    Timer function used in this code.

    @param v input parameter if needed
*/
void myTimer(int v){
    if(activateRFlipper == 1){
        flipperPositions();
        lockR = 1;
        clockR++;

        if(clockR < 30){
            angleR -= 1.0;
        } else if(clockR < 60){
            angleR += 1.0;
        } else if(clockR >= 60){
            angleR = 25;
            clockR = 0;
            lockR = 0;
            activateRFlipper = 0;
        }
        glutPostRedisplay();
    }
    if(activateLFlipper == 1){
        flipperPositions();
        lockL = 1;
        clockL++;

        if(clockL < 30){
            angleL -= 1.0;
        } else if(clockL < 60){
            angleL += 1.0;
        } else if(clockL >= 60){
            angleL = 25;
            clockL = 0;
            lockL = 0;
            activateLFlipper = 0;
        }
        glutPostRedisplay();
    }
    glutTimerFunc(5, myTimer, 1);

}
/**
    Keyboard function used in the code.
    If f is pressed, activate left flipper
    If j is pressed, activate right flipper
    If e is pressed, exit

    @param theKey unsigned char representing the key pressed
    @param mouseX position of the mouse in x axis
    @param mouseY position of the mouse in y axis
*/
void myKeyboard(unsigned char theKey, int mouseX, int mouseY)
{
    GLint x = mouseX;
    GLint y = screenHeight - mouseY; // openGL coordinates
    switch (theKey)
    {
    case 'f':
    case 'F':
        activateFlipper(1);
        break;
    case 'j':
    case 'J':
        activateFlipper(2);
        break;
    case 'e':
    case 'E':
    case 27:
          exit(-1);
        //terminate the program
    default:
        break;
    }
    glutPostRedisplay();
}

// main method
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB );
	glutInitWindowSize(screenWidth, screenHeight);
	glutCreateWindow("Pinball");
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutMouseFunc( mousePress );
	glutMotionFunc(mouseDrag);
	glutPassiveMotionFunc( mouseMovement );
	glutTimerFunc(5, myTimer, 1);
	glutKeyboardFunc(myKeyboard);
	init();
	glutMainLoop();
	return 0;
}
