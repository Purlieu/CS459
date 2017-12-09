#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


static GLfloat lOnePos[] = {0.0, 2.0, 0.0, 1.0 };
static GLfloat lTwoPos[] = { 3.0, 2.0, 0.0, 1.0 };
static GLfloat lThreePos[] = { -3.0, 2.0, 0.0, 1.0 };

static GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };
static GLfloat red[] = { 1.0, 0.0, 0.0, 1.0 };
static GLfloat blue[] = { 0.0, 0.0, 1.0, 1.0 };
static GLfloat green[] = { 0.0, 1.0, 0.0, 1.0 };
static GLfloat lookat[] = { 0.0, 0.0, -20.0 };

float xrot = 0.0f;
float yrot = 0.0f;

float xdiff = 0.0f;
float ydiff = 0.0f;

bool mouseDown = false;
bool rotateState = true;
bool lightsOutOne = false;
bool lightsOutTwo = false;

bool mLightOne = false;
bool mLightTwo = false;
bool mLightThree = false;

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		mouseDown = true;
		if (rotateState) {
			xdiff = x - yrot;
			ydiff = -y + xrot;
		}
		else
			mouseDown = false;
	}
}

void mouseMotion(int x, int y)
{
	if (mouseDown)
	{
		if (rotateState) {
			yrot = x - xdiff;
			xrot = y + ydiff;
		}
	}
	glutPostRedisplay();

}

