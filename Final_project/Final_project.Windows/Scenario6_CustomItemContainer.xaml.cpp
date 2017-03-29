//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
//
//*********************************************************

//
// Scenario6.xaml.cpp
// Implementation of the Scenario6 class
//


// Header files

#include "pch.h"
#include <fstream>
#include <string>
#include <math.h>
#include "Scenario6_CustomItemContainer.xaml.h"

using namespace SDKSample::ListViewSimple;
using namespace std;
using namespace Platform;
using namespace concurrency;
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

Scenario6::Scenario6()
{
    InitializeComponent();

	// create a new instance of store data
	storeData = ref new StoreData();
	// set the source of the GridView to be the sample data
	//ItemGridView->ItemsSource = storeData->Items;
	BeginButton->Click += ref new RoutedEventHandler(this, &Scenario6::BeginButton_Click);
	ClearConsoleButton->Click += ref new RoutedEventHandler(this, &Scenario6::ClearConsoleButton_Click);
	ContinueExecButton->Click += ref new RoutedEventHandler(this, &Scenario6::ContinueExecButton_Click);
	StepExecButton->Click += ref new RoutedEventHandler(this, &Scenario6::StepExecButton_Click);
	DebugExecButton->Click += ref new RoutedEventHandler(this, &Scenario6::DebugExecButton_Click);
	ExecButton->Click += ref new RoutedEventHandler(this, &Scenario6::ExecButton_Click);

}

/// <summary>
/// Invoked when this page is about to be displayed in a Frame.
/// </summary>
/// <param name="e">Event data that describes how this page was reached.  The Parameter
/// property is typically used to configure the page.</param>
void Scenario6::OnNavigatedTo(NavigationEventArgs^ e)
{
	// A pointer back to the main page.  This is needed if you want to call methods in MainPage such
	// as NotifyUser()
	rootPage = MainPage::Current;
}

//utilities

unsigned short floattoword(float);
float w2f(unsigned short);
int PSW(float);
int PSW(unsigned short);
int SignBit = 0;
int zerobit = 0;
String^ ConsoleOutputText;
std::fstream fin;
String^ integertoString(int n);
String^ stringValuetoString(string in);
String^ floattoString(float n);
std::string to_stringValue(const std::wstring &wtxt);
std::string to_stringValue(Platform::String^ pstring);

//global variables
int choice = 0;	//set by execution selection functions
bool inputFlag = true;	//for inputting variables
bool continueFlag=false;	//for continuing execution
int temp;
int temp2;      //for ALU
float latch;      //for Float Input
char latchA;    //for ASCII Input
unsigned short latchI;  //for integer input
float tempa, tempb, tempc;
unsigned short ta, tb, tc;
int EndOfProgram = 0;	//for while loop reading the program

						
						
// Registers and memories
unsigned short IR;
unsigned short CODE[500];
unsigned short PC = -1;
unsigned short registers[32];
unsigned short MEM[65535];
unsigned short STCK[10000];

int top = -1;

// Basic program prerequisites- open file and clear console.

void SDKSample::ListViewSimple::Scenario6::BeginButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	

	//Clear console:
	ConsoleOutputText = " ";
	ConsoleOutputBlock->Text = " ";

	//get file from user's documents library:
	auto folder = KnownFolders::DocumentsLibrary;
	create_task(folder->GetFileAsync("output.txt")).then([this](StorageFile^ file) 
	{
		if(file)	//if file exists, open it.
		{		
			ConsoleOutputText += "\nFile " + file->Path + " Selected.\n";
			ConsoleOutputBlock->Text = ConsoleOutputText;

			//get path of file and store
			String^ filename = file->Path;
			Windows::Storage::StorageFolder^ localFolder = Windows::Storage::ApplicationData::Current->LocalFolder;
			Platform::String^ destfilename = file->Name + ".copy";
			auto option = Windows::Storage::NameCollisionOption::ReplaceExisting;

			create_task(file->CopyAsync(localFolder, destfilename, option)).then([=](StorageFile^ sampleFileCopy) 
			{

				//get ascii version of copied file path
				std::string localPath = to_stringValue(sampleFileCopy->Path);
				fin.open(localPath.data(), ios::in);


				//open file for ifstream object


				if (!fin.is_open())
				{
					ConsoleOutputText += "\n File not opened properly. Please try again.\n";
					ConsoleOutputBlock->Text = ConsoleOutputText;
					return;
				}

				else
				{
					ConsoleOutputText += "\n File " + filename + " successfully opened. \n";
					ConsoleOutputBlock->Text = ConsoleOutputText;

						int i = 0;
						ConsoleOutputText += "\nCode in the file:\n";
						while (fin >> IR)
						{
							MEM[i] = IR;
							CODE[i] = IR;
							//cout << MEM[i] << "\n";
							ConsoleOutputText += integertoString(MEM[i]) + "\n";
							i++;
						}
						ConsoleOutputBlock->Text = ConsoleOutputText;
						fin.close();    // Closing object file

						choice = 0;	//set by execution selection functions
						inputFlag = true;	//for inputting variables
						continueFlag = false;	//for continuing execution
						EndOfProgram = 0;
						PC = -1;
						top = -1;
					//}
				}


			});

		}
		else 
		{
		ConsoleOutputBlock->Text += "\n Operation Cancelled";

		}

	});

	//ExecuteProgram();
	
}

