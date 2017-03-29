//
// Scenario7_FIRFilterDesign.xaml.cpp
// Implementation of the Scenario7_FIRFilterDesign class
//

#include "pch.h"
#include "Scenario7_FIRFilterDesign.xaml.h"
#include <string>
#include <string.h>
#include <math.h>

using namespace SDKSample::ListViewSimple;
using namespace std;
using namespace concurrency;
using namespace Platform;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;





Scenario7::Scenario7()
{
	
	InitializeComponent();

}

void Scenario7::OnNavigatedTo(NavigationEventArgs^ e)
{
	// A pointer back to the main page.  This is needed if you want to call methods in MainPage such
	// as NotifyUser()
	rootPage = MainPage::Current;
}

//global variables and functions
#define PI 3.1415926535897932384626433832795
double Ap, As, deltaP, deltaS, omegaP[2], omegaS[2], Ft; //filter specifications
double omegaC[2], delta, deltaOmega, A;  //filter coefficient intermediate results
int  M;		//Length of Filter
float tau;
void FindDelta();		//to find deltaP and deltaS when Ap and As are given
String^ CurrText = "***FIR FILTER DESIGN UTILITY***\n\n";		//current text of textbox
String^ doubletostring(double);	//utility function to convert double to String^
String^ doubletostring(int n);

double* Hm;		//truncated filter function


struct window
{
public:
	string NameOfWindow;
	double ExactTransitionWidth;
	double minSBAttenuation;
};

//Window details:

window Rectwin = { "Rectangular Window", 1.8*PI, 21 };
window Triwin = { "Triangular Window", 6.1*PI, 26};
window Hanwin = { "Han (Hanning) Window", 6.2*PI, 44 };
window Hamwin = { "Hamming Window", 6.6*PI, 54.50 };
window Blackwin = {"Blackman Window", 11*PI, 75};


//window utility functions
double rectanglewindow(int);
double trianglewindow(int);
double hannwindow(int);
double hammingwindow(int);
double blackmanwindow(int);

//calc filter coeffs
double LPF(int);
double HPF(int);


