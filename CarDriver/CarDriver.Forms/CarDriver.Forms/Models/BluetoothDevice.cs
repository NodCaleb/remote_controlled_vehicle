using System;
using System.Collections.Generic;
using System.Text;
using Plugin.BLE.Abstractions.Contracts;
using Xamarin.Forms.PlatformConfiguration;
using Plugin.BLE;
using Plugin.BluetoothClassic.Abstractions;

namespace CarDriver.Forms.Models
{
    public class BluetoothDevice
    {
        public Guid Id { get; set; }
        public string Name { get; set; }
        public string Address { get; set; }
        public string DisplayName => Name ?? Address;
        public IDevice DeviceLe { get; set; }
        public BluetoothDeviceModel Device { get; set; }

        public BluetoothDevice()
        {
            
        }

        public BluetoothDevice(IDevice device, string address)
        {
            Id = device.Id;
            Name = device.Name;
            Address = address;
            DeviceLe = device;
        }

        public BluetoothDevice(BluetoothDeviceModel model)
        {
            Name = model.Name;
            Address = model.Address;
            Device = model;
        }
    }
}
