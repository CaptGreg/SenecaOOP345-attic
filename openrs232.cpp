// serial port includes
#include <fcntl.h>       // File Control   Definitions  O_RDWR, O_NOCTTY, and O_NDELAY
#include <termios.h>     // POSIX Terminal Definitions  tcgetattr, tcsetattr, ...
#include <unistd.h>      // UNIX Standard  Definitions  read, write, close
#include <sys/ioctl.h>   // ioctl          Definitions  

// Open serial interface
// Example Usage: int fd = OpenRS232( "/dev/ttyUSB0" );
// Example Usage: int fd = OpenRS232( "/dev/ttyACM0" );
// close with close(fd);
int OpenRS232( const char* port )
{
    int fd  = open(port, O_RDWR | O_NOCTTY| O_NDELAY); 
      // O_RDWR   - Read/Write access to serial port
      // O_NOCTTY - No terminal will control the process
      // Open in blocking mode,read will wait
      // O_NDELAY -Non Blocking Mode, do not care about the status of DCD line, open returns immediately

    if(fd == -1) {
      perror( port );
		  cerr << "\n";
      return fd;
    }

    struct termios tio;
    if(tcgetattr(fd, &tio) == -1) {
      close(fd);
      fd = -1;
      // perror(port); // "Inappropriate ioctl for device"
      cerr << "'" << port << "' is not a serial port\n";
      return fd;
    }

    cfmakeraw(&tio);
    cfsetspeed(&tio, B9600);
    
		/* 8N1 Mode */
		tio.c_cflag &= ~PARENB;                          // No Parity
		tio.c_cflag &= ~CSTOPB;                          // CSTOPB cleared so 1 Stop bit
		tio.c_cflag &= ~CSIZE;	                         // Clears the mask for setting the data size
		tio.c_cflag |=  CS8;                             // Set the data bits = 8
		
		tio.c_cflag &= ~CRTSCTS;                         // No Hardware flow Control
		tio.c_cflag |= CREAD | CLOCAL;                   // Enable receiver,Ignore Modem Control lines
		
		tio.c_iflag &= ~(IXON | IXOFF | IXANY);          // Disable XON/XOFF flow control
		tio.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  // RAW mode

		tio.c_oflag &= ~OPOST;/*No Output Processing*/
		
		if(tcsetattr(fd,TCSANOW,&tio)) { // Set the attributes to the termios structure
		  cerr << "\n  ERROR ! Setting serial port attributes ";
      perror( port );
		  cerr << "\n";
      return fd;
    }
    cout << "\nopened '" << port << "' BaudRate=9600, StopBits=1, Parity=none\n";

    return fd;
}