void SDKSample::ListViewSimple::Scenario7::DesignFilterButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) 
{
	unsigned int i = 0;
	int index = 0;

	int inputStyle = InputType->Value;
	int filterType = FilterType->Value;

	String^ Signal1 = PassBandBox->Text->ToString();
	std::wstring sign1(Signal1->Data());		//extract signal1 as string

	String^ Signal2 = StopBandBox->Text->ToString();
	std::wstring sign2(Signal2->Data());

	String^ Signal3 = PassBandFreqBox->Text->ToString();
	std::wstring sign3(Signal3->Data());

	String^ Signal4 = StopBandFreqBox->Text->ToString();
	std::wstring sign4(Signal4->Data());

	String^ Signal5 = FreqBox->Text->ToString();
	std::wstring sign5(Signal5->Data());

	while (i < sign1.length()) {		//convert string to array of double
		unsigned int sum = 0;
		bool flag = 0;
		while (i < sign1.length() && sign1[i] != ' ') {
			sum = sum * 10 + (sign1[i] - '0');
			flag = 1;
			i++;
		}
		if (flag && sign1[i] == ' ') {
			if (!inputStyle)
				Ap = sum;
			else
				deltaP = sum;
			i++;
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
			if (!inputStyle)
				As = sum;
			else
				deltaS = sum;
			i++;
		}
		
	}

	i = 0;
	index = 0;

	while (i < sign3.length()) {
		unsigned int sum = 0;
		bool flag = 0;
		while (i < sign3.length() && sign3[i] != ' ') {
			sum = sum * 10 + (sign3[i] - '0');
			flag = 1;
			i++;
		}
		if (flag && sign3[i] == ' ') {
			omegaP[index++] = sum;
			i++;
		}
		
	}

	index = 0;
	i = 0;

	while (i < sign4.length()) {
		unsigned int sum = 0;
		bool flag = 0;
		while (i < sign4.length() && sign4[i] != ' ') {
			sum = sum * 10 + (sign4[i] - '0');
			flag = 1;
			i++;
		}
		if (flag && sign4[i] == ' ') {
			omegaS[index++] = sum;
			i++;
		}
		
	}

	index = 0;
	i = 0;

	while (i < sign5.length()) {		//convert string to array of double
		unsigned int sum = 0;
		bool flag = 0;
		while (i < sign5.length() && sign5[i] != ' ') {
			sum = sum * 10 + (sign5[i] - '0');
			flag = 1;
			i++;
		}
		if (flag && sign5[i] == ' ') {
			Ft = sum;
			i++;
		}

	}

	//normalise frequencies into (0,pi)

	for (int i = 0; i < 2; i++) {
		omegaP[i] *= (2 * PI);
		omegaS[i] *= (2 * PI);
		omegaP[i] /= Ft;
		omegaS[i] /= Ft;
	}


	if (!inputStyle)
		FindDelta();

	CurrText = CurrText + "Entered Variables: \n 1. Pass band ripple: " + doubletostring(deltaP) + "\n 2. Stop band ripple: " + doubletostring(deltaS);
	OutputBlock->Text = CurrText;
	switch (filterType)
	{
	case 0: 
		CurrText = CurrText + "\n 3. Pass band edge frequency: " + doubletostring(omegaP[0]) + "\n 4. Stop band edge frequency: " + doubletostring(omegaS[0]) + "\n\n Filter Type: Low Pass Filter.";
		OutputBlock->Text = CurrText;
		break;
	case 1:
		CurrText = CurrText + "\n 3. Pass band edge frequency: " + doubletostring(omegaP[0]) + "\n 4. Stop band edge frequency: " + doubletostring(omegaS[0]) + "\n\n Filter Type: High Pass Filter.";
		OutputBlock->Text = CurrText;
		break;
	default:
		CurrText = CurrText + "\n ----Incorrectly Specified Filter. Please select filter type properly and try again.------";
		OutputBlock->Text = CurrText;
		break;
	}

	switch (filterType)
	{
	case 0: DesignLPF();
		break;
	case 1:	DesignHPF();
		break;
	}
	
	


}

void FindDelta() {

	double tempVar = pow(10, (Ap / 20));
	deltaP = (tempVar - 1) / (tempVar + 1);
	deltaS = pow(10, (-1 * As / 20));
	return;
}

//Design of an LPF:
void SDKSample::ListViewSimple::Scenario7::DesignLPF() {

	//step 1: get critical frequency;
	omegaC[0] = 0.5*(omegaP[0]+omegaS[0]);

	//step 2: get deltaOmega
	deltaOmega = omegaS[0] - omegaP[0];

	//step 3: compute delta
	delta = min(deltaP, deltaS);

	//step4: recompute A from delta
	A = -20 * log10(delta);

	//step 5: choose window giving min stop band attenuation >= A
	if (A < Rectwin.minSBAttenuation)
		Rectangular(0);
	else if (A < Triwin.minSBAttenuation)
		Triangular(0);
	else if (A < Hanwin.minSBAttenuation)
		Hann(0);
	else if (A < Hamwin.minSBAttenuation)
		Hamming(0);
	else if (A < Blackwin.minSBAttenuation)
		Blackman(0);
	else 
	{
		CurrText += "\n\n The Given filter has a stop band attenuation of >75 dB and hence cannot be made.";
		OutputBlock->Text = CurrText;
	}

	return;

}

