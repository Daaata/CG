#include <stdio.h> 
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <vector>
#include "D://CG/OpenglLib/freeglut/include/GL/freeglut.h"

using namespace std;

#define PI 3.14159265

void SetupRC();
void ChangeSize(int, int);
void RenderScene(void);
void setPixel(int, int, float);
void menu(int);
void myMouse(int, int, int, int);
void MidpointAlgorithm(int, int, int, int);

int newx = 0, newy = 0, setsize = 10, setmode = 1;
int dx = 0, dy = 0, d = 0, delE = 0, delNE = 0;
vector <int> ck_x, ck_y;

enum MENU_TYPE
{
	MENU_TEN,
	MENU_FIFTEEN,
	MENU_TWENTY,
	MENU_NORMAL,
	MENU_DEBUG,
	MENU_RESET,
};
MENU_TYPE select = MENU_TEN;

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(600, 80);
	glutCreateWindow("Lab 07: Rasterization - Line");
	glutCreateMenu(menu); //建立選單
	glutAddMenuEntry("-10 ~ 10", MENU_TEN);
	glutAddMenuEntry("-15 ~ 15", MENU_FIFTEEN);
	glutAddMenuEntry("-20 ~ 20", MENU_TWENTY);
	glutAddMenuEntry("Normal mode", MENU_NORMAL);
	glutAddMenuEntry("Debug mode", MENU_DEBUG);
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
	glColor3f(0.95, 0.95, 0.95);
	glBegin(GL_POLYGON);
	glVertex3f(0, 0, -3);
	glVertex3f(0, 1, -3);
	glVertex3f(1, 1, -3);
	glVertex3f(1, 0, -3);
	glEnd();

	/*畫格*/
	for (int i = -25; i < 26; i++)
	{
		glColor3f(0, 0, 0); //畫X軸
		glBegin(GL_LINES);
		glVertex3f(-25, i, 0);
		glVertex3f(26, i, 0);
		glEnd();

		glColor3f(0, 0, 0); //畫Y軸
		glBegin(GL_LINES);
		glVertex3f(i, -25, 0);
		glVertex3f(i, 26, 0);
		glEnd();
	}

	/*畫點*/
	for (int i = 0; i < ck_x.size(); i++)
	{
		glColor3f(1, 0, 0);
		glBegin(GL_POLYGON);
		glVertex3f(ck_x[i], ck_y[i], -1);
		glVertex3f(ck_x[i], ck_y[i] + 1, -1);
		glVertex3f(ck_x[i] + 1, ck_y[i] + 1, -1);
		glVertex3f(ck_x[i] + 1, ck_y[i], -1);
		glEnd();
	}

	//確認是否點兩點
	if (ck_x.size()>=2) {
		//輔助線
		for (int i = 0; i < ck_x.size(); i++)
		{
			glColor3f(0.12, 0.36, 0.36);
			glBegin(GL_LINES);
			if (i == (ck_x.size() - 1) && ck_x.size() != 2)
			{
				glVertex3f(ck_x[i] + 0.5, ck_y[i] + 0.5, 0);
				glVertex3f(ck_x[0] + 0.5, ck_y[0] + 0.5, 0);
			}
			else if(i < ck_x.size()-1)
			{
				glVertex3f(ck_x[i] + 0.5, ck_y[i] + 0.5, 0);
				glVertex3f(ck_x[i + 1] + 0.5, ck_y[i + 1] + 0.5, 0);//
			}
			glEnd();
		}

		//畫線
		cout << "---" << endl;
		for (int i = 0; i < ck_x.size(); i++)
		{
			if (i == (ck_x.size() - 1) && ck_x.size() != 2)
			{
				MidpointAlgorithm(ck_x[i], ck_y[i], ck_x[0], ck_y[0]);
			}
			else if (i < ck_x.size() - 1)
			{
				MidpointAlgorithm(ck_x[i], ck_y[i], ck_x[i + 1], ck_y[i + 1]);
			}
			
		}
	}
	
	glutSwapBuffers();

	return;
}

