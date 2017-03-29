//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************


#include "pch.h"
#include <ppltasks.h>
#include "MainPage.xaml.h"
#include "SampleConfiguration.h"


using namespace SDKSample;

using namespace concurrency;
using namespace Platform;
using namespace Windows::Storage;

#if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
Platform::Array<Scenario>^ MainPage::scenariosInner = ref new Platform::Array<Scenario>  
{
    // The format here is the following:
    //     { "Description for the sample", "Fully qualified name for the class that implements the scenario" }
    { "Instantiating a GridView", "SDKSample.ListViewSimple.Scenario1" }, 
    { "Responding to click events", "SDKSample.ListViewSimple.Scenario2" },
    { "Instantiating a ListView", "SDKSample.ListViewSimple.Scenario3" }
}; 
#else
Platform::Array<Scenario>^ MainPage::scenariosInner = ref new Platform::Array<Scenario>
{
	// The format here is the following:
	//     { "Description for the sample", "Fully qualified name for the class that implements the scenario" }
	{ "Utility List and Descriptions", "SDKSample.ListViewSimple.Scenario2" },
	{ "Linear Convolution", "SDKSample.ListViewSimple.Scenario1" },
	{ "Circular Convolution", "SDKSample.ListViewSimple.Scenario3" },
	{ "Radix 2 FFT", "SDKSample.ListViewSimple.Scenario4" },
	{ "DSP Assembler Kit", "SDKSample.ListViewSimple.Scenario5" },
	{ "Simulator Kit", "SDKSample.ListViewSimple.Scenario6" },
	{"FIR Filter Design","SDKSample.ListViewSimple.Scenario7"}
};


void MainPage::ValidateFile()
{
	create_task(Windows::Storage::ApplicationData::Current->LocalFolder->GetFileAsync(Filename)).then([=](task<StorageFile^> getFileTask)
	{
		try
		{
			sampleFile = getFileTask.get();
		}
		catch (Platform::COMException^ e)
		{
			// If file doesn't exist, indicate users to use scenario 1 
			HandleFileNotFoundException(e);
		}
	});
}

void MainPage::NotifyUserFileNotExist()
{
	//NotifyUser("The file '" + Filename + "' does not exist. Use scenario one to create this file.", NotifyType::ErrorMessage);
}

void MainPage::HandleFileNotFoundException(Platform::COMException^ e)
{
	if (e->HResult == 0x80070002) // Catch FileNotExistException 
	{
		NotifyUserFileNotExist();
	}
	else
	{
		throw e;
	}
}

#endif