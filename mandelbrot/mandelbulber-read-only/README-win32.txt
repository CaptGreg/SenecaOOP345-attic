MANDELBULBER 1.21-1

project leader: Krzysztof Marczak
project members: knighty, makemeunsee, Marius Schilder, Ryan Hitchman, Jeff Epler, Martin Reinecke, Quazgaa

contact: buddhi1980@gmail.com
www: http://www.mandelbulber.com
download site: http://sourceforge.net/projects/mandelbulber/
SVN: http://code.google.com/p/mandelbulber/


Licence: GNU GPL v3.0


1. REQUIREMENTS

- very fast CPU (recomended 2GHz, 4 cores)
- 2GB RAM

2. INSTALATION

Please run:
Mandelbulber-Setup.exe

3. TESTED ON:

- Windows XP SP3
- Windows Vista x64 SP2
(there is also available Linux and Mac version)

5. USAGE

Please visit: http://wiki.mandelbulber.com/index.php?title=Main_Page

command-line mode:

Syntax:                                                                                              
mandelbulber [options...] [settings_file]
options:
  -nogui            - start program without GUI
  -flight           - render flight animation
  -keyframe         - render keyframe animation
  -start N          - start renderig from frame number N
  -end N            - rendering will end on frame number N
  -o key=value      - override item 'key' from settings file with new value 'value'
  -res WIDTHxHEIGHT - override image resolution
  -fpk N            - override frames per key parameter
  -ip N.N.N.N       - set application as a client connected to server of given IP address
  -port      				- set network port number for Netrender (default 5555)
  -format FORMAT    - image output format
     jpg - JPEG format
     png - PNG format
     png16 - 16-bit PNG format
     png16alpha - 16-bit PNG with alpha channel format
[settings_file]     - file with fractal settings (program also tries
to find file in ./mandelbulber/settings directory)
When settings_file is put as command argument then program will start in noGUI mode
