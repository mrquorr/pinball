/*
 * Much thanks to Github user Skatrud, whos pinball
 * code gave us the base for this app.
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
 
using namespace::std;

/// window stuff
int window; // main window id
const int BALL_SIZE = 7;
const float PI = 22.0/7.0;
const int ANIMATION_TIME = 20;
const int screenHeight = 500, screenWidth = 300;

// physics variables
float g = .00002;
float f = .00001;
float initVelX = 0;
float currVelX = 0;
float initVelY = 0;
float currVelY = 0;
float vx = 0;
float vy = 0;

// mouse variables
int dragging = 0;
int mousePressed = 0;
int mouseX = 0;
int mouseY = 0;

// Misc.
int ballCounter = 5;
float acc = 0;
float launch = 0;
int score = 0;

// keyboard variables
int lFlipper = 0;
int rFlipper = 0;

// colour struct for rgba colours
struct Color {
	float r;
	float g;
	float b;
	float a;
};

// point struct
struct Point {
	float x;
	float y;
	float z;
};

// Vector struct
struct Vector {
	float angle;
	float speed;
};

// Points
Point origin;
Point leftFlipper;
Point rightFlipper;
Point upperWall;
Point leftWall;
Point holeWallLeft;
Point rightWall;
Point farRightWall;
Point holeWallRight;
Point insertWall;
Point bumper1;
Point bumper2;
Point target1;
Point target2;
Point hole1;
Point hole2;

Point leftLFlipperState;
Point leftRFlipperState;
Point rightLFlipperState;
Point rightRFlipperState;

// Ball Points
Point currentPosition;
Point initialPosition;
Point lastPosition;

// Vector
Vector current;
Vector initial;

// flipper variables
float angle = 0.0;
const float ANGLE_CHANGE = 45;

// draw text
void output(int x, int y, float r, float g, float b, const char *string)
{
  glColor3f( r, g, b );
  glRasterPos2f(x, y);
  int len, i;
  len = (int)strlen(string);
  for (i = 0; i < len; i++) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
  }
}

// draw the Pinball
void drawCircle(Point& p, float radius, Color& c ) {
	float delta_theta = 0.01;
	glColor3f(c.r, c.g, c.b);
	glBegin( GL_POLYGON );
	for( float angle = 0; angle < 2*PI; angle += delta_theta )
		glVertex3f( radius*cos(angle)+p.x, radius*sin(angle)+p.y, 0 );
	glEnd();

	//cout << "y coordinate: " << p.y << endl;
	//cout << "x coordinate: " << p.x << endl;

    //ball falls
	lastPosition = p;
	if((p.y < 1 || p.x > 300 || p.y > 440) && ballCounter > 0)
	{
		ballCounter--;
		p.x = 280;
		p.y = 77;
		vx = -vx;
		vy = -vy;
	}

	if(p.x > 262)
	{
		if(p.y >= 420)
		{
			vx = vy;
			p.x -= vx;
		}
		else
		{
			vy = 0;
			vy += (launch/2000);
			p.y += vy;
		}
	}
	else
	{
		acc = 0;
		p.x -= vx;
		if(vy > 0){
			vy += g;
			vy -= f;
		}
		else if(vy < 0){
			vy -= g;
			vy += f;
		}
		else if(vx > 0){
			vx -= f;
		}
		else if(vx < 0){
			vx += f;
		}
		p.y -= vy;
	}
}

// draws a bumper obstacle
void drawBumper(Point& p, float radius, Color& c ) {
	// Collision code
	float d = sqrt((pow(p.x - currentPosition.x, 2)) + (pow(p.y - currentPosition.y, 2)));
	if(d <= 22)
	{
		// do this when there is a collision
		score += 500;
		vx = -vx;
		vy = -vy;
	}

	// draw circle
	float delta_theta = 0.01;
	glColor3f(c.r, c.g, c.b);
	glBegin( GL_POLYGON );
	for( float angle = 0; angle < 2*PI; angle += delta_theta )
		glVertex3f( radius*cos(angle)+p.x, radius*sin(angle)+p.y, 0 );
	glEnd();
}

// draws a pitfall obstacle
void drawHole(Point& p, float radius, Color& c ) {
	// Collision code
	float d = sqrt((pow(p.x - currentPosition.x, 2)) + (pow(p.y - currentPosition.y, 2)));
	if(d <= 22 && ballCounter > 0)
	{
		// do this when there is a collision
		ballCounter--;
		cout << "Ball Counter: " << ballCounter << endl;
		currentPosition.x = 280;
		currentPosition.y = 77;
		vx = -vx;
		vy = -vy;
	}

	// draw circle
	float delta_theta = 0.01;
	glColor3f(c.r, c.g, c.b);
	glBegin( GL_POLYGON );
	for( float angle = 0; angle < 2*PI; angle += delta_theta )
		glVertex3f( radius*cos(angle)+p.x, radius*sin(angle)+p.y, 0 );
	glEnd();
}

// draws a target obstacle
void drawTarget(Point& p, float height, float width, Color& c ) {
	if(p.y < currentPosition.y + 7 && (currentPosition.x > p.x && currentPosition.x < p.x + 20))
	{
		score += 1000;
		vy = -vy;
	}
	glColor3f(c.r, c.g, c.b);
	glBegin( GL_POLYGON );
	glVertex3f( p.x, p.y, 0 );
	glVertex3f( p.x+width, p.y, 0 );
	glVertex3f( p.x+width, p.y+height, 0 );
	glVertex3f( p.x, p.y+height, 0 );
	glVertex3f( p.x, p.y, 0 );
	glEnd();
}

// draws a rectangle
void drawRectangle(Point& p, float height, float width, Color& c ) {
	glColor3f(c.r, c.g, c.b);
	glBegin( GL_POLYGON );
	glVertex3f( p.x, p.y, 0 );
	glVertex3f( p.x+width, p.y, 0 );
	glVertex3f( p.x+width, p.y+height, 0 );
	glVertex3f( p.x, p.y+height, 0 );
	glVertex3f( p.x, p.y, 0 );
	glEnd();
}

// Outer side walls
void drawWallS(Point& p, float height, float width, Color& c ) {
	// handle collision
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

// Inner Side Walls
void drawWallSM(Point& p, float height, float width, Color& c ) {
	// handle collision
	if((p.x + 10 < currentPosition.x - 7 || p.x > currentPosition.x + 7) && currentPosition.y < 110)
	{
		// do this when there is collision with side of wall
		vx = -vx;
	}

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

// Insert Side Wall
void drawWallIS(Point& p, float height, float width, Color& c ) {
	// handle collision
	if(p.x < currentPosition.x + 7 && p.y + 400 > currentPosition.y)
	{
		// collision with side of wall
		vx = -vx;
	}

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

// Top Wall
void drawWallTB(Point& p, float height, float width, Color& c ) {
	// handle collision
	if(p.y < currentPosition.y + 7)
	{
		vy = -vy;
	}

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

// Flipper
void drawFlipper( Point& p, float segmentWidth, float segmentHeight, Color& c ) {
	Point current;
	current.x = p.x+segmentHeight/2;
	current.y = p.y;
	drawRectangle(current, segmentHeight, segmentWidth-segmentHeight/2, c);
}

// display function
void display(void)
{
	glutSetWindow(window);
	glClear(GL_COLOR_BUFFER_BIT);
	float ld;
	float rd;

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

	// COLOR
	// red
	Color red;
	red.r = 1.0f; red.g = 0.0f; red.b = 0.0f;

	// grey
	Color grey;
	grey.r = 0.827451; grey.g = 0.827451; grey.b = 0.827451;

	// dark red
	Color dRed;
	dRed.r = 0.545098; dRed.g = 0; dRed.b = 0;

	// gold
	Color gold;
	gold.r = 0.721569; gold.g = 0.52549; gold.b = 0.0431373;

	// black
	Color black;
	black.r = 0.0; black.g = 0.0; black.b = 0.0;

	// blue
	Color blue;
	blue.r = 0.372549; blue.g = 0.619608; blue.b = 0.627451;

	// green
	Color green;
	green.r = 0.180392; green.g = 0.545098; green.b = 0.341176;

	// DRAWING ALL OBJECTS
	// background
	drawRectangle(origin, 450, 300, dRed);

	// Left Flipper
	glPushMatrix();
	glTranslatef(40,55,0);
	glRotatef(-25,0,0,1);

	if(GetAsyncKeyState(0x5A)&0x8000 && angle < 45.0)
	{
		glRotatef(ANGLE_CHANGE,0,0,1);  // rotate n degrees around z axis

		leftRFlipperState.x = 105;
		leftRFlipperState.y = 95;

		leftLFlipperState.x = 50;
		leftLFlipperState.y = 75;

		ld = abs((4 * currentPosition.x) + (-11 * currentPosition.y) + 625) / 11.70469991;
	}
	else
	{
		if(angle > 0.0){
			glRotatef(-ANGLE_CHANGE,0,0,1);  // rotate n degrees around z axis
		}

		leftRFlipperState.x = 112;
		leftRFlipperState.y = 37;

		leftLFlipperState.x = 50;
		leftLFlipperState.y = 65;

		ld = abs((14 * currentPosition.x) + (31 * currentPosition.y) - 2715) / 34.0147027;
	}

	drawFlipper(leftFlipper, 75, 15, blue); // draw the flipper
	glPopMatrix();

	if(ld <= 15 && (currentPosition.x >= 50 && currentPosition.x <= 112))
	{
		vy = -vy;
	}

	// Right Flipper
	glPushMatrix();
	glTranslatef(220,50,0);
	glRotatef(25,0,0,1);

	if(GetAsyncKeyState(0xBF)&0x8000 && angle < 45.0)
	{
		glRotatef(-ANGLE_CHANGE,0,0,1);  // rotate n degrees around z axis

		rightLFlipperState.x = 170;
		rightLFlipperState.y = 85;

		rightRFlipperState.x = 220;
		rightRFlipperState.y = 67;

		rd = abs((9 * currentPosition.x) + (25 * currentPosition.y) - 3655) / 26.57066051;
	}
	else
	{
		if(angle > 0.0){
			glRotatef(ANGLE_CHANGE,0,0,1);  // rotate n degrees around z axis
		}
		rightLFlipperState.x = 160;
		rightLFlipperState.y = 37;

		rightRFlipperState.x = 220;
		rightRFlipperState.y = 67;

		rd = abs((1 * currentPosition.x) + (-2 * currentPosition.y) - 86) / 2.236067977;
	}

	drawFlipper(rightFlipper, -60, 15, blue);
	glPopMatrix();

	if(rd <= 15 && (currentPosition.x >= 160 && currentPosition.x <= 220))
	{
		vy = -vy;
	}

	// obstacles
	drawBumper(bumper1,15,gold); // bumper1
	drawBumper(bumper2,15,gold); // bumper2
	drawTarget(target1, 5, 20, green); // target1
	drawTarget(target2, 5, 20, green); // target2
	drawHole(hole1,10,black); // hole1
	drawHole(hole2,10,black); // hole2

	// ball
	drawCircle(currentPosition,7,grey); // the pinball
	// walls
	drawWallTB(upperWall,10,300,red); // top wall
	drawWallS(leftWall,440,10,red); // left playspace wall
	drawWallSM(holeWallLeft,110,10,red); // left hole wall
	drawWallSM(holeWallRight,110,10,red); // right hole wall
	drawWallIS(rightWall,400,10,red); // right playspace wall
	drawWallS(farRightWall,440,290,red); // far right wall
	drawRectangle(insertWall,10,50,red); // wall under ball in starting position
	// plunger mechanism
	glBegin(GL_POLYGON);
		glColor3f(1.0,1.0,1.0);
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
	output(10,460,1.0,1.0,1.0,ss.str().c_str());

	stringstream bs;
	bs << "Balls: " << ballCounter;
	output(200,460,1.0,1.0,1.0,bs.str().c_str());

	glutSwapBuffers();
}

void idle (void)
{
    //  Call display function (draw the current frame)
    glutPostRedisplay ();
}

// initializes openGL, glut, and glui
void init(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, 0, screenHeight, 0, 1);
	initialPosition.x = 280;	initialPosition.y = 77;
	currentPosition = initialPosition;
}

// checks mouse button press
void mousePress( int button, int state, int x, int y) {
	if (button != 0 ) {  // left mouse button pressed?
		mousePressed = 1;

	} else { mousePressed = 0;
		dragging = 0;
	}
}

// says whether the mouse is dragging or not
void mouseDrag( int x, int y) {

	mousePressed = 1;
	dragging = 1;
	mouseX = x;
	mouseY = y;
}

// this sets the mouse position in window
void mouseMovement( int x, int y) {

	mousePressed = 0;
	dragging = 0;
	mouseX = x;
	mouseY = y;
}

void activateLeftFlipper(){
    lFlipper = 1;
}

void activateRightFlipper(){
    rFlipper = 1;
}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY)
{
    GLint x = mouseX;
    GLint y = screenHeight - mouseY; // openGL coordinates
    switch (theKey)
    {
    case 'f':
    case 'F':
        activateLeftFlipper();
        break;
    case 'j':
    case 'J':
        activateRightFlipper();
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
	window = glutCreateWindow("Skatrud Pinball");
	glutDisplayFunc(display);
	glutIdleFunc(idle);

	glutMouseFunc( mousePress );
	glutMotionFunc(mouseDrag);
	glutPassiveMotionFunc( mouseMovement );

	init();
	glutMainLoop();
	return 0;
}
