//
// Scenario7_FIRFilterDesign.xaml.h
// Declaration of the Scenario7_FIRFilterDesign class
//

#pragma once

#include "Scenario7_FIRFilterDesign.g.h"
#include "pch.h"
#include "MainPage.xaml.h"
#include "SampleDataSource.h"

namespace SDKSample 
{
	namespace ListViewSimple
	{
		/// <summary>
		/// An empty page that can be used on its own or navigated to within a Frame.
		/// </summary>
		[Windows::Foundation::Metadata::WebHostHiddenAttribute]
		public ref class Scenario7 sealed
		{
		public:
			Scenario7();

		protected:
			virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
		private:
			MainPage^ rootPage;
			void DesignFilterButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
			//filter designing functions
			void DesignLPF();
			void DesignHPF();
			void DesignBPF();
			void DesignBSF();
			//window functions
			void Rectangular(int arg);
			void Triangular(int arg);
			void Hann(int arg);
			void Hamming(int arg);
			void Blackman(int arg);
			void ClearConsoleButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		};

	}
}
