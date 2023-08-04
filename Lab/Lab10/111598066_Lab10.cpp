#include <stdio.h> 
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <time.h>
#include "D://CG/OpenglLib/freeglut/include/GL/freeglut.h"

using namespace std;

#define PI 3.14159265

void SetupRC();
void ChangeSize(int, int);
void RenderScene(void);
void setPixel(int, int, float);
void setPixel(int, int, float, float, float);
void menu(int);
void myMouse(int, int, int, int);
void CrowsAlgorithm(int);
void Timer(int);
void MidpointAlgorithm(int, int, int, int, int, int);

int newx = 0, newy = 0, setsize = 10, setmode = 0, setdraw = 0;
int dx = 0, dy = 0, d = 0, delE = 0, delNE = 0;
int drawbox[41][41];
float drawR[41][41], drawG[41][41], drawB[41][41];
bool AnimationSet = true;
vector <int> ck_x, ck_y;
vector <double> ck_m, ck_c;
vector <double> get_colorR, get_colorG, get_colorB;
vector <int> temp_Linex, temp_Liney;

enum MENU_TYPE
{
	MENU_TEN,
	MENU_FIFTEEN,
	MENU_TWENTY,
	MENU_EDGE,
	MENU_POLYGON,
	MENU_RESET,
};
MENU_TYPE select = MENU_TEN;

int main(int argc, char** argv)
{
	srand(time(NULL));
	for (int i = 0; i < 41; i++)
	{
		for (int j = 0; j < 41; j++)
		{
			drawbox[i][j] = 0;
			drawR[i][j] = 0.0;
			drawG[i][j] = 0.0;
			drawB[i][j] = 0.0;
		}
	}
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(600, 80);
	glutCreateWindow("Lab 10: Crow’s Algorithm");
	glutCreateMenu(menu); //建立選單
	glutAddMenuEntry("-10 ~ 10", MENU_TEN);
	glutAddMenuEntry("-15 ~ 15", MENU_FIFTEEN);
	glutAddMenuEntry("-20 ~ 20", MENU_TWENTY);
	glutAddMenuEntry("Draw Edge", MENU_EDGE);
	glutAddMenuEntry("Draw Polygon", MENU_POLYGON);
	glutAddMenuEntry("Reset", MENU_RESET);
	SetupRC();
	glutTimerFunc(33, CrowsAlgorithm, 1);
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

void Timer(int i)
{
	if (AnimationSet)
	{
		glutPostRedisplay();
	}

	glutTimerFunc(33, CrowsAlgorithm, 1);

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
		glColor3f(get_colorR[i]/255, get_colorG[i] / 255, get_colorB[i] / 255);
		glBegin(GL_POLYGON);
		glVertex3f(ck_x[i], ck_y[i], -1);
		glVertex3f(ck_x[i], ck_y[i] + 1, -1);
		glVertex3f(ck_x[i] + 1, ck_y[i] + 1, -1);
		glVertex3f(ck_x[i] + 1, ck_y[i], -1);
		glEnd();
		drawbox[ck_x[i] + 20][ck_y[i] + 20] = 1;
		drawR[ck_x[i] + 20][ck_y[i] + 20] = get_colorR[i] / 255;
		drawG[ck_x[i] + 20][ck_y[i] + 20] = get_colorG[i] / 255;
		drawB[ck_x[i] + 20][ck_y[i] + 20] = get_colorB[i] / 255;
	}

	//確認是否點兩點
	if (setdraw!=0 && ck_x.size() >= 2) {
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
			else if (i < ck_x.size() - 1)
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
				MidpointAlgorithm(ck_x[i], ck_y[i], ck_x[0], ck_y[0], i, 0);
			}
			else if (i < ck_x.size() - 1)
			{
				MidpointAlgorithm(ck_x[i], ck_y[i], ck_x[i + 1], ck_y[i + 1], i, i+1);
			}

		}

		if (setdraw==2 && ck_x.size() >= 3)
		{
			CrowsAlgorithm(1);
			AnimationSet = false;
		}
	}

	glutSwapBuffers();
	return;
}

void CrowsAlgorithm(int i)
{
	int start_x, end_x, check = 0;
	
	for (int i = 0; i < 41; i++)
	{
		for (int j = 0; j < 41; j++)
		{
			if (j < 40 && drawbox[j][i] == 1 && drawbox[j + 1][i] == 1 && check < 2)
			{
				check += 0;
			}
			else if (drawbox[j][i] == 1 && check == 0)
			{
				start_x = j;
				check++;
			}
			else if (drawbox[j][i] == 1 && check == 1)
			{
				end_x = j;
				check++;
			}

			if (check == 2)
			{
				float gapR = (drawR[end_x][i] - drawR[start_x][i]) / (end_x - start_x);
				float gapG = (drawG[end_x][i] - drawG[start_x][i]) / (end_x - start_x);
				float gapB = (drawB[end_x][i] - drawB[start_x][i]) / (end_x - start_x);
				for (int k = start_x; k < end_x; k++)
				{
					glMatrixMode(GL_MODELVIEW);     // To operate on Model-View matrix
					glLoadIdentity();
					glOrtho(-1 * setsize, setsize + 1, -1 * setsize, setsize + 1, -10, 20);
					gluLookAt(0, 0, 6.0f, 0, 0, 0, 0, 1, 0);
					glPushMatrix();
					setPixel(k - 20, i - 20, (drawR[start_x][i] +gapR*(k-start_x+1)), (drawG[start_x][i] + gapG * (k - start_x + 1)), (drawB[start_x][i] + gapB * (k - start_x + 1))); //上色
					glPopMatrix();
					Sleep(12.5);
					glutSwapBuffers();
				}
				check = 0;
			}
		}
		check = 0;
	}

	check = 0;
	AnimationSet = false;
	return;
}

