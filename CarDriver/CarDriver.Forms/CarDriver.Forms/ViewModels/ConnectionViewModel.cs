using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
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

        public ConnectionViewModel()
        {
            _ble = CrossBluetoothLE.Current;
            _adapter = CrossBluetoothLE.Current.Adapter;
            _adapter.DeviceDiscovered += DeviceDiscovered;
            _devices = new ObservableCollection<BluetoothDevice>();

            Title = "Bluetooth connection";
            ButtonText = "Scan";
            ConnectCommand = new Command(async () => await ConnectBluetooth());
        }

        private void DeviceDiscovered(object sender, Plugin.BLE.Abstractions.EventArgs.DeviceEventArgs e)
        {
            var device = e.Device;
            _devices.Add(new BluetoothDevice(e.Device));
        }

        public ICommand ConnectCommand { get; }

        public async Task ConnectBluetooth()
        {
            var state = _ble.State;

            if (state == BluetoothState.On)
            {
                _devices.Clear();
                _adapter.ScanMode = ScanMode.Balanced;
                _adapter.ScanTimeout = 10000;
                ButtonText = "Scanning";
                await _adapter.StartScanningForDevicesAsync();
                ButtonText = "Scan";
            }
        }
    }
}
