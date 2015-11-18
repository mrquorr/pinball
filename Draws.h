#include <string.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <assert.h>
#include <fstream>

#include "imageloader.h"

using namespace std;


/**
    Function that draws the circle in the screen given its position

    @param p point in where the circle should be draw
    @param radius of the circle to draw
    @param c Color struct to paint the circle

*/
void drawCircle(Point& p, float radius, Color& c ) {

  glPushMatrix();

    changeMaterial(pinballMaterial);
    glScaled(1, 1, 0.02);
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
        glScaled(1.0, 1.0, 0.02);
        glTranslated(p.x, p.y, 0);
        glutSolidSphere(radius,40,40);
	glPopMatrix();
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
    glScaled(1.0, 1.0, 0.02);
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
    glScaled(width, height, 0.05);
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

    /*
    glPushMatrix();
      glColor3f(c.r, c.g, c.b);
      changeMaterial(wallMaterial);
      glTranslated(50, 50, 0);
      glScaled(5,4,0.05);
      glutSolidCube(2);
    glPopMatrix();
    */

	 glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texName[0]);
    glDisable(GL_LIGHTING);

	glBegin(GL_POLYGON);
        //define vertices as the flipper's position
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(f.backLeft.x, f.backLeft.y+20, .0);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(f.backRight.x, f.backRight.y-20, .0 );
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(f.frontRight.x, f.frontRight.y-20,.0);

        glVertex3f(f.frontTop.x, f.frontTop.y, .0);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(f.frontLeft.x, f.frontLeft.y+20, .0);
    glEnd();
    glEnable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

}

/**
    Makes the image into a texture, and returns the id of the texture
*/

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

Image::Image(char* ps, int w, int h) : pixels(ps), width(w), height(h)
{

}

Image::~Image()
{
    delete[] pixels;
}

namespace
{
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
}

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
