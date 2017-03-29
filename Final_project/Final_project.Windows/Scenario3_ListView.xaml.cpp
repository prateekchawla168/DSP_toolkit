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
// Scenario3.xaml.cpp
// Implementation of the Scenario3 class
//

#include "pch.h"
#include "Scenario3_ListView.xaml.h"
#include "ItemViewer.xaml.h"
#include <string.h>
#include <string>

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

Scenario3::Scenario3()
{
    InitializeComponent();
	/*
    storeData = ref new StoreData();
    ItemListView->ItemsSource = storeData->Items;
	*/
}

// <summary>
// Invoked when this page is about to be displayed in a Frame.
// </summary>
// <param name="e">Event data that describes how this page was reached.  The Parameter
// property is typically used to configure the page.</param>
void Scenario3::OnNavigatedTo(NavigationEventArgs^ e)
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
void SDKSample::ListViewSimple::Scenario3::ItemGridView_ContainerContentChanging(
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

void SDKSample::ListViewSimple::Scenario3::LoadState(Object^ sender, Common::LoadStateEventArgs^ e)
{
	(void)sender;	// Unused parameter
	(void)e;	// Unused parameter

	if (e->PageState != nullptr && e->PageState->HasKey("outp_1utText"))
	{
		ResultBlock->Text = e->PageState->Lookup("outp_1utText")->ToString();
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
	if (roamingSettings->Values->HasKey("Signal3"))
	{
		NumberBox->Text = roamingSettings->Values->Lookup("Signal3")->ToString();
	}
}


void SDKSample::ListViewSimple::Scenario3::SaveState(Object^ sender, Common::SaveStateEventArgs^ e) {
	(void)sender;	// Unused parameter
	(void)e; // Unused parameter
	e->PageState->Insert("outputText", ResultBlock->Text);
}


void SDKSample::ListViewSimple::Scenario3::InputBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	auto roamingSettings =
		Windows::Storage::ApplicationData::Current->RoamingSettings;
	roamingSettings->Values->Insert("Signal1", InputBox->Text);
}


void SDKSample::ListViewSimple::Scenario3::TransferBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	auto roamingSettings =
		Windows::Storage::ApplicationData::Current->RoamingSettings;
	roamingSettings->Values->Insert("Signal2", TransferBox->Text);
}

void SDKSample::ListViewSimple::Scenario3::NumberBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	auto roamingSettings =
		Windows::Storage::ApplicationData::Current->RoamingSettings;
	roamingSettings->Values->Insert("Signal3", NumberBox->Text);
}


//Global Variables.
int len1_1 = 0;	//length of 1st signal
int len2_1 = 0;	//length of 2nd signal
double input_1[30];	//signal1
double transfer_1[30];	//signal2
char* substring_1;	//substring_1 extraction for strtod()
double outp_1[30];	//outp_1ut array
double temp_1[30];	//temp_1orary array for matrix_1 calc
double matrix_1[30][30]; //matrix_1 of h[n]
double number_1;		//number_1 of points

					   //local functions
void convolve(double[30], double[30]);
void matrix_1_multiply(int);
void rotate_right_1(double[30], int); 
String^ doubletostr_1(double);

void SDKSample::ListViewSimple::Scenario3::button_Click_1(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	rootPage->NotifyUser("Working...", NotifyType::ErrorMessage);

	unsigned int i = 0;
	int index = 0;

	String^ Signal1 = InputBox->Text->ToString();
	std::wstring sign1(Signal1->Data());		//extract signal1 as string

	String^ Signal2 = TransferBox->Text->ToString();
	std::wstring sign2(Signal2->Data());
	
	String^ Signal3 = NumberBox->Text->ToString();
	std::wstring sign3(Signal3->Data());


	while (i < sign1.length()) {
		unsigned int sum = 0;
		bool flag = 0;
		while (i < sign1.length() && sign1[i] != ' ') {
			sum = sum * 10 + (sign1[i] - '0');
			flag = 1;
			i++;
		}
		if (flag && sign1[i] == ' ') {
			input_1[index++] = sum;

			i++;
		}
		/*if (sign1[i] == '.') {
		break;
		}*/
		len1_1 = index;
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
			transfer_1[index++] = sum;
			i++;
		}
		/*	if (sign2[i] == '.') {
		break;
		}*/
		//len3 = index;
		len2_1 = index;
	}

	i = 0;
	index = 0;
	while (i < sign3.length()) {			//converting number_1 of points.
		int sum = 0;
		int flag = 0;
		while (i<sign3.length() && sign3[i] != ' ') {
			sum = sum * 10 + (sign3[i] - '0');
			flag = 1;
			i++;
		}
		if (flag && sign3[i] == ' ') {
			number_1 = sum;
			i++;
		}
		/*	if (sign2[i] == '.') {
		break;
		}*/
		//	len2_1 = index;
	}
	//Main convolution loop

	convolve_c();

	rootPage->NotifyUser("Ready. ", NotifyType::StatusMessage);
}


