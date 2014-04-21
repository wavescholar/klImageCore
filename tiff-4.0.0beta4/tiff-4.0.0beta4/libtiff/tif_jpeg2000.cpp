#include "tiffiop.h"
#ifdef JPEG2000_SUPPORT

#include "kdu_compressed.h"
#include "jp2.h"
#include "kdu_region_compositor.h"
#include "kdu_region_decompressor.h"
#include "kdu_stripe_decompressor.h"

#define CodecState(TIF) ((JPEG2000CodecState *)(TIF->tif_data))
typedef struct {
	kdu_byte * Y;
	kdu_byte * Cb;
	kdu_byte * Cr;
	int tileWidth;
	int tileHeight;
	kdu_codestream * codestream;
	kdu_stripe_decompressor * decoder;
} JPEG2000CodecState;
JPEG2000CodecState jp2k_state;

class mem_source:public kdu_compressed_source {
	kdu_long cur_pos;
	int data_size;
	unsigned char * data;
public:
	mem_source(unsigned char * buf, int size) {
		data = buf;
		data_size = size;
		cur_pos = 0;
	}
	int get_capabilities() { return KDU_SOURCE_CAP_SEQUENTIAL | KDU_SOURCE_CAP_SEEKABLE; }
	int read(kdu_byte * buf, int num_bytes) {
		int copy_bytes = 0;
		if(cur_pos >= data_size) {
			return 0;
		}
		copy_bytes = ((data_size - cur_pos) > num_bytes ? num_bytes : (data_size - cur_pos));
		if(copy_bytes > 0) {
			memcpy(buf, &data[cur_pos], copy_bytes);
			cur_pos += num_bytes;
		}
		return(copy_bytes);
		
	}
	bool seek(kdu_long offset) { cur_pos += offset; return true;}
	kdu_long get_pos() { return cur_pos;}
	bool close() {return true;}

};

static int JP2KSetupDecode(TIFF* tif) 
{	

	int tileWidth, tileHeight;
	TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tileWidth);
	TIFFGetField(tif, TIFFTAG_TILELENGTH, &tileHeight);

	CodecState(tif)->tileWidth = tileWidth;
	CodecState(tif)->tileHeight = tileHeight;

	CodecState(tif)->Y = new kdu_byte[tileWidth*tileHeight];
	CodecState(tif)->Cb = new kdu_byte[(tileWidth >> 1)*tileHeight];
	CodecState(tif)->Cr = new kdu_byte[(tileWidth >> 1)*tileHeight];

	if(CodecState(tif)->decoder == NULL)
		CodecState(tif)->decoder = new kdu_stripe_decompressor();
	if(CodecState(tif)->codestream == NULL)
		CodecState(tif)->codestream = new kdu_codestream();
	return(1);
}




static int
JP2KPreDecode(TIFF* tif, tsample_t s) {
	return(1);
}

static int
JP2KDecodeRow(TIFF* tif, tidata_t buf, tsize_t cc, tsample_t s) 
{	
	printf("Decode row");

	return(1);
}

static int
JP2KDecodeStrip(TIFF* tif, tidata_t buf, tsize_t cc, tsample_t s) 

{	
	printf("Decode strip: cc size:%d raw cc:%d\n", cc, tif->tif_rawcc);
	return(1);
}





unsigned char clamp(float f) {
	if(f<0) return 0;
	if(f>255) return 255;
	return (unsigned char)f;
}

unsigned short clamp16(float f) {
	if(f<0) return 0;
	if(f>65535) return 65535;
	return (unsigned short)f;
}