void SDKSample::ListViewSimple::Scenario7::DesignHPF() {

	//step 1: get critical frequency;
	omegaC[0] = 0.5*(omegaP[0] + omegaS[0]);

	//step 2: get deltaOmega
	deltaOmega = omegaP[0] - omegaS[0];

	//step 3: compute delta
	delta = min(deltaP, deltaS);

	//step4: recompute A from delta
	A = -20 * log10(delta);

	//step 5: choose window giving min stop band attenuation >= A
	if (A < Rectwin.minSBAttenuation)
		Rectangular(1);
	else if (A < Triwin.minSBAttenuation)
		Triangular(1);
	else if (A < Hanwin.minSBAttenuation)
		Hann(1);
	else if (A < Hamwin.minSBAttenuation)
		Hamming(1);
	else if (A < Blackwin.minSBAttenuation)
		Blackman(1);
	else
	{
		CurrText += "\n\n The Given filter has a stop band attenuation of >75 dB and hence cannot be made.";
		OutputBlock->Text = CurrText;
	}


	return;
}

//design using rectangular window
void SDKSample::ListViewSimple::Scenario7::Rectangular(int arg)
{
	//announce to user:
	CurrText = CurrText + "\n Using Rectangular Window: \n\n";

	//step 6: obtain length of filter
	M = ceil((Rectwin.ExactTransitionWidth / deltaOmega));
StartRect:	Hm = new double[M];
	tau = (M - 1) * 0.5;

	//show to user
	CurrText = CurrText + "\n Transition Width: " + doubletostring(deltaOmega) + "\n Length of Filter:  " + doubletostring(M);

	//step 7: compute truncated filter function
	for (int i = 0; i < M; i++)
	{
		switch (arg)
		{

		case 0: 
			Hm[i] = rectanglewindow(i) * LPF(i);
			break;
		case 1:
			Hm[i] = rectanglewindow(i) * HPF(i);
			break;
		default:
			break;
		}
		
		
	}


	//show Hm to user
	CurrText = CurrText + "\n Filter coefficients  : ";
	OutputBlock->Text = CurrText;
	for (int i = 0; i < M; i++) {
		CurrText = CurrText + "\n Hm[" + doubletostring(i) + "] = " + doubletostring(Hm[i]);
		OutputBlock->Text = CurrText;
	}

	//check if this filter satisfies conditions
	double a, sum = 0;

	if ((M - 1) % 2 != 0) {				//M is even=> Type 2 filter.

										//sum += b(k)*cos(kw) for k=1 to M/2, 

		for (int i = 1; i <= (M / 2); i++) {
			sum += 2 * Hm[((M / 2) - i)] * cos((i - 0.5)*omegaP[0]);
		}

		a = -20 * log10(sum);
		CurrText = CurrText + "\n\n The calculated value of passband attenuation is: " + doubletostring(a);

		if (a > Ap) {
			CurrText = CurrText + " which is greater than what was specified. Hence we'll increase filter order and recalculate. ";
			M += 2;
			CurrText = CurrText + " \n\n New value of filter length is: " + doubletostring(M);
			goto StartRect;

		}
		else {
			CurrText = CurrText + " which is smaller than what was specified. Hence this filter will work. ";
		}
	}

	else {								//M is odd - Type 1 filter

										//sum += a(k)*cos(kw) for k=0 to (M-1)/2; a(0) = Hm(tau), a(k) = 2*Hm(tau-k)

		sum += Hm[((M - 1) / 2)];
		for (int i = 1; i <= ((M - 1) / 2); i++) {
			sum += (2 * Hm[(((M - 1) / 2) - i)] * cos(omegaP[0] * i));
		}


		a = -20 * log10(sum);
		CurrText = CurrText + "\n\n The calculated value of passband attenuation is: " + doubletostring(a);

		if (a > Ap) {
			CurrText = CurrText + " which is greater than what was specified. Hence we'll increase filter order and recalculate. ";
			M += 2;
			CurrText = CurrText + " \n\n New value of filter length is: " + doubletostring(M);
			goto StartRect;

		}
		else {
			CurrText = CurrText + " which is smaller than what was specified. Hence this filter will work. ";
		}
	}

	
	OutputBlock->Text = CurrText;
	return;
}
//design using triangular window
void SDKSample::ListViewSimple::Scenario7::Triangular(int arg)
{
	//announce to user:
	CurrText = CurrText + "\n Using Triangular Window: \n\n";
	OutputBlock->Text = CurrText;

	//step 6: obtain length of filter
	M = ceil((Triwin.ExactTransitionWidth / deltaOmega));
StartTri:	Hm = new double[M];
	tau = (M - 1) * 0.5;
	 
	//show to user
	CurrText = CurrText + "\n Transition Width: " + doubletostring(deltaOmega) + "\n Length of Filter:  " + doubletostring(M);
	OutputBlock->Text = CurrText;

	//step 7: compute truncated filter function
	for (int i = 0; i < M; i++)
	{
		switch (arg)
		{

		case 0:
			Hm[i] = trianglewindow(i) * LPF(i);
			break;
		case 1:
			Hm[i] = trianglewindow(i) * HPF(i);
			break;
		default:
			break;
		}


	}



	//show Hm to user
	CurrText = CurrText + "\n Filter coefficients: ";
	OutputBlock->Text = CurrText;
	for (int i = 0; i < M; i++) {
		CurrText = CurrText + "\n Hm[" + doubletostring(i) + "] = " + doubletostring(Hm[i]);
		OutputBlock->Text = CurrText;
	}

	//check if this filter satisfies conditions
	
	double a, sum = 0;

	if ((M - 1) % 2 != 0) {				//M is even=> Type 2 filter.

										//sum += b(k)*cos(kw) for k=1 to M/2, 

		for (int i = 1; i <= (M / 2); i++) {
			sum += 2 * Hm[((M / 2) - i)] * cos((i - 0.5)*omegaP[0]);
		}

		a = -20 * log10(sum);
		CurrText = CurrText + "\n\n The calculated value of passband attenuation is: " + doubletostring(a);

		if (a > Ap) {
			CurrText = CurrText + " which is greater than what was specified. Hence we'll increase filter order and recalculate. ";
			M += 2;
			CurrText = CurrText + " \n\n New value of filter length is: " + doubletostring(M);
			goto StartTri;

		}
		else {
			CurrText = CurrText + " which is smaller than what was specified. Hence this filter will work. ";
		}
	}

	else {								//M is odd - Type 1 filter

										//sum += a(k)*cos(kw) for k=0 to (M-1)/2; a(0) = Hm(tau), a(k) = 2*Hm(tau-k)

		sum += Hm[((M - 1) / 2)];
		for (int i = 1; i <= ((M - 1) / 2); i++) {
			sum += (2 * Hm[(((M - 1) / 2) - i)] * cos(omegaP[0] * i));
		}


		a = -20 * log10(sum);
		CurrText = CurrText + "\n\n The calculated value of passband attenuation is: " + doubletostring(a);

		if (a > Ap) {
			CurrText = CurrText + " which is greater than what was specified. Hence we'll increase filter order and recalculate. ";
			M += 2;
			CurrText = CurrText + " \n\n New value of filter length is: " + doubletostring(M);
			goto StartTri;

		}
		else {
			CurrText = CurrText + " which is smaller than what was specified. Hence this filter will work. ";
		}
	}

	OutputBlock->Text = CurrText;
	return;
}

