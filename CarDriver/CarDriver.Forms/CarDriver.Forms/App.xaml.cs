using CarDriver.Forms.Services;
using CarDriver.Forms.Views;
using System;
using System.Text;
using System.Threading;
using Plugin.BluetoothClassic.Abstractions;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace CarDriver.Forms
{
    public partial class App : Application
    {
        public static IBluetoothManagedConnection CurrentBluetoothConnection { get; set; }
        public static int LeftPower { get; set; }
        public static int RightPower { get; set; }

        public App()
        {
            InitializeComponent();

            DependencyService.Register<MockDataStore>();
            MainPage = new AppShell();

            var transmittingThread = new Thread(TransmitPowers);
            transmittingThread.Start();
        }

        private static void TransmitPowers()
        {
            if (CurrentBluetoothConnection?.ConnectionState == ConnectionState.Connected)
            {
                CurrentBluetoothConnection.Transmit(Encoding.ASCII.GetBytes($"L{LeftPower}{Environment.NewLine}"));
                CurrentBluetoothConnection.Transmit(Encoding.ASCII.GetBytes($"R{RightPower}{Environment.NewLine}"));
            }

            Thread.Sleep(100);
            TransmitPowers();
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
