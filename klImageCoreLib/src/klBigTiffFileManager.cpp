
/*******************************
 * Copyright (c) <2007>, <Bruce Campbell> All rights reserved. Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  *  
 * Bruce B Campbell 11 30 2012  *
 ********************************/


void klRunImageWorkFlowImage(const char* basefilename);

#include "klBigTiffFileManager.h"

#include "kl_ppm_image_io.h"


#include "kl_image_processing_functors.h"

//Kakadu includes
//#include "kdu_helpers.h"
#include "time.h"

//ICC Headers
#include "IccCmm.h"
#include "IccUtil.h"
#include "tiffio.h"                  // need tiff library
#define PHOTO_MINISBLACK  0
#define PHOTO_MINISWHITE  1
#define PHOTO_CIELAB      2
//ICC profile funcitons

#include "psapi.h"
#include "Strsafe.h"

void PrintGetLastError(FILE* logFile,LPTSTR lpszFunction) 
{ 
    // Retrieve the system error message for the last-error code
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
        (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR)); 
    StringCchPrintf((LPTSTR)lpDisplayBuf, 
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("GetLastError Returned  code %d: \n"),  dw); 
   
	fprintf(logFile, "\n\tBegin Formatted Error Message :  \n ");

	fwrite(lpMsgBuf,1,LocalSize(lpMsgBuf),logFile);

	fprintf(logFile, "\n\tEnd Formatted Message \n ");

	LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
}

void PrintModules(FILE* logFile )
{

	DWORD processID;
	HMODULE hMods[1024];
	HANDLE hProcess;
	DWORD cbNeeded;
	unsigned int i;

	hProcess=GetCurrentProcess ();
	fprintf(logFile, "\nProcess ID: %u\n", hProcess );
	
	if (NULL == hProcess)
	{

		return;
	}

	if( EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
	{
		for ( i = 0; i < (cbNeeded / sizeof(HMODULE)); i++ )
		{
			LPTSTR szModName=NULL;

			szModName=new TCHAR[1024];

			DWORD nSize = 1024 ;
			LPTSTR lpFilename;
			
			if ( GetModuleFileNameEx( hProcess, hMods[i], szModName,nSize))
			{
				// Print the module name and handle value.
				fprintf(logFile,"(0x%08X)\n", szModName, hMods[i] );
				std::wstring name  = szModName;
				fprintf(logFile,"%ls",name.c_str());
			}
			delete szModName;
		}
	}
	CloseHandle( hProcess );
	fprintf(logFile,"\n");
}

void logError(FILE* logFile, LPTSTR lpszFunction, const char* what)
{
	time_t time_of_day;
	struct tm *tm_buf;
	time_of_day = time( NULL );
	tm_buf=localtime(&time_of_day);

	fprintf(logFile, "\n\t\tHandling Error in image file reading\n");

	fprintf (logFile,"TimeStamp : %s",asctime(tm_buf));

	fprintf(logFile, " \n\t\tCalling PrintGetLastError \n");

	PrintGetLastError( logFile, lpszFunction);

	fprintf(logFile, "\n\t\tException info : \n");

	fprintf(logFile,"%s \n",what);

	fprintf(logFile, "\n\t\tPrinting Loaded Modules \n");

	PrintModules(logFile);

	fprintf(logFile, "\n\t\tEnd Error Info\n");

}

void logInfo(FILE* logFile, LPTSTR lpszFunction, const char* what)
{
	time_t time_of_day;
	struct tm *tm_buf;
	time_of_day = time( NULL );
	tm_buf=localtime(&time_of_day);

	fprintf(logFile, "\n\t\t----------------BigTiff Reader Info-----------------\n");

	fprintf (logFile,"TimeStamp : %s",asctime(tm_buf));

	fprintf(logFile, " \n\t\tCalling PrintGetLastError \n");

	PrintGetLastError( logFile, lpszFunction);

	fprintf(logFile, "\n\t\tException info : \n");

	fprintf(logFile,"%s \n",what);

	fprintf(logFile, "\n\t\tPrinting Loaded Modules \n");

	PrintModules(logFile);

	fprintf(logFile, "\n\t\t----------------End Info----------------------------\n");

}

static icFloatNumber UnitClip(icFloatNumber v)
{
	if (v<0.0)
		return 0.0;
	if (v>1.0)
		return 1.0;
	return v;
}
extern "C" int dumpICC(char* filename );

extern "C" bool ApplyICC_FileBased( const char *szSrcImage, char *szSrcProfile,  char *szDstProfile, char *szDstImage, int nIntent);

TiffImageInfo::operator std::map<std::string, std::string> () 
{
	std::map<std::string, std::string> mdMap;
	mdMap.insert(std::map<std::string ,std::string>::value_type("tiffDirs",num_to_string<unsigned int>(tiffDirs,std::dec) ) );
	mdMap.insert(std::map<std::string ,std::string>::value_type("currentDir",num_to_string<unsigned int>(currentDir,std::dec) ) );
	mdMap.insert(std::map<std::string ,std::string>::value_type("height",num_to_string<unsigned int>(height,std::dec) ) );
	mdMap.insert(std::map<std::string ,std::string>::value_type("width",num_to_string<unsigned int>(width,std::dec) ) );
	mdMap.insert(std::map<std::string ,std::string>::value_type("bitspersample",num_to_string<unsigned int>(bitspersample,std::dec) ) );
	mdMap.insert(std::map<std::string ,std::string>::value_type("samplesperpixel",num_to_string<unsigned int>(samplesperpixel,std::dec) ) );
	mdMap.insert(std::map<std::string ,std::string>::value_type("rowsperstrip",num_to_string<unsigned int>(rowsperstrip,std::dec) ) );
	mdMap.insert(std::map<std::string ,std::string>::value_type("photometric",num_to_string<unsigned int>(photometric,std::dec) ) );
	mdMap.insert(std::map<std::string ,std::string>::value_type("compression",num_to_string<unsigned int>(compression,std::dec) ) );
	mdMap.insert(std::map<std::string ,std::string>::value_type("orientation",num_to_string<unsigned int>(orientation,std::dec) ) );
	mdMap.insert(std::map<std::string ,std::string>::value_type("res_unit",num_to_string<unsigned int>(res_unit,std::dec) ) );
	mdMap.insert(std::map<std::string ,std::string>::value_type("x",num_to_string<unsigned int>(x,std::dec) ) );
	mdMap.insert(std::map<std::string ,std::string>::value_type("y",num_to_string<unsigned int>(y,std::dec) ) );
	mdMap.insert(std::map<std::string ,std::string>::value_type("xresolution",num_to_string<unsigned int>(xresolution,std::dec) ) );
	mdMap.insert(std::map<std::string ,std::string>::value_type("xoffset",num_to_string<unsigned int>(xoffset,std::dec) ) );
	mdMap.insert(std::map<std::string ,std::string>::value_type("yresolution",num_to_string<unsigned int>(yresolution,std::dec) ) );
	mdMap.insert(std::map<std::string ,std::string>::value_type("yoffset",num_to_string<unsigned int>(yoffset,std::dec) ) );
	mdMap.insert(std::map<std::string ,std::string>::value_type("isRGB",num_to_string<unsigned int>(isRGB,std::dec) ) );
	mdMap.insert(std::map<std::string ,std::string>::value_type("isTiled",num_to_string<unsigned int>(isTiled,std::dec) ) );
	mdMap.insert(std::map<std::string ,std::string>::value_type("tileWidth",num_to_string<unsigned int>(tileWidth,std::dec) ) );
	mdMap.insert(std::map<std::string ,std::string>::value_type("tileHeight",num_to_string<unsigned int>(tileHeight,std::dec) ) );
	mdMap.insert(std::map<std::string ,std::string>::value_type("isBigTiff",num_to_string<unsigned int>(isBigTiff,std::dec) ) );
	mdMap.insert(std::map<std::string ,std::string>::value_type("tiffTileMemSz",num_to_string<unsigned int>(tiffTileMemSz,std::dec) ) );
	mdMap.insert(std::map<std::string ,std::string>::value_type("numberOfStrips",num_to_string<unsigned int>(numberOfStrips,std::dec) ) );
	mdMap.insert(std::map<std::string ,std::string>::value_type("isInterleaved",num_to_string<unsigned int>(isInterleaved,std::dec) ) );
	mdMap.insert(std::map<std::string ,std::string>::value_type("isSubsample",num_to_string<unsigned int>(isSubsample,std::dec) ) );
	mdMap.insert(std::map<std::string ,std::string>::value_type("ImageDescription",ImageDescription ) );
	mdMap.insert(std::map<std::string ,std::string>::value_type("readEntireImage",num_to_string<unsigned int>(readEntireImage,std::dec) ) );

	return mdMap;

}


klBigTiffFileManager::klBigTiffFileManager(const char* fileName)
{
	tifFile=NULL;

	TIFFSetWarningHandler(0);

	TIFFSetErrorHandler(TIFFError);

	TIFFSetErrorHandlerExt(TIFFErrorExtFn);

	char* logFileName = new char[1024];

	sprintf(logFileName,"klBigTiffReader.log");
	logFile = fopen (logFileName, "a");

	logInfo(logFile,TEXT("\nklBigTiffFileManager\n"), fileName);

	//Open tif file and store handle in member var tifFile
	try
	{
		tifFile=TIFFOpen(fileName, "r");

	}
	catch(klBigTiffError e)
	{
		std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
		ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
		std::string err = ANSI_INFO_ss.str();
		err+="Caught klBigTiffError in klBigTiffFileManager::(char* fileName) : ";
		err+=fileName;
		err+=e.what();
		logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());flushall();throw err;  
	}
	catch(...)
	{
		std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
		ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
		std::string err = ANSI_INFO_ss.str();
		err+="Caught non-klBigTiffError in klBigTiffFileManager::klBigTiffFileManager(char* fileName) : ";
		err+=fileName;
		logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());flushall();throw err; 

	}
	if (!tifFile)
	{
		std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
		ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
		std::string err = ANSI_INFO_ss.str();
		err+="Bad Tiff File Handle n klBigTiffFileManager::klBigTiffFileManager(char* fileName)";
		err+=fileName;
		logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());flushall();throw err; 
	}

	//logError( logFile, TEXT(" \n klBigTiffFileManager ctor \n") ,"What?");

	delete logFileName;
}


