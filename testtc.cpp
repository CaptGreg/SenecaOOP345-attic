#include <iostream>
#include <sstream>
#include <iomanip>       // setw
#include <cmath>         // fmod = remainder (%) function for floating point


using namespace std;

int main(int argc, char**argv)
{
	double fps = 29.97;

  ostringstream textTimeCode;
	for(int frameCount = 0; frameCount < 1.5*fps ; frameCount++) {

    // figure out time code
    int iFrame  =  int( fmod(frameCount, fps) );
    int iSecond =  double(frameCount) / fps;
    int iMinute =  iSecond / 60;
    int iHour   =  iMinute / 60;
    iSecond     %= 60;
    iMinute     %= 60;
    textTimeCode.seekp(0);
    textTimeCode << frameCount << " ";
    textTimeCode << setw(3)  << setfill('0') << iHour << ":" 
                 << setw(2)  << setfill('0') << iMinute << ":" 
                 << setw(2)  << setfill('0') << iSecond  << ":" 
                 << setw(2)  << setfill('0') << iFrame;

    cout << textTimeCode.str() << "\n";
	}
}


