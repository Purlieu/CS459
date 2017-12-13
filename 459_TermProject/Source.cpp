/*******************************************
/* Citations: 
/* dragon.off file included, downloaded from http://www.holmes3d.net/graphics/offfiles/
/*
////////////////////////////////////////////////////////
// The idea of readOFF() method is borrowed from a
// template that Dr.Zeyun Yu provided.
// Used the calculateNormal() method that was used in Lab 08
// at UW-Milwaukee

//Modified by: Reihaneh Rostami
//Used by: Michael Dorsey and Jake Wysor

//Used methods that were provided in sample .cpp files
//along with the sample_polygon.off file
//Provied by Dr. Zeyun Yu
//These methods include partial implementation of
//rotation and menu, along with keyboard functions
////////////////////////////////////////////////////////
*/
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>


static GLfloat lOnePos[] = {0.0, 1.5, 3.0, 1.0 };
static GLfloat lTwoPos[] = { 3.0, 1.0, -2.0, 1.0 };
static GLfloat lThreePos[] = { -3.0, 1.0, -2.0, 1.0 };

static GLfloat ambientColor[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat yellow[] = { 1.0, 1.0, 0.0, 1.0 };
static GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };
static GLfloat lightOneColor[] = { 1.0, 0.0, 0.0, 1.0 };
static GLfloat lightTwoColor[] = { 0.0, 1.0, 0.0, 1.0 };
static GLfloat lightThreeColor[] = { 0.0, 0.0, 1.0, 1.0 };
static GLfloat lookat[] = { 0.0, 0.0, -20.0 };
static GLfloat direction[] = { 0.0, 0.0, -1.0 };

static GLfloat ninteydegrees = 90.0;
static GLfloat sixtydegree = 60.0;
static GLfloat thirtydegrees = 30.0;
static GLfloat exponent_val = 10.0;

bool smoothOrFlat = false;

float xrot = 0.0f;
float yrot = 0.0f;

float xdiff = 0.0f;
float ydiff = 0.0f;

float scalefactor = 1.0f;

float xtranslate = 0.0f;
float ytranslate = 0.0f;

bool scaleState = false;
bool translateState = false;
bool rotateState = false;

bool mouseDown = false;

bool lightsOutOne = false;
bool lightsOutTwo = false;
bool lightsOutThree = false;

bool mLightOne = false;
bool mLightTwo = false;
bool mLightThree = false;
bool ambientLight = false;

bool smoothShading = true;
bool flatShading = false;

bool spotlightOne = false;
bool spotlightTwo = false;
bool spotlightThree = false;

bool materialShiny = false;
bool materialDiffuse = false;
bool materialSpecular = true;
bool materialEmission = false;

bool polygonOne = true;
bool polygonTwo = false;
bool polygonThree = false;

static GLint numVertices, numPolygons, numEdges;
static GLfloat **vdata;   //array for vertex data
static GLuint **pdata;    //array for polygon data (vertex indices)
static GLuint *psize;     //array for polygon size
static GLfloat **normals; //array for polygon normals
static GLfloat **cdata;//array for polygon color


#define min(x,y) ((x)<(y) ? (x) : (y))
#define max(x,y) ((x)>(y) ? (x) : (y))


void readOFF()//method to read .off format data. Borrowed partially form Dr. Zeyun Yu homework
			  //modified further to read polygons of any size
			  //nonconvex and nonflat polygons may not be rendered correctly
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
					  //using the first three vertices, assuming they occur in ccw order
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
		//---------------------------------------------------------------------

	}
}

void writemessage()
{
	printf("\n\
		   r ------------------------- Rotates the object\n\
		   t ------------------------- Translate the object\n\
		   s ------------------------- Scale the object\n\
		   v ------------------------- Change flat or smooth model\n\
		   1, 2, 3, 4 ---------------- Turn off light sources\n\
		   o ------------------------- Change from Local Light Sources to Distant Lights\n\
		   8, 9, 0, -----------------  Select lighting objects to move(GL_LIGHT0....GL_LIGHT3)\n\
		   n, m, --------------------  Draw 2 separate glut function polygons\n\
		   b ------------------------  Draw off file.  Included is a dragon.off file\n\
		   z, Z, w, W, y, Y ---------  Move correspoding light source after selecting 8, 9, 0\n\
		   f, g, h, j, k, l --------- (f, g Red +-, hj, Green +-. kl, Blue +- \n\
		   p ------------------------  reset to original position and color\n\
		   c ------------------------  closeup mode\n\
		   (Color Changing Needs Light source 8,9,0 Chosen first)\n\
		   \n\n\n\
		   Possible Steps:\n\
		   Choose Possible Light Sources to turn Off or on using...\n\
		   1 - GL_LIGHT0, 2 - GL_LIGHT1, 3 - GL_LIGHT2, 4 - GL_LIGHT3\n\
		   Choose Possible Light Source to move using...\n\
		   8 - Red Light, 9 - Green Light, 0 - Blue Light\n\
		   While light source is chosen for moving, change color using f, g, h, j, k, l\n\
		   Load Separate Polygon, or Transform Polygon\n\
		   \n");
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		mouseDown = true;
		if (rotateState) {
			xdiff = x - yrot;
			ydiff = -y + xrot;
		}
		else if (scaleState) {
			xdiff = x - xtranslate;
			ydiff = y - ytranslate;
		}
		else if (translateState) {
			xdiff = x - xtranslate;
			ydiff = y - ytranslate;
		}
	}
	else
		mouseDown = false;
}


