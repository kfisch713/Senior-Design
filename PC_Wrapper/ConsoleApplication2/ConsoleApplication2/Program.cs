using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading;
using System.Threading.Tasks;
using System.Diagnostics;
using Windows.Devices.Bluetooth;
using Windows.Devices.Bluetooth.GenericAttributeProfile;

namespace ConsoleApplication2
{
    class Program
    {
        static volatile string accel_x = "0";
        static volatile string accel_y = "0";
        static volatile string accel_z = "0";
               
        static volatile string gyro_x = "0";
        static volatile string gyro_y = "0";
        static volatile string gyro_z = "0";
               
        static volatile string mag_x = "0";
        static volatile string mag_y = "0";
        static volatile string mag_z = "0";
               
        static volatile string joy_x = "0";
        static volatile string joy_y = "0";
               
        static volatile string buttons = "00000";
               
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

            // TODO: (maybe) selectively pull only the data we request

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
                
                // Polling loop for data
                while (true)
                {
                    string output = "";
                    StringBuilder sb = new StringBuilder(output);

                    // Time
                    if (flags.Contains('t'))
                    {
                        time = party.ElapsedMilliseconds;
                        sb.Append(time + ", ");
                    }

                    // Accelerometer
                    if (flags.Contains('a'))
                    {
                        // Accelerometer calculation
                        double data_x = (short)ushort.Parse(accel_x) * (6.0 / 32768.0);
                        double data_y = (short)ushort.Parse(accel_y) * (6.0 / 32768.0);
                        double data_z = (short)ushort.Parse(accel_z) * (6.0 / 32768.0);

                        sb.Append(data_x + ", ");
                        sb.Append(data_y + ", ");
                        sb.Append(data_z + ", ");
                    }

                    // Gyroscope
                    if (flags.Contains('g'))
                    {
                        double data_x = (short)ushort.Parse(gyro_x);
                        double data_y = (short)ushort.Parse(gyro_y);
                        double data_z = (short)ushort.Parse(gyro_z);

                        sb.Append(data_x + ", ");
                        sb.Append(data_y + ", ");
                        sb.Append(data_z + ", ");
                    }

                    // Magnetometer
                    if (flags.Contains('m'))
                    {
                        double data_x = (short)ushort.Parse(mag_x);
                        double data_y = (short)ushort.Parse(mag_y);
                        double data_z = (short)ushort.Parse(mag_z);

                        sb.Append(data_x + ", ");
                        sb.Append(data_y + ", ");
                        sb.Append(data_z + ", ");
                    }

                    // Buttons
                    if (flags.Contains('b'))
                    {
                        // Button order: TBD by Kyle
                        for (int i = 0; i < 5; i++)
                        {
                            sb.Append(buttons[i] + ", ");
                        }
                    }

                    // Joystick
                    if (flags.Contains('j'))
                    {
                        sb.Append(joy_x + ", ");
                        sb.Append(joy_y + ", ");
                    }
                    
                    Console.WriteLine(sb.ToString().Substring(0, sb.ToString().Length - 2));
                    Thread.Sleep(10);
                }
                
            } else
            {
                Console.WriteLine("Usage: explore.exe -[agmtjb]");
            }
            
            // Uncomment this to hang the console after execution
            //Console.Read();
        }
    }
}
