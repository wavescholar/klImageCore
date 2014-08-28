void dither_decroative(char* imgFile,char* decorativeFile,char* outputFile) ;
void qdah(char* motifFile, char* ditherFile,char* outputFile);
void multi_dither(char* inputFile, char* ditherFileList,char* sequenceFileList,char* outputFile);


int main(int argc, char *argv[]) 
{
	{
		char* inputFile = "img2.ppm";
		char* decFile1 =  "dec.ppm";
		char* decFile2 =  "dec2.ppm";
		char* decFile3 =  "dec3.ppm";

		char* motifTexture =   "motif.ppm";


		qdah(motifTexture, decFile1,"test.ppm");

		char* ditherFileList = "c:/temp/ditherFileList.txt";
		char* sequenceFileList = "c:/temp/sequenceFileList.txt";

		multi_dither( inputFile,  ditherFileList, sequenceFileList, "result_multi_dither.ppm");
	}

	{
		char* inputFile = "c:/temp/img2.ppm";
		char* decFile1 =  "c:/temp/dec.ppm";
		char* decFile2 =  "c:/temp/dec2.ppm";
		char* decFile3 =  "c:/temp/dec3.ppm";

		char* motifTexture =   "c:/temp/motif.ppm";

		char* ditherFileList = "c:/temp/ditherFileList2.txt";
		char* sequenceFileList = "c:/temp/sequenceFileList2.txt";

		multi_dither( inputFile,  ditherFileList, sequenceFileList, "result_multi_dither_2.ppm");
	}




}