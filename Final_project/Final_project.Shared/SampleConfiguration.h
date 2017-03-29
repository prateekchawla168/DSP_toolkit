//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

#pragma once

#include <collection.h>

namespace SDKSample
{
    public value struct Scenario
    {
        Platform::String^ Title;
        Platform::String^ ClassName;
    };

    partial ref class MainPage
    {
    public:
        static property Platform::String^ FEATURE_NAME 
        {
            Platform::String^ get() 
            {  
                return ref new Platform::String(L"DSP Toolkit"); 
            }
        }

        static property Platform::Array<Scenario>^ scenarios 
        {
            Platform::Array<Scenario>^ get() 
            { 
                return scenariosInner; 
            }
        }

		static property Platform::String^ Filename
		{
			Platform::String^ get()
			{
				return ref new Platform::String(L"output.txt");
			}
		}

		property Windows::Storage::StorageFile^ SampleFile
		{
			Windows::Storage::StorageFile^ get()
			{
				return sampleFile;
			}
			void set(Windows::Storage::StorageFile^ value)
			{
				sampleFile = value;
			}
		}

	internal:
		void ValidateFile();
		void NotifyUserFileNotExist();
		void HandleFileNotFoundException(Platform::COMException^ e);
    private:
        static Platform::Array<Scenario>^ scenariosInner;
		Windows::Storage::StorageFile^ sampleFile;
    };
}