void klBigTiffFileManager::printTiffInfo(FILE* fname, TiffImageInfo info)
{
	fprintf(fname,"tiffDir %u :",info.tiffDirs);
	fprintf(fname,"height %u :",info.height);
	fprintf(fname,"width %u :",info.width);
	fprintf(fname,"bitspersample %u : ",info.bitspersample);
	fprintf(fname,"samplesperpixel %u :",info.samplesperpixel);
	fprintf(fname,"rowsperstrip %u : ",info.rowsperstrip);
	fprintf(fname,"photometric %u : ",info.photometric);
	fprintf(fname,"compression %u : ",info.compression);
	fprintf(fname,"orientation %u : ",info.orientation);
	fprintf(fname,"res_unit %u : ",info.res_unit);
	fprintf(fname,"x %u :",info.x); 
	fprintf(fname,"y %u :",info.y);
	fprintf(fname,"xresolution %d : ",info.xresolution);
	fprintf(fname,"xoffset %d : ",info.xoffset);
	fprintf(fname,"yresolution %d : ",info.yresolution);
	fprintf(fname,"yoffset %d : ",info.yoffset);
	fprintf(fname,"isRGB %u : " ,info.isRGB);
	fprintf(fname,"isTiled %u : ",info.isTiled);
	fprintf(fname,"tileWidth %u : ",info.tileWidth);
	fprintf(fname,"tileHeight %u : ",info.tileHeight);
	fprintf(fname,"isBigTiff %u : ",info.isBigTiff);
	fprintf(fname,"tiffTileMemSz %u : ",info.tiffTileMemSz);
	fprintf(fname,"numberOfStrips %u :",info.numberOfStrips);
	fprintf(fname,"isInterleaved %u :",info.isInterleaved);
	fprintf(fname,"isSubsample %u :",info.isSubsample);
	fprintf(fname,"readEntireImage : %u",info.readEntireImage);
}

void klBigTiffFileManager::writeTiffFile(char* filename,  void* raster, TiffImageInfo info ){
	TIFF *output;
	uint32 width, height;

	if((output = TIFFOpen(filename, "w")) == NULL)
	{
		std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
		ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
		std::string err = ANSI_INFO_ss.str();
		err+="Could not open image file in writeTiffFile: ";
		err+=filename;
		logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());flushall();throw err; 
	}

	width = info.width;
	height = info.height;

	TIFFSetField(output, TIFFTAG_IMAGEWIDTH, width);
	TIFFSetField(output, TIFFTAG_IMAGELENGTH, height);
	TIFFSetField(output, TIFFTAG_COMPRESSION, COMPRESSION_NONE);

	if(info.isInterleaved)
		TIFFSetField(output, TIFFTAG_PLANARCONFIG,PLANARCONFIG_CONTIG );
	else
		TIFFSetField(output, TIFFTAG_PLANARCONFIG,PLANARCONFIG_SEPARATE );

	if(info.isRGB)
		TIFFSetField(output, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);


	if(info.bitspersample==8)
	{
		TIFFSetField(output, TIFFTAG_BITSPERSAMPLE, 8);
	}
	else
	{
		std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
		ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
		std::string err = ANSI_INFO_ss.str();
		err+="klBigTiffFileManager::writeTiffFile only supports 8 bit image data.";
		logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());flushall();throw err; 
	}

	TIFFSetField(output, TIFFTAG_SAMPLESPERPIXEL, info.samplesperpixel);

	if(TIFFWriteEncodedStrip(output, 0, raster, width * height * info.samplesperpixel) == 0)
	{
		std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
		ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
		std::string err = ANSI_INFO_ss.str();
		err+="Could not write tiff image in  writeTiffFile \n";
		logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());flushall();throw err; 
	}

	TIFFClose(output);
}

