﻿

//------------------------------------------------------------------------------
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
//------------------------------------------------------------------------------
#include "pch.h"
#include "Scenario2_ClickEvents.xaml.h"




void ::SDKSample::ListViewSimple::Scenario2::InitializeComponent()
{
    if (_contentLoaded)
        return;

    _contentLoaded = true;

    // Call LoadComponent on ms-appx:///Scenario2_ClickEvents.xaml
    ::Windows::UI::Xaml::Application::LoadComponent(this, ref new ::Windows::Foundation::Uri(L"ms-appx:///Scenario2_ClickEvents.xaml"), ::Windows::UI::Xaml::Controls::Primitives::ComponentResourceLocation::Application);

    // Get the Grid named 'LayoutRoot'
    LayoutRoot = safe_cast<::Windows::UI::Xaml::Controls::Grid^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"LayoutRoot"));
    // Get the Grid named 'Input'
    Input = safe_cast<::Windows::UI::Xaml::Controls::Grid^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"Input"));
    // Get the Grid named 'Output'
    Output = safe_cast<::Windows::UI::Xaml::Controls::Grid^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"Output"));
    // Get the GridView named 'GridView2'
    GridView2 = safe_cast<::Windows::UI::Xaml::Controls::GridView^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"GridView2"));
    // Get the TextBlock named 'DescriptionBox'
    DescriptionBox = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"DescriptionBox"));
    // Get the TextBlock named 'InputTextBlock1'
    InputTextBlock1 = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"InputTextBlock1"));
    // Get the VisualState named 'DefaultLayout'
    DefaultLayout = safe_cast<::Windows::UI::Xaml::VisualState^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"DefaultLayout"));
    // Get the VisualState named 'Below768Layout'
    Below768Layout = safe_cast<::Windows::UI::Xaml::VisualState^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"Below768Layout"));
}

void ::SDKSample::ListViewSimple::Scenario2::Connect(int connectionId, Platform::Object^ target)
{
    switch (connectionId)
    {
    case 1:
        (safe_cast<::Windows::UI::Xaml::Controls::ListViewBase^>(target))->ContainerContentChanging +=
            ref new ::Windows::Foundation::TypedEventHandler<::Windows::UI::Xaml::Controls::ListViewBase^, ::Windows::UI::Xaml::Controls::ContainerContentChangingEventArgs^>(this, (void (::SDKSample::ListViewSimple::Scenario2::*)(Windows::UI::Xaml::Controls::ListViewBase^, Windows::UI::Xaml::Controls::ContainerContentChangingEventArgs^))&Scenario2::ItemGridView_ContainerContentChanging);
        (safe_cast<::Windows::UI::Xaml::Controls::ListViewBase^>(target))->ItemClick +=
            ref new ::Windows::UI::Xaml::Controls::ItemClickEventHandler(this, (void (::SDKSample::ListViewSimple::Scenario2::*)(Platform::Object^, Windows::UI::Xaml::Controls::ItemClickEventArgs^))&Scenario2::ItemClickHandler);
        break;
    }
    (void)connectionId; // Unused parameter
    (void)target; // Unused parameter
    _contentLoaded = true;
}

