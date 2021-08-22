using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using CarDriver.Forms.Models;
using Plugin.BLE;
using Plugin.BLE.Abstractions.Contracts;
using Xamarin.Forms;

namespace CarDriver.Forms.ViewModels
{
    public class ConnectionViewModel : BaseViewModel
    {
        private IBluetoothLE _ble;
        private IAdapter _adapter;

        private ObservableCollection<BluetoothDevice> _devices;
        public ObservableCollection<BluetoothDevice> Devices => _devices;

        private string _buttonText;
        public string ButtonText
        {
            get => _buttonText;
            set => SetProperty(ref _buttonText, value);
        }

        private bool _buttonEnabled;
        public bool ButtonEnabled
        {
            get => _buttonEnabled;
            set => SetProperty(ref _buttonEnabled, value);
        }

        public ConnectionViewModel()
        {
            _ble = CrossBluetoothLE.Current;
            _adapter = CrossBluetoothLE.Current.Adapter;
            _adapter.DeviceDiscovered += DeviceDiscovered;
            _devices = new ObservableCollection<BluetoothDevice>();
            
            Title = "Bluetooth connection";
            ButtonText = "Scan";
            ButtonEnabled = true;
            ConnectCommand = new Command(async (d) => await ConnectBluetooth(d));
            ScanCommand = new Command(async () => await ScanBluetooth());
        }

        private void DeviceDiscovered(object sender, Plugin.BLE.Abstractions.EventArgs.DeviceEventArgs e)
        {
            var device = e.Device;

            // this assumes that you already have the BLE object
            // from the BLE plugin
            var obj = device.NativeDevice;
            // we want the "Address" property
            PropertyInfo propInfo = obj.GetType().GetProperty("Address");
            string address = (string)propInfo.GetValue(obj, null);

            _devices.Add(new BluetoothDevice(device, address));
        }

        public ICommand ConnectCommand { get; }
        public ICommand ScanCommand { get; }

        public async Task ScanBluetooth()
        {
            if (!ButtonEnabled) return;

            var state = _ble.State;

            if (state == BluetoothState.On)
            {
                ButtonEnabled = false;
                _devices.Clear();
                _adapter.ScanMode = ScanMode.Balanced;
                _adapter.ScanTimeout = 10000;
                ButtonText = "Scanning";
                await _adapter.StartScanningForDevicesAsync();
                ButtonText = "Scan";
                ButtonEnabled = true;
            }
        }

        public async Task ConnectBluetooth(object d)
        {
            try
            {
                var device = d as BluetoothDevice;

                await _adapter.ConnectToDeviceAsync(device.Device);

                var services = await device.Device.GetServicesAsync();
            }
            catch (Exception e)
            {

            }
        }
    }
}
