#include "D://CG/OpenglLib/freeglut/include/GL/freeglut.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <vector>
#include <algorithm>

#define Pi 3.14159

using namespace std;

string LocationPath = "D:\\CG\\Lab\\final\\";
string FileName;
int Resize = 0, tx = 0, ty = 0;
int ColorMode = 3, AnimationSet = 1;
double Scale = 1;
double BoundBox_X = 0, BoundBox_Y = 0, BoundBox_Z = 0;
double BoundBox_Xb = 0, BoundBox_Yb = 0, BoundBox_Zb = 0;
GLfloat d1[3], d2[3], n[3];
GLfloat vLightPos[] = { -80.0f, 120.0f, 100.0f, 0.0f };
GLuint textures[5];
int Angel = 15;

struct BoundingBox
{
	double BoundBox_X = 0, BoundBox_Y = 0, BoundBox_Z = 0;
	double BoundBox_Xb = 0, BoundBox_Yb = 0, BoundBox_Zb = 0;
};

struct OBJfile
{
	vector<vector<float>> Vertex;
	vector<vector<float>> VertexNormal;
	vector<vector<float>> VertexTexture;
	vector<vector<float>> Face;
	BoundingBox objBoundBox;
	void SetupResize()
	{
		if (Resize == 1)
		{
			double resize_mode = max(max(abs(objBoundBox.BoundBox_X), abs(objBoundBox.BoundBox_Xb)), max(max(abs(objBoundBox.BoundBox_Y), abs(objBoundBox.BoundBox_Yb)), max(abs(objBoundBox.BoundBox_Z), abs(objBoundBox.BoundBox_Zb))));
			double temp = abs(objBoundBox.BoundBox_X) + abs(objBoundBox.BoundBox_Xb);

			if (resize_mode == abs(objBoundBox.BoundBox_Xb) || resize_mode == abs(objBoundBox.BoundBox_X))
			{
				if (abs(objBoundBox.BoundBox_X) + abs(objBoundBox.BoundBox_Xb) > 50)
				{
					while (temp - (abs(objBoundBox.BoundBox_X) + abs(objBoundBox.BoundBox_Xb)) / 10 > 50)
					{
						Scale -= 0.1;
						temp -= (abs(objBoundBox.BoundBox_X) + abs(objBoundBox.BoundBox_Xb)) / 10;
					}
				}
				else if (abs(objBoundBox.BoundBox_X) + abs(objBoundBox.BoundBox_Xb) < 40)
				{
					while (temp + (abs(objBoundBox.BoundBox_X) + abs(objBoundBox.BoundBox_Xb)) / 10 < 40)
					{
						Scale += 0.1;
						temp += (abs(objBoundBox.BoundBox_X) + abs(objBoundBox.BoundBox_Xb)) / 10;
					}
				}
			}
			else if (resize_mode == abs(objBoundBox.BoundBox_Yb) || resize_mode == abs(objBoundBox.BoundBox_Y))
			{
				temp = abs(objBoundBox.BoundBox_Y) + abs(objBoundBox.BoundBox_Yb);

				if (abs(objBoundBox.BoundBox_Y) + abs(objBoundBox.BoundBox_Yb) > 40)
				{
					while (temp - (abs(objBoundBox.BoundBox_Y) + abs(objBoundBox.BoundBox_Yb)) / 10 > 404)
					{
						Scale -= 0.1;
						temp -= (abs(objBoundBox.BoundBox_Y) + abs(objBoundBox.BoundBox_Yb)) / 10;
					}
				}
				else if (abs(objBoundBox.BoundBox_Y) + abs(objBoundBox.BoundBox_Yb) < 30)
				{
					while (temp + (abs(objBoundBox.BoundBox_Y) + abs(objBoundBox.BoundBox_Yb)) / 10 < 30)
					{
						Scale += 0.1;
						temp += (abs(objBoundBox.BoundBox_Y) + abs(objBoundBox.BoundBox_Yb)) / 10;
					}
				}
			}
			else if (resize_mode == abs(objBoundBox.BoundBox_Zb) || resize_mode == abs(objBoundBox.BoundBox_Z))
			{
				temp = abs(objBoundBox.BoundBox_Z) + abs(objBoundBox.BoundBox_Zb);

				if (abs(objBoundBox.BoundBox_Z) + abs(objBoundBox.BoundBox_Zb) > 40)
				{
					while (temp - (abs(objBoundBox.BoundBox_Z) + abs(objBoundBox.BoundBox_Zb)) / 10 > 40)
					{
						Scale -= 0.1;
						temp -= (abs(objBoundBox.BoundBox_Z) + abs(objBoundBox.BoundBox_Zb)) / 10;
					}
				}
				else if (abs(objBoundBox.BoundBox_Z) + abs(objBoundBox.BoundBox_Zb) < 30)
				{
					while (temp + (abs(objBoundBox.BoundBox_Z) + abs(objBoundBox.BoundBox_Zb)) / 10 < 30)
					{
						Scale += 0.1;
						temp += (abs(objBoundBox.BoundBox_Z) + abs(objBoundBox.BoundBox_Zb)) / 10;
					}
				}
			}
			Resize = 0;
		}

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

	void icoNormVec(int i)
	{
		vector<float>  ::iterator iter = Face[i].begin();

		for (int k = 0; k < 3; k++)
		{
			d1[k] = Vertex[*iter - 1][k] - Vertex[*(iter + 1) - 1][k];
			d2[k] = Vertex[*(iter + 1) - 1][k] - Vertex[*(iter + 2) - 1][k];
		}

		normCrossProd(d1, d2, n);
		glNormal3fv(n);

		return;
	}

};

OBJfile obj1, obj2, obj3, obj4;

void ChangeSize(int, int);
void RenderScene(void);
void SetupRC(void);
OBJfile LoadOBJ(string);
OBJfile LoadOBJ2(string);
BoundingBox GetBoundingBox(double x, double y, double z, OBJfile o);
void Timer(int);

OBJfile LoadOBJ2(string filename)
{
	char temp_string[256];
	OBJfile obj;
	FILE *file = fopen((filename).c_str(), "r");
	cout << (filename).c_str() << endl;

	if (file == NULL) 
	{
		printf("Impossible to open the file !");
		return obj;
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
			float X_Y_Z[3];

			fscanf(file, "%f %f %f", &X_Y_Z[0], &X_Y_Z[1], &X_Y_Z[2]);
			obj.objBoundBox = GetBoundingBox(X_Y_Z[0], X_Y_Z[1], X_Y_Z[2], obj);

			for (int i = 0; i < 3; i++) 
			{
				temp_vertices.push_back(200.0*X_Y_Z[i]);
			} 
			obj.Vertex.push_back(temp_vertices);
		}
		else if (strcmp(temp_string, "vt") == 0)
		{
			vector<float> temp_vertices;
			float dontcare;
			float X_Y_Z[3];

			fscanf(file, "%f %f", &X_Y_Z[0], &X_Y_Z[1]);

			for (int i = 0; i < 2; i++)
			{
				temp_vertices.push_back(X_Y_Z[i]);
			}
			obj.VertexTexture.push_back(temp_vertices);
		}
		else if (strcmp(temp_string, "f") == 0)
		{
			float X_Y_Z[4];
			float vX_vY_vZ[4];
			int dontcare[8];
			vector<float> temp_vertices, temp_verticesT;

			fscanf(file, "%f/%f/%f %f/%f/%f %f/%f/%f %f/%f/%f", &X_Y_Z[0], &vX_vY_vZ[0], &dontcare[1], &X_Y_Z[1], &vX_vY_vZ[1], &dontcare[3],
																&X_Y_Z[2], &vX_vY_vZ[2], &dontcare[5], &X_Y_Z[3], &vX_vY_vZ[3], &dontcare[7]);

			for (int i = 0; i < 4; i++) 
			{
				temp_vertices.push_back(X_Y_Z[i]);
			} 
			obj.Face.push_back(temp_vertices);
		}
	}

