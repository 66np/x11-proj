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
#include <X11/Xlib.h>
#include "headers/rgbo.hpp"
#include "headers/globals.hpp"
using namespace std;

std::string buffer;
int lineNum = 0;
int pSize = 1;
char** pCommand = (char**)malloc(pSize * sizeof(char*));
int pPos = 0;

Display* xDisplay = XOpenDisplay(NULL);
Screen* xScreen = DefaultScreenOfDisplay(xDisplay);
float clickXPos = 0.0f;
float clickYPos = 0.0f;
	
void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		clickXPos = ((float) x - (float) xScreen->width / 2) / ((float) xScreen->width / 2);
		clickYPos = -1 * ((float) y - (float) xScreen->height / 2) / ((float) xScreen->height / 2);
	}
}

enum codes {
	clear,
};

codes hashIt(std::string const& command) {
	codes s; 
	if (command == "clear") s = clear;
	return s;
}

static void keyboard(unsigned char key, int x, int y) {
	if ((int) key == 13) {
		if (buffer[buffer.size() - 1] == '\n') {
			buffer.append("err: you must enter a valid command\n");
		} else {
			// find last position of \n before inserting new \n 
			std::size_t pos = buffer.rfind("\n");
			if (pos == std::string::npos)
				pos = -1;
			switch (hashIt(buffer.substr(pos + 1))) {
				case clear:
					pPos = 0;
					pSize = 1;	
					pCommand = (char**)realloc(pCommand, pSize * sizeof(char*));	
					buffer = "";
					// accounts for 'clear' command to take up a line
					lineNum = -1;
					break;
				default:
					*(pCommand+pPos) = (char*)malloc(buffer.substr(pos+1).length() * sizeof(char));
					pPos = pPos + 1;
					pSize = pSize + 1;
					pCommand = (char**)realloc(pCommand, pSize * sizeof(char*));
					buffer.push_back('\n');
			}
		}
		lineNum = lineNum + 1;
		if (lineNum > 5) {
			std::size_t pos = buffer.find('\n');
			if (pos != std::string::npos) {
				buffer = buffer.substr(pos+1);
			}
		}
	} else if ((int) key == 8) {
		buffer.pop_back();
	} else if ((int) key == 27) {
		free(pCommand);
		pCommand = NULL;	
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

void renderAxes(float x, float y, RGBO rgbo) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(rgbo.r, rgbo.g, rgbo.b, rgbo.opacity);

	glBegin(GL_LINES);
		glVertex2f(x, -1.0f);
		glVertex2f(x, 1.0f);
		glVertex2f(-1.0f, y);
		glVertex2f(1.0f, y);
	glEnd();
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	

	// offset from edge of the window on y-axis to account for title bar height
	float offset = 0.065f;

	displayUserInput(-1.0f, 1.0f - offset, GLUT_BITMAP_8_BY_13, (const unsigned char*) buffer.c_str(), {1.0f, 1.0f, 1.0f, 1.0f});
	renderAxes(clickXPos, clickYPos, {1.0f, 0.0f, 0.0f, 1.0f});
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
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);	
	glutMainLoop();
	return 0;
}
