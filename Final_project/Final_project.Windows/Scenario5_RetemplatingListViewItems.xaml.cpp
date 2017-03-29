//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
//
//*********************************************************

//
// Scenario5.xaml.cpp
// Implementation of the Scenario5 class
//

#include "pch.h"
#include "Scenario5_RetemplatingListViewItems.xaml.h"
#include <unordered_map>
#include <fstream>
#include <math.h>
#include <string>







using namespace SDKSample::ListViewSimple;
using namespace concurrency;
using namespace Concurrency;
using namespace std;
using namespace Platform;
using namespace Windows::Storage;
using namespace Windows::Storage::Pickers;
using namespace Windows::Storage::Streams;

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

Scenario5::Scenario5()
{
    InitializeComponent();
	
	FileOpenButton->Click += ref new RoutedEventHandler(this, &Scenario5::FileOpenButton_Click);
	AssembleButton->Click += ref new RoutedEventHandler(this, &Scenario5::AssembleButton_Click);
	DisplayOutputButton->Click += ref new RoutedEventHandler(this, &Scenario5::DisplayOutputButton_Click);
	ConsoleOutputBlock->Text = " ";		//empty the console output

	FileOpenButton->IsEnabled = true;
	AssembleButton->IsEnabled = false;		//lock down assemble button until file is picked
	DisplayOutputButton->IsEnabled = false;	//obviously.
}

/// <summary>
/// Invoked when this page is about to be displayed in a Frame.
/// </summary>
/// <param name="e">Event data that describes how this page was reached.  The Parameter
/// property is typically used to configure the page.</param>
void Scenario5::OnNavigatedTo(NavigationEventArgs^ e)
{
    // A pointer back to the main page.  This is needed if you want to call methods in MainPage such
    // as NotifyUser()
    rootPage = MainPage::Current;
}


int counter = 0;
std::fstream outfile;	//output file (read for display at end)
std::fstream infile;
String^ ConsoleText;	//output console text
String^ filename;
Platform::String^ userContent;	//textbox content

//utility functions
unsigned short f2w(float data);
int length_of(int temp);
unsigned short int b_to_d(string a);           //to convert binary data into decimal
string d_to_b(int num);                              //to convert decimal data into binary
string d_to_b4(int num);
string before_space(string temp);                     //to detect the opcode
int check_space(string temp);
String^ inttoString(int n);
String^ stringtoString(string in);
std::string to_string(const std::wstring &wtxt);
std::string to_string(Platform::String^ pstring);
//#define XAML_GENERATED_BREAK_ON_UNHANDLED_EXCEPTION;



void SDKSample::ListViewSimple::Scenario5::FileOpenButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	ConsoleText = " ";
	ConsoleOutputBlock->Text = " ";			//clear textbox
	AssembleButton->IsEnabled = false;		//lock down assemble button until file is picked
	DisplayOutputButton->IsEnabled = false;	//obviously.

	FileOpenPicker^ picker = ref new FileOpenPicker();
	picker->ViewMode = Windows::Storage::Pickers::PickerViewMode::Thumbnail;
	picker->SuggestedStartLocation = Windows::Storage::Pickers::PickerLocationId::DocumentsLibrary;
	picker->FileTypeFilter->Append(".txt");
	//Windows::Storage::StorageFile file = await picker->PickSingleFileAsync();

	create_task(picker->PickSingleFileAsync()).then([this](StorageFile^ file) 
	{
		if (file)			//if opened a valid file
		{
			
			ConsoleText += "\nFile " + file->Path + " Selected.\n";
			ConsoleOutputBlock->Text = ConsoleText;
			
				//get path of file and store
			filename = file->Path;
			Windows::Storage::StorageFolder^ localFolder = Windows::Storage::ApplicationData::Current->LocalFolder;		
			Platform::String^ destfilename = file->Name + ".copy";
			auto option = Windows::Storage::NameCollisionOption::ReplaceExisting;

			create_task(file->CopyAsync(localFolder, destfilename, option)).then([=](StorageFile^ sampleFileCopy) {

				//get ascii version of copied file path
				std::string localPath = to_string(sampleFileCopy->Path);
				infile.open(localPath.data(), ios::in);
		

			//open file for ifstream object
			

			if (!infile.is_open())
			{
				ConsoleText += "\n File not opened properly. Please try again.\n";
				ConsoleOutputBlock->Text = ConsoleText;
				return;
			}

			else
			{
				ConsoleText += "\n File " + filename + " successfully opened. \n";
				ConsoleOutputBlock->Text = ConsoleText;
				AssembleButton->IsEnabled = true;		//file picked successfully - enable button.
			}

			
			});
			
		}
		else {
			ConsoleOutputBlock->Text += "\n Operation Cancelled";

		}

	});

	
}


