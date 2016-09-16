// https://www.opengl.org/discussion_boards/showthread.php/165856-Minimal-GLX-OpenGL3-0-example
#ifdef ORIGINAL_POST
// ORIGINAL_POST 10-25-2008, 01:18 PM works with NVidia, fails with AMD
#include <GL/glx.h>
#include <GL/gl.h>
#include <unistd.h>
#include <iostream>
 
#define GLX_CONTEXT_MAJOR_VERSION_ARB		0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB		0x2092
typedef GLXContext (*GLXCREATECONTEXTATTRIBSARBPROC)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
 
 
int main (int argc, char ** argv)
{
	Display *dpy = XOpenDisplay(0);
 
	int nelements;
	GLXFBConfig *fbc = glXChooseFBConfig(dpy, DefaultScreen(dpy), 0, &nelements);
 
	static int attributeList[] = { GLX_RGBA, GLX_DOUBLEBUFFER, GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 1, GLX_BLUE_SIZE, 1, None };
	XVisualInfo *vi = glXChooseVisual(dpy, DefaultScreen(dpy),attributeList);
 
	XSetWindowAttributes swa;
	swa.colormap = XCreateColormap(dpy, RootWindow(dpy, vi->screen), vi->visual, AllocNone);
	swa.border_pixel = 0;
	swa.event_mask = StructureNotifyMask;
	Window win = XCreateWindow(dpy, RootWindow(dpy, vi->screen), 0, 0, 100, 100, 0, vi->depth, InputOutput, vi->visual, CWBorderPixel|CWColormap|CWEventMask, &swa);
 
	XMapWindow (dpy, win);
 
	//oldstyle context:
	//	GLXContext ctx = glXCreateContext(dpy, vi, 0, GL_TRUE);

  std::cout << "glXCreateContextAttribsARB " << (void*) glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB") << std::endl;
	GLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB = (GLXCREATECONTEXTATTRIBSARBPROC) glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");
 
	int attribs[] = {
		GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
		GLX_CONTEXT_MINOR_VERSION_ARB, 0,
		0};
 
	GLXContext ctx = glXCreateContextAttribsARB(dpy, *fbc, 0, true, attribs);
 
	glXMakeCurrent (dpy, win, ctx);
 
		glClearColor (0, 0.5, 1, 1);
		glClear (GL_COLOR_BUFFER_BIT);
		glXSwapBuffers (dpy, win);
 
		sleep(1);
 
		glClearColor (1, 0.5, 0, 1);
		glClear (GL_COLOR_BUFFER_BIT);
		glXSwapBuffers (dpy, win);
 
		sleep(1);
 
	ctx = glXGetCurrentContext(); 
	glXDestroyContext(dpy, ctx); 
}
#else // updated post 03-07-2009, 07:55 AM
// updated post works with NVidia and AMD
#include <GL/glx.h>
#include <GL/gl.h>
#include <unistd.h>
#include <iostream>
 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
 
#define GLX_CONTEXT_MAJOR_VERSION_ARB       0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB       0x2092
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
 
int main (int argc, char ** argv)
{
    Display *display = XOpenDisplay(0);
 
    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = NULL;
 
    const char *extensions = glXQueryExtensionsString(display, DefaultScreen(display));
    std::cout << extensions << std::endl;
 
    static int visual_attribs[] = {
        GLX_RENDER_TYPE, GLX_RGBA_BIT,
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_DOUBLEBUFFER, true,
        GLX_RED_SIZE, 1,
        GLX_GREEN_SIZE, 1,
        GLX_BLUE_SIZE, 1,
        None
     };
 
    std::cout << "Getting framebuffer config" << std::endl;
    int fbcount;
    GLXFBConfig *fbc = glXChooseFBConfig(display, DefaultScreen(display), visual_attribs, &fbcount);
    if (!fbc) {
        std::cout << "Failed to retrieve a framebuffer config" << std::endl;
        return 1;
    }
 
    std::cout << "Getting XVisualInfo" << std::endl;
    XVisualInfo *vi = glXGetVisualFromFBConfig(display, fbc[0]);
 
    XSetWindowAttributes swa;
    std::cout << "Creating colormap" << std::endl;
    swa.colormap = XCreateColormap(display, RootWindow(display, vi->screen), vi->visual, AllocNone);
    swa.border_pixel = 0;
    swa.event_mask = StructureNotifyMask;
 
    std::cout << "Creating window" << std::endl;
    Window win = XCreateWindow(display, RootWindow(display, vi->screen), 0, 0, 100, 100, 0, vi->depth, InputOutput, vi->visual, CWBorderPixel|CWColormap|CWEventMask, &swa);
    if (!win) {
        std::cout << "Failed to create window." << std::endl;
        return 1;
    }
 
    std::cout << "Mapping window" << std::endl;
    XMapWindow(display, win);
 
    // Create an oldstyle context first, to get the correct function pointer for glXCreateContextAttribsARB
    GLXContext ctx_old = glXCreateContext(display, vi, 0, GL_TRUE);
    glXCreateContextAttribsARB =  (glXCreateContextAttribsARBProc)glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");
    glXMakeCurrent(display, 0, 0);
    glXDestroyContext(display, ctx_old);
 
    if (glXCreateContextAttribsARB == NULL) {
        std::cout << "glXCreateContextAttribsARB entry point not found. Aborting." << std::endl;
        return false;
    }
 
    static int context_attribs[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
        GLX_CONTEXT_MINOR_VERSION_ARB, 0,
        None
    };
 
    std::cout << "Creating context" << std::endl;
    GLXContext ctx = glXCreateContextAttribsARB(display, fbc[0], NULL, true, context_attribs);
    if (!ctx) {
        std::cout << "Failed to create GL3 context." << std::endl;
        return 1;
    }
 
    std::cout << "Making context current" << std::endl;
    glXMakeCurrent(display, win, ctx);
 
        glClearColor (0, 0.5, 1, 1);
        glClear (GL_COLOR_BUFFER_BIT);
        glXSwapBuffers (display, win);
 
        sleep(1);
 
        glClearColor (1, 0.5, 0, 1);
        glClear (GL_COLOR_BUFFER_BIT);
        glXSwapBuffers (display, win);
 
        sleep(1);
 
    ctx = glXGetCurrentContext();
    glXMakeCurrent(display, 0, 0);
    glXDestroyContext(display, ctx);
}
#endif
