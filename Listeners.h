#include <string.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>

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
    Function to start a game
*/
void start(){
    srand (time(NULL));
    int r = rand() % 20;
    if(gameState == 0){
        gameState = 1;
        score = 0;
        ballCounter = 5;
    } else if(gameState == 1 || gameState == 2){
        cout << "Error, action not allowed" << endl;
    } else if(gameState == 3){
        gameState = 1;
        score = 0;
        ballCounter = 5;
    }
}

/**
    Function to end a game
*/
void finish(){
    srand (time(NULL));
    r = rand() % 20;
    if(gameState == 0){
        cout << "Error, action not allowed" << endl;
    } else if(gameState == 1 || gameState == 2){
        gameState = 3;
        ballCounter = 0;
        gameOver = true;
    } else if(gameState == 3){
        cout << "Error, action not allowed" << endl;
    }
}

/**
    Function to pause a game
*/
void pause(){
    srand (time(NULL));
    r = rand() % 20;
    if(gameState == 0){
        cout << "Error, action not allowed" << endl;
    } else if(gameState == 1){
        isPaused = true;
        srand (time(NULL));
        gameState = 2;
    } else if(gameState == 2){
        isPaused = false;
        gameState = 1;
    } else if(gameState == 3){
        cout << "Error, action not allowed" << endl;
    }
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
    case 'T':
    case 't':
        finish();
        break;
    case 'P':
    case 'p':
        pause();
        break;
    case 'I':
    case 'i':
        start();
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
