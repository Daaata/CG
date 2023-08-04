/***C/C++  http://bbs.csdn.net/topics/100091584***/
#include <stdio.h> 
#include <stdlib.h>
#include <iostream>
#include <math.h>
/***freeglut***/
#include "D://CG/OpenglLib/freeglut/include/GL/freeglut.h"

using namespace std;

#define PI 3.14159265
#define X .525731112119133606
#define Z .850650808352039932

void SetupRC();
void ChangeSize(int, int);
void RenderScene(void);
void icoNormVec(int);
void normalize(float[3]);
void normCrossProd(float[3], float[3], float[3]);
void subdivide(GLfloat[3], GLfloat[3], GLfloat[3], int);
void drawTriangle(GLfloat*, GLfloat*, GLfloat*);
void myKeyboard(unsigned char, int, int);
void mySpecialKey(int, int, int);

static GLfloat vdata[12][3] = { {-X,0.0,Z},{X,0.0,Z},{-X,0.0,-Z},{X,0.0,-Z},
								{0.0,Z,X},{0.0,Z,-X},{0.0,-Z,X},{0.0,-Z,-X},
								{Z,X,0.0},{-Z,X,0.0},{Z,-X,0.0},{-Z,-X,0.0} };
static GLuint tindices[20][3] = { {1,4,0},{4,9,0},{4,9,5},{8,5,4},{1,8,4},
								  {1,10,8},{10,3,8},{8,3,5},{3,2,5},{3,7,2},
								  {3,10,7},{10,6,7},{6,11,7},{6,0,11},{6,1,0},
								  {10,1,6},{11,0,9},{2,11,9},{5,2,9},{11,2,7} };
GLfloat d1[3], d2[3], n[3];
GLfloat x_1 = -4, y_1 = 0, z_1 = 0, a_1 = 0;
GLfloat x_2 = 0, y_2 = 0, z_2 = 0, a_2 = 0;
GLfloat x_3 = 4, y_3 = 0, z_3 = 0, a_3 = 60;
GLfloat angle = 0;
int depth = 1, mode = 0;

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1200, 400);
	glutInitWindowPosition(600, 80);
	glutCreateWindow("Lighting and Subdivision");
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
	//glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);

	return;
}

void ChangeSize(int w, int h)
{
	printf("Window Size= %d X %d\n", w, h);
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-6, 6, -2, 2, -10, 10);
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
	gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
	
	
	
	/*Flat*/
	glShadeModel(GL_FLAT);
	glPushMatrix();
	glTranslatef(-4, 0, 0);
	glRotatef(angle, x_1, y_1, 0);
	if(mode==0){ glBegin(GL_TRIANGLES); }
	else{ glBegin(GL_LINE_LOOP); }
	glColor3f(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 20; i++) {
		icoNormVec(i);
		glVertex3fv(&vdata[tindices[i][0]][0]);
		glVertex3fv(&vdata[tindices[i][1]][0]);
		glVertex3fv(&vdata[tindices[i][2]][0]);
	}
	glEnd();
	glPopMatrix();
	
	
	
	/*Interpolate*/
	glPushMatrix();
	glTranslatef(0, 0, 0);
	glRotatef(angle, x_2, y_2, 0);
	if (mode == 0) { glBegin(GL_TRIANGLES); }
	else { glBegin(GL_LINE_LOOP); }
	glColor3f(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 20; i++) {
		glNormal3fv(&vdata[tindices[i][0]][0]);
		glVertex3fv(&vdata[tindices[i][0]][0]);
		glNormal3fv(&vdata[tindices[i][1]][0]);
		glVertex3fv(&vdata[tindices[i][1]][0]);
		glNormal3fv(&vdata[tindices[i][2]][0]);
		glVertex3fv(&vdata[tindices[i][2]][0]);
	}
	glEnd();
	glPopMatrix();


	
	/*Subdivide*/
	glPushMatrix();
	glTranslatef(4, 0, 0);
	glRotatef(angle, x_3, y_3, 0);
	for (int i = 0; i < 20; i++) {
		icoNormVec(i);
		subdivide(&vdata[tindices[i][0]][0], &vdata[tindices[i][1]][0], &vdata[tindices[i][2]][0], depth);
	}
	glFlush();
	glPopMatrix();
	
	glutSwapBuffers();

	return;
}