void MidpointAlgorithm(int get_x0, int get_y0, int get_x1, int get_y1)
{
	int x0 = 0, x1 = 0, y0 = 0, y1 = 0;
	double color = 0.75;
	if (get_x1 - get_x0 != 0)
	{
		double m = (get_y1 - get_y0 * 0.1) / ((get_x1 - get_x0 * 0.1));
		double degree = atan((get_y1 - get_y0 * 0.1) / ((get_x1 - get_x0 * 0.1))) * 180 / PI;
		//cout << "m = " << m << ", deg = " << degree << endl;
		if (m >= 0 && m <=1 && get_x0 < get_x1)
		{
			cout << "Line (" << get_x0 << ", " << get_y0 << ")(" << get_x1 << ", " << get_y1 << "): region 1" << endl;
		}
		else if (m > 1 && m <= 20 && get_x0 < get_x1)
		{
			cout << "Line (" << get_x0 << ", " << get_y0 << ")(" << get_x1 << ", " << get_y1 << "): region 2" << endl;
		}
		else if (m > -1 && m < 0 && get_x0 < get_x1)
		{
			cout << "Line (" << get_x0 << ", " << get_y0 << ")(" << get_x1 << ", " << get_y1 << "): region 8" << endl;
		}
		else if (m > -20 && m <= -1 && get_x0 < get_x1)
		{
			cout << "Line (" << get_x0 << ", " << get_y0 << ")(" << get_x1 << ", " << get_y1 << "): region 7" << endl;
		}
		else if (m >= 0 && m <= 1)
		{
			cout << "Line (" << get_x0 << ", " << get_y0 << ")(" << get_x1 << ", " << get_y1 << "): region 5" << endl;
		}
		else if (m > 1 && m <= 20)
		{
			cout << "Line (" << get_x0 << ", " << get_y0 << ")(" << get_x1 << ", " << get_y1 << "): region 6" << endl;
		}
		else if (m > -1 && m < 0)
		{
			cout << "Line (" << get_x0 << ", " << get_y0 << ")(" << get_x1 << ", " << get_y1 << "): region 4" << endl;
		}
		else if (m > -20 && m <= -1)
		{
			cout << "Line (" << get_x0 << ", " << get_y0 << ")(" << get_x1 << ", " << get_y1 << "): region 3" << endl;
		}
	}
	
	if (get_x0 > get_x1)
	{
		x0 = get_x1; x1 = get_x0;
		y0 = get_y1; y1 = get_y0;
	}
	else
	{
		x0 = get_x0; x1 = get_x1;
		y0 = get_y0; y1 = get_y1;
	}

	dx = x1 - x0;
	dy = y1 - y0;

	if (x0 == x1 && y0 != y1)
	{
		int temp_x = x0, temp_y = y0;
		while (temp_y < y1)
		{
			temp_y += 1;
			if (setmode == 1) { cout << '(' << temp_x << ',' << temp_y << ')'; }
			
			setPixel(temp_x, temp_y, 0.75);
		}
		while (temp_y > y1)
		{
			temp_y -= 1;
			if (setmode == 1) { cout << '(' << temp_x << ',' << temp_y << ')'; }
			setPixel(temp_x, temp_y, 0.75);
		}
		if (setmode == 1) { cout << endl; }
	}
	else if (x0 < x1 && y0 <= y1)
	{
		d = 2 * dy - dx;
		delE = 2 * dy;
		delNE = 2 * (dy - dx);
		int dd = 2 * dx - dy;
		int delN = 2 * dx;
		int delEN = 2 * (dx - dy);
		int temp_x = x0, temp_y = y0;
		while (temp_x < x1)
		{
			if (d <= 0) 
			{ 
				d += delE; temp_x += 1; 
				color = 2;
			}
			else if (dd <= 0) 
			{ 
				dd += delN; temp_y += 1; 
				color = 0.75;
			}
			else 
			{ 
				d += delNE; dd += delEN; temp_x += 1; temp_y += 1; 
				color = 3;
			}
			if (setmode == 1) { cout << '(' << temp_x << ',' << temp_y << ')'; }
			setPixel(temp_x, temp_y, color);
		}
		while (temp_y < y1)
		{
			temp_y += 1;
			if (setmode == 1) { cout << '(' << temp_x << ',' << temp_y << ')'; }
			setPixel(temp_x, temp_y, 0.75);
		}
		if (setmode == 1) { cout << endl; }
	}
	else
	{
		dy *= -1;
		d = 2 * dy - dx;
		delE = 2 * dy;
		delNE = 2 * (dy - dx);
		int dd = 2 * dx - dy;
		int delN = 2 * dx;
		int delEN = 2 * (dx - dy);
		int temp_x = x0, temp_y = y0;
		while (temp_x < x1)
		{
			if (d <= 0) 
			{ 
				d += delE; temp_x += 1; 
				color = 2;
			}
			else if (dd <= 0) 
			{
				dd += delN; temp_y -= 1; 
				color = 0.75;
			}
			else 
			{
				d += delNE; dd += delEN; temp_x += 1; temp_y -= 1; 
				color = 3;
			}
			if (setmode == 1) { cout << '(' << temp_x << ',' << temp_y << ')'; }
			setPixel(temp_x, temp_y, color);
		}
		while (temp_y > y1)
		{
			temp_y -= 1;
			if (setmode == 1) {	cout << '(' << temp_x << ',' << temp_y << ')'; }
			setPixel(temp_x, temp_y, 0.75);
		}
		if (setmode == 1) { cout << endl; }
	}
}

void setPixel(int x, int y, float c) //畫點
{
	if (c == 1)
	{
		glColor3f(1, 0, 0);
	}
	else if (c == 2)
	{
		glColor3f(0, 1, 0);

	}
	else if (c == 3)
	{
		glColor3f(0, 0, 1);
	}
	else
	{
		glColor3f(c, c, c);
	}
	glBegin(GL_POLYGON);
	glVertex3f(x, y, -1);
	glVertex3f(x, y + 1, -1);
	glVertex3f(x + 1, y + 1, -1);
	glVertex3f(x + 1, y, -1);
	glEnd();
	return;
}

void menu(int item)
{
	switch (item)
	{
	case MENU_TEN: //設定-10~10
		setsize = 10;
		break;
	case MENU_FIFTEEN: //設定-15~15
		setsize = 15;
		break;
	case MENU_TWENTY: //設定-20~20
		setsize = 20;
		break;
	case MENU_NORMAL:
		setmode = 1;
		break;
	case MENU_DEBUG:
		setmode = 0;
		break;
	case MENU_RESET: //重設
		setmode = 1;
		setsize = 10; //預設大小
		ck_x.clear(); //清除點
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