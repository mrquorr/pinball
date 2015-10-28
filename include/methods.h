#ifndef METHODS_H
#define METHODS_H

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


#endif // METHODS_H