TiffImageInfo klBigTiffFileManager::getTiffImageInfo(unsigned int i)
{
	uint32 height=0;
	uint32 width=0;
	uint16 bitspersample=1;
	uint16 samplesperpixel=1;
	uint32 rowsperstrip=(DWORD)-1;
	uint16 photometric=0;
	uint16 compression=1;
	uint16 orientation=ORIENTATION_TOPLEFT; 
	uint16 res_unit=0; 
	uint32 tileWidth=0;
	uint32 tileHeight=0;
	uint16 planarconfig=0;
	uint32 subfiletype=0;
	uint32 x, y;
	float xresolution, xoffset;
	float yresolution, yoffset;
	bool isRGB;
	TiffImageInfo info;

	try
	{
		if(tifFile)
		{
			//Set the directory.
			setTiffDirectory(i);
			TIFFGetField(tifFile, TIFFTAG_IMAGEWIDTH, &info.width);
			TIFFGetField(tifFile, TIFFTAG_IMAGELENGTH, &info.height);
			TIFFGetField(tifFile, TIFFTAG_SAMPLESPERPIXEL, &info.samplesperpixel);
			TIFFGetField(tifFile, TIFFTAG_BITSPERSAMPLE, &info.bitspersample);
			TIFFGetField(tifFile, TIFFTAG_ROWSPERSTRIP, &info.rowsperstrip);   
			TIFFGetField(tifFile, TIFFTAG_PHOTOMETRIC, &info.photometric);
			TIFFGetField(tifFile, TIFFTAG_ORIENTATION, &orientation);
			TIFFGetFieldDefaulted(tifFile, TIFFTAG_RESOLUTIONUNIT, &info.res_unit);
			TIFFGetField(tifFile, TIFFTAG_XRESOLUTION, &info.xresolution);
			TIFFGetField(tifFile, TIFFTAG_YRESOLUTION, &info.yresolution);
			TIFFGetField(tifFile, TIFFTAG_XPOSITION, &info.xoffset);
			TIFFGetField(tifFile, TIFFTAG_YPOSITION, &info.yoffset);
			TIFFGetField(tifFile, TIFFTAG_TILEWIDTH, &info.tileWidth);
			TIFFGetField(tifFile, TIFFTAG_TILELENGTH, &info.tileHeight);
			TIFFGetField(tifFile, TIFFTAG_COMPRESSION, &info.compression);
			TIFFGetField(tifFile, TIFFTAG_SUBFILETYPE, &subfiletype);

			if(subfiletype==FILETYPE_REDUCEDIMAGE)
			{
				info.isSubsample = 1;   //Other options FILETYPE_PAGE	FILETYPE_MASK	
			}

			info.isTiled=TIFFIsTiled(tifFile);
			info.tiffTileMemSz=TIFFTileSize(tifFile);
			info.numberOfStrips=TIFFNumberOfStrips(tifFile);

			TIFFGetFieldDefaulted(tifFile, TIFFTAG_PLANARCONFIG, &planarconfig);
			//check the interleave status - this is important!
			if(planarconfig==PLANARCONFIG_CONTIG)
				info.isInterleaved=true;
			if(planarconfig==PLANARCONFIG_SEPARATE)
				info.isInterleaved=false;	

			//These are unused presently.
			//info.stripByteCounts=new uint32[info.numberOfStrips];
			//TIFFGetField(tifFile, TIFFTAG_STRIPBYTECOUNTS, info.stripByteCounts);

			////We need a chaeck to see what the offset pointer type is.
			//uint64* longOffsets=new uint64[info.numberOfStrips];
			//unsigned int ret=TIFFGetField(tifFile, TIFFTAG_STRIPBYTECOUNTS, longOffsets);
			//info.longOffsets= longOffsets;

			//check if this is RGB
			info.isRGB = (info.photometric == PHOTOMETRIC_RGB) ||
				(info.photometric == PHOTOMETRIC_YCBCR) ||
				(info.photometric == PHOTOMETRIC_SEPARATED) ||
				(info.photometric == PHOTOMETRIC_LOGL) ||
				(info.photometric == PHOTOMETRIC_LOGLUV);

			info.tiffDirs=TIFFNumberOfDirectories(tifFile);

			char * description = new char[1024];			
			TIFFGetField(tifFile, TIFFTAG_IMAGEDESCRIPTION, &description);
			info.ImageDescription=description;

			info.currentDir = i;

		}
		else
		{
			std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
			ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
			std::string err = ANSI_INFO_ss.str();
			err+="Problem with tif file handle in klBigTiffFileManager::getTiffImageInfo";		
			logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());flushall();throw err; 

		}
	}

	catch(klBigTiffError e)
	{
		std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
		ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
		std::string err = ANSI_INFO_ss.str();
		err+="Caught klBigTiffError in klBigTiffFileManager::getTiffImageInfo";
		err+=e.what();
		logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());flushall();throw err; 
	}
	catch(...)
	{
		std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
		ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
		std::string err = ANSI_INFO_ss.str();
		err+="Problem in klBigTiffFileManager::getTiffImageInfo";
		logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());flushall();throw err; 

	}

	return info;

}
unsigned int klBigTiffFileManager::getNumberOfTiffDirs()
{
	int numDirs=0;

	if(!tifFile)
	{
		throw "Bad tif file handle in klBigTiffFileManager::getNumberOfTiffDirs";
	}
	else
	{
		try
		{
			numDirs = TIFFNumberOfDirectories(tifFile);
		}
		catch(klBigTiffError e)
		{
			std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
			ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
			std::string err = ANSI_INFO_ss.str();
			err+="Caught klBigTiffError in klBigTiffFileManager::getNumberOfTiffDirs";
			err+=e.what();
			logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());flushall();throw err; 
		}
		catch(...)
		{
			std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
			ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
			std::string err = ANSI_INFO_ss.str();
			err+="Problem getting the number of tif dirs in  klBigTiffFileManager::getNumberOfTiffDirs";
			logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());flushall();throw err; 
		}
		return numDirs;

	}
}

void klBigTiffFileManager::setTiffDirectory(unsigned int i)
{
	try
	{
		if (!TIFFSetDirectory(tifFile, i))
		{
			std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
			ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
			std::string err = ANSI_INFO_ss.str();
			err+="Error setting tiff directory in klBigTiffFileManager::setTiffDirectory";
			logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());flushall();throw err; 
		}
	}
	catch( klBigTiffError e)
	{
		std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
		ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
		std::string err = ANSI_INFO_ss.str();
		err+="Caught klBigTiffError in klBigTiffFileManager::setTiffDirectory(char* ofileName)";
		err+=e.what();
		logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());flushall();throw err; 
	}
	catch(...)
	{
		std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
		ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
		std::string err = ANSI_INFO_ss.str();
		err+="Error setting tiff directory in klBigTiffFileManager::setTiffDirectory";
		logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());flushall();throw err; 
	}

}

bool klBigTiffFileManager::getThumbnail(const char* ofileName)
{
	unsigned char* thumbnail = NULL;

	try
	{
		//The thumbnail is always the second tiff dir in an klBigTiff image.
		//It's guaranteed to be there accorind to the klBigTiff documentation.

		TiffImageInfo info = getTiffImageInfo(1);

		TIFFGetField(tifFile, TIFFTAG_IMAGEWIDTH,&info.width );

		TIFFGetField(tifFile, TIFFTAG_IMAGELENGTH,&info.height );

		//The thumbnail is 1024x768 
		thumbnail=new unsigned char[info.height*info.width* info.samplesperpixel];

		int stripSize = TIFFStripSize(tifFile);

		int numStrips = TIFFNumberOfStrips(tifFile);

		unsigned char * buf = thumbnail;

		for (int strip = 0; strip < numStrips; strip++) 
		{
			TIFFReadEncodedStrip(tifFile, strip, buf, (tsize_t) -1);
			buf += stripSize;
		}

		char* ppmFileName=new char[1024];
		sprintf(ppmFileName,"%s",ofileName);
		write_ppm(ppmFileName,info.width,info.height,thumbnail);

		delete ppmFileName;

		delete thumbnail;

		return true;
	}
	catch(klBigTiffError e)
	{
		if(thumbnail)
			delete thumbnail;
		std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
		ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
		std::string err = ANSI_INFO_ss.str();
		err+="Caught klBigTiffError in klBigTiffFileManager::getThumbnail(char* ofileName)";
		err+=e.what();
		logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());flushall();throw err; 


	}
	catch(...)
	{
		if(thumbnail)
			delete thumbnail;

		std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
		ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
		std::string err = ANSI_INFO_ss.str();
		err+="Caught a non klBigTiffError in  klBigTiffFileManager::getThumbnail(const char* ofileName)";
		logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());flushall();throw err; 
	}

	return false;
}