// This one does it's own color space mapping
static int JP2KDecodeTile(TIFF* tif, tidata_t buf, tsize_t cc, tsample_t s) {	
	mem_source m(tif->tif_rawcp, tif->tif_rawcc);

	kdu_stripe_decompressor * decode = CodecState(tif)->decoder;
	kdu_codestream * codestream = CodecState(tif)->codestream;

	int i, j;
	int iY, iCb, iCr;
	float fR, fG, fB;
	kdu_byte * Y, * Cb, * Cr;
	int tileW, tileH;
	Y = CodecState(tif)->Y;
	Cb = CodecState(tif)->Cb;
	Cr = CodecState(tif)->Cr;
	
	tileW = CodecState(tif)->tileWidth;
	tileH = CodecState(tif)->tileHeight;

	kdu_byte * stripe_bufs[] = {Y, Cb, Cr};
	int stripe_heights[] = {tileH,tileH,tileH};
	int sample_gaps[] = {1,1,1};
	int row_gaps[] = {tileW, tileW>>1,tileW>>1};

	int precisions[]={16};

	//////////////////////Begin code to enable multithreaded decoding
	kdu_thread_env kdu_te;
	kdu_te.create();
	SYSTEM_INFO siSysInfo;
	GetSystemInfo(&siSysInfo); 
    unsigned int numProcessors=siSysInfo.dwNumberOfProcessors;
	unsigned int npj=0;
	for(npj=0;npj<numProcessors-1;npj++)
	{
		kdu_te.add_thread();
	}
	codestream->create(&m,&kdu_te);
	//////////////////////End code to enable multithreaded decoding

	decode->start(*codestream);
	while(decode->pull_stripe(stripe_bufs, stripe_heights, sample_gaps, row_gaps));
	
	decode->finish();
	
	codestream->destroy();

	// Convert to RGB and stupidly upsample Cb and Cr
	j=0; 
	iY = 0;

	for (i=0; i<(tileW>>1)*tileW; i++) 
	{
		fR = (298.082f * Y[iY] + 408.583f*Cr[i])/256-222.921f;
		fG = ( 298.082f * Y[iY] - 100.291f * Cb[i] - 208.120f * Cr[i] ) / 256 + 135.576f;
		fB = ( 298.082f * Y[iY] + 516.412f * Cb[i]                ) / 256 - 276.836f;

		buf[j++] = clamp(fR);
		buf[j++] = clamp(fG);
		buf[j++] = clamp(fB);
		iY++;

		fR = (298.082f * Y[iY] + 408.583f*Cr[i])/256-222.921f;
		fG = ( 298.082f * Y[iY] - 100.291f * Cb[i] - 208.120f * Cr[i] ) / 256 + 135.576f;
		fB = ( 298.082f * Y[iY] + 516.412f * Cb[i]                ) / 256 - 276.836f;

		buf[j++] = clamp(fR);
		buf[j++] = clamp(fG);
		buf[j++] = clamp(fB);
		iY++;
	}
	return(1);
}


static int
JP2KSetupEncode(TIFF * tif) {
	return(1);
}

static int
JP2KPreEncode(TIFF* tif, tsample_t s) {
	return(1);
}

static int
JP2KEncode(TIFF* tif, tidata_t buf, tsize_t cc, tsample_t s) {
	printf("Encode - Not implemented");
	return(1);
}

static int
JP2KPostEncode(TIFF* tif) {
	return(1);
}

static void JP2KCleanup(TIFF* tif) {
	if(CodecState(tif)->codestream != NULL) {
		delete CodecState(tif)->codestream;
		CodecState(tif)->codestream = NULL;
	}
	if(CodecState(tif)->decoder != NULL) {
		delete CodecState(tif)->decoder;
		CodecState(tif)->decoder = NULL;
	}

	if(CodecState(tif)->Y != NULL ) {
		delete [] CodecState(tif)->Y;
		CodecState(tif)->Y = NULL; 
	}
	
	if(CodecState(tif)->Cb != NULL ) {
		delete [] CodecState(tif)->Cb;
		CodecState(tif)->Cb = NULL; 
	}
	
	if(CodecState(tif)->Cr != NULL ) {
		delete [] CodecState(tif)->Cr;
		CodecState(tif)->Cr = NULL; 
	}

}

int TIFFInitJPEG2000(TIFF* tif, int scheme)
{
	assert(scheme == COMPRESSION_JPEG2000 );
	
	tif->tif_data = (tidata_t)&jp2k_state;

	CodecState(tif)->Y = NULL;
	CodecState(tif)->Cb = NULL;
	CodecState(tif)->Cr = NULL;

	CodecState(tif)->tileWidth = 1;
	CodecState(tif)->tileHeight = 1;

	CodecState(tif)->decoder = NULL;
	CodecState(tif)->codestream = NULL;


	tif->tif_setupdecode = JP2KSetupDecode;
	tif->tif_predecode = JP2KPreDecode;
	tif->tif_decoderow = JP2KDecodeRow;
	tif->tif_decodestrip = JP2KDecodeStrip;
	tif->tif_decodetile = JP2KDecodeTile;
	tif->tif_setupencode = JP2KSetupEncode;
	tif->tif_preencode = JP2KPreEncode;
	tif->tif_postencode = JP2KPostEncode;
	tif->tif_encoderow = JP2KEncode;
	tif->tif_encodestrip = JP2KEncode;
	tif->tif_encodetile = JP2KEncode;
	tif->tif_cleanup = JP2KCleanup;

	return (1);
bad:
	TIFFErrorExt(tif->tif_clientdata, "TIFFInitJPEG2000", 
		     "No space for JPEG2000 state block");
	return (0);
}



static int JP2KSetupDecodeNSS(TIFF* tif) 
{	

	int tileWidth, tileHeight;
	TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tileWidth);
	TIFFGetField(tif, TIFFTAG_TILELENGTH, &tileHeight);

	CodecState(tif)->tileWidth = tileWidth;
	CodecState(tif)->tileHeight = tileHeight;

	CodecState(tif)->Y = new kdu_byte[tileWidth*tileHeight];
	CodecState(tif)->Cb = new kdu_byte[(tileWidth)*tileHeight];
	CodecState(tif)->Cr = new kdu_byte[(tileWidth)*tileHeight];

	if(CodecState(tif)->decoder == NULL)
		CodecState(tif)->decoder = new kdu_stripe_decompressor();
	if(CodecState(tif)->codestream == NULL)
		CodecState(tif)->codestream = new kdu_codestream();
	return(1);
}

