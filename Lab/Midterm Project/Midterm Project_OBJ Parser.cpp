#include "D://CG/OpenglLib/freeglut/include/GL/freeglut.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

#define PI 3.14159265

string Location_Path = "D:\\CG\\Lab\\midterm\\Project1\\";
string OBJ_Path;
vector<vector<float>> Vertex_Index;
vector<vector<float>> Color;
vector<vector<float>> Vertex;
int Resize = 0, tx = 0, ty = 0, tz = 0;
int PolygonSet = 1, BoundingSet = 0, ColorMode = 0;
double BoundBox_X = 0, BoundBox_Y = 0, BoundBox_Z = 0, BoundBox_Xb = 0, BoundBox_Yb = 0, BoundBox_Zb = 0;
double newx = 0, newy = 0, newz = 0, newangle = 0, newtemp, newsize, rx = 0, ry = 0, rz = 0, newx1 = 0, newy1 = 0, newz1 = 0, ox = 0.0, oy = 0.0, oz =5.0, thetaX=0, thetaY=0, thetaZ=0, Scale = 1;
GLfloat d1[3], d2[3], n[3];

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

void ChangeSize(int, int);
void RenderScene(void);
void SetupRC(void);
void myMouse(int, int, int, int);
void myKeyboard(unsigned char, int, int);
void mySpecialKey(int, int, int);
void createMenu(void);
void MainMenu(int);
void SubMenu1(int);
void SubMenu2(int);
void SubMenu3(int);
void LoadOBJ(void);
void icoNormVec(int);
void normalize(float v[3]);
void normCrossProd(float u[3], float v[3], float out[3]);
void GetBoundingBox(double x, double y, double z);
void SetupResize(void);

void LoadOBJ()
{
	char temp_string[256];
	float X_Y_Z[3];
	FILE *file = fopen((Location_Path + OBJ_Path).c_str(), "r");
	cout << (Location_Path + OBJ_Path).c_str() << endl;

	if (file == NULL) 
	{
		printf("Impossible to open the file !");
		return;
	}

	while (1) {
		int eof = fscanf(file, "%s", temp_string);

		if (eof == EOF) 
		{
			break;
		}

		if (strcmp(temp_string, "v") == 0)
		{
			vector<float> temp_vertices;
			vector<float> temp_colors;

			fscanf(file, "%f %f %f", &X_Y_Z[0], &X_Y_Z[1], &X_Y_Z[2]);
			GetBoundingBox(X_Y_Z[0], X_Y_Z[1], X_Y_Z[2]);

			for (int i = 0; i < 3; i++) 
			{
				temp_vertices.push_back(X_Y_Z[i]);
			} 

			Vertex.push_back(temp_vertices);
			X_Y_Z[0] = (double)rand() / (RAND_MAX + 1.1);
			X_Y_Z[1] = (double)rand() / (RAND_MAX + 1.1);
			X_Y_Z[2] = (double)rand() / (RAND_MAX + 1.1);

			for (int i = 0; i < 3; i++) 
			{
				temp_colors.push_back(X_Y_Z[i]);
			} 

			Color.push_back(temp_colors);
		}
		else if (strcmp(temp_string, "f") == 0)
		{
			vector<float> temp_vertices;

			fscanf(file, "%f %f %f", &X_Y_Z[0], &X_Y_Z[1], &X_Y_Z[2]);

			for (int i = 0; i < 3; i++) 
			{
				temp_vertices.push_back(X_Y_Z[i]);
			} 

			Vertex_Index.push_back(temp_vertices);
		}
	}

	fclose(file);
	Resize = 1;

	return;
}

void icoNormVec(int i)
{
	vector<float>  ::iterator iter = Vertex_Index[i].begin();

	for (int k = 0; k < 3; k++) 
	{
		d1[k] = Vertex[*iter - 1][k] - Vertex[*(iter + 1) - 1][k];
		d2[k] = Vertex[*(iter + 1) - 1][k] - Vertex[*(iter + 2) - 1][k];
	}

	normCrossProd(d1, d2, n);
	glNormal3fv(n);

	return;
}

void normalize(float v[3])
{
	GLfloat d = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	v[0] /= d; v[1] /= d; v[2] /= d;

	return;
}

