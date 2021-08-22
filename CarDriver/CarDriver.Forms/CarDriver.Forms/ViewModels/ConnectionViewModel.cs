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
using Plugin.BluetoothClassic.Abstractions;
using Xamarin.Forms;


namespace CarDriver.Forms.ViewModels
{
    public class ConnectionViewModel : BaseViewModel
    {
        private IBluetoothLE _ble;
        private IAdapter _adapterLe;
        private IBluetoothAdapter _bluetoothAdapter;

        //private BluetoothAdapter mBluetoothAdapter = null;
        //private BluetoothSocket btSocket = null;

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
            _adapterLe = CrossBluetoothLE.Current.Adapter;
            _adapterLe.DeviceDiscovered += DeviceDiscoveredLe;
            _devices = new ObservableCollection<BluetoothDevice>();
            _bluetoothAdapter = DependencyService.Resolve<IBluetoothAdapter>();

            Title = "Bluetooth connection";
            ButtonText = "Scan";
            ButtonEnabled = false;
            ConnectCommand = new Command(async (d) => await ConnectBluetooth(d));
            ScanCommand = new Command(async () => await ScanBluetoothLe());
            ListSavedCommand = new Command(ListSavedBluetooth);
            TestCommand = new Command(TransmitTest);

            ListSavedBluetooth();
        }

        public ICommand ConnectCommand { get; }
        public ICommand ScanCommand { get; }
        public ICommand ListSavedCommand { get; }
        public ICommand TestCommand { get; }

        public void ListSavedBluetooth()
        {
            if (!_bluetoothAdapter.Enabled) return;

            var devices = _bluetoothAdapter.BondedDevices;

            foreach (var device in devices)
            {
                _devices.Add(new BluetoothDevice(device));
            }
        }

        public async Task<bool> ConnectBluetooth(object d)
        {
            var device = (d as BluetoothDevice)?.Device;

            if (device != null)
            {
                var connection = _bluetoothAdapter.CreateManagedConnection(device);
                try
                {
                    connection.Connect();
                    App.CurrentBluetoothConnection = connection;

                    ButtonEnabled = true;

                    return true;
                }
                catch (BluetoothConnectionException exception)
                {
                    //await DisplayAlert("Connection error",
                    //    $"Can not connect to the device: {bluetoothDeviceModel.Name}" +
                    //    $"({bluetoothDeviceModel.Address}).\n" +
                    //    $"Exception: \"{exception.Message}\"\n" +
                    //    "Please, try another one.",
                    //    "Close");

                    return false;
                }
                catch (Exception exception)
                {
                    //await DisplayAlert("Generic error", exception.Message, "Close");
                    return false;
                }
            }

            return false;
        }

        public void TransmitTest()
        {
            var data = new[] {(byte) 'H', (byte) 'e', (byte) 'l', (byte) 'l', (byte) 'o', (byte) '!'};

            if (App.CurrentBluetoothConnection?.ConnectionState == ConnectionState.Connected)
            {
                App.CurrentBluetoothConnection.Transmit(data);
            }
        }

        #region BLE
        private void DeviceDiscoveredLe(object sender, Plugin.BLE.Abstractions.EventArgs.DeviceEventArgs e)
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
        public async Task ScanBluetoothLe()
        {
            if (!ButtonEnabled) return;

            var state = _ble.State;

            if (state == BluetoothState.On)
            {
                ButtonEnabled = false;
                _devices.Clear();
                _adapterLe.ScanMode = ScanMode.Balanced;
                _adapterLe.ScanTimeout = 10000;
                ButtonText = "Scanning";
                await _adapterLe.StartScanningForDevicesAsync();
                ButtonText = "Scan";
                ButtonEnabled = true;
            }
        }

        public async Task ListSavedBluetoothLe()
        {
            if (!ButtonEnabled) return;

            var state = _ble.State;

            if (state == BluetoothState.On)
            {
                var savedDevices = _adapterLe.GetSystemConnectedOrPairedDevices();

                foreach (var device in savedDevices)
                {
                    // this assumes that you already have the BLE object
                    // from the BLE plugin
                    var obj = device.NativeDevice;
                    // we want the "Address" property
                    PropertyInfo propInfo = obj.GetType().GetProperty("Address");
                    string address = (string)propInfo.GetValue(obj, null);

                    _devices.Add(new BluetoothDevice(device, address));
                }
            }
        }

        public async Task ConnectBluetoothLe(object d)
        {
            try
            {
                var device = d as BluetoothDevice;

                //_adapterLe.

                await _adapterLe.ConnectToDeviceAsync(device.DeviceLe);

                var services = await device.DeviceLe.GetServicesAsync();
            }
            catch (Exception e)
            {

            }
        } 
        #endregion
    }
}
