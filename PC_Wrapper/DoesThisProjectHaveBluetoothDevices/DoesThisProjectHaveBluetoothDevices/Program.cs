using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Devices.Bluetooth.GenericAttributeProfile;
using Windows.Devices.Enumeration;
using Windows.Devices.Sensors;

namespace DoesThisProjectHaveBluetoothDevices
{
    class Program
    {

         async static Task BLE()
        {
            /*var themometerServices = await Windows.Devices.Enumeration
                .DeviceInformation.FindAllAsync(GattDeviceService
                    .GetDeviceSelectorFromUuid(GattServiceUuids.GenericAccess),
                null);
                */

            //DeviceInformationCollection services = await DeviceInformation
            //    .FindAllAsync(GattDeviceService.GetDeviceSelectorFromUuid(GattServiceUuids.GenericAccess), null);

            DeviceInformationCollection services = await DeviceInformation.FindAllAsync(
               GattDeviceService.GetDeviceSelectorFromUuid(new Guid("00000000-0000-1000-8000-00805f9b34fb")));

            
            //DeviceInformationCollection services = await DeviceInformation.FindAllAsync();

            //service = await GattDeviceService.FromIdAsync(device.Id);
            
            DeviceInformation device = null;
            GattDeviceService service = null;
            List<GattCharacteristic> characteristicsList = null;

            Console.WriteLine("Finding devices");
            foreach (var i in services)
            {
                Console.WriteLine(i.Name);

                if(i.Name.Equals("Cymote Alpha"))
                {
                    Console.WriteLine("Found it");
                    
                    foreach (var j in i.Properties)
                    {
                        Console.WriteLine(j);
                    }

                    Console.WriteLine("Device ID\n");
                    Console.WriteLine(device.Properties["System.Devices.DeviceInstanceId"]);

                    device = i;
                    service = await GattDeviceService.FromIdAsync("000000000001");
                    Console.WriteLine("assigned service");
                    //characteristicsList = GattDeviceService.GetCharacteristics(new Guid("00000000-0000-1000-8000-00805f9b34fb"));
                }


            }
            /*
            foreach (var i in device.Properties)
            {
                Console.WriteLine(i);
            }
            */

            foreach (var i in service.GetIncludedServices(new Guid("00000000-0000-1000-8000-00805f9b34fb")))
            {
                Console.Write(i);
            }


            foreach (var i in characteristicsList)
            {
                Console.WriteLine(i);
            }

            Console.WriteLine("Done!");
        }

        static void Main()
        {
            //Task t1 = new Task(BLE).Start;
            BLE();
            
            Console.Read();

        }

        
    }
}
