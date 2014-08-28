/*******************************
 * Copyright (c) <2007>, <Bruce Campbell> All rights reserved. Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  *  
 * Bruce B Campbell 11 30 2012  *
 ********************************/



//The ordering of the includes SVSInterop.h and SVSReader.h is important due to the error below.
//		C:\Program Files\Microsoft SDKs\Windows\v6.0A\include\objidl.h(12364) : error C2872: 'BIND_OPTS' : ambiguous symbol
//        could be 'C:\Program Files\Microsoft SDKs\Windows\v6.0A\include\objidl.h(1891) : tagBIND_OPTS BIND_OPTS'
//        or       'c:\windows\microsoft.net\framework\v2.0.50727\mscorlib.dll : System::Runtime::InteropServices::BIND_OPTS'
//This occurs because because the standard Windows headers (usually included from <windows.h> ) have a load of
//#defines that clash with .NET.
//Since the reader is unsing dbghelp we acutally need the windef defines. 
#include "klBigTiffFileManager.h"
#include "klBigTiffManagedReader.h"

using namespace klBigTiff;

using namespace log4net;

void klBigTiffManagedReader::ImportImage(System::String ^filename, System::String^ basefilename)
{
	GetSubImages(filename, basefilename);

	GetImageTiles( filename,  basefilename);
	
	std::string bc = "";

	SVSGetMetadata(filename,basefilename);
}


