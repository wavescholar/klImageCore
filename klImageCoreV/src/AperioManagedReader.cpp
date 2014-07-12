/*******************************
 * WSCMP [2003] - [2012] WSCMP  *  
 * Bruce B Campbell 11 30 2012  *
 ********************************/



//The ordering of the includes SVSInterop.h and SVSReader.h is important due to the error below.
//		C:\Program Files\Microsoft SDKs\Windows\v6.0A\include\objidl.h(12364) : error C2872: 'BIND_OPTS' : ambiguous symbol
//        could be 'C:\Program Files\Microsoft SDKs\Windows\v6.0A\include\objidl.h(1891) : tagBIND_OPTS BIND_OPTS'
//        or       'c:\windows\microsoft.net\framework\v2.0.50727\mscorlib.dll : System::Runtime::InteropServices::BIND_OPTS'
//This occurs because because the standard Windows headers (usually included from <windows.h> ) have a load of
//#defines that clash with .NET.
//Since the reader is unsing dbghelp we acutally need the windef defines. 
#include "AperioFileManager.h"
#include "AperioManagedReader.h"

using namespace AperioManagedReader;

using namespace log4net;

void BigTiffDotNetReader::ImportImage(System::String ^filename, System::String^ basefilename)
{
	//GetSubImages(filename, basefilename);

	GetImageTiles( filename,  basefilename);
	
	std::string bc = "";

	SVSGetMetadata(filename,basefilename);
}