void MidpointAlgorithm(int get_x0, int get_y0, int get_x1, int get_y1, int getcolor1, int getcolor2)
{
	int x0 = 0, x1 = 0, y0 = 0, y1 = 0;
	int color = 0;
	if (get_x1 - get_x0 != 0)
	{
		double m = (get_y1 - get_y0 * 1.0) / ((get_x1 - get_x0 * 1.0));
		double degree = atan((get_y1 - get_y0 * 1.0) / ((get_x1 - get_x0 * 1.0))) * 180 / PI;
		ck_m.push_back(m);
		ck_c.push_back(get_y0 * 1.0 - m * (get_x0 * 1.0));
		if (m >= 0 && m <= 1 && get_x0 < get_x1)
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
		color = getcolor1;
		getcolor1 = getcolor2;
		getcolor2 = color;
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
			drawbox[temp_x + 20][temp_y + 20] = 1;
			temp_Linex.push_back(temp_x);
			temp_Liney.push_back(temp_y);
		}
		while (temp_y > y1)
		{
			temp_y -= 1;
			if (setmode == 1) { cout << '(' << temp_x << ',' << temp_y << ')'; }
			drawbox[temp_x + 20][temp_y + 20] = 1;
			temp_Linex.push_back(temp_x);
			temp_Liney.push_back(temp_y);
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
			drawbox[temp_x + 20][temp_y + 20] = 1;
			temp_Linex.push_back(temp_x);
			temp_Liney.push_back(temp_y);
		}
		while (temp_y < y1)
		{
			temp_y += 1;
			if (setmode == 1) { cout << '(' << temp_x << ',' << temp_y << ')'; }
			drawbox[temp_x + 20][temp_y + 20] = 1;
			temp_Linex.push_back(temp_x);
			temp_Liney.push_back(temp_y);
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
			drawbox[temp_x + 20][temp_y + 20] = 1;
			temp_Linex.push_back(temp_x);
			temp_Liney.push_back(temp_y);
		}
		while (temp_y > y1)
		{
			temp_y -= 1;
			if (setmode == 1) { cout << '(' << temp_x << ',' << temp_y << ')'; }
			drawbox[temp_x + 20][temp_y + 20] = 1;
			temp_Linex.push_back(temp_x);
			temp_Liney.push_back(temp_y);
		}
		if (setmode == 1) { cout << endl; }
	}

	float gapR = (get_colorR[getcolor2] - get_colorR[getcolor1])/ temp_Linex.size();
	float gapG = (get_colorG[getcolor2] - get_colorG[getcolor1]) / temp_Linex.size();
	float gapB = (get_colorB[getcolor2] - get_colorB[getcolor1]) / temp_Linex.size();

	for (int i = 0; i < temp_Linex.size(); i++)
	{
		setPixel(temp_Linex[i], temp_Liney[i], (get_colorR[getcolor1]+gapR*(i+1))/255.0, (get_colorG[getcolor1] + gapG * (i + 1)) / 255.0, (get_colorB[getcolor1] + gapB * (i + 1)) / 255.0);
		drawR[temp_Linex[i] + 20][temp_Liney[i] + 20] = (get_colorR[getcolor1] + gapR * (i + 1)) / 255.0;
		drawG[temp_Linex[i] + 20][temp_Liney[i] + 20] = (get_colorG[getcolor1] + gapG * (i + 1)) / 255.0;
		drawB[temp_Linex[i] + 20][temp_Liney[i] + 20] = (get_colorB[getcolor1] + gapB * (i + 1)) / 255.0;
	}
	temp_Linex.clear();
	temp_Liney.clear();

	return;
}

void setPixel(int x, int y, float r, float g, float b) //畫點
{
	glColor3f(r, g, b);
	glBegin(GL_POLYGON);
	glVertex3f(x, y, -1);
	glVertex3f(x, y + 1, -1);
	glVertex3f(x + 1, y + 1, -1);
	glVertex3f(x + 1, y, -1);
	glEnd();
	return;
}

void setPixel(int x, int y, float c) //畫點
{
	if (c == 4)
	{
		glColor3f(66.0 / 255, 245.0 / 255, 233.0 / 255);
		glBegin(GL_POLYGON);
		glVertex3f(x, y, -1);
		glVertex3f(x, y + 1, -1);
		glVertex3f(x + 1, y + 1, -1);
		glVertex3f(x + 1, y, -1);
		glEnd();

		return;
	}
	
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
	else if (c == 4)
	{
		glColor3f(66.0 / 255, 245.0 / 255, 233.0 / 255);
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
	case MENU_EDGE:
		setdraw = 1;
		break;
	case MENU_POLYGON:
		setdraw = 2;
		break;
	case MENU_RESET: //重設
		setdraw = 0;
		AnimationSet = true;
		setsize = 10; //預設大小
		ck_x.clear(); //清除點
		ck_y.clear();
		ck_m.clear();
		ck_c.clear();
		get_colorR.clear();
		get_colorG.clear();
		get_colorB.clear();
		for (int i = 0; i < 41; i++)
		{
			for (int j = 0; j < 41; j++)
			{
				drawbox[i][j] = 0;
				drawR[i][j] = 0.0;
				drawG[i][j] = 0.0;
				drawB[i][j] = 0.0;
			}
		}
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
			get_colorR.push_back(rand() % 255);
			get_colorG.push_back(rand() % 255);
			get_colorB.push_back(rand() % 255);
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