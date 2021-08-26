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
                var leftBytes = new byte[3];
                var rightBytes = new byte[3];

                leftBytes[0] = (byte)'L';
                rightBytes[0] = (byte)'R';

                leftBytes[1] = (byte)(LeftPower > 0 ? 'F' : 'B');
                rightBytes[1] = (byte)(RightPower > 0 ? 'F' : 'B');

                leftBytes[2] = (byte)(Math.Abs(LeftPower));
                rightBytes[2] = (byte)(Math.Abs(RightPower));

                CurrentBluetoothConnection.Transmit(leftBytes);
                CurrentBluetoothConnection.Transmit(rightBytes);
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
