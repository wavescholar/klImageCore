Doxygen Code Documentation :  http://wavescholar.github.io/klImageCore/inherits.html

This repository contains a high performance image processing framework. Intel IPP libraries are leveraged where possible. There are a number of image codecs including BigTiff.  Ues the solution in the wavescholar SolutionFiles repository to build the libraries. Project files are provided for Visual Studio 2010 with Intel Compiler XE2011 for C++ and Fortran libraries. 

The best way to use build everything is to clone the following repositories to a folder that is mapped to d:  Libraries are included in the Visual Studio projects as files rather than lib includes,  therefore the paths need to match up.  Relative paths won't work for this setup.  

Some projects are not in the main solution; libjpeg-turbo for example.  This is a SIMD enabled libjpeg.  There is a working x32 project. 
  

  
