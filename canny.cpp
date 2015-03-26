#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv/highgui.h> // GB this include is needed with opencv 3.0 (for CV_CAP_PROP_XXX)
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

/// Global variables

Mat src, src_gray;
Mat dst, detected_edges;

int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
char* window_name = "Edge Map";

/**
 * @function CannyThreshold
 * @brief Trackbar callback - Canny thresholds input with a ratio 1:3
 */
void CannyThreshold(int, void*)
{
  /// Reduce noise with a kernel 3x3
  blur( src_gray, detected_edges, Size(3,3) );

  /// Canny detector
  Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );

  /// Using Canny's output as a mask, we display our result
  dst = Scalar::all(0);

  src.copyTo( dst, detected_edges);
  imshow( window_name, dst );
 }


/** @function main */
int main( int argc, char** argv )
{
  if(argc < 2) throw std::string("Usage:") + argv[0] + " EITHER_camera-number_OR_input-video-file-name [video-out-file-name]";

  cv::VideoCapture cap(argv[1]);                // try to open as a video file or image sequence.
  if (!cap.isOpened()) cap.open(atoi(argv[1])); // try to open as a video camera.
  if (!cap.isOpened())
    throw "Failed to open the video device, video file or image sequence!\n";

  while(1) {
    /// Load an image
    cap >> src;
  
    if( !src.data )
    { return -1; }
  
    /// Create a matrix of the same type and size as src (for dst)
    dst.create( src.size(), src.type() );
  
    /// Convert the image to grayscale
    cvtColor( src, src_gray, CV_BGR2GRAY );
  
    /// Create a window
    namedWindow( window_name, CV_WINDOW_AUTOSIZE );
  
    /// Create a Trackbar for user to enter threshold
    createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );
  
    /// Show the image
    CannyThreshold(0, 0);
  
    /// Wait until user exit program by pressing a key
    if(waitKey(1) == 27) break;
}

  return 0;
}
