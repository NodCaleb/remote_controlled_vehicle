using CarDriver.Forms.Services;
using CarDriver.Forms.Views;
using System;
using Plugin.BluetoothClassic.Abstractions;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace CarDriver.Forms
{
    public partial class App : Application
    {
        public static IBluetoothManagedConnection CurrentBluetoothConnection { get; set; }

        public App()
        {
            InitializeComponent();

            DependencyService.Register<MockDataStore>();
            MainPage = new AppShell();
        }

        protected override void OnStart()
        {
        }

        protected override void OnSleep()
        {
        }

        protected override void OnResume()
        {
        }
    }
}