	fclose(file);

	return obj;
}

OBJfile LoadOBJ(string filename)
{
	char temp_string[256];
	OBJfile obj;
	FILE* file = fopen((filename).c_str(), "r");
	cout << (filename).c_str() << endl;

	if (file == NULL)
	{
		printf("Impossible to open the file !");
		return obj;
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
			float X_Y_Z[3];

			fscanf(file, "%f %f %f", &X_Y_Z[0], &X_Y_Z[1], &X_Y_Z[2]);
			obj.objBoundBox = GetBoundingBox(X_Y_Z[0], X_Y_Z[1], X_Y_Z[2], obj);

			for (int i = 0; i < 3; i++)
			{
				temp_vertices.push_back(X_Y_Z[i]);
			}
			obj.Vertex.push_back(temp_vertices);
		}
		else if (strcmp(temp_string, "vt") == 0)
		{
			vector<float> temp_vertices;
			float dontcare;
			float X_Y_Z[3];

			fscanf(file, "%f %f %f", &X_Y_Z[0], &X_Y_Z[1], &X_Y_Z[2]);

			for (int i = 0; i < 3; i++)
			{
				temp_vertices.push_back(X_Y_Z[i]);
			}
			obj.VertexTexture.push_back(temp_vertices);
		}
		else if (strcmp(temp_string, "f") == 0)
		{
			float X_Y_Z[4];
			float vX_vY_vZ[4];
			int dontcare[8];
			vector<float> temp_vertices, temp_verticesT;

			fscanf(file, "%f/%f/%f %f/%f/%f %f/%f/%f %f/%f/%f", &X_Y_Z[0], &vX_vY_vZ[0], &dontcare[1], &X_Y_Z[1], &vX_vY_vZ[1], &dontcare[3],
				&X_Y_Z[2], &vX_vY_vZ[2], &dontcare[5], &X_Y_Z[3], &vX_vY_vZ[3], &dontcare[7]);

			for (int i = 0; i < 4; i++)
			{
				temp_vertices.push_back(X_Y_Z[i]);
			}
			obj.Face.push_back(temp_vertices);
		}
	}

	fclose(file);

	return obj;
}

