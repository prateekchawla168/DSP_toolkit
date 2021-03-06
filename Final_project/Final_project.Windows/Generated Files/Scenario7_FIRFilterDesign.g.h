﻿

#pragma once
//------------------------------------------------------------------------------
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
//------------------------------------------------------------------------------

namespace Windows {
    namespace UI {
        namespace Xaml {
            namespace Controls {
                ref class Grid;
                ref class TextBlock;
                ref class Slider;
                ref class TextBox;
                ref class Button;
            }
        }
    }
}
namespace Windows {
    namespace UI {
        namespace Xaml {
            ref class VisualState;
        }
    }
}

namespace SDKSample
{
    namespace ListViewSimple
    {
        partial ref class Scenario7 : public ::Windows::UI::Xaml::Controls::Page, 
            public ::Windows::UI::Xaml::Markup::IComponentConnector
        {
        public:
            void InitializeComponent();
            virtual void Connect(int connectionId, ::Platform::Object^ target);
        
        private:
            bool _contentLoaded;
        
            private: ::Windows::UI::Xaml::Controls::Grid^ LayoutRoot;
            private: ::Windows::UI::Xaml::Controls::Grid^ Input;
            private: ::Windows::UI::Xaml::Controls::Grid^ Output;
            private: ::Windows::UI::Xaml::Controls::TextBlock^ OutputBlock;
            private: ::Windows::UI::Xaml::Controls::Slider^ InputType;
            private: ::Windows::UI::Xaml::Controls::Slider^ FilterType;
            private: ::Windows::UI::Xaml::Controls::TextBox^ FreqBox;
            private: ::Windows::UI::Xaml::Controls::TextBox^ PassBandBox;
            private: ::Windows::UI::Xaml::Controls::TextBox^ StopBandBox;
            private: ::Windows::UI::Xaml::Controls::TextBox^ PassBandFreqBox;
            private: ::Windows::UI::Xaml::Controls::TextBox^ StopBandFreqBox;
            private: ::Windows::UI::Xaml::Controls::Button^ DesignFilterButton;
            private: ::Windows::UI::Xaml::Controls::Button^ ClearConsoleButton;
            private: ::Windows::UI::Xaml::Controls::TextBlock^ InputTextBlock1;
            private: ::Windows::UI::Xaml::VisualState^ InputDefaultLayout;
            private: ::Windows::UI::Xaml::VisualState^ InputBelow768Layout;
        };
    }
}

