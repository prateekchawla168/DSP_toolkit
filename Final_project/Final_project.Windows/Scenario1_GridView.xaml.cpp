//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

//
// Scenario1.xaml.cpp
// Implementation of the Scenario1 class
//

#include "pch.h"
#include "Scenario1_GridView.xaml.h"
#include "ItemViewer.xaml.h"
#include <string>
#include <string.h>

using namespace SDKSample::ListViewSimple;
using namespace std;
using namespace Platform;
using namespace concurrency;

using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Navigation;

using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Display;
using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;

using namespace SDKSample::ListViewSampleDataSource;

Scenario1::Scenario1()
{
    InitializeComponent();

 /*   storeData = ref new StoreData();
    GridView1->ItemsSource = storeData->Items;*/
}

// <summary>
// Invoked when this page is about to be displayed in a Frame.
// </summary>
// <param name="e">Event data that describes how this page was reached.  The Parameter
// property is typically used to configure the page.</param>
void Scenario1::OnNavigatedTo(NavigationEventArgs^ e)
{
    // A pointer back to the main page.  This is needed if you want to call methods in MainPage such
    // as NotifyUser()
    rootPage = MainPage::Current;
}

// <summary>
// We will visualize the data item in asynchronously in multiple phases for improved panning user experience 
// of large lists.  In this sample scneario, we will visualize different parts of the data item
// in the following order:
// 
//     1) Placeholders (visualized synchronously - Phase 0)
//     2) Tilte (visualized asynchronously - Phase 1)
//     3) Category and Image (visualized asynchronously - Phase 2)
//
// </summary>
// <param name="sender"></param>
// <param name="args"></param>
void SDKSample::ListViewSimple::Scenario1::ItemGridView_ContainerContentChanging(
    ListViewBase^ sender,
    Windows::UI::Xaml::Controls::ContainerContentChangingEventArgs^ args)
{
    ItemViewer^ iv = safe_cast<ItemViewer^>(args->ItemContainer->ContentTemplateRoot);

    if (iv != nullptr)
    {
        // if the container is being added to the recycle queue (meaning it will not particiapte in 
        // visualizig data items for the time being), we clear out the data item
        if (args->InRecycleQueue == true)
        {
            iv->ClearData();
        }
        else if (args->Phase == 0)
        {
            iv->ShowPlaceholder(safe_cast<Item^>(args->Item));

            // Register for async callback to visualize Title asynchronously
            args->RegisterUpdateCallback(ContainerContentChangingDelegate);
        }
        else if (args->Phase == 1)
        {
            iv->ShowTitle();
            args->RegisterUpdateCallback(ContainerContentChangingDelegate);
        }
        else if (args->Phase == 2)
        {
            iv->ShowCategory();
            iv->ShowImage();
        }

        args->Handled = true;
    }
}

void SDKSample::ListViewSimple::Scenario1::LoadState(Object^ sender, Common::LoadStateEventArgs^ e)
{
	(void)sender;	// Unused parameter
	(void)e;	// Unused parameter

	if (e->PageState != nullptr && e->PageState->HasKey("OutputText"))
	{
		ResultBlock->Text = e->PageState->Lookup("OutputText")->ToString();
	}
	// Restore values stored in app data.
	auto roamingSettings =
		Windows::Storage::ApplicationData::Current->RoamingSettings;
	if (roamingSettings->Values->HasKey("Signal1"))
	{
		InputBox->Text = roamingSettings->Values->Lookup("Signal1")->ToString();
	}
	if (roamingSettings->Values->HasKey("Signal2"))
	{
		TransferBox->Text = roamingSettings->Values->Lookup("Signal2")->ToString();
	}
}


void SDKSample::ListViewSimple::Scenario1::SaveState(Object^ sender, Common::SaveStateEventArgs^ e) {
	(void)sender;	// Unused parameter
	(void)e; // Unused parameter
	e->PageState->Insert("OutputText", ResultBlock->Text);
}


void SDKSample::ListViewSimple::Scenario1::InputBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	auto roamingSettings =
		Windows::Storage::ApplicationData::Current->RoamingSettings;
	roamingSettings->Values->Insert("Signal1", InputBox->Text);
}


void SDKSample::ListViewSimple::Scenario1::TransferBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	auto roamingSettings =
		Windows::Storage::ApplicationData::Current->RoamingSettings;
	roamingSettings->Values->Insert("Signal2", TransferBox->Text);
}


//Global Variables.
int len1 = 0;	//length of 1st signal
int len2 = 0;	//length of 2nd signal
double input[30];	//signal1
double transfer[30];	//signal2
char* substring;	//substring extraction for strtod()
double outp[30];	//output array
double temp[30];	//temporary array for matrix calc
double matrix[30][30]; //matrix of h[n]

					   //local functions
