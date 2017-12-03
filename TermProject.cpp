//George Wysor
//CS 459
//Project 3
//The code for this project was written using code taken from Lab08, proj3_rotation_example.cpp, and proj3_menu_example.cpp
//and was assembled, modified, and tested by George Wysor.
//

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h> 
#include <string.h>
#include <cstdlib>//for random numbers


#define min(x,y) ((x)<(y) ? (x) : (y))
#define max(x,y) ((x)>(y) ? (x) : (y))
#define PI 3.14159265 

#pragma warning(disable : 4996)//disables VS 2017 warnings regarding scanf, fscanf, fgets etc. 

//*******Variable declaration from Lab08, initializes arrays to store information from OFF file.************
static float alpha = 0.0;
static float beta = PI / 6.0;
static GLdouble cpos[3];
static GLint angleView = 30.0;
static GLint flag = 0;//now used for tracking the current transformation mode

static GLfloat lpos[] = { -1.0, 2.0, 1.0, 1.0 };
static GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };
static GLfloat red[] = { 1.0, 0.0, 0.0, 1.0 };
static GLfloat blue[] = { 0.0, 0.0, 1.0, 1.0 };

static GLint numVertices, numPolygons, numEdges;
static GLfloat **vdata;   //array for vertex data
static GLuint **pdata;    //array for polygon data (vertex indices)
static GLuint *psize;     //array for polygon size
static GLfloat **normals; //array for polygon normals
//****************************************************************************************************

//*********Variables from proj3_rotation_example.cpp - stores information for left mouse click transformations
bool mouseDown = false;

float xrot = 0.0f;
float yrot = 0.0f;

float xdiff = 0.0f;
float ydiff = 0.0f;
//***********************************************************************************************

float scalefactor = 1.0f; //variables for tranlsation and scaling

float scalediff = 0.0f;

float xtrans = 0.0f;
float ytrans = 0.0f;

float xdifft = 0.0f;
float ydifft = 0.0f;

//********************Variables from proj3_menu_example.cpp - menu variables
static int menuExit;
static int draw_menu;
//*********************************************************************************

void writemessage() //Prints instructions to console - From Lab08
{
	printf("\n\
		   CS 459 - Fall 2017 - Project 3 - George Wysor\n\n\
		   The code for this project was written using code taken from \n\
		   Lab08, proj3_rotation_example.cpp, and proj3_menu_example.cpp\n\
		   and was assembled, modified, and tested by George Wysor.\n\n\
		   Reads an OFF file and renders the object\n\
		   c ------------------------- change off files\n\
		   x, X, y, Y, z, Z ---------- change light source position\n\
		   t-------------------------- toggle to translate mode\n\
		   r-------------------------- toggle to rotation mode\n\
		   s-------------------------- toggle to scaling mode\n\
		   hold and drag left click -- perform current transformation\n\
		   right click --------------- open drawing mode menu\n\
		   ESC ----------------------- exit\n\
		   \n");
}

//***********************Menu functions - From proj3_menu_example.cpp
void menu(int value){
	if(value == 0){
		exit(0);
	}
	else{
		switch(value){
			case 1:
				glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
				break;
			case 2:
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				break;
			case 3:
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				break;
			case 4:
				glPolygonMode(GL_FRONT, GL_LINE);
				glPolygonMode(GL_BACK, GL_FILL);
				break;
		}
	}
	
	// you would want to redraw now
	glutPostRedisplay();
}

void createMenu(void){ 
	//////////
	// MENU //
	//////////
	
	draw_menu = glutCreateMenu(menu);
	
	// Add sub menu entry

	glutAddMenuEntry("Point Mode", 1);
	glutAddMenuEntry("Line Mode", 2);
	glutAddMenuEntry("Fill Mode", 3);
	glutAddMenuEntry("Line and Fill Mode", 4);
					 
	menuExit = glutCreateMenu(menu);
	
	// Create an entry
	glutAddSubMenu("Draw Mode", draw_menu);
	glutAddMenuEntry("Exit", 0);
	
	// Let the menu respond on the right mouse button
	glutAttachMenu(GLUT_RIGHT_BUTTON);	
}
//************************************************************************