//design using hann window
void SDKSample::ListViewSimple::Scenario7::Hann(int arg)
{
	//announce to user:
	CurrText = CurrText + "\n Using Hanning Window: \n\n";
	OutputBlock->Text = CurrText;

	//step 6: obtain length of filter
	M = ceil((Hanwin.ExactTransitionWidth / deltaOmega));
StartHan:	Hm = new double[M];
	tau = (M - 1) * 0.5;

	//show to user
	CurrText = CurrText + "\n Transition Width: " + doubletostring(deltaOmega) + "\n Length of Filter:  " + doubletostring(M);
	OutputBlock->Text = CurrText;

	//step 7: compute truncated filter function
	for (int i = 0; i < M; i++)
	{
		switch (arg)
		{

		case 0:
			Hm[i] = hannwindow(i) * LPF(i);
			break;
		case 1:
			Hm[i] = hannwindow(i) * HPF(i);
			break;
		default:
			break;
		}


	}



	//show Hm to user
	CurrText = CurrText + "\n Filter coefficients  : ";
	OutputBlock->Text = CurrText;
	for (int i = 0; i < M; i++) {
		CurrText = CurrText + "\n Hm[" + doubletostring(i) + "] = " + doubletostring(Hm[i]);
		OutputBlock->Text = CurrText;
	}

	//check if this filter satisfies conditions
	
	double a, sum = 0;

	if ((M - 1) % 2 != 0) {				//M is even=> Type 2 filter.

										//sum += b(k)*cos(kw) for k=1 to M/2, 

		for (int i = 1; i <= (M / 2); i++) {
			sum += 2 * Hm[((M / 2) - i)] * cos((i - 0.5)*omegaP[0]);
		}

		a = -20 * log10(sum);
		CurrText = CurrText + "\n\n The calculated value of passband attenuation is: " + doubletostring(a);

		if (a > Ap) {
			CurrText = CurrText + " which is greater than what was specified. Hence we'll increase filter order and recalculate. ";
			M += 2;
			CurrText = CurrText + " \n\n New value of filter length is: " + doubletostring(M);
			goto StartHan;

		}
		else {
			CurrText = CurrText + " which is smaller than what was specified. Hence this filter will work. ";
		}
	}

	else {								//M is odd - Type 1 filter

										//sum += a(k)*cos(kw) for k=0 to (M-1)/2; a(0) = Hm(tau), a(k) = 2*Hm(tau-k)

		sum += Hm[((M - 1) / 2)];
		for (int i = 1; i <= ((M - 1) / 2); i++) {
			sum += (2 * Hm[(((M - 1) / 2) - i)] * cos(omegaP[0] * i));
		}


		a = -20 * log10(sum);
		CurrText = CurrText + "\n\n The calculated value of passband attenuation is: " + doubletostring(a);

		if (a > Ap) {
			CurrText = CurrText + " which is greater than what was specified. Hence we'll increase filter order and recalculate. ";
			M += 2;
			CurrText = CurrText + " \n\n New value of filter length is: " + doubletostring(M);
			goto StartHan;

		}
		else {
			CurrText = CurrText + " which is smaller than what was specified. Hence this filter will work. ";
		}
	}


	OutputBlock->Text = CurrText;
	return;
}