void icoNormVec(int k)
{
	for (int i = 0; i < 3; i++)
	{
		d1[i] = vdata[tindices[k][0]][i] - vdata[tindices[k][1]][i];
		d2[i] = vdata[tindices[k][1]][i] - vdata[tindices[k][2]][i];
	}
	normCrossProd(d1, d2, n);
	glNormal3fv(n);
	return;
}

void normalize(float v[3])
{
	GLfloat d = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	v[0] /= d; v[1] /= d; v[2] /= d;
}

void normCrossProd(float u[3], float v[3], float out[3])
{
	out[0] = u[1] * v[2] - u[2] * v[1];
	out[1] = u[2] * v[0] - u[0] * v[2];
	out[2] = u[0] * v[1] - u[1] * v[0];
	normalize(out);
}

void subdivide(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3], int depth)
{
	GLfloat v12[3], v23[3], v31[3]; int i;
	if (depth == 0) {
		drawTriangle(v1, v2, v3);
		return;
	}
	for (i = 0; i < 3; i++) {
		v12[i] = (v1[i] + v2[i]) / 2.0;
		v23[i] = (v2[i] + v3[i]) / 2.0;
		v31[i] = (v3[i] + v1[i]) / 2.0;
	}
	normalize(v12);
	normalize(v23);
	normalize(v31);
	subdivide(v1, v12, v31, depth - 1);
	subdivide(v2, v23, v12, depth - 1);
	subdivide(v3, v31, v23, depth - 1);
	subdivide(v12, v23, v31, depth - 1);
}

void drawTriangle(GLfloat* v1, GLfloat* v2, GLfloat* v3)
{
	if (mode == 0) { glBegin(GL_TRIANGLES); }
	else { glBegin(GL_LINE_LOOP); }
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3fv(v1);
	glVertex3fv(v2);
	glVertex3fv(v3);
	glEnd();
}

void myKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'r': //重設
		x_1 = -4, y_1 = 0, z_1 = 0, a_1 = 0;
		x_2 = 0, y_2 = 0, z_2 = 0, a_2 = 0;
		x_3 = 4, y_3 = 0, z_3 = 0, a_3 = 60;
		angle = 0;
		depth = 1;
		mode = 0;
		break;
	case '+': //Depth設定
		depth += 1;
		break;
	case '-':
		depth -= 1;
		if(depth < 0){depth = 0; }
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
	case GLUT_KEY_LEFT: //Y軸旋轉
		x_1 = -4;
		x_2 = 0;
		x_3 = 4;
		y_3 = y_2 = y_1 = -65535;
		angle += 5;
		break;
	case GLUT_KEY_RIGHT:
		x_1 = -4;
		x_2 = 0;
		x_3 = 4;
		y_3 = y_2 = y_1 = -65535;
		angle -= 5;
		break;
	case GLUT_KEY_UP: //X軸旋轉
		y_3 = y_2 = y_1 = 0; 
		x_3 = x_2 = x_1 = -65535;
		angle += 5;
		break;
	case GLUT_KEY_DOWN:
		y_3 = y_2 = y_1 = 0;
		x_3 = x_2 = x_1 = -65535;
		angle -= 5;
		break;
	case GLUT_KEY_PAGE_UP: //mode選擇
		mode = 0;
		break;
	case GLUT_KEY_PAGE_DOWN:
		mode = 1;
		break;
	default:
		break;
	}
	glutPostRedisplay();

	return;
}