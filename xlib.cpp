// https://tronche.com/gui/x/xlib-tutorial/2nd-program-anatomy.html
// GB link with -lX11

// GB more complete examples http://tldp.org/LDP/LGNET/78/tougher.html

// Anatomy of the most basic Xlib program
// The program starts with the legal stuff:


#include <X11/Xlib.h> // Every Xlib program must include this
#include <assert.h>   // I include this to test return values the lazy way
#include <unistd.h>   // So we got the profile for 10 seconds

#define NIL (0)       // A name for the void pointer

int main()
{
// Then the serious thing. First we open a connection to the server.


Display *dpy = XOpenDisplay(NIL);
assert(dpy);

// If it fails (and it may), XOpenDisplay() will return NIL.

// We gonna create a window, but we need to get the window's background color first. X uses a quite complex color model in order to accommodate to every conceivable piece of hardware. Each color is encoded by an integer, but the integer for a given color may change from a machine to another one, and even on the same machine, from an execution of the program to the next. The only "colors" that X guarantees to exist are black and white. We can get them using the BlackPixel() and WhitePixel() macros.


      int blackColor = BlackPixel(dpy, DefaultScreen(dpy));
      int whiteColor = WhitePixel(dpy, DefaultScreen(dpy));

// As we yet can see, most of the Xlib calls take the "display" (the value returned by XOpenDisplay()) as their first parameter. You want to know why ?

// There is still someting magic, (the DefaultScreen() stuff), but we gonna keep it for a later explanation. We now can create our window:


      // Create the window

      Window w = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0, 
				     200, 100, 0, blackColor, blackColor);

// Unlike what appears in the dialog, we use the function XCreateSimpleWindow() instead of XCreateWindow(). XCreateSimpleWindow() is not really simpler than XCreateWindow() (it takes only a few less parameters), but it uses less concepts, so we gonna stick to it for now. There are still a bunch of parameters to explain:

//     dpy is the usual display connection (remember).

//     DefaultRootWindow(dpy): yet another parameter that may seem magic until now. This is the "parent window" of the window we are creating. The window we create appears inside its parent, and is bounded by it (the window is said to be "clipped" by its parent). Those who guess from the name "Default" that they may be other root windows guess right. More on this later. For now, the default root window makes appear our window on the screen, and will give the window manager a chance to decorate our window.

//     0, 0 These are the coordinates of the upper left corner of the window (the origin of the coordinates in X is at the upper left, contrary to most mathematical textbooks). The dimensions, like every dimensions in X, are in pixels (X does not support user-defined scales, unlike some other graphical systems like OpenGL).

//     Contrary to what may seem, there is very little chance for the window to appear, at 0,0. The reason is that the window manager will put the window at its policy-defined position.

//     200, 100: these are the width and height of the window, in pixels.

//     0: this is the width of the window's border. This has nothing to do with the border appended by the window manager, so this is most often set to zero.

//     blackColor, blackColor: these are the colors of the window's border (NOT the window manager's border), and the window's background, respectively. XCreateSimpleWindow() clears the window when created, XCreateWindow() does not. 


      // We want to get MapNotify events

      XSelectInput(dpy, w, StructureNotifyMask);

// As we're starting to know, X is based upon a client-server architecture. The X server sends events to the client (the program we're writing), to keep it informed of the modifications in the server. There are many of them (each time a window is created, moved, masked, unmasked, and many other things), so a client must tell the server the events it is interested in. With this XSelectInput() stuff, we tell the server we want to be informed of "structural" changes occuring on the w window. Creation and mapping are such changes. There is no way to be informed for example of only mapping modification, and not creations, so we've to take everything. In this particular application we're interesting in "mapping" events (grosso modo, the window appears on the screen).


      // "Map" the window (that is, make it appear on the screen)

      XMapWindow(dpy, w);

// And (once again) this is a client-server system. The map request is asynchronous, meaning that the time this instruction is executed doesn't tell us when the window is actually mapped. To be sure, we've to wait for the server to send us a MapNotify event (this is why we want to be sensitive to such events).


      // Create a "Graphics Context"

      GC gc = XCreateGC(dpy, w, 0, NIL);

// Yet another magic stuff. But mastering them is the reason of the existence of this tutorial...

// For several reasons, the graphical model of X is stateless, meaning that the server doesn't remember (among other things) attributes such as the drawing color, the thickness of the lines and so on. Thus, we've to give all these parameters to the server on each drawing request. To avoid passing two dozens of parameters, many of them unchanged from one request to the next, X uses an object called the Graphics Context, or GC for short. We store in the graphics context all the needed parameters. Here, we want the color used to draw lines, called the foregound color:


      // Tell the GC we draw using the white color

      XSetForeground(dpy, gc, whiteColor);

// There are many other parameters used to draw a line, but all of them have reasonable default values.

// That's okay for now. Everything is set up, and we wait for the window mapping.


      // Wait for the MapNotify event

      for(;;) {
	    XEvent e;
	    XNextEvent(dpy, &e);
	    if (e.type == MapNotify)
		  break;
      }

// We loop, taking events as they come and discarding them. When we get a MapNotify, we exit the loop. We may get events other than MapNotify for two reasons:

    // We have selected StructureNotifyMask to get MapNotify events, but we could get other events as well (such as ConfigureNotify, telling the window has changed in position, and/or size).
    // Some events can be received, even if we don't have asked for them, they are called "non-maskable". GraphicsExpose is such an event. 

// The non-maskable events are sent only in response to some program requests (such as copying an area), so they aren't likely to happen in our context.

// The XNextEvent() procedure is blocking, so if there are no event to read, the program just wait inside the XNextEvent().

// When we have exited the loop, we have good confidence that the window appears on the screen. Actually, this may not be the case since, for example, the user may have iconified it using the window manager, but for now, we assume the window actually appears. We can draw our line:


      // Draw the line
      
      XDrawLine(dpy, w, gc, 10, 60, 180, 20);

// The line is between points (10, 60) and (180, 20). The (0,0) is at the upper left corner of the window, as usual. If the program just sleeps here, nothing will happen, because, in case you don't know, X has a client-server architecture. Thus the request stays in the client, unless we tell it to go to the server. This is done by XFlush():


      // Send the "DrawLine" request to the server

      XFlush(dpy);

// Clever readers may have noticed that we didn't use XFlush() before, and it didn't prevent all the requests such as XMapWindow() to be sent to the server. The answer is that XNextEvent() performs an implicit XFlush() before trying to read some events. We have our line now, we just wait for 10 seconds, so we can make people see how beautiful is our work:


      // Wait for 10 seconds

      sleep(10);

// That's all for now. In next lesson, we will have a (very) little more interaction. [to be continued]
// Christophe Tronche, ch@tronche.com

}
