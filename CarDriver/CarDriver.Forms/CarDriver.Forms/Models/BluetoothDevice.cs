using System;
using System.Collections.Generic;
using System.Text;
using Plugin.BLE.Abstractions.Contracts;

namespace CarDriver.Forms.Models
{
    public class BluetoothDevice
    {
        public Guid Id { get; set; }
        public string Name { get; set; }
        public string DisplayName => Name ?? Id.ToString();
        public IDevice Device { get; set; }

        public BluetoothDevice()
        {
            
        }

        public BluetoothDevice(IDevice device)
        {
            Id = device.Id;
            Name = device.Name;
            Device = device;
        }
    }
}