void SDKSample::ListViewSimple::Scenario5::AssembleButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	//disable buttons
	FileOpenButton->IsEnabled = false;
	AssembleButton->IsEnabled = false;
	rootPage->NotifyUser("Working.",NotifyType::ErrorMessage);

	unordered_map<string, string>my_map;
	unordered_map<string, string>option2;
	my_map["NOP"] = "000000";
	my_map["RET"] = "000010";
	my_map["HLT"] = "000100";
	my_map["CMP"] = "000110";
	my_map["INC"] = "001000";
	my_map["DEC"] = "001010";
	my_map["ADD"] = "001100";
	my_map["ADD.I"] = "001101";
	my_map["SUB"] = "001110";
	my_map["SUB.I"] = "001111";
	my_map["MUL"] = "010000";
	my_map["MUL.I"] = "010001";
	my_map["MAC"] = "010010";
	my_map["AND"] = "010100";
	my_map["AND.I"] = "010101";
	my_map["OR"] = "010110";
	my_map["OR.I"] = "010111";
	my_map["XOR"] = "011000";
	my_map["XOR.I"] = "011001";
	my_map["SHL"] = "011010";
	my_map["SHL.I"] = "011011";
	my_map["SHR"] = "011100";
	my_map["INC"] = "001000";
	my_map["INC.IN"] = "001001";
	my_map["DEC"] = "001010";
	my_map["DEC.IN"] = "001011";
	my_map["SHR.I"] = "011101";
	my_map["ADD.IN"] = "110100";
	my_map["ADD.IN.I"] = "110101";
	my_map["SUB.IN"] = "110110";
	my_map["SUB.IN.I"] = "110111";
	my_map["MOV.IN.I"] = "111000";
	my_map["IN.IN"] = "111010";
	my_map["OUT.IN"] = "111100";
	my_map["MOV"] = "011110";
	my_map["MOV.I"] = "011111";
	my_map["LOD"] = "100000";
	my_map["LOD.RI"] = "100001";
	my_map["LOD.BR"] = "100010";
	my_map["LOD.CR"] = "100011";
	my_map["STR"] = "100100";
	my_map["STR.RI"] = "100101";
	my_map["STR.BR"] = "100110";
	my_map["STR.CR"] = "100111";
	my_map["IN"] = "101000";
	my_map["IN.A"] = "101001";
	my_map["OUT"] = "101010";
	my_map["JMP"] = "101100";
	my_map["OUT.A"] = "101011";
	my_map["JMP.RI"] = "101101";
	my_map["JMP.Z"] = "101100";
	my_map["JMP.Z.RI"] = "101101";
	my_map["JMP.NZ.RI"] = "101101";
	my_map["JMP.NZ"] = "101100";
	my_map["JMP.N"] = "101100";
	my_map["JMP.N.RI"] = "101101";
	my_map["JMP.NN"] = "101100";
	my_map["JMP.NN.RI"] = "101101";
	my_map["JMP.C"] = "101100";
	my_map["JMP.C.RI"] = "101101";
	my_map["JMP.NC"] = "101100";
	my_map["JMP.NC.RI"] = "101101";
	my_map["CALL"] = "101110";
	my_map["CALL.RI"] = "101111";
	my_map["PUSH"] = "110000";
	my_map["POP"] = "110010";
	option2["JMP"] = "000";
	option2["JMP.RI"] = "000";
	option2["JMP.Z"] = "001";
	option2["JMP.Z.RI"] = "001";
	option2["JMP.NZ"] = "010";
	option2["JMP.NZ.RI"] = "010";
	option2["JMP.N"] = "011";
	option2["JMP.N.RI"] = "011";
	option2["JMP.NN"] = "100";
	option2["JMP.NN.RI"] = "100";
	option2["JMP.C"] = "101";
	option2["JMP.C.RI"] = "101";
	option2["JMP.NC"] = "110";
	option2["JMP.NC.RI"] = "110";


	std::string temp;
