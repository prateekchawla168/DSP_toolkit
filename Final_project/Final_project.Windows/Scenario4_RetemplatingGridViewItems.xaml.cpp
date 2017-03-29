//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
//
//*********************************************************

//
// Scenario4.xaml.cpp
// Implementation of the Scenario4 class
//

#include "pch.h"
#include "Scenario4_RetemplatingGridViewItems.xaml.h"
#include <string.h>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>


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

Scenario4::Scenario4()
{
    InitializeComponent();

	// create a new instance of store data
    storeData = ref new StoreData();
    // set the source of the GridView to be the sample data
    //ItemGridView->ItemsSource = storeData->Items;

	//clear all text boxes.
	ClearAllTextBoxes();
}

/// <summary>
/// Invoked when this page is about to be displayed in a Frame.
/// </summary>
/// <param name="e">Event data that describes how this page was reached.  The Parameter
/// property is typically used to configure the page.</param>
void Scenario4::OnNavigatedTo(NavigationEventArgs^ e)
{
    // A pointer back to the main page.  This is needed if you want to call methods in MainPage such
    // as NotifyUser()
    rootPage = MainPage::Current;
}


//Global variables and functions

//pair<float, float>& multiply(pair<float, float>&, pair<float, float>&); 
void multiply(pair<float, float>&, pair<float, float>&);

pair<float, float> cplx[8], output[8], temp4[8], temp8[8], temp_store4[8], temp_store8[8], temparray[8];
//complex<float> cplx[8], output[8], temp[8], temp_store[8], temparray[8];
int length1 = 0;
int length2 = 0;
//int Points = 0;		//selector value
String^ floattostr_2(float);	//convert to string for display as textblock.
float round_off_to(float);	//round_off_to off to first 3 digits

void SDKSample::ListViewSimple::Scenario4::FindFFTButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	rootPage->NotifyUser("Working...", NotifyType::ErrorMessage);

	unsigned int i = 0;
	int index = 0;

	int Points = Selector->Value;
	int algo = FlipSwitch->Value;

	String^ Signal1 = inputBox_Real->Text->ToString();
	std::wstring sign1(Signal1->Data());		//extract signal1 as string

	String^ Signal2 = inputBox_Imaginary->Text->ToString();
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
			cplx[index++].first = sum;

			i++;
		}
		/*if (sign1[i] == '.') {
		break;
		}*/
		length1 = index;
	}
		//adjusting for points:
	if (Points > length1) {
		for (int i = length1; i < Points; i++) {
			cplx[i].first = 0;
		}
	}

	i = 0;
	index = 0;

	while (i < sign2.length()) {
		unsigned int sum = 0;
		bool flag = 0;
		while (i < sign2.length() && sign2[i] != ' ') {
			sum = sum * 10 + (sign2[i] - '0');
			flag = 1;
			i++;
		}
		if (flag && sign2[i] == ' ') {
			cplx[index++].second = sum;

			i++;
		}
		/*if (sign1[i] == '.') {
		break;
		}*/
		length2 = index;
	}
	//adjusting for points:
	if (Points > length2) {
		for (int i = length2; i < Points; i++) {
			cplx[i].second = 0;
		}
	}
	 
	ClearAllTextBoxes();

	switch (algo) {

	case 0:

		switch (Points) {
		case 1:
			FFT_2pt(cplx);
			display2pt();
			rootPage->NotifyUser("Done.", NotifyType::StatusMessage);
			break;
		case 2:
			FFT_4pt(cplx);
			display4pt();
			rootPage->NotifyUser("Done.", NotifyType::StatusMessage);
			break;
		case 3:
			FFT_8pt(cplx);
			display8pt();
			rootPage->NotifyUser("Done.", NotifyType::StatusMessage);
			break;
		default:
			rootPage->NotifyUser("Error. Please slide selector properly to one of the ticks and press 'Find FFT!' button again.", NotifyType::ErrorMessage);
			break;
		}

		break;
	case 1:
		switch (Points) {
		case 1:
			FFT_2pt(cplx);
			display2pt();
			rootPage->NotifyUser("Done.", NotifyType::StatusMessage);
			break;
		case 2:
			FFT_4pt_dit(cplx);
			display4pt();
			rootPage->NotifyUser("Done.", NotifyType::StatusMessage);
			break;
		case 3:
			FFT_8pt_dit(cplx);
			display8pt();
			rootPage->NotifyUser("Done.", NotifyType::StatusMessage);
			break;
		default:
			rootPage->NotifyUser("Error. Please slide selector properly to one of the ticks and press 'Find FFT!' button again.", NotifyType::ErrorMessage);
			break;
		}
		break;
	default:
		rootPage->NotifyUser("Error. Please slide selector properly to one of the ticks and press 'Find FFT!' button again.", NotifyType::ErrorMessage);
		break;
	}

}