//design using hamming window
void SDKSample::ListViewSimple::Scenario7::Hamming(int arg)
{
	//announce to user:
	CurrText = CurrText + "\n Using Hamming Window: \n\n";
	OutputBlock->Text = CurrText;

	//step 6: obtain length of filter
	M = ceil((Hamwin.ExactTransitionWidth / deltaOmega));
StartHam:	Hm = new double[M];
	tau = (M - 1) * 0.5 ;

	//show to user
	CurrText = CurrText + "\n Transition Width: " + doubletostring(deltaOmega) + "\n Length of Filter:  " + doubletostring(M);
	OutputBlock->Text = CurrText;

	//step 7: compute truncated filter function
	for (int i = 0; i < M; i++)
	{
		switch (arg)
		{

		case 0:
			Hm[i] = hammingwindow(i) * LPF(i);
			break;
		case 1:
			Hm[i] = hammingwindow(i) * HPF(i);
			break;
		default:
			break;
		}


	}



	//show Hm to user
	CurrText = CurrText + "\n Filter coefficients  : ";
	OutputBlock->Text = CurrText;
	for (int i = 0; i < M; i++) {
		CurrText = CurrText + "\n Hm[" + doubletostring(i) + "] = " + doubletostring(Hm[i]);
		OutputBlock->Text = CurrText;
	}

	//check if this filter satisfies conditions
	double a, sum = 0;

	if ((M - 1) % 2 != 0) {				//M is even=> Type 2 filter.

						//sum += b(k)*cos(kw) for k=1 to M/2, 
		
		for (int i = 1; i <= (M / 2); i++) {
			sum += 2 * Hm[((M / 2) - i)] * cos((i - 0.5)*omegaP[0]);
		}

		a = -20 * log10(sum);
		CurrText = CurrText + "\n\n The calculated value of passband attenuation is: " + doubletostring(a);

		if (a > Ap) {
			CurrText = CurrText + " which is greater than what was specified. Hence we'll increase filter order and recalculate. ";
			M += 2;
			CurrText = CurrText + " \n\n New value of filter length is: " + doubletostring(M);
			goto StartHam;

		}
		else {
			CurrText = CurrText + " which is smaller than what was specified. Hence this filter will work. ";
		}
	}

	else {								//M is odd - Type 1 filter

						//sum += a(k)*cos(kw) for k=0 to (M-1)/2; a(0) = Hm(tau), a(k) = 2*Hm(tau-k)

		sum += Hm[((M - 1) / 2)];
		for (int i = 1; i <= ((M - 1) / 2); i++) {
			sum += (2 * Hm[(((M - 1) / 2) - i)] * cos(omegaP[0] * i));
		}


		a = -20 * log10(sum);
		CurrText = CurrText + "\n\n The calculated value of passband attenuation is: " + doubletostring(a);

		if (a > Ap) {
			CurrText = CurrText + " which is greater than what was specified. Hence we'll increase filter order and recalculate. ";
			M += 2;
			CurrText = CurrText + " \n\n New value of filter length is: " + doubletostring(M);
			goto StartHam;

		}
		else {
			CurrText = CurrText + " which is smaller than what was specified. Hence this filter will work. ";
		}
	}

	OutputBlock->Text = CurrText;
	return;
}

