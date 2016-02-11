// compile on UNIX, Linux, cygwin, iOS, Android, Raspberry Pi with
// g++ `pkg-config --cflags opencv` camera.cpp `pkg-config --libs opencv` -o camera

// OpenCV reference:
// http://docs.opencv.org/modules/highgui/doc/reading_and_writing_images_and_video.html
// http://docs.opencv.org/modules/core/doc/drawing_functions.html#puttext

#include <iostream>
#include <sstream>       // time code to string formating

#include <opencv/cv.hpp>
#include <opencv/highgui.h> // GB this include is needed with opencv 3.0 (for CV_CAP_PROP_XXX)

int process(int argc, char**argv)
{
  if(argc < 2) throw std::string("Usage:") + argv[0] + " EITHER_camera-number_OR_input-video-file-name [video-out-file-name]";

  cv::VideoCapture cap(argv[1]);                // try to open as a video file or image sequence.
  if (!cap.isOpened()) cap.open(atoi(argv[1])); // try to open as a video camera.
  if (!cap.isOpened())
    throw "Failed to open the video device, video file or image sequence!\n";

  int    w   = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  int    h   = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
  double fps = cap.get(CV_CAP_PROP_FPS);
  std::cout << "Frame size : " << w << " x " << h << "\n";

  cv::VideoWriter writer;
  if(argc > 2) // NOTE: XVID is MPEG-4
    writer.open(argv[2], cv::VideoWriter::fourcc('X','V','I','D'), fps==-1.? 24.: fps, cv::Size(w,h));


  const char* title = "video";
  cv::namedWindow(title, CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"

  // cv::putText parameters
  std::ostringstream      text;
  cv::Point               org(20,20);
  int fontFace          = cv::FONT_HERSHEY_PLAIN;  // choose one of:
    // FONT_HERSHEY_SIMPLEX        FONT_HERSHEY_PLAIN          FONT_HERSHEY_DUPLEX 
    // FONT_HERSHEY_COMPLEX        FONT_HERSHEY_TRIPLEX        FONT_HERSHEY_COMPLEX_SMALL
    // FONT_HERSHEY_SCRIPT_SIMPLEX FONT_HERSHEY_SCRIPT_COMPLEX
  double fontScale      = 1.; 
  cv::Scalar              color(0,255,0);   // green
  int thickness         = 1; 
  int lineType          = 8; 
  bool bottomLeftOrigin = false;  // up-side-down text if true

  std::cout << "Press Esc key to quit\n";
  cv::Mat frame;
  for(int frameCount = 0; ; frameCount++) {
    cap >> frame;
    if (frame.empty()) break;  // EOF if reading video file

    // add some text to the image
    text.seekp(0);  // - Operator '<<' will append to 'text' if we don't do this!
    text << "frame: " << frameCount;
    cv::putText(frame, text.str(), org, fontFace, fontScale, color, thickness, lineType, bottomLeftOrigin);

    if(writer.isOpened()) 
      writer << frame;

    cv::imshow(title, frame); // show frame
    if (cv::waitKey(1) == 27) // wait up to 1 ms for 'Esc' key, ascii decimal 27, press.
      break;
  }
  return 0;
}

int main(int argc, char**argv)
{
  try {

    return process(argc,argv);

  } catch(const std::string& e) { 
    std::cerr << e << "\n"; 
  } catch(const char*e) { 
    std::cerr << e << "\n"; 
  } catch(const cv::Exception& e) { 
    std::cerr << "cv::Exception " << e.what() << "\n"; 
  }
  return 1;
}
