using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Devices.Bluetooth;
using Windows.Devices.Bluetooth.GenericAttributeProfile;

namespace ConsoleApplication2
{
    class Program
    {
        async static void BLE()
        {

            BluetoothLEDevice device = await BluetoothLEDevice.FromBluetoothAddressAsync(000000000001);
            Console.WriteLine(device);

            foreach (GattDeviceService i in device.GattServices)
            {
                Console.WriteLine(i.GetAllIncludedServices());
            }

            Guid ServiceId = new Guid("00000000-0000-1000-8000-00805f9b34fb");
            GattDeviceService services = device.GetGattService(ServiceId);

            foreach (var i in services.GetAllCharacteristics())
            {
                if (i.Uuid != ServiceId) { 
                    Console.WriteLine(i.Uuid);
                    Console.WriteLine(i.UserDescription);
                    GattReadResult result = await i.ReadValueAsync(BluetoothCacheMode.Cached);

                    //Console.WriteLine(result.Value);
                    Console.WriteLine(string.Format("{0}", result.Value));
                    
                    foreach (char j in result.Value.Capacity.ToString())
                    {
                        Console.WriteLine(string.Format("{0}", j));
                    }
                }
            }
        }

        static void Main(string[] args)
        {
            BLE();

            Console.Read();
        }
    }
}