//design using blackman window
void SDKSample::ListViewSimple::Scenario7::Blackman(int arg)
{
	//announce to user:
	CurrText = CurrText + "\n Using Blackman Window: \n\n";
	OutputBlock->Text = CurrText;

	//step 6: obtain length of filter
	M = ceil((Blackwin.ExactTransitionWidth / deltaOmega));
StartBlack:	Hm = new double[M];
	tau = (M - 1) * 0.5;

	//show to user
	CurrText = CurrText + "\n Transition Width: " + doubletostring(deltaOmega) + "\n Length of Filter:  " + doubletostring(M);
	OutputBlock->Text = CurrText;

	//step 7: compute truncated filter function
	for (int i = 0; i < M; i++)
	{
		switch (arg)
		{

		case 0:
			Hm[i] = blackmanwindow(i) * LPF(i);
			break;
		case 1:
			Hm[i] = blackmanwindow(i) * HPF(i);
			break;
		default:
			break;
		}


	}



	//show Hm to user
	CurrText = CurrText + "\n Filter coefficients  : ";
	OutputBlock->Text = CurrText;
	for (int i = 0; i < M; i++) {
		CurrText = CurrText + "\n Hm[" + doubletostring(i) + "] = " + doubletostring(Hm[i]);
		OutputBlock->Text = CurrText;
	}

	//check if this filter satisfies conditions
	double a, sum = 0;

	if ((M - 1) % 2 != 0) {				//M is even=> Type 2 filter.

										//sum += b(k)*cos(kw) for k=1 to M/2, 

		for (int i = 1; i <= (M / 2); i++) {
			sum += 2 * Hm[((M / 2) - i)] * cos((i - 0.5)*omegaP[0]);
		}

		a = -20 * log10(sum);
		CurrText = CurrText + "\n\n The calculated value of passband attenuation is: " + doubletostring(a);

		if (a > Ap) {
			CurrText = CurrText + " which is greater than what was specified. Hence we'll increase filter order and recalculate. ";
			M += 2;
			CurrText = CurrText + " \n\n New value of filter length is: " + doubletostring(M);
			goto StartBlack;

		}
		else {
			CurrText = CurrText + " which is smaller than what was specified. Hence this filter will work. ";
		}
	}

	else {								//M is odd - Type 1 filter

										//sum += a(k)*cos(kw) for k=0 to (M-1)/2; a(0) = Hm(tau), a(k) = 2*Hm(tau-k)

		sum += Hm[((M - 1) / 2)];
		for (int i = 1; i <= ((M - 1) / 2); i++) {
			sum += (2 * Hm[(((M - 1) / 2) - i)] * cos(omegaP[0] * i));
		}


		a = -20 * log10(sum);
		CurrText = CurrText + "\n\n The calculated value of passband attenuation is: " + doubletostring(a);

		if (a > Ap) {
			CurrText = CurrText + " which is greater than what was specified. Hence we'll increase filter order and recalculate. ";
			M += 2;
			CurrText = CurrText + " \n\n New value of filter length is: " + doubletostring(M);
			goto StartBlack;

		}
		else {
			CurrText = CurrText + " which is smaller than what was specified. Hence this filter will work. ";
		}
	}

	OutputBlock->Text = CurrText;

	return;
}