//The main program
void SDKSample::ListViewSimple::Scenario6::ExecuteProgram()
{

	

					// Initializing variables
	//int EndOfProgram = 0;
	//PC = -1;
	//int choice = 0;
	//cin >> choice;		//set by execution functions.
	//Begin Processing
	while (!EndOfProgram)        //EndOfProgram detects HLT
	{
		//instruction decoder

		PC++;       //Program counter increment
		char ch;
		//FETCH
		IR = MEM[PC]; //instruction fetch
		if (choice == 1)           // step by step execution
		{
			if (!continueFlag)
			{
				PC--;	//go back so PC can run afresh.
				return;
			}
		}
		if (choice == 2 || choice == 1)
		{
			//cout << "\nPC:" << PC;
			ConsoleOutputText += "\n PC: " + integertoString(PC) + "\n";
			ConsoleOutputBlock->Text = ConsoleOutputText;

			//continueFlag = false;	//so that the rest of the program can step through
		}


		//DECODE
		unsigned int x = IR;
		unsigned int y = IR;
		int opcode = x & 0xF800;
		opcode = opcode >> 11;
		int option1 = x & 0x0400;
		option1 = option1 >> 10;
		int reg1 = x & 0x03E0;
		reg1 = reg1 >> 5;
		int reg2 = x & 0x1F;
		int option2 = x & 0x1C;
		option2 = option2 >> 2;

		unsigned short add_var;

		int mask = 32768;
		int bin;
		// output the instruction word in binary form
		if (choice == 2 || choice == 1)
		{
			//cout << "\n";
			ConsoleOutputText += "\n ";
			for (int i = 15; i >= 0; i--)
			{
				bin = y & mask;
				bin = bin >> i;
				//cout << bin;
				ConsoleOutputText += integertoString(bin);
				mask = mask >> 1;
			}
			//cout << "\nopcode:" << opcode;
			ConsoleOutputText += "\n Opcode: " + integertoString(opcode);
			ConsoleOutputBlock->Text = ConsoleOutputText;
		}

		//EXECUTE
		switch (opcode)
		{
		case  0://NOP
			if (choice == 2 || choice == 1)
			{
				//cout << "\nNOP\n";
				ConsoleOutputText += "\n NOP \n";
				ConsoleOutputBlock->Text = ConsoleOutputText;
				if (choice == 1) {
					continueFlag = false;	//so that the program can step again.
				}
			}
			break;
		case  1://RET
			PC = STCK[top--];
			if (choice == 2 || choice == 1)
			{
				//cout << "\nRET\n";
				//cout << "\nreturning to PC:" << PC + 1;
				ConsoleOutputText += "\n RET \n" + " Returning to PC: " + integertoString((PC+1));
				ConsoleOutputBlock->Text = ConsoleOutputText;

				if (choice == 1) {
					continueFlag = false;	//so that the program can step again.
				}
			}
			break;
		case  2://HLT
			if (choice == 2 || choice == 1)
			{
				//cout << "\nHLT\n";
				ConsoleOutputText += "\n HLT \n";
				ConsoleOutputBlock->Text = ConsoleOutputText;

				if (choice == 1) {
					continueFlag = false;	//so that the program can step again.
				}
			}
			EndOfProgram = 1;
			break;
		case  3://CMP

			registers[reg1] = ~registers[reg1];
			if (choice == 2 || choice == 1)
			{
				//cout << "\nCMP\n";
				//cout << "reg" << reg1 << ":" << registers[reg1];
				ConsoleOutputText += "\n CMP \n" + "reg" + integertoString(reg1) + ":" + integertoString(registers[reg1]);
				ConsoleOutputBlock->Text = ConsoleOutputText;

				if (choice == 1) {
					continueFlag = false;	//so that the program can step again.
				}
			}
			//complement reg1;
			break;
		case  4://INC

			if (!option1)
			{
				tempa = w2f(registers[reg1]);
				tempa++;
				PSW(tempa);
				registers[reg1] = floattoword(tempa);
				if (choice == 2 || choice == 1)
				{
					//cout << "\nINC\n";
					tempa = w2f(registers[reg1]);
					//cout << "reg" << reg1 << ":" << tempa;
					ConsoleOutputText += "\n INC \n" + "reg" + integertoString(reg1) + ":" + floattoString(tempa);
					ConsoleOutputBlock->Text = ConsoleOutputText;

					if (choice == 1) {
						continueFlag = false;	//so that the program can step again.
					}
				}

			}
			else
			{
				ta = registers[reg1];
				ta++;
				PSW(ta);
				registers[reg1] = ta;
				if (choice == 2 || choice == 1)
				{
					//cout << "\nINC.IN\n";
					//cout << "reg" << reg1 << ":" << registers[reg1];
					ConsoleOutputText += "\n INC.IN \n" + "reg" + integertoString(reg1) + ":" + integertoString(registers[reg1]);
					ConsoleOutputBlock->Text = ConsoleOutputText;

					if (choice == 1) {
						continueFlag = false;	//so that the program can step again.
					}
				}

			}

			break;
		case  5://DEC

			if (!option1)
			{
				tempa = w2f(registers[reg1]);
				tempa--;
				PSW(tempa);
				registers[reg1] = floattoword(tempa);
				if (choice == 2 || choice == 1)
				{
					//cout << "\nDEC\n";
					tempa = w2f(registers[reg1]);
					//cout << "reg" << reg1 << ":" << tempa;
					ConsoleOutputText += "\n DEC \n" + "reg" + integertoString(reg1) + ":" + floattoString(tempa);
					ConsoleOutputBlock->Text = ConsoleOutputText;

					if (choice == 1) {
						continueFlag = false;	//so that the program can step again.
					}
				}

			}
			else
			{
				ta = registers[reg1];
				ta--;
				PSW(ta);
				registers[reg1] = ta;
				if (choice == 2 || choice == 1)
				{
					//cout << "\nDEC.IN\n";
					//cout << "reg" << reg1 << ":" << registers[reg1];
					ConsoleOutputText += "\n DEC.IN \n" + "reg" + integertoString(reg1) + ":" + integertoString(registers[reg1]);
					ConsoleOutputBlock->Text = ConsoleOutputText;

					if (choice == 1) {
						continueFlag = false;	//so that the program can step again.
					}
				}
			}

			break;
		case  6://ADD
			if (!option1) //reg-reg
			{
				tempa = w2f(registers[reg1]);
				tempb = w2f(registers[reg2]);
				tempc = tempa + tempb;

				PSW(tempc);
				registers[reg1] = floattoword(tempc);
			}

			else //immediate
			{
				add_var = MEM[++PC];
				tempa = w2f(registers[reg1]);
				tempb = w2f(add_var);
				tempc = tempa + tempb;
				PSW(tempc);
				registers[reg1] = floattoword(tempc);
			}
			if (choice == 2 || choice == 1)
			{
				//cout << "\nADD\n";
				tempa = w2f(registers[reg1]);
				//cout << "reg" << reg1 << ":" << tempa;
				ConsoleOutputText += "\n ADD \n" + "reg" + integertoString(reg1) + ":" + floattoString(tempa);
				ConsoleOutputBlock->Text = ConsoleOutputText;

				if (choice == 1) {
					continueFlag = false;	//so that the program can step again.
				}
			}

			break;

		case  7://SUB
			if (!option1) //reg-reg
			{
				tempa = w2f(registers[reg1]);
				tempb = w2f(registers[reg2]);
				tempc = tempa - tempb;
				PSW(tempc);
				registers[reg1] = floattoword(tempc);
			}
			else//immediate
			{
				add_var = MEM[++PC];
				tempa = w2f(registers[reg1]);
				tempb = w2f(add_var);
				tempc = tempa - tempb;
				PSW(tempc);
				registers[reg1] = floattoword(tempc);
			}

			if (choice == 2 || choice == 1)
			{
				//cout << "\nSUB\n";
				tempa = w2f(registers[reg1]);
				//cout << "reg" << reg1 << ":" << tempa;
				ConsoleOutputText += "\n SUB \n" + "reg" + integertoString(reg1) + ":" + floattoString(tempa);
				ConsoleOutputBlock->Text = ConsoleOutputText;

				if (choice == 1) {
					continueFlag = false;	//so that the program can step again.
				}
			}


			break;

		case  8://MUL
			if (choice == 2 || choice == 1)
			{
				//cout << "\nMUL\n";
				ConsoleOutputText += "\n MUL \n" ;
				ConsoleOutputBlock->Text = ConsoleOutputText;
			}
			if (!option1) //reg-reg
			{
				tempa = w2f(registers[reg1]);
				tempb = w2f(registers[reg2]);
				tempc = tempa*tempb;
				PSW(tempc);
				registers[reg1] = floattoword(tempc);

				if (choice == 2 || choice == 1)
				{
					tempa = w2f(registers[reg1]);
					tempb = w2f(registers[reg2]);
					//cout << "reg" << reg1 << ":" << tempa;
					//cout << "\nreg" << reg2 << ":" << tempb;
					
					ConsoleOutputText += "reg" + integertoString(reg1) + ":" + floattoString(tempa);
					ConsoleOutputText += "\n reg" + integertoString(reg2) + ":" + floattoString(tempb);
					ConsoleOutputBlock->Text = ConsoleOutputText;

					if (choice == 1) {
						continueFlag = false;	//so that the program can step again.
					}
				}
			}

			else//immediate
			{//multiply reg1 and immediate value
				add_var = MEM[++PC];
				tempa = w2f(registers[reg1]);
				tempb = w2f(add_var);
				tempc = tempa*tempb;
				PSW(tempc);
				registers[reg1] = floattoword(tempc);
				if (choice == 2 || choice == 1)
				{
					tempa = w2f(registers[reg1]);
					//cout << "reg" << reg1 << ":" << tempa;
					ConsoleOutputText += "reg" + integertoString(reg1) + ":" + floattoString(tempa);
					ConsoleOutputBlock->Text = ConsoleOutputText;

					if (choice == 1) {
						continueFlag = false;	//so that the program can step again.
					}
				}
			}

			break;

		case  9://MAC
			if (choice == 2 || choice == 1)
			{
				//cout << "\nMAC\n";
				ConsoleOutputText += "\n MAC \n" ;
				ConsoleOutputBlock->Text = ConsoleOutputText;
			}

			//multiply reg2 and T and add it to reg1
			tempa = w2f(registers[reg2]);
			tempb = w2f(registers[0]);
			tempa = tempa*tempb;
			tempc = w2f(registers[reg1]);
			tempc = tempc + tempa;
			PSW(tempc);
			registers[reg1] = floattoword(tempc);
			if (choice == 2 || choice == 1)
			{
				//cout << "reg" << reg1 << ":" << registers[reg1];
				//cout << "\nreg" << reg2 << ":" << registers[reg2];
				ConsoleOutputText += "reg" + integertoString(reg1) + ":" + integertoString(registers[reg1]);
				ConsoleOutputText += "\n reg" + integertoString(reg2) + ":" + integertoString(registers[reg2]);
				ConsoleOutputBlock->Text = ConsoleOutputText;

				if (choice == 1) {
					continueFlag = false;	//so that the program can step again.
				}
			}

			break;

		case  10://AND
			if (choice == 2 || choice == 1)
			{
				//cout << "\nAND\n";
				ConsoleOutputText += "\n AND \n" ;
				ConsoleOutputBlock->Text = ConsoleOutputText;
			}
			if (!option1) //reg-reg
			{//AND reg1 and reg2
				registers[reg1] = registers[reg1] & registers[reg2];
				if (choice == 2 || choice == 1)
				{
					//cout << "reg" << reg1 << ":" << registers[reg1];
					//cout << "\nreg" << reg2 << ":" << registers[reg2];
					ConsoleOutputText += "reg" + integertoString(reg1) + ":" + integertoString(registers[reg1]);
					ConsoleOutputText += "\n reg" + integertoString(reg2) + ":" + integertoString(registers[reg2]);
					ConsoleOutputBlock->Text = ConsoleOutputText;

					if (choice == 1) {
						continueFlag = false;	//so that the program can step again.
					}
				}
			}
			else//immediate
			{//AND reg1 and immediate value
				add_var = MEM[++PC];
				registers[reg1] = registers[reg1] & add_var;
				if (choice == 2 || choice == 1)
				{
					//cout << "reg" << reg1 << ":" << registers[reg1];
					ConsoleOutputText += "reg" + integertoString(reg1) + ":" + integertoString(registers[reg1]);
					ConsoleOutputBlock->Text = ConsoleOutputText;

					if (choice == 1) {
						continueFlag = false;	//so that the program can step again.
					}
				}
			}

			break;

		case  11://OR
			if (choice == 2 || choice == 1)
			{
				//cout << "\nOR\n";
				ConsoleOutputText += "\n OR \n" + "reg" + integertoString(reg1) + ":" + floattoString(tempa);
				ConsoleOutputBlock->Text = ConsoleOutputText;
			}
			if (!option1) //reg-reg
			{//OR reg1 and reg2
				registers[reg1] = registers[reg1] | registers[reg2];
				if (choice == 2 || choice == 1)
				{
					//cout << "reg" << reg1 << ":" << registers[reg1];
					//cout << "\nreg" << reg2 << ":" << registers[reg2];
					ConsoleOutputText += "reg" + integertoString(reg1) + ":" + integertoString(registers[reg1]);
					ConsoleOutputText += "\n reg" + integertoString(reg2) + ":" + integertoString(registers[reg2]);
					ConsoleOutputBlock->Text = ConsoleOutputText;

					if (choice == 1) {
						continueFlag = false;	//so that the program can step again.
					}
				}
			}
			else//immediate
			{//OR reg1 and immediate
				add_var = MEM[++PC];
				registers[reg1] = registers[reg1] | add_var;
				if (choice == 2 || choice == 1)
				{
					//cout << "reg" << reg1 << ":" << registers[reg1];
					ConsoleOutputText += "reg" + integertoString(reg1) + ":" + integertoString(registers[reg1]);
					ConsoleOutputBlock->Text = ConsoleOutputText;

					if (choice == 1) {
						continueFlag = false;	//so that the program can step again.
					}
				}
			}

			break;

		case  12://XOR
			if (choice == 2 || choice == 1)
			{
				//cout << "\nXOR\n";
				ConsoleOutputText += "\n XOR \n" ;
				ConsoleOutputBlock->Text = ConsoleOutputText;
			}
			if (!option1) //reg-reg
			{//XOR reg1 and reg2
				registers[reg1] = registers[reg1] ^ registers[reg2];
				if (choice == 2 || choice == 1)
				{
					//cout << "reg" << reg1 << ":" << registers[reg1];
					//cout << "\nreg" << reg2 << ":" << registers[reg2];
					ConsoleOutputText += "reg" + integertoString(reg1) + ":" + integertoString(registers[reg1]);
					ConsoleOutputText += "\n reg" + integertoString(reg2) + ":" + integertoString(registers[reg2]);
					ConsoleOutputBlock->Text = ConsoleOutputText;

					if (choice == 1) {
						continueFlag = false;	//so that the program can step again.
					}
				}
			}
			else//immediate
			{
				add_var = MEM[++PC];
				registers[reg1] = registers[reg1] ^ add_var;
				if (choice == 2 || choice == 1)
				{
					//cout << "reg" << reg1 << ":" << registers[reg1];
					ConsoleOutputText += "reg" + integertoString(reg1) + ":" + integertoString(registers[reg1]);
					ConsoleOutputBlock->Text = ConsoleOutputText;
				
					if (choice == 1) {
						continueFlag = false;	//so that the program can step again.
					}
				}

			}

			break;

		case  13://SHL
			if (choice == 2 || choice == 1)
			{
				//cout << "\nSHL\n";
				ConsoleOutputText += "\n SHL \n";
				ConsoleOutputBlock->Text = ConsoleOutputText;
			}
			if (!option1) //reg-reg
			{
				registers[reg1] = registers[reg2] << 1;
				if (choice == 2 || choice == 1)
				{
					//cout << "reg" << reg1 << ":" << registers[reg1];
					//cout << "\nreg" << reg2 << ":" << registers[reg2];
					ConsoleOutputText += "reg" + integertoString(reg1) + ":" + integertoString(registers[reg1]);
					ConsoleOutputText += "\n reg" + integertoString(reg2) + ":" + integertoString(registers[reg2]);
					ConsoleOutputBlock->Text = ConsoleOutputText;

					if (choice == 1) {
						continueFlag = false;	//so that the program can step again.
					}
				}
			}
			else//immediate
			{
				add_var = MEM[++PC];
				registers[reg1] = add_var << 1;
				if (choice == 2 || choice == 1)
				{
					//cout << "reg" << reg1 << ":" << registers[reg1];
					ConsoleOutputText += "reg" + integertoString(reg1) + ":" + integertoString(registers[reg1]);
					ConsoleOutputBlock->Text = ConsoleOutputText;

					if (choice == 1) {
						continueFlag = false;	//so that the program can step again.
					}
				}
			}

			break;

		case  14://SHR
			if (choice == 2 || choice == 1)
			{
				//cout << "\nSHR\n";
				ConsoleOutputText += "\n SHR \n";
				ConsoleOutputBlock->Text = ConsoleOutputText;
			}
			if (!option1) //reg-reg
			{
				registers[reg1] = registers[reg2] >> 1;

				if (choice == 2 || choice == 1)
				{
					//cout << "reg" << reg1 << ":" << registers[reg1];
					//cout << "\nreg" << reg2 << ":" << registers[reg2];
					ConsoleOutputText += "reg" + integertoString(reg1) + ":" + integertoString(registers[reg1]);
					ConsoleOutputText += "\n reg" + integertoString(reg2) + ":" + integertoString(registers[reg2]);
					ConsoleOutputBlock->Text = ConsoleOutputText;

					if (choice == 1) {
						continueFlag = false;	//so that the program can step again.
					}
				}
			}
			else//immediate
			{
				add_var = MEM[++PC];

				registers[reg1] = add_var >> 1;

				if (choice == 2 || choice == 1)
				{
					//cout << "reg" << reg1 << ":" << registers[reg1];
					ConsoleOutputText += "reg" + integertoString(reg1) + ":" + integertoString(registers[reg1]);
					ConsoleOutputBlock->Text = ConsoleOutputText;

					if (choice == 1) {
						continueFlag = false;	//so that the program can step again.
					}
				}
			}

			break;

		case 15: //MOV

			if (!option1) //reg-reg
			{
				registers[reg1] = registers[reg2];
				if (choice == 2 || choice == 1)
				{
					//cout << "\nMOV\n";
					//cout << "reg" << reg1 << ":" << registers[reg1];
					//cout << "\nreg" << reg2 << ":" << registers[reg2];

					ConsoleOutputText += "\n MOV \n" + "reg" + integertoString(reg1) + ":" + integertoString(registers[reg1]);
					ConsoleOutputText += "\n reg" + integertoString(reg2) + ":" + integertoString(registers[reg2]);
					ConsoleOutputBlock->Text = ConsoleOutputText;

					if (choice == 1) {
						continueFlag = false;	//so that the program can step again.
					}
				}
			}
			else//MOV.I immediate
			{
				add_var = MEM[++PC];
				registers[reg1] = add_var;
				if (choice == 2 || choice == 1)
				{
					//cout << "\nMOV.I\n";
					tempa = w2f(registers[reg1]);
					//cout << "reg" << reg1 << ":" << tempa;

					ConsoleOutputText += "\n MOV.I \n" + "reg" + integertoString(reg1) + ":" + floattoString(tempa);
					ConsoleOutputBlock->Text = ConsoleOutputText;

					if (choice == 1) {
						continueFlag = false;	//so that the program can step again.
					}
				}
			}
			break;
		case 16://LOD
			if (!option1) //direct addressing mode
			{
				add_var = CODE[++PC];
				registers[reg1] = MEM[add_var];
			}
			else//LOD.RI indirect addressing mode
			{
				registers[reg1] = MEM[registers[reg2]];
			}
			if (choice == 2 || choice == 1)
			{
				//cout << "\nLOD\n";
				tempa = w2f(registers[reg1]);
				//cout << "reg" << reg1 << "(as float):" << tempa;
				//cout << "\nreg" << reg1 << "(as integer):" << registers[reg1];

				ConsoleOutputText += "\n LOD \n" + "reg" + integertoString(reg1) + "(as float):" + floattoString(tempa);
				ConsoleOutputText += "\n reg" + integertoString(reg1) + "(as integer):" + integertoString(registers[reg1]);
				ConsoleOutputBlock->Text = ConsoleOutputText;

				if (choice == 1) {
					continueFlag = false;	//so that the program can step again.
				}
			}
			break;
		case 17://LOD.BR/LOD.CR

			if (!option1) //bit reversal addressing mode
			{
				unsigned short bitrev, no_bits, p, k, g, d, b;
				p = 1;
				g = 1;
				d = 0xffff;
				no_bits = reg2 >> 1;
				no_bits--;
				g = g << no_bits;
				no_bits++;
				b = registers[1];
				bitrev = 0;
				for (int i = no_bits; i>0; i--)
				{
					k = b&p;

					p = p << 1;
					if (k)
					{
						bitrev = bitrev | g;

					}
					g = g >> 1;
					d = d << 1;

				}

				b = b & d;
				b = b | bitrev;
				registers[reg1] = MEM[b];
				if (choice == 2 || choice == 1)
				{
					//cout << "\nLOD.BR\n";
					//cout << "\naccess reg:" << registers[1];
					//cout << "\nmem:" << b;
					//cout << "reg" << reg1 << ":" << registers[reg1];

					ConsoleOutputText += "\n LOD.BR \n" + "\n access reg" + integertoString(registers[1]) + "\n mem:" + integertoString(b);
					ConsoleOutputText += "reg" + integertoString(reg1) + ":" + integertoString(registers[reg1]);
					ConsoleOutputBlock->Text = ConsoleOutputText;

					if (choice == 1) {
						continueFlag = false;	//so that the program can step again.
					}
				}


			}
			else//LOD.CR circular addressing mode
			{
				//registers[1] -> buffer pointer
				//registers[2] -> access register
				//reg1 -> register where data is to be loaded
				//reg2 -> buffer length
				short a1;
				unsigned a2;
				a1 = registers[2] - registers[1];
				if (a1 >= 0 && a1<registers[reg2])
				{
					a2 = registers[2];
				}
				else if (a1<0)
				{
					while (a1<0)
					{
						a1 = a1 + registers[reg2];
					}
					a2 = registers[1] + a1;
				}
				else
				{
					while (a1>registers[reg2])
					{
						a1 = a1 - registers[reg2];
					}
					a2 = registers[1] + a1;
				}

				registers[reg1] = MEM[a2];


				if (choice == 2 || choice == 1)
				{
					//cout << "\nLOD.CR\n";
					//cout << "\nL:" << registers[reg2];
					//cout << "\npointer" << registers[1];
					//cout << "\nreg2:" << registers[2];
					//cout << "\nMEM" << a2;
					//cout << "reg" << reg1 << ":" << registers[reg1];

					ConsoleOutputText += "\n LOD.CR \n" + "\n L:" + integertoString(registers[reg2]) + "pointer:" + integertoString(registers[1]);
					ConsoleOutputText += "\n reg2:" + integertoString(registers[2]) + "\n MEM" + integertoString(a2);
					ConsoleOutputText += "reg" + integertoString(reg1) + ":" + integertoString(registers[reg1]);
					ConsoleOutputBlock->Text = ConsoleOutputText;

					if (choice == 1) {
						continueFlag = false;	//so that the program can step again.
					}
				}
			}
			break;
		case 18://STR

			if (!option1) //direct addressing mode
			{
				add_var = CODE[++PC];
				MEM[add_var] = registers[reg1];
				if (choice == 2 || choice == 1)
				{
					//cout << "\nSTR\n";
					tempa = w2f(MEM[add_var]);
					//cout << "MEM" << add_var << "(as float):" << tempa;
					//cout << "\nMEM" << add_var << "(as int):" << MEM[add_var];

					ConsoleOutputText += "\n STR \n" + "MEM" + integertoString(add_var) + "(as float):" + floattoString(tempa);
					ConsoleOutputText += "\n MEM" + integertoString(add_var) + "(as int):" + integertoString(MEM[add_var]);
					ConsoleOutputBlock->Text = ConsoleOutputText;

					if (choice == 1) {
						continueFlag = false;	//so that the program can step again.
					}
				}
			}

			else//STR.RI indirect addressing mode
			{
				MEM[registers[reg2]] = registers[reg1];
				if (choice == 2 || choice == 1)
				{
					//cout << "\nSTR.RI\n";
					tempa = w2f(MEM[registers[reg2]]);
					//cout << "MEM" << registers[reg2] << ":" << tempa;

					ConsoleOutputText += "\n STR.RI \n" + "MEM" + integertoString(registers[reg2]) + ":" + floattoString(tempa);
					ConsoleOutputBlock->Text = ConsoleOutputText;

					if (choice == 1) {
						continueFlag = false;	//so that the program can step again.
					}
				}
			}

			break;
		case 19://STR.BR/STR.CR

			if (!option1) //STR.BR
			{
				unsigned short bitrev, no_bits, p, k, g, d, b;
				p = 1;
				g = 1;
				d = 0xffff;
				no_bits = reg2 >> 1;
				no_bits--;
				g = g << no_bits;
				no_bits++;
				b = registers[1];
				bitrev = 0;
				for (int i = no_bits; i>0; i--)
				{
					k = b&p;

					p = p << 1;
					if (k)
					{
						bitrev = bitrev | g;

					}
					g = g >> 1;
					d = d << 1;

				}

				b = b & d;
				b = b | bitrev;
				MEM[b] = registers[reg1];
				if (choice == 2 || choice == 1)
				{
					//cout << "\nSTR.BR\n";
					//cout << "\naccess reg:" << registers[1];
					//cout << "\nmem:" << b;
					//tempa = w2f(MEM[b]);
					//cout << "MEM" << add_var << ":" << tempa;

					ConsoleOutputText += "\n STR.BR \n" + "access reg:" + integertoString(registers[1]) + "\n mem:" + integertoString(b);
					ConsoleOutputText += "\n MEM" + integertoString(add_var) + ":" + floattoString(tempa);
					ConsoleOutputBlock->Text = ConsoleOutputText;
					
					if (choice == 1) {
						continueFlag = false;	//so that the program can step again.
					}
				}
			}

			else//STR.CR circular addressing mode
			{
				//registers[1] -> buffer pointer
				//registers[2] -> access register
				//reg1 -> register with address where data is to be loaded
				//reg2 -> buffer length
				short a1;
				unsigned a2;
				a1 = registers[2] - registers[1];
				if (a1>0 && a1<registers[reg2])
				{
					a2 = registers[2];
				}
				else if (a1<0)
				{
					while (a1<0)
					{
						a1 = a1 + registers[reg2];
					}
					a2 = registers[1] + a1;
				}
				else
				{
					while (a1>registers[reg2])
					{
						a1 = a1 - registers[reg2];
					}
					a2 = registers[1] + a1;
				}

				MEM[a2] = registers[reg1];
				if (choice == 2 || choice == 1)
				{
					//cout << "\nSTR.CR\n";
					//cout << "\nL:" << registers[reg2];
					//cout << "\npointer" << registers[1];
					//cout << "\nreg2:" << registers[2];
					//cout << "\nMEM" << a2;
					tempa = w2f(MEM[a2]);
					//cout << "MEM" << a2 << ":" << tempa;

					ConsoleOutputText += "\n STR.CR \n" + "\n L:" + integertoString(registers[reg2]) + "\n pointer:" + integertoString(registers[1]);
					ConsoleOutputText += "\n reg2" + integertoString(registers[2]) + "\n MEM:" + integertoString(a2) + "\n MEM:" + integertoString(a2) + ":" + floattoString(tempa);
					ConsoleOutputBlock->Text = ConsoleOutputText;

					if (choice == 1) {
						continueFlag = false;	//so that the program can step again.
					}
				}

			}
			break;

		case  20://IN
			if (!option1)
			{
				//cout << "\nIN(in float): ";
				//cin >> latch;
				//cout << "\n";

				

				if(inputFlag) 
					inputFlag  = false; 
				else 
					inputFlag = true;	//toggle inputflag. False for the first time.
				//wait for float value to be entered, and then proceed when the continue button is clicked:
				if (!inputFlag)
				{
					PC--;	//go back on PC so this can run again.
					ConsoleOutputText += "\n IN(float): (Enter FLOAT type value in the text box below, then press 'Continue Execution'.) \n";
					ConsoleOutputBlock->Text = ConsoleOutputText;	//show this once only, when asking.
					return;
				}
				if (!continueFlag)
				{
					PC--;
					return;
				}

				else            //when continueFlag==1, i.e. the continue button has been pressed
				
				{
					std::string tempstr = to_stringValue(ValueAcceptor->Text);	//accept value as string
					latch = std::stof(tempstr);		//extract float value in latch
					
					//show the value:
					ConsoleOutputText += "\n Entered Value: " + floattoString(latch) + "\n";
					ConsoleOutputBlock->Text = ConsoleOutputText;
					
					//set flag to 0;
					continueFlag = false;

				}

				
				//Change Latch as required
				registers[reg1] = floattoword(latch);
			}
			else
			{
				//cout << "\nIN(in ASCII):";
				//cin >> latchA;
				//cout << "\n";

				
				if(inputFlag) 
					inputFlag  = false; 
				else 
					inputFlag = true;	//toggle inputflag. False for the first time.
											//wait for float value to be entered, and then proceed when the continue button is clicked:
				if (!inputFlag)
				{
					PC--;	//go back on PC so this can run again.
					ConsoleOutputText += "\n IN(ASCII): (Enter ASCII type value in the text box below, then press 'Continue Execution'.) \n";
					ConsoleOutputBlock->Text = ConsoleOutputText;	//show this once only, when asking.
					return;
				}

				//wait for float value to be entered, and then proceed when the continue button is clicked:
				if (!continueFlag)
				{
					PC--;	//go back so PC can run this part again.
					return;
				}
							else            //when inputFlag = true, i.e. the continue button has been pressed

							{
								std::string tempstr = to_stringValue(ValueAcceptor->Text);	//accept value as string
								char tempChar = stoi(tempstr);	//extract string value in latch
								latchA = tempChar;
								
								
								//convert the int value :
								std::string tempstr2(1,latchA);

								ConsoleOutputText += "\n Entered Value: " + stringValuetoString(tempstr2) + "\n";
								ConsoleOutputBlock->Text = ConsoleOutputText;

								//set flag to 0;
								continueFlag = false;

							}

				registers[reg1] = latchA;
			}
			if (choice == 2 || choice == 1)
			{
				//cout << "reg" << reg1 << "(bit rep):" << registers[reg1];
				ConsoleOutputText += " reg" + integertoString(reg1) + "(bit rep):" + integertoString(registers[reg1]);
				ConsoleOutputBlock->Text = ConsoleOutputText;

			}
			break;
		case  21://OUT
			if (!option1)
			{
				latch = w2f(registers[reg1]);
				//cout << "\nOUT reg" << reg1 << "(in float):" << latch << "\n";
				ConsoleOutputText += "\n OUT reg" + integertoString(reg1) + "(in float):" + floattoString(latch);
				ConsoleOutputBlock->Text = ConsoleOutputText;

				if (choice == 1) {
					continueFlag = false;	//so that the program can step again.
				}
			}
			else
			{
				latchA = registers[reg1];
				//cout << "\nOUT reg" << reg1 << "(in ASCII):" << latchA << "\n";
				string tempstr(1, latchA);
				ConsoleOutputText += "\n OUT reg" + integertoString(reg1) + "(in ASCII):" + stringValuetoString(tempstr);
				ConsoleOutputBlock->Text = ConsoleOutputText;

				if (choice == 1) {
					continueFlag = false;	//so that the program can step again.
				}
			}
			break;
		case 22://JMP
			if (choice == 2 || choice == 1)
			{
				//cout << "\nJMP\n";

				ConsoleOutputText += "\n JMP \n";
				ConsoleOutputBlock->Text = ConsoleOutputText;
			}
			switch (option2)
			{
			case 0://JMP
				if (!option1)
					PC = MEM[++PC] - 2;
				else//JMP.RI
				{
					PC = registers[reg1] - 2;
					if (choice == 2 || choice == 1)
					{
						//cout << "reg:" << reg1 << "\n";
						ConsoleOutputText += "reg:" + integertoString(reg1) + "\n";
						ConsoleOutputBlock->Text = ConsoleOutputText;

						if (choice == 1) {
							continueFlag = false;	//so that the program can step again.
						}
					}
				}
				break;
			case 1://JMP.Z
				if (!option1)
				{
					if (zerobit == 1)
					{
						PC = MEM[++PC] - 2;
					}
				}
				else//JMP.Z.RI
				{
					if (zerobit == 1)
						PC = registers[reg1] - 2;
					if (choice == 2 || choice == 1)
					{
						//cout << "reg:" << reg1 << "\n";
						ConsoleOutputText += "reg:" + integertoString(reg1) + "\n";
						ConsoleOutputBlock->Text = ConsoleOutputText;

						if (choice == 1) {
							continueFlag = false;	//so that the program can step again.
						}
					}
				}
				break;
			case 2://JMP.NZ
				if (!option1)
				{
					if (!zerobit)
						PC = MEM[++PC] - 2;

				}
				else//JMP.NZ.RI
				{
					if (!zerobit)
						PC = registers[reg1] - 2;
					if (choice == 2 || choice == 1)
					{
						//cout << "reg:" << reg1 << "\n";
						ConsoleOutputText += "reg:" + integertoString(reg1) + "\n";
						ConsoleOutputBlock->Text = ConsoleOutputText;

						if (choice == 1) {
							continueFlag = false;	//so that the program can step again.
						}
					}
				}
				break;

			case 3://JMP.N
				if (!option1)
				{
					if (SignBit)
						PC = MEM[++PC] - 2;
				}
				else//JMP.N.RI
				{
					if (SignBit)
						PC = registers[reg1] - 2;
					if (choice == 2 || choice == 1)
					{
						//cout << "reg:" << reg1 << "\n";
						ConsoleOutputText += "reg:" + integertoString(reg1) + "\n" ;
						ConsoleOutputBlock->Text = ConsoleOutputText;

						if (choice == 1) {
							continueFlag = false;	//so that the program can step again.
						}
					}
				}
				break;
			case 4://JMP.NN
				if (!option1)
				{
					if (!SignBit)
						PC = MEM[++PC] - 2;
				}
				else//JMP.NN.RI
				{
					if (!SignBit)
						PC = registers[reg1] - 2;
					if (choice == 2 || choice == 1)
					{
						//cout << "reg:" << reg1 << "\n";
						ConsoleOutputText += "reg:" + integertoString(reg1) + "\n";
						ConsoleOutputBlock->Text = ConsoleOutputText;

						if (choice == 1) {
							continueFlag = false;	//so that the program can step again.
						}
					}
				}
				break;


			}
			if (!option1)                          // direct addressing mode is a 2 word instruction
			{
				PC++;
			}
			break;

		case 23://CALL
		{if (choice == 2 || choice == 1)
		{
			//cout << "\nCALL";
			ConsoleOutputText += "\n CALL \n" ;
			ConsoleOutputBlock->Text = ConsoleOutputText;

		}

		if (!option1)
		{
			STCK[++top] = PC + 1;
			PC = MEM[++PC] - 1;

		}

		else //CALL.RI
		{
			STCK[++top] = PC;
			PC = registers[reg1] - 1;

			if (choice == 2 || choice == 1)
			{
				//cout << "\nreg:" << reg1 << "\n";
				ConsoleOutputText += "\n reg" + integertoString(reg1) + "\n" ;
				ConsoleOutputBlock->Text = ConsoleOutputText;

				if (choice == 1) {
					continueFlag = false;	//so that the program can step again.
				}
			}

		}
		if (choice == 2 || choice == 1)
		{
			//cout << "\ntop:" << top << "\tStack[top]:" << STCK[top];
			ConsoleOutputText += "\n top: " + integertoString(top) + "\t Stack[top]:" + integertoString(STCK[top]);
			ConsoleOutputBlock->Text = ConsoleOutputText;

			if (choice == 1) {
				continueFlag = false;	//so that the program can step again.
			}
		}
		}
		break;
		case  24://PUSH

			STCK[++top] = registers[reg1];
			if (choice == 2 || choice == 1)
			{
				//cout << "\nPUSH\n";
				//cout << "Stack" << top << ":" << STCK[top] << "\n";
				ConsoleOutputText += "\n PUSH \n Stack " + integertoString(top) + ":" + integertoString(STCK[top]) + "\n";
				ConsoleOutputBlock->Text = ConsoleOutputText;

				if (choice == 1) {
					continueFlag = false;	//so that the program can step again.
				}
			}

			break;

		case  25://POP
			registers[reg1] = STCK[top--];
			if (choice == 2 || choice == 1)
			{
				//cout << "\nPOP\n";
				//cout << "reg" << reg1 << ":" << registers[reg1] << "\n";
				ConsoleOutputText += "\n POP \n";
				ConsoleOutputText += "reg" + integertoString(reg1) + ":" + integertoString(registers[reg1]);
				ConsoleOutputBlock->Text = ConsoleOutputText;

				if (choice == 1) {
					continueFlag = false;	//so that the program can step again.
				}
			}
			break;
		case 26://ADD.IN
			if (!option1) //reg-reg
			{
				ta = registers[reg1];
				tb = registers[reg2];
				tc = ta + tb;

				PSW(tc);
				registers[reg1] = tc;
			}

			else //immediate
			{
				add_var = MEM[++PC];
				ta = registers[reg1];
				tb = add_var;
				tc = ta + tb;
				PSW(tc);
				registers[reg1] = tc;
			}
			if (choice == 2 || choice == 1)
			{
				//cout << "\nADD.IN\n";
				//cout << "reg" << reg1 << ":" << registers[reg1];
				ConsoleOutputText += "\n ADD.IN \n";
				ConsoleOutputText += "reg" + integertoString(reg1) + ":" + integertoString(registers[reg1]);
				ConsoleOutputBlock->Text = ConsoleOutputText;

				if (choice == 1) {
					continueFlag = false;	//so that the program can step again.
				}
			}
			break;

		case 27://SUB.IN
			if (!option1) //reg-reg
			{
				ta = registers[reg1];
				tb = registers[reg2];
				tc = ta - tb;
				PSW(tc);
				registers[reg1] = tc;
			}
			else//immediate
			{
				add_var = MEM[++PC];
				ta = registers[reg1];
				tb = add_var;
				tc = ta - tb;
				PSW(tc);
				registers[reg1] = tc;
			}

			if (choice == 2 || choice == 1)
			{
				//cout << "\nSUB.IN\n";
				//cout << "reg" << reg1 << ":" << registers[reg1];
				ConsoleOutputText += "\n SUB.IN \n";
				ConsoleOutputText += "reg" + integertoString(reg1) + ":" + integertoString(registers[reg1]);
				ConsoleOutputBlock->Text = ConsoleOutputText;

				if (choice == 1) {
					continueFlag = false;	//so that the program can step again.
				}
			}


			break;

		case 28://MOV.IN.I
			add_var = MEM[++PC];
			registers[reg1] = add_var;
			if (choice == 2 || choice == 1)
			{
				//cout << "\nMOV.IN.I";
				//cout << "\nreg" << reg1 << ":" << registers[reg1];
				ConsoleOutputText += "\n MOV.IN.I \n";
				ConsoleOutputText += "reg" + integertoString(reg1) + ":" + integertoString(registers[reg1]);
				ConsoleOutputBlock->Text = ConsoleOutputText;

				if (choice == 1) {
					continueFlag = false;	//so that the program can step again.
				}
			}
			break;

		case 29: //IN.IN
			//cout << "\nIN.IN(in integer):";
			//cin >> latchI;
			//cout << "\n";
			

			if(inputFlag) 
				inputFlag  = false; 
			else 
				inputFlag = true;	//toggle inputflag. False for the first time.
										//wait for float value to be entered, and then proceed when the continue button is clicked:
			if (!inputFlag)
			{
				PC--;	//go back on PC so this can run again.
				ConsoleOutputText += "\n IN(Integer): (Enter Integer type value in the text box below, then press 'Continue Execution'.) \n";
				ConsoleOutputBlock->Text = ConsoleOutputText;	//show this once only, when asking.
				return;
			}
			//wait for integral value to be entered, and then proceed when the continue button is clicked:
			if (!continueFlag)
			{
				PC--;		//go back so PC can run this again.
				return;
			}
						else            //when continueFlag==1, i.e. the continue button has been pressed

						{
							std::string tempstr = to_stringValue(ValueAcceptor->Text);	//accept value as string
							latchI = stoi(tempstr);	//extract string value in latch

							ConsoleOutputText += "\n Entered Value: " + integertoString(latchI) + "\n";
							ConsoleOutputBlock->Text = ConsoleOutputText;

							//set flag to 0;
							continueFlag = false;

						}

			registers[reg1] = latchI;
			if (choice == 2 || choice == 1)
			{
				//cout << "reg" << reg1 << ":" << registers[reg1];
				ConsoleOutputText += "reg" + integertoString(reg1) + ":" + integertoString(registers[reg1]);
				ConsoleOutputBlock->Text = ConsoleOutputText;
			}

			break;

		case 30: //OUT.IN
			latchI = registers[reg1];
			//cout << "\nOUT.IN reg" << reg1 << "(in integer):" << latchI << "\n";
			ConsoleOutputText += "\n OUT.IN \n reg" + integertoString(reg1) + "(in integer):" + integertoString(latchI) + "\n";
			ConsoleOutputBlock->Text = ConsoleOutputText;

			if (choice == 1) {
				continueFlag = false;	//so that the program can step again.
			}
			break;

		default://ERROR
			//cout << "INCORRECT INSTRUCTION: " << int(MEM[PC]) << "PC" << PC << "\n";
			ConsoleOutputText += "\n INCORRECT INSTRUCTION \n" + integertoString(int(MEM[PC])) + "PC:" + integertoString(PC) + "\n";
			ConsoleOutputBlock->Text = ConsoleOutputText;
			EndOfProgram = 1;

		}

		if (choice == 2 || choice == 1)
		{
			//cout << "\n\n\n";
			ConsoleOutputText += "\n\n\n";
			ConsoleOutputBlock->Text = ConsoleOutputText;

			if (choice == 1) {
				continueFlag = false;	//so that the program can step again.
			}
		}

	}
}


