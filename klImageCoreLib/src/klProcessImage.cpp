
#include "klProcessImage.h"

using namespace klProcessImage;

using namespace log4net;

void Dithering(std::string fileName,std::string outputPath);

String^ klImageProcessor::Process(String^ fileName)
{
	_log->Debug("Calling klImageProcessor::Process on filename : "+ gcnew String(fileName) );

	try 
	{
		char* chars =  (char*)(Marshal::StringToHGlobalAnsi(fileName)).ToPointer();
		std::string ofileName="temp";
		std::string lfileName=chars;
		Marshal::FreeHGlobal(IntPtr(chars));
		
		Dithering(lfileName.c_str() ,"");

		return gcnew String("");

	}
	catch(...)
	{
		_log->Error("klImageProcessor::Process(String^ fileName)e recieved non klBigTiffError  " +  " on  filename :"+gcnew String( fileName) );
		throw "klImageProcessor::Process(String^ fileName) recieved Error  " +  " on  filename :"+gcnew String( fileName);
	}
}