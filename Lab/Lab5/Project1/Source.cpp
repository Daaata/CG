/***C/C++  http://bbs.csdn.net/topics/100091584***/
#include <stdio.h> 
#include <stdlib.h>
#include <iostream>
#include <math.h>
/***freeglut***/
#include "C://CG/freeglut/include/GL/freeglut.h"

using namespace std;

#define PI 3.14159265

void SetupRC();
void ChangeSize(int, int);
void RenderScene(void);
void myKeyboard(unsigned char, int, int);
void mySpecialKey(int, int, int);
void myMouse(int, int, int, int);
double newx = 0, newy = 0, newz = 0, newangle = 0, newtemp, newsize, rx, ry, rz;
double tx = 0, ty = 0, tz = 0, thetaX, thetaY, thetaZ, s = 1, sx, sy, sz;

GLfloat scaMatrix[] = { 1.0, 0.0, 0.0, 0.0,
						0.0, 1.0, 0.0, 0.0,
						0.0, 0.0, 1.0, 0.0,
						0.0, 0.0, 0.0, 1.0 };
GLfloat rotMatriX[] = { 1.0, 0.0, 0.0, 0.0,
						0.0, 1.0, 0.0, 0.0,
						0.0, 0.0, 1.0, 0.0,
						0.0, 0.0, 0.0, 1.0 };
GLfloat rotMatriY[] = { 1.0, 0.0, 0.0, 0.0,
						0.0, 1.0, 0.0, 0.0,
						0.0, 0.0, 1.0, 0.0,
						0.0, 0.0, 0.0, 1.0 };
GLfloat rotMatriZ[] = { 1.0, 0.0, 0.0, 0.0,
						0.0, 1.0, 0.0, 0.0,
						0.0, 0.0, 1.0, 0.0,
						0.0, 0.0, 0.0, 1.0 };
GLfloat rotMatriN[] = { 1.0, 0.0, 0.0, 0.0,
						0.0, 1.0, 0.0, 0.0,
						0.0, 0.0, 1.0, 0.0,
						0.0, 0.0, 0.0, 1.0 };
GLfloat translateMatrix[] = { 1.0, 0.0, 0.0, 0.0,
							  0.0, 1.0, 0.0, 0.0,
							  0.0, 0.0, 1.0, 0.0,
							  0.0, 0.0, 0.0, 1.0 };

int main(int argc, char** argv)
{
	tx = ty = tz = thetaX = thetaY = thetaZ = 0;
	s = sx = sy = sz = 1;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(600, 80);
	glutCreateWindow("Moving Cube");
	SetupRC();
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	glutKeyboardFunc(myKeyboard);
	glutMouseFunc(myMouse);
	glutSpecialFunc(mySpecialKey);
	glutMainLoop();

	return 0;
}

void SetupRC()
{
	GLfloat whiteLight[] = { 0.45f,0.45f,0.45f,1.0f };
	GLfloat sourceLight[] = { 0.25f,0.25f,0.25f,1.0f };
	GLfloat lightPos[] = { 0.f,25.0f,20.0f,0.0f };

	glEnable(GL_LIGHTING);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, sourceLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sourceLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);

	return;
}

void ChangeSize(int w, int h)
{
	printf("Window Size= %d X %d\n", w, h);
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10, 10, -10, 10, -10, 20);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	return;
}

