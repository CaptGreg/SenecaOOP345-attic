// http://www.andrewewhite.net/wordpress/2008/09/02/very-simple-jpeg-writer-in-c-c/
// GB link with -lX11

// GB add headers
// GB still need XInfo_t +  getXInfo:
// XInfo_t      xinfo  = getXInfo(":0");


// Very Simple JPEG writer in C/C++

// Update: If you want to see another example or would like to know how to write JPEG data straight to a buffer, see this new post.

// This little tutorial is for all of those who have searched for how to write jpeg files from C/C++ and have been sadly disapointed by the results. I asume the reader already has some image data in memory that is intended to be compressed into a jpeg file. This article is based on libjpeg by IJG which appears to be the most complete and open jpeg library out there. The primary reference for this tutorial is the libjpeg doc page at  http://www.jpegcameras.com/libjpeg/libjpeg.html.

// Let’s drive right into the code…

// First, do some setup and take a screen shot using Xlib. Note that Xlib this has next to nothing to do with the JPEG code and is only included to create a context. In the end, the info and screen_shot object will have some meta-data about the image that we will
// be making into a JPEG. The buffer vector will hold raw RGB values for screen shot.

#include <cstdio>      // GB fopen, fwrite, fclose
#include <vector>      // GB
#include <string>      // GB
using namespace std;   // GB
#include <X11/Xlib.h>  // GB
#include <X11/Xatom.h> // GB
#include <X11/Xutil.h> // GB
#include <jpeglib.h>


#if 0
// from http://www.sourcecodebrowser.com/xscreensaver/4.23/xflame_8c.html#a8e89e098981ce162cdfe83064b10201a
static void GetXInfo 	( 	Display *  	disp, Window  	win )
{
  XWindowAttributes xwa;
   
  XGetWindowAttributes(disp,win,&xwa);

  window   = win;
  display  = disp;
  colormap = xwa.colormap;
  depth    = xwa.depth;
  visual   = xwa.visual;
  screen   = xwa.screen;
  width    = xwa.width;
  height   = xwa.height;

  if (width%2)
    width++;
  if (height%2)
    height++;
}
#endif

int main()
{
  vector<char> buffer;
  XInfo_t      xinfo  = getXInfo(":0");
  XImage* screen_shot = takeScreenshot(xinfo, buffer);

// Now we need to open an output file for the jpeg.

  FILE* outfile = fopen("/tmp/test.jpeg", "wb");
 
  if (!outfile)
    // throw FormattedException("%s:%d Failed to open output file", __FILE__, __LINE__);
    throw string(__FILE__ ":") + to_string(__LINE__) + string(" Failed to open output file"); // GB
// Now, lets setup the libjpeg objects. Note this is setup for my screen shot so we are using RGB color space.

  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr       jerr;
 
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);
  jpeg_stdio_dest(&cinfo, outfile);
 
  cinfo.image_width      = xinfo.width;
  cinfo.image_height     = xinfo.height;
  cinfo.input_components = 3;
  cinfo.in_color_space   = JCS_RGB;

// Call the setup defualts helper function to give us a starting point. Note, don’t call any of the helper functions before you call this, they will no effect if you do. Then call other helper functions, here I call the set quality. Then start the compression.

  jpeg_set_defaults(&cinfo);
/*set the quality [0..100]  */
  jpeg_set_quality (&cinfo, 75, true);
  jpeg_start_compress(&cinfo, true);

// Next we setup a pointer and start looping though lines in the image. Notice that the row_pointer is set to the first byte of the row via some pointer math/magic. Once the pointer is calculated, do a write_scanline.

  JSAMPROW row_pointer;          /* pointer to a single row */
 
  while (cinfo.next_scanline < cinfo.image_height) {
    row_pointer = (JSAMPROW) &buffer[cinfo.next_scanline*(screen_shot->depth>>3)*screen_shot->width];
    jpeg_write_scanlines(&cinfo, &row_pointer, 1);
  }

// Finally, call finish and in my case close the connection to the X server.

  jpeg_finish_compress(&cinfo);
  XCloseDisplay(xinfo.display);
}

// Compiling and linking the code is straight forward. As with any library, get the headers and the shared library and setup the compiler and you are good to go. For reading JPEG files via C/C++ I would recommend using SDL’s Image library. You can use libjpeg but the interface will be more complex than needed for most applications.
