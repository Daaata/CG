/***C/C++  http://bbs.csdn.net/topics/100091584***/
#include <stdio.h> 
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <vector>
/***freeglut***/
#include "D://CG/OpenglLib/freeglut/include/GL/freeglut.h"

using namespace std;

#define PI 3.14159265

void SetupRC();
void ChangeSize(int, int);
void RenderScene(void);
void menu(int);
void myMouse(int, int, int, int);

int newx = 0, newy = 0, setsize = 10;
vector <int> ck_x, ck_y;

enum MENU_TYPE
{
	MENU_TEN,
	MENU_FIFTEEN,
	MENU_TWENTY,
	MENU_RESET,
};
MENU_TYPE select = MENU_TEN;

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(600, 80);
	glutCreateWindow("2D Pixel Grid");
	glutCreateMenu(menu); //建立選單
	glutAddMenuEntry("10 * 10", MENU_TEN);
	glutAddMenuEntry("15 * 15", MENU_FIFTEEN);
	glutAddMenuEntry("20 * 20", MENU_TWENTY);
	glutAddMenuEntry("Reset", MENU_RESET);
	SetupRC();
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	glutMouseFunc(myMouse);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
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
	glOrtho(-1 * setsize, setsize + 1, -1 * setsize, setsize + 1, -10, 20);
	gluLookAt(0, 0, 6.0f, 0, 0, 0, 0, 1, 0);

	/*原點參考點*/
	glColor3f(0.87, 0.87, 0.87);
	glBegin(GL_POLYGON);
	glVertex3f(0, 0, -3);
	glVertex3f(0, 1, -3);
	glVertex3f(1, 1, -3);
	glVertex3f(1, 0, -3);
	glEnd();

	/*畫格*/
	for (int i = -25; i < 26; i++)
	{
		glColor3f(0.5, 0.12, 0); //畫X軸
		glBegin(GL_LINES);
		glVertex3f(-25, i, 0);
		glVertex3f(26, i, 0);
		glEnd();

		glColor3f(0, 0.5, 0.12); //畫Y軸
		glBegin(GL_LINES);
		glVertex3f(i, -25, 0);
		glVertex3f(i, 26, 0);
		glEnd();
	}

	/*畫點*/
	for (int i = 0; i < ck_x.size(); i++)
	{
		glColor3f(0, 0.5, 0.12);
		glBegin(GL_POLYGON);
		glVertex3f(ck_x[i], ck_y[i], -1);
		glVertex3f(ck_x[i], ck_y[i] + 1, -1);
		glVertex3f(ck_x[i] + 1, ck_y[i] + 1, -1);
		glVertex3f(ck_x[i] + 1, ck_y[i], -1);
		glEnd();
	}

	glutSwapBuffers();

	return;
}

void menu(int item)
{
	switch (item)
	{
	case MENU_TEN: //設定10*10
		setsize = 10;
		ck_x.clear(); //清除點
		ck_y.clear();
		break;
	case MENU_FIFTEEN: //設定15*15
		setsize = 15;
		ck_x.clear(); //清除點
		ck_y.clear();
		break;
	case MENU_TWENTY: //設定20*20
		setsize = 20;
		ck_x.clear(); //清除點
		ck_y.clear();
		break;
	case MENU_RESET: //重設(僅清除點)
		ck_x.clear();
		ck_y.clear();
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
	case GLUT_LEFT_BUTTON:  //取得點
		if (state == GLUT_UP)
		{
			newx = int(round((2 * x * setsize) / 400.0 - setsize)); //計算
			newy = int(round(setsize - (2 * y * setsize) / 400.0));
			ck_x.push_back(newx); //填入點
			ck_y.push_back(newy);
			cout << "Mouse left button down, and get point (" << newx << "," << newy << ')' << endl;
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