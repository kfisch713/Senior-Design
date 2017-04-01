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
    public delegate void ValueChangeCompletedHandler(string value);
    public delegate void DeviceConnectionUpdatedHandler(bool isConnected, string error);

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

        BluetoothLEDevice _device = null;
        private GattDeviceService _service = null;
        private GattCharacteristic _characteristic = null;

        public event ValueChangeCompletedHandler ValueChangeCompleted;
        public event DeviceConnectionUpdatedHandler DeviceConnectionUpdated;

        public void Deinitialize()
        {
            if (_characteristic != null)
            {
                _characteristic.ValueChanged -= Oncharacteristic_ValueChanged;
                _characteristic = null;
            }

            if (_service != null)
            {
                _service.Device.ConnectionStatusChanged -= OnConnectionStatusChanged;
                _service = null;
            }
        }

        public async void InitializeServiceAsync(string deviceId)
        {
            try
            {
                Deinitialize();
                _device = await BluetoothLEDevice.FromBluetoothAddressAsync(000000000001);
                _service = _device.GetGattService(new Guid("00000000-0000-0000-0000-000000000000"));

                if (DeviceConnectionUpdated != null && (_service.Device.ConnectionStatus == BluetoothConnectionStatus.Connected))
                {
                    DeviceConnectionUpdated(true, null);
                }

                _service.Device.ConnectionStatusChanged += OnConnectionStatusChanged;

                _characteristic = _service.GetCharacteristics(new Guid("01100000-0000-0000-0000-000000000000"))[0];
                _characteristic.ValueChanged += Oncharacteristic_ValueChanged;

                var currentDesciptorValue = await _characteristic.ReadClientCharacteristicConfigurationDescriptorAsync();
                if ((currentDesciptorValue.Status != GattCommunicationStatus.Success) || 
                    (currentDesciptorValue.ClientCharacteristicConfigurationDescriptor != GattClientCharacteristicConfigurationDescriptorValue.Notify)){
                    await _characteristic.WriteClientCharacteristicConfigurationDescriptorAsync(GattClientCharacteristicConfigurationDescriptorValue.Notify);
                }
            }
            catch (Exception e)
            {
                Debug.WriteLine("ERROR: Accessing your device failed." + Environment.NewLine + e.Message);

                DeviceConnectionUpdated?.Invoke(false, "Accessing device failed: " + e.Message);
            }
        }

        private void Oncharacteristic_ValueChanged(GattCharacteristic sender, GattValueChangedEventArgs args)
        {
            var data = new byte[args.CharacteristicValue.Length];

            DataReader.FromBuffer(args.CharacteristicValue).ReadBytes(data);

            DateTimeOffset time = args.Timestamp;

            Debug.WriteLine("Oncharacteristic_ValueChanged : " + data.ToString());

            ValueChangeCompleted?.Invoke(data.ToString());
        }

        private void OnConnectionStatusChanged(BluetoothLEDevice sender, object args)
        {
            if (sender.ConnectionStatus == BluetoothConnectionStatus.Connected)
            {
                Debug.WriteLine("Connected");
            }
            else
            {
                Debug.WriteLine("Disconnected");
            }

            DeviceConnectionUpdated?.Invoke(sender.ConnectionStatus == BluetoothConnectionStatus.Connected, null);
        }

        async static void BLE()
        {
            GattReadResult result = null;
            BluetoothLEDevice device = await BluetoothLEDevice.FromBluetoothAddressAsync(000000000001);
            
            Console.WriteLine(device.Name);

            //foreach (GattDeviceService i in device.GattServices)
            //{
            //    Console.WriteLine(i.GetAllIncludedServices());
            //}

            //Guid ServiceId = new Guid("00000000-0000-1000-8000-00805f9b34fb");

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
                        // Console.WriteLine(output);
                    }

                    switch (gc.Uuid.ToString())
                    {
                        // Accel X
                        case "01100000-0000-0000-0000-000000000000":
                            string[] AccelValues = output.Split(' ');
                            accel_x = AccelValues[0];
                            accel_y = AccelValues[1];
                            accel_z = AccelValues[2];
                            break;

                        // Accel Y
                        case "01200000-0000-0000-0000-000000000000":
                            //accel_y = output;
                            break;

                        // Accel Z
                        case "01300000-0000-0000-0000-000000000000":
                            //accel_z = output;
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
            //try
            //{
            //    Task.Run(async () =>
            //    {
            //        await BLE();
            //    });
            //}
            //catch(Exception e)
            //{
            //    Console.WriteLine("A Bluetooth-related error has occured. Please check that the Bluetooth device is properly connected.");
            //    return;
            //}

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
                        double data_x = (short)ushort.Parse(accel_x) * (8.0 / 32768.0);
                        double data_y = (short)ushort.Parse(accel_y) * (8.0 / 32768.0);
                        double data_z = (short)ushort.Parse(accel_z) * (8.0 / 32768.0);

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
                    
                    // Print the output string with the trailing comma cut off
                    Console.WriteLine(sb.ToString().Substring(0, sb.ToString().Length - 2));

                    // Edit this depending on the output rate of BLE
                    //Thread.Sleep(100);
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