//	char* temp2 = new char[256];
	

	counter = 0;

	while (std::getline(infile, temp))// && (temp != "HLT"))
	{
		//temp = temp2;
		//std::getline(infile, temp);
		while (temp[temp.length() - 1] == '\r' || temp[temp.length() - 1] == ' ' || temp[temp.length() - 1] == '\n')
		{
			temp.resize(temp.length() - 1);
		}


		if (temp.length() == 0)
		{
			break;
		}
		counter++;
		if (my_map.find(before_space(temp)) == my_map.end())
		{
			ConsoleText = ConsoleText + "Error in line: " + counter + "\n" + "Type of error: Invalid instruction " + stringtoString(temp) + "\n";
			ConsoleOutputBlock->Text = ConsoleText;

			userContent = "";

			break;
		}

		if (check_space(temp) == 0)
		{

			userContent += inttoString(b_to_d(my_map[temp])) + "\n";
			continue;
		}
		else if (check_space(temp) == 1)
		{
			string tp4 = before_space(temp);
			// string tp4=before_space(temp);
			if (tp4 == "JMP.RI" || tp4 == "JMP.Z.RI" || tp4 == "JMP.NZ.RI" || tp4 == "JMP.N.RI" || tp4 == "JMP.NN.RI" || tp4 == "JMP.C.RI" || tp4 == "JMP.NC.RI" || tp4 == "CALL.RI")
			{
				string tp5 = my_map[before_space(temp)];
				int index = 0;
				while (temp[index] != ' ')
					index++;
				if (temp[index + 1] != 'R')
				{
					ConsoleText += "Error in line: " + inttoString(counter) + "\n" + "Type of error: Invalid instruction " + stringtoString(temp) + "\n";
					ConsoleOutputBlock->Text = ConsoleText;
					userContent = "";
					break;
				}
				index += 2;


				int num2 = 0;
				while (index < temp.length())
				{
					num2 = num2 * 10 + (temp[index] - '0');
					if (temp[index] > '9' || temp[index] < '0')
					{
						ConsoleText += "Error in line: " + inttoString(counter) + "\n" + "Type of error: Invalid Register" + "\n";
						ConsoleOutputBlock->Text = ConsoleText;
						userContent = "";
						break;
					}
					index++;
				}
				if (num2 > 31)
				{
					ConsoleText += "Error in line: " + inttoString(counter) + "\n" + "Type of error: Invalid Register " + inttoString(num2) + "\n";
					ConsoleOutputBlock->Text = ConsoleText;
					userContent = "";
					break;
				}
				userContent += inttoString( b_to_d(tp5 + d_to_b(num2) + option2[before_space(temp)] + "00")) + "\n";

			}
			else
			{

				int index = 0;

				while (temp[index] != ' ')
					index++;
				index++;
				bool sign = true;
				if (temp[index] == '-')
				{
					sign = false;
					index++;
				}
				if (temp[index] >= '0'&&temp[index] <= '9')
				{
					string tp2;
					if (option2.find(before_space(temp)) != option2.end())
						tp2 = my_map[before_space(temp)] + "00000" + option2[before_space(temp)] + "00";
					else
						tp2 = my_map[before_space(temp)] + "0000000000";

					userContent+= inttoString( b_to_d(tp2)) + "\n";

					// outfile<<b_to_d(tp2)<<"\n";
					int sum = 0;
					int sum1 = 0;
					bool flag = false;

					while (index < temp.length())
					{
						if (temp[index] == '.')
						{
							index++;
							while (index < temp.length())
							{
								if (temp[index] > '9' || temp[index] < '0')
								{
									ConsoleText += "Error in line: " + inttoString(counter) + "\n" + "Type of error: Invalid data" + "\n";
									ConsoleOutputBlock->Text = ConsoleText;
									userContent = "";
									break;
								}

								sum1 = sum1 * 10 + (temp[index] - '0');
								index++;
							}
							break;
						}
						else if (temp[index] > '9' || temp[index] < '0')
						{
							ConsoleText += "Error in line: " + inttoString(counter) + "\n" + "Type of error: Invalid data" + "\n";
							ConsoleOutputBlock->Text = ConsoleText;
							userContent = "";
							break;
						}

						sum = sum * 10 + (temp[index] - '0');
						index++;
					}
					if (sum1 == 0)
					{
						if (sum > (pow(2, 16) - 1))
						{
							ConsoleText += "Error in line: " + inttoString(counter) + "\n" + "Type of error: Invalid address" + "\n";
							ConsoleOutputBlock->Text = ConsoleText;
							userContent = "";
							break;
						}
						userContent+= inttoString( sum) + "\n";
					}
					else
					{
						double final_sum = sum*(pow(10, length_of(sum1))) + sum1;
						final_sum = final_sum / (pow(10, length_of(sum1)));
						if (!sign)
							final_sum = -final_sum;
						userContent += inttoString( f2w(final_sum)) + "\n";
					}


				}
				else
				{
					if (temp[index] != 'R')
					{
						ConsoleText += "Error in line: " + inttoString(counter) + "\n" + "Type of error: Invalid instruction " + stringtoString(temp) + "\n";
						ConsoleOutputBlock->Text = ConsoleText;
						userContent = "";
						break;
					}
					index++;
					int sum2 = 0;
					while (index < temp.length())
					{
						if (temp[index] > '9' || temp[index] < '0')
						{


							{throw_error();



							break; }

						}
						sum2 = sum2 * 10 + (temp[index] - '0');
						index++;
					}
					if (sum2 > 31)
					{
						ConsoleText += "Error in line: " + inttoString(counter) + "\n" + "Type of error: Invalid register" + "\n";
						ConsoleOutputBlock->Text = ConsoleText;
						userContent = "";
						break;
					}
					string tp2 = my_map[before_space(temp)];
					string tp3 = d_to_b(sum2);
					userContent += inttoString( b_to_d(tp2 + tp3))  + "\n";
				}


			}
		}
		else if (check_space(temp) == 2)
		{
			int index = 0;
			while (temp[index] != ' ')
				index++;
			int nor = 0;
			while (index < temp.length())
			{
				if (temp[index] == 'R')
					nor++;
				index++;
			}
			if (nor == 2)
			{
				int num1 = 0;
				int num2 = 0;
				index = 0;
				while (temp[index] != ' ')
					index++;
				if (temp[index + 1] != 'R')
				{
					ConsoleText += "Error in line: " + inttoString(counter) + "\n" + "Type of error: Invalid instruction " + stringtoString(temp) + "\n";
					ConsoleOutputBlock->Text = ConsoleText;
					userContent = "";
					break;
				}
				index += 2;
				while (temp[index] != ' ')
				{
					if (temp[index] > '9' || temp[index] < '0')
					{
						ConsoleText += "Error in line: " + inttoString(counter) + "\n" + "Type of error: Invalid register" + "\n";
						ConsoleOutputBlock->Text = ConsoleText;
						userContent = "";
						break;
					}
					num1 = num1 * 10 + (temp[index] - '0');
					index++;
				}
				if (temp[index + 1] != 'R')
				{
					ConsoleText += "Error in line: " + inttoString(counter) + "\n" + "Type of error: Invalid register" + "\n";
					ConsoleOutputBlock->Text = ConsoleText;
					userContent = "";
					break;
				}

				index += 2;
				while (index < temp.length())
				{
					if (temp[index] > '9' || temp[index] < '0')
					{

						throw_error();

						break;

					}
					num2 = num2 * 10 + (temp[index] - '0');
					index++;
				}
				if (num1 > 31 || num2 > 32)
				{
					ConsoleText += "Error in line: " + inttoString(counter) + "\n" + "Type of error: Invalid register" + "\n";
					ConsoleOutputBlock->Text = ConsoleText;
					userContent = "";
					break;
				}
				userContent += inttoString( b_to_d(my_map[before_space(temp)] + d_to_b(num1) + d_to_b(num2))) + "\n";


			}
			else if (nor == 1)
			{
				int index = 0;
				while (temp[index] != ' ')
					index++;
				if (temp[index + 1] != 'R')
				{
					ConsoleText += "Error in line: " + inttoString(counter) + "\n" + "Type of error: Invalid instruction " + stringtoString(temp) + "\n";
					ConsoleOutputBlock->Text = ConsoleText;
					userContent = "";
					break;
				}
				index += 2;
				int num1 = 0;
				while (temp[index] != ' ')
				{
					if (temp[index] > '9' || temp[index] < '0')
					{
						ConsoleText += "Error in line: " + inttoString(counter) + "\n" + "Type of error: Invalid instruction " + stringtoString(temp) + "\n";
						ConsoleOutputBlock->Text = ConsoleText;
						userContent = "";
						break;
					}
					num1 = num1 * 10 + (temp[index] - '0');
					index++;
				}
				if (num1 > 31)
				{
					ConsoleText += "Error in line: " + inttoString(counter) + "\n" + "Type of error: Invalid register" + "\n";
					ConsoleOutputBlock->Text = ConsoleText;
					userContent = "";
					break;
				}

				unsigned short int num2 = 0;
				index++;
				if (temp[index] == '-')
					index++;
				double num3 = 0;
				bool flag = false;
				while (index < temp.length())
				{
					if (temp[index] == '.')
					{
						num3 = make_double(temp);
						flag = true;
						break;
					}


					else if (temp[index] > '9' || temp[index] < '0')
					{
						ConsoleText += "Error in line: " + inttoString(counter) + "\n" + "Type of error: Invalid instruction " + stringtoString(temp) + "\n";
						ConsoleOutputBlock->Text = ConsoleText;
						userContent = "";
						break;
					}

					num2 = num2 * 10 + (temp[index] - '0');
					index++;
				}
				if (before_space(temp) == "LOD.BR" || before_space(temp) == "STR.BR")
				{

					userContent += inttoString( b_to_d(my_map[before_space(temp)] + d_to_b(num1) + d_to_b4(num2) + "0")) + "\n";

				}

				else
				{

					userContent +=  inttoString( b_to_d(my_map[before_space(temp)] + d_to_b(num1) + "00000")) + "\n";
					if (!flag)
						userContent += inttoString( num2) + "\n";
					else
					{
						userContent += inttoString( f2w(num3)) + "\n";
					}


				}
			}
			else
			{
				ConsoleText += "Error in line: " + inttoString(counter) + "\n" + "Type of error: Invalid instruction " + stringtoString(temp) + "\n";
				ConsoleOutputBlock->Text = ConsoleText;
				userContent = "";
				break;
			}


		}
		else
		{
			ConsoleText += "Error in line: " + inttoString(counter) + "\n" + "Type of error: Invalid instruction " + stringtoString(temp) + "\n";
			ConsoleOutputBlock->Text = ConsoleText;
			userContent = "";
			break;
		}


		

	}
	UpdateOutputFile();
	//verify all text has been written to screen
	ConsoleOutputBlock->Text = ConsoleText;
	//enable buttons
	FileOpenButton->IsEnabled = true;
	AssembleButton->IsEnabled = true;
	DisplayOutputButton->IsEnabled = true;
	rootPage->NotifyUser("Done.", NotifyType::StatusMessage);
		
}

