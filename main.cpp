
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
#include <assert.h>
#include <fstream>
#include "imageloader.h"

using namespace std;

/**
    Converting functions
*/
/*
//Converts a four-character array to an integer, using little-endian form
    int toInt(const char* bytes)
    {
        return (int)(((unsigned char)bytes[3] << 24) |
                     ((unsigned char)bytes[2] << 16) |
                     ((unsigned char)bytes[1] << 8) |
                     (unsigned char)bytes[0]);
    }

//Converts a two-character array to a short, using little-endian form
    short toShort(const char* bytes)
    {
        return (short)(((unsigned char)bytes[1] << 8) |
                       (unsigned char)bytes[0]);
    }

//Reads the next four bytes as an integer, using little-endian form
    int readInt(ifstream &input)
    {
        char buffer[4];
        input.read(buffer, 4);
        return toInt(buffer);
    }

//Reads the next two bytes as a short, using little-endian form
    short readShort(ifstream &input)
    {
        char buffer[2];
        input.read(buffer, 2);
        return toShort(buffer);
    }

//Just like auto_ptr, but for arrays
    template<class T>
    class auto_array
    {
    private:
        T* array;
        mutable bool isReleased;
    public:
        explicit auto_array(T* array_ = NULL) :
                array(array_), isReleased(false)
        {
        }

        auto_array(const auto_array<T> &aarray)
        {
            array = aarray.array;
            isReleased = aarray.isReleased;
            aarray.isReleased = true;
        }

        ~auto_array()
        {
            if (!isReleased && array != NULL)
            {
                delete[] array;
            }
        }

        T* get() const
        {
            return array;
        }

        T &operator*() const
        {
            return *array;
        }

        void operator=(const auto_array<T> &aarray)
        {
            if (!isReleased && array != NULL)
            {
                delete[] array;
            }
            array = aarray.array;
            isReleased = aarray.isReleased;
            aarray.isReleased = true;
        }

        T* operator->() const
        {
            return array;
        }

        T* release()
        {
            isReleased = true;
            return array;
        }

        void reset(T* array_ = NULL)
        {
            if (!isReleased && array != NULL)
            {
                delete[] array;
            }
            array = array_;
        }

        T* operator+(int i)
        {
            return array + i;
        }

        T &operator[](int i)
        {
            return array[i];
        }
    };

/**
Image* loadBMP(const char* filename)
{
    ifstream input;
    input.open(filename, ifstream::binary);
    assert(!input.fail() || !"Could not find file");
    char buffer[2];
    input.read(buffer, 2);
    assert(buffer[0] == 'B' && buffer[1] == 'M' || !"Not a bitmap file");
    input.ignore(8);
    int dataOffset = readInt(input);

    //Read the header
    int headerSize = readInt(input);
    int width;
    int height;
    switch (headerSize)
    {
    case 40:
        //V3
        width = readInt(input);
        height = readInt(input);
        input.ignore(2);
        assert(readShort(input) == 24 || !"Image is not 24 bits per pixel");
        assert(readShort(input) == 0 || !"Image is compressed");
        break;
    case 12:
        //OS/2 V1
        width = readShort(input);
        height = readShort(input);
        input.ignore(2);
        assert(readShort(input) == 24 || !"Image is not 24 bits per pixel");
        break;
    case 64:
        //OS/2 V2
        assert(!"Can't load OS/2 V2 bitmaps");
        break;
    case 108:
        //Windows V4
        assert(!"Can't load Windows V4 bitmaps");
        break;
    case 124:
        //Windows V5
        assert(!"Can't load Windows V5 bitmaps");
        break;
    default:
        assert(!"Unknown bitmap format");
    }

    //Read the data
    int bytesPerRow = ((width * 3 + 3) / 4) * 4 - (width * 3 % 4);
    int size = bytesPerRow * height;
    auto_array<char> pixels(new char[size]);
    input.seekg(dataOffset, ios_base::beg);
    input.read(pixels.get(), size);

    //Get the data into the right format
    auto_array<char> pixels2(new char[width * height * 3]);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            for (int c = 0; c < 3; c++)
            {
                pixels2[3 * (width * y + x) + c] =
                    pixels[bytesPerRow * y + 3 * x + (2 - c)];
            }
        }
    }

    input.close();
    return new Image(pixels2.release(), width, height);
}
*/
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

