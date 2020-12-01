using Plugin.BLE;
using Plugin.BLE.Abstractions.Contracts;
using Plugin.BLE.Abstractions.Exceptions;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;

using static RobotVoiceControlMobileApp.Constants;

namespace RobotVoiceControlMobileApp.Handlers
{
    class BLEHandler
    {
        public IAdapter adapter;
        public IBluetoothLE ble;

        public ObservableCollection<IDevice> devicesList = new ObservableCollection<IDevice>();

        ICharacteristic characteristic;

        public BLEHandler()
        {
            ble = CrossBluetoothLE.Current;
            adapter = CrossBluetoothLE.Current.Adapter;

            devicesList = new ObservableCollection<IDevice>();
        }

        public async Task Scan()
        {
            if (ble.IsOn)
            {
                devicesList.Clear();

                adapter.ScanTimeout = 5000;
                adapter.ScanMode = ScanMode.Balanced;

                adapter.DeviceDiscovered += (obj, a) =>
                {
                    if (!devicesList.Contains(a.Device))
                        devicesList.Add(a.Device);
                };

                await adapter.StartScanningForDevicesAsync();
            }
        }        

        public async Task<bool> Connect()
        {
            if (devicesList.Count == 0) return false;

            IDevice device = devicesList.Where(d => d.Name.Equals(BLEName)).FirstOrDefault();

            if (device != null)
            {
                await adapter.StopScanningForDevicesAsync();

                try
                {
                    await adapter.ConnectToDeviceAsync(device);

                    IService service = await device.GetServiceAsync(BLE_SERVICE);
                    characteristic = await service.GetCharacteristicAsync(BLE_CHARACTERISTIC);
                    characteristic.WriteType = Plugin.BLE.Abstractions.CharacteristicWriteType.WithoutResponse;

                    return true;
                }
                catch (DeviceConnectionException e)
                {
                    Debug.WriteLine(e.Message);
                    return false;
                }
            }
            else return false;
        }

        public async Task Disconnect()
        {
            foreach(var device in adapter.ConnectedDevices)
            {
                try
                {
                    await adapter.DisconnectDeviceAsync(device);
                }
                catch(Exception e)
                {
                    Debug.WriteLine(e.Message);
                }
            }
        }

        public bool IsConnected()
        {
            if (ble.IsOn && adapter.ConnectedDevices.Count > 0) return true;
            else return false;
        }

        public async Task<bool> Send(byte[] msg)
        {
            if (IsConnected() && characteristic.CanWrite)
            {
                return await characteristic.WriteAsync(msg);
            }
            else return false;
        }

        public async Task<bool> Send(byte command, byte[] msg)
        {
            List<byte> data = new List<byte>();
            data.Add(START);
            data.Add(command);
            data.AddRange(msg);
            data.Add(END);

            return await Send(data.ToArray());
        }

        public async Task<bool> Send(byte command)
        {
            return await Send(command, new byte[] { 0x00, 0x00 });
        }

        public async Task<byte[]> Recv()
        {
            if (IsConnected() && characteristic.CanRead)
            {
                return await characteristic.ReadAsync();
            }
            else
            {
                return null;
            }
        }
    }
}
