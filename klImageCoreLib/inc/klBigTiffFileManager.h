/*******************************
 * Copyright (c) <2007>, <Bruce Campbell> All rights reserved. Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  *  
 * Bruce B Campbell 11 30 2012  *
 ********************************/

#ifndef __BigTiffReader__
#define __BigTiffReader__

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>

//using namespace std;
//Generic numeric conversion.  The second param, should be std::hex, std::dec or std::oct.
template <class T> std::string num_to_string(T t, std::ios_base & (*f)(std::ios_base&))
{
  std::ostringstream oss;
  oss << f << t;
  return oss.str();
}

#include <stdio.h>
#include <malloc.h>
#include <math.h>


#include "tiffio.h"
#include"tiffiop.h"
typedef unsigned long  DWORD;


#include "kl_stack_walker.h"

//This function is a custom tiff error handler defined in SVSReader.cpp.
//It throws an exception of type klBigTiffError
void TIFFError(const char* module, const char* fmt, va_list argptr);


//This function is a custom tiff error handler defined in SVSReader.cpp.
//It throws an exception of type klBigTiffError
void TIFFErrorExtFn(thandle_t fd, const char* module, const char* fmt, va_list argptr);

//Exception type for tiff errors
class klBigTiffError : public  std::exception //klStackTraceException
{
public:
	klBigTiffError(std::string msg) : exception(msg.c_str() )//exception( (msg+ klStackWalkFn()).c_str() ) 
	{
	
	}

};


//SVS is one of klBigTiff’s proprietary image formats for scanned image data.  
//It is a modified tiff format allowing for BigTIff extensions which support more than 4GB of image data.  
//Tiff Directory Structure of SVS File :
//	Each SVS file has multiple images. 
//	Dir 1: Main Image compressed according to one of the compression formats listed below.
//	Dir2: An Image thumbnail typically 1024x768
//	Dirs3-5  [Optional] A multi-resolution pyramid compressed with the same format as the main image
//	Dir 6 – 7:  [Optional] LZW compressed Label Image and or JPEG compressed Macro Whole Slide Image  

//klBigTiff Compression / associated file extension:
//	0=None			[ .tif ]
//	1=LZW			[ .tif ]
//	2=SVS w/JPEG	[ .svs ]
//	3=SVS w/J2K		[ .svs ]
//	7=YUYV			[ .tif ]
//	8=JPEG			[ .tif ]  
//For the first version of klBigTiff file ingestion, we will be using BigTiff enabled libtiff [ver 4.0.0.beta4] from remotesensing.org.  
//Initially, Kakadu 6.1 will provide Jpeg 2000 decompression , Jpeg 6.b will provide standard decompression,  and Libz 1.2.3 will provide LZW decompression.

//klBigTiffFileManager is the main class for controlling all functionality inSVSCompositeLib.
//It manages the extraction of image data and metadata for klBigTiff image files.
//The name is somewhat of a misnomer, since some klBigTiff files are saved with a tiff extension


//TODO:  
//IPP/MKL libs may be incorporated in SVSComposite Lib to provide alternate decompression.
//#include "ipp.h"

//This embedded class represents data obtained from a tiff directory.
class TiffImageInfo {

public:

	TiffImageInfo() 	
	{
		tiffDirs=0, height=0, width=0, bitspersample=0, samplesperpixel=0, rowsperstrip=0, photometric=0, compression=0, 
		orientation=0, res_unit=0, x=0, y=0, xresolution=0, xoffset=0, yresolution=0,  yoffset=0, isRGB=0, isTiled=0, 
		tileWidth=0, tileHeight=0, isBigTiff=0, tiffTileMemSz=0, numberOfStrips=0, stripByteCounts=0, isInterleaved=0, 
		isSubsample=0, ImageDescription="", readEntireImage=0;
	}

	//Number of images in tiff file
	uint32 tiffDirs;

	//The dir currently represented by this object
	uint16 currentDir;

	uint32 height;

	uint32 width;

	uint16 bitspersample;

	uint16 samplesperpixel;

	uint32 rowsperstrip;

	uint16 photometric;

	uint16 compression;

	uint16 orientation; 

	uint16 res_unit; 

	uint32  x;

	uint32  y;

	float xresolution;

	float xoffset;

	float yresolution;

	float yoffset;

	//This is  deduced from photometric
	bool isRGB;

	bool isTiled;