String^ klBigTiffManagedReader::GetExposure(String^ fileName)
{
	_log->Debug("Calling klBigTiffManagedReader::getExposure on filename : "+ gcnew String(fileName) );
	bool isklBigTiff;
	try 
	{
		char* chars =  (char*)(Marshal::StringToHGlobalAnsi(fileName)).ToPointer();
		std::string ofileName="temp";
		std::string lfileName=chars;
		Marshal::FreeHGlobal(IntPtr(chars));
		klBigTiffFileManager svsmgr(lfileName.c_str() );
		std::map<std::string,std::string> metadata= svsmgr.getMetadata(0);
		
		std::string Exposure_Scale="";
		std::string Exposure_Time="";

		for(std::map<std::string, std::string>::const_iterator it = metadata.begin(); it != metadata.end(); it++)
		{
			std::string key = it->first.c_str();
			std::string val = it->second.c_str();
			std::cout<<key<<"   :   "<<val<<std::endl;
			if(key =="Exposure Scale")
				Exposure_Scale = val;
			if(key =="Exposure Time")
				Exposure_Time = val;
		}
		
		std::string exposure = Exposure_Time;

		return gcnew String(exposure.c_str());
	}
	catch(klBigTiffError e)
	{
		_log->Error("klBigTiffManagedReader::isklBigTiffImage recieved klBigTiffError  " +  gcnew String(e.what()) + " on  filename :"+gcnew String( fileName) );
		return gcnew String(e.what());
	}
	catch(...)
	{
		_log->Error("klBigTiffManagedReader::isklBigTiffImage recieved non klBigTiffError  " +  " on  filename :"+gcnew String( fileName) );
		throw "klBigTiffManagedReader::isklBigTiffImage recieved non klBigTiffError  " +  " on  filename :"+gcnew String( fileName);
	}
}
String^ klBigTiffManagedReader::GetImageExtents(System::String ^fileName)
{
	_log->Debug("Calling klBigTiffManagedReader::getExposure on filename : "+ gcnew String(fileName) );
	bool isklBigTiff;
	try 
	{
		char* chars =  (char*)(Marshal::StringToHGlobalAnsi(fileName)).ToPointer();
		std::string ofileName="temp";
		std::string lfileName=chars;
		Marshal::FreeHGlobal(IntPtr(chars));
		klBigTiffFileManager svsmgr(lfileName.c_str() );
		std::map<std::string,std::string> metadata= svsmgr.getMetadata(0);
		
		std::string width="";
		std::string height="";

		for(std::map<std::string, std::string>::const_iterator it = metadata.begin(); it != metadata.end(); it++)
		{
			std::string key = it->first.c_str();
			std::string val = it->second.c_str();
			std::cout<<key<<"   :   "<<val<<std::endl;
			if(key =="width")
				width = val;
			if(key =="height")
				height = val;
		}
		String^ w = gcnew String(width.c_str());
		String^ h = gcnew String(height.c_str());
		String^ extents = w+ "," + h;
		
		return extents;

	}
	catch(klBigTiffError e)
	{
		_log->Error("klBigTiffManagedReader::isklBigTiffImage recieved klBigTiffError  " +  gcnew String(e.what()) + " on  filename :"+gcnew String( fileName) );
	
	}
	catch(...)
	{
		_log->Error("klBigTiffManagedReader::isklBigTiffImage recieved non klBigTiffError  " +  " on  filename :"+gcnew String( fileName) );
		throw "klBigTiffManagedReader::isklBigTiffImage recieved non klBigTiffError  " +  " on  filename :"+gcnew String( fileName);
	}
}
bool klBigTiffManagedReader::isklBigTiffImage(String^ fileName)
{
	_log->Info("Calling klBigTiffManagedReader::isklBigTiffImage on filename : "+ gcnew String(fileName) );
	bool isklBigTiff;
	try 
	{
		char* chars =  (char*)(Marshal::StringToHGlobalAnsi(fileName)).ToPointer();
		std::string ofileName="temp";
		std::string lfileName=chars;
		Marshal::FreeHGlobal(IntPtr(chars));
		klBigTiffFileManager svsmgr(lfileName.c_str() );
		std::map<std::string,std::string> metadata= svsmgr.getMetadata(0);
		//Look through all of the tags for klBigTiff
		for(std::map<std::string, std::string>::const_iterator it = metadata.begin(); it != metadata.end(); it++)
		{
			std::string search = it->first.c_str();
			while (search.find("klBigTiff", 0) != std::string::npos)
			{
				size_t  pos = search.find("klBigTiff", 0); 
				_log->Debug("klBigTiffManagedReader::isklBigTiffImage returning true on filename: "+gcnew String(fileName) );
				return true;
			}
			search = it->second.c_str();

			while (search.find("klBigTiff", 0) != std::string::npos)
			{
				size_t  pos = search.find("klBigTiff", 0); 
				_log->Debug("klBigTiffManagedReader::isklBigTiffImage returning true on filename: "+gcnew String(fileName ) );
				return true;
			}
		}

		_log->Debug("klBigTiffManagedReader::isklBigTiffImage returning false on  filename :"+gcnew String( fileName) );
		return false;
	}
	catch(klBigTiffError e)
	{
		_log->Error("klBigTiffManagedReader::isklBigTiffImage recieved klBigTiffError  " +  gcnew String(e.what()) + " on  filename :"+gcnew String( fileName) );
		return false;
	}
	catch(...)
	{
		_log->Error("klBigTiffManagedReader::isklBigTiffImage recieved non klBigTiffError  " +  " on  filename :"+gcnew String( fileName) );
		return false;
	}
}

void klBigTiffManagedReader::GetImageTiles(String^ fileName, String^ baseFileName)
{
	try
	{
		char* chars =  ( char*)(Marshal::StringToHGlobalAnsi(fileName)).ToPointer();
		std::string lfileName=chars;

		klBigTiffFileManager svsmgr(lfileName.c_str());

		char* ofilepathchars =  ( char*)(Marshal::StringToHGlobalAnsi(baseFileName)).ToPointer();

		std::string ofilename;
		try
		{
			ofilename = std::string(ofilepathchars ) ;
			svsmgr.ExtractImageTiles(ofilename.c_str() );

		}
		catch(klBigTiffError e)
		{
			_log->Error("klBigTiffManagedReader::GetSubImages recieved klBigTiffError from  ExtractImageTiles" +  gcnew String(e.what()) + " on  filename :"+gcnew String( fileName) );

		}
		catch(...)
		{
			_log->Error("klBigTiffManagedReader::GetSubImages recieved non klBigTiffError  from ExtractImageTiles" +  " on  filename :"+gcnew String( fileName) );
		}

		Marshal::FreeHGlobal(IntPtr(chars));
		Marshal::FreeHGlobal(IntPtr(ofilepathchars));
	}

	catch(klBigTiffError e)
	{
		_log->Error("klBigTiffManagedReader::GetSubImages receved klBigTiffError  " +  gcnew String(e.what()) + " on  filename :"+gcnew String( fileName) );

	}
	catch(...)
	{
		_log->Error("klBigTiffManagedReader::GetSubImages receved non klBigTiffError  " +  " on  filename :"+gcnew String( fileName) );

	}	
}



