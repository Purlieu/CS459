#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>

static GLfloat lOnePos[] = {0.0, 2.0, 3.0, 1.0 };
static GLfloat lTwoPos[] = { 3.0, 2.0, -2.0, 1.0 };
static GLfloat lThreePos[] = { -3.0, 2.0, -2.0, 1.0 };

static GLfloat ambientColor[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat yellow[] = { 1.0, 1.0, 0.0, 1.0 };
static GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };
static GLfloat lightOneColor[] = { 1.0, 0.0, 0.0, 1.0 };
static GLfloat lightTwoColor[] = { 0.0, 1.0, 0.0, 1.0 };
static GLfloat lightThreeColor[] = { 0.0, 0.0, 1.0, 1.0 };
static GLfloat lookat[] = { 0.0, 0.0, -20.0 };
static GLfloat direction[] = { 0.0, 0.0, -1.0 };
static GLfloat angle_in_degree = 90.0;
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
		   b, n, m, -----------------  Draw 3 separate polygons\n\
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
			xtranslate = (x - xdiff) * 0.01;
			ytranslate = (y - ydiff) * 0.01;
		}
	}
	glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y)
{
	static int polygonmode[2];
	switch (key){
	case 'v':
		if(smoothOrFlat)
			smoothOrFlat = false;
		else
			smoothOrFlat = true;
	case 'c':
		lOnePos[0] = 0.0;
		lOnePos[1] = 0.0;
		lOnePos[2] = 1.5;
		lTwoPos[0] = 1.0;
		lTwoPos[1] = 0.0;
		lTwoPos[2] = 1.0;
		lThreePos[0] = -1.0;
		lThreePos[1] = 0.0;
		lThreePos[2] = 1.0;
		glutPostRedisplay();
		break;
	case 'o':
		if (lOnePos[3] == 0) {
			lOnePos[3] = 1;
			lTwoPos[3] = 1;
			lThreePos[3] = 1;
		}
		else {
			lOnePos[3] = 0;
			lTwoPos[3] = 0;
			lThreePos[3] = 0;
			}
		glutPostRedisplay();
		break;
	case 'p':
		lightOneColor[0] = 1.0;
		lightOneColor[1] = 0.0;
		lightOneColor[2] = 0.0;
		lightTwoColor[0] = 0.0;
		lightTwoColor[1] = 1.0;
		lightTwoColor[2] = 0.0;
		lightThreeColor[0] = 0.0;
		lightThreeColor[1] = 0.0;
		lightThreeColor[2] = 1.0;
		lOnePos[0] = 0.0;
		lOnePos[1] = 2.0;
		lOnePos[2] = 3.0;
		lTwoPos[0] = 3.0;
		lTwoPos[1] = 2.0;
		lTwoPos[2] = -2.0;
		lThreePos[0] = -3.0;
		lThreePos[1] = 2.0;
		lThreePos[2] = -2.0;
		glutPostRedisplay();
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
		glutPostRedisplay();
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
		glutPostRedisplay();
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
		glutPostRedisplay();
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
		glutPostRedisplay();
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
		glutPostRedisplay();
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
		glutPostRedisplay();
		break;
	case 'n':
		polygonOne = false;
		polygonTwo = true;
		polygonThree = false;
		glutPostRedisplay();
		break;
	case 'b':
		polygonOne = false;
		polygonTwo = false;
		polygonThree = true;
		glutPostRedisplay();
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
		glutPostRedisplay();
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
		glutPostRedisplay();
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
		glutPostRedisplay();
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
		glutPostRedisplay();
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
		glutPostRedisplay();
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
		glutPostRedisplay();
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
		glutPostRedisplay();
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
		glutPostRedisplay();
		break;
	default:
		break;
	}

}
void lightsOneRender(void) {
	glPushMatrix();

	glLightfv(GL_LIGHT1, GL_POSITION, lOnePos);
	if (lOnePos[4] == 1) {
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, angle_in_degree);
		glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, exponent_val);
	}
	else {
		glLightfv(GL_LIGHT1, GL_DIFFUSE, lightOneColor);
		glMaterialf(GL_FRONT, GL_SHININESS, 1.0);
	}
	glTranslatef(lOnePos[0], lOnePos[1], lOnePos[2]);
	glMaterialfv(GL_FRONT, GL_EMISSION, black);
	glColor3f(1.0f, 0.0f, 0.0f);
	glutSolidSphere(.08, 25, 25);
	glPopMatrix();

}

