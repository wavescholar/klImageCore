// TestUIC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <windows.h>

#include <iostream>
#include <list>
#include <queue>
#include <string>
#include <iostream>
#include <clocale>
#include <locale>
#include <vector>
using namespace std;

std::queue<wstring> ListDirectoryContents(const wchar_t *sDir)
{ 
	std::queue<wstring> fileQueue;

	WIN32_FIND_DATA fdFile; 
	HANDLE hFind = NULL; 

	wchar_t sPath[2048]; 

	//Specify a file mask. *.* = We want everything! 
	wsprintf(sPath, L"%s\\*.*", sDir); 

	if((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE) 
	{ 
		wprintf(L"Path not found: [%s]\n", sDir); 
		return fileQueue; 
	} 
	do
	{
		if(wcscmp(fdFile.cFileName, L".") != 0
			&& wcscmp(fdFile.cFileName, L"..") != 0) 
		{ 
			wsprintf(sPath, L"%s\\%s", sDir, fdFile.cFileName); 
			{ 
				wstring fileName(sPath);
				fileQueue.push(fileName);

			} 
		}
	} 
	while(FindNextFile(hFind, &fdFile)); //Find the next file. 

	FindClose(hFind); //Always, Always, clean things up! 

	return fileQueue; 
} 

std::queue<wstring> ListDirectoryContentsFileNameShort(const wchar_t *sDir)
{ 
	std::queue<wstring> fileQueue;

	WIN32_FIND_DATA fdFile; 
	HANDLE hFind = NULL; 

	wchar_t sPath[2048]; 

	//Specify a file mask. *.* = We want everything! 
	wsprintf(sPath, L"%s\\*.jpg", sDir); 

	if((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE) 
	{ 
		wprintf(L"Path not found: [%s]\n", sDir); 
		return fileQueue; 
	} 
	do
	{
		if(wcscmp(fdFile.cFileName, L".") != 0
			&& wcscmp(fdFile.cFileName, L"..") != 0) 
		{
			wstring fileName(fdFile.cFileName);
			unsigned int found = fileName.find(L".");
			wstring fnameShort = fileName.substr(0,found);
			fileQueue.push(fnameShort);

		} 
	}

	while(FindNextFile(hFind, &fdFile)); //Find the next file. 

	FindClose(hFind); //Always, Always, clean things up! 

	return fileQueue; 
}


int runJPEG(const char* fileName,bool inout,const char* fileID);
int _tmain(int argc, _TCHAR* argv[])
{
	wchar_t* dirName = L"D:\\testImages";

	std::queue<wstring> files  = ListDirectoryContents(dirName);
	
	std::queue<wstring> fileIDs  = ListDirectoryContentsFileNameShort(dirName);
	
	for(int i =0;i< files.size();i++)
	{
		wstring fileNamew = files.front();
		files.pop();
		
		wstring fileID = fileIDs.front();
		fileIDs.pop();

		//Convert wide to std string using stl.
		std::setlocale(LC_ALL, "");
		const std::wstring ws = fileNamew;
		const std::locale locale("");
		typedef std::codecvt<wchar_t, char, std::mbstate_t> converter_type;
		const converter_type& converter = std::use_facet<converter_type>(locale);
		std::vector<char> to(ws.length() * converter.max_length());
		std::mbstate_t state;
		const wchar_t* from_next;
		char* to_next;
		const converter_type::result result = converter.out(state, ws.data(), ws.data() + ws.length(), from_next, &to[0], &to[0] + to.size(), to_next);
		if (result == converter_type::ok || result == converter_type::noconv) 
		{
			const std::string s(&to[0], to_next);
			const char* fileName =s.c_str();


			//Convert the fileID
			const std::wstring wsID = fileID;
			std::vector<char> to(wsID.length() * converter.max_length());
			const converter_type::result resultID = converter.out(state, wsID.data(), wsID.data() + wsID.length(), from_next, &to[0], &to[0] + to.size(), to_next);
			const std::string sID(&to[0], to_next);
			const char* fileIDc =sID.c_str();


			FILE *pFile = NULL;
			fopen_s( &pFile, fileName, "rb" );
			fseek( pFile, 0, SEEK_END );
			unsigned char fileBytes =  ftell( pFile );
			fclose( pFile );
							
			if( fileBytes==0)
			{
				cout<<"Decode, " <<fileID.c_str()<<","<<"0"<<","<<"NaN"<<endl;
				cout<<"Encode, " <<fileID.c_str()<<","<<"0"<<","<<"NaN"<<endl;

				continue;
			}			
			
			runJPEG(fileName,true,fileIDc); 
		}
	}
	return 0;
}

/*******************************
* WSCMP [2003] - [2012] WSCMP  *  
* Bruce B Campbell 11 30 2012  *
********************************/
#include "kl_image_processing_functors.h"
#include "ppm_helper.h"
#include "kl_img_pca.h"
#include "kl_image_io.h"
#include "kl_image_io_factory.h"

void TestJPEG(string fileName,string outputPath)
{	
	klImageFileSource* klifs= klImageFileSourceFactory::getFileSource(fileName.c_str());

	bool ansOK = klifs->queryImage();

	klRasterBufferPointer klrbp = klifs->operator()();

	klJPEGSourceFunctor kljfs(fileName.c_str());

	klRasterBufferPointer lsrc  =kljfs();

	/*
	klTIFFSinkFunctor kltsf("out.tif",lsrc);
	kltfs();
	kltsf.render();

	klPPMSinkFunctor klpsf("out.pppm",lsrc);
	klpsf();
	klpsf.render();	

	klImageFileSource* klifs = klImageFileSourceFactory::getFileSource(fileName.c_str() );
	klRasterBufferPointer lsrcfact=   klifs->render();
	*/	

}