String^ BigTiffDotNetReader::GetExposure(String^ fileName)
{
	_log->Debug("Calling BigTiffDotNetReader::getExposure on filename : "+ gcnew String(fileName) );
	bool isAperio;
	try 
	{
		char* chars =  (char*)(Marshal::StringToHGlobalAnsi(fileName)).ToPointer();
		std::string ofileName="temp";
		std::string lfileName=chars;
		Marshal::FreeHGlobal(IntPtr(chars));
		AperioFileManager svsmgr(lfileName.c_str() );
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
	catch(SVSTiffError e)
	{
		_log->Error("BigTiffDotNetReader::isAperioImage recieved SVSTiffError  " +  gcnew String(e.what()) + " on  filename :"+gcnew String( fileName) );
		return gcnew String(e.what());
	}
	catch(...)
	{
		_log->Error("BigTiffDotNetReader::isAperioImage recieved non SVSTiffError  " +  " on  filename :"+gcnew String( fileName) );
		throw "BigTiffDotNetReader::isAperioImage recieved non SVSTiffError  " +  " on  filename :"+gcnew String( fileName);
	}
}
String^ BigTiffDotNetReader::GetImageExtents(System::String ^fileName)
{
	_log->Debug("Calling BigTiffDotNetReader::getExposure on filename : "+ gcnew String(fileName) );
	bool isAperio;
	try 
	{
		char* chars =  (char*)(Marshal::StringToHGlobalAnsi(fileName)).ToPointer();
		std::string ofileName="temp";
		std::string lfileName=chars;
		Marshal::FreeHGlobal(IntPtr(chars));
		AperioFileManager svsmgr(lfileName.c_str() );
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
	catch(SVSTiffError e)
	{
		_log->Error("BigTiffDotNetReader::isAperioImage recieved SVSTiffError  " +  gcnew String(e.what()) + " on  filename :"+gcnew String( fileName) );
	
	}
	catch(...)
	{
		_log->Error("BigTiffDotNetReader::isAperioImage recieved non SVSTiffError  " +  " on  filename :"+gcnew String( fileName) );
		throw "BigTiffDotNetReader::isAperioImage recieved non SVSTiffError  " +  " on  filename :"+gcnew String( fileName);
	}
}
bool BigTiffDotNetReader::isAperioImage(String^ fileName)
{
	_log->Info("Calling BigTiffDotNetReader::isAperioImage on filename : "+ gcnew String(fileName) );
	bool isAperio;
	try 
	{
		char* chars =  (char*)(Marshal::StringToHGlobalAnsi(fileName)).ToPointer();
		std::string ofileName="temp";
		std::string lfileName=chars;
		Marshal::FreeHGlobal(IntPtr(chars));
		AperioFileManager svsmgr(lfileName.c_str() );
		std::map<std::string,std::string> metadata= svsmgr.getMetadata(0);
		//Look through all of the tags for Aperio
		for(std::map<std::string, std::string>::const_iterator it = metadata.begin(); it != metadata.end(); it++)
		{
			std::string search = it->first.c_str();
			while (search.find("Aperio", 0) != std::string::npos)
			{
				size_t  pos = search.find("Aperio", 0); 
				_log->Debug("BigTiffDotNetReader::isAperioImage returning true on filename: "+gcnew String(fileName) );
				return true;
			}
			search = it->second.c_str();

			while (search.find("Aperio", 0) != std::string::npos)
			{
				size_t  pos = search.find("Aperio", 0); 
				_log->Debug("BigTiffDotNetReader::isAperioImage returning true on filename: "+gcnew String(fileName ) );
				return true;
			}
		}

		_log->Debug("BigTiffDotNetReader::isAperioImage returning false on  filename :"+gcnew String( fileName) );
		return false;
	}
	catch(SVSTiffError e)
	{
		_log->Error("BigTiffDotNetReader::isAperioImage recieved SVSTiffError  " +  gcnew String(e.what()) + " on  filename :"+gcnew String( fileName) );
		return false;
	}
	catch(...)
	{
		_log->Error("BigTiffDotNetReader::isAperioImage recieved non SVSTiffError  " +  " on  filename :"+gcnew String( fileName) );
		return false;
	}
}

void BigTiffDotNetReader::GetImageTiles(String^ fileName, String^ baseFileName)
{
	try
	{
		char* chars =  ( char*)(Marshal::StringToHGlobalAnsi(fileName)).ToPointer();
		std::string lfileName=chars;

		AperioFileManager svsmgr(lfileName.c_str());

		char* ofilepathchars =  ( char*)(Marshal::StringToHGlobalAnsi(baseFileName)).ToPointer();

		std::string ofilename;
		try
		{
			ofilename = std::string(ofilepathchars ) ;
			svsmgr.ExtractImageTiles(ofilename.c_str() );

		}
		catch(SVSTiffError e)
		{
			_log->Error("BigTiffDotNetReader::GetSubImages recieved SVSTiffError from  ExtractImageTiles" +  gcnew String(e.what()) + " on  filename :"+gcnew String( fileName) );

		}
		catch(...)
		{
			_log->Error("BigTiffDotNetReader::GetSubImages recieved non SVSTiffError  from ExtractImageTiles" +  " on  filename :"+gcnew String( fileName) );
		}

		Marshal::FreeHGlobal(IntPtr(chars));
		Marshal::FreeHGlobal(IntPtr(ofilepathchars));
	}

	catch(SVSTiffError e)
	{
		_log->Error("BigTiffDotNetReader::GetSubImages receved SVSTiffError  " +  gcnew String(e.what()) + " on  filename :"+gcnew String( fileName) );

	}
	catch(...)
	{
		_log->Error("BigTiffDotNetReader::GetSubImages receved non SVSTiffError  " +  " on  filename :"+gcnew String( fileName) );

	}	
}



void BigTiffDotNetReader::GetImageROI(String^ fileName, String^ baseFileName, int^ x0,int^ y0, int^ w,int^h)
{
	char* chars =  ( char*)(Marshal::StringToHGlobalAnsi(fileName)).ToPointer();
	std::string lfileName=chars;

	AperioFileManager svsmgr(lfileName.c_str());
	char* ofilepathchars =  ( char*)(Marshal::StringToHGlobalAnsi(baseFileName)).ToPointer();

	std::string ofilename;
	ofilename = std::string(ofilepathchars );

	try
	{

			try
		{
			
			svsmgr.ExtractMainResTile(ofilename.c_str() ,"",*x0,*y0, *w,*h);
			_log->Info("BigTiffDotNetReader::GetImageROI extracted image data " +  " on  filename :"+gcnew String( fileName) +" at ROI " +x0->ToString() +" " +y0->ToString() +" " +w->ToString() +" " +h->ToString() +" " );

		}
		catch(SVSTiffError e)
		{
			_log->Error("BigTiffDotNetReader::GetImageROI recieved SVSTiffError from ExtractMainResTile " +  gcnew String(e.what()) + " on  filename :"+gcnew String( fileName) +" at ROI " +x0->ToString() +" " +y0->ToString() +" " +w->ToString() +" " +h->ToString() +" " );

			_log->Info("RETRY extract of image data " +  " on  filename :"+gcnew String( fileName) +" at ROI " +x0->ToString() +" " +y0->ToString() +" " +w->ToString() +" " +h->ToString() +" " );
			svsmgr.ExtractMainResTile(ofilename.c_str() ,"",*x0,*y0, *w-1,*h-1);

		}
		catch(...)
		{
			_log->Error("BigTiffDotNetReader::GetImageROI recieved non SVSTiffError  from ExtractMainResTile" +  " on  filename :"+gcnew String( fileName)+" at ROI " +x0->ToString() +" " +y0->ToString() +" " +w->ToString() +" " +h->ToString() +" " );

			_log->Info("RETRY extract of image data " +  " on  filename :"+gcnew String( fileName) +" at ROI " +x0->ToString() +" " +y0->ToString() +" " +w->ToString() +" " +h->ToString() +" " );
			svsmgr.ExtractMainResTile(ofilename.c_str() ,"",*x0,*y0, *w-1,*h-1);
		}

		Marshal::FreeHGlobal(IntPtr(chars));
		Marshal::FreeHGlobal(IntPtr(ofilepathchars));
	}

	catch(SVSTiffError e)
	{
		_log->Error("BigTiffDotNetReader::GetImageROI receved SVSTiffError  " +  gcnew String(e.what()) + " on  filename :"+gcnew String( fileName) +" at ROI " +x0->ToString() +" " +y0->ToString() +" " +w->ToString() +" " +h->ToString() +" ");

		_log->Info("RETRY extract of image data " +  " on  filename :"+gcnew String( fileName) +" at ROI " +x0->ToString() +" " +y0->ToString() +" " +w->ToString() +" " +h->ToString() +" " );
		svsmgr.ExtractMainResTile(ofilename.c_str() ,"",*x0,*y0, *w-1,*h-1);

	}
	catch(...)
	{
		_log->Error("BigTiffDotNetReader::GetImageROI receved non SVSTiffError  " +  " on  filename :"+gcnew String( fileName) +" at ROI " +x0->ToString() +" " +y0->ToString() +" " +w->ToString() +" " +h->ToString() +" ");

		_log->Info("RETRY extract of image data " +  " on  filename :"+gcnew String( fileName) +" at ROI " +x0->ToString() +" " +y0->ToString() +" " +w->ToString() +" " +h->ToString() +" " );
		svsmgr.ExtractMainResTile(ofilename.c_str() ,"",*x0,*y0, *w,*h);

	}	
}



System::Collections::Hashtable^  BigTiffDotNetReader::GetSubImages(String^ fileName, String^ baseFileName)
{

	_log->Debug("BigTiffDotNetReader::GetSubImages called with source file : "+gcnew String(fileName) +" and destination directory :"+ gcnew String(baseFileName) );

	System::Collections::Hashtable^ files = gcnew System::Collections::Hashtable();

	try
	{
		char* chars =  ( char*)(Marshal::StringToHGlobalAnsi(fileName)).ToPointer();
		std::string lfileName=chars;

		AperioFileManager svsmgr(lfileName.c_str());

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
			//_log->Debug("BigTiffDotNetReader::GetSubImages calling getThumbnail");
			//isThumb=svsmgr.getThumbnail(ofilename_thumb.c_str() );
			//files->Add("thumbnail", gcnew String(ofilename_thumb.c_str()));

			//ofilename_label = std::string(ofilepathchars) + "_label.ppm";
			//_log->Debug("BigTiffDotNetReader::GetSubImages calling getLabelImage");
			//isLabel =svsmgr.getLabelImage(ofilename_label.c_str() );

			//ofilename_macro = std::string(ofilepathchars) +  "_macro.ppm";
			//_log->Debug("BigTiffDotNetReader::GetSubImages calling getWSIImage");
			//isMacro= svsmgr.getWSIImage(ofilename_macro.c_str() );
			
			ofilename_dir2 = std::string(ofilepathchars) +  "_dir2.tif";
			_log->Debug("BigTiffDotNetReader::GetSubImages calling getTiffDirImage for dir 2");
			isDir2= svsmgr.getTiffDirImage(2, ofilename_dir2.c_str());
			files->Add("dir2",gcnew String(ofilename_dir2.c_str()));

						
			/*ofilename_dir3 = std::string(ofilepathchars) +  "_dir3.tif";
			_log->Debug("BigTiffDotNetReader::GetSubImages calling getTiffDirImage for dir 3");
			isDir3= svsmgr.getTiffDirImage(3, ofilename_dir3.c_str());*/

		}
		catch(SVSTiffError e)
		{
			_log->Error("BigTiffDotNetReader::GetSubImages recieved SVSTiffError while trying to get thumbnail, label or WSI " +  gcnew String(e.what()) + " on  filename :"+gcnew String( fileName) );

		}
		catch(...)
		{
			_log->Error("BigTiffDotNetReader::GetSubImages recieved non SVSTiffError while trying to get thumbnail, label or WSI" +  " on  filename :"+gcnew String( fileName) );

		}
		Marshal::FreeHGlobal(IntPtr(chars));
		Marshal::FreeHGlobal(IntPtr(ofilepathchars));

	}	

	catch(SVSTiffError e)
	{
		_log->Error("BigTiffDotNetReader::GetSubImages receved SVSTiffError  " +  gcnew String(e.what()) + " on  filename :"+gcnew String( fileName) );

	}
	catch(...)
	{
		_log->Error("BigTiffDotNetReader::GetSubImages receved non SVSTiffError  " +  " on  filename :"+gcnew String( fileName) );

	}	
	return files;
}


void BigTiffDotNetReader::SVSGetMetadata(String^ fileName, String^ baseFileName)
{
	_log->Debug("BigTiffDotNetReader::SVSGetMetadata called with source file : "+gcnew String(fileName) +" and destination directory :"+ gcnew String(baseFileName) );

	char* chars =  (char*)(Marshal::StringToHGlobalAnsi(fileName)).ToPointer();
	std::string lfileName=chars;
	try
	{
		AperioFileManager svsmgr(lfileName.c_str() );

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
			_log->Debug("BigTiffDotNetReader::SVSGetMetadata calling getMetadata");
			std::map<std::string,std::string> metadata= svsmgr.getMetadata(i);
			// Set attribute name and value!
			parentNode->SetAttribute(gcnew String("ID"), gcnew String(metadata["currentDir"].c_str() ) );

			_log->Debug("BigTiffDotNetReader::SVSGetMetadata setting metadata "+ gcnew String("ID")+"   "+  gcnew String(metadata["currentDir"].c_str() )) ;

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

					_log->Debug("BigTiffDotNetReader::SVSGetMetadata setting metadata "+ key + "   "+  gcnew String(it->second.c_str() ) ) ;


				}
				catch(...)
				{
					_log->Error("BigTiffDotNetReader::SVSGetMetadata receved non SVSTiffError  " +  " on  filename :"+gcnew String( fileName) );
				}
			}
			
		}

		// Save to the XML file
		ofilename = std::string(ofilepathchars) + "_meta.xml";
		xmlDoc->Save(gcnew String(ofilename.c_str() ) );

		Marshal::FreeHGlobal(IntPtr(chars));
		Marshal::FreeHGlobal(IntPtr(ofilepathchars));
	}

	catch(SVSTiffError e)
	{
		_log->Error("BigTiffDotNetReader::SVSGetMetadata receved SVSTiffError  " +  gcnew String(e.what()) + " on  filename :"+gcnew String( fileName) );
	}
	catch(...)
	{
		_log->Error("BigTiffDotNetReader::SVSGetMetadata receved non SVSTiffError  " +  " on  filename :"+gcnew String( fileName) );

	}	

}