void SDKSample::ListViewSimple::Scenario3::convolve_c() {


	int new_length = 0;


	new_length = number_1;

	/* Pad both sequences with zeroes */

	for (int i = len2_1; i < new_length; i++) {
		if (i > 0)
			transfer_1[i] = 0;
	}

	for (int i = len1_1; i < new_length; i++) {
		if (i > 0)
			input_1[i] = 0;
	}






	//create matrix_1 of h[n]


	for (int i = 0; i < new_length; i++)
		for (int j = 0; j < new_length; j++)
			matrix_1[i][j] = 0;

	for (int i = 0; i < new_length; i++)
		temp_1[i] = input_1[i];

	for (int i = 0; i < new_length; i++) {
		for (int j = 0; j < new_length; j++) {
			matrix_1[j][i] = temp_1[j];
		}
		rotate_right_1(temp_1, new_length);
	}


	matrix_1_multiply(new_length);

	//displaying final outp_1ut.

	//	Platform::String^ outp_1utMessage; 
	Platform::String^ outp_1M;
	Platform::String^ matrix_1M;
	Platform::String^ inpM;

	outp_1M = "Output\n";
	matrix_1M = "Transfer function matrix\n";
	inpM = "Input\n";
	//outp_1utMessage = "\t outp_1ut \t\t\t\t h(n) \t\t\t input_1\n";
	for (int i = 0; i < new_length; i++) {

		//	outp_1utMessage = outp_1utMessage + "\t   " + doubletostr_1(outp_1[i]) + "\t\t";
		outp_1M = outp_1M + doubletostr_1(outp_1[i]) + "\n";

		for (int j = 0; j < new_length; j++) {
			//	outp_1utMessage = outp_1utMessage + " " + doubletostr_1(matrix_1[i][j]);
			matrix_1M = matrix_1M + " " + doubletostr_1(matrix_1[i][j]);

		}
		//	outp_1utMessage = outp_1utMessage + "\t     " + doubletostr_1(transfer_1[i]) + "\n" ;
		inpM = inpM + doubletostr_1(transfer_1[i]) + "\n";
		matrix_1M = matrix_1M + "\n";
		//outp_1M = outp_1M + "\n";
	}
	//	outp_1ut->Text = outp_1utMessage;
	OutputBlock_1->Text = outp_1M;
	MatrixBlock_1->Text = matrix_1M;
	InputBlock_1->Text = inpM;
}




void matrix_1_multiply(int n)
{

	for (int i = 0; i < n; i++)
		outp_1[i] = 0;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			outp_1[i] += matrix_1[i][j] * transfer_1[j];
		}
	}

}

void rotate_right_1(double inp[30], int n)
{
	double ans[30] = { 0 };

	ans[0] = inp[n - 1];
	for (int i = 0; i < (n - 1); i++) {
		ans[i + 1] = inp[i];
	}
	for (int i = 0; i < n; i++) {
		temp_1[i] = ans[i];
	}

}


String^ doubletostr_1(double n) {

	string str = to_string(n);
	std::wstring wstr = std::wstring(str.begin(), str.end());
	const wchar_t* tem3 = wstr.c_str();
	String^ res = ref new String(tem3);
	return res;

}