BoundingBox GetBoundingBox(double x, double y, double z, OBJfile o)
{
	if (o.objBoundBox.BoundBox_X < x)
	{
		o.objBoundBox.BoundBox_X = x;
	}
	else if (o.objBoundBox.BoundBox_Xb > x)
	{ 
		o.objBoundBox.BoundBox_Xb = x;
	}

	if (o.objBoundBox.BoundBox_Y < y)
	{ 
		o.objBoundBox.BoundBox_Y = y;
	}
	else if (o.objBoundBox.BoundBox_Yb > y)
	{
		o.objBoundBox.BoundBox_Yb = y;
	}

	if (o.objBoundBox.BoundBox_Z < z)
	{
		o.objBoundBox.BoundBox_Z = z;
	}
	else if (o.objBoundBox.BoundBox_Zb > z)
	{
		o.objBoundBox.BoundBox_Zb = z;
	}

	return o.objBoundBox;
}

void Timer(int i) 
{
	if (AnimationSet)
	{
		glutPostRedisplay();
	}

	glutTimerFunc(30, Timer, 1);

	return;
}

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_MODELVIEW);
	glLightfv(GL_LIGHT0, GL_POSITION, vLightPos);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPushMatrix();
	//
	glDisable(GL_LIGHTING);
	glColor3ub(255, 255, 255);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-500.0f, 500.0f, -999.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-500.0f, -500.0f, -999.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(500.0f, -500.0f, -999.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(500.0f, 500.0f, -999.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	//
	ColorMode *= -1;
	glPushMatrix();
	glTranslatef(-350+ ColorMode, -560, -180);
	glRotatef(150, 0, 1, 0);
	glColor3f(0, 0.6, 0);
	glBegin(GL_QUADS);
	obj3.SetupResize();
	for (vector<vector<float>> ::iterator line_iter = obj3.Face.begin(); line_iter != obj3.Face.end(); line_iter++)
	{
		vector<float>  ::iterator line_iter2;
		line_iter2 = line_iter->begin();
		glTexCoord2fv(&(obj3.VertexTexture)[*line_iter2 - 1][0]);
		glVertex3fv(&(obj3.Vertex)[*line_iter2 - 1][0]);
		line_iter2++;
		glTexCoord2fv(&(obj3.VertexTexture)[*line_iter2 - 1][0]);
		glVertex3fv(&(obj3.Vertex)[*line_iter2 - 1][0]);
		line_iter2++;
		glTexCoord2fv(&(obj3.VertexTexture)[*line_iter2 - 1][0]);
		glVertex3fv(&(obj3.Vertex)[*line_iter2 - 1][0]);
		line_iter2++;
		glTexCoord2fv(&(obj3.VertexTexture)[*line_iter2 - 1][0]);
		glVertex3fv(&(obj3.Vertex)[*line_iter2 - 1][0]);
		line_iter2 = line_iter->begin();

		glVertex3fv(&(obj3.Vertex)[*line_iter2 - 1][0]);
		glTexCoord2fv(&(obj3.VertexTexture)[*line_iter2 - 1][0]);
	}
	glEnd();
	glPopMatrix();
	//
	glPushMatrix();
	glTranslatef(-200- ColorMode, -500, -250);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[3]);
	glBegin(GL_QUADS);
	obj3.SetupResize();
	for (vector<vector<float>> ::iterator line_iter = obj3.Face.begin(); line_iter != obj3.Face.end(); line_iter++)
	{
		vector<float>  ::iterator line_iter2;
		line_iter2 = line_iter->begin();
		glTexCoord2fv(&(obj3.VertexTexture)[*line_iter2 - 1][0]);
		glVertex3fv(&(obj3.Vertex)[*line_iter2 - 1][0]);
		line_iter2++;
		glTexCoord2fv(&(obj3.VertexTexture)[*line_iter2 - 1][0]);
		glVertex3fv(&(obj3.Vertex)[*line_iter2 - 1][0]);
		line_iter2++;
		glTexCoord2fv(&(obj3.VertexTexture)[*line_iter2 - 1][0]);
		glVertex3fv(&(obj3.Vertex)[*line_iter2 - 1][0]);
		line_iter2++;
		glTexCoord2fv(&(obj3.VertexTexture)[*line_iter2 - 1][0]);
		glVertex3fv(&(obj3.Vertex)[*line_iter2 - 1][0]);
		line_iter2 = line_iter->begin();

		glVertex3fv(&(obj3.Vertex)[*line_iter2 - 1][0]);
		glTexCoord2fv(&(obj3.VertexTexture)[*line_iter2 - 1][0]);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	//
	/*Fish 1*/
	glPushMatrix();
	glTranslatef(0, 0, 0);
	Angel += 5;
	glRotatef(-1*Angel, 0, 1, 0);
	glColor3f(1, 0.9588, 0.6392);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glBegin(GL_QUADS);
	obj1.SetupResize();
	for (vector<vector<float>> ::iterator line_iter = obj1.Face.begin(); line_iter != obj1.Face.end(); line_iter++)
	{
		vector<float>  ::iterator line_iter2;
		line_iter2 = line_iter->begin();
		glTexCoord3fv(&(obj1.VertexTexture)[*line_iter2 - 1][0]);
		glVertex3fv(&(obj1.Vertex)[*line_iter2 - 1][0]);
		line_iter2++;
		glTexCoord3fv(&(obj1.VertexTexture)[*line_iter2 - 1][0]);
		glVertex3fv(&(obj1.Vertex)[*line_iter2 - 1][0]);
		line_iter2++;
		glTexCoord3fv(&(obj1.VertexTexture)[*line_iter2 - 1][0]);
		glVertex3fv(&(obj1.Vertex)[*line_iter2 - 1][0]);
		line_iter2++;
		glTexCoord3fv(&(obj1.VertexTexture)[*line_iter2 - 1][0]);
		glVertex3fv(&(obj1.Vertex)[*line_iter2 - 1][0]);
		line_iter2 = line_iter->begin();
		
		glVertex3fv(&(obj1.Vertex)[*line_iter2 - 1][0]);
		glTexCoord3fv(&(obj1.VertexTexture)[*line_iter2 - 1][0]);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	//
	/*Fish 2*/
	glPushMatrix();
	obj2.SetupResize();
	float x = 1.0f * cos(Angel * 0.5f * Pi / 12.0f) - 500;
	float y = 2.0f * sin(Angel * 0.5f * Pi / 12.0f) - 150;
	glRotatef(Angel * 0.5f, 0, 1, 0);
	glTranslatef(x, y, 300.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[2]);
	glBegin(GL_QUADS);
	for (vector<vector<float>> ::iterator line_iter = obj2.Face.begin(); line_iter != obj2.Face.end(); line_iter++)
	{
		vector<float>  ::iterator line_iter2;
		line_iter2 = line_iter->begin();
		glTexCoord3fv(&(obj2.VertexTexture)[*line_iter2 - 1][0]);
		glVertex3fv(&(obj2.Vertex)[*line_iter2 - 1][0]);
		line_iter2++;
		glTexCoord3fv(&(obj2.VertexTexture)[*line_iter2 - 1][0]);
		glVertex3fv(&(obj2.Vertex)[*line_iter2 - 1][0]);
		line_iter2++;
		glTexCoord3fv(&(obj2.VertexTexture)[*line_iter2 - 1][0]);
		glVertex3fv(&(obj2.Vertex)[*line_iter2 - 1][0]);
		line_iter2++;
		glTexCoord3fv(&(obj2.VertexTexture)[*line_iter2 - 1][0]);
		glVertex3fv(&(obj2.Vertex)[*line_iter2 - 1][0]);
		line_iter2 = line_iter->begin();

		glVertex3fv(&(obj2.Vertex)[*line_iter2 - 1][0]);
		glTexCoord3fv(&(obj2.VertexTexture)[*line_iter2 - 1][0]);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	//
	/*Fish 3*/
	glPushMatrix();
	obj2.SetupResize();
	x = 1.0f * cos(-1 * Angel * 0.03f * Pi / 12.0f) - 800;
	y = 2.0f * sin(-1 * Angel * 0.03f * Pi / 12.0f) + 350;
	glRotatef(-1 * Angel * 0.03f, 0, 1, 0);
	glTranslatef(x, y, 100.0f);
	glRotatef(180, 0, 1, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[4]);
	glBegin(GL_QUADS);
	obj4.SetupResize();
	for (vector<vector<float>> ::iterator line_iter = obj4.Face.begin(); line_iter != obj4.Face.end(); line_iter++)
	{
		vector<float>  ::iterator line_iter2;
		line_iter2 = line_iter->begin();
		glTexCoord3fv(&(obj4.VertexTexture)[*line_iter2 - 1][0]);
		glVertex3fv(&(obj4.Vertex)[*line_iter2 - 1][0]);
		line_iter2++;
		glTexCoord3fv(&(obj4.VertexTexture)[*line_iter2 - 1][0]);
		glVertex3fv(&(obj4.Vertex)[*line_iter2 - 1][0]);
		line_iter2++;
		glTexCoord3fv(&(obj4.VertexTexture)[*line_iter2 - 1][0]);
		glVertex3fv(&(obj4.Vertex)[*line_iter2 - 1][0]);
		line_iter2++;
		glTexCoord3fv(&(obj4.VertexTexture)[*line_iter2 - 1][0]);
		glVertex3fv(&(obj4.Vertex)[*line_iter2 - 1][0]);
		line_iter2 = line_iter->begin();

		glVertex3fv(&(obj4.Vertex)[*line_iter2 - 1][0]);
		glTexCoord3fv(&(obj4.VertexTexture)[*line_iter2 - 1][0]);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	//
	glutSwapBuffers();
	return;
}

void ChangeSize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-500.0f, 500.0f, -500.0f, 500.0f, -1000.0f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLightfv(GL_LIGHT0, GL_POSITION, vLightPos);

	return;
}

void SetupRC()
{
	GLbyte* pBytes;
	GLint nWidth, nHeight, nComponents;
	GLenum format;

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

	glShadeModel(GL_SMOOTH); /*enable smooth shading */
	glEnable(GL_LIGHTING); /* enable lighting */
	glEnable(GL_LIGHT0); /* enable light 0 */
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	//
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // 設定openGL材質紋理的參數和材質的組合模式
	glGenTextures(4, textures); // 註冊一個大小為4的陣列讓openGL儲存材質，名稱為textures

	FileName = "background";
	cv::Mat Image = cv::imread(LocationPath + FileName + ".jpg"); // 利用openCV讀取圖片檔案 
	if (Image.empty()) {
		std::cout << "Background empty\n";
	}
	else {
		// 將讀取進來的圖片檔案當作材質存進textures中
		cv::flip(Image, Image, 0);
		glGenTextures(1, &textures[0]);
		glBindTexture(GL_TEXTURE_2D, textures[0]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Image.cols, Image.rows, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, Image.ptr());
	}

	FileName = "PurpleFish";
	Image = cv::imread(LocationPath+FileName+".jpg"); // 利用openCV讀取圖片檔案 
	if (Image.empty()) {
		std::cout << "Purple Fish empty\n";
	}
	else {
		// 將讀取進來的圖片檔案當作材質存進textures中
		cv::flip(Image, Image, 0);
		glGenTextures(1, &textures[1]);
		glBindTexture(GL_TEXTURE_2D, textures[1]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Image.cols, Image.rows, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, Image.ptr());
	}
	//
	obj1 = LoadOBJ(LocationPath + FileName + ".obj");
	FileName = "RedFish";
	Image = cv::imread(LocationPath + FileName + ".jpg"); // 利用openCV讀取圖片檔案 
	if (Image.empty()) {
		std::cout << "Red Fish empty\n";
	}
	else {
		// 將讀取進來的圖片檔案當作材質存進textures中
		cv::flip(Image, Image, 0);
		glGenTextures(1, &textures[2]);
		glBindTexture(GL_TEXTURE_2D, textures[2]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Image.cols, Image.rows, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, Image.ptr());
	}
	obj2 = LoadOBJ(LocationPath + FileName + ".obj");

	FileName = "seaweed";
	Image = cv::imread(LocationPath + FileName + ".jpg"); // 利用openCV讀取圖片檔案 
	if (Image.empty()) {
		std::cout << "Seaweed empty\n";
	}
	else {
		// 將讀取進來的圖片檔案當作材質存進textures中
		cv::flip(Image, Image, 0);
		glGenTextures(1, &textures[3]);
		glBindTexture(GL_TEXTURE_2D, textures[3]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Image.cols, Image.rows, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, Image.ptr());
	}
	obj3 = LoadOBJ2(LocationPath + FileName + ".obj");

	FileName = "BlueFish";
	Image = cv::imread(LocationPath + FileName + ".jpg"); // 利用openCV讀取圖片檔案 
	if (Image.empty()) {
		std::cout << "Blue Fish empty\n";
	}
	else {
		// 將讀取進來的圖片檔案當作材質存進textures中
		cv::flip(Image, Image, 0);
		glGenTextures(1, &textures[4]);
		glBindTexture(GL_TEXTURE_2D, textures[4]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Image.cols, Image.rows, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, Image.ptr());
	}
	obj4 = LoadOBJ(LocationPath + FileName + ".obj");

	return;
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Final Project: Ocean");
	SetupRC();
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	glutTimerFunc(30, Timer, 1);
	glutMainLoop();

	return 0;
}