unsigned short int f2w(float data)
{
	unsigned short int y;
	int sign, mantissa, exp;
	
	//float to word
	if (data >= 0){
		sign = 0;
	}
	else{
		data = -data;
		sign = 1;
	}

	exp = 0;
	int flag = 1;
	while (flag){
		if (data>0.998047){
			data = data / 2;
			exp++;
		}
		else if (data<0.00195312 && data != 0){
			data = data * 2;
			exp--;
		}
		else{
			flag = 0;
		}

	}

	//data to mantissa

	mantissa = 0;

	for (int i = -1; i >= -9; i--){
		if (data >= pow(2.0, i))
		{
			data = data - pow(2.0, i);
			mantissa++;
		}

		mantissa = mantissa << 1;
	}

	mantissa = mantissa >> 1;
	exp = exp + 32;
	y = sign;

	y = y << 9;

	y = y | mantissa;

	y = y << 6;

	y = y | exp;
	
	return(y);

}


int length_of(int temp)
{
	int length = 0;
	if (temp == 0)
		return 1;
	while (temp)
	{
		length++;
		temp /= 10;
	}
	return length;
}
void SDKSample::ListViewSimple::Scenario5::throw_error()                              //Function which displays errors
{
	ConsoleText += "error in line: " + inttoString(counter) + "\n";
	ConsoleOutputBlock->Text = ConsoleText;
	return;
}
unsigned short int b_to_d(string a)             //to convert binary data into decimal
{
	unsigned short int ans = 0;
	for (int i = 0; i < a.length(); i++)
	{
		ans += (a[i] - '0')*pow(2, 15 - i);
	}
	return ans;
}
float SDKSample::ListViewSimple::Scenario5::make_double(string temp)                 //to detect float values in instruction
{																					 //declared in .h file because it uses console.
	int index = 0;
	int nos = 0;
	while (index<temp.length() && nos<2)
	{
		if (temp[index] == ' ')
			nos++;
		index++;
	}
	int num1 = 0;
	bool sign = true;
	if (temp[index] == '-')
	{
		sign = false;
		index++;
	}
	while (temp[index] != '.')
	{
		if (temp[index]>'9' || temp[index]<'0')
		{
			ConsoleText += "Error in line: " + inttoString(counter) + "\n" + "Type of error: Invalid instruction" + "\n";
			ConsoleOutputBlock->Text = ConsoleText;
			ofstream outfile("output.txt");
			return 0;
		}
		num1 = num1 * 10 + (temp[index] - '0');
		index++;
	}
	index++;
	int num2 = 0;
	while (index<temp.length())
	{
		num2 = num2 * 10 + (temp[index] - '0');
		index++;
	}
	num1 = num1*pow(10, (length_of(num2))) + num2;
	float num3 = (num1*1.0000) / ((pow(10, length_of(num2)))*1.0000);
	if (!sign)
		num3 = -num3;

	return num3;


}
string d_to_b(int num)                              //to convert decimal data into binary
{
	string a;
	for (int i = 4; i >= 0; i--)
	{
		int k = pow(2, i);
		if (num / k)
		{
			a.push_back('1');
			num = num - k;
		}
		else
			a.push_back('0');
	}
	return a;
}
string d_to_b4(int num)
{
	string a;
	for (int i = 3; i >= 0; i--)
	{
		int k = pow(2, i);
		if (num / k)
		{
			a.push_back('1');
			num = num - k;
		}
		else
			a.push_back('0');
	}
	return a;
}
string before_space(string temp)                        //to detect the opcode
{
	string ansr;
	int index = 0;
	while (index<temp.length() && temp[index] != ' ')
	{
		ansr.push_back(temp[index]);
		index++;
	}
	return ansr;
}
int check_space(string temp)
{
	int counter = 0;
	for (int i = 0; i<temp.length(); i++)
	{
		if (temp[i] == ' ')
			counter++;
	}
	return counter;
}

