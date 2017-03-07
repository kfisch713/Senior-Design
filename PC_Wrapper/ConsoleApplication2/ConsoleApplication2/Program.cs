using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Diagnostics;
using Windows.Devices.Bluetooth;
using Windows.Devices.Bluetooth.GenericAttributeProfile;

namespace ConsoleApplication2
{
    class Program
    {
        // GUID/UUID definitions
        Guid accel_x_guid = new Guid("01100000-0000-0000-0000-000000000000");
        Guid accel_y_guid = new Guid("01200000-0000-0000-0000-000000000000");
        Guid accel_z_guid = new Guid("01300000-0000-0000-0000-000000000000");

        Guid gyro_x_guid = new Guid("02100000-0000-0000-0000-000000000000");
        Guid gyro_y_guid = new Guid("02200000-0000-0000-0000-000000000000");
        Guid gyro_z_guid = new Guid("02300000-0000-0000-0000-000000000000");

        Guid mag_x_guid = new Guid("03100000-0000-0000-0000-000000000000");
        Guid mag_y_guid = new Guid("03200000-0000-0000-0000-000000000000");
        Guid mag_z_guid = new Guid("03300000-0000-0000-0000-000000000000");

        Guid joy_x_guid = new Guid("04100000-0000-0000-0000-000000000000");
        Guid joy_y_guid = new Guid("04200000-0000-0000-0000-000000000000");

        Guid butt_guid = new Guid("05000000-0000-0000-0000-000000000000");

        async static void BLE()
        {
            GattReadResult result = null;
            BluetoothLEDevice device = await BluetoothLEDevice.FromBluetoothAddressAsync(000000000001);
            Stopwatch stopwatch = new Stopwatch();
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



                    if (i.Uuid == new Guid("04000000-0000-0000-0000-000000000000"))
                    {
                        try
                        {
                            stopwatch.Reset();
                            stopwatch.Start();
                            result = await i.ReadValueAsync(BluetoothCacheMode.Uncached);
                            if (result.Status == GattCommunicationStatus.Success)
                            {
                                var dataReader = Windows.Storage.Streams.DataReader.FromBuffer(result.Value);
                                var output = dataReader.ReadString(result.Value.Length);
                                //Console.WriteLine(String.Format("raw output: {0}", output));
                                Int16 data = (Int16)UInt16.Parse(output);
                                Console.WriteLine(data * (6.0 / 32768.0));
                                stopwatch.Stop();
                                //Console.WriteLine(stopwatch.ElapsedMilliseconds);
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
            // TODO: check for correct argument structure

            // Flag checking
            // a - acceelrometer
            // g - gyroscope
            // m - magnetometer
            // t - time
            // j - joystick
            // b - buttons
            if (args.Length > 0 && Regex.IsMatch(args[0], "^-[agmtjb]*$"))
            {
                String flags = args[0];

                // Accelerometer
                if (flags.Contains('a'))
                {
                    
                }

                // Gyroscope
                if (flags.Contains('g'))
                {
                    
                }

                // Magnetometer
                if (flags.Contains('m'))
                {
                    
                }

                // Time
                if (flags.Contains('t'))
                {
                    
                }

                // Joystick
                if (flags.Contains('j'))
                {
                    
                }

                // Buttons
                if (flags.Contains('b'))
                {
                    
                }
            } else
            {
                Console.WriteLine("Usage: explore.exe -[agmtjb]");
            }

            BLE();

            Console.Read();
        }
    }
}