void convolve(double[30], double[30]);
void matrix_multiply(int);
void rotate_right(double[30], int);
String^ doubletostr(double);

void SDKSample::ListViewSimple::Scenario1::button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	rootPage->NotifyUser("Working...", NotifyType::ErrorMessage);
	unsigned int i = 0;
	int index = 0;
	String^ Signal1 = InputBox->Text->ToString();
	std::wstring sign1(Signal1->Data());		//extract signal1 as string

	String^ Signal2 = TransferBox->Text->ToString();
	std::wstring sign2(Signal2->Data());


	while (i < sign1.length()) {
		unsigned int sum = 0;
		bool flag = 0;
		while (i < sign1.length() && sign1[i] != ' ') {
			sum = sum * 10 + (sign1[i] - '0');
			flag = 1;
			i++;
		}
		if (flag && sign1[i] == ' ') {
			input[index++] = sum;

			i++;
		}
		/*if (sign1[i] == '.') {
		break;
		}*/
		len1 = index;
	}
	i = 0;
	index = 0;
	while (i < sign2.length()) {
		int sum = 0;
		int flag = 0;
		while (i<sign2.length() && sign2[i] != ' ') {
			sum = sum * 10 + (sign2[i] - '0');
			flag = 1;
			i++;
		}
		if (flag && sign2[i] == ' ') {
			transfer[index++] = sum;
			i++;
		}
		/*	if (sign2[i] == '.') {
		break;
		}*/
		len2 = index;
	}

	//Main convolution loop

	convolve();

	rootPage->NotifyUser("Ready. " , NotifyType::StatusMessage);
}



void SDKSample::ListViewSimple::Scenario1::convolve() {


		int new_length = 0;


		new_length = len1 + len2 - 1;

		/* Pad both sequences with zeroes */

		for (int i = len2; i < new_length; i++) {
			if (i > 0)
				transfer[i] = 0;
		}

		for (int i = len1; i < new_length; i++) {
			if (i > 0)
				input[i] = 0;
		}






		//create matrix of h[n]


		for (int i = 0; i < new_length; i++)
			for (int j = 0; j < new_length; j++)
				matrix[i][j] = 0;

		for (int i = 0; i < new_length; i++)
			temp[i] = input[i];

		for (int i = 0; i < new_length; i++) {
			for (int j = 0; j < new_length; j++) {
				matrix[j][i] = temp[j];
			}
			rotate_right(temp, new_length);
		}


		matrix_multiply(new_length);

		//displaying final output.

		//	Platform::String^ outputMessage;
		Platform::String^ outpM;
		Platform::String^ matrixM;
		Platform::String^ inpM;

		outpM = "Output\n";
		matrixM = "Transfer function matrix\n";
		inpM = "Input\n";
		//outputMessage = "\t Output \t\t\t\t h(n) \t\t\t Input\n";
		for (int i = 0; i < new_length; i++) {

			//	outputMessage = outputMessage + "\t   " + doubletostr(outp[i]) + "\t\t";
			outpM = outpM + doubletostr(outp[i]) + "\n";

			for (int j = 0; j < new_length; j++) {
				//	outputMessage = outputMessage + " " + doubletostr(matrix[i][j]);
				matrixM = matrixM + " " + doubletostr(matrix[i][j]);

			}
			//	outputMessage = outputMessage + "\t     " + doubletostr(transfer[i]) + "\n" ;
			inpM = inpM + doubletostr(transfer[i]) + "\n";
			matrixM = matrixM + "\n";
			//outpM = outpM + "\n";
		}
		//	Output->Text = outputMessage;
		OutputBlock->Text = outpM;
		MatrixBlock->Text = matrixM;
		InputBlock->Text = inpM;
	}




void matrix_multiply(int n)
{

	for (int i = 0; i < n; i++)
		outp[i] = 0;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			outp[i] += matrix[i][j] * transfer[j];
		}
	}

}

void rotate_right(double inp[30], int n)
{
	double ans[30] = { 0 };

	ans[0] = inp[n - 1];
	for (int i = 0; i < (n - 1); i++) {
		ans[i + 1] = inp[i];
	}
	for (int i = 0; i < n; i++) {
		temp[i] = ans[i];
	}

}


String^ doubletostr(double n) {

	string str = to_string(n);
	std::wstring wstr = std::wstring(str.begin(), str.end());
	const wchar_t* tem3 = wstr.c_str();
	String^ res = ref new String(tem3);
	return res;

}