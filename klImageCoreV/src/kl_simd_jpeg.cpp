
#include <stdio.h>
#include <tchar.h>
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

/* This file contains code to benchmark the JPEG SIMD library
   http://libjpeg-turbo.virtualgl.org/ 
*/

std::queue<wstring> ListDirectoryContents(const wchar_t *sDir)
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


class CTimer
{
public:
	enum units
	{
		sec, msec, usec
	};

	CTimer(void);
	virtual ~CTimer(void);

	void Init(void);
	void Start(void);
	void Stop(void);

	void RaisePriority();
	void LowerPriority();

	double GetTime(CTimer::units u = msec);

private:
	LARGE_INTEGER m_freq;
	LARGE_INTEGER m_t0;
	LARGE_INTEGER m_t1;
};

CTimer::CTimer(void)
{
	m_freq.QuadPart = 0;
	m_t0.QuadPart = 0;
	m_t1.QuadPart = 0;
	QueryPerformanceFrequency(&m_freq);
	return;
} 


CTimer::~CTimer(void)
{
	return;
} 


void CTimer::Init(void)
{

	QueryPerformanceFrequency(&m_freq);
	return;
} 


void CTimer::Start(void)
{
	QueryPerformanceCounter(&m_t0);

	return;
} 


void CTimer::Stop(void)
{
	QueryPerformanceCounter(&m_t1);
	return;
} 


void CTimer::RaisePriority(void)
{
	SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_TIME_CRITICAL);
	Sleep(0);

	return;
} // CTimer::RaisePriority()


void CTimer::LowerPriority(void)
{
	SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_NORMAL);

	return;
} // CTimer::LowerPriority()

double CTimer::GetTime(CTimer::units u)
{
	double t;

	t = (double)(m_t1.QuadPart - m_t0.QuadPart) / m_freq.QuadPart;

	return t;
}

struct klJPEGTurboImageStruct
{
	unsigned int width;
	unsigned int height;
	unsigned int bands;
	unsigned char* buffer;
};

extern "C" struct klJPEGTurboImageStruct read_JPEG_fileTurbo(const char * filename);

extern "C" void write_JPEG_fileTurbo (const char * filename, int quality,struct klJPEGTurboImageStruct klImageStruct);

int _tmain(int argc, _TCHAR* argv[])
{
	CTimer cmnTimer;

	cmnTimer.Init();

	wchar_t* dirName = argv[1];//L"D:\\testImages\\pt25x\\";

	std::queue<wstring> files  = ListDirectoryContents(dirName);

	std::queue<wstring> fileIDs  = ListDirectoryContentsFileNameShort(dirName);


	for(int i =0;i< files.size();i++)
	{
		wstring fileNamew = files.front();
		files.pop();

 		wstring fileID = fileIDs.front();
		fileIDs.pop();

		//Converts wide to std string using stl.
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
			
			int quality =100;
			struct klJPEGTurboImageStruct klImageStruct;

			double msec =0.0;
			try
			{
				cmnTimer.Start();

				//fileName = "D:\\testImages\\1x\\1561951106.jpg";
				string errTest2("1561951106"); //This file generates a buffer overrun!
				string errTest1(fileName);

				unsigned int found   = errTest1.find(errTest2);
				if( found !=std::string::npos)
				{
					cout<<"Decode, " <<fileIDc<<","<<"NaN"<<","<<"NaN"<<endl;
					cout<<"Encode, " <<fileIDc<<","<<"NaN"<<","<<"NaN"<<endl;

					continue;
				}

				FILE *pFile = NULL;
				fopen_s( &pFile, fileName, "rb" );
				fseek( pFile, 0, SEEK_END );
				unsigned char fileBytes =  ftell( pFile );
				fclose( pFile );

				if( fileBytes==0)
				{
					cout<<"Decode, " <<fileIDc<<","<<"0"<<","<<"NaN"<<endl;
					cout<<"Encode, " <<fileIDc<<","<<"0"<<","<<"NaN"<<endl;

					continue;
				}

				unsigned int loops =1;

				for(int i=0; i<loops;i++)
				{
					klImageStruct=read_JPEG_fileTurbo(fileName);
				}

				cmnTimer.Stop();

				msec = cmnTimer.GetTime(CTimer::msec);

				cout<<"Decode, " <<fileIDc<<","<<klImageStruct.width*klImageStruct.height*klImageStruct.bands<<","<<msec/loops<<endl;
			}
			catch(...)
			{
				cout<<"Decode, " <<fileIDc<<","<<klImageStruct.width*klImageStruct.height*klImageStruct.bands<<","<<"NaN"<<endl;
			}

			//write_ppm("out.ppm", klImageStruct.width,klImageStruct.height,klImageStruct.buffer);

			try
			{
				msec =0.0;
				cmnTimer.Start();

				unsigned int loops =1;

				for(int i=0; i<loops;i++)
				{
					write_JPEG_fileTurbo ("out.jpg",quality,klImageStruct);
				}
				cmnTimer.Stop();
				msec = cmnTimer.GetTime(CTimer::msec);
				cout<<"Encode, " <<fileIDc<<","<<klImageStruct.width*klImageStruct.height*klImageStruct.bands<<","<<msec/loops<<endl;
				free(klImageStruct.buffer);
			}
			catch(...)
			{
				cout<<"Encode, " <<fileIDc<<","<<klImageStruct.width*klImageStruct.height*klImageStruct.bands<<","<<"NaN"<<endl;

			}
		}
	}
	return 0;
}