#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string>

int main(void) {
	Display *d;
	Window w;
	XEvent e;
	KeySym key;

	unsigned long bg = 0x001a1a1a;	
	char buffer[255];
	int numKeys = 0;	
	int s;

	d = XOpenDisplay(NULL);
	if (d == NULL) {
		fprintf(stderr, "Cannot open display\n");
		exit(1);
	}

	s = DefaultScreen(d);
	w = XCreateSimpleWindow(d, RootWindow(d, s), 30, 333, 600, 400, 1, WhitePixel(d, s), bg);
	XSetStandardProperties(d, w, "cad-project", "cad-project", None, NULL, 0, NULL);
	XSelectInput(d, w, ExposureMask|ButtonPressMask|KeyPressMask);
	XMapWindow(d, w);
	
	while (1) {
		XNextEvent(d, &e);

		if (e.type==KeyPress&&XLookupString(&e.xkey,buffer,255,&key,0)==1){
			printf("%c", buffer[0]);
			XDrawString(d, w, DefaultGC(d, s), 10, 50, (std::string) buffer[0], 1);
		}
		if (e.type == Expose) {
	//		XFillRectangle(d, w, DefaultGC(d, s), 20, 20, 10, 10);
//			XDrawString(d, w, DefaultGC(d, s), 10, 50, buffer, strlen(buffer));
		}
	}

	XCloseDisplay(d);
	return 0;
}
