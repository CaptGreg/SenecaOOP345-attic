
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv/highgui.h> // GB this include is needed with opencv 3.0 (for CV_CAP_PROP_XXX)
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdio>
#include <cstdlib>

using namespace cv;
using namespace std;

Mat src; Mat src_gray;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);

/// Function header
void thresh_callback(int, void* );

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


  /// Convert image to gray and blur it
  cvtColor( src, src_gray, CV_BGR2GRAY );
  blur( src_gray, src_gray, Size(3,3) );

  /// Create Window
  const char* source_window = "Source";
  namedWindow( source_window, CV_WINDOW_AUTOSIZE );
  imshow( source_window, src );

  createTrackbar( " Canny thresh:", "Source", &thresh, max_thresh, thresh_callback );
  thresh_callback( 0, 0 );

  if(waitKey(1) == 27) break;
  }
  return(0);
}

/** @function thresh_callback */
void thresh_callback(int, void* )
{
  Mat canny_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  /// Detect edges using canny
  Canny( src_gray, canny_output, thresh, thresh*2, 3 );
  /// Find contours
  findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  /// Get the moments
  vector<Moments> mu(contours.size() );
  for( int i = 0; i < contours.size(); i++ )
     { mu[i] = moments( contours[i], false ); }

  ///  Get the mass centers:
  vector<Point2f> mc( contours.size() );
  for( int i = 0; i < contours.size(); i++ )
     { mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); }

  /// Draw contours
  Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
  for( int i = 0; i< contours.size(); i++ )
     {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
       circle( drawing, mc[i], 4, color, -1, 8, 0 );
     }

  /// Show in a window
  namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
  imshow( "Contours", drawing );

  /// Calculate the area with the moments 00 and compare with the result of the OpenCV function
  cout << "\t Info: Area and Contour Length \n";
  for( int i = 0; i< contours.size(); i++ )
     {
       #if 1
       printf(" * Contour[%d] - Area (M_00) = %.2f - Area OpenCV: %.2f - Length: %.2f \n", 
       i, mu[i].m00, contourArea(contours[i]), arcLength( contours[i], true ) );
       #else
       stringstream ss;
       ss   << " * Contour[" << i << "]"
               " - Area (M_00) = " << setprecision(2) << mu[i].m00 
            << " - Area OpenCV: "  << setprecision(2) << contourArea(contours[i])
            << " - Length: "       << setprecision(2) << arcLength( contours[i], true ) 
            << "\n";
       cout << ss.str();
       #endif


       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
       circle( drawing, mc[i], 4, color, -1, 8, 0 );
     }
}
