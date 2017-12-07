#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


static GLfloat lOnePos[] = {0.0, 0.0, 0.0, 1.0 };
static GLfloat lTwoPos[] = { 0.0, 1.0, 2.2, 1.0 };

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
	case 'r':
		rotateState = true;
		break;
	case 27:
		exit(0);
		break;
	case 'x':
		lOnePos[0] = lOnePos[0] + 0.2;
		glutPostRedisplay();
		break;
	case 'X':
		lOnePos[0] = lOnePos[0] - 0.2;
		glutPostRedisplay();
		break;
	case 'y':
		lOnePos[1] = lOnePos[1] + 0.2;
		glutPostRedisplay();
		break;
	case 'Y':
		lOnePos[1] = lOnePos[1] - 0.2;
		glutPostRedisplay();
		break;
	case 'z':
		lOnePos[2] = lOnePos[2] + 0.2;
		glutPostRedisplay();
		break;
	case 'Z':
		lOnePos[2] = lOnePos[2] - 0.2;
		glutPostRedisplay();
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
	gluLookAt(
		0.0f, 0.0f, 10.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f);

	glPushMatrix();
	glLightfv(GL_LIGHT0, GL_POSITION, lOnePos);
	glRotatef(xrot, 1.0f, 0.0f, 0.0f);
	glRotatef(-yrot, 0.0f, 1.0f, 0.0f);
	glTranslatef(lOnePos[0], lOnePos[1], lOnePos[2]);
	glMaterialfv(GL_FRONT, GL_EMISSION, white);
	glutSolidSphere(.08, 25, 25);
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_EMISSION, black);
	//Polygon One
	glPushMatrix();
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 1);
	glRotatef(xrot, 1.0f, 0.0f, 0.0f);
	glRotatef(-yrot, 0.0f, 1.0f, 0.0f);
	glTranslatef(3.0, 0.0, 0.0);
	glColor3f(1.0, 1.0, 0.0);
	glutSolidTeapot(1.0);
	glPopMatrix();

	//Polygon Two
	glPushMatrix();
	glRotatef(xrot, 1.0f, 0.0f, 0.0f);
	glRotatef(-yrot, 0.0f, 1.0f, 0.0f);
	glTranslatef(-3.0, 0.0, 0.0);
	glColor3f(0.0, 1.0, 0.0);
	glutSolidSphere(.8, 25, 25);
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