struct Material {
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat shininess[1];
};
// Colors
Color red, grey, dRed, gold, black, blue, green, white;

// Materials
Material bumperMaterial, pinballMaterial, holeMaterial, wallMaterial;

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

// Textures
//static GLuint texName[36];

// physics variables
float g = .0002;
float f = .00001;
float initVelX = 0;
float currVelX = 0;
float initVelY = 0;
float currVelY = 0;
bool isMoving = false;
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


void initializeMaterials() {
        // gris
    bumperMaterial.ambient[0] = 0.135f;
    bumperMaterial.ambient[1] = 0.2225f;
    bumperMaterial.ambient[2] = 0.1575f;
    bumperMaterial.ambient[3] = 1.0f;
    bumperMaterial.diffuse[0] = 0.54f;
    bumperMaterial.diffuse[1] = 0.89f;
    bumperMaterial.diffuse[2] = 0.63f;
    bumperMaterial.diffuse[3] = 1.0;
    bumperMaterial.specular[0] = 0.316228f;
    bumperMaterial.specular[1] = 0.316228f;
    bumperMaterial.specular[2] = 0.316228f;
    bumperMaterial.specular[3] = 1.0f;
    bumperMaterial.shininess[0] = .1*128.0f;


    pinballMaterial.ambient[0] = 0.19225f;
    pinballMaterial.ambient[1] = 0.19225f;
    pinballMaterial.ambient[2] = 0.19225f;
    pinballMaterial.ambient[3] = 1.0f;
    pinballMaterial.diffuse[0] = 0.50754f;
    pinballMaterial.diffuse[1] = 0.50754f;
    pinballMaterial.diffuse[2] = 0.50754f;
    pinballMaterial.diffuse[3] = 1.0;
    pinballMaterial.specular[0] = 0.508273f;
    pinballMaterial.specular[1] = 0.508273f;
    pinballMaterial.specular[2] = 0.508273f;
    pinballMaterial.specular[3] = 1.0f;
    pinballMaterial.shininess[0] = 51.2f;

    holeMaterial.ambient[0] = 0.02f;
    holeMaterial.ambient[1] = 0.02f;
    holeMaterial.ambient[2] = 0.02f;
    holeMaterial.ambient[3] = 1.0f;
    holeMaterial.diffuse[0] = 0.01f;
    holeMaterial.diffuse[1] = 0.01f;
    holeMaterial.diffuse[2] = 0.01f;
    holeMaterial.diffuse[3] = 1.0;
    holeMaterial.specular[0] = 0.4f;
    holeMaterial.specular[1] = 0.4f;
    holeMaterial.specular[2] = 0.4f;
    holeMaterial.specular[3] = 1.0f;
    holeMaterial.shininess[0] = 10.0f;

    wallMaterial.ambient[0] = 0.1745f;
    wallMaterial.ambient[1] = 0.01175f;
    wallMaterial.ambient[2] = 0.01175f;
    wallMaterial.ambient[3] = 1.0f;
    wallMaterial.diffuse[0] = 0.61424f;
    wallMaterial.diffuse[1] = 0.04136f;
    wallMaterial.diffuse[2] = 0.04136f;
    wallMaterial.diffuse[3] = 1.0;
    wallMaterial.specular[0] = 0.727811f;
    wallMaterial.specular[1] = 0.626959f;
    wallMaterial.specular[2] = 0.626959f;
    wallMaterial.specular[3] = 1.0f;
    wallMaterial.shininess[0] = 76.8f;

};
void changeMaterial(Material& material ) {

    glMaterialfv(GL_FRONT,GL_AMBIENT,material.ambient);
    glMaterialfv(GL_FRONT,GL_DIFFUSE,material.diffuse);
    glMaterialfv(GL_FRONT,GL_SPECULAR,material.specular);
    glMaterialfv(GL_FRONT,GL_SHININESS,material.shininess);
}



