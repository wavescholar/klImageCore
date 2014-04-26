 
#include "stdafx.h"

#include "jpeglib.h"

extern "C" void  moo();


extern "C" void jpeg_destroy_compress JPP((j_compress_ptr cinfo));


//int read_JPEG_file (char * filename);
//extern  JSAMPLE * image_buffer;	/* Points to large array of R,G,B-order data */
//int image_height =512 ;	/* Number of rows in image */
//int image_width=512;		/* Number of columns in image */
//

int main(int argc, char* argv[])
{
	moo();
	return 0;
}