void keyboard(unsigned char key, int x, int y)
{
	static int polygonmode[2];
	switch (key) {
	case '1':
		if (!lightsOutOne) {
			glDisable(GL_LIGHT0);
			lightsOutOne = true;
		}
		else if (lightsOutOne) {
			glEnable(GL_LIGHT0);
			lightsOutOne = false;
		}
		glutPostRedisplay();
		break;
	case '2':
		if (!lightsOutTwo) {
			glDisable(GL_LIGHT1);
			lightsOutTwo = true;
		}
		else if (lightsOutTwo) {
			glEnable(GL_LIGHT1);
			lightsOutTwo = false;
		}
		glutPostRedisplay();
		break;
	case 'r':
		rotateState = true;
		break;
	case 27:
		exit(0);
		break;
	case 'x':
		if (mLightOne) {
			lOnePos[0] = lOnePos[0] + 0.2;

		}
		else if (mLightTwo) {
			lTwoPos[0] = lTwoPos[0] + 0.2;

		}
		else if (mLightThree) {
			lThreePos[0] = lThreePos[0] + 0.2;
		}
		glutPostRedisplay();
		break;
	case 'X':
		if (mLightOne) {
			lOnePos[0] = lOnePos[0] - 0.2;

		}
		else if (mLightTwo) {
			lTwoPos[0] = lTwoPos[0] - 0.2;

		}
		else if (mLightThree) {
			lThreePos[0] = lThreePos[0] - 0.2;
		}
		glutPostRedisplay();
		break;
	case 'y':
		if (mLightOne) {
			lOnePos[1] = lOnePos[1] + 0.2;

		}
		else if (mLightTwo) {
			lTwoPos[1] = lTwoPos[1] + 0.2;

		}
		else if (mLightThree) {
			lThreePos[1] = lThreePos[1] + 0.2;
		}		
		glutPostRedisplay();
		break;
	case 'Y':
		if (mLightOne) {
			lOnePos[1] = lOnePos[1] - 0.2;

		}
		else if (mLightTwo) {
			lTwoPos[1] = lTwoPos[1] - 0.2;

		}
		else if (mLightThree) {
			lThreePos[1] = lThreePos[1] - 0.2;
		}
		glutPostRedisplay();
		break;
	case 'z':
		if (mLightOne) {
			lOnePos[2] = lOnePos[2] + 0.2;

		}
		else if (mLightTwo) {
			lTwoPos[2] = lTwoPos[2] + 0.2;

		}
		else if (mLightThree) {
			lThreePos[2] = lThreePos[2] + 0.2;
		}
		glutPostRedisplay();
		break;
	case 'Z':
		if (mLightOne) {
			lOnePos[2] = lOnePos[2] - 0.2;

		}
		else if (mLightTwo) {
			lTwoPos[2] = lTwoPos[2] - 0.2;

		}
		else if (mLightThree) {
			lThreePos[2] = lThreePos[2] - 0.2;
		}		
		glutPostRedisplay();
		break;
	case '8':
		mLightOne = true;
		mLightTwo = false;
		mLightThree = false;
		break;
	case '9':
		mLightOne = false;
		mLightTwo = true;
		mLightThree = false;
		break;
	case '0':
		mLightOne = false;
		mLightTwo = false;
		mLightThree = true;
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


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMaterialfv(GL_FRONT, GL_EMISSION, white);

	//Lookat Camera
	

	/*glPushMatrix();
	glLightfv(GL_LIGHT0, GL_POSITION, lOnePos);
	glRotatef(xrot, 1.0f, 0.0f, 0.0f);
	glRotatef(-yrot, 0.0f, 1.0f, 0.0f);
	glTranslatef(lOnePos[0], lOnePos[1], lOnePos[2]);
	glMaterialfv(GL_FRONT, GL_EMISSION, white);
	glutSolidSphere(.08, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glLightfv(GL_LIGHT1, GL_POSITION, lTwoPos);
	glRotatef(xrot, 1.0f, 0.0f, 0.0f);
	glRotatef(-yrot, 0.0f, 1.0f, 0.0f);
	glTranslatef(lTwoPos[0], lTwoPos[1], lTwoPos[2]);
	glMaterialfv(GL_FRONT, GL_EMISSION, blue);
	glutSolidSphere(.08, 25, 25);
	glPopMatrix();
	*/

	glTranslatef(0.0f, 0.0f, -12.0f);



	glPushMatrix();
	glLightfv(GL_LIGHT1, GL_DIFFUSE, red);
	glLightfv(GL_LIGHT1, GL_POSITION, lOnePos);
	glRotatef(xrot, 1.0f, 0.0f, 0.0f);
	glRotatef(-yrot, 0.0f, 1.0f, 0.0f);
	glTranslatef(lOnePos[0], lOnePos[1], lOnePos[2]);
	glColor3f(1.0f, 0.0f, 0.0f);
	glutSolidSphere(.08, 25, 25);
	glMaterialfv(GL_FRONT, GL_EMISSION, black);
	glPopMatrix();

	glPushMatrix();
	glLightfv(GL_LIGHT2, GL_DIFFUSE, green);
	glLightfv(GL_LIGHT2, GL_POSITION, lTwoPos);
	glRotatef(xrot, 1.0f, 0.0f, 0.0f);
	glRotatef(-yrot, 0.0f, 1.0f, 0.0f);
	glTranslatef(lTwoPos[0], lTwoPos[1], lTwoPos[2]);
	glColor3f(0.0f, 1.0f, 0.0f);
	glutSolidSphere(.08, 25, 25);
	glMaterialfv(GL_FRONT, GL_EMISSION, black);
	glPopMatrix();

	glPushMatrix();
	glLightfv(GL_LIGHT3, GL_DIFFUSE, blue);
	glLightfv(GL_LIGHT3, GL_POSITION, lThreePos);
	glRotatef(xrot, 1.0f, 0.0f, 0.0f);
	glRotatef(-yrot, 0.0f, 1.0f, 0.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glTranslatef(lThreePos[0], lThreePos[1], lThreePos[2]);
	glutSolidSphere(.08, 25, 25);
	glMaterialfv(GL_FRONT, GL_EMISSION, black);
	glPopMatrix();

	//Polygon One
	glPushMatrix();
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 1);
	glRotatef(xrot, 1.0f, 0.0f, 0.0f);
	glRotatef(-yrot, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0, 0.0, 0.0);
	glColor3f(1.0, 1.0, 0.0);
	glutSolidTeapot(1.0);
	glPopMatrix();


	glFlush();
	glutSwapBuffers();

}

void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING); //Enable lighting
	glEnable(GL_LIGHT0); //Enable light #0
	glEnable(GL_LIGHT1); //Enable light #1
	glEnable(GL_NORMALIZE); //Automatically normalize normals
	//glShadeModel(GL_SMOOTH); //Enable smooth shading

}

void resize(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 1.5, 20.0);
	glMatrixMode(GL_MODELVIEW);

}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1200, 1200);
		
	glutCreateWindow("CS459 - Term Project");
	initRendering();


	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(resize);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);


	glutMainLoop();
	return 0;
}
