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