void readOFF()//method to read .off format data. Borrowed partially from Dr. Zeyun Yu homework
			  //modified further to read polygons of any size
			  //nonconvex and nonflat polygons may not be rendered correctly - From Lab08
{
	int n, j;
	int a, b;
	float x, y, z;
	float resize;
	char line[256];

	FILE* fin;
	char filename[FILENAME_MAX];

	fin = NULL;
	while (fin == NULL) {
		printf("\n\nEnter an .off file name including extension (or press Enter to abort): ");
		int result = scanf("%99[^\n]%*c", filename);
		if (result != 1) exit(0);
		fin = fopen(filename, "rb");
	};

	/* OFF format */
	while (fgets(line, 256, fin) != NULL) {
		if (line[0] == 'O' && line[1] == 'F' && line[2] == 'F')
			break;
	}
	fscanf(fin, "%d %d %d\n", &numVertices, &numPolygons, &numEdges);

	printf("Number of vertices  = %d\n", numVertices);
	printf("Number of polygons = %d\n", numPolygons);
	printf("Number of edges = %d\n", numEdges);
	printf("loading vedrtices and polygons... ");

	vdata = new GLfloat*[numVertices];
	for (int i = 0; i<numVertices; i++)
		vdata[i] = new GLfloat[3];

	pdata = new GLuint*[numPolygons]; //array for storing polygon data (vertex indices)
	psize = new GLuint[numPolygons];  //array for storing polygon size

	resize = 0.0001;
	for (n = 0; n < numVertices; n++) { //read vertex data
		fscanf(fin, "%f %f %f\n", &x, &y, &z);
		vdata[n][0] = x;
		resize = max(resize, fabs(x));
		vdata[n][1] = y;
		resize = max(resize, fabs(y));
		vdata[n][2] = z;
		resize = max(resize, fabs(z));
	}

	for (n = 0; n < numVertices; n++) { //adjust vertex data
		vdata[n][0] = vdata[n][0] / resize;
		vdata[n][1] = vdata[n][1] / resize;
		vdata[n][2] = vdata[n][2] / resize;
	}

	for (n = 0; n < numPolygons; n++) {
		fscanf(fin, "%d", &a);
		psize[n] = a;  //store n-th polygon size in psize[n]
		pdata[n] = new GLuint[a];
		for (j = 0; j < a; j++) { //read and save vertices of n-th polygon
			fscanf(fin, "%d", &b);
			pdata[n][j] = b;
		}
	}
	fclose(fin);
	printf("done.\n");

}

void calculateNormal()//calculates the normal vector for every polygon
					  //using the first three vertices, assuming they occur in ccw order - From Lab08
{
	normals = new GLfloat*[numPolygons];
	for (int i = 0; i<numPolygons; i++)
		normals[i] = new GLfloat[3];

	for (int i = 0; i<numPolygons; i++) {

		GLint t1 = pdata[i][0], t2 = pdata[i][1], t3 = pdata[i][2];
		GLfloat v1[3] = { vdata[t1][0],vdata[t1][1],vdata[t1][2] };
		GLfloat v2[3] = { vdata[t2][0],vdata[t2][1],vdata[t2][2] };
		GLfloat v3[3] = { vdata[t3][0],vdata[t3][1],vdata[t3][2] };

		GLfloat n1[3] = { v2[0] - v1[0],v2[1] - v1[1],v2[2] - v1[2] };
		GLfloat n2[3] = { v3[0] - v1[0],v3[1] - v1[1],v3[2] - v1[2] };

		float	normx = (n1[1] * n2[2]) - (n2[1] * n1[2]),
			normy = (n1[2] * n2[0]) - (n2[2] * n1[0]),
			normz = (n1[0] * n2[1]) - (n2[0] * n1[1]);

		float factor = sqrt(pow(normx, 2) + pow(normy, 2) + pow(normz, 2));
		normx /= factor;
		normy /= factor;
		normz /= factor;
		normals[i][0] = normx;
		normals[i][1] = normy;
		normals[i][2] = normz;

	}
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(angleView, (GLfloat)w / (GLfloat)h, 1.5, 20.0);
	glMatrixMode(GL_MODELVIEW);
}

