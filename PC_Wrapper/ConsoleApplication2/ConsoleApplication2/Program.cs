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
        static string accel_x = "";
        static string accel_y = "";
        static string accel_z = "";
         
        static string gyro_x = "";
        static string gyro_y = "";
        static string gyro_z = "";
         
        static string mag_x = "";
        static string mag_y = "";
        static string mag_z = "";
         
        static string joy_x = "";
        static string joy_y = "";
         
        static string buttons = "";

        static long time = 0;

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
                foreach (var gc in services.GetAllCharacteristics())
                {
                    string output = "";

                    if (gc.Uuid != ServiceId)
                    {
                        result = await gc.ReadValueAsync(BluetoothCacheMode.Uncached);
                        var dataReader = Windows.Storage.Streams.DataReader.FromBuffer(result.Value);
                        output = dataReader.ReadString(result.Value.Length);
                    }

                    switch (gc.Uuid.ToString())
                    {
                        // Accel X
                        case "01100000-0000-0000-0000-000000000000":
                            accel_x = output;
                            break;

                        // Accel Y
                        case "01200000-0000-0000-0000-000000000000":
                            accel_y = output;
                            break;

                        // Accel Z
                        case "01300000-0000-0000-0000-000000000000":
                            accel_z = output;
                            break;

                        // Gyro X
                        case "02100000-0000-0000-0000-000000000000":
                            gyro_x = output;
                            break;
                        
                        // Gyro Y
                        case "02200000-0000-0000-0000-000000000000":
                            gyro_y = output;
                            break;

                        // Gyro Z
                        case "02300000-0000-0000-0000-000000000000":
                            gyro_z = output;
                            break;

                        // Mag X
                        case "03100000-0000-0000-0000-000000000000":
                            mag_x = output;
                            break;

                        // Mag Y
                        case "03200000-0000-0000-0000-000000000000":
                            mag_y = output;
                            break;

                        // Mag Z
                        case "03300000-0000-0000-0000-000000000000":
                            mag_z = output;
                            break;

                        // Joystick X
                        case "04100000-0000-0000-0000-000000000000":
                            joy_x = output;
                            break;

                        // Joystick Y
                        case "04200000-0000-0000-0000-000000000000":
                            joy_y = output;
                            break;

                        // Buttons
                        case "05000000-0000-0000-0000-000000000000":
                            buttons = output;
                            break;

                        default:
                            break;
                    }
                }
            }
        }

        static void Main(string[] args)
        {
            // Variable to keep track of time since program execution
            // (This is when the party of the wrapper starts)
            Stopwatch party = new Stopwatch();
            party.Start();

            // Start the BLE polling
            BLE();


            // TODO: check for correct argument structure

            // TODO: selectively pull only the data we request

            // Flag checking
            // a - acceelrometer
            // g - gyroscope
            // m - magnetometer
            // t - time
            // j - joystick
            // b - buttons
            if (args.Length > 0 && Regex.IsMatch(args[0], "^-[agmtjb]*$"))
            {
                string flags = args[0];
                // Time | Accel | Gyro | Mag | Butts | Joy

                // Time
                if (flags.Contains('t'))
                {
                    time = party.ElapsedMilliseconds;
                }

                // Accelerometer
                if (flags.Contains('a'))
                {
                    // Accelerometer calculator
                    short data_x = (short)ushort.Parse(accel_x);
                    short data_y = (short)ushort.Parse(accel_y);
                    short data_z = (short)ushort.Parse(accel_z);


                    //Console.WriteLine(data * (6.0 / 32768.0));
                }

                // Gyroscope
                if (flags.Contains('g'))
                {
                    
                }

                // Magnetometer
                if (flags.Contains('m'))
                {
                    
                }
                
                // Buttons
                if (flags.Contains('b'))
                {

                }

                // Joystick
                if (flags.Contains('j'))
                {
                    
                }
                
            } else
            {
                Console.WriteLine("Usage: explore.exe -[agmtjb]");
            }
            
            Console.Read();
        }
    }
}