static int JP2KDecodeTileNSS(TIFF* tif, tidata_t buf, tsize_t cc, tsample_t s) {	
	mem_source m(tif->tif_rawcp, tif->tif_rawcc);

	kdu_stripe_decompressor * decode = CodecState(tif)->decoder;
	kdu_codestream * codestream = CodecState(tif)->codestream;

	unsigned int bitdepth = tif->tif_dir.td_bitspersample;
	unsigned int bands = tif->tif_dir.td_samplesperpixel;

	int i, j;
	int iY, iCb, iCr;
	float fR, fG, fB;
	kdu_byte * Y, * Cb, * Cr;
	int tileW, tileH;
	Y = CodecState(tif)->Y;
	Cb = CodecState(tif)->Cb;
	Cr = CodecState(tif)->Cr;
	
	tileW = CodecState(tif)->tileWidth;
	tileH = CodecState(tif)->tileHeight;

	if(bands ==1)
	{

	kdu_byte * stripe_bufs[] = {Y};

	int stripe_heights[] = {tileH};
	int sample_gaps[] = {1};
	int row_gaps[] = {tileW };
	int precisions[]={16};
	
	//////////////////////Begin code to enable multithreaded decoding
	kdu_thread_env kdu_te;
	kdu_te.create();
	SYSTEM_INFO siSysInfo;
	GetSystemInfo(&siSysInfo); 
    unsigned int numProcessors=siSysInfo.dwNumberOfProcessors;
	unsigned int npj=0;
	for(npj=0;npj<numProcessors-1;npj++)
	{
		kdu_te.add_thread();
	}
	codestream->create(&m,&kdu_te);
	//////////////////////End code to enable multithreaded decoding
	
	decode->start(*codestream);

	while(decode->pull_stripe(stripe_bufs, stripe_heights, sample_gaps, row_gaps,precisions));
	
	decode->finish();
	
	codestream->destroy();

	j=0; 
	iY = 0;
	
	for (i=0; i<tileW*tileW; i++) 
	{
		fR = Y[iY]; 
		buf[j++]   =clamp16(fR);
		buf[j++]   =clamp16(fR);
		iY++;
	}
	
	return(1);
	}
	else
	{
		kdu_byte * stripe_bufs[] = {Y, Cb, Cr};
	int stripe_heights[] = {tileH,tileH,tileH};
	int sample_gaps[] = {1,1,1};
	int row_gaps[] = {tileW, tileW,tileW};
	
	//////////////////////Begin code to enable multithreaded decoding
	kdu_thread_env kdu_te;
	kdu_te.create();
	SYSTEM_INFO siSysInfo;
	GetSystemInfo(&siSysInfo); 
    unsigned int numProcessors=siSysInfo.dwNumberOfProcessors;
	unsigned int npj=0;
	for(npj=0;npj<numProcessors-1;npj++)
	{
		kdu_te.add_thread();
	}
	codestream->create(&m,&kdu_te);
	//////////////////////End code to enable multithreaded decoding
	
	decode->start(*codestream);

	while(decode->pull_stripe(stripe_bufs, stripe_heights, sample_gaps, row_gaps));
	
	decode->finish();
	
	codestream->destroy();

	j=0; 
	iY = 0;
	for (i=0; i<tileW*tileW; i++) 
	{
		fR = Y[iY]; 
		fG = Cb[i]; 
		fB = Cr[i];

		buf[j++] = clamp(fR);
		buf[j++] = clamp(fG);
		buf[j++] = clamp(fB);
		iY++;
	}

	return(1);
	}
}

int TIFFInitJPEG2000NSS(TIFF* tif, int scheme)
{
	assert( scheme == COMPRESSION_JPEG2000_NSS );

	tif->tif_data = (tidata_t)&jp2k_state;

	CodecState(tif)->Y = NULL;
	CodecState(tif)->Cb = NULL;
	CodecState(tif)->Cr = NULL;

	CodecState(tif)->tileWidth = 1;
	CodecState(tif)->tileHeight = 1;

	CodecState(tif)->decoder = NULL;
	CodecState(tif)->codestream = NULL;

	tif->tif_setupdecode = JP2KSetupDecodeNSS;

	tif->tif_predecode = JP2KPreDecode;
	tif->tif_decoderow = JP2KDecodeRow;
	tif->tif_decodestrip = JP2KDecodeStrip;
	tif->tif_decodetile = JP2KDecodeTileNSS;
	tif->tif_setupencode = JP2KSetupEncode;
	tif->tif_preencode = JP2KPreEncode;
	tif->tif_postencode = JP2KPostEncode;
	tif->tif_encoderow = JP2KEncode;
	tif->tif_encodestrip = JP2KEncode;
	tif->tif_encodetile = JP2KEncode;
	tif->tif_cleanup = JP2KCleanup;

	return (1);
bad:
	TIFFErrorExt(tif->tif_clientdata, "TIFFInitJPEG2000", 
		     "No space for JPEG2000 state block");
	return (0);
}

#endif