bool klBigTiffFileManager::getLabelImage(const char* ofileName)
{
	unsigned char* label = NULL;

	unsigned int i=0;

	//This represents the tiff dir which has the label image. 
	//Negative value indicates we did not find the label indicator in the metadata.
	int labelDir=-1;

	TiffImageInfo info = getTiffImageInfo(0);

	for(i=0;i<info.tiffDirs;i++)
	{
		try
		{
			std::map<std::string,std::string> metadata= getMetadata(i);

			std::string imageDescriptionTag=metadata["ImageDescription"];

			std::string labelKey="label";
			size_t found;
			found=imageDescriptionTag.rfind(labelKey);
			if (found!=std::string::npos)
			{
				labelDir=i;
			}
		}
		catch(klBigTiffError e)
		{
			std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
			ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
			std::string err = ANSI_INFO_ss.str();
			err+="Caught klBigTiffError in klBigTiffFileManager::getLabelImage(char* ofileName)";
			err+=e.what();
			logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());flushall();throw err; 

		}

	}

	if (labelDir<0)
		return false;
	char* ppmFileName=NULL;
	try
	{
		ppmFileName=new char[1024];

		info = getTiffImageInfo(labelDir);

		TIFFGetField(tifFile, TIFFTAG_IMAGEWIDTH,&info.width );

		TIFFGetField(tifFile, TIFFTAG_IMAGELENGTH,&info.height );

		label = new unsigned char[info.height*info.width* info.samplesperpixel];

		int stripSize = TIFFStripSize(tifFile);

		int numStrips = TIFFNumberOfStrips(tifFile);

		unsigned char * buf = label;

		for (int strip = 0; strip < numStrips; strip++) 
		{
			TIFFReadEncodedStrip(tifFile, strip, buf, (tsize_t) -1);
			buf += stripSize;
		}

		sprintf(ppmFileName,"%s",ofileName);
		write_ppm(ppmFileName,info.width,info.height,label);

		delete ppmFileName;
		delete label;
	}
	catch(klBigTiffError e)
	{
		if(label)
			delete label;
		if(ppmFileName)
			delete ppmFileName;
		std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
		ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
		std::string err = ANSI_INFO_ss.str();
		err+="Caught klBigTiffError in klBigTiffFileManager::getLabelImage(char* ofileName)";

		err+=e.what();
		logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());flushall();throw err; 
	}

	catch(...)
	{
		if(label)
			delete label;
		if(ppmFileName)
			delete ppmFileName;
		std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
		ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
		std::string err = ANSI_INFO_ss.str();
		err+="Caught non klBigTiffError in klBigTiffFileManager::getLabelImage(char* ofileName)";
		logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());flushall();throw err; 
	}

	return true;

}

bool klBigTiffFileManager::getWSIImage(const char* ofileName)
{
	unsigned char* macro=NULL;

	unsigned char* rotated_macro=NULL;

	unsigned int i=0;

	//This represents the tiff dir which has the WSI image. 
	//Negative value indicates we did not find the label indicator in the metadata.
	int macroDir=-1;

	TiffImageInfo info = getTiffImageInfo(0);

	for(i=0;i<info.tiffDirs;i++)
	{
		try
		{
			std::map<std::string,std::string> metadata= getMetadata(i);

			std::string imageDescriptionTag=metadata["ImageDescription"];

			std::string labelKey="macro";
			size_t found;
			found=imageDescriptionTag.rfind(labelKey);
			if (found!=std::string::npos)
			{
				macroDir=i;
			}
		}
		catch(klBigTiffError e)
		{
			throw e;
		}

	}
	if(macroDir<0)
		return false;

	char* ppmFileName=new char[1024];
	try
	{

		info = getTiffImageInfo(macroDir);

		int width=info.width;

		int height = info.height;

		TIFFGetField(tifFile, TIFFTAG_IMAGEWIDTH,&info.width );

		TIFFGetField(tifFile, TIFFTAG_IMAGELENGTH,&info.height );

		unsigned int pix_stride = info.samplesperpixel;

		rotated_macro = new unsigned char[info.height*info.width* pix_stride];

		macro = new unsigned char[info.height*info.width* pix_stride];

		int stripSize = TIFFStripSize(tifFile);

		int numStrips = TIFFNumberOfStrips(tifFile);

		unsigned char * buf = macro;

		for (int strip = 0; strip < numStrips; strip++) 
		{
			TIFFReadEncodedStrip(tifFile, strip, buf, (tsize_t) -1);
			buf += stripSize;
		}

		int i=0;
		int j=0;
		int k=0;
		for(i=0;i<width;i++)
		{
			for(j=height-1, k=0;j>=0;j--, k++)
			{

				unsigned char* iPtr = macro+j*width*pix_stride +i*pix_stride;

				unsigned char R=*(iPtr  +0);
				unsigned char G=*(iPtr  +1);
				unsigned char B=*(iPtr  +2);

				unsigned char* oPtr =rotated_macro+ i*height*pix_stride +k*pix_stride;

				*(oPtr +0)=R;
				*(oPtr +1)=G;
				*(oPtr +2)=B;

			}
		}

		sprintf(ppmFileName,"%s",ofileName);
		write_ppm(ppmFileName,info.width,info.height,macro);

		delete ppmFileName;
		delete macro;
		delete rotated_macro;
	}
	catch(klBigTiffError e)
	{
		if(macro)
			delete macro;
		if(rotated_macro)
			delete rotated_macro;
		if(ppmFileName)
			delete ppmFileName;
		throw e;
	}

	catch(...)
	{
		if(macro)
			delete macro;
		if(rotated_macro)
			delete rotated_macro;
		if(ppmFileName)
			delete ppmFileName;
		std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
		ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
		std::string err = ANSI_INFO_ss.str();
		err+="Problem in klBigTiffFileManager::getWSIImage(char* ofileName)";
		logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());flushall();throw err; 
	}
	return true;
}

std::map<std::string,std::string> klBigTiffFileManager::getMetadata(unsigned int i)
{
	if(tifFile)
	{
		try
		{
			TiffImageInfo info = getTiffImageInfo(i);

			char * description;	
			//  Example ImageInfoTag : <klBigTiff Image Library v9.1.6\r\n30600x22789 [0,100 30000x22689] (240x240) RAW|AppMag = 20        \
			//  |StripeWidth = 2040|ScanScope ID = CPAPERIOCS|Filename = Slide1-005|Title = none|Date = 10/07/09|Time = 14:27:35|         \
			//  User = 14711d63-1efa-442c-a2d1-cbe9ea9ede80|MPP = 0.4990|Left = 30.594622|Top = 17.086874|LineCameraSkew = -0.000424|     \
			//  LineAreaXOffset = 0.019265|LineAreaYOffset = -0.000313|DSR ID = ss5263|ImageID = 1003812|OriginalWidth = 30600|Originalheight = 22789|Filtered = 5|ICC Profile = ScanScope v1\0>

			TIFFGetField(tifFile, TIFFTAG_IMAGEDESCRIPTION, &description);

			//Parse the klBigTiff string.
			std::string tag(description);

			std::vector<std::string> elements=parseImageDescriptionTag(tag);

			std::map<std::string,std::string> xmlMap= createXMLMap( elements, info);

			return xmlMap;

		}
		catch(...)
		{
			std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
			ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
			std::string err = ANSI_INFO_ss.str();
			err+="Problem Extracting Metadata in klBigTiffFileManager::getMetadata";
			logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());flushall();throw err; 
		}
	}
	else
	{
		std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
		ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
		std::string err = ANSI_INFO_ss.str();
		err+="Bad tifFile in klBigTiffFileManager::getMetadata";
		logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());flushall();throw err; 

	}
}

std::vector<std::string> klBigTiffFileManager::parseImageDescriptionTag(std::string tag)
{

	std::vector < std::string > elements;   
	std::string  temp;

	while (tag.find("|", 0) != std::string::npos)
	{
		size_t  pos = tag.find("|", 0); 
		//get the delimiter
		temp = tag.substr(0, pos);      
		tag.erase(0, pos + 1);          
		//and put it into the array
		elements.push_back(temp);       
	}
	//the element is all that's left
	elements.push_back(tag);           

	return elements;
}