void klBigTiffManagedReader::GetImageROI(String^ fileName, String^ baseFileName, int^ x0,int^ y0, int^ w,int^h)
{
	char* chars =  ( char*)(Marshal::StringToHGlobalAnsi(fileName)).ToPointer();
	std::string lfileName=chars;

	klBigTiffFileManager svsmgr(lfileName.c_str());
	char* ofilepathchars =  ( char*)(Marshal::StringToHGlobalAnsi(baseFileName)).ToPointer();

	std::string ofilename;
	ofilename = std::string(ofilepathchars );

	try
	{

			try
		{
			
			svsmgr.ExtractMainResTile(ofilename.c_str() ,"",*x0,*y0, *w,*h);
			_log->Info("klBigTiffManagedReader::GetImageROI extracted image data " +  " on  filename :"+gcnew String( fileName) +" at ROI " +x0->ToString() +" " +y0->ToString() +" " +w->ToString() +" " +h->ToString() +" " );

		}
		catch(klBigTiffError e)
		{
			_log->Error("klBigTiffManagedReader::GetImageROI recieved klBigTiffError from ExtractMainResTile " +  gcnew String(e.what()) + " on  filename :"+gcnew String( fileName) +" at ROI " +x0->ToString() +" " +y0->ToString() +" " +w->ToString() +" " +h->ToString() +" " );

			_log->Info("RETRY extract of image data " +  " on  filename :"+gcnew String( fileName) +" at ROI " +x0->ToString() +" " +y0->ToString() +" " +w->ToString() +" " +h->ToString() +" " );
			svsmgr.ExtractMainResTile(ofilename.c_str() ,"",*x0,*y0, *w-1,*h-1);

		}
		catch(...)
		{
			_log->Error("klBigTiffManagedReader::GetImageROI recieved non klBigTiffError  from ExtractMainResTile" +  " on  filename :"+gcnew String( fileName)+" at ROI " +x0->ToString() +" " +y0->ToString() +" " +w->ToString() +" " +h->ToString() +" " );

			_log->Info("RETRY extract of image data " +  " on  filename :"+gcnew String( fileName) +" at ROI " +x0->ToString() +" " +y0->ToString() +" " +w->ToString() +" " +h->ToString() +" " );
			svsmgr.ExtractMainResTile(ofilename.c_str() ,"",*x0,*y0, *w-1,*h-1);
		}

		Marshal::FreeHGlobal(IntPtr(chars));
		Marshal::FreeHGlobal(IntPtr(ofilepathchars));
	}

	catch(klBigTiffError e)
	{
		_log->Error("klBigTiffManagedReader::GetImageROI receved klBigTiffError  " +  gcnew String(e.what()) + " on  filename :"+gcnew String( fileName) +" at ROI " +x0->ToString() +" " +y0->ToString() +" " +w->ToString() +" " +h->ToString() +" ");

		_log->Info("RETRY extract of image data " +  " on  filename :"+gcnew String( fileName) +" at ROI " +x0->ToString() +" " +y0->ToString() +" " +w->ToString() +" " +h->ToString() +" " );
		svsmgr.ExtractMainResTile(ofilename.c_str() ,"",*x0,*y0, *w-1,*h-1);

	}
	catch(...)
	{
		_log->Error("klBigTiffManagedReader::GetImageROI receved non klBigTiffError  " +  " on  filename :"+gcnew String( fileName) +" at ROI " +x0->ToString() +" " +y0->ToString() +" " +w->ToString() +" " +h->ToString() +" ");

		_log->Info("RETRY extract of image data " +  " on  filename :"+gcnew String( fileName) +" at ROI " +x0->ToString() +" " +y0->ToString() +" " +w->ToString() +" " +h->ToString() +" " );
		svsmgr.ExtractMainResTile(ofilename.c_str() ,"",*x0,*y0, *w,*h);

	}	
}



