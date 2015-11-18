#include <string.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>

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
    Prints given string in the screen

    @param x the position of x
    @param y the position of y
    @param color the Color structure to be usded
    @param str character array that wants to be printed

*/
void drawMenu(float x, float y, Color& background, Color& title, Color& words){
    float width = 250;
    float height = 300;
    float wordJump = 30;
    glDisable(GL_LIGHTING);

    glColor3f(background.r, background.g, background.b);

    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x+width, y);
        glVertex2f(x+width, y-height);
        glVertex2f(x, y-height);
    glEnd();

    glColor3f(title.r, title.g, title.b);
    output(x, y-wordJump, 0.01, "No te Pinbarazes!", 0.2);

    glColor3f(words.r, words.g, words.b);
    output(x, y-(2*wordJump), 0.1, "Terminar - T", 0.15);
    output(x, y-(3*wordJump), 0.1, "Pausa - P", 0.15);
    output(x, y-(4*wordJump), 0.1, "Iniciar - I", 0.15);

    glEnable(GL_LIGHTING);
}