std::map<std::string,std::string> klBigTiffFileManager::createXMLMap(std::vector<std::string> elements, TiffImageInfo info)
{
	//Call conversion op form TiffImageInfo to map 
	std::map<std::string, std::string> mdMap=info;

	//Parse key/val pairs from vector of elements
	unsigned int i;
	unsigned int numElements=elements.size();
	for(i=0;i<numElements;i++)
	{
		std::vector < std::string > key_val;   
		std::string  temp;
		while (elements[i].find(" =", 0) != std::string::npos)
		{
			//store the position of the delimiter
			size_t  pos = elements[i].find(" =", 0); 

			temp = elements[i].substr(0, pos);  

			elements[i].erase(0, pos + 2);    

			key_val.push_back(temp);       
		}
		key_val.push_back(elements[i]);           

		//Sanity check and verify the vector has two elements
		if(key_val.size()==2)
		{
			std::map<std::string ,std::string>::value_type element(key_val[0],key_val[1]);
			mdMap.insert(element);

		}

	}
	return mdMap;
}

#include "kl_image_processing_functors.h"
#include "kl_ppm_image_io.h"
#include "kl_img_pca.h"
#include "kl_thread_workflow.h"
class klStringArgThread :public  klThread<klMutex>
{
public:
	klStringArgThread(void (*pf)(string,string ) )
	{
		fPtr=pf;
	}
	void run()
	{
		fPtr(arg,outpath);
	}
	void (*fPtr)(string, string);

	string arg;
	string outpath;
};

void TissueAnalysis(string fileName,string outputPath);

void klBigTiffFileManager::ExtractImageTiles(const char * outputFileBase) 
{
	TiffImageInfo info =getTiffImageInfo(0);

	unsigned int x=0;
	unsigned int y=0;
	unsigned int w=info.width;
	unsigned int h=info.height;

	int tsx =info.tileHeight;
	int tsy =info.tileWidth;

	int nTilesW = info.width  / tsx ;
	int nTilesH = info.height / tsy;

	HANDLE	 hProcess=GetCurrentProcess ();

	klThreadId thisThread=klThread<klMutex>::getCurrentThreadId();	

	klThreadWorkflow<int,klSmartPtr<klStringArgThread > > workflow;

	void (*pf)(string,string) =TissueAnalysis;

	int numThreads = 1;
	int k=0;
	char** files = new char*[numThreads];
	char** ids = new char*[numThreads];  //this little scheme is leaking memory
	
	for(int iy=0; iy+tsy*nTilesH<=h; iy+=tsy*nTilesH)
	{	
		for(int ix=0; ix+tsx*nTilesW<w; ix+=tsx*nTilesW) 
		{
			char* tiffile=new char[1024];
			sprintf(tiffile,"%s_%d_%d_%d_%d.tif",outputFileBase,ix,iy,tsx*nTilesW,tsy*nTilesH);
			std::cout<<tiffile<<endl;
			char* ppmfile=new char[1024];

			sprintf(ppmfile,"%s_%d_%d_%d_%d.ppm",outputFileBase,ix,iy,tsx*nTilesW,tsy*nTilesH);

			char* id = new char[1024];
			sprintf(id,"%s_%d_%d_%d_%d",outputFileBase,ix,iy,tsx*nTilesW,tsy*nTilesH);

			*(files+k) = ppmfile;
			*(ids+k) = id;

			try
			{
				ExtractMainResTile( tiffile,ppmfile,ix,iy,tsx*nTilesW,tsy*nTilesH);
				//TissueAnalysis(ppmfile,id);
				/*k++;
				if(k==numThreads)
				{
				for(int l=0;l<numThreads;l++)
				{
				klSmartPtr<klStringArgThread > element=new klStringArgThread(pf);
				std::string fileN(*(files +l));
				element->arg = fileN;
				std::string idN(*(ids+l));
				element->outpath = idN;
				workflow.insertWorkElement(l, element);
				}
				workflow.doWork();
				k=0;
				}*/
			}
			catch(...)
			{
				cout<<"Bad news dumping tiles";
			}
		}
	}
}

