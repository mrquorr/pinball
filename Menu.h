#include <string.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>

using namespace std;

string tipsArray[20] = {"Sin globito no hay fiesta", "Cuidado, el SIDA si da", "Hay condones para mujeres", "El diafragma tapa el camino a los ovarios",
                        "Los D.I.U. ayudan, pero no garantizan", "La del dia no, envez la pildora del mes", "El espermicida solo es como un condon roto",
                        "Hablar sobre sexo, mejor en exceso", "Parches y anillo, asi si te pillo", "Una vasectomia, tu hermana no sera tia",
                        "Lavados vaginales, mejor compra paniales", "Marcha atras, esperen ser papas", "Durante de la menstruacion, lo dejaste en manos del senior",
                        "Pildora del dia, acaba en camilla", "Huele a pescado, dejalo vetado", "Granos y una verruga, mejor pega a la fuga", "Pus amarilla, huye a la esquina",
                        "Globo usado, globo tronado", "Planea tu familia, seras feliz en tus dias", "Checate seguido, no caigas en el descuido"};



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
    Prints start menu screen

    @param x the position of x
    @param y the position of y
    @param color the Color structure to be usded
    @param str character array that wants to be printed

*/
void drawMenu(float x, float y, Color& background, Color& title, Color& words){
    float width = 250;
    float height = 300;
    float wordJump = 30;
    srand (time(NULL));
    int r = rand() % 20;
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
    //string tip = tipsArray[5];
    output(x, y-(5*wordJump), 0.1, "La del dia no, envez la pildora del mes", 0.08);

    glEnable(GL_LIGHTING);
}

/**
    Prints pause screen

    @param x the position of x
    @param y the position of y
    @param color the Color structure to be usded
    @param str character array that wants to be printed

*/
void drawPause(float x, float y, Color& background, Color& title, Color& words){
    float width = 250;
    float height = 300;
    float wordJump = 30;
    srand (time(NULL));
    int r = rand() % 20;
    glDisable(GL_LIGHTING);

    glColor3f(background.r, background.g, background.b);

    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x+width, y);
        glVertex2f(x+width, y-height);
        glVertex2f(x, y-height);
    glEnd();

    glColor3f(title.r, title.g, title.b);
    output(x, y-wordJump, 0.01, "En pausa!", 0.2);

    glColor3f(words.r, words.g, words.b);
    output(x, y-(2*wordJump), 0.1, "Terminar - T", 0.15);
    output(x, y-(3*wordJump), 0.1, "Pausa - P", 0.15);
    output(x, y-(4*wordJump), 0.1, "Iniciar - I", 0.15);

    output(x, y-(5*wordJump), 0.1, "Marcha atras, esperen ser papas", 0.08);

    glEnable(GL_LIGHTING);
}

/**
    Prints given string in the screen

    @param x the position of x
    @param y the position of y
    @param color the Color structure to be usded
    @param str character array that wants to be printed

*/
void drawGameover(float x, float y, Color& background, Color& title, Color& words){
    float width = 250;
    float height = 300;
    float wordJump = 30;
    srand (time(NULL));
    int r = rand() % 20;
    glDisable(GL_LIGHTING);

    glColor3f(background.r, background.g, background.b);

    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x+width, y);
        glVertex2f(x+width, y-height);
        glVertex2f(x, y-height);
    glEnd();

    glColor3f(title.r, title.g, title.b);
    output(x, y-wordJump, 0.01, "FIN DEL JUEGO, TE EMBARAZASTE", 0.1);

    glColor3f(words.r, words.g, words.b);

    if(familyLevel == 0){
        output(x, y-(2*wordJump), 0.1, "No sabes quien es papa! Como le diras a tus padres?", 0.065);
    } else if(familyLevel == 1){
        output(x, y-(2*wordJump), 0.1, "Pero tu novio como que no... Ojala te apoye", 0.067);
    } else if(familyLevel == 2){
        output(x, y-(2*wordJump), 0.1, "Tu novio dice que te apoya! Esperemos que si", 0.067);
    } else if(familyLevel == 3){
        output(x, y-(2*wordJump), 0.1, "Tu esposo se ve feliz, pero sus ojos no...", 0.067);
    } else if(familyLevel == 4){
        output(x, y-(2*wordJump), 0.1, "Tu esposo se ve igual de feliz que tu!", 0.067);
    } else if(familyLevel == 5){
        output(x, y-(2*wordJump), 0.1, "Justo como lo venian planeando, felicidades!", 0.067);
    }

    if(careerLevel == 0){
        output(x, y-(3*wordJump), 0.1, "Hijole, y apunto de graduarte de secundaria...", 0.067);
    } else if(careerLevel == 1){
        output(x, y-(3*wordJump), 0.1, "Bueno, minimo ya estas en la prepa.", 0.067);
    } else if(careerLevel == 2){
        output(x, y-(3*wordJump), 0.1, "Muchas chavas se embarazan en la universidad tambien.", 0.065);
    } else if(careerLevel == 3){
        output(x, y-(3*wordJump), 0.1, "Tienes trabajo. Perdon, tenias.", 0.067);
    } else if(careerLevel == 4){
        output(x, y-(3*wordJump), 0.1, "Tienes la confianza que te esperaran cuando quieras.", 0.067);
    } else if(careerLevel == 5){
        output(x, y-(3*wordJump), 0.1, "Eres duenia de una superempresa y mujer del anio.", 0.067);
    }

    output(x, y-(4*wordJump), 0.1, "Puntaje: 40", 0.15);
    output(x, y-(5*wordJump), 0.1, "Iniciar un nuevo juego - I", 0.1);

    output(x, y-(6*wordJump), 0.1, "Hablar sobre sexo, mejor en exceso", 0.08);

    glEnable(GL_LIGHTING);
}
