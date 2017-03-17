#include <stdio.h>     // - Just for some ASCII messages
#include <time.h>
#include <math.h>
#include <iostream>
#include "gl/glut.h"   // - An interface and windows 
#include "planets.h"
                       //   management library
using namespace std ;

CObjLoader::CObjLoader()
{
}

void CObjLoader::Clear()
{
	v.clear();
	n.clear();
	t.clear();
}


int CObjLoader::Load(const char *filename)
{
	FILE *f = fopen(filename, "rt");
	if (f == NULL)
		return 1;
	char line[100];

	// loading verteces, normals and triangles
	while (fgets(line, 100, f))
	{
		if (line[0] == 'v' && line[1] == ' ')	// new vertex
		{
			float a,b,c;
			if (sscanf(line, "v %f %f %f", &a, &b, &c) != 3)
			{
				Clear();
				fclose(f);
				return 2;
			}
			if (v.size() == 0)
			{
				min[0] = max[0] = a;
				min[1] = max[1] = b;
				min[2] = max[2] = c;
			}
			else
			{
				if (min[0] > a) min[0] = a;
				if (min[1] > b) min[1] = b;
				if (min[2] > c) min[2] = c;
				if (max[0] < a) max[0] = a;
				if (max[1] < b) max[1] = b;
				if (max[2] < c) max[2] = c;
			}
			v.push_back(a);
			v.push_back(b);
			v.push_back(c);
		}
		else if (line[0] == 'v' && line[1] == 'n')
		{
			float a,b,c;
			if (sscanf(line, "vn %f %f %f", &a, &b, &c) != 3)
			{
				Clear();
				fclose(f);
				return 2;
			}
			n.push_back(a);
			n.push_back(b);
			n.push_back(c);
		}
		else if (line[0] == 'f')
		{
			int a,b,c,d,e,g;
			if (sscanf(line, "f %d//%d %d//%d %d//%d", &a, &b, &c, &d, &e, &g) != 6)
			{
				Clear();
				fclose(f);
				return 2;
			}
			t.push_back(a-1);
			t.push_back(b-1);
			t.push_back(c-1);
			t.push_back(d-1);
			t.push_back(e-1);
			t.push_back(g-1);
		}
	}
	fclose(f);
	return (v.size() > 0 && n.size() > 0 && t.size() > 0) ? 0 : 3;
}

void CObjLoader::DrawSolid(float maxSize)
{
	glPushMatrix();

	float maximun = max[0] - min[0];
	if (max[1] - min[1] > maximun) maximun = max[1] - min[1];
	if (max[2] - min[2] > maximun) maximun = max[2] - min[2];

	float center[3];
	center[0] = (max[0] + min[0]) * 0.5f;
	center[1] = (max[1] + min[1]) * 0.5f;
	center[2] = (max[2] + min[2]) * 0.5f;

	glScalef(maxSize/maximun, maxSize/maximun, maxSize/maximun);
	glTranslatef(-center[0], -center[1], -center[2]);

	glBegin(GL_TRIANGLES);
		int s = t.size();
		for (int i=0; i<s; i+=6)
		{
			// first vertex and normal
			int j = t[i + 1] * 3;
			glNormal3f(n[j], n[j+1], n[j+2]);
			j = t[i + 0] * 3;
			glVertex3f(v[j], v[j+1], v[j+2]);

			//second vertex and normal
			j = t[i + 3] * 3;
			glNormal3f(n[j], n[j+1], n[j+2]);
			j = t[i + 2] * 3;
			glVertex3f(v[j], v[j+1], v[j+2]);

			//third vertex and normal
			j = t[i + 5] * 3;
			glNormal3f(n[j], n[j+1], n[j+2]);
			j = t[i + 4] * 3;
			glVertex3f(v[j], v[j+1], v[j+2]);
		}
	glEnd();
	glPopMatrix();
}