void normCrossProd(float u[3], float v[3], float out[3])
{
	out[0] = u[1] * v[2] - u[2] * v[1];
	out[1] = u[2] * v[0] - u[0] * v[2];
	out[2] = u[0] * v[1] - u[1] * v[0];
	normalize(out);

	return;
}

void GetBoundingBox(double x, double y, double z)
{
	if (BoundBox_X < x)
	{
		BoundBox_X = x;
	}
	else if (BoundBox_Xb > x) 
	{ 
		BoundBox_Xb = x; 
	}

	if (BoundBox_Y < y) 
	{ 
		BoundBox_Y = y; 
	}
	else if (BoundBox_Yb > y)
	{
		BoundBox_Yb = y;
	}

	if (BoundBox_Z < z)
	{
		BoundBox_Z = z;
	}
	else if (BoundBox_Zb > z)
	{
		BoundBox_Zb = z;
	}

	return;
}

void SetupResize()
{
	if (Resize == 1) 
	{
		double resize_mode = max(max(abs(BoundBox_X), abs(BoundBox_Xb)), max(max(abs(BoundBox_Y), abs(BoundBox_Yb)), max(abs(BoundBox_Z), abs(BoundBox_Zb))));
		double temp = abs(BoundBox_X) + abs(BoundBox_Xb);

		if (resize_mode == abs(BoundBox_Xb) || resize_mode == abs(BoundBox_X)) 
		{
			if (abs(BoundBox_X) + abs(BoundBox_Xb) > 50) 
			{
				while (temp - (abs(BoundBox_X) + abs(BoundBox_Xb)) / 10 > 50)
				{
					Scale -= 0.1;
					temp -= (abs(BoundBox_X) + abs(BoundBox_Xb)) / 10;
				}
			}
			else if (abs(BoundBox_X) + abs(BoundBox_Xb) < 40) 
			{
				while (temp + (abs(BoundBox_X) + abs(BoundBox_Xb)) / 10 < 40)
				{
					Scale += 0.1;
					temp += (abs(BoundBox_X) + abs(BoundBox_Xb)) / 10;
				}
			}
		}
		else if (resize_mode == abs(BoundBox_Yb) || resize_mode == abs(BoundBox_Y)) 
		{
			temp = abs(BoundBox_Y) + abs(BoundBox_Yb);

			if (abs(BoundBox_Y) + abs(BoundBox_Yb) > 40) 
			{
				while (temp - (abs(BoundBox_Y) + abs(BoundBox_Yb)) / 10 > 404)
				{
					Scale -= 0.1;
					temp -= (abs(BoundBox_Y) + abs(BoundBox_Yb)) / 10;
				}
			}
			else if (abs(BoundBox_Y) + abs(BoundBox_Yb) < 30) 
			{
				while (temp + (abs(BoundBox_Y) + abs(BoundBox_Yb)) / 10 < 30)
				{
					Scale += 0.1;
					temp += (abs(BoundBox_Y) + abs(BoundBox_Yb)) / 10;
				}
			}
		}
		else if (resize_mode == abs(BoundBox_Zb) || resize_mode == abs(BoundBox_Z)) 
		{
			temp = abs(BoundBox_Z) + abs(BoundBox_Zb);

			if (abs(BoundBox_Z) + abs(BoundBox_Zb) > 40) 
			{
				while (temp - (abs(BoundBox_Z) + abs(BoundBox_Zb)) / 10 > 40)
				{
					Scale -= 0.1;
					temp -= (abs(BoundBox_Z) + abs(BoundBox_Zb)) / 10;
				}
			}
			else if (abs(BoundBox_Z) + abs(BoundBox_Zb) < 30) 
			{
				while (temp + (abs(BoundBox_Z) + abs(BoundBox_Zb)) / 10 < 30)
				{
					Scale += 0.1;
					temp += (abs(BoundBox_Z) + abs(BoundBox_Zb)) / 10;
				}
			}
		}
		Resize = 0;
	}

	return;
}

void createMenu(void)
{
	int subMenu1 = glutCreateMenu(SubMenu1);
	glutAddMenuEntry("Gourd", 1);
	glutAddMenuEntry("Teddy", 2);
	glutAddMenuEntry("Teapot", 3);
	glutAddMenuEntry("Octahedron", 4);
	glutAddMenuEntry("Load An OBJ File...", 5);

	int subMenu2 = glutCreateMenu(SubMenu2);
	glutAddMenuEntry("Point", 1);
	glutAddMenuEntry("Line", 2);
	glutAddMenuEntry("Face", 3);

	int subMenu3 = glutCreateMenu(SubMenu3);
	glutAddMenuEntry("Single Color", 1);
	glutAddMenuEntry("Random Color", 2);

	int mainMenu = glutCreateMenu(MainMenu);
	glutAddSubMenu("File", subMenu1);
	glutAddSubMenu("Render Mode", subMenu2);
	glutAddSubMenu("Color Mode", subMenu3);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	return;
}