void RenderScene(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(6, 6, 6, 0, 0, 0, 0, 1, 0);
	//gluLookAt(2, 2, 10.0f, 0, 0, 0, 0, 1, 0);
	//gluLookAt(0, 0, -10.0f, 0, 0, 0, 0, 1, 0);

	glColor3f(0.5, 0.12, 0); //畫X軸
	glBegin(GL_LINES);
	glVertex3f(-9, 0, 0);
	glVertex3f(9, 0, 0);
	glEnd();
	glColor3f(0, 0.5, 0.12); //畫Y軸
	glBegin(GL_LINES);
	glVertex3f(0, -9, 0);
	glVertex3f(0, 9, 0);
	glEnd();
	glColor3f(0.12, 0.12, 0.5); //畫Z軸
	glBegin(GL_LINES);
	glVertex3f(0, 0, -9);
	glVertex3f(0, 0, 9);
	glEnd();

	glColor3f(0.5, 0.5, 0.5); //畫點
	glPointSize(10);
	glBegin(GL_POINTS);
	//glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(newx, newy, newz);
	glEnd();

	glColor3f(0.5, 0.5, 0.5); //畫線
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(newx, newy, 0.0);
	glEnd();
	/*放大*/
	scaMatrix[0] = sx;
	scaMatrix[5] = sy;
	scaMatrix[10] = sz;
	/*平移*/
	translateMatrix[12] = tx;
	translateMatrix[13] = ty;
	translateMatrix[14] = tz;
	/*旋轉*/
	double gin = PI / 180.0;
	rotMatriX[5] = cos(thetaX*gin);
	rotMatriX[6] = sin(thetaX*gin);
	rotMatriX[9] = -1 * sin(thetaX*gin);
	rotMatriX[10] = cos(thetaX*gin);

	rotMatriY[0] = cos(thetaY*gin);
	rotMatriY[2] = -1 * sin(thetaY*gin);
	rotMatriY[8] = sin(thetaY*gin);
	rotMatriY[10] = cos(thetaY*gin);

	rotMatriZ[0] = cos(thetaZ*gin);
	rotMatriZ[1] = sin(thetaZ*gin);
	rotMatriZ[4] = -1 * sin(thetaZ*gin);
	rotMatriZ[5] = cos(thetaZ*gin);
	/**/
	newsize = newangle * gin;
	newtemp = sqrt(newx*newx + newy * newy);
	if (newtemp != 0)
	{
		rx = newx / newtemp;
		ry = newy / newtemp;
		rz = newz / newtemp;
	}
	rotMatriN[0] = 1 - cos(newsize)*rx*rx + cos(newsize);
	rotMatriN[1] = 1 - cos(newsize)*rx*ry + sin(newsize)*rz;
	rotMatriN[2] = 1 - cos(newsize)*rx*rz - sin(newsize)*ry;
	rotMatriN[4] = 1 - cos(newsize)*rx*ry - sin(newsize)*rz;
	rotMatriN[5] = 1 - cos(newsize)*ry*ry + cos(newsize);
	rotMatriN[6] = 1 - cos(newsize)*ry*rz + sin(newsize)*rx;
	rotMatriN[8] = 1 - cos(newsize)*rx*rz + sin(newsize)*ry;
	rotMatriN[9] = 1 - cos(newsize)*ry*rz - sin(newsize)*rx;
	rotMatriN[10] = 1 - cos(newsize)*rz*rz + cos(newsize);

	glMultMatrixf(scaMatrix);
	glMultMatrixf(rotMatriX);
	glMultMatrixf(rotMatriY);
	glMultMatrixf(rotMatriZ);
	glMultMatrixf(rotMatriN);
	glMultMatrixf(translateMatrix);

	glColor3f(1, 1, 0);
	glutSolidCube(6 * s);
	glutSwapBuffers();

	return;
}

void myKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'r':
		tx = 0, ty = 0, tz = 0;
		thetaX = 0, thetaY = 0, thetaZ = 0;
		rx = 0, ry = 0, rz = 0;
		newx = 0, newy = 0, newz = 0;
		newangle = 0;
		sx = sy = sz = 1;
		s = 1;
		break;
	case 'a':
		thetaX += 3;
		break;
	case 'd':
		thetaX -= 3;
		break;
	case 'w':
		thetaY += 3;
		break;
	case 's':
		thetaY -= 3;
		break;
	case 'z':
		thetaZ -= 3;
		break;
	case 'x':
		thetaZ += 3;
		break;
	case 'n':
		thetaZ -= 3;
		break;
	case 'm':
		thetaZ += 3;
		break;
	case '1':
		sx -= 0.1;
		break;
	case '2':
		sx += 0.1;
		break;
	case '4':
		sy -= 0.1;
		break;
	case '5':
		sy += 0.1;
		break;
	case '7':
		sz -= 0.1;
		break;
	case '8':
		sz += 0.1;
		break;
	case '+':
		s *= 2; //xyz放大2倍
		break;
	case '-':
		s *= 0.25; //xyz放大0.25倍
		break;
	default:
		break;
	}
	glutPostRedisplay();

	return;
}

void mySpecialKey(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		tx -= 0.2;
		break;
	case GLUT_KEY_RIGHT:
		tx += 0.2;
		break;
	case GLUT_KEY_UP:
		ty += 0.2;
		break;
	case GLUT_KEY_DOWN:
		ty -= 0.2;
		break;
	case GLUT_KEY_PAGE_UP:
		tz += 0.2;
		break;
	case GLUT_KEY_PAGE_DOWN:
		tz -= 0.2;
		break;
	default:
		break;
	}
	glutPostRedisplay();

	return;
}

void myMouse(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_UP)
		{
			//newx = (x - 200) / 200.0 * 10;
			//newy = (-1 * (y - 200)) / 200.0 * 10;
			newx = (x * 40) / 400 - 20;
			newy = 20 - (y * 40) / 400;
			newz = 0;
			cout << "Mouse left button down" << newx << "," << newy << endl;
		}
		break;
	case GLUT_MIDDLE_BUTTON:
		if (state == GLUT_DOWN)
		{
			cout << "Mouse middle button down" << endl;
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
		{
			cout << "Mouse right button down" << endl;
		}
		break;
	default:
		break;
	}
	return;
}