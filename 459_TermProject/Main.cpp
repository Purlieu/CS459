#include <stdlib.h>
#include <GL/glut.h>

GLfloat ctrlpoints[4][3] = {
	{ -4.0, -4.0, 0.0 },{ -2.0, 4.0, 0.0 },
	{ 2.0, -4.0, 0.0 },{ 4.0, 4.0, 0.0 } };

void init(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);

}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);


	glFlush();
}

void resize(int w, int h) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, w, h);
	gluPerspective(45.0f, 1.0f * w / h, 1.0f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 500);
	glutInitWindowPosition(300, 100);
	glutCreateWindow(argv[0]);
	glShadeModel(GL_SMOOTH);

	init();
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutMainLoop();
	return 0;
}