void display(void) //display function, draws polygon from arrays filled from OFF file. - From Lab08
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cpos[0] = 5.0 * cos(beta) * sin(alpha);
	cpos[1] = 5.0 * sin(beta);
	cpos[2] = 5.0 * cos(beta) * cos(alpha);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cpos[0], cpos[1], cpos[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glLightfv(GL_LIGHT0, GL_POSITION, lpos);
	glPushMatrix();
	glTranslatef(lpos[0], lpos[1], lpos[2]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, white);
	glMaterialfv(GL_FRONT, GL_EMISSION, white);
	glutSolidSphere(0.05, 10, 8);
	glPopMatrix();
	
	glTranslatef(xtrans/100, ytrans/100, 0.0f);
	glRotatef(xrot, 1.0f, 0.0f, 0.0f);
	glRotatef(yrot, 0.0f, 1.0f, 0.0f);
	glScalef(scalefactor, scalefactor, scalefactor);

	glMaterialfv(GL_FRONT, GL_EMISSION, black);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
	glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, blue);

	for (int i = 0; i<numPolygons; i++) {
		glBegin(GL_POLYGON);
		for (int j = 0; j<psize[i]; j++) {
			glVertex3fv(vdata[pdata[i][j]]);
		}
		glNormal3fv(normals[i]);
		glEnd();
	}

	glFlush();
	glutSwapBuffers();
}



void keyboard(unsigned char key, int x, int y) //keyboard input listener - From Lab08
{
	static int polygonmode[2];

	switch (key) {
	case 27:
		exit(0);
		break;
	case 'c':   // change off file        
		printf("change off file...\n");
		readOFF();
		calculateNormal();
		glutPostRedisplay();
		break;
	case 'x':
		lpos[0] = lpos[0] + 0.2;
		glutPostRedisplay();
		break;
	case 'X':
		lpos[0] = lpos[0] - 0.2;
		glutPostRedisplay();
		break;
	case 'y':
		lpos[1] = lpos[1] + 0.2;
		glutPostRedisplay();
		break;
	case 'Y':
		lpos[1] = lpos[1] - 0.2;
		glutPostRedisplay();
		break;
	case 'z':
		lpos[2] = lpos[2] + 0.2;
		glutPostRedisplay();
		break;
	case 'Z':
		lpos[2] = lpos[2] - 0.2;
		glutPostRedisplay();
		break;
	case 't': //modify for translation on mouse drag
		printf("\nTransformation Mode - Translation\n");
		flag = 0;
		glutPostRedisplay();
		break;
	case 'r':
		//code here - rotation
		printf("\nTransformation Mode - Rotation\n");
		flag = 1;
		glutPostRedisplay();
		break;
	case 's':
		//code here - scale
		printf("\nTransformation Mode - Scaling\n");
		flag = 2;
		glutPostRedisplay();
		break;
	default:
		break;
	}
}
void specialkey(GLint key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_RIGHT:
		alpha = alpha + PI / 180;
		if (alpha > 2 * PI)
			alpha = alpha - 2 * PI;
		glutPostRedisplay();
		break;
	case GLUT_KEY_LEFT:
		alpha = alpha - PI / 180;
		if (alpha < 0)
			alpha = alpha + 2 * PI;
		glutPostRedisplay();
		break;
	case GLUT_KEY_UP:
		if (beta < 0.45*PI)
			beta = beta + PI / 180;
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:
		if (beta > -0.45*PI)
			beta = beta - PI / 180;
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

void mouse(int button, int state, int x, int y){ //mouse button press listener - From proj3_rotation_example.cpp
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		mouseDown = true;

		xdiff = x - yrot;
		ydiff = -y + xrot;
		
		xdifft = x;
		ydifft = y;
		
		scalediff = y;
	}
	else{
		mouseDown = false;
	}
}

void mouseMotion(int x, int y){ //mouse motion listener - From proj3_rotation_example.cpp
	switch(flag){
		case 0:
			if(mouseDown){
				xtrans = x - xdifft;
				ytrans = -(y - ydifft);
				glutPostRedisplay();
			}
			break;
		case 1:
			if(mouseDown){
				yrot = x - xdiff;
				xrot = y - ydiff;
				glutPostRedisplay();
			}
			break;		
		case 2:
			if(mouseDown){
				scalefactor = -(y - scalediff);
				if(scalefactor < 0){
					scalefactor = scalefactor/(-800);
				}
				else{
					scalefactor = scalefactor/10;
				}
				glutPostRedisplay();
			}
			break;
	}
}

int main(int argc, char** argv){
	writemessage();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1200, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(argv[0]);
	
	createMenu();

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialkey);

	readOFF();
	calculateNormal();
	glutMainLoop();
	return 0;
}


