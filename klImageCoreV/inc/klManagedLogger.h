/*******************************
 * Copyright (c) <2007>, <Bruce Campbell> All rights reserved. Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  *  
 * Bruce B Campbell 11 30 2012  *
 ********************************/

#pragma once 
using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Xml;
using namespace System::Text;
using namespace log4net;

#include <string>

namespace klManagedLogger
{
	public ref class klLogWrapper 
    {
	public: 
		klLogWrapper( Object^ type)
		{
			_log = LogManager::GetLogger( type->GetType() );
			_log->Info("klLogWrapper loading logger");
		}
		
		ILog^ getLogger()
		{
			return _log;
		}
		
		void logDebug (std::string msg)
		{
			_log->Debug(gcnew String(msg.c_str()) );

		}
		
		void logError(std::string msg)
		{
			_log->Error(gcnew String(msg.c_str() ) );
		}

		void logInfo(std::string msg)
		{
			_log->Info(gcnew String(msg.c_str() ) );
		}

	protected:

		ILog^ _log;

    };
};