void CObjLoader::DrawWireframe(float maxSize)
{
	glPushMatrix();

	float maximun = max[0] - min[0];
	if (max[1] - min[1] > maximun) maximun = max[1] - min[1];
	if (max[2] - min[2] > maximun) maximun = max[2] - min[2];

	float center[3];
	center[0] = (max[0] + min[0]) * 0.5f;
	center[1] = (max[1] + min[1]) * 0.5f;
	center[2] = (max[2] + min[2]) * 0.5f;

	maxSize *= 1.01f;
	glScalef(maxSize/maximun, maxSize/maximun, maxSize/maximun);
	glTranslatef(-center[0], -center[1], -center[2]);

	glBegin(GL_LINE_LOOP);
		int s = t.size();
		for (int i=0; i<s; i+=6)
		{
			// first vertex and normal
			int j = t[i + 1] * 3;
			glNormal3f(n[j], n[j+1], n[j+2]);
			j = t[i + 0] * 3;
			glVertex3f(v[j], v[j+1], v[j+2]);

			//second vertex and normal
			j = t[i + 3] * 3;
			glNormal3f(n[j], n[j+1], n[j+2]);
			j = t[i + 2] * 3;
			glVertex3f(v[j], v[j+1], v[j+2]);

			//third vertex and normal
			j = t[i + 5] * 3;
			glNormal3f(n[j], n[j+1], n[j+2]);
			j = t[i + 4] * 3;
			glVertex3f(v[j], v[j+1], v[j+2]);
		}
	glEnd();
	glPopMatrix();
}


#define N_STARS  50
#define Z_STARS -150
#define X_STARS 800
#define Y_STARS 600
#define STAR_SIZE 10
#define PERCENT 0.25f
#define BRIGHT_FRAMES 50

vector<float> stars;
vector<int>   starFrameCounter;

float random(float a, float b)
{
	float value = rand() / (float)RAND_MAX;
	return a + (b-a) * value;
}

void initStars()
{
	for (int i=0; i<N_STARS; i++)
	{
		stars.push_back(random(-X_STARS, X_STARS));
		stars.push_back(random(-Y_STARS, Y_STARS));
		stars.push_back(Z_STARS);
		starFrameCounter.push_back((int)random(0,BRIGHT_FRAMES));
	}
}

void drawStar(float x, float y, float z)
{
	glPushMatrix();
	glTranslatef(x,y,z);
	glBegin(GL_TRIANGLES);
		glVertex3f(0, STAR_SIZE, 0);
		glVertex3f(-PERCENT *STAR_SIZE, +PERCENT * STAR_SIZE, 0);
		glVertex3f(+PERCENT * STAR_SIZE, +PERCENT * STAR_SIZE, 0);

		glVertex3f(+STAR_SIZE, 0, 0);
		glVertex3f(+PERCENT * STAR_SIZE, +PERCENT * STAR_SIZE, 0);
		glVertex3f(+PERCENT * STAR_SIZE, -PERCENT * STAR_SIZE, 0);

		glVertex3f(0, -STAR_SIZE, 0);
		glVertex3f(+PERCENT * STAR_SIZE, -PERCENT * STAR_SIZE, 0);
		glVertex3f(-PERCENT *STAR_SIZE, -PERCENT * STAR_SIZE, 0);

		glVertex3f(-STAR_SIZE, 0, 0);
		glVertex3f(-PERCENT *STAR_SIZE, -PERCENT * STAR_SIZE, 0);
		glVertex3f(-PERCENT *STAR_SIZE, +PERCENT * STAR_SIZE, 0);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3f(-PERCENT *STAR_SIZE, -PERCENT * STAR_SIZE, 0);
		glVertex3f(+PERCENT * STAR_SIZE, -PERCENT * STAR_SIZE, 0);
		glVertex3f(+PERCENT * STAR_SIZE, +PERCENT * STAR_SIZE, 0);
		glVertex3f(-PERCENT *STAR_SIZE, +PERCENT * STAR_SIZE, 0);
	glEnd();
	glPopMatrix();
}

void drawStars()
{
	glNormal3f(0,0,1.0f);
	int n = stars.size();
	for (int i=0; i<n; i+=3)
	{
		int animFrame = starFrameCounter[i/3];
		if (animFrame > BRIGHT_FRAMES/2)
			glColor3f(1.0f-(animFrame-BRIGHT_FRAMES/2)/float(BRIGHT_FRAMES/2), 1.0f-(animFrame-BRIGHT_FRAMES/2)/float(BRIGHT_FRAMES/2), 1.0f - 0.5f*(animFrame-BRIGHT_FRAMES/2)/float(BRIGHT_FRAMES/2));
		else
			glColor3f(animFrame/float(BRIGHT_FRAMES/2), animFrame/float(BRIGHT_FRAMES/2), 0.5f + 0.5f * animFrame/float(BRIGHT_FRAMES/2));
		drawStar(stars[i], stars[i+1], stars[i+2]);
		starFrameCounter[i/3] = (animFrame+1) % BRIGHT_FRAMES;
	}
}