String^ stringtoString(string in) {
	std::wstring wstr = std::wstring(in.begin(), in.end());
	const wchar_t* tem3 = wstr.c_str();
	String^ res = ref new String(tem3);
	return res;

}

String^ inttoString(int n) {
	
	string str = to_string(n);
	std::wstring wstr = std::wstring(str.begin(), str.end());
	const wchar_t* tem3 = wstr.c_str();
	String^ res = ref new String(tem3);
	return res;

}

// Convert from wstring into a string
std::string to_string(const std::wstring & wtxt)
{
	std::string str;
	str.resize(wtxt.size() + 1);

	size_t numConverted;
	errno_t		err = wcstombs_s(&numConverted, (char *)str.data(), str.size(), wtxt.data(), wtxt.size());

	str.pop_back();
	return str;
}

std::string to_string(Platform::String^ pstring)
{
	std::wstring wStr(pstring->Data());
	return to_string(wStr);
}

void SDKSample::ListViewSimple::Scenario5::UpdateOutputFile()
{
		
	
	//create new file in app local folder:

	 create_task(Windows::Storage::ApplicationData::Current->LocalFolder->CreateFileAsync(rootPage->Filename, CreationCollisionOption::ReplaceExisting)).then([this](StorageFile^ file)
	{

			if (file != nullptr)
			{
				rootPage->SampleFile = file;
				//rootPage->NotifyUser("The file '" + file->Name + "' was created.", NotifyType::StatusMessage);
				ConsoleText += "The file " + file->Name + " was created. \n";
				ConsoleOutputBlock->Text = ConsoleText;
			}

			else return;
		
	}).then([this] 
	{
		//write data to file
		StorageFile^ file = rootPage->SampleFile;
			String^ userContentData = userContent;
			
				if (userContentData != nullptr && !userContentData->IsEmpty())
				{
					create_task(FileIO::WriteTextAsync(file, userContentData)).then([this, file, userContentData](task<void> task)
					{
						try
						{
							task.get();
							//rootPage->NotifyUser("The following text was written to '" + file->Name + "':\n" + userContent, NotifyType::StatusMessage);
							ConsoleText += "\n The output has been written to '" + file->Name + "': \n";
							ConsoleOutputBlock->Text = ConsoleText;
						}
						catch (COMException^ ex)
						{
							rootPage->HandleFileNotFoundException(ex);
						}
					});

				}
				else
				{
					//rootPage->NotifyUser("The text box is empty, please write something and then click 'Write' again.", NotifyType::ErrorMessage);
					ConsoleText += " The Output file is empty. There's some problem(s) with your code. \n";
					ConsoleOutputBlock->Text = ConsoleText;
					//ConsoleOutputBlock->Text = ConsoleText;
				}
		//	}
	}).then([this]
	{
		//copy file into documents library
		StorageFile^ file = rootPage->SampleFile;
		
			// Get the returned file and copy it into documents library
			StorageFolder^ documentsFolder = KnownFolders::DocumentsLibrary;
		//	if (file != nullptr)
		//	{
				create_task(file->CopyAsync(documentsFolder, "output.txt", NameCollisionOption::ReplaceExisting)).then([this, file](task<StorageFile^> task)
				{
					try
					{
						StorageFile^ sampleFileCopy = task.get();
						//rootPage->NotifyUser("The file '" + file->Name + "' was copied and the new file was named '" + sampleFileCopy->Name + "'.", NotifyType::StatusMessage);
						ConsoleText += "The file " + file->Name + " was successfully copied to your documents folder. \n";
						ConsoleOutputBlock->Text = ConsoleText;

					}
					catch (COMException^ ex)
					{
						rootPage->HandleFileNotFoundException(ex);
					}
				});


		//	}
	});
	


}