void MainMenu(int index)
{
	switch (index)
	{
	default:
		break;
	}

	glutPostRedisplay();
	return;
}


void SubMenu1(int index)
{
	switch (index)
	{
	case 1:
		OBJ_Path = "gourd.obj";
		break;
	case 2:
		OBJ_Path = "teddy.obj";
		break;
	case 3:
		OBJ_Path = "teapot.obj";
		break;
	case 4:
		OBJ_Path = "octahedron.obj";
		break;
	case 5:
		cout << "OBJ檔須在 " << Location_Path << " 裡，";
		cout << "請輸入完整檔名(包含副檔名)：";
		cin >> OBJ_Path;
		FILE* file = fopen((Location_Path + OBJ_Path).c_str(), "r");
		while (file == NULL)
		{
			printf("錯誤！請重新輸入檔名\n");
			cin >> OBJ_Path;
			file = fopen((Location_Path + OBJ_Path).c_str(), "r");
		}
		fclose(file);
		break;
	}
	Resize = Scale = 1;
	tx = ty = tz = thetaX = thetaY = thetaZ = BoundBox_X = BoundBox_Xb = BoundBox_Y = BoundBox_Yb = BoundBox_Z = BoundBox_Zb = ox = oy = oz = 0;
	oz = 5.0;
	Vertex.clear();
	Vertex_Index.clear();
	LoadOBJ();
	glutPostRedisplay();

	return;
}

void SubMenu2(int index)
{
	switch (index)
	{
	case 1:
		PolygonSet = 0;
		break;
	case 2:
		PolygonSet = 1;
		break;
	case 3:
		PolygonSet = 2;
		break;
	default:
		break;
	}
	glutPostRedisplay();

	return;
}

void SubMenu3(int index)
{
	switch (index)
	{
	case 1:
		ColorMode = 0;
		break;
	case 2:
		ColorMode = 1;
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
	case GLUT_KEY_LEFT: //X軸平移
		tx -= 2;
		break;
	case GLUT_KEY_RIGHT:
		tx += 2;
		break;
	case GLUT_KEY_UP: //Y軸平移
		ty += 2;
		break;
	case GLUT_KEY_DOWN:
		ty -= 2;
		break;
	case GLUT_KEY_PAGE_UP: //Z軸平移
		tz += 2;
		break;
	case GLUT_KEY_PAGE_DOWN:
		tz -= 2;
		break;
	default:
		break;
	}
	glutPostRedisplay();

	return;
}

void myKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a': //X軸旋轉
		thetaX += 3;
		break;
	case 'd':
		thetaX -= 3;
		break;
	case 'w': //Y軸旋轉
		thetaY += 3;
		break;
	case 's':
		thetaY -= 3;
		break;
	case 'z': //Z軸旋轉
		thetaZ -= 3;
		break;
	case 'x':
		thetaZ += 3;
		break;
	case 'n': //自定軸旋轉
		newangle -= 3;
		break;
	case 'm':
		newangle += 3;
		break;
	case '4': //鏡頭調整
		if (ox < 10)
		{
			ox += 1.0;
		}
		break;
	case '5':
		if (oy < 10)
		{
			oy += 1.0;
		}
		break;
	case '6':
		if (oz < 10)
		{
			oz += 1.0;
		}
		break;
	case '1':
		if (ox > -10)
		{
			ox -= 1.0;
		}
		break;
	case '2':
		if (oy > -10)
		{
			oy -= 1.0;
		}
		break;
	case '3':
		if (oz > -10)
		{
			oz -= 1.0;
		}
		break;
	}
	glutPostRedisplay();

	return;
}

