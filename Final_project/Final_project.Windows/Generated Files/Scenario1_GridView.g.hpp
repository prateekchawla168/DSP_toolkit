﻿

//------------------------------------------------------------------------------
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
//------------------------------------------------------------------------------
#include "pch.h"
#include "Scenario1_GridView.xaml.h"




void ::SDKSample::ListViewSimple::Scenario1::InitializeComponent()
{
    if (_contentLoaded)
        return;

    _contentLoaded = true;

    // Call LoadComponent on ms-appx:///Scenario1_GridView.xaml
    ::Windows::UI::Xaml::Application::LoadComponent(this, ref new ::Windows::Foundation::Uri(L"ms-appx:///Scenario1_GridView.xaml"), ::Windows::UI::Xaml::Controls::Primitives::ComponentResourceLocation::Application);

    // Get the Grid named 'LayoutRoot'
    LayoutRoot = safe_cast<::Windows::UI::Xaml::Controls::Grid^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"LayoutRoot"));
    // Get the Grid named 'Input'
    Input = safe_cast<::Windows::UI::Xaml::Controls::Grid^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"Input"));
    // Get the Grid named 'Output'
    Output = safe_cast<::Windows::UI::Xaml::Controls::Grid^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"Output"));
    // Get the TextBlock named 'OutputBlock'
    OutputBlock = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"OutputBlock"));
    // Get the TextBlock named 'MatrixBlock'
    MatrixBlock = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"MatrixBlock"));
    // Get the TextBlock named 'InputBlock'
    InputBlock = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"InputBlock"));
    // Get the TextBox named 'InputBox'
    InputBox = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"InputBox"));
    // Get the TextBox named 'TransferBox'
    TransferBox = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"TransferBox"));
    // Get the TextBlock named 'ResultBlock'
    ResultBlock = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"ResultBlock"));
    // Get the Button named 'button'
    button = safe_cast<::Windows::UI::Xaml::Controls::Button^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"button"));
    // Get the TextBlock named 'InputTextBlock1'
    InputTextBlock1 = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"InputTextBlock1"));
    // Get the VisualState named 'DefaultLayout'
    DefaultLayout = safe_cast<::Windows::UI::Xaml::VisualState^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"DefaultLayout"));
    // Get the VisualState named 'Below768Layout'
    Below768Layout = safe_cast<::Windows::UI::Xaml::VisualState^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"Below768Layout"));
}

void ::SDKSample::ListViewSimple::Scenario1::Connect(int connectionId, Platform::Object^ target)
{
    switch (connectionId)
    {
    case 1:
        (safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(target))->TextChanged +=
            ref new ::Windows::UI::Xaml::Controls::TextChangedEventHandler(this, (void (::SDKSample::ListViewSimple::Scenario1::*)(Platform::Object^, Windows::UI::Xaml::Controls::TextChangedEventArgs^))&Scenario1::InputBox_TextChanged);
        break;
    case 2:
        (safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(target))->TextChanged +=
            ref new ::Windows::UI::Xaml::Controls::TextChangedEventHandler(this, (void (::SDKSample::ListViewSimple::Scenario1::*)(Platform::Object^, Windows::UI::Xaml::Controls::TextChangedEventArgs^))&Scenario1::TransferBox_TextChanged);
        break;
    case 3:
        (safe_cast<::Windows::UI::Xaml::Controls::Primitives::ButtonBase^>(target))->Click +=
            ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::SDKSample::ListViewSimple::Scenario1::*)(Platform::Object^, Windows::UI::Xaml::RoutedEventArgs^))&Scenario1::button_Click);
        break;
    }
    (void)connectionId; // Unused parameter
    (void)target; // Unused parameter
    _contentLoaded = true;
}