void klBigTiffFileManager::ExtractMainResTile(const char * tiffoutputFile,const char * ppmoutputFile,unsigned int x,unsigned int y,unsigned int w,unsigned int h) 
{
	TiffImageInfo info =getTiffImageInfo(0);

	if(x<0 || y <0 || w<=0 || h<=0) 
	{
		std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
		ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
		std::string err = ANSI_INFO_ss.str();
		err+="Negative image coords in void klBigTiffFileManager::ExtractMainResTile.";
		logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());
		flushall();
		throw err;
		
	}
	if(x+w > info.width || y+h > info.height) 
	{
		std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
		ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
		std::string err = ANSI_INFO_ss.str();
		err+="Region out of bounds in klBigTiffFileManager::ExtractMainResTile";
		logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());
		flushall();
		throw err;
		

	}

	int tsx =info.tileHeight;
	int tsy = info.tileWidth;

	unsigned int bitdepth = info.bitspersample;
	unsigned int bands = info.samplesperpixel;

	int stepSize=bands * w;
	//FILE * ppmfile;
	//ppmfile = fopen (ppmoutputFile, "wb");
	//fprintf (ppmfile, "P6\n%d %d\n255\n", w, h);

	//Tile indices
	int sx,sy,ex,ey;
	sx = x/tsx;
	sy = y/tsy;	
	//The extent in tiles for the ROI we need to extract
	ex = (x+w)/tsx;
	ey = (y+h)/tsy;


	TIFF * tif = TIFFOpen(tiffoutputFile, "w");
	if(tif == NULL) 
	{
		std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
		ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
		std::string err = ANSI_INFO_ss.str();
		err+="Could not open output TIFF file : ";
		err+=tiffoutputFile;
		logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());flushall();throw err; 
	}

	TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, w);
	TIFFSetField(tif, TIFFTAG_IMAGELENGTH, h);
	TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bitdepth);
	TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, bands);
	if(bands == 3)
		TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
	if(bands ==1)
		TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);

	TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);

	if(bitdepth ==8)
	{
		//Holds one tile of data
		unsigned char * databuf =0;

		//Put the tiles in here
		unsigned char * tilebuf =0;
		try
		{
			databuf =new unsigned char[tsx*tsy*bands];
			tilebuf =new unsigned char[(ex-sx+1)*tsx*tsy*bands];
		}
		catch
			(...)
		{
			if(databuf!=0)
				delete databuf;
			if(tilebuf!=0)
				delete tilebuf;
			std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
			ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
			std::string err = ANSI_INFO_ss.str();
			err+="Error in klBigTiffFileManager::ExtractMainResPPM(const char * outputFile) - memory allocation error.";
			logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());flushall();throw err; 
		}
		int tilebuf_start_y ; 
		int tilebuf_end_y ; 
		int tilebuf_width = (ex-sx+1)*tsx;

		int region_x = (sx*tsx); 
		int region_y = y; 
		int output_y = 0; 

		for(int iy=sy; iy<=ey; iy++) 
		{
			tilebuf_start_y = iy*tsy;
			tilebuf_end_y = tilebuf_start_y + tsy;

			for(int ix=sx; ix<=ex; ix++) 
			{
				int tileSize = TIFFTileSize(tifFile);

				TIFFReadTile(tifFile, (tdata_t)databuf, ix*info.tileWidth, iy*info.tileHeight, 0, 0); 

				for(int copy_y=0; copy_y<tsy; copy_y++) 
				{
					memcpy(tilebuf+(copy_y*tilebuf_width*bands)+(ix-sx)*tsx*bands, databuf+(copy_y*tsx*bands), tsx*bands);
				}
			}		

			while(region_y < tilebuf_end_y && region_y < y+h) 
			{ 
				TIFFWriteScanline(tif, &tilebuf[(region_y-tilebuf_start_y)*tilebuf_width*bands + (x-region_x)*bands], output_y, 0);	
			//	fwrite (&tilebuf[(region_y-tilebuf_start_y)*tilebuf_width*bands + (x-region_x)*bands], bands * w, 1, ppmfile);
				region_y++;
				output_y++;
			}
		}
		delete tilebuf;
		delete databuf;
	}
	if(bitdepth ==16)
	{
		//Holds one tile of data
		unsigned short * databuf =0;

		//Put the tiles in here
		unsigned short * tilebuf =0;
		try
		{
			databuf =new unsigned short[tsx*tsy*bands];
			tilebuf =new unsigned short[(ex-sx+1)*tsx*tsy*bands];
		}
		catch
			(...)
		{
			if(databuf!=0)
				delete databuf;
			if(tilebuf!=0)
				delete tilebuf;
			std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
			ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
			std::string err = ANSI_INFO_ss.str();
			err+="Error in klBigTiffFileManager::ExtractMainResPPM(const char * outputFile) - memory allocation error.";
			logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());flushall();throw err; 
		}
		int tilebuf_start_y ; 
		int tilebuf_end_y ; 
		int tilebuf_width = (ex-sx+1)*tsx;

		int region_x = (sx*tsx); 
		int region_y = y; 
		int output_y = 0; 
		
		for(int iy=sy; iy<=ey; iy++) 
		{
			tilebuf_start_y = iy*tsy;
			tilebuf_end_y = tilebuf_start_y + tsy;

			for(int ix=sx; ix<=ex; ix++) 
			{
				int tileSize = TIFFTileSize(tifFile);

				TIFFReadTile(tifFile, (tdata_t)databuf, ix*info.tileWidth, iy*info.tileHeight, 0, 0); 

				/*{
					unsigned int pixelC;
					for(pixelC=0;pixelC<tsx*tsy*bands; pixelC++)
					{
						unsigned short val =databuf[pixelC];
						std::cout<< val<< "   :   \t\t"<<GetBit(val,0)<<"   "<< GetBit(val,1)<<"   "<< GetBit(val,2)<<"   "<< GetBit(val,3)<<"   "<< GetBit(val,4)<<"   "<< GetBit(val,5)<<"   "<< GetBit(val,6)<<"   "<< GetBit(val,7)<<" || "<< GetBit(val,8)<<"   "<< GetBit(val,9)<<"   "<< GetBit(val,10)<<"   "<< GetBit(val,11)<<"   "<< GetBit(val,12)<<"   "<< GetBit(val,13)<<"   "<< GetBit(val,14)<<"   "<< GetBit(val,15)<<std::endl;
						databuf[pixelC]= val>>6;
						val =databuf[pixelC];
						std::cout<< val<< "   :   \t\t"<< GetBit(val,0)<<"   "<< GetBit(val,1)<<"   "<< GetBit(val,2)<<"   "<< GetBit(val,3)<<"   "<< GetBit(val,4)<<"   "<< GetBit(val,5)<<"   "<< GetBit(val,6)<<"   "<< GetBit(val,7)<<" || "<< GetBit(val,8)<<"   "<< GetBit(val,9)<<"   "<< GetBit(val,10)<<"   "<< GetBit(val,11)<<"   "<< GetBit(val,12)<<"   "<< GetBit(val,13)<<"   "<< GetBit(val,14)<<"   "<< GetBit(val,15)<<std::endl;
					}
				}*/

				////////////////////////////////////////////////////
				const Ipp16u* pSrc = databuf;
				int val = 6; 
				Ipp16u* pDst = databuf;
				int len = tsx*tsy*bands;
				IppStatus status  = ippsRShiftC_16u(pSrc,  val, pDst, len);


				//{
				//	unsigned int pixelC;
				//	for(pixelC=0;pixelC<tsx*tsy*bands; pixelC++)
				//	{
				//		unsigned short val =databuf[pixelC];
				//		std::cout<< GetBit(val,0)<<"   "<< GetBit(val,1)<<"   "<< GetBit(val,2)<<"   "<< GetBit(val,3)<<"   "<< GetBit(val,4)<<"   "<< GetBit(val,5)<<"   "<< GetBit(val,6)<<"   "<< GetBit(val,7)<<" || "<< GetBit(val,8)<<"   "<< GetBit(val,9)<<"   "<< GetBit(val,10)<<"   "<< GetBit(val,11)<<"   "<< GetBit(val,12)<<"   "<< GetBit(val,13)<<"   "<< GetBit(val,14)<<"   "<< GetBit(val,15)<<std::endl;
				//		//databuf[pixelC]= val>>4;
				//		//val =databuf[pixelC];
				//		//std::cout<< GetBit(val,0)<<"   "<< GetBit(val,1)<<"   "<< GetBit(val,2)<<"   "<< GetBit(val,3)<<"   "<< GetBit(val,4)<<"   "<< GetBit(val,5)<<"   "<< GetBit(val,6)<<"   "<< GetBit(val,7)<<" || "<< GetBit(val,8)<<"   "<< GetBit(val,9)<<"   "<< GetBit(val,10)<<"   "<< GetBit(val,11)<<"   "<< GetBit(val,12)<<"   "<< GetBit(val,13)<<"   "<< GetBit(val,14)<<"   "<< GetBit(val,15)<<std::endl;
				//	}
				//}

				for(int copy_y=0; copy_y<tsy; copy_y++) 
				{
					memcpy(tilebuf+(copy_y*tilebuf_width*bands)+(ix-sx)*tsx*bands, databuf+(copy_y*tsx*bands), tsx*bands*2);
				}
			}		

			while(region_y < tilebuf_end_y && region_y < y+h) 
			{ 
				TIFFWriteScanline(tif, &tilebuf[(region_y-tilebuf_start_y)*tilebuf_width*bands + (x-region_x)*bands], output_y, 0);	
				region_y++;
				output_y++;
			}
		}
		delete databuf;		
		delete tilebuf;
	}

	flushall();

	TIFFClose(tif);
	int  size=0;
	void *iccProfile;
}

