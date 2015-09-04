### http://preshing.com/20110926/high-resolution-mandelbrot-in-obfuscated-python/
### and
### http://preshing.com/20110822/penrose-tiling-in-obfuscated-python/

### run in 5'7" on the 2011 I5 mobile laptop

### BMP headers:
### typedef struct tagBITMAPFILEHEADER // GB size - 3x2 + 2x4 = 14 bytes, some compilers pad to 16 bytes 
### {
###   WORD    bfType;        // must be 'BM' // a 16-bit field, makes trouble for structure alignment
###   DWORD   bfSize;        // size of the whole .bmp file
###   WORD    bfReserved1;   // must be 0
###   WORD    bfReserved2;   // must be 0
###   DWORD   bfOffBits;     //                               (56 = 16 + 40 = 0x36)
### } BITMAPFILEHEADER; 
###
### typedef struct tagBITMAPINFOHEADER // GB size = 9x4 + 2x2 = 40 bytes 
### {
###    DWORD  biSize;            // size of the structure     (40 bytes)
###    LONG   biWidth;           // image width
###    LONG   biHeight;          // image height
###    WORD   biPlanes;          // bitplanes
###    WORD   biBitCount;        // resolution 
###    DWORD  biCompression;     // compression
###    DWORD  biSizeImage;       // size of the image
###    LONG   biXPelsPerMeter;   // pixels per meter X
###    LONG   biYPelsPerMeter;   // pixels per meter Y
###    DWORD  biClrUsed;         // colors used
###    DWORD  biClrImportant;    // important colors
### } BITMAPINFOHEADER;

### NOTES
### _abc was _
### W was v
### H was x
### fh was j (file handle)

_abc = ( 255, lambda V,B,c :c and Y(V*V+B,B, c -1) 

if(abs(V)<6) else ( 2+c-4*abs(V)**-0.4)/i) 

### v,x=1500,1000
W,H=1500,1000

C=range(W*H);

import  struct

P=struct.pack

M ='<QIIHHHH'

fh =open('Maa2.py.bmp','wb').write

### understand 'BM' = bfType
### what is W*H*3+26?
### what is 26?
### what is 12?
### understand W,H,1,24
### don't see biCompression biSizeImage biXPelsPerMeter biYPelsPerMeter biClrUsed biClrImportant
for X in fh('BM'+P(M,W*H*3+26,26,12,W,H,1,24)) or C: i ,Y=_abc; fh(

### looks like lambda calculates BGR pixel values
###    B = T*80+T**9 *i-950*T**99
###    G = T*70-880*T**18+701*T**9
###    R = T*i**(1-T**45*2)
###  what are the T**9, T**99, T**18, T**45, T*i**(1-T**45*2) all about?
P('BBB',*(lambda T:( T*80+T**9 *i-950*T**99, T*70-880*T**18+701*T**9, T*i**(1-T**45*2) )) 

( sum( [ Y(0, (A%3/3.+X%W+(X/W+ A/3/3.-H/2)/1j)*2.5 /H -2.7, i) ** 2 for A in C [:9]] ) / 9)

) 

)
