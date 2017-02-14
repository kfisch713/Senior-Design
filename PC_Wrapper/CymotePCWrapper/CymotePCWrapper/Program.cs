using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Devices.Bluetooth;
using Windows.Devices.Bluetooth.GenericAttributeProfile;
using Windows.Devices.Enumeration;

namespace CymotePCWrapper
{

    class Program
    {
        private BluetoothLEDevice ble = new BluetoothLEDevice();

        static void Main(string[] args)
        {
            Console.WriteLine("hello world.");


        }


    }
}