void SDKSample::ListViewSimple::Scenario6::ClearConsoleButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	ConsoleOutputText = "";
	ConsoleOutputBlock->Text = ConsoleOutputText;
}


void SDKSample::ListViewSimple::Scenario6::ContinueExecButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	continueFlag = true;
	inputFlag = false;
	ExecuteProgram();
	return;
}

void SDKSample::ListViewSimple::Scenario6::ExecButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	choice = 3;
	continueFlag = true;
	ExecuteProgram();
	return;
}


void SDKSample::ListViewSimple::Scenario6::StepExecButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	choice = 1;
	continueFlag = true;
	ExecuteProgram();
	return;
}


void SDKSample::ListViewSimple::Scenario6::DebugExecButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	choice = 2;
	continueFlag = true;
	ExecuteProgram();
	return;
}


unsigned short floattoword(float data)
{
	unsigned short y;
	int sign, mantissa, exp;

	//word to float


	if (data >= 0)
	{
		sign = 0;
	}

	else
	{
		data = -data;
		sign = 1;
	}
	exp = 0;
	int flag = 1;

	while (flag)
	{
		if (data>0.998047)
		{
			data = data / 2;
			exp++;
		}
		else if (data<0.00195312 && data != 0)
		{

			data = data * 2;
			exp--;

		}
		else
		{
			flag = 0;
		}
	}
	//data to mantissa
	mantissa = 0;
	for (int i = -1; i >= -9; i--)
	{
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
float w2f(unsigned short y)
{
	int sign, mantissa, exp;

	sign = y & 0x8000;
	mantissa = y & 0x7FC0;
	mantissa = mantissa >> 6;
	exp = y & 0x003F;
	if (sign == 0)
		sign = 1;
	else
		sign = -1;
	//mantissa
	int j = 0x0100;
	int temp1;
	float mant = 0;

	for (int i = -1; i >= -9; i--)
	{
		temp1 = mantissa &j;

		if (temp1)
		{
			mant += pow(2.0, i);

		}
		j = j >> 1;
	}

	exp = exp - 32;
	float exp1 = pow(2.0, exp);

	float data1 = mant*exp1*sign;


	// float to word
	return (data1);
}

int PSW(float temp3)
{
	if (temp3<0)
	{
		SignBit = 1;
		zerobit = 0;
	}
	else if (temp3 == 0)
	{
		SignBit = 0;
		zerobit = 1;
	}
	else
	{
		SignBit = 0;
		zerobit = 0;
	}
	return 0;
}
int PSW(unsigned short temp3)
{
	if (temp3 == 0)
		zerobit = 1;
	else
		zerobit = 0;
	temp3 = temp3 >> 15;
	if (temp3 == 1)
	{
		SignBit = 1;
	}
	else if (temp3 == 0)
	{
		SignBit = 0;
	}
	else
	{
		//cout << "\nCHECK";
		ConsoleOutputText += "\n CHECK";
	}
	return 0;
}

String^ stringValuetoString(string in) {
	std::wstring wstr = std::wstring(in.begin(), in.end());
	const wchar_t* tem3 = wstr.c_str();
	String^ res = ref new String(tem3);
	return res;

}

String^ integertoString(int n) {

	string str = to_string(n);
	std::wstring wstr = std::wstring(str.begin(), str.end());
	const wchar_t* tem3 = wstr.c_str();
	String^ res = ref new String(tem3);
	return res;

}

String^ floattoString(float n) {

	string str = to_string(n);
	std::wstring wstr = std::wstring(str.begin(), str.end());
	const wchar_t* tem3 = wstr.c_str();
	String^ res = ref new String(tem3);
	return res;
}

// Convert from wstring into a string
std::string to_stringValue(const std::wstring & wtxt)
{
	std::string str;
	str.resize(wtxt.size() + 1);

	size_t numConverted;
	errno_t		err = wcstombs_s(&numConverted, (char *)str.data(), str.size(), wtxt.data(), wtxt.size());

	str.pop_back();
	return str;
}

std::string to_stringValue(Platform::String^ pstring)
{
	std::wstring wStr(pstring->Data());
	return to_stringValue(wStr);
}