void mouseMotion(int x, int y)
{
	if (mouseDown)
	{
		if (rotateState) {
			yrot = x - xdiff;
			xrot = y + ydiff;
		}
		else if (scaleState) {
			scalefactor = abs((x - xdiff) + (y - ydiff)) / 50.0;
		}
		else if (translateState) {
			xtranslate = (x - xdiff) * 0.005;
			ytranslate = (y - ydiff) * 0.005;
		}
	}
	glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y)
{
	static int polygonmode[2];
	switch (key) {
	case 'v':
		if (smoothOrFlat)
			smoothOrFlat = false;
		else
			smoothOrFlat = true;
		break;
	case 'c':
		lOnePos[0] = 0.0;
		lOnePos[1] = 0.0;
		lOnePos[2] = 1.2;
		lTwoPos[0] = 1.0;
		lTwoPos[1] = 0.0;
		lTwoPos[2] = 0.8;
		lThreePos[0] = -1.0;
		lThreePos[1] = 0.0;
		lThreePos[2] = 0.8;
		break;
	case 'o':
		if (lOnePos[3] == 0) {
			lOnePos[3] = 1.0;
			lTwoPos[3] = 1.0;
			lThreePos[3] = 1.0;
		}
		else {
			lOnePos[3] = 0.0;
			lTwoPos[3] = 0.0;
			lThreePos[3] = 0.0;
		}
		break;
	case 'p':
		lightOneColor[0] = 0.8;
		lightOneColor[1] = 0.0;
		lightOneColor[2] = 0.0;
		lightTwoColor[0] = 0.0;
		lightTwoColor[1] = 0.8;
		lightTwoColor[2] = 0.0;
		lightThreeColor[0] = 0.0;
		lightThreeColor[1] = 0.0;
		lightThreeColor[2] = 0.8;
		lOnePos[0] = 0.0;
		lOnePos[1] = 2.0;
		lOnePos[2] = 3.0;
		lTwoPos[0] = 3.0;
		lTwoPos[1] = 2.0;
		lTwoPos[2] = -2.0;
		lThreePos[0] = -3.0;
		lThreePos[1] = 2.0;
		lThreePos[2] = -2.0;
		break;
	case 'f':
		if (mLightOne) {
			lightOneColor[0] += 0.2;
		}
		if (mLightTwo) {
			lightTwoColor[0] += 0.2;
		}
		if (mLightThree) {
			lightThreeColor[0] += 0.2;
		}
		break;
	case 'g':
		if (mLightOne) {
			lightOneColor[0] -= 0.2;
		}
		if (mLightTwo) {
			lightTwoColor[0] -= 0.2;
		}
		if (mLightThree) {
			lightThreeColor[0] -= 0.2;
		}
		break;
	case 'h':
		if (mLightOne) {
			lightOneColor[1] += 0.2;
		}
		if (mLightTwo) {
			lightTwoColor[1] += 0.2;
		}
		if (mLightThree) {
			lightThreeColor[1] += 0.2;
		}
		glutPostRedisplay();
		break;
	case 'j':
		if (mLightOne) {
			lightOneColor[1] -= 0.2;
		}
		if (mLightTwo) {
			lightTwoColor[1] -= 0.2;
		}
		if (mLightThree) {
			lightThreeColor[1] -= 0.2;
		}
		break;
	case 'k':
		if (mLightOne) {
			lightOneColor[2] += 0.2;
		}
		if (mLightTwo) {
			lightTwoColor[2] += 0.2;
		}
		if (mLightThree) {
			lightThreeColor[2] += 0.2;
		}
		break;
	case 'l':
		if (mLightOne) {
			lightOneColor[2] -= 0.2;
		}
		if (mLightTwo) {
			lightTwoColor[2] -= 0.2;
		}
		if (mLightThree) {
			lightThreeColor[2] -= 0.2;
		}
		break;
	case 'r':
		rotateState = true;
		translateState = false;
		scaleState = false;
		mLightOne = false;
		mLightTwo = false;
		mLightThree = false;
		break;
	case 's':
		scaleState = true;
		translateState = false;
		rotateState = false;
		mLightOne = false;
		mLightTwo = false;
		mLightThree = false;
		break;
	case 't':
		translateState = true;
		scaleState = false;
		rotateState = false;
		mLightOne = false;
		mLightTwo = false;
		mLightThree = false;
		break;
	case 'm':
		polygonOne = true;
		polygonTwo = false;
		polygonThree = false;
		break;
	case 'n':
		polygonOne = false;
		polygonTwo = true;
		polygonThree = false;
		break;
	case 'b':
		polygonOne = false;
		polygonTwo = false;
		polygonThree = true;
		break;
	case '1':
		if (!ambientLight) {
			glDisable(GL_LIGHT0);
			ambientLight = true;
		}
		else if (ambientLight) {
			glEnable(GL_LIGHT0);
			ambientLight = false;
		}
		glutPostRedisplay();
		break;
	case '2':
		if (!lightsOutOne) {
			glDisable(GL_LIGHT1);
			lightsOutOne = true;
		}
		else if (lightsOutOne) {
			glEnable(GL_LIGHT1);
			lightsOutOne = false;
		}
		break;
	case '3':
		if (!lightsOutTwo) {
			glDisable(GL_LIGHT2);
			lightsOutTwo = true;
		}
		else if (lightsOutTwo) {
			glEnable(GL_LIGHT2);
			lightsOutTwo = false;
		}
		break;
	case '4':
		if (!lightsOutThree) {
			glDisable(GL_LIGHT3);
			lightsOutThree = true;
		}
		else if (lightsOutThree) {
			glEnable(GL_LIGHT3);
			lightsOutThree = false;
		}
		break;
	case 27:
		exit(0);
		break;
	case 'x':
		if (mLightOne) {
			lOnePos[0] = lOnePos[0] + 0.5;
		}
		else if (mLightTwo) {
			lTwoPos[0] = lTwoPos[0] + 0.5;

		}
		else if (mLightThree) {
			lThreePos[0] = lThreePos[0] + 0.5;
		}
		break;
	case 'X':
		if (mLightOne) {
			lOnePos[0] = lOnePos[0] - 0.5;

		}
		else if (mLightTwo) {
			lTwoPos[0] = lTwoPos[0] - 0.5;

		}
		else if (mLightThree) {
			lThreePos[0] = lThreePos[0] - 0.5;
		}
		break;
	case 'y':
		if (mLightOne) {
			lOnePos[1] = lOnePos[1] + 0.5;
		}
		else if (mLightTwo) {
			lTwoPos[1] = lTwoPos[1] + 0.5;
		}
		else if (mLightThree) {
			lThreePos[1] = lThreePos[1] + 0.5;
		}
		glutPostRedisplay();
		break;
	case 'Y':
		if (mLightOne) {
			lOnePos[1] = lOnePos[1] - 0.5;
		}
		else if (mLightTwo) {
			lTwoPos[1] = lTwoPos[1] - 0.5;

		}
		else if (mLightThree) {
			lThreePos[1] = lThreePos[1] - 0.5;
		}
		break;
	case 'z':
		if (mLightOne) {
			lOnePos[2] = lOnePos[2] + 0.5;

		}
		else if (mLightTwo) {
			lTwoPos[2] = lTwoPos[2] + 0.5;

		}
		else if (mLightThree) {
			lThreePos[2] = lThreePos[2] + 0.5;
		}
		glutPostRedisplay();
		break;
	case 'Z':
		if (mLightOne) {
			lOnePos[2] = lOnePos[2] - 0.5;

		}
		else if (mLightTwo) {
			lTwoPos[2] = lTwoPos[2] - 0.5;

		}
		else if (mLightThree) {
			lThreePos[2] = lThreePos[2] - 0.5;
		}
		break;
	case '7':
		mLightOne = true;
		mLightTwo = true;
		mLightThree = true;
		rotateState = false;
		translateState = false;
		scaleState = false;
	case '8':
		mLightOne = true;
		mLightTwo = false;
		mLightThree = false;
		rotateState = false;
		translateState = false;
		scaleState = false;
		break;
	case '9':
		mLightOne = false;
		mLightTwo = true;
		mLightThree = false;
		rotateState = false;
		translateState = false;
		scaleState = false;
		break;
	case '0':
		mLightOne = false;
		mLightTwo = false;
		mLightThree = true;
		rotateState = false;
		translateState = false;
		scaleState = false;
		break;
	case 'w':
		glGetIntegerv(GL_POLYGON_MODE, polygonmode);
		if (polygonmode[0] == GL_FILL)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void lightsOneRender(void) {
	glPushMatrix();
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightOneColor);
    glMaterialf(GL_FRONT, GL_SHININESS, 1.0);
	glLightfv(GL_LIGHT1, GL_POSITION, lOnePos);
	
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, thirtydegrees);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, exponent_val);
	glTranslatef(lOnePos[0], lOnePos[1], lOnePos[2]);
	glMaterialfv(GL_FRONT, GL_EMISSION, black);
	glColor3f(1.0f, 0.0f, 0.0f);
	glutSolidSphere(.1, 25, 25);
	glPopMatrix();

}