void myMouse(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON: //取得點
		if (state == GLUT_UP)
		{
			newx1 = newx;
			newy1 = newy;
			newx = (x * 100.0) / 1200.0 - 50;
			newy = 30 - (y * 60.0) / 760.0;
			cout << "Mouse left button down, and get point (" << newx << "," << newy << ')' << endl;
		}
		break;
	default:
		break;
	}
	return;
}

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPushMatrix();
	gluLookAt(ox,oy,oz, 0.0+ox,0.0+oy,-1.0+oz, 0.0,0.1,0.0);

	glColor3f(0.5, 0.12, 0); //畫X軸
	glBegin(GL_LINES);
	glVertex3f(-100, 0, 0);
	glVertex3f(100, 0, 0);
	glEnd();
	glColor3f(0, 0.5, 0.12); //畫Y軸
	glBegin(GL_LINES);
	glVertex3f(0, -100, 0);
	glVertex3f(0, 100, 0);
	glEnd();
	glColor3f(0.12, 0.12, 0.5); //畫Z軸
	glBegin(GL_LINES);
	glVertex3f(0, 0, -100);
	glVertex3f(0, 0, 100);
	glEnd();

	glColor3f(0.5, 0.5, 0.5); //畫點
	glPointSize(10);
	glBegin(GL_POINTS);
	glVertex3f(newx1, newy1, newz1);
	glEnd();
	glBegin(GL_POINTS);
	glVertex3f(newx, newy, newz);
	glEnd();

	glColor3f(0.5, 0.5, 0.5); //畫線
	glBegin(GL_LINES);
	glVertex3f(newx1, newy1, 0.0);
	glVertex3f(newx, newy, 0.0);
	glEnd();

	/*平移*/
	translateMatrix[12] = tx;
	translateMatrix[13] = ty;
	translateMatrix[14] = tz;
	
	/*旋轉*/
	double gin = PI / 180.0;
	rotMatriX[5] = cos(thetaX * gin);
	rotMatriX[6] = sin(thetaX * gin);
	rotMatriX[9] = -1 * sin(thetaX * gin);
	rotMatriX[10] = cos(thetaX * gin);

	rotMatriY[0] = cos(thetaY * gin);
	rotMatriY[2] = -1 * sin(thetaY * gin);
	rotMatriY[8] = sin(thetaY * gin);
	rotMatriY[10] = cos(thetaY * gin);

	rotMatriZ[0] = cos(thetaZ * gin);
	rotMatriZ[1] = sin(thetaZ * gin);
	rotMatriZ[4] = -1 * sin(thetaZ * gin);
	rotMatriZ[5] = cos(thetaZ * gin);

	/*自定軸旋轉*/
	newsize = newangle * gin;
	newtemp = sqrt((newx - newx1) * (newx - newx1) + (newy - newy1) * (newy - newy1));
	if (newtemp != 0)
	{
		rx = (newx - newx1) / newtemp;
		ry = (newy - newy1) / newtemp;
		rz = newz / newtemp;
	}
	rotMatriN[0] = (1 - cos(newsize)) * rx * rx + cos(newsize);
	rotMatriN[1] = (1 - cos(newsize)) * rx * ry + sin(newsize) * rz;
	rotMatriN[2] = (1 - cos(newsize)) * rx * rz - sin(newsize) * ry;
	rotMatriN[4] = (1 - cos(newsize)) * rx * ry - sin(newsize) * rz;
	rotMatriN[5] = (1 - cos(newsize)) * ry * ry + cos(newsize);
	rotMatriN[6] = (1 - cos(newsize)) * ry * rz + sin(newsize) * rx;
	rotMatriN[8] = (1 - cos(newsize)) * rx * rz + sin(newsize) * ry;
	rotMatriN[9] = (1 - cos(newsize)) * ry * rz - sin(newsize) * rx;
	rotMatriN[10] = (1 - cos(newsize)) * rz * rz + cos(newsize);

	glMultMatrixf(rotMatriX);
	glMultMatrixf(rotMatriY);
	glMultMatrixf(rotMatriZ);
	glMultMatrixf(rotMatriN);
	glMultMatrixf(translateMatrix);

	SetupResize();

	glScalef(Scale, Scale, Scale);
	glColor3f(1, 0.9588, 0.6392);

	if (PolygonSet == 2) 
	{
		glBegin(GL_TRIANGLES);
		vector<vector<float>> ::iterator face_iter = Vertex_Index.begin();
		vector<float>  ::iterator face_iter2 = face_iter->begin();
		for (int i = 0; face_iter != Vertex_Index.end(); face_iter++, i++) 
		{
			icoNormVec(i);
			face_iter2 = face_iter->begin();
			if (ColorMode) glColor3fv(&Color[*(face_iter2)-1][0]);
			glVertex3fv(&Vertex[*(face_iter2++) - 1][0]);
			if (ColorMode) glColor3fv(&Color[*(face_iter2)-1][0]);
			glVertex3fv(&Vertex[*(face_iter2++) - 1][0]);
			if (ColorMode) glColor3fv(&Color[*(face_iter2)-1][0]);
			glVertex3fv(&Vertex[*(face_iter2++) - 1][0]);
		}
		glEnd();
	}
	else if (PolygonSet == 1) 
	{
		glBegin(GL_LINES);
		vector<vector<float>> ::iterator line_iter;
		vector<float>  ::iterator line_iter2;
		for (line_iter = Vertex_Index.begin(); line_iter != Vertex_Index.end(); line_iter++)
		{
			line_iter2 = line_iter->begin();
			if (ColorMode) glColor3fv(&Color[*(line_iter2)-1][0]);
			glVertex3fv(&Vertex[*line_iter2 - 1][0]);
			line_iter2++;
			if (ColorMode) glColor3fv(&Color[*(line_iter2)-1][0]);
			glVertex3fv(&Vertex[*line_iter2 - 1][0]);
			glVertex3fv(&Vertex[*line_iter2 - 1][0]);
			line_iter2++;
			if (ColorMode) glColor3fv(&Color[*(line_iter2)-1][0]);
			glVertex3fv(&Vertex[*line_iter2 - 1][0]);
			glVertex3fv(&Vertex[*line_iter2 - 1][0]);
			line_iter2 = line_iter->begin();
			if (ColorMode) glColor3fv(&Color[*(line_iter2)-1][0]);
			glVertex3fv(&Vertex[*line_iter2 - 1][0]);
		}
		glEnd();
	}
	else 
	{
		glPointSize(6.7);
		glBegin(GL_POINTS);
		vector<vector<float>> ::iterator point_iter;
		vector<float>  ::iterator point_iter2;
		for (point_iter = Vertex_Index.begin(); point_iter != Vertex_Index.end(); point_iter++)
		{
			point_iter2 = point_iter->begin();
			if (ColorMode) glColor3fv(&Color[*(point_iter2)-1][0]);
			glVertex3fv(&Vertex[*point_iter2 - 1][0]);
			point_iter2++;
			if (ColorMode) glColor3fv(&Color[*(point_iter2)-1][0]);
			glVertex3fv(&Vertex[*point_iter2 - 1][0]);
			point_iter2++;
			if (ColorMode) glColor3fv(&Color[*(point_iter2)-1][0]);
			glVertex3fv(&Vertex[*point_iter2 - 1][0]);
		}
		glEnd();
	}

	glPopMatrix();
	glutSwapBuffers();
	return;
}

