Tetris README
=============
      
How to build
------------

* Windows (MinGw)  
  a) download and install MinGw  
     http://www.mingw.org/  
  b) run  
     make -f Makefile.w32  
     
* UNIX / Linux (Console)  
  a) run  
     make -f Makefile.con  

* DOS (Turbo C 2.01)  
  a) download and install Turbo C:  
     http://edn.embarcadero.com/article/20841  
  b) download and extract Tetris sources into tetris directory  
  c) copy *.BGI drivers from Turbo C directory to tetris sources directory  
  d) run Turbo C  
  e) go to Project->Project name and choose TETRIS.PRJ or PTAKOVIN.PRJ  
  f) go to Compile->Make EXE file  

* ZX Spectrum (Z88DK)  
  a) download and install Z88DK  
     http://www.z88dk.org  
  b) run  
     make -f Makefile.zx  