void lightsTwoRender(void) {
	glPushMatrix();
	glLightfv(GL_LIGHT2, GL_DIFFUSE, lightTwoColor);
	glMaterialf(GL_FRONT, GL_SPECULAR, 1.0);
	glLightfv(GL_LIGHT2, GL_POSITION, lTwoPos);

	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, sixtydegree);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, exponent_val);
	glTranslatef(lTwoPos[0], lTwoPos[1], lTwoPos[2]);
	glColor3f(0.0f, 1.0f, 0.0f);
	glutSolidSphere(.1, 25, 25);
	glPopMatrix();
}

void lightsThreeRender(void) {
	glPushMatrix();
	glLightfv(GL_LIGHT3, GL_DIFFUSE, lightThreeColor);
	glLightfv(GL_LIGHT3, GL_POSITION, lThreePos);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, ninteydegrees);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, exponent_val);
	glTranslatef(lThreePos[0], lThreePos[1], lThreePos[2]);
	glColor3f(0.0f, 0.0f, 1.0f);
	glutSolidSphere(.1, 25, 25);
	glMaterialfv(GL_FRONT, GL_EMISSION, black);
	glPopMatrix();
}

void polygonOneRender(void) {
	glPushMatrix();
	if (smoothOrFlat)
		glShadeModel(GL_SMOOTH);
	else
		glShadeModel(GL_FLAT);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128);
	glTranslatef(xtranslate, -ytranslate, 0.0);
	glScalef(scalefactor, scalefactor, scalefactor);
	glRotatef(xrot, 1.0f, 0.0f, 0.0f);
	glRotatef(-yrot, 0.0f, 1.0f, 0.0f);
	glColor4f(1.0, 1.0, 1.0, 0.5);
	if (polygonOne) {
		glutSolidTeapot(0.8);
	}
	else if (polygonTwo) {
		glutSolidSphere(0.8, 25, 25);
	}
	else if (polygonThree) {
		glShadeModel(GL_SMOOTH);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POLYGON);
		for (int i = 0; i < numPolygons; i++) {
			glBegin(GL_POLYGON);
			for (int j = 0; j < psize[i]; j++) {
				glNormal3fv(normals[i]);
				glVertex3fv(vdata[pdata[i][j]]);
			}
			glEnd();
		}
	}
	glPopMatrix();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Add ambient light
	glMaterialfv(GL_FRONT, GL_EMISSION, ambientColor);

	//Lookat Camera
	glTranslatef(0.0f, 0.0f, -15.0f);


	//Add directed light
	glPushMatrix();
	lightsOneRender();
	lightsTwoRender();
	lightsThreeRender();
	glPopMatrix();

	glPushMatrix();
	polygonOneRender();
	glPopMatrix();

	glFlush();
	glutSwapBuffers();

}

void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING); //Enable lighting
	glEnable(GL_LIGHT0); //Enable light #1
	glEnable(GL_LIGHT1); //Enable light #2
	glEnable(GL_LIGHT2); //Enable light #3
	glEnable(GL_LIGHT3); //Enable light #4
	glEnable(GL_NORMALIZE); //Automatically normalize normals
}

void resize(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(12.0, (GLfloat)w / (GLfloat)h, 1.5, 20.0);
	glMatrixMode(GL_MODELVIEW);

}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
		
	glutCreateWindow("CS459 - Term Project");
	initRendering();
	readOFF();
	calculateNormal();


	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(resize);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	writemessage();

	glutMainLoop();
	return 0;
}