void ChangeSize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-50, 50, -30, 30, -30, 30);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	return;
}

void SetupRC()
{
	GLfloat whiteLight[] = { 0.45f,0.45f,0.45f,1.0f };
	GLfloat sourceLight[] = { 0.25f,0.25f,.25f,1.0f };
	GLfloat lightPos[] = { 0.f,25.0f,20.0f,0.0f };
	GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat mat_diffuse[] = { 0.8, 0.6, 0.4, 1.0 };
	GLfloat mat_ambient[] = { 0.8, 0.6, 0.4, 1.0 };
	GLfloat mat_shininess = { 20.0 };

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	Resize = Scale = 1;
	tx = ty = thetaX = thetaY = thetaZ = BoundBox_X = BoundBox_Xb = BoundBox_Y = BoundBox_Yb = BoundBox_Z = BoundBox_Zb = 0;
	OBJ_Path = "octahedron.obj";
	Vertex.clear();
	Vertex_Index.clear();
	LoadOBJ();
	return;
}

int main(int argc, char** argv)
{
	thetaX = thetaY = thetaZ = 0;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1200, 760);
	glutInitWindowPosition(600, 80);
	glutCreateWindow("OBJ Parser - Midterm Project 2023");
	SetupRC();
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	glutKeyboardFunc(myKeyboard);
	glutMouseFunc(myMouse);
	glutSpecialFunc(mySpecialKey);
	createMenu();
	glutMainLoop();
	return 0;
}