void klBigTiffFileManager::ExtractMainResTileApplyICC(const char * outputFile,unsigned int x,unsigned int y,unsigned int w,unsigned int h) 
{
	unsigned char * databuf =0;
	unsigned char * tilebuf =0;
	unsigned char * tilebufICC = 0;
	try
	{
		TiffImageInfo info =getTiffImageInfo(0);

		if(x<0 || y <0 || w<=0 || h<=0) 
		{
			std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
			ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
			std::string err = ANSI_INFO_ss.str();
			err+="Negative image coords in void klBigTiffFileManager::ExtractMainResTile.";
			logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());flushall();throw err; 
		}
		if(x+w > info.width || y+h > info.height) 
		{std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
		ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
		std::string err = ANSI_INFO_ss.str();
		err+= "Region out of bounds in klBigTiffFileManager::ExtractMainResTile";
		logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());flushall();throw err; 

		}
		int tsx =info.tileHeight;
		int tsy = info.tileWidth;
		int sx,sy,ex,ey;
		sx = x/tsx;
		sy = y/tsy;	
		ex = (x+w)/tsx;
		ey = (y+h)/tsy;
		TIFF * tif = TIFFOpen(outputFile, "w");
		if(tif == NULL) 
		{
			std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
			ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
			std::string err = ANSI_INFO_ss.str();
			err+="Could not open Tiff file";
			err+=outputFile;
			logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());flushall();throw err; 
		}

		TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, w);
		TIFFSetField(tif, TIFFTAG_IMAGELENGTH, h);
		TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
		TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
		TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 3);
		TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
		TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);

		try
		{
			databuf =new unsigned char[tsx*tsy*3];
			tilebuf =new unsigned char[(ex-sx+1)*tsx*tsy*3];
			tilebufICC= new unsigned char[(ex-sx+1)*tsx*tsy*3];
		}
		catch
			(...)//bbcrevisit - instrument SVS and klIPP with klnew handler.
		{
			if(databuf!=0)
				delete databuf;
			if(tilebuf!=0)
				delete tilebuf;
			if(tilebufICC!=0)
				delete tilebufICC;
			std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
			ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
			std::string err = ANSI_INFO_ss.str();
			err+="Error in klBigTiffFileManager::ExtractMainResTileApplyICC(const char * outputFile) - memory allocation error.";
			logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());flushall();throw err; 
		}

		//////////////////ICC////////////////////////////////////
		const char *szSrcProfile="aperioICC.icm";
		int  size=0;
		void *iccProfile;
		TIFFGetField(tifFile, TIFFTAG_ICCPROFILE, &size, &iccProfile);
		if(size!=0)
		{
			FILE* iccFile;
			iccFile= fopen(szSrcProfile, "wb");
			fwrite(iccProfile,size,1,iccFile);
			flushall();
			fclose(iccFile);
		}
		else
		{std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
		ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
		std::string err = ANSI_INFO_ss.str();
		err+="No ICC profile found in klBigTiffFileManager::ExtractMainResTileApplyICC";
		logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());flushall();throw err; 

		}
		int ans = dumpICC("aperioICC.icm");
		const char *szDstProfile="sRGB_IEC61966-2-1_no_black_scaling.icc";
		int nIntent=0;
		//0 - Perceptual 1 - Relative Colorimetric 2 - Saturation 3 - Absolute Colorimetric
		unsigned long i, j, k, sn, sphoto, dn, photo, space;
		CIccCmm cmm;
		unsigned char *sptr, *dptr;
		bool bSuccess = true;
		bool bConvert = false;
		std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
		ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
		std::string err = ANSI_INFO_ss.str();
		if (cmm.AddXform(szSrcProfile, nIntent<0 ? icUnknownIntent : (icRenderingIntent)nIntent)) 
		{
			err+= "Problem with AddXform for source Possible Invalid Profile in klBigTiffFileManager::ExtractMainResTileApplyICC";
			logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());flushall();throw err; 
		}
		if (szDstProfile && *szDstProfile && cmm.AddXform(szDstProfile)) 
		{
			err+="Problem with AddXform for destination profile Possible Invalid Profile in klBigTiffFileManager::ExtractMainResTileApplyICC";
			logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());flushall();throw err; 
		}
		if (cmm.Begin() != icCmmStatOk) 
		{
			err+="Bad Icc status in klBigTiffFileManager::ExtractMainResPPM_ApplyICC";
			logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());flushall();throw err; 
		}
		sn =3;
		sphoto = info.photometric; //Should be PHOTOMETRIC_RGB
		space =icSigRgbData;
		switch (cmm.GetDestSpace()) 
		{
		case icSigRgbData:
			photo = PHOTO_MINISBLACK;
			dn = 3;
			break;
		case icSigCmyData:
			photo = PHOTO_MINISWHITE;
			dn = 3;
			break;
		case icSigXYZData:
			bConvert = true;
			//Fall through - No break here
		case icSigLabData:
			photo = PHOTO_CIELAB;
			dn = 3;
			break;
		case icSigCmykData:
		case icSig4colorData:
			photo = PHOTO_MINISWHITE;
			dn = 4;
			break;
		case icSig5colorData:
			photo = PHOTO_MINISWHITE;
			dn = 5;
			break;
		case icSig6colorData:
			photo = PHOTO_MINISWHITE;
			dn = 6;
			break;
		case icSig7colorData:
			photo = PHOTO_MINISWHITE;
			dn = 7;
			break;
		case icSig8colorData:
			photo = PHOTO_MINISWHITE;
			dn = 8;
			break;
		default:
			{
				ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
				std::string err = ANSI_INFO_ss.str();
				err+="Bad Destination Colorspace in klBigTiffFileManager::ExtractMainResTileApplyICC";
				logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());flushall();throw err; 
			}
		}
		//////////////////END ICC ///////////////////////////////////
		int tilebuf_start_y ; 
		int tilebuf_end_y ; 
		int tilebuf_width = (ex-sx+1)*tsx;
		int region_x = (sx*tsx); 
		int region_y = y; 
		int output_y = 0; 
		for(int iy=sy; iy<=ey; iy++) 
		{
			tilebuf_start_y = iy*tsy;
			tilebuf_end_y = tilebuf_start_y + tsy;
			for(int ix=sx; ix<=ex; ix++) 
			{
				int tileSize = TIFFTileSize(tifFile);
				TIFFReadTile(tifFile, (tdata_t)databuf, ix*info.tileWidth, iy*info.tileHeight, 0, 0); 
				for(int copy_y=0; copy_y<tsy; copy_y++) 
				{
					memcpy(tilebuf+(copy_y*tilebuf_width*3)+(ix-sx)*tsx*3, databuf+(copy_y*tsx*3), tsx*3);
				}
			}	
			/////////////////////////////////////ICC
			icFloatNumber Pixel[16];
			unsigned char *pSBuf;
			pSBuf=tilebuf;//Source Data

			unsigned char* pDBuf=tilebufICC;
			unsigned int tileBufHeight=tsy;
			unsigned int tileBufWidth=tilebuf_width;
			for (i=0; i<tileBufHeight; i++) 
			{
				for (sptr=(pSBuf+i*tilebuf_width*3), dptr=(pDBuf+i*tilebuf_width*3), j=0; j<tilebuf_width; j++, sptr+=sn, dptr+=dn) 
				{
					for (k=0; k<sn; k++) 
					{
						Pixel[k] = (icFloatNumber)sptr[k] / 255.0f;
					}
					cmm.Apply(Pixel, Pixel);
					for (k=0; k<dn; k++) 
					{
						dptr[k] = (unsigned char)(UnitClip(Pixel[k]) * 255.0 + 0.5);
					}
				}
			}
			////////////////////////////////////END ICC
			while(region_y < tilebuf_end_y && region_y < y+h) 
			{ 
				TIFFWriteScanline(tif, &tilebufICC[(region_y-tilebuf_start_y)*tilebuf_width*3 + (x-region_x)*3], output_y, 0);		
				region_y++;
				output_y++;
			}		
		}
		TIFFClose(tif);

		if(databuf!=0)
			delete databuf;
		if(tilebuf!=0)
			delete tilebuf;
		if(tilebufICC!=0)
			delete tilebufICC;


	}
	catch(klBigTiffError e)
	{
		if(databuf!=0)
			delete databuf;
		if(tilebuf!=0)
			delete tilebuf;
		if(tilebufICC!=0)
			delete tilebufICC;

		throw e;
	}
	catch(...)
	{
		std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
		ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
		std::string err = ANSI_INFO_ss.str()+ "Caught non- klBigTiffError in klBigTiffFileManager::ExtractMainResTileApplyICC";
		logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());flushall();throw err; 
	}

}

void TIFFError(const char* module, const char* fmt, va_list argptr)
{
	char* msg= new char[4048];  
	vsprintf (msg, fmt,argptr);
	std::string mErr(msg);
	klBigTiffError err(mErr);
	delete msg;	
	
}

void TIFFErrorExtFn(thandle_t fd, const char* module, const char* fmt, va_list argptr)
{
	if(fd)
		CloseHandle(fd);

	char* msg= new char[4048];  
	vsprintf (msg, fmt,argptr);
	std::string mErr(msg);
	klBigTiffError err(mErr);
	delete msg;	
	
}

