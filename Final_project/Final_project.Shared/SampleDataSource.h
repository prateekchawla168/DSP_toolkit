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

using namespace Windows::ApplicationModel::Resources::Core;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Media::Imaging;


namespace SDKSample
{
	namespace ListViewSampleDataSource
	{
		[Windows::UI::Xaml::Data::Bindable]
		[Windows::Foundation::Metadata::WebHostHiddenAttribute]
		public ref class Item sealed
		{
			Platform::String^  _Title;
			Platform::String^  _Subtitle;
			Platform::String^  _Link;
			Platform::String^  _Category;
			Platform::String^ _Description;
			Platform::String^ _Content;
			ImageSource^ _Image;
			event PropertyChangedEventHandler^ _PropertyChanged; 


		public:
			Item();

			void OnPropertyChanged(Platform::String^ propertyName)
			{
				PropertyChangedEventArgs^ pcea = ref new  PropertyChangedEventArgs(propertyName);
				_PropertyChanged(this,pcea);
			}


			//Title
			property Platform::String^ Title
			{
				Platform::String^ get()
				{
					return _Title;
				}
				void set(Platform::String^ value)
				{
					_Title = value;
					OnPropertyChanged("Title");
				}
			}

			//Subtitle
			property Platform::String^ Subtitle
			{
				Platform::String^ get()
				{
					return _Subtitle;
				}
				void set(Platform::String^ value)
				{
					_Subtitle = value;
					OnPropertyChanged("Subtitle");
				}
			}

			//Link
			property Platform::String^ Link
			{
				Platform::String^ get()
				{
					return _Link;
				}
				void set(Platform::String^ value)
				{
					_Link = value;
					OnPropertyChanged("Link");
				}
			}

		
			//Category
			property Platform::String^ Category
			{
				Platform::String^ get()
				{
					return _Category;
				}
				void set(Platform::String^ value)
				{
					_Category = value;
					OnPropertyChanged("Category");
				}
			}

			//Description
			property Platform::String^ Description
			{
				Platform::String^ get()
				{
					return _Description;
				}
				void set(Platform::String^ value)
				{
					_Description = value;
					OnPropertyChanged("Description");
				}
			}

			//Content
			property Platform::String^ Content
			{
				Platform::String^ get()
				{
					return _Content;
				}
				void set(Platform::String^ value)
				{
					_Content = value;
					OnPropertyChanged("Content");
				}
			}

			//Image
			property ImageSource^ Image
			{
				ImageSource^ get()
				{
					return _Image;
				}
				void set(ImageSource^ value)
				{
					_Image = value;
					OnPropertyChanged("Image");
				}
			}

			void SetImage(Windows::Foundation::Uri^ baseUri, Platform::String^ path) //SetImage() in C# code
			{
				Windows::Foundation::Uri^ uri = ref new Windows::Foundation::Uri("ms-appx:///" + path);		
				Image = ref new BitmapImage(uri);
			}
		};


