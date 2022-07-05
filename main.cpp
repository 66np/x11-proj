#include <iostream>
#include <sstream>
#include <cstring>
#include <string>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "headers/rgbo.hpp"
#include "headers/globals.hpp"
using namespace std;

std::string buffer;
static int lineNum = 0;
int stringSize = 0;
std::string *pCommand = new (nothrow) std::string[stringSize];
int pPos = 0;

static void keyboard(unsigned char key, int x, int y) {
	if ((int) key == 13) {
		if (buffer[buffer.size() - 1] == '\n') {
			buffer.append("err: you must enter a valid command\n");
		} else { 
			std::size_t pos = buffer.rfind('\n');
			if (pos != std::string::npos) {

				switch (buffer.substr(pPos + 1)) {
					case "clear":
						delete[] pCommand;
						buffer = "";
						break;
					default:
						stringSize = buffer.substr(pPos + 1).length();
						pCommand[pPos] = buffer.substr(pPos + 1);
						pPos = pPos + 1;
				}
			}
			buffer.push_back('\n');
		}
		lineNum = lineNum + 1;
		if (lineNum >= 5) {
			std::size_t pos = buffer.find('\n');
			if (pos != std::string::npos) {
				buffer = buffer.substr(pos+1);
			}
		}
	} else if ((int) key == 8) {
		buffer.pop_back();
	} else if ((int) key == 27) {
		exit(3);
	} else {
		buffer.push_back((char) key); 
	}	
	glutPostRedisplay();
}

void displayUserInput(float x, float y, void *font, const unsigned char* string, RGBO rgbo) {
	char *c;
	glColor4f(rgbo.r, rgbo.g, rgbo.b, rgbo.opacity);
	glRasterPos2f(x, y);

	glutBitmapString(font, string);
}

void display(void) {
	struct winsize windowObj;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &windowObj);

	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	

	// offset from edge of the window on y-axis to account for title bar height
	float offset = 0.065f;

	displayUserInput(-1.0f, 1.0f - offset, GLUT_BITMAP_8_BY_13, (const unsigned char*) buffer.c_str(), {1.0f, 1.0f, 1.0f, 1.0f});

	glutSwapBuffers();
}

int main(int argc, char** argv) {
	struct winsize windowObj;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &windowObj);

	glutInit(&argc, argv);
	glutInitWindowSize(windowObj.ws_xpixel, windowObj.ws_ypixel);
	glutInitWindowPosition(-1, 1);
	glutCreateWindow("cad");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);	
	glutMainLoop();
	return 0;
}
