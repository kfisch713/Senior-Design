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
            GattReadResult result = null;
            BluetoothLEDevice device = await BluetoothLEDevice.FromBluetoothAddressAsync(000000000001);
            Console.WriteLine(device);

            foreach (GattDeviceService i in device.GattServices)
            {
                Console.WriteLine(i.GetAllIncludedServices());
            }

            Guid ServiceId = new Guid("00000000-0000-1000-8000-00805f9b34fb");
            GattDeviceService services = device.GetGattService(ServiceId);

            while (true)
            {
                foreach (var i in services.GetAllCharacteristics())
                {
                    /*
                    if (i.Uuid != ServiceId) { 
                        Console.WriteLine(i.Uuid);
                        Console.WriteLine(i.UserDescription);
                        result = await i.ReadValueAsync(BluetoothCacheMode.Uncached);

                        //Console.WriteLine(result.Value);
                        Console.WriteLine(string.Format("{0}", result.Value));

                        var dataReader = Windows.Storage.Streams.DataReader.FromBuffer(result.Value);
                        var output = dataReader.ReadString(result.Value.Length);
                        Console.WriteLine(output);
                    */

                    if (i.Uuid == new Guid("03000000-0000-0000-0000-000000000000"))
                    {
                        try
                        {
                            result = await i.ReadValueAsync(BluetoothCacheMode.Uncached);
                            if (result.Status == GattCommunicationStatus.Success)
                            {
                                var dataReader = Windows.Storage.Streams.DataReader.FromBuffer(result.Value);
                                var output = dataReader.ReadString(result.Value.Length);
                                Console.WriteLine(output);
                            }
                            else
                            {
                                Console.WriteLine("GattReadResult is unreachable");
                            }
                        }

                        catch (System.ArgumentException e)
                        {
                            Console.WriteLine(e);
                        }
                        
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