void lightsTwoRender(void) {
	glPushMatrix();
	glLightfv(GL_LIGHT2, GL_DIFFUSE, lightTwoColor);
	glLightfv(GL_LIGHT2, GL_POSITION, lTwoPos);
	if (lTwoPos[4] == 1) {
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, angle_in_degree);
		glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, exponent_val);
	}
	glTranslatef(lTwoPos[0], lTwoPos[1], lTwoPos[2]);
	glColor3f(0.0f, 1.0f, 0.0f);
	glutSolidSphere(.08, 25, 25);
	glMaterialfv(GL_FRONT, GL_EMISSION, black);
	glPopMatrix();
}

void lightsThreeRender(void) {
	glPushMatrix();
	glLightfv(GL_LIGHT3, GL_DIFFUSE, lightThreeColor);
	glLightfv(GL_LIGHT3, GL_POSITION, lThreePos);
	if (lThreePos[4] == 1) {
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, angle_in_degree);
		glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, exponent_val);
	}
	else {
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 180.0);
		glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 0.0);
	}
	glColor3f(0.0f, 0.0f, 1.0f);
	glTranslatef(lThreePos[0], lThreePos[1], lThreePos[2]);
	glutSolidSphere(.08, 25, 25);
	glMaterialfv(GL_FRONT, GL_EMISSION, black);
	glPopMatrix();
}

void polygonOneRender(void) {
	glPushMatrix();
	if (smoothOrFlat)
		glShadeModel(GL_SMOOTH);
	else
		glShadeModel(GL_FLAT);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.1);
	glTranslatef(xtranslate, -ytranslate, 0.0);
	glScalef(scalefactor, scalefactor, scalefactor);
	glRotatef(xrot, 1.0f, 0.0f, 0.0f);
	glRotatef(-yrot, 0.0f, 1.0f, 0.0f);
	glColor3f(1.0, 1.0, 1.0);
	if (polygonOne) {
		glutSolidTeapot(1.0);
	}
	else if (polygonTwo) {
		glutSolidSphere(1, 25, 25);
	}
	else if (polygonThree) {
		glutSolidTorus(1.0, 1.5, 2, 100);
	}
	glPopMatrix();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMaterialfv(GL_FRONT, GL_EMISSION, ambientColor);

	//Lookat Camera
	glTranslatef(0.0f, 0.0f, -12.0f);

	//Add ambient light

	//Add directed light
	glPopMatrix();

	glPushMatrix();
	lightsOneRender();
	lightsTwoRender();
	lightsThreeRender();
	glPopMatrix();

	glPushMatrix();
	polygonOneRender();

	glPopMatrix();

	//Polygon One

	glFlush();
	glutSwapBuffers();

}

void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING); //Enable lighting
	glEnable(GL_LIGHT0); //Enable light #1
	glEnable(GL_LIGHT1); //Enable light #1
	glEnable(GL_LIGHT2); //Enable light #2
	glEnable(GL_LIGHT3); //Enable light #3
	glEnable(GL_NORMALIZE); //Automatically normalize normals
	//glShadeModel(GL_SMOOTH); //Enable smooth shading

}

void resize(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(20.0, (GLfloat)w / (GLfloat)h, 1.5, 20.0);
	glMatrixMode(GL_MODELVIEW);

}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
		
	glutCreateWindow("CS459 - Term Project");
	initRendering();


	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(resize);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	writemessage();

	glutMainLoop();
	return 0;
}