void SDKSample::ListViewSimple::Scenario5::DisplayOutputButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	//get file if it exists
//	rootPage->ValidateFile();
//	//store file as local pointer
//	StorageFile^ file = rootPage->SampleFile;
//	//use file iff not NULL.
//	if (file != nullptr)
//	{
//		create_task(FileIO::ReadTextAsync(file)).then([this, file](task<String^> task)
//		{
//			try
//			{
//				String^ fileContent = task.get();
//				ConsoleText += "\n Output File Contents: \n" + fileContent;
//				ConsoleOutputBlock->Text = ConsoleText;
//			}
//			catch (COMException^ ex)
//			{
//				rootPage->HandleFileNotFoundException(ex);
//			}
//		});
//	}
//	else
//	{
//		rootPage->NotifyUserFileNotExist();
//	}


	auto folder = KnownFolders::DocumentsLibrary;
	create_task(folder->GetFileAsync("output.txt")).then([](StorageFile^ file)
	{
		return FileIO::ReadTextAsync(file);
	}).then([](String^ fileContent)
	{
		ConsoleText += "\n Output File Contents: \n";
		ConsoleText += fileContent + "\n";
		
	}).then([](task<void> t)
	{
		try
		{
			t.get();
		}
		catch (Exception^ e) 
		{
			OutputDebugString(e->Message->Data());
		}
	});

	
	ConsoleOutputBlock->Text = ConsoleText;

}
