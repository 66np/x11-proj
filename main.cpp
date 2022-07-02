#include <iostream>
#include <sstream>
#include <cstring>
#include <string>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include "headers/rgbo.hpp"

std::string buffer;

static void keyboard(unsigned char key, int x, int y) {
	buffer.push_back((char) key); 
	glutPostRedisplay();
}

void displayUserInput(float x, float y, void *font, const unsigned char* string, RGBO rgbo) {
	char *c;
	glColor4f(rgbo.r, rgbo.g, rgbo.b, rgbo.opacity);
	glRasterPos2f(x, y);

	glutBitmapString(font, string);
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	
	
	displayUserInput(0.0f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*) buffer.c_str(), {1.0f, 0.4f, 0.2f, 1.0f});
	
	glutSwapBuffers(); 
}

int main(int argc, char** argv) {
	struct winsize windowObj;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &windowObj);
	
	glutInit(&argc, argv);
	glutInitWindowSize(windowObj.ws_xpixel, windowObj.ws_ypixel);
	glutInitWindowPosition(-windowObj.ws_xpixel/2, -windowObj.ws_ypixel/2);
	glutCreateWindow("cad");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);	
	glutMainLoop();
	return 0;
}