bool klBigTiffFileManager::getTiffDirImage(unsigned int dir, const char* tiffoutputFile)
{
	TiffImageInfo info =getTiffImageInfo(dir);
	unsigned int x=0;
	unsigned int y=0;
	unsigned int w=info.width;
	unsigned int h = info.height;

	if(x<0 || y <0 || w<=0 || h<=0) 
	{
		std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
		ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
		std::string err = ANSI_INFO_ss.str();
		err+="Negative image coords in void klBigTiffFileManager::ExtractMainResTile.";
		throw std::exception(err.c_str());
	}
	if(x+w > info.width || y+h > info.height) 
	{
		std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
		ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
		std::string err = ANSI_INFO_ss.str();
		err+="Region out of bounds in klBigTiffFileManager::ExtractMainResTile";
		throw std::exception(err.c_str());

	}

	int tsx =info.tileHeight;
	int tsy = info.tileWidth;

	unsigned int bitdepth = info.bitspersample;
	unsigned int bands = info.samplesperpixel;

	int stepSize=bands * w;
	//FILE * ppmfile;
	//ppmfile = fopen (ppmoutputFile, "wb");
	//fprintf (ppmfile, "P6\n%d %d\n255\n", w, h);

	//Tile indices
	int sx,sy,ex,ey;
	sx = x/tsx;
	sy = y/tsy;	
	//The extent in tiles for the ROI we need to extract
	ex = (x+w)/tsx;
	ey = (y+h)/tsy;


	TIFF * tif = TIFFOpen(tiffoutputFile, "w");
	if(tif == NULL) 
	{
		std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
		ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
		std::string err = ANSI_INFO_ss.str();
		err+="Could not open output TIFF file : ";
		err+=tiffoutputFile;
		logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());flushall();throw err; 
	}

	TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, w);
	TIFFSetField(tif, TIFFTAG_IMAGELENGTH, h);
	TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bitdepth);
	TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, bands);
	if(bands == 3)
		TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
	if(bands ==1)
		TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);

	TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);

	if(bitdepth ==8)
	{
		//Holds one tile of data
		unsigned char * databuf =0;

		//Put the tiles in here
		unsigned char * tilebuf =0;
		try
		{
			databuf =new unsigned char[tsx*tsy*bands];
			tilebuf =new unsigned char[(ex-sx+1)*tsx*tsy*bands];
		}
		catch
			(...)
		{
			if(databuf!=0)
				delete databuf;
			if(tilebuf!=0)
				delete tilebuf;
			std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
			ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
			std::string err = ANSI_INFO_ss.str();
			err+="Error in klBigTiffFileManager::ExtractMainResPPM(const char * outputFile) - memory allocation error.";
			logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());flushall();throw err; 
		}
		int tilebuf_start_y ; 
		int tilebuf_end_y ; 
		int tilebuf_width = (ex-sx+1)*tsx;

		int region_x = (sx*tsx); 
		int region_y = y; 
		int output_y = 0; 

		for(int iy=sy; iy<=ey; iy++) 
		{
			tilebuf_start_y = iy*tsy;
			tilebuf_end_y = tilebuf_start_y + tsy;

			for(int ix=sx; ix<=ex; ix++) 
			{
				int tileSize = TIFFTileSize(tifFile);

				TIFFReadTile(tifFile, (tdata_t)databuf, ix*info.tileWidth, iy*info.tileHeight, 0, 0); 

				for(int copy_y=0; copy_y<tsy; copy_y++) 
				{
					memcpy(tilebuf+(copy_y*tilebuf_width*bands)+(ix-sx)*tsx*bands, databuf+(copy_y*tsx*bands), tsx*bands);
				}
			}		

			while(region_y < tilebuf_end_y && region_y < y+h) 
			{ 
				TIFFWriteScanline(tif, &tilebuf[(region_y-tilebuf_start_y)*tilebuf_width*bands + (x-region_x)*bands], output_y, 0);	
			//	fwrite (&tilebuf[(region_y-tilebuf_start_y)*tilebuf_width*bands + (x-region_x)*bands], bands * w, 1, ppmfile);
				region_y++;
				output_y++;
			}
		}
		delete tilebuf;
		delete databuf;
	}
	if(bitdepth ==16)
	{

		//Holds one tile of data
		unsigned short * databuf =0;

		//Put the short in here
		unsigned short * tilebuf =0;
		try
		{
			databuf =new unsigned short[tsx*tsy*bands];
			tilebuf =new unsigned short[(ex-sx+1)*tsx*tsy*bands];
		}
		catch
			(...)
		{
			if(databuf!=0)
				delete databuf;
			if(tilebuf!=0)
				delete tilebuf;
			std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
			ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
			std::string err = ANSI_INFO_ss.str();
			err+="Error in klBigTiffFileManager::ExtractMainResPPM(const char * outputFile) - memory allocation error.";
			logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());flushall();throw err; 
		}
		int tilebuf_start_y ; 
		int tilebuf_end_y ; 
		int tilebuf_width = (ex-sx+1)*tsx;

		int region_x = (sx*tsx); 
		int region_y = y; 
		int output_y = 0; 


		for(int iy=sy; iy<=ey; iy++) 
		{
			tilebuf_start_y = iy*tsy;
			tilebuf_end_y = tilebuf_start_y + tsy;

			for(int ix=sx; ix<=ex; ix++) 
			{
				int tileSize = TIFFTileSize(tifFile);

				TIFFReadTile(tifFile, (tdata_t)databuf, ix*info.tileWidth, iy*info.tileHeight, 0, 0); 

				for(int copy_y=0; copy_y<tsy; copy_y++) 
				{
					memcpy(tilebuf+(copy_y*tilebuf_width*bands)+(ix-sx)*tsx*bands, databuf+(copy_y*tsx*bands), tsx*bands*2);
				}
			}		

			while(region_y < tilebuf_end_y && region_y < y+h) 
			{ 
				TIFFWriteScanline(tif, &tilebuf[(region_y-tilebuf_start_y)*tilebuf_width*bands + (x-region_x)*bands], output_y, 0);	
				region_y++;
				output_y++;
			}
		}
		delete databuf;		
		delete tilebuf;
	}

	flushall();

	TIFFClose(tif);
	int  size=0;
	void *iccProfile;
}

bool klBigTiffFileManager::getStripSubDir(unsigned int subDir,const char* ofileName)
{
	unsigned char* thumbnail = NULL;

	try
	{
		TiffImageInfo info = getTiffImageInfo(subDir);

		TIFFGetField(tifFile, TIFFTAG_IMAGEWIDTH,&info.width );

		TIFFGetField(tifFile, TIFFTAG_IMAGELENGTH,&info.height );

		thumbnail=new unsigned char[info.height*info.width* info.samplesperpixel];

		int stripSize = TIFFStripSize(tifFile);

		int numStrips = TIFFNumberOfStrips(tifFile);

		unsigned char * buf = thumbnail;

		for (int strip = 0; strip < numStrips; strip++) 
		{
			TIFFReadEncodedStrip(tifFile, strip, buf, (tsize_t) -1);
			buf += stripSize;
		}

		char* ppmFileName=new char[1024];
		sprintf(ppmFileName,"%s",ofileName);
		write_ppm(ppmFileName,info.width,info.height,thumbnail);

		delete ppmFileName;

		delete thumbnail;

		return true;
	}
	catch(klBigTiffError e)
	{
		if(thumbnail)
			delete thumbnail;
		std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
		ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
		std::string err = ANSI_INFO_ss.str();
		err+="Caught klBigTiffError in klBigTiffFileManager::getThumbnail(char* ofileName)";
		err+=e.what();
		logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());flushall();throw err; 


	}
	catch(...)
	{
		if(thumbnail)
			delete thumbnail;

		std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
		ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
		std::string err = ANSI_INFO_ss.str();
		err+="Caught a non klBigTiffError in  klBigTiffFileManager::getThumbnail(const char* ofileName)";
		logError( logFile, TEXT("\nklBigTiffFileManager\n") ,err.c_str());flushall();throw err; 
	}

	return false;
}

void klBigTiffFileManager::printMetadata()
{
	TiffImageInfo info = getTiffImageInfo(0);

	for(int i=0;i<info.tiffDirs;i++)
	{
		std::cout<<"------------------------- DIR "<<i<<"--------------------------- "<<std::endl;
		std::map<std::string,std::string> metadata= getMetadata(i);

		using namespace std;
		{
			map<string,string>::iterator position;
			int i=0;
			for(position=metadata.begin();position!=metadata.end();position++,i++)
			{
				pair<string, string> pos = *(position);
				cout<<pos.first<<"    " <<pos.second <<endl;

			}

		}

	}
}