//utility function

String^ doubletostring(double n) {


	string str = to_string(n);
	std::wstring wstr = std::wstring(str.begin(), str.end());
	const wchar_t* tem3 = wstr.c_str();
	String^ res = ref new String(tem3);
	return res;

}


String^ doubletostring(int n) {


	string str = to_string(n);
	std::wstring wstr = std::wstring(str.begin(), str.end());
	const wchar_t* tem3 = wstr.c_str();
	String^ res = ref new String(tem3);
	return res;

}


//window utility functions

double rectanglewindow(int arg) 
{
	if ((arg >= 0) && (arg <= (M - 1)))
		return 1;
	else 
		return 0;
}

double trianglewindow(int arg) 
{
	if ((arg >= 0) && (arg <= (M - 1)))
	{
		double ans = 2 * (abs(arg - tau) / (M-1));
		return (1 - ans);
	}
	else
		return 0;
}

double hannwindow(int arg) 
{
	if ((arg >= 0) && (arg <= (M - 1)))
	{
		double ans = 0.5*cos(2 * PI*arg / (M - 1));
		return (0.5 - ans);
	}
	else
		return 0;
}

double hammingwindow(int arg)
{
	if ((arg >= 0) && (arg <= (M - 1)))
	{
		double ans = 0.46*cos(2 * PI*arg / (M - 1));
		return (0.54 - ans);
	}
	else
		return 0;
}

double blackmanwindow(int arg)
{
	if ((arg >= 0) && (arg <= (M - 1)))
	{
		double ans = 0.5*cos(2 * PI*arg / (M - 1)) - 0.08*cos(4*PI*arg/(M-1));
		return (0.42 - ans);
	}
	else
		return 0;
}


//filter utility functions

double LPF(int arg)
{
	if (tau == arg) {
		return (omegaC[0] / PI);
	}
	else {
		double temp = sin(omegaC[0] * (arg - tau)) / (PI*(arg - tau));
		return temp;
	}
}

double HPF(int arg)
{
	double ans;
	if (tau == arg) {
		ans = (1 - (omegaC[0] / PI));
	}
	else {
		double temp1 = (sin(PI*(arg - tau))) / (PI*(arg - tau));
		double temp2 = (sin(omegaC[0]*(arg - tau))) / (PI*(arg - tau));
		ans =  (temp1-temp2);
	}

	return ans;
}



void SDKSample::ListViewSimple::Scenario7::ClearConsoleButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
		OutputBlock->Text = "***FIR FILTER DESIGN UTILITY***\n\n";
		return;
}