System::Collections::Hashtable^  klBigTiffManagedReader::GetSubImages(String^ fileName, String^ baseFileName)
{

	_log->Debug("klBigTiffManagedReader::GetSubImages called with source file : "+gcnew String(fileName) +" and destination directory :"+ gcnew String(baseFileName) );

	System::Collections::Hashtable^ files = gcnew System::Collections::Hashtable();

	try
	{
		char* chars =  ( char*)(Marshal::StringToHGlobalAnsi(fileName)).ToPointer();
		std::string lfileName=chars;

		klBigTiffFileManager svsmgr(lfileName.c_str());

		char* ofilepathchars =  ( char*)(Marshal::StringToHGlobalAnsi(baseFileName)).ToPointer();

		std::string ofilename;
		std::string ofilename_thumb;
		std::string ofilename_label;
		std::string ofilename_macro;
		std::string ofilename_dir2;
		std::string ofilename_dir3;

		bool isThumb=false;
		bool isLabel=false;
		bool isMacro=false;
		bool isDir2=false;
		bool isDir3=false;
		
		try
		{
			//ofilename_thumb = std::string(ofilepathchars) + "_thumb.ppm";
			//_log->Debug("klBigTiffManagedReader::GetSubImages calling getThumbnail");
			//isThumb=svsmgr.getThumbnail(ofilename_thumb.c_str() );
			//files->Add("thumbnail", gcnew String(ofilename_thumb.c_str()));

			//ofilename_label = std::string(ofilepathchars) + "_label.ppm";
			//_log->Debug("klBigTiffManagedReader::GetSubImages calling getLabelImage");
			//isLabel =svsmgr.getLabelImage(ofilename_label.c_str() );

			//ofilename_macro = std::string(ofilepathchars) +  "_macro.ppm";
			//_log->Debug("klBigTiffManagedReader::GetSubImages calling getWSIImage");
			//isMacro= svsmgr.getWSIImage(ofilename_macro.c_str() );
			
			ofilename_dir2 = std::string(ofilepathchars) +  "_dir2.tif";
			_log->Debug("klBigTiffManagedReader::GetSubImages calling getTiffDirImage for dir 2");
			isDir2= svsmgr.getTiffDirImage(2, ofilename_dir2.c_str());
			files->Add("dir2",gcnew String(ofilename_dir2.c_str()));

						
			/*ofilename_dir3 = std::string(ofilepathchars) +  "_dir3.tif";
			_log->Debug("klBigTiffManagedReader::GetSubImages calling getTiffDirImage for dir 3");
			isDir3= svsmgr.getTiffDirImage(3, ofilename_dir3.c_str());*/

		}
		catch(klBigTiffError e)
		{
			_log->Error("klBigTiffManagedReader::GetSubImages recieved klBigTiffError while trying to get thumbnail, label or WSI " +  gcnew String(e.what()) + " on  filename :"+gcnew String( fileName) );

		}
		catch(...)
		{
			_log->Error("klBigTiffManagedReader::GetSubImages recieved non klBigTiffError while trying to get thumbnail, label or WSI" +  " on  filename :"+gcnew String( fileName) );

		}
		Marshal::FreeHGlobal(IntPtr(chars));
		Marshal::FreeHGlobal(IntPtr(ofilepathchars));

	}	

	catch(klBigTiffError e)
	{
		_log->Error("klBigTiffManagedReader::GetSubImages receved klBigTiffError  " +  gcnew String(e.what()) + " on  filename :"+gcnew String( fileName) );

	}
	catch(...)
	{
		_log->Error("klBigTiffManagedReader::GetSubImages receved non klBigTiffError  " +  " on  filename :"+gcnew String( fileName) );

	}	
	return files;
}


