using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Input;
using Xamarin.Forms;

namespace CarDriver.Forms.ViewModels
{
    public class ManualDriverViewModel : BaseViewModel
    {
        private int _clicks;

        string _labelContent = string.Empty;
        public string LabelContent
        {
            get { return _labelContent; }
            set { SetProperty(ref _labelContent, value); }
        }

        private string _title;
        public string Title
        {
            get { return _title; }
            set { SetProperty(ref _labelContent, value); }
        }

        private int _joystickXposition;
        public int JoystickXposition
        {
            get { return _joystickXposition; }
            set { SetProperty(ref _joystickXposition, value); }
        }

        private int _joystickYposition;
        public int JoystickYposition
        {
            get { return _joystickYposition; }
            set { SetProperty(ref _joystickYposition, value); }
        }

        private int _joystickDistance;
        public int JoystickDistance
        {
            get { return _joystickDistance; }
            set { SetProperty(ref _joystickDistance, value); }
        }

        private int _joystickAngle;

        public int JoystickAngle
        {
            get { return _joystickAngle; }
            set { SetProperty(ref _joystickAngle, value); }
        }

        public ManualDriverViewModel()
        {
            _clicks = 0;
            Title = "Manual driver";
            LabelContent = $"Clicked {_clicks} times";
            ClickCommand = new Command(IncreaseCounter);
        }

        public ICommand ClickCommand { get; }

        public void IncreaseCounter()
        {
            _clicks++;
            LabelContent = $"Clicked {_clicks} times";
        }
    }
}
