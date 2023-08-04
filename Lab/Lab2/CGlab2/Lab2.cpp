/***C/C++  http://bbs.csdn.net/topics/100091584***/
#include <stdio.h> 
#include <stdlib.h>
/***freeglut***/
#include "D://CG/OpenglLib/freeglut/include/GL/freeglut.h"

void SetupRC();
void ChangeSize(int, int);
void RenderScene(void);
void myKeyboard(unsigned char, int, int);
void mySpecialKey(int, int, int);

double tx, ty, tz, thetaX, thetaY, thetaZ;

int main(int argc, char** argv)
{
	tx = 0, ty = 0, tz = 0;
	thetaX = 0, thetaY = 0, thetaZ = 0;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(600, 80);
	glutCreateWindow("Moving Cube");
	SetupRC();
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	glutKeyboardFunc(myKeyboard);
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
}
void RenderScene(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(0, 0, 10.0f, 0, 0, 0, 0, 1, 0);
	gluLookAt(6, 6, 6, 0, 0, 0, 0, 1, 0);
	//
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
	glRotatef(thetaX, 1, 0, 0); //旋轉
	glRotatef(thetaY, 0, 1, 0);
	glRotatef(thetaZ, 0, 0, 1);
	glTranslatef(tx, ty, tz); //平移
	//
	glColor3f(1, 1, 0);
	glutSolidCube(6);
	glutSwapBuffers();
}

void myKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'r':
		tx = 0, ty = 0, tz = 0;
		thetaX = 0, thetaY = 0, thetaZ = 0;
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
	case '.'://GLUT_KEY_PAGE_UP:
		tz += 0.2;
		break;
	case ','://GLUT_KEY_PAGE_DOWN:
		tz -= 0.2;
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
	/*case '.'://GLUT_KEY_PAGE_UP:
		tz += 0.2;
		break;
	case ','://GLUT_KEY_PAGE_DOWN:
		tz -= 0.2;
		break;*/
	default:
		break;
	}
	glutPostRedisplay();

	return;
}