/**
    Function that draws the circle in the screen given its position

    @param p point in where the circle should be draw
    @param radius of the circle to draw
    @param c Color struct to paint the circle

*/
void drawCircle(Point& p, float radius, Color& c ) {

    glPushMatrix();

    changeMaterial(pinballMaterial);
    glScaled(1,1,0.1);
    glTranslated(p.x, p.y, 0);
    glutSolidSphere(radius,15,15);
	glPopMatrix();
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

    glPushMatrix();

    changeMaterial(bumperMaterial);
    glScaled(1,0.5,0.1);
    glTranslated(p.x, p.y, 0);
    glutSolidSphere(radius,40,40);
	glPopMatrix();

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
        case 1: { //bumper
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
        case 2: { //target
            if(p.y < currentPosition.y + 7 && (currentPosition.x > p.x && currentPosition.x < p.x + 20))
            {
                score += 1000;
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



    }

}

/**
    Draws the hole in which the player looses one life

    @param p Point in which hole is located
    @param radius of the hole to draw
    @param c Color structure containing r,g,b
*/
void drawHole(Point& p, float radius, Color& c ) {

	glPushMatrix();
    changeMaterial(holeMaterial);
    glScaled(1,1,0.1);
    glTranslated(p.x, p.y, 0);
    glutSolidSphere(radius,15,15);
	glPopMatrix();
	lastPosition = p;
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
    Function used to draw any wall given point and sizes

    @param p Point in which the rectangle should be
    @param height of the rectangle
    @param width of the rectangle
    @param c Color structure in the form of r,g,b
*/
void drawWall(Point& p, float height, float width, Color& c ) {

	// draw rectangle
    glPushMatrix();
    glColor3f(c.r, c.g, c.b);
    changeMaterial(wallMaterial);
    glTranslated(p.x, p.y, 0);
    glScaled(width,height,0.1);
    glutSolidCube(2);
	glPopMatrix();

}

/**
    Function that draws a given flipper

    @param c Color structure containing r,g,b
    @param f Flipper structure containing the values of the flipper.
*/
void drawFlipper( Color& c, Flipper& f ) {

    //define flipper color
    glColor3f(c.r, c.g, c.b);

    //glBindTexture(GL_TEXTURE_2D, texName[0]);
	glBegin(GL_POLYGON);
        //define vertices as the flipper's position
        //glTexCoord2f(0.0f, 0.0f);
        glVertex2f(f.backLeft.x, f.backLeft.y);
        //glTexCoord2f(1.0f, 0.0f);
        glVertex2f(f.backRight.x, f.backRight.y);
        //glTexCoord2f(1.0f, 1.0f);
        glVertex2f(f.frontRight.x, f.frontRight.y);
        glVertex2f(f.frontTop.x, f.frontTop.y);
        //glTexCoord2f(0.0f, 1.0f);
        glVertex2f(f.frontLeft.x, f.frontLeft.y);
    glEnd();

}

/**
    Makes the image into a texture, and returns the id of the texture
*/
/*
void loadTexture(Image* image, int k)
{

    glBindTexture(GL_TEXTURE_2D, texName[k]); //Tell OpenGL which texture to edit

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    //Map the image to the texture
    glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
                 0,                            //0 for now
                 GL_RGB,                       //Format OpenGL uses for image
                 image->width, image->height,  //Width and height
                 0,                            //The border of the image
                 GL_RGB, //GL_RGB, because pixels are stored in RGB format
                 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
                 //as unsigned numbers
                 image->pixels);               //The actual pixel data

}
*/

/**
    Function that preloads textures
*/
/*
void initRendering(){
    Image* image;
    glEnable(GL_TEXTURE_2D);

    glGenTextures(36, texName); //Make room for our texture
    image = loadBMP("C:/Users/SergioJesúsCorderoBa/Documents/ITESM/graficos/12a Semana GC/12a Semana GC/ojo.bmp");
    loadTexture(image,0);
    delete image;

}
*/

/**
    Function used as Display. Called every time there
    is a change that needs to be displayed
*/
void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);

	//drawRectangle(origin, 450, 300, dRed);
   glPushMatrix();
 //  glRotated(10,0,1,0);

	//draw flippers
	drawFlipper(white, lFlipper);
	drawFlipper(white, rFlipper);

	// obstacles

	drawBumper(bumper1,15,gold); // bumper1
	drawBumper(bumper2,15,gold); // bumper2glColor3f (1.0, 1.0, 1.0);

	drawTarget(target1, 5, 20, green); // target1
	drawTarget(target2, 5, 20, green); // target2
	drawHole(hole1,10,black); // hole1
	drawHole(hole2,10,black); // hole2
	drawCircle(currentPosition,circleRadius,grey); // the pinball


	drawWall(upperWall,10,600,red); // top wall
	drawWall(leftWall,440,10,red); // left playspace wall
	drawWall(holeWallLeft,110,10,red); // left hole wall
	drawWall(holeWallRight,110,10,red); // right hole wall
	drawWall(rightWall,400,10,red); // right playspace wall
	drawWall(farRightWall,440,5,red); // right playspace wall
	drawWall(insertWall,10,20,red); // wall under ball in starting position


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


    glColor3f(red.r, red.g, red.b);
    changeMaterial(wallMaterial);
	glBegin(GL_TRIANGLES);

        glVertex3f(270, 440, 1.0);
        glVertex3f(290, 440, 1.0);
        glVertex3f(290, 420, 1.0);
    glEnd();

	stringstream ss;
	ss << "Score: " << score;
	output(10,460,white,ss.str().c_str());

	stringstream bs;
	bs << "Balls: " << ballCounter;
	output(200,460,white,bs.str().c_str());

glPopMatrix();
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
	farRightWall.x = 295; farRightWall.y = 0;
	insertWall.x = 270; insertWall.y = 60;

	// obstacles
	bumper1.x = 65; bumper1.y = 270;
	bumper2.x = 210; bumper2.y = 270;
	target1.x = 110; target1.y = 435;
	target2.x = 145; target2.y = 435;
	hole1.x = 20; hole1.y = 10;
	hole2.x = 240; hole2.y = 10;


    initializeMaterials();

    glFrontFace(GL_CW);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);


    // asigna la apropiada fuente de luz
   GLfloat lightIntensity[] = {0.7f, 0.7f, 0.7f, 1.0f};
    //GLfloat light_position[] = {1.0f, 1.0f, 3.0f, 0.0f};
    GLfloat light_position[] = {1.0f, 1.0f, 3.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION,light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,lightIntensity);

    glClearColor (0.5, 0.5, 0.5, 1.0);



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
    Ball physics.

    @param v input parameter if needed
*/
void gravity(int v){
    if(isMoving){
        currVelY -= 0.01;
    } else {
        currVelY = 0.0;
    }
    currentPosition.y += currVelY;
    glutTimerFunc(2, gravity, 1);
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


void reshape(int ancho, int alto) {
    glViewport(0,0,ancho, alto);

    //double winHt = 1.0;//altura mitad de la ventana. Probar con valores 0.5, 0.25, 0.125
    //glOrtho(-winHt*64/48.0, winHt*64/48.0, -winHt, winHt, 0.1,100.0);


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
   // glOrtho(0, screenWidth, 0, screenHeight, 5, 200);
    glFrustum(0, screenWidth, 0, screenHeight, 6, 200);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(.01,.01,7, //eye
              0,0,0,  //center
              0,1,7);//up


}

// main method
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screenWidth, screenHeight);
	glutCreateWindow("Pinball");
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutMouseFunc( mousePress );
	glutMotionFunc(mouseDrag);
	glutReshapeFunc(reshape);
	glutPassiveMotionFunc( mouseMovement );
	glutTimerFunc(5, myTimer, 1);
	glutTimerFunc(2, gravity, 1);
	glutKeyboardFunc(myKeyboard);
	init();
	glutMainLoop();
	return 0;
}
