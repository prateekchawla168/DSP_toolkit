﻿//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
//
//*********************************************************

//
// Scenario5.xaml.h
// Declaration of the Scenario5 class
//

#pragma once

#include "pch.h"
#include "Scenario5_RetemplatingListViewItems.g.h"
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
		public ref class Scenario5 sealed
        {
        public:
            Scenario5();

		protected:
			virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

        private:
			MainPage^ rootPage;
			void FileOpenButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
			void AssembleButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
			void throw_error();
			float make_double(std::string temp);
			void UpdateOutputFile();
			void DisplayOutputButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		};
    }
}
