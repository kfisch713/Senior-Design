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
using Windows.Devices.Enumeration.Pnp;
using Windows.Storage.Streams;

// BLE data extractor leaning heavily on code from 
// https://github.com/DrJukka/BLETestStuffWindows/blob/master/HeartbeatFg/HeartbeatFg/Engine/HeartBeatEngine.cs
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

        async static Task BLE()
        {
            GattReadResult result = null;
            BluetoothLEDevice device = await BluetoothLEDevice.FromBluetoothAddressAsync(000000000002);
            
            Console.WriteLine(device.Name);

            // Get the service containing our characteristic
            Guid ServiceId = new Guid("00000000-0000-0000-0000-000000000000");
            GattDeviceService services = device.GetGattService(ServiceId);

            // This block assumes multiple characteristics and polls from each separate characteristic containing the needed data
            while (true)
            {
                foreach (var gc in services.GetAllCharacteristics())
                {
                    string output = "";

                    if (gc.Uuid != ServiceId)
                    {
                        result = await gc.ReadValueAsync(BluetoothCacheMode.Uncached);
                        var dataReader = DataReader.FromBuffer(result.Value);
                        output = dataReader.ReadString(result.Value.Length);
                        //Console.WriteLine(output);
                    }

                    switch (gc.Uuid.ToString())
                    {
                        // AccelX AccelY AccelZ
                        case "01100000-0000-0000-0000-000000000000":
                            string[] AccelValues = output.Split(' ');
                            accel_x = AccelValues[0];
                            accel_y = AccelValues[1];
                            accel_z = AccelValues[2];

                            break;

                        // GyroX GyroY GyroZ
                        case "02100000-0000-0000-0000-000000000000":
                            string[] GyroValues = output.Split(' ');
                            gyro_x = GyroValues[0];
                            gyro_y = GyroValues[1];
                            gyro_z = GyroValues[2];
                            break;

                        // MagX MagY MagZ
                        case "03100000-0000-0000-0000-000000000000":
                            string[] MagValues = output.Split(' ');
                            mag_x = MagValues[0];
                            mag_y = MagValues[1];
                            mag_z = MagValues[2];
                            break;

                        // JoystickX JoystickY JoystickZ Buttons
                        case "04100000-0000-0000-0000-000000000000":
                            string[] JoyButValues = output.Split(' ');
                            joy_x = JoyButValues[0];
                            joy_y = JoyButValues[1];
                            buttons = JoyButValues[2];
                            break;

                        // Time
                        case "05100000-0000-0000-0000-000000000000":
                            
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
            try
            {
                Task.Run(async () =>
                {
                    await BLE();
                });
            }
            catch (Exception e)
            {
                Console.WriteLine("A Bluetooth-related error has occured. Please check that the Bluetooth device is properly connected.");
                return;
            }

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
                        double data_x = (short)ushort.Parse(accel_x) * 8.0 / 32768;
                        double data_y = (short)ushort.Parse(accel_y) * 8.0 / 32768;
                        double data_z = (short)ushort.Parse(accel_z) * 8.0 / 32768; 

                        sb.Append(data_x + ", ");
                        sb.Append(data_y + ", ");
                        sb.Append(data_z + ", ");
                    }

                    // Gyroscope
                    if (flags.Contains('g'))
                    {
                        double data_x = (short)ushort.Parse(gyro_x) * 0.00875;
                        double data_y = (short)ushort.Parse(gyro_y) * 0.00875;
                        double data_z = (short)ushort.Parse(gyro_z) * 0.00875;

                        sb.Append(data_x + ", ");
                        sb.Append(data_y + ", ");
                        sb.Append(data_z + ", ");
                    }

                    // Magnetometer
                    if (flags.Contains('m'))
                    {
                        double data_x = (short)ushort.Parse(mag_x) * 0.00014;
                        double data_y = (short)ushort.Parse(mag_y) * 0.00014;
                        double data_z = (short)ushort.Parse(mag_z) * 0.00014;

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
                    
                    // Print the output string with the trailing comma cut off
                    Console.WriteLine(sb.ToString().Substring(0, sb.ToString().Length - 2));

                    // Edit this depending on the output rate of BLE
                    Thread.Sleep(100);
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
