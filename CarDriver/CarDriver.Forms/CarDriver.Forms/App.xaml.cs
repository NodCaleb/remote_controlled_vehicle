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
                //3 bytes approach
                //var leftBytes = new byte[3];
                //var rightBytes = new byte[3];

                //leftBytes[0] = (byte)'L';
                //rightBytes[0] = (byte)'R';

                //leftBytes[1] = (byte)(LeftPower > 0 ? 'F' : 'B');
                //rightBytes[1] = (byte)(RightPower > 0 ? 'F' : 'B');

                //leftBytes[2] = (byte)(Math.Abs(LeftPower));
                //rightBytes[2] = (byte)(Math.Abs(RightPower));

                //1 byte approach

                //Read power values
                var leftValue = Math.Abs(LeftPower);
                var rightValue = Math.Abs(RightPower);

                //Reduce definition by 2 digits to contain motor index and direction
                leftValue = leftValue >> 2;
                rightValue = rightValue >> 2;

                //First bit: 0 - left motor, 1 - right motor
                leftValue = leftValue & 0b01111111;
                rightValue = rightValue | 0b10000000;

                //Second bit: 0 - forward, 1 - backward
                leftValue = LeftPower > 0 ? leftValue & 0b10111111 : leftValue | 0b01000000;
                rightValue = RightPower > 0 ? rightValue & 0b10111111 : rightValue | 0b01000000;

                CurrentBluetoothConnection.Transmit(new[] { (byte)leftValue });
                CurrentBluetoothConnection.Transmit(new[] { (byte)rightValue });
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