void planets(CObjLoader & sphere, double AX,double AY,double EX, double EY ,double EEX, double EEY,double SX, double SY,double PX,double PY, float rot,float coloor,float sunsize)
{
	drawStars();
	// Uranus
	glPushMatrix();
	glTranslatef(PX, -PX, PY);
	glColor3f(0.0, 0.0 , 0.3);
	glutSolidSphere(10.0, 24.0, 24);
	// sphere.DrawSolid(20.0f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(PX, -PX, PY);
	glRotatef(-90,0,1,0);
	glRotatef(45,1,0,0);
	glRotatef(rot*2,0,0, 1);
	glColor3f(1, 1, 1);
	glutWireTorus(1, 10.0, 40 , 20);
	glPopMatrix();
	

	// Saturn

	glPushMatrix();
	glTranslatef(SX, SX, SY);
	glRotatef(90,1,0,0);
	glRotatef(rot/2,0,0, 1);
	glColor3f(0.5, 0.3, 0.0);
	glutSolidSphere(5.5, 50.0, 10);
	// sphere.DrawSolid(11.0f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(SX, SX, SY);
	glRotatef(90,1,0,0);
	glRotatef(rot*5,0.0,0.0, 1);
	glColor3f(0.6, 0.6, 0.6);
	glutWireTorus(1, 8.0, 50, 10);
	glPopMatrix();


	// Earth
	glPushMatrix();
	glTranslatef(EEX,0.0,EEY);
	glRotatef(100,1,0.0,0.0);
	glRotatef(20,0.0 , 1 , 0.0);
	glRotatef(rot,0.0,0.0, 1);
	glColor3f(0.0, 0.1, 0.6);
	glutWireSphere(6.0, 24.0, 90);
	// sphere.DrawWireframe(12.0f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(EEX,0.0,EEY);
	glRotatef(100,1,0.0,0.0);
	glRotatef(20,0.0 , 1 , 0.0);
	glRotatef(rot,0.0,0.0, 1);
	glColor3f(0.0, 0.4, 0.1);
	glutWireSphere(6.0, 12.0, 90);
	// sphere.DrawWireframe(12.0f);
	glPopMatrix();

	// Moon
	glPushMatrix();
	glTranslatef(EEX,0.0,EEY);
	glTranslatef(0.0,EY/6,EX/6);
	glRotatef(100,1,0.0,0.0);
	glRotatef(-30,0.0 , 1 , 0.0);
	glRotatef(rot,0.0,0.0, 1);
	glColor3f(0.5, 0.5, 0.5);
	glutSolidSphere(1.5, 10.0, 170);
	// sphere.DrawSolid(3.0f);
	glPopMatrix();
	
	// //Afrodite
	glPushMatrix();
	glTranslatef(0.0,20.0, -35.0);
	glTranslatef(AX,AY,AY);
	glRotatef(100,1,0.0,0.0);
	glRotatef(-30,0.0 , 1 , 0.0);
	glRotatef(rot,0.0,0.0, 1);
	glColor3f(0.5, 0.0, 0.3);
	glutWireSphere(10.0, 12.0 , 300);
	// sphere.DrawWireframe(20.0f);
	glPopMatrix();

	// Ermes
	glPushMatrix();
	glTranslatef(EX,0.0,EY);
	glRotatef(100,1,0.0,0.0);
	glRotatef(-10,0.0 , 1 , 0.0);
	glRotatef(rot*2,0.0,0.0, 1);
	glColor3f(0.3, 0.3, 0.3);
	glutSolidSphere(7.0, 7.0 , 200);
	// sphere.DrawSolid(14.0f);
	glPopMatrix();

	// Pluton
	glPushMatrix();
	glTranslatef(0,AY*3,AX*3);
	glRotatef(100,1,0.0,0.0);
	glRotatef(-10,0.0 , 1 , 0.0);
	glRotatef(rot*2,0.0,0.0, 1);
	glColor3f(0.7, 0.7, 0.7);
	glutSolidSphere(7.0, 7.0 , 200);
	// sphere.DrawSolid(14.0f);
	glPopMatrix();

	// Sun 
	glTranslatef(0.0, 0.0, 0.0);
	glRotatef(100,1,0.0,0.0);
	glRotatef(-30,0.0 , 1 , 0.0);
	glRotatef(rot/2,0.0,0.0, 1);
	glColor3f(coloor+0.5, coloor-0.08, 0.0);
	glutWireSphere(sunsize, 50 , 100);
	glutSolidSphere(45, 15 , 300);
	

	  
}