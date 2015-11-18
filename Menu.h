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

float width = 250;
float height = 300;
float wordJump = 30;

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
    Print current score
*/
void outputScore (int x, int y, Color &c, int level){
    ostringstream convert;
    convert << score;
    string totalScore = "Puntaje: " + convert.str();
    char tab2[1024];
    strncpy(tab2, totalScore.c_str(), sizeof(tab2));
    tab2[sizeof(tab2) - 1] = 0;

    output(x, y-(level*wordJump), 0.1, tab2, 0.15);
}

/**
    Draw background image
*/
void drawBackground(int x, int y, Color &c){
    glColor3f(c.r, c.g, c.b);

    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x+width, y);
        glVertex2f(x+width, y-height);
        glVertex2f(x, y-height);
    glEnd();
}

/**
    Print current random tip
*/
void outputTip (int x, int y, Color &c, int level){
    char tip[1024];
    strncpy(tip, tipsArray[r].c_str(), sizeof(tip));
    tip[sizeof(tip) - 1] = 0;

    output(x, y-(level*wordJump), 0.1, tip, 0.065);
}

/**
    Prints start menu screen

    @param x the position of x
    @param y the position of y
    @param color the Color structure to be usded
    @param str character array that wants to be printed

*/
void drawMenu(float x, float y, Color& background, Color& title, Color& words){
    glDisable(GL_LIGHTING);

    drawBackground(x, y, background);

    glColor3f(title.r, title.g, title.b);
    output(x, y-wordJump, 0.01, "No te Pinbarazes!", 0.2);

    glColor3f(words.r, words.g, words.b);
    output(x, y-(4*wordJump), 0.1, "Iniciar - I", 0.15);
    output(x, y-(5*wordJump), 0.1, "Instrucciones - O", 0.15);

    outputTip(x, y, words, 6);

    output(x, y-(8*wordJump), 0.1, "Sergio Cordero A01191167", 0.08);
    output(x, y-(9*wordJump), 0.1, "Emilio Flores A01035087", 0.08);

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
    glDisable(GL_LIGHTING);

    drawBackground(x, y, background);

    glColor3f(title.r, title.g, title.b);
    output(x, y-wordJump, 0.01, "En pausa!", 0.2);

    glColor3f(words.r, words.g, words.b);
    output(x, y-(2*wordJump), 0.1, "Terminar - T", 0.15);
    output(x, y-(3*wordJump), 0.1, "Pausa - P", 0.15);
    output(x, y-(4*wordJump), 0.1, "Iniciar - I", 0.15);
    output(x, y-(5*wordJump), 0.1, "Como jugar - O", 0.15);

    outputTip(x, y, words, 6);
    outputScore(x, y, words, 8);

    glEnable(GL_LIGHTING);
}

/**
    Prints information screen

    @param x the position of x
    @param y the position of y
    @param color the Color structure to be usded
    @param str character array that wants to be printed

*/
void drawInformation(float x, float y, Color& background, Color& title, Color& words){
    glDisable(GL_LIGHTING);

    drawBackground(x, y, background);

    glColor3f(title.r, title.g, title.b);
    output(x, y-wordJump, 0.01, "Como jugar", 0.2);

    glColor3f(words.r, words.g, words.b);
    output(x, y-(2*wordJump), 0.1, "Pantalla anterior - O", 0.07);
    output(x, y-((2+0.5)*wordJump), 0.1, "Pausa - P", 0.07);
    output(x, y-(3*wordJump), 0.1, "Iniciar - I", 0.07);
    output(x, y-((3+0.5)*wordJump), 0.1, "Cerrar ventana - E", 0.07);
    output(x, y-(4*wordJump), 0.1, "Terminar - T", 0.07);

    float siz = 0.06;
    output(x, y-(5*wordJump), 0.1, "En \"No te pinbarazes!\ el objetivo es desarrollar tu vida", siz);
    output(x, y-((5+0.5)*wordJump), 0.1, "lo mas que puedas antes de embarazarte. La pelota son tus", siz);
    output(x, y-(6*wordJump), 0.1, "piernas y la pelota es un esperma que posiblemente te deje", siz);
    output(x, y-((6+0.5)*wordJump), 0.1, "embarazada. Sin embargo, embarazarse no es malo si es tu", siz);
    output(x, y-(7*wordJump), 0.1, "desicion y ya tienes un buen desarrollo. Los bumpers rojos", siz);
    output(x, y-((7+0.5)*wordJump), 0.1, "haran que tu vida amorosa mejore mientras los verdes haran", siz);
    output(x, y-(8*wordJump), 0.1, "que tu vida profesional mejore. Para tener una buena familia", siz);
    output(x, y-((8+0.5)*wordJump), 0.1, "Se ocupa un balance entre ambos. Suerte! Y recuerda,", siz);
    output(x, y-(9*wordJump), 0.1, "sin globito no hay fiesta!", 0.07);

    outputTip(x, y, words, 10);

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
    glDisable(GL_LIGHTING);

    drawBackground(x, y, background);

    glColor3f(title.r, title.g, title.b);
    output(x, y-wordJump, 0.01, "FIN DEL JUEGO, TE EMBARAZASTE", 0.1);

    glColor3f(words.r, words.g, words.b);

    if(familyLevel == 0){
        output(x, y-(2*wordJump), 0.1, "No sabes quien es papa! Como le diras a tus padres?", 0.065);
    } else if(familyLevel == 1){
        output(x, y-(2*wordJump), 0.1, "Pero tu novio como que no... Ojala te apoye", 0.067);
    } else if(familyLevel == 2){
        output(x, y-(2*wordJump), 0.1, "Tu novio dice que te apoya! Esperemos que si.", 0.067);
    } else if(familyLevel == 3){
        output(x, y-(2*wordJump), 0.1, "Tu esposo se ve feliz, pero sus ojos no...", 0.067);
    } else if(familyLevel == 4){
        output(x, y-(2*wordJump), 0.1, "Tu esposo se ve igual de feliz que tu!", 0.067);
    } else{
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
    } else{
        output(x, y-(3*wordJump), 0.1, "Eres duenia de una superempresa y mujer del anio.", 0.067);
    }

    outputScore(x, y, words, 4);
    output(x, y-(5*wordJump), 0.1, "Iniciar un nuevo juego - I", 0.1);
    outputTip(x, y, words, 6);

    glEnable(GL_LIGHTING);
}
