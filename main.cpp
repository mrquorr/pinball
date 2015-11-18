
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
#include <time.h>
#include "Structs.h"
#include "Menu.h"
#include "Draws.h"
#include "Listeners.h"
#include "Collision.h"

using namespace std;

/**
    Function used as Display. Called every time there
    is a change that needs to be displayed
*/
void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	if(gameState == 0){
        drawMenu(25, 400, black, blue, green);
	} else if(gameState == 2){
        drawPause(25, 400, black, red, blue);
    } else if(gameState == 3){
        drawGameover(25, 400, white, red, black);
    }else if(gameState == 1){

        //drawRectangle(origin, 450, 300, dRed);
      glPushMatrix();
      //glRotated(-.04,1,0,0);

        //draw flippers
        drawFlipper(white, lFlipper);
        drawFlipper(white, rFlipper);

        // obstacles
        drawBumperGreen(bumper1,15,gold); // bumper1
        drawBumperGreen(bumper2,15,gold); // bumper2glColor3f (1.0, 1.0, 1.0);
        drawBumperRed(bumper3,15,gold); // bumper2glColor3f (1.0, 1.0, 1.0);
        drawBumperRed(bumper4,15,gold); // bumper2glColor3f (1.0, 1.0, 1.0);

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


        checkColission(bumper1, 1,1);
        checkColission(bumper2, 1,1);
        checkColission(bumper3, 1,2);
        checkColission(bumper4, 1,2);
        checkColission(target1, 2, 0);
        checkColission(target2, 2, 0 );
        checkColission(upperWall,3, 0);
        checkColission(holeWallLeft,4, 0);
        checkColission(holeWallRight,4, 0);
        checkColission(rightWall, 5, 0);
        checkColission(leftWall, 6, 0);
        checkColission(farRightWall, 6, 0);
        checkColission(hole1, 7, 0);
        checkColission(hole2, 7, 0);
        checkColission(currentPosition, 8, 0 );
        checkColission(bumper1, 9, 0);
        checkColission(bumper1, 10, 0);


        // plunger mechanism
        glBegin(GL_POLYGON);
            glColor3f(white.r,white.g, white.b);
            glVertex2f(270,60);
            glVertex2f(290,60);
            if(dragging)
            {
                launch = 0;
                acc += 5;
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
	}
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
    black.r = 0.0001; black.g = 0.0001; black.b = 0.0001;
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
	rightWall.x = 265;	rightWall.y = 0;
	farRightWall.x = 305; farRightWall.y = 0;
	insertWall.x = 280; insertWall.y = 60;

	// obstacles
	bumper1.x = 65; bumper1.y = 270;
	bumper2.x = 210; bumper2.y = 270;
	bumper3.x = 140; bumper3.y = 370;
	bumper4.x = 140; bumper4.y = 200;
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
    GLfloat ambientLight[] = {0.9f, 0.9f, 0.9f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

    glClearColor (0.5, 0.5, 0.5, 1.0);



    glEnable(GL_TEXTURE_2D);

    GLuint i=0;
    glGenTextures(2, texName); //Make room for our texture
    Image* image;
    image = loadBMP("C:\\Users\\Fabiola\\Dropbox\\Tec\\Septimo_semestre\\Graficos Computacionales\\Proyecto Pinball\\pinball\\src\\left.bmp");
    //image = loadBMP("C:\\Users\\SergioJesúsCorderoBa\\Documents\\ITESM\\graficos\\pinball\\src\\left.bmp");
    loadTexture(image,i++);


    image = loadBMP("C:\\Users\\Fabiola\\Dropbox\\Tec\\Septimo_semestre\\Graficos Computacionales\\Proyecto Pinball\\pinball\\src\\heart.bmp");
    //image = loadBMP("C:\\Users\\SergioJesúsCorderoBa\\Documents\\ITESM\\graficos\\pinball\\src\\left.bmp");
    loadTexture(image,i++);



    delete image;

    PlaySound(TEXT("C:\\Users\\Fabiola\\Dropbox\\Tec\\Septimo_semestre\\Graficos Computacionales\\Proyecto Pinball\\pinball\\src\\music.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, 0, screenHeight, 0, 1);
    initialPosition.x = 285;	initialPosition.y = 77;
    currentPosition = initialPosition;

	flipperPositions();
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

    gluLookAt(0.0, 0.0, 6.2, //eye
              0.0, 0.0, 0.0, //center
              0.0, 1.0, 7.0);//up
}

/**
    Ball physics.

    @param v input parameter if needed
*/
void gravity(int v){
    if(isMoving){
        currVelY -= 0.07;
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