void klBigTiffManagedReader::SVSGetMetadata(String^ fileName, String^ baseFileName)
{
	_log->Debug("klBigTiffManagedReader::SVSGetMetadata called with source file : "+gcnew String(fileName) +" and destination directory :"+ gcnew String(baseFileName) );

	char* chars =  (char*)(Marshal::StringToHGlobalAnsi(fileName)).ToPointer();
	std::string lfileName=chars;
	try
	{
		klBigTiffFileManager svsmgr(lfileName.c_str() );

		char* ofilepathchars =  ( char*)(Marshal::StringToHGlobalAnsi(baseFileName)).ToPointer();

		std::string ofilename;

		XmlDocument^ xmlDoc = gcnew XmlDocument();
		// Write down the XML declaration
		XmlDeclaration^ xmlDeclaration = xmlDoc->CreateXmlDeclaration(gcnew String("1.0"),gcnew String("utf-8"),gcnew String(""));
		// Create the root element
		XmlElement^ rootNode  = xmlDoc->CreateElement(gcnew String("TiffImage"));
		xmlDoc->InsertBefore(xmlDeclaration, xmlDoc->DocumentElement); 
		xmlDoc->AppendChild(rootNode);

		unsigned int numTifDirs=svsmgr.getNumberOfTiffDirs();
		int i=0;
		for(i=numTifDirs-1;i>=0;i--)
		{
			// Create a new <Category> element and add it to the root node
			XmlElement^ parentNode  = xmlDoc->CreateElement(gcnew String("Dir") );
			_log->Debug("klBigTiffManagedReader::SVSGetMetadata calling getMetadata");
			std::map<std::string,std::string> metadata= svsmgr.getMetadata(i);
			// Set attribute name and value!
			parentNode->SetAttribute(gcnew String("ID"), gcnew String(metadata["currentDir"].c_str() ) );

			_log->Debug("klBigTiffManagedReader::SVSGetMetadata setting metadata "+ gcnew String("ID")+"   "+  gcnew String(metadata["currentDir"].c_str() )) ;

			xmlDoc->DocumentElement->PrependChild(parentNode);

			unsigned int mapSize = metadata.size();
			unsigned int j=0;

			for(std::map<std::string, std::string>::const_iterator it = metadata.begin(); it != metadata.end(); it++,j++)
			{
				std::cout << "key["<<j<<"] = " << it->first<<"\t\t\t\t\t   value = " << it->second << std::endl;
				std::wstring widestringKey(it->first.begin(), it->first.end());
				std::wstring widestringVal(it->second.begin(), it->second.end());

				try
				{
					String^ key =gcnew String(it->first.c_str());

					StringBuilder^ sb = gcnew StringBuilder(key);

					sb->Replace(" ", "_");

					XmlElement^ xmlNode  = xmlDoc->CreateElement(sb->ToString() );
					//XmlElement^ xmlNode  = xmlDoc->CreateElement(gcnew String(widestringKey.c_str() ));

					parentNode->AppendChild(xmlNode);
					xmlNode->AppendChild(xmlDoc->CreateTextNode (gcnew String(it->second.c_str() ) ) ) ;
					//xmlNode->AppendChild(xmlDoc->CreateTextNode (gcnew String(widestringVal.c_str() ) ) ) ;

					_log->Debug("klBigTiffManagedReader::SVSGetMetadata setting metadata "+ key + "   "+  gcnew String(it->second.c_str() ) ) ;


				}
				catch(...)
				{
					_log->Error("klBigTiffManagedReader::SVSGetMetadata receved non klBigTiffError  " +  " on  filename :"+gcnew String( fileName) );
				}
			}
			
		}

		// Save to the XML file
		ofilename = std::string(ofilepathchars) + "_meta.xml";
		xmlDoc->Save(gcnew String(ofilename.c_str() ) );

		Marshal::FreeHGlobal(IntPtr(chars));
		Marshal::FreeHGlobal(IntPtr(ofilepathchars));
	}

	catch(klBigTiffError e)
	{
		_log->Error("klBigTiffManagedReader::SVSGetMetadata receved klBigTiffError  " +  gcnew String(e.what()) + " on  filename :"+gcnew String( fileName) );
	}
	catch(...)
	{
		_log->Error("klBigTiffManagedReader::SVSGetMetadata receved non klBigTiffError  " +  " on  filename :"+gcnew String( fileName) );

	}	

}





