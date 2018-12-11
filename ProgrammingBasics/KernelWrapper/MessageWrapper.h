#pragma once

#include "ManagedObject.h"
#include "MailBox.h"
#include "Converter.h"

using namespace System;

using namespace Kernel;

using namespace System::Runtime::InteropServices;

namespace KernelWrapper 
{
	public ref class MessageWrapper : public ManagedObject<Message> 
	{
	private:
		array<double>^ doubleArr;
		array<int>^ intArr;
		array <String^>^ strArr;
		int code;
	public:
		
		MessageWrapper(Message* mess) : ManagedObject(mess) 
		{
			doubleArr = Helpers::Converter::ToDotNetArray(instance->doubleArr);
			intArr = Helpers::Converter::ToDotNetArray(instance->intArr);
			strArr = Helpers::Converter::ToDotNetArrayStr(instance->strArr);
			code = instance->code;
		}

		array<double>^ GetDoubleArr() 
		{
			return doubleArr;
		}

		array<int>^ GetIntArr() 
		{
			return intArr;
		}

		array<String^>^ GetStrArr()
		{
			return strArr;
		}

		int GetCode() 
		{
			return code;
		}

	};
}