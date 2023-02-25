/*The Lorenz system is a system of ordinary differential equations first studied by Edward Lorenz. 
It is notable for having chaotic solutions for certain parameter values and initial conditions.
In particular, the Lorenz attractor is a set of chaotic solutions of the Lorenz system which, when plotted,
resemble a butterfly or figure eight.*/


#include <GL/glut.h>
#include <GL/freeglut.h>
#include "Header.h"
#include <stdlib.h>
#include <stdio.h>
#include<Mmsystem.h> //for PlaySound()

#pragma comment(lib,"Winmm.lib")  //For PlaySound()

bool bFullscreen = false;


int numPts = 200000;

double th = 0;  //  Rotation angle

float pts[200000][3];

static GLfloat RotX = 25.0, RotY = 50.0, RotZ = 35.0;

static GLfloat Position = 60.0;

/*  Lorenz Parameters Constants
{\displaystyle \sigma =10} \sigma =10, {\displaystyle \beta =8/3} \beta =8/3 and {\displaystyle \rho =28} \rho =28.*/
double a = 10;
double b = 8/3;
double c = 28;

GLfloat angle_Tri = 0.0f;

int iterate = 0;
int iterInc = 10;



void display(void) 
{
	/*If {\displaystyle \rho <1} \rho <1 then there is only one equilibrium point, which is at the origin.
	This point corresponds to no convection. All orbits converge to the origin, which is a global attractor,
	when {\displaystyle \rho <1} \rho <1.

	When {\displaystyle \rho =28} \rho =28, {\displaystyle \sigma =10} \sigma =10, 
	{\displaystyle \beta =8/3} \beta =8/3, the Lorenz system has chaotic solutions (but not all solutions are chaotic).
	Almost all initial points will tend to an invariant set – the Lorenz attractor – a strange attractor and a fractal. 
	Its Hausdorff dimension is estimated to be 2.06 ± 0.01, and the correlation dimension is estimated to be 2.05 ± 0.01.
	The exact Lyapunov dimension (Kaplan-Yorke dimension) formula of the global attractor can be found analytically under
	classical restrictions on the parameters
	*/
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPointSize(1.5f);

	glPushMatrix();

	Sleep(10);

	//glScalef(1.5, 1.5f, 1.5f);

	glTranslatef(0.0f, 0.0f, -65.0f);

	glRotatef(angle_Tri, 1.0f, 1.0f, 1.0f);

	glRotatef(RotX, 1.0, 1.0, 0.0);
	glRotatef(RotY, 0.0, 1.0, 0.0);
	glRotatef(RotZ, 0.0, 0.0, 1.0);

	glBegin(GL_LINE_STRIP);              //GL_LINE_STRIP  or    GL_POINTS
 
	int i = 0;

	while (i < iterate && i < numPts)
	{
		//glColor3fv(pts[i]);
		glColor3f(0.11f, 0.635f, 0.933f);             //light blue color

		//glColor3f(0.933f, 0.415f, 0.11f);				//light orange

		glVertex3fv(pts[i++]);
	}

	glEnd();


	if (iterate < numPts)
	{
		if (iterate + iterInc > numPts)
		{
			iterate = numPts;
		}
		else
		{
			iterate += iterInc;
		}
	}

	glFlush();
	glutSwapBuffers();

	glPopMatrix();

}

static void reshape(int width, int height) 
{
	if (height == 0)
		height = 1;
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

static void special(int key, int x, int y) {
	switch (key)
	{
	case GLUT_KEY_UP:
		RotX += 5.0;
		break;
	case GLUT_KEY_DOWN:
		RotX -= 5.0;
		break;
	case GLUT_KEY_LEFT:
		RotY += 5.0;
		break;
	case GLUT_KEY_RIGHT:
		RotY -= 5.0;
		break;
	default:
		return;
	}
	glutPostRedisplay();
}

static void key(unsigned char key, int x, int y)
{
	switch (key)
	{
	
	case 'r':						//repeat refresh
		iterate = 0;
		break;

	case 'c':						//complete num points
		iterate = numPts;
		break;

	case 't':						//iterate faster times
		iterInc += 5;
		break;

	case 'I':								//zoom In
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		Position -= 1;
		gluLookAt(0, 0, Position, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		break;

	case 'O':								//zoom out
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		Position += 1;
		gluLookAt(0, 0, Position, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		break;

	case 27:								//esc
		glutLeaveMainLoop();
		break;

	/*case 'F':								//full screen window
	case 'f':
		if (bFullscreen == false)
		{
			glutFullScreen();
			bFullscreen = true;
		}
		else
		{
			glutLeaveFullScreen();
			bFullscreen = false;
		}
		break;*/

	default:
		return;
	}
	glutPostRedisplay();
}

void spin(void) 
{
	// for rotation

	angle_Tri = angle_Tri + 0.05f;
	if (angle_Tri >= 360.0f)
		angle_Tri = angle_Tri - 360.0f;


	glutPostRedisplay();
}

static void LorenzAttractor(void)
{
	int i;

	//  Coordinates  
	float x = pts[0][0] = 1;
	float y = pts[0][1] = 1;
	float z = pts[0][2] = 1;

	//  Time step  
	float dt = 0.001;         //differential integrator

	for (i = 0;i<numPts - 1;i++)
	{
		/* The model is a system of three ordinary differential equations now known as the Lorenz equations:

{\displaystyle {\begin{aligned}{\frac {\mathrm {d} x}{\mathrm {d} t}}&=\sigma (y-x),
\\[6pt]{\frac {\mathrm {d} y}{\mathrm {d} t}}&=x(\rho -z)-y,\\[6pt]{\frac {\mathrm {d} z}
{\mathrm {d} t}}&=xy-\beta z.\end{aligned}}} {\displaystyle {\begin{aligned}{\frac {\mathrm {d} x}
{\mathrm {d} t}}&=\sigma (y-x),\\[6pt]{\frac {\mathrm {d} y}{\mathrm {d} t}}&=x(\rho -z)-y,\\[6pt]
{\frac {\mathrm {d} z}{\mathrm {d} t}}&=xy-\beta z.\end{aligned}}}*/

		float dx = a*(y - x);         
		float dy = x*(c - z) - y;
		float dz = x*y - b*z;
		x += dt*dx;
		y += dt*dy;
		z += dt*dz;

		pts[i + 1][0] = x;
		pts[i + 1][1] = y;
		pts[i + 1][2] = z;

	}
}


int main(int argc, char* argv[])
{
	//fuction declarations
	void LorenzAttractor(void);
	void display(void);
	void reshape(int, int);
	void special(int, int, int);
	void key(unsigned char, int, int);
	void spin(void);



	LorenzAttractor();

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);

	glutCreateWindow("Lorenz Attractor ");

	glutFullScreen();

	PlaySound(MAKEINTRESOURCE(My_Song), NULL, SND_RESOURCE | SND_ASYNC);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutKeyboardFunc(key);
	glutIdleFunc(spin);

	glutMainLoop();

	return 0;
}
