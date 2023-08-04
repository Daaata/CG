/***C/C++  http://bbs.csdn.net/topics/100091584***/
#include <stdio.h> 
#include <stdlib.h>
#include <math.h>
/***freeglut***/
#include "D://CG/OpenglLib/freeglut/include/GL/freeglut.h"

#define PI 3.14159265

void SetupRC();
void ChangeSize(int, int);
void RenderScene(void);
//void myTranslate(double, double, double);
void myKeyboard(unsigned char, int, int);
void mySpecialKey(int, int, int);

double tx = 0, ty = 0, tz = 0, thetaX, thetaY, thetaZ, s=1;
/*GLfloat rotMatrix[] = { 1.0,0.0,0.0,0.0,
						0.0,1.0,0.0,0.0,
						0,0,0.0,1.0,0.0,
						0.0,0.0,0.0,1.0 };*/
GLfloat scaMatrix[] = { 1.0,0.0,0.0,0.0,
						0.0,1.0,0.0,0.0,
						0,0,0.0,1.0,0.0,
						0.0,0.0,0.0,1.0 };
GLfloat rotMatriX[] = { 1.0,0.0,0.0,0.0,
						0.0,1.0,0.0,0.0,
						0,0,0.0,1.0,0.0,
						0.0,0.0,0.0,1.0 };
GLfloat rotMatriY[] = { 1.0,0.0,0.0,0.0,
						0.0,1.0,0.0,0.0,
						0,0,0.0,1.0,0.0,
						0.0,0.0,0.0,1.0 };
GLfloat rotMatriZ[] = { 1.0,0.0,0.0,0.0,
						0.0,1.0,0.0,0.0,
						0,0,0.0,1.0,0.0,
						0.0,0.0,0.0,1.0 };
GLfloat translateMatrix[] = { 1.0,0.0,0.0,0.0,
							  0.0,1.0,0.0,0.0,
							  0,0,0.0,1.0,0.0,
							  0.0,0.0,0.0,1.0 };

int main(int argc, char** argv)
{
	tx = 0, ty = 0, tz = 0;
	thetaX = 0, thetaY = 0, thetaZ = 0;
	s = 1;

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

	//return;
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
	gluLookAt(2, 2, 10.0f, 0, 0, 0, 0, 1, 0);
	//gluLookAt(6, 6, 6, 0, 0, 0, 0, 1, 0);
	//
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//
	
	//
	//glLoadMatrixf(rotMatrix);
	//glMultMatrixf(translateMatrix);
	//glRotatef(thetaX, 1, 0, 0); //旋轉
	//glRotatef(thetaY, 0, 1, 0);
	//glRotatef(thetaZ, 0, 0, 1);
	//glTranslatef(tx, ty, tz); //平移
	//myTranslate(tx, ty, tz); //平移
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
	//
	double gin = PI / 180.0;

	scaMatrix[0] = s;
	scaMatrix[5] = s;
	scaMatrix[10] = s;
	
	translateMatrix[12] = tx;
	translateMatrix[13] = ty;
	translateMatrix[14] = tz;

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
	
	glMultMatrixf(scaMatrix);
	glMultMatrixf(rotMatriX);
	glMultMatrixf(rotMatriY);
	glMultMatrixf(rotMatriZ);
	glMultMatrixf(translateMatrix);
	//glLoadMatrixf(translateMatrix);
	//printf("%f", tx);
	//
	glColor3f(1, 1, 0);
	glutSolidCube(12);
	glutSwapBuffers();
}

/*void myRotate()
{
	//
}

void myTranslate(double x, double y, double z)
{
	//
	GLfloat rotMatrix[] = { 1.0,0.0,0.0,x,
							0.0,1.0,0.0,y,
							0,0,1.0,0.0,z,
							0.0,0.0,0.0,1.0 };
}

void myScale()
{
	//
}*/

void myKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'r':
		tx = 0, ty = 0, tz = 0;
		thetaX = 0, thetaY = 0, thetaZ = 0;
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
	case '+':
		s *= 2;
		break;
	case '-':
		s *= 0.5;
		break;
	default:
		break;
	}
	glutPostRedisplay();

	//return;
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

	//return;
}