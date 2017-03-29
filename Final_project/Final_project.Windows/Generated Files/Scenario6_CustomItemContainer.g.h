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
                ref class Button;
                ref class TextBox;
                ref class TextBlock;
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
        partial ref class Scenario6 : public ::Windows::UI::Xaml::Controls::Page, 
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
            private: ::Windows::UI::Xaml::VisualState^ OutputDefaultLayout;
            private: ::Windows::UI::Xaml::VisualState^ OutputBelow768Layout;
            private: ::Windows::UI::Xaml::Controls::Button^ BeginButton;
            private: ::Windows::UI::Xaml::Controls::Button^ ClearConsoleButton;
            private: ::Windows::UI::Xaml::Controls::Button^ ContinueExecButton;
            private: ::Windows::UI::Xaml::Controls::Button^ StepExecButton;
            private: ::Windows::UI::Xaml::Controls::Button^ DebugExecButton;
            private: ::Windows::UI::Xaml::Controls::Button^ ExecButton;
            private: ::Windows::UI::Xaml::Controls::TextBox^ ValueAcceptor;
            private: ::Windows::UI::Xaml::Controls::TextBlock^ ConsoleOutputBlock;
            private: ::Windows::UI::Xaml::Controls::TextBlock^ InputTextBlock1;
            private: ::Windows::UI::Xaml::VisualState^ InputDefaultLayout;
            private: ::Windows::UI::Xaml::VisualState^ InputBelow768Layout;
        };
    }
}
