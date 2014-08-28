/*******************************
 * WSCMP [2003] - [2012] WSCMP  *  
 * Bruce B Campbell 11 30 2012  *
 ********************************/

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>

#include "kl_image_io.h"
#include "kl_tiff_image_io.h"

////////////////////////////////////////////////TIFF/////////////////////////////////////////////
#include "tiffio.h"                 
bool read_tiff (const char* filename,unsigned int width,unsigned int height,unsigned int bands, unsigned char * inputbuf)
{
	/*FILE * tifffile;

	tifffile = fopen (filename, "rb");

	if (!tifffile) 
	{
	std::stringstream ANSI_INFO_ss (std::stringstream::in | std::stringstream::out );
	ANSI_INFO_ss<<"ANSI COMPILE INFO: " <<__DATE__<<"     "<<__TIME__<<"   "<<__FILE__<<"   "<<__LINE__<<"       "<<std::endl;
	std::string err = ANSI_INFO_ss.str();
	err+="bool read_tiff(const char* filename,unsigned int width,unsigned int height,unsigned int bands, unsigned char * inputbuf) :  Could not open file for reading tiff image data";
	throw err;
	}*/

	TIFF* tif = TIFFOpen(filename, "r");
	if (tif) 
	{
		uint32 imagelength;
		tsize_t scanline;
		uint32 imageheight;
		unsigned char* buf;
		uint32 row;
		uint32 col;

		TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &imagelength);
		scanline = TIFFScanlineSize(tif);
		imageheight = scanline /3 ;
		buf =(unsigned char*) _TIFFmalloc(scanline);

		for (row = 0; row < imagelength; row++)
		{
			TIFFReadScanline(tif, buf, row);
			for (col = 0; col < scanline; col++)
					printf("%d ", buf[col]);

			printf("\n");
		}
		_TIFFfree(buf);
		TIFFClose(tif);
	}
	return true;

}
              
klRasterBufferPointer read_tiff (const char* filename)
{	
	TIFF* tif = TIFFOpen(filename, "r");
	if (tif) 
	{
		tsize_t scanline;
		unsigned char* buf;
		unsigned int width;
		unsigned int height;
		unsigned int bpp;

		TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);

		TIFFGetField(tif, TIFFTAG_IMAGEWIDTH,&width);	
	
		TIFFGetField(tif,TIFFTAG_BITSPERSAMPLE,&bpp);

		scanline = TIFFScanlineSize(tif);

		unsigned int widthTemp = scanline /3 ;
		buf =(unsigned char*) _TIFFmalloc(scanline);
		
		unsigned int inbands = 3;
		
		klRasterBufferPointer rbp= new  klPackedHeapRasterBuffer<unsigned char>(width,height,inbands);
		size_t outbands=rbp->numBands();
		size_t outwidth =rbp->width();
		size_t outheight=rbp->height(); 
		size_t outbandStride=rbp->bandStride();
		size_t outxStride =rbp->xStride();
		size_t outyStride= rbp->yStride();

		unsigned char* outbuf=rbp->buffer();

		unsigned int row;
		unsigned int col;
		for (row = 0; row < height; row++)
		{
			TIFFReadScanline(tif, buf, row);

			memcpy(outbuf+scanline*row ,buf, outyStride);

		}
		_TIFFfree(buf);
		TIFFClose(tif);
		return rbp;
	}
	else
	{
		return NULL;
	}	
}

bool query_tiff (const char* filename,unsigned int &width,unsigned int &height,unsigned int &bands)
{
	TIFF* tif = TIFFOpen(filename, "r");
	if (tif) 
	{		
		TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
  		TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
		TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &bands);
				
		TIFFClose(tif);
		return true;
	}
	else
	{
		return false;
	}

}

void writeTiffFile(const char* fileName,	klRasterBufferPointer buffer)
{
	uint32 width = buffer->width();
	uint32 height = buffer->height();

	uint32 bands = buffer->numBands();

	TIFF *out= TIFFOpen(fileName, "w"); 
	
	int sampleperpixel = bands;    

	unsigned char *image= buffer->buffer();
		
	TIFFSetField (out, TIFFTAG_IMAGEWIDTH, width);  // set the width of the image
	TIFFSetField(out, TIFFTAG_IMAGELENGTH, height);    // set the height of the image

	TIFFSetField(out, TIFFTAG_SAMPLESPERPIXEL, sampleperpixel);   // set number of channels per pixel
	TIFFSetField(out, TIFFTAG_BITSPERSAMPLE, 8);    // set the size of the channels

	//TIFFSetField(out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);    // set the origin of the image.
	TIFFSetField(out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField(out, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB); 
	
	tsize_t linebytes = sampleperpixel * width;     // length in memory of one row of pixel in the image. 

	unsigned char *buf = NULL;        // buffer used to store the row of pixel information for writing to file

	//    Allocating memory to store the pixels of current row
	if (TIFFScanlineSize(out)==linebytes)
		buf =(unsigned char *)_TIFFmalloc(linebytes);
	else
		buf = (unsigned char *)_TIFFmalloc(TIFFScanlineSize(out));

	// We set the strip size of the file to be size of one row of pixels
	TIFFSetField(out, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(out, width*sampleperpixel));

	//Now writing image to the file one strip at a time
	for (uint32 row = 0; row < height; row++)
	{
		
		memcpy(buf, &image[(row)*linebytes], linebytes);    
		if (TIFFWriteScanline(out, buf, row, 0) < 0)
			break;
	}
	
	(void) TIFFClose(out); 
	if (buf)
		_TIFFfree(buf);



}









