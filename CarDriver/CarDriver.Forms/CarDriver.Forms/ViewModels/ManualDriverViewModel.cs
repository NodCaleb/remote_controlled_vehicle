using System;
using System.Windows.Input;
using CarDriver.Forms.CustomControls;
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
            set { SetProperty(ref _title, value); }
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

        int _xposition;
        public int Xposition
        {
            get { return _xposition; }
            set
            {
                SetProperty(ref _xposition, value);
                CalculatePower();
            }
        }

        int _yposition;
        public int Yposition
        {
            get { return _yposition; }
            set
            {
                SetProperty(ref _yposition, value);
                CalculatePower();
            }
        }

        int _leftPower;
        public int LeftPower
        {
            get { return _leftPower; }
            set
            {
                SetProperty(ref _leftPower, value);
            }
        }

        int _rightPower;
        public int RightPower
        {
            get { return _rightPower; }
            set
            {
                SetProperty(ref _rightPower, value);
            }
        }

        private void CalculatePower()
        {
            var reduceLeft = (Xposition < 0 ? -Xposition : 0) * (double)Math.Abs(Yposition) / 100D;
            var reduceRight = (Xposition > 0 ? Xposition : 0) * (double)Math.Abs(Yposition) / 100D;

            var leftPower = Yposition - (Yposition > 0 ? reduceLeft : -reduceLeft);
            var rightPower = Yposition - (Yposition > 0 ? reduceRight : -reduceRight);

            LeftPower = (int)Math.Round(leftPower * 2.5);
            RightPower = (int)Math.Round(rightPower * 2.5);
        }
    }
}