		[Windows::Foundation::Metadata::WebHostHiddenAttribute]
		public ref class StoreData sealed 
		{
			Windows::UI::Xaml::Interop::IBindableObservableVector^  _items;

		public:
			StoreData()
			{
				_items = ref new Platform::Collections::Vector<Item^>();
				Windows::Foundation::Uri^ _baseUri = ref new Windows::Foundation::Uri("ms-appx:///");
				Platform::String^ LONG_LOREM_IPSUM = "";//"Curabitur class aliquam vestibulum nam curae maecenas sed integer cras phasellus suspendisse quisque donec dis praesent accumsan bibendum pellentesque condimentum adipiscing etiam consequat vivamus dictumst aliquam duis convallis scelerisque est parturient ullamcorper aliquet fusce suspendisse nunc hac eleifend amet blandit facilisi condimentum commodo scelerisque faucibus aenean ullamcorper ante mauris dignissim consectetuer nullam lorem vestibulum habitant conubia elementum pellentesque morbi facilisis arcu sollicitudin diam cubilia aptent vestibulum auctor eget dapibus pellentesque inceptos leo egestas interdum nulla consectetuer suspendisse adipiscing pellentesque proin lobortis sollicitudin augue elit mus congue fermentum parturient fringilla euismod feugiat";
			
				Item^ item;
				item = ref new Item();
				item->Title="Main Menu";
				item->Subtitle="Maecenas class nam praesent cras aenean mauris aliquam nullam aptent accumsan duis nunc curae donec integer auctor sed congue amet";
				item->SetImage(_baseUri, "SampleData/Images/briefcase.png");
				item->Link = "http://www.adatum.com/";
				item->Category = "Menu";
				item->Description = "Click on any item to display instructions on using the selected utility.";
				item->Content = LONG_LOREM_IPSUM;
				_items->Append(item);

				item = ref new Item();
				item->Title = "Linear Convolution";
				item->Subtitle = "Quisque vivamus bibendum cursus dictum dictumst dis aliquam aliquet etiam lectus eleifend fusce libero ante facilisi ligula est";
				item->SetImage(_baseUri, "SampleData/Images/60Lemon.png");
				item->Link = "http://www.adventure-works.com/";
				item->Category = "Utility";
				item->Description = "Enter the two real signals to be linearly convolved, then click the 'Convolve!' button. \nMake sure to add a space after entering each number, otherwise the app may not work properly. \nPlease enter different coefficients of the signals one after the other, separated by a space. Finally, when all data entry is complete, click 'Convolve!' to execute.  \nOutput is displayed in the matrix method of convolution, and the transfer function is shown in form of a matrix. ";
				item->Content = LONG_LOREM_IPSUM;
				_items->Append(item);


				item = ref new Item();
				item->Title = "Circular Convolution";
				item->Subtitle = "Litora luctus magnis arcu lorem morbi blandit faucibus mattis commodo hac habitant inceptos conubia cubilia nulla mauris diam proin augue eget dolor mollis interdum lobortis";
				item->SetImage(_baseUri, "SampleData/Images/60Mint.png");
				item->Link = "http://www.adventure-works.com/";
				item->Category = "Utility";
				item->Description = "Enter the two real signals to be circularly convolved, and then enter the number of points of convolution. \nPlease make sure to add a space after each signal ends otherwise the app may not work properly. \nThe different coefficients of a particular signal are to be separated by a space. Finally, when all data entry is complete, click 'Convolve!' to execute. \nOutput is displayed with the transfer function in the form of a matrix.  ";
				item->Content = LONG_LOREM_IPSUM;
				_items->Append(item);

				item = ref new Item();
				item->Title = "Fast Fourier Transform";
				item->Subtitle = "Leo mus nec nascetur dapibus non fames per felis ipsum pharetra egestas montes elit nostra placerat euismod enim justo ornare feugiat platea pulvinar sed sagittis";
				item->SetImage(_baseUri, "SampleData/Images/60Orange.png");
				item->Link = "http://www.alpineskihouse.com/";
				item->Category = "Utility";
				item->Description = "This utility calculates the direct fourier transform of the complex signal entered. \nEnter the cofficients of the signal in the two text boxes given, one for the real part and the other for the imaginary part. \nFor example, if x(0) and x(1) of the signal are 1+j0 and 1+j(-1) respectively, the real signal is '1 1 ' and the imaginary signal will be '0 -1 '. \nMake sure that you add a space after the end of the last entry in each text box, otherwise the utility may not work properly. \nUse the slider at the top to select the number of points and the slider below the textboxes to select the algorithm used to calculate the result, and finally click the 'Find FFT! button to display the output in the form of a butterfly diagram. ";
				item->Content = LONG_LOREM_IPSUM;
				 _items->Append(item);

				item = ref new Item();
				item->Title = "DSP Assembler Kit";
				item->Subtitle = "Senectus sem lacus erat sociosqu eros suscipit primis nibh nisi nisl gravida torquent";
				item->SetImage(_baseUri, "SampleData/Images/60Strawberry.png");
				item->Link = "http://www.baldwinmuseumofscience.com/";
				item->Category = "Simulator";
				item->Description = "This utility simulates a real Digital Signal Processor, the details of which are supplied to you in the Instruction Manual.\n Press 'Open File' to load a .txt file in the program, and then press 'Assemble!' to assemble the file. \nAny errors encountered during assembly will be displayed on the console.\nPress 'Show Output File' to show the contents of the assembled file, if assembly is completesd with no errors. ";
				item->Content = LONG_LOREM_IPSUM;
				_items->Append(item);

				item = ref new Item();
				item->Title = "FIR Filter Design";
				item->Subtitle = "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat";
				item->SetImage(_baseUri, "SampleData/Images/60Vanilla.png");
				item->Link = "http://www.adatum.com/";
				item->Category = "Utility";
				item->Description = "This utility designs a filter of the given specifications. Select the input format (ripples/attenuations), enter the appropriate input data in the appropriate boxes, select the type of filter, and click on 'Design Filter' button to see the designing process in the console. Please enter a spacebar after entering the value in the input data otherwise the input may not get accepted properly by the application. \nPress 'Clear Console' at any time to clear the console.";
				item->Content = LONG_LOREM_IPSUM;
				_items->Append(item);

				item = ref new Item();
				item->Title = "DSP Simulator Kit";
				item->Subtitle = "Leo mus nec nascetur dapibus non fames per felis ipsum pharetra egestas montes elit nostra placerat euismod enim justo ornare feugiat platea pulvinar sed sagittis";
				item->SetImage(_baseUri, "SampleData/Images/60Mint.png");
				item->Link = "http://www.alpineskihouse.com/";
				item->Category = "Simulator";
				item->Description = "This utility simulates a real Digital Signal Processor, the details of which have been provided in the instruction manual. This simulator accepts only the input which has been assembled previously by the assembler. Any other assembly files are not supported.\n Press 'Begin Execution' to load the file containing the assembly code. Then Select your mode of execution. \n Step Mode executes the program one instruction at a time, and needs pressing the 'CE' (Continue Execution) button after each step.\n Debug Mode executes the program normally, but displays the contents of the Program Counter and the Instruction executed while executing. The Debug Mode pauses when a user input is required, and requires the user to press the 'CE' button after data entry is done. \n Run Mode is activated by the 'Execute' button, and runs the program until a statement requiring an input is encountered. In Run Mode, the register and memory contents are only displayed when the program executes a statement containing an I/O instruction. \nPress CLC at any time to clear the console.";
				item->Content = LONG_LOREM_IPSUM;
				_items->Append(item);
          
			}

			property Windows::UI::Xaml::Interop::IBindableObservableVector^ Items
			{
				Windows::UI::Xaml::Interop::IBindableObservableVector^ get()
				{
					return _items;
				}
			
			}
		};

	}
}