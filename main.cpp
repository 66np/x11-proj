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

static void key(unsigned char key, int x, int y) {
	buffer.push_back((char) key); 
	glutPostRedisplay;
}

void renderUserInput(float x, float y, void *font, const unsigned char* string, RGBO rgbo) {
	char *c;
	glColor4f(rgbo.r, rgbo.g, rgbo.b, rgbo.opacity);
	glRasterPos2f(x, y);

	glutBitmapString(font, string);
}

void display(void) {
//	const unsigned char* userInput = reinterpret_cast<const unsigned char*>(buffer);
	renderUserInput(0.0f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*) buffer.c_str(), {1.0f, 1.0f, 1.0f, 1.0f}); 
}

int main(int argc, char** argv) {
	struct winsize windowObj;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &windowObj);
	
	glutInit(&argc, argv);

	glutKeyboardFunc(key);

	glutInitWindowSize(windowObj.ws_xpixel, windowObj.ws_ypixel);
	glutInitWindowPosition(-windowObj.ws_xpixel/2, -windowObj.ws_ypixel/2);
	glutCreateWindow("cad");
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}
