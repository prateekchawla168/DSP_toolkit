//
// PhotoPage.xaml.cpp
// Implementation of the PhotoPage class
//

#include "pch.h"
#include "PhotoPage.xaml.h"
#include <ppltasks.h>
#include <string.h>
#include <string>

using namespace concurrency;
using namespace HelloWorld;
using namespace std;

using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// The Basic Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234237

PhotoPage::PhotoPage()
{
	InitializeComponent();
	SetValue(_defaultViewModelProperty, ref new Map<String^,Object^>(std::less<String^>()));
	auto navigationHelper = ref new Common::NavigationHelper(this);
	SetValue(_navigationHelperProperty, navigationHelper);
	navigationHelper->LoadState += ref new Common::LoadStateEventHandler(this, &PhotoPage::LoadState);
	navigationHelper->SaveState += ref new Common::SaveStateEventHandler(this, &PhotoPage::SaveState);
}

DependencyProperty^ PhotoPage::_defaultViewModelProperty =
	DependencyProperty::Register("DefaultViewModel",
		TypeName(IObservableMap<String^,Object^>::typeid), TypeName(PhotoPage::typeid), nullptr);

/// <summary>
/// used as a trivial view model.
/// </summary>
IObservableMap<String^, Object^>^ PhotoPage::DefaultViewModel::get()
{
	return safe_cast<IObservableMap<String^, Object^>^>(GetValue(_defaultViewModelProperty));
}

DependencyProperty^ PhotoPage::_navigationHelperProperty =
	DependencyProperty::Register("NavigationHelper",
		TypeName(Common::NavigationHelper::typeid), TypeName(PhotoPage::typeid), nullptr);

/// <summary>
/// Gets an implementation of <see cref="NavigationHelper"/> designed to be
/// used as a trivial view model.
/// </summary>
Common::NavigationHelper^ PhotoPage::NavigationHelper::get()
{
	return safe_cast<Common::NavigationHelper^>(GetValue(_navigationHelperProperty));
}

#pragma region Navigation support

/// The methods provided in this section are simply used to allow
/// NavigationHelper to respond to the page's navigation methods.
/// 
/// Page specific logic should be placed in event handlers for the  
/// <see cref="NavigationHelper::LoadState"/>
/// and <see cref="NavigationHelper::SaveState"/>.
/// The navigation parameter is available in the LoadState method 
/// in addition to page state preserved during an earlier session.

void PhotoPage::OnNavigatedTo(NavigationEventArgs^ e)
{
	NavigationHelper->OnNavigatedTo(e);
}

void PhotoPage::OnNavigatedFrom(NavigationEventArgs^ e)
{
	NavigationHelper->OnNavigatedFrom(e);
}

#pragma endregion

/// <summary>
/// Populates the page with content passed during navigation. Any saved state is also
/// provided when recreating a page from a prior session.
/// </summary>
/// <param name="sender">
/// The source of the event; typically <see cref="NavigationHelper"/>
/// </param>
/// <param name="e">Event data that provides both the navigation parameter passed to
/// <see cref="Frame.Navigate(Type, Object)"/> when this page was initially requested and
/// a dictionary of state preserved by this page during an earlier
/// session. The state will be null the first time a page is visited.</param>
void PhotoPage::LoadState(Object^ sender, Common::LoadStateEventArgs^ e)
{
	(void) sender;	// Unused parameter
	(void) e;	// Unused parameter

	if (e->PageState != nullptr && e->PageState->HasKey("OutputText"))
	{
		Output->Text = e->PageState->Lookup("OutputText")->ToString();
	}
	// Restore values stored in app data.
	auto roamingSettings =
		Windows::Storage::ApplicationData::Current->RoamingSettings;
	if (roamingSettings->Values->HasKey("Signal1"))
	{
		Input->Text = roamingSettings->Values->Lookup("Signal1")->ToString();
	}
	if (roamingSettings->Values->HasKey("Signal2"))
	{
		Transfer->Text = roamingSettings->Values->Lookup("Signal2")->ToString();
	}
}

/// <summary>
/// Preserves state associated with this page in case the application is suspended or the
/// page is discarded from the navigation cache.  Values must conform to the serialization
/// requirements of <see cref="SuspensionManager::SessionState"/>.
/// </summary>
/// <param name="sender">The source of the event; typically <see cref="NavigationHelper"/></param>
/// <param name="e">Event data that provides an empty dictionary to be populated with
/// serializable state.</param>
void PhotoPage::SaveState(Object^ sender, Common::SaveStateEventArgs^ e){
	(void) sender;	// Unused parameter
	(void) e; // Unused parameter
	e->PageState->Insert("OutputText", Output->Text);
}


void HelloWorld::PhotoPage::PhotoPage_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e)
{
	if (e->NewSize.Height / e->NewSize.Width >= 1)
	{
		VisualStateManager::GoToState(this, "Portrait", true);
	}
	else
	{
		VisualStateManager::GoToState(this, "DefaultLayout", true);
	}

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
void matrix_multiply( int);
void rotate_right(double[30], int);
String^ doubletostr(double);

void HelloWorld::PhotoPage::GetPhotoButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	unsigned int i = 0;
	int index = 0;
	String^ Signal1 = Input->Text->ToString();
	std::wstring sign1(Signal1->Data());		//extract signal1 as string

	String^ Signal2 = Transfer->Text->ToString();
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


}



void HelloWorld::PhotoPage::Input_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	auto roamingSettings =
		Windows::Storage::ApplicationData::Current->RoamingSettings;
	roamingSettings->Values->Insert("Signal1", Input->Text);
}


void HelloWorld::PhotoPage::Transfer_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	auto roamingSettings =
		Windows::Storage::ApplicationData::Current->RoamingSettings;
	roamingSettings->Values->Insert("Signal2", Transfer->Text);
}

void HelloWorld::PhotoPage::convolve() {

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