void SDKSample::ListViewSimple::Scenario4::FFT_2pt(pair<float, float>cplx[8]) {
	
	output[0].first = cplx[0].first + cplx[1].first;
	output[0].second = cplx[0].second + (cplx[1].second);
	output[1].first = cplx[0].first - cplx[1].first;
	output[1].second = cplx[0].second - (cplx[1].second);

	

	return;

}

void SDKSample::ListViewSimple::Scenario4::display2pt() {

	//change image background_off_to.
	auto bitmapImage = ref new BitmapImage();
	bitmapImage->UriSource = ref new Uri("ms-appx:///Assets/2-pt-fft-output_w.png");
	outputImage->Source = bitmapImage;

	ClearAllTextBoxes();

	//display input
	String^ CurrText;
	CurrText = floattostr_2(round_off_to(cplx[0].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(cplx[0].second));
	Input20->Text = CurrText;

	CurrText = floattostr_2(cplx[1].first);
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(cplx[1].second));
	Input21->Text = CurrText;
	//display output
	CurrText = floattostr_2(output[0].first);
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(output[0].second));
	_2Out0->Text = CurrText;
	 
	CurrText = floattostr_2(output[1].first);
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(output[1].second)) + " ";
	_2Out1->Text = CurrText;

	return;
}

void SDKSample::ListViewSimple::Scenario4::FFT_4pt(pair<float, float>cplx[8]) {

	pair<float, float>mult;

	

	temp4[0] = cplx[0];
	temp4[1] = cplx[2];
	FFT_2pt(temp4);
	temp_store4[0] = output[0];
	temp_store4[2] = output[1];
	temp4[0] = cplx[1];
	temp4[1] = cplx[3];
	FFT_2pt(temp4);
	temp_store4[1] = output[0];
	temp_store4[3] = output[1];

	mult.first = 0;
	mult.second = -1;
	//temp_store[3] = multiply(temp_store[3], mult);
	multiply(temp_store4[3], mult);

	

	output[0].first = temp_store4[0].first + temp_store4[1].first;			//output[0]=temp_store[0] + temp_store[1];
	output[0].second = temp_store4[0].second + (temp_store4[1].second);

	output[1].first = temp_store4[0].first - ((temp_store4[1].first));		//output[1] = temp_store[0] - temp_store[1];
	output[1].second = temp_store4[0].second - (temp_store4[1].second);

	output[2].first = temp_store4[2].first + temp_store4[3].first;			//output[2] = temp_store[2] + temp_store[3];
	output[2].second = temp_store4[2].second + (temp_store4[3].second);

	output[3].first = temp_store4[2].first - ((temp_store4[3].first));		//output[3] = temp_store[2] - temp_store[3];
	output[3].second = temp_store4[2].second - (temp_store4[3].second);

	/*FFT_2pt(temp_store);
	for (int i = 0; i < 2; i++)
		temp[i] = output[i];
	for (int i = 0; i < 2; i++)
		temp_store[i] = temp_store[i + 2];
	FFT_2pt(temp_store);
	for (int i = 0; i < 2; i++)
		output[2 + i] = output[i];
	for (int i = 0; i < 2; i++)
		output[i] = temp[i];*/

	

	return;
}

void SDKSample::ListViewSimple::Scenario4::display4pt() {

	
	//change image background to.
	auto bitmapImage = ref new BitmapImage();

	if (FlipSwitch->Value) {
		bitmapImage->UriSource = ref new Uri("ms-appx:///Assets/4-pt-fft-output_w.png");
		outputImage->Source = bitmapImage;
	}
	else {
		bitmapImage->UriSource = ref new Uri("ms-appx:///Assets/4-pt-fft-output_dit_w.png");
		outputImage->Source = bitmapImage;
	}
		
			
	ClearAllTextBoxes();

	//display input
	String^ CurrText;
	CurrText = floattostr_2(round_off_to(cplx[0].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(cplx[0].second));
	Input40->Text = CurrText;

	CurrText = floattostr_2(round_off_to(cplx[1].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(cplx[1].second));
	Input41->Text = CurrText;

	CurrText = floattostr_2(round_off_to(cplx[2].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(cplx[2].second));
	Input42->Text = CurrText;

	CurrText = floattostr_2(round_off_to(cplx[3].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(cplx[3].second)) + " ";
	Input43->Text = CurrText;

	//Display output of 4 point block
	CurrText = floattostr_2(round_off_to(temp_store4[0].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(temp_store4[0].second));
	_4pt0->Text = CurrText;

	CurrText = floattostr_2(round_off_to(temp_store4[1].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(temp_store4[1].second));
	_4pt1->Text = CurrText;

	CurrText = floattostr_2(round_off_to(temp_store4[2].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(temp_store4[2].second));
	_4pt2->Text = CurrText;

	CurrText = floattostr_2(round_off_to(temp_store4[3].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(temp_store4[3].second)) + " ";
	_4pt3->Text = CurrText;

	//display final output
	CurrText = floattostr_2(round_off_to(output[0].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(output[0].second));
	_2pt0->Text = CurrText;

	CurrText = floattostr_2(round_off_to(output[1].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(output[1].second));
	_2pt1->Text = CurrText;

	CurrText = floattostr_2(round_off_to(output[2].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(output[2].second));
	_2pt2->Text = CurrText;

	CurrText = floattostr_2(round_off_to(output[3].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(output[3].second)) + " ";
	_2pt3->Text = CurrText;

	return;
}

void SDKSample::ListViewSimple::Scenario4::FFT_8pt(pair<float, float>cplx[8]) {
	
	pair<float, float>mult;

	temp8[0] = cplx[0];					
	temp8[1] = cplx[4];
	FFT_2pt(temp8);							//do 2 point FFT of inputs 0 and 4.
	temp_store8[0] = output[0];
	temp_store8[4] = output[1];
	temp8[0] = cplx[2];
	temp8[1] = cplx[6];
	FFT_2pt(temp8);							//do 2 point FFT of inputs 2 and 6
	temp_store8[2] = output[0];
	temp_store8[6] = output[1];
	temp8[0] = cplx[1];
	temp8[1] = cplx[5];
	FFT_2pt(temp8);							//do 2 point FFT of inputs 1 and 5
	temp_store8[1] = output[0];
	temp_store8[5] = output[1];
	temp8[0] = cplx[3];
	temp8[1] = cplx[7];
	FFT_2pt(temp8);							//do 2 point FFT of inputs 3 and 7
	temp_store8[3] = output[0];
	temp_store8[7] = output[1];

	mult.first = 0.707;
	mult.second = -0.707;
	//temp_store[5] = multiply(temp_store[5], mult);
	multiply(temp_store8[5], mult);			//multiply output[5] with W8^1
	mult.first = 0;
	mult.second = -1;
	//temp_store[6] = multiply(temp_store[6], mult);
	multiply(temp_store8[6], mult);			//multiply output[6] with W8^2
	mult.first = -0.707;
	mult.second = -0.707;
	//temp_store[7] = multiply(temp_store[7], mult);
	multiply(temp_store8[7], mult);			//multiply output[7] with W8^3


	//store calculated outputs in temparray[] for display purposes
	for (int i = 0; i < 8; i++) 
		temparray[i] = temp_store8[i];

	//for (int i = 4; i < 8; i++)			//store second half of temp_store in temp
	//	temp8[i - 4] = temp_store8[i];

	//FFT_4pt(temp_store8);				//then take 4-pt FFT of first half

	//for (int i = 0; i < 4; i++)			//then store back the output in temp_store to preserve space
	//	temp_store8[i] = output[i];			

	//FFT_4pt(temp8);						//now take 4-pt FFT of first half

	//for (int i = 0; i < 4; i++)			//store back output in second half of temp_store.
	//	temp_store8[i + 4] = output[i];
	

	//alternative method =>

	temp8[0].first = temp_store8[0].first + temp_store8[2].first;
	temp8[2].first = temp_store8[0].first - temp_store8[2].first;
	temp8[1].first = temp_store8[1].first + temp_store8[3].first;
	temp8[3].first = temp_store8[1].first - temp_store8[3].first;
	temp8[0].second = temp_store8[0].second + temp_store8[2].second;
	temp8[2].second = temp_store8[0].second - temp_store8[2].second;
	temp8[1].second = temp_store8[1].second + temp_store8[3].second;
	temp8[3].second = temp_store8[1].second - temp_store8[3].second;
	
	mult.first = 0;
	mult.second = -1;
	multiply(temp8[3], mult);

	temp8[4].first = temp_store8[4].first + temp_store8[6].first;
	temp8[6].first = temp_store8[4].first - temp_store8[6].first;
	temp8[5].first = temp_store8[5].first + temp_store8[7].first;
	temp8[7].first = temp_store8[5].first - temp_store8[7].first;
	temp8[4].second = temp_store8[4].second + temp_store8[6].second;
	temp8[6].second = temp_store8[4].second - temp_store8[6].second;
	temp8[5].second = temp_store8[5].second + temp_store8[7].second;
	temp8[7].second = temp_store8[5].second - temp_store8[7].second;

	//mult is already (0,-1), but just in case:
	mult.first = 0;
	mult.second = -1;
	multiply(temp8[7], mult);

	// temp8 becomes output of 4-point block. Store this in temp_store8 for continuity.

	for (int i = 0; i < 8; i++)
		temp_store8[i] = temp8[i];
	



	output[0].first = temp_store8[0].first + temp_store8[1].first;		//output[0] = temp_store[0] + temp_store[1];
	output[0].second = temp_store8[0].second + (temp_store8[1].second);

	output[1].first = temp_store8[0].first - (temp_store8[1].first);		//output[1] = temp_store[0] - temp_store[1];
	output[1].second = temp_store8[0].second - (temp_store8[1].second);

	output[2].first = temp_store8[2].first + (temp_store8[3].first);		//output[2] = temp_store[2] + temp_store[3];
	output[2].second = temp_store8[2].second + (temp_store8[3].second);

	output[3].first = temp_store8[2].first - (temp_store8[3].first);		//output[3] = temp_store[2] - temp_store[3];
	output[3].second = temp_store8[2].second - ((temp_store8[3].second));

	output[4].first = temp_store8[4].first + temp_store8[5].first;		//output[4] = temp_store[4] + temp_store[5];
	output[4].second = temp_store8[4].second + (temp_store8[5].second);

	output[5].first = temp_store8[4].first - (temp_store8[5].first);		//output[5] = temp_store[4] - temp_store[5];
	output[5].second = temp_store8[4].second - ((temp_store8[5].second));

	output[6].first = temp_store8[6].first + (temp_store8[7].first);		//output[6] = temp_store[6] + temp_store[7];
	output[6].second = temp_store8[6].second + (temp_store8[7].second);
	
	output[7].first = temp_store8[6].first - (temp_store8[7].first);		//output[7] = temp_store[6] - temp_store[7]
	output[7].second = temp_store8[6].second - ((temp_store8[7].second));

	
	return;
}

void SDKSample::ListViewSimple::Scenario4::display8pt() {


	//change image background_off_to.
	auto bitmapImage = ref new BitmapImage();
	if (FlipSwitch->Value) {
		bitmapImage->UriSource = ref new Uri("ms-appx:///Assets/8-pt-fft-output_w.png");
		outputImage->Source = bitmapImage;
	}
	else {
		bitmapImage->UriSource = ref new Uri("ms-appx:///Assets/8-pt-fft-output_dit_w.png");
		outputImage->Source = bitmapImage;
	}

	ClearAllTextBoxes();

	//display input
	String^ CurrText;
	CurrText = floattostr_2(round_off_to(cplx[0].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(cplx[0].second));
	Input80->Text = CurrText;

	CurrText = floattostr_2(round_off_to(cplx[1].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(cplx[1].second));
	Input81->Text = CurrText;

	CurrText = floattostr_2(round_off_to(cplx[2].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(cplx[2].second));
	Input82->Text = CurrText;

	CurrText = floattostr_2(round_off_to(cplx[3].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(cplx[3].second));
	Input83->Text = CurrText;

	CurrText = floattostr_2(round_off_to(cplx[4].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(cplx[4].second));
	Input84->Text = CurrText;

	CurrText = floattostr_2(round_off_to(cplx[5].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(cplx[5].second));
	Input85->Text = CurrText;

	CurrText = floattostr_2(round_off_to(cplx[6].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(cplx[6].second));
	Input86->Text = CurrText;

	CurrText = floattostr_2(round_off_to(cplx[7].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(cplx[7].second)) + " ";
	Input87->Text = CurrText;

	//display output of 8-point block
	CurrText = floattostr_2(round_off_to(temparray[0].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(temparray[0].second));
	pt80->Text = CurrText;

	CurrText = floattostr_2(round_off_to(temparray[1].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(temparray[1].second));
	pt81->Text = CurrText;

	CurrText = floattostr_2(round_off_to(temparray[2].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(temparray[2].second));
	pt82->Text = CurrText;

	CurrText = floattostr_2(round_off_to(temparray[3].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(temparray[3].second));
	pt83->Text = CurrText;

	CurrText = floattostr_2(round_off_to(temparray[4].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(temparray[4].second));
	pt84->Text = CurrText;

	CurrText = floattostr_2(round_off_to(temparray[5].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(temparray[5].second));
	pt85->Text = CurrText;

	CurrText = floattostr_2(round_off_to(temparray[6].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(temparray[6].second));
	pt86->Text = CurrText;

	CurrText = floattostr_2(round_off_to(temparray[7].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(temparray[7].second)) + " ";
	pt87->Text = CurrText;

	//display output of 4 point block
	CurrText = floattostr_2(round_off_to(temp_store8[0].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(temp_store8[0].second));
	pt40->Text = CurrText;

	CurrText = floattostr_2(round_off_to(temp_store8[1].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(temp_store8[1].second));
	pt41->Text = CurrText;

	CurrText = floattostr_2(round_off_to(temp_store8[2].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(temp_store8[2].second));
	pt42->Text = CurrText;

	CurrText = floattostr_2(round_off_to(temp_store8[3].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(temp_store8[3].second));
	pt43->Text = CurrText;

	CurrText = floattostr_2(round_off_to(temp_store8[4].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(temp_store8[4].second));
	pt44->Text = CurrText;

	CurrText = floattostr_2(round_off_to(temp_store8[5].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(temp_store8[5].second));
	pt45->Text = CurrText;

	CurrText = floattostr_2(round_off_to(temp_store8[6].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(temp_store8[6].second));
	pt46->Text = CurrText;

	CurrText = floattostr_2(round_off_to(temp_store8[7].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(temp_store8[7].second)) + " ";
	pt47->Text = CurrText;
	
	//display final output
	CurrText = floattostr_2(round_off_to(output[0].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(output[0].second));
	pt20->Text = CurrText;

	CurrText = floattostr_2(round_off_to(output[1].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(output[1].second));
	pt21->Text = CurrText;

	CurrText = floattostr_2(round_off_to(output[2].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(output[2].second));
	pt22->Text = CurrText;

	CurrText = floattostr_2(round_off_to(output[3].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(output[3].second));
	pt23->Text = CurrText;

	CurrText = floattostr_2(round_off_to(output[4].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(output[4].second));
	pt24->Text = CurrText;

	CurrText = floattostr_2(round_off_to(output[5].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(output[5].second));
	pt25->Text = CurrText;

	CurrText = floattostr_2(round_off_to(output[6].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(output[6].second));
	pt26->Text = CurrText;

	CurrText = floattostr_2(round_off_to(output[7].first));
	CurrText = CurrText + " +j" + floattostr_2(round_off_to(output[7].second)) + " ";
	pt27->Text = CurrText;
	
	return;

}


void multiply(pair<float, float>& a1, pair<float, float>& a2) {
	pair<float, float>ans;
	ans.first = ((a1.first*a2.first) - (a1.second*a2.second));
	ans.second = ((a1.second*a2.first) + (a1.first*a2.second));
	a1 = ans;
	//return ans;
	return;
}

String^ floattostr_2(float n) {
	

	string str = to_string(n);
	std::wstring wstr = std::wstring(str.begin(), str.end());
	const wchar_t* tem3 = wstr.c_str();
	String^ res = ref new String(tem3);
	return res;

}

float round_off_to(float val) {
	stringstream s;
	s.setf(ios::fixed);
	s << std::setprecision(6);
	s >> val;
	return val;
}

void SDKSample::ListViewSimple::Scenario4::ClearAllTextBoxes() {
	
	//8 point FFT Input
	Input80->Text = " ";
	Input81->Text = " ";
	Input82->Text = " ";
	Input83->Text = " ";
	Input84->Text = " ";
	Input85->Text = " "; 
	Input86->Text = " ";
	Input87->Text = " ";
	//8 point block output
	pt80->Text = " ";
	pt81->Text = " ";
	pt82->Text = " ";
	pt83->Text = " ";
	pt84->Text = " ";
	pt85->Text = " ";
	pt86->Text = " ";
	pt87->Text = " ";
	//8 point FFT, 4 point output
	pt40->Text = " ";
	pt41->Text = " ";
	pt42->Text = " ";
	pt43->Text = " ";
	pt44->Text = " ";
	pt45->Text = " ";
	pt46->Text = " ";
	pt47->Text = " ";
	//8 point FFT, 2 point output
	pt20->Text = " ";
	pt21->Text = " ";
	pt22->Text = " ";
	pt23->Text = " ";
	pt24->Text = " ";
	pt25->Text = " ";
	pt26->Text = " ";
	pt27->Text = " ";
	//4 point FFT, input
	Input40->Text = " ";
	Input41->Text = " ";
	Input42->Text = " ";
	Input43->Text = " ";
	//4 point FFT, 4 point block output
	_4pt0->Text = " ";
	_4pt1->Text = " ";
	_4pt2->Text = " ";
	_4pt3->Text = " ";
	//4 point FFT, 2 point output
	_2pt0->Text = " ";
	_2pt1->Text = " ";
	_2pt2->Text = " ";
	_2pt3->Text = " ";
	//2 point FFT Input
	Input20->Text = " ";
	Input21->Text = " ";
	//2 point FFT Output
	_2Out0->Text = " ";
	_2Out1->Text = " ";

	return;
}

void SDKSample::ListViewSimple::Scenario4::Selector_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	// Points = Selector->Value;
}


//dit code


void SDKSample::ListViewSimple::Scenario4::FFT_4pt_dit(pair<float, float>cplx[8])
{
	pair<float, float> mult;
	temp4[0] = cplx[0];
	temp4[1] = cplx[2];
	FFT_2pt(temp4);
	temp_store4[0] = output[0];
	temp_store4[1] = output[1];


	temp4[0] = cplx[1];
	temp4[1] = cplx[3];
	FFT_2pt(temp4);
	temp_store4[2] = output[0];
	temp_store4[3] = output[1];

	output[0].first = temp_store4[0].first + temp_store4[2].first;
	output[0].second = temp_store4[0].second + temp_store4[2].second;

	output[2].first = temp_store4[0].first - temp_store4[2].first;
	output[2].second = temp_store4[0].second - temp_store4[2].second;

	output[1].first = temp_store4[1].first + temp_store4[3].first;
	output[1].second = temp_store4[1].second + temp_store4[3].second;

	output[3].first = temp_store4[1].first - temp_store4[3].first;
	output[3].second = temp_store4[1].second - temp_store4[3].second;

	mult.first = 0;
	mult.second = -1;
	multiply(output[3], mult);


	return;

}


void SDKSample::ListViewSimple::Scenario4::FFT_8pt_dit(pair<float, float>cplx[8])
{   

	pair<float, float> mult;

	temp8[0] = cplx[0];
	temp8[1] = cplx[4];
	FFT_2pt(temp8);
	temp_store8[0] = output[0];
	temp_store8[1] = output[1];


	temp8[0] = cplx[2];
	temp8[1] = cplx[6];
	FFT_2pt(temp8);
	temp_store8[2] = output[0];
	temp_store8[3] = output[1];

	temp8[0] = cplx[1];
	temp8[1] = cplx[5];
	FFT_2pt(temp8);
	temp_store8[4] = output[0];
	temp_store8[5] = output[1];


	temp8[0] = cplx[3];
	temp8[1] = cplx[7];
	FFT_2pt(temp8);
	temp_store8[6] = output[0];
	temp_store8[7] = output[1];

	for (int i = 0; i < 8; i++)
		temparray[i] = temp_store8[i];

	//taking 4-point block
	
	temp8[0] = temparray[0];
	temp8[1] = temparray[2];
	FFT_2pt(temp8);
	temp_store8[0] = output[0];
	temp_store8[1] = output[1];


	temp8[0] = temparray[1];
	temp8[1] = temparray[3];
	FFT_2pt(temp8);
	temp_store8[2] = output[0];
	temp_store8[3] = output[1];


	temp8[0] = temparray[4];
	temp8[1] = temparray[6];
	FFT_2pt(temp8);
	temp_store8[4] = output[0];
	temp_store8[5] = output[1];


	temp8[0] = temparray[5];
	temp8[1] = temparray[7];
	FFT_2pt(temp8);
	temp_store8[6] = output[0];
	temp_store8[7] = output[1];
	
	mult.first = 0;
	mult.second = -1;

	
	multiply(temp_store8[3], mult); 
	multiply(temp_store8[7], mult);


	output[0].first = temp_store8[0].first + temp_store8[4].first;		//output[0] = temp_store[0] + temp_store[1];
	output[0].second = temp_store8[0].second + (temp_store8[4].second);

	output[4].first = temp_store8[0].first - (temp_store8[4].first);		//output[1] = temp_store[0] - temp_store[1];
	output[4].second = temp_store8[0].second - (temp_store8[4].second);

	output[2].first = temp_store8[2].first + (temp_store8[6].first);		//output[2] = temp_store[2] + temp_store[3];
	output[2].second = temp_store8[2].second + (temp_store8[6].second);

	output[6].first = temp_store8[2].first - (temp_store8[6].first);		//output[3] = temp_store[2] - temp_store[3];
	output[6].second = temp_store8[2].second - ((temp_store8[6].second));

	output[1].first = temp_store8[1].first + temp_store8[5].first;		//output[4] = temp_store[4] + temp_store[5];
	output[1].second = temp_store8[1].second + (temp_store8[5].second);

	output[5].first = temp_store8[1].first - (temp_store8[5].first);		//output[5] = temp_store[4] - temp_store[5];
	output[5].second = temp_store8[1].second - ((temp_store8[5].second));

	output[3].first = temp_store8[3].first + (temp_store8[7].first);		//output[6] = temp_store[6] + temp_store[7];
	output[3].second = temp_store8[3].second + (temp_store8[7].second);

	output[7].first = temp_store8[3].first - (temp_store8[7].first);		//output[7] = temp_store[6] - temp_store[7]
	output[7].second = temp_store8[3].second - ((temp_store8[7].second));

	mult.first = 0.707;
	mult.second = -0.707;
	//temp_store[5] = multiply(temp_store[5], mult);
	multiply(output[5], mult);			//multiply output[5] with W8^1
	mult.first = 0;
	mult.second = -1;
	//temp_store[6] = multiply(temp_store[6], mult);
	multiply(output[6], mult);			//multiply output[6] with W8^2
	mult.first = -0.707;
	mult.second = -0.707; 
	//temp_store[7] = multiply(temp_store[7], mult);
	multiply(output[7], mult);			//multiply output[7] with W8^3

	return;
}