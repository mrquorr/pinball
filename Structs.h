#include <string.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>

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
Material bumperMaterial, pinballMaterial, holeMaterial, wallMaterial, bumper2Material;

// Points
Point origin, leftFlipper, rightFlipper, upperWall, leftWall,holeWallLeft;
Point rightWall, farRightWall, holeWallRight, insertWall, bumper1, bumper2, bumper3, bumper4, target1, target2, hole1, hole2;
Point leftLFlipperState, leftRFlipperState, rightLFlipperState, rightRFlipperState;
Point currentPosition, initialPosition, lastPosition;

// Vectors
Vector current;
Vector initial;

// Flippers
Flipper rFlipper;
Flipper lFlipper;

// Textures
static GLuint texName[2];

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

// game variables
int ballCounter = 3;
int score = 0;
int gameState = 0;
int prevGameState;
bool isPaused = false;
bool gameOver = false;
int familyLevel = 0;
int careerLevel = 0;
int r = 0;
int careerCounter = 0;
int familyCounter = 0;

//flipper values
int activateRFlipper=0;
int activateLFlipper=0;
int clockR = 0;
int clockL = 0;
int lockR = 0;
int lockL = 0;
float angleR = 25.0;
float angleL = 25.0;

void initializeMaterials() {
        // gris
    bumperMaterial.ambient[0] = 0.0f;
    bumperMaterial.ambient[1] = 0.0f;
    bumperMaterial.ambient[2] = 0.0f;
    bumperMaterial.ambient[3] = 1.0f;
    bumperMaterial.diffuse[0] = 0.1f;
    bumperMaterial.diffuse[1] = 0.35f;
    bumperMaterial.diffuse[2] = 0.1f;
    bumperMaterial.diffuse[3] = 1.0;
    bumperMaterial.specular[0] = 0.45f;
    bumperMaterial.specular[1] = 0.55f;
    bumperMaterial.specular[2] = 0.45f;
    bumperMaterial.specular[3] = 1.0f;
    bumperMaterial.shininess[0] = 32.0f;


    // rojo plastico
    bumper2Material.ambient[0] = 0.0f;
    bumper2Material.ambient[1] = 0.0f;
    bumper2Material.ambient[2] = 0.0f;
    bumper2Material.ambient[3] = 1.0f;
    bumper2Material.diffuse[0] = 0.5f;
    bumper2Material.diffuse[1] = 0.0f;
    bumper2Material.diffuse[2] = 0.0f;
    bumper2Material.diffuse[3] = 1.0;
    bumper2Material.specular[0] = 0.7f;
    bumper2Material.specular[1] = 0.6f;
    bumper2Material.specular[2] = 0.6f;
    bumper2Material.specular[3] = 1.0f;
    bumper2Material.shininess[0] = 32.0f;


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

void output(GLfloat x, GLfloat y, GLfloat z, char* text, float siz)
{
    glPushMatrix();
    glTranslatef(x, y, z);
    glScaled(siz, siz, siz);
    for( char* p = text; *p; p++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
    }
    glPopMatrix();
}