	uint32 tileWidth;

	uint32 tileHeight;

	bool isBigTiff;

	//# bytes in a variable length, row-aligned tile.
	tsize_t tiffTileMemSz;

	tstrip_t numberOfStrips;

	//This memory is cleaned up when the tiff file is closed/ or when the directory is changed.  -bbcrevisit 
	uint32* stripByteCounts;
	
	uint64* longOffsets;

	//This indicates if the image is band interleaved or planar
	bool isInterleaved;  

	bool isSubsample;

	//This is the string from which klBigTiff specific metadata is parsed.
	std::string ImageDescription;

	//bbcrevisit For testing 
	bool readEntireImage;

	//Conversion operator to assist in metadata 
	operator std::map<std::string, std::string> ();
}; 


//This class represents access to an svs file.
//It provides the image and klBigTiff metadata.
class klBigTiffFileManager
{
public:
	//One instance per SVS file.  No resources are created in the constructor save the 
	//handle to the tiff file.
	klBigTiffFileManager(const char* fileName);

	//Closes tiff file.
	~klBigTiffFileManager()
	{
		if(tifFile)
		{
			TIFFClose(tifFile);
		}
		if(logFile)
		{
			fclose(logFile);
		}
	}

	//Get the number of images in this tiff file.
	unsigned int getNumberOfTiffDirs();

	//Get the Whole Slide Image if there is one. 
	//Returns false if there is none, throws an error if a problem is encountered writing the WSI image. 
	bool getWSIImage(const char* ofileName);	

	//Get the label image if there is one.  
	//Returns false if there is none, throws an error if a problem is encountered writing the label image. 
	bool getLabelImage(const char* ofileName);

	//Get the thumbnail image if there is one. 
	//Returns false if there is none, throws an error if a problem is encountered writing the thumbnail image. 
	bool getThumbnail(const char* ofileName);

	//Get the tile image at a specified directory.  
	bool klBigTiffFileManager::getTiffDirImage(unsigned int dir, const char* ofileName);

	//Get the strip image at a specified directory. 
	bool klBigTiffFileManager::getStripSubDir(unsigned int dir, const char* ofileName);

	//Returns Image and klBigTiff Metadata in a map format for the specified tiff directory.  
	//Throws an error if there is a problem.  The consumer of this is intended to be a managed assembly which puts the metadata in an xml format.
	std::map<std::string,std::string> getMetadata(unsigned int i);

	//Get's a ROI from the first tiff image.
	void ExtractMainResTile(const char * tiffoutputFile, const char * ppmoutputFile,unsigned int x,unsigned int y,unsigned int w,unsigned int h);
	
	//Get an ROI use the embedded ICC profile.
	void ExtractMainResTileApplyICC(const char * outputFile,unsigned int x,unsigned int y,unsigned int w,unsigned int h);
	
	void klBigTiffFileManager::ExtractImageTiles(const char * outputFileBase);

	//Extracts an image buffer from the micro image.  The caller must delete the buffer.
	unsigned char* getImageBuffer(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int& tw,unsigned int& th); 

	//Print the metadata for all directories to std::cout
	void printMetadata();
	
protected:

	//Gets the TiffImageInfo for the specified image.
	//This also sets the tifFile tiff handle memeber to the appropriate subdir.
	//This is called as a setup and data extraction step prior to reading metadata & image data.
	TiffImageInfo getTiffImageInfo(unsigned int i);	

	//For test purposes - not used in production 
	void writeTiffFile(char* filename,  void* raster, TiffImageInfo info);	

	std::vector<std::string> parseImageDescriptionTag(std::string tag);

	//This fn merges metadata obtained from klBigTiff Image Information tag and the other tiff tags.
	//The elements parameter contains klBigTiff data, info contians tiff metadata.
	std::map<std::string,std::string> klBigTiffFileManager::createXMLMap(std::vector<std::string> elements, TiffImageInfo info);

	//Writes the ImageMetadata to file.  Primarily for debugging purposes.
	void printTiffInfo(FILE* fname, TiffImageInfo info);

	//This is the handle to the tiff image
	TIFF * tifFile;	

	FILE* logFile;
	
private:
	//This method should only be called by getTiffImageInfo
	//Sets the tiff directory to the specified image.  
	//Throws an error if directory is out of range.
	void setTiffDirectory(unsigned int i);

};

#endif //__BigTiffReader__ conditional include