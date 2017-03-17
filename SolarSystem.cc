#include <stdio.h>     // - Just for some ASCII messages

#include <time.h>
#include "SolarSystem.h"   // Header file for our OpenGL functions
#include <math.h>
#include <stdio.h>
#include <iostream>
#include "gl/glut.h"   // - An interface and windows 
                       //   management library
#include "planets.h"
using namespace std ;

float PI=3.1415926535;
float coloor=0.1f;
bool coloorchech=true;

float sunsize=38.0f;
bool sunsizechech=true;

double AY=0.0f;
double AX=20.0f;
bool dirA=false;
float rot=0.0f;
float rotA=0.0f;
float rotS=90.0f;
float rotU=210.0f;
float rotE=180.0f;

double EY=0.0f;
double EX=-20.0f;
bool dirE=true;

double EEY=0.0f;
double EEX=-20.0f;
bool dirEE=true;

double SY=0.0f;
double SX=-10.0f;
bool dirS=true;

double PY=0.0f;
double PX=-80.0f;
bool dirP=true;

bool doAnimation = true;

CObjLoader sphere;

// deltaX, deltaY is how much we are going to move the view point
float deltaX = 0.0f;
float deltaY = 0.0f;

// how fast are we going to move the view point where the user press the arrow keys
float step = 1.0f;

void Render()
{    
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(deltaX, deltaY, -150.0);
	planets(sphere, AX,AY,EX,EY,EEX,EEY,SX,SY,PX,PY,rot,coloor,sunsize);
	glutSwapBuffers();
}

//-----------------------------------------------------------


void Idle()
{

	if (!doAnimation)	// nothing to update
	{
		glutPostRedisplay(); 
		return;
	}


	// Self Rotation
	rot++;
	if(rot > 360)
	{ 
		rot = 1;
	}

	// Sun Animation
	if(coloorchech == true )
	{
		coloor=coloor + 0.005;
		if(0.4 <= coloor) coloorchech=false;
	}
	else if (coloorchech == false)
	{
		coloor=coloor - 0.005;
		if (coloor < 0.005) coloorchech=true;
	}

	if(sunsizechech == true )
	{
		sunsize=sunsize + 0.2;
		if(55 <= sunsize) sunsizechech=false;
	}
	else if (sunsizechech == false)
	{
		sunsize=sunsize - 0.2;
		if (sunsize < 38) sunsizechech=true;
	}


	// ClockWize
		
	// Afrodite Animation

	double R = 90;

	rotA+= 0.5;
	if(rotA > 360)
	{ 
		rotA = 0.5;
	}

	AX=R*sin(rotA*PI/180);
	AY=R*cos(rotA*PI/180);

	// Saturn Animation
	double RS=115;

	rotS+= 0.3;
	if(rotS > 360)
	{ 
		rotS = 0.3;
	}

	SX=RS*sin(rotS*PI/180);
	SY=RS*cos(rotS*PI/180);
	
	// Uranus Animation

	double RP=110;

	rotU+= 0.2;
	if(rotU > 360)
	{ 
		rotU = 0.2;
	}

	PX=RP*sin(rotU*PI/180);
	PY=RP*cos(rotU*PI/180);

	// Counter ClockWise
	
	// Ermes Animation
	double RE = 65;
	

	EX= -(RE*sin(rot*PI/180));
	EY=RE*cos(rot*PI/180);
	
	// // Earth Animation

	double REE = 120;
	

	rotE+= 0.4;
	if(rotE > 360)
	{ 
		rotE = 0.4;
	}

	EEX=-(REE*sin(rotE*PI/180));
	EEY=REE*cos(rotE*PI/180);

	

    glutPostRedisplay(); 

}

//-----------------------------------------------------------

void Resize(int w, int h)
{ 
	// define the visible area of the window ( in pixels )
	if (h==0) h=1;
	glViewport(0,0,w,h); 

	// Setup viewing volume

	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	 
////(02b)
	          // L     R       B      T      N      F
	// glOrtho (-500.0f, 500.0f, -500.0f, 500.0f,-500.0f,500.0f);

	float aspect = (float)w/(float)h;             /// aspect ratio

	gluPerspective(130.0, aspect, 1.0, 500.0);     // to 500.0 the base of the piramid and 1.0 where I cut the piramid
}


//-----------------------------------------------------------

void Setup()  
{ 
	int ret = sphere.Load("planet.obj");
	if (ret)
	{
		printf("Error loading planet.obj. Loader has returner code = %d. Press enter to exit -->\n", ret);
		getchar();
		exit(1);
	}
	else
	{
		printf("planet.obj has been properly loaded\n\n");
		printf("Press:\n");
		printf("  a     to start/stop animation\n");
		printf("  q/esc to exit\n");
		printf("  left  to move left\n");
		printf("  right to move right\n");
		printf("  up    to move up\n");
		printf("  down  to move down\n");
	}

	initStars();

	glEnable( GL_CULL_FACE );

	glShadeModel( GL_SMOOTH );

	glEnable(GL_DEPTH_TEST);
	glDepthFunc( GL_LEQUAL );      
	glClearDepth(1.0); 		      


	//Set up light source
	GLfloat ambientLight[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat diffuseLight[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat lightPos[] = { -20.0, 20.0, 150.0, 1.0 };

	glLightfv( GL_LIGHT0, GL_AMBIENT, ambientLight );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseLight );
	glLightfv( GL_LIGHT0, GL_POSITION,lightPos );

 
	// polygon rendering mode and material properties
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
   
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
   
	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0);



	// Black background
	glClearColor(0.0f,0.0f,0.0f,1.0f);
}

void specialKeys(int key, int x, int y)
{
	switch (key)
	{
		case 100:	deltaX -= step;  break;	// left arrow key
		case 102:	deltaX += step;  break;	// right arrow key
		case 101:	deltaY += step;  break;	// up arrow key
		case 103:	deltaY -= step;  break;	// down arrow key
	}
}

void keyboardDown(unsigned char key, int x, int y) 
{
  switch(key) 
  {
		case 'Q':
		case 'q':
		case  27: exit(1); break;
		case 'a':
		case 'A': doAnimation = !doAnimation; break;
	}
}


