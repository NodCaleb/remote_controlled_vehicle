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
