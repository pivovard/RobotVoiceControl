using RobotVoiceControlMobileApp.Handlers;
using System;
using System.Diagnostics;
using Xamarin.Forms;

using static RobotVoiceControlMobileApp.Constants;

namespace RobotVoiceControlMobileApp
{
    public partial class MainPage : ContentPage
    {
        BLEHandler bleHandler;
        VRHandler vrHandler;

        bool _connected = false;

        public MainPage()
        {
            InitializeComponent();
            bleHandler = new BLEHandler();
            vrHandler = new VRHandler(bleHandler, lText, indicatorVR);

            if (!bleHandler.ble.IsOn)
            {
                DisplayAlert("", "Turn Bluetooth ON!", "Ok");
            }
        }

        private async void btnConnect_Clicked(object sender, EventArgs e)
        {
            if (!bleHandler.ble.IsOn)
            {
                await DisplayAlert("", "Turn Bluetooth ON!", "Ok");
                return;
            }

            if (!bleHandler.IsConnected())
            {
                bool res = false;
                indicatorBLE.IsRunning = true;

                try
                {
                    await bleHandler.Scan();
                    res = await bleHandler.Connect();
                }
                catch(Exception ex)
                {
                    Debug.WriteLine(ex.Message);
                }
                finally
                {
                    indicatorBLE.IsRunning = false;
                }

                if (res)
                {
                    _connected = true;
                    btnConnect.Source = "connected.png";
                    await DisplayAlert("", $"Connected to {bleHandler.adapter.ConnectedDevices[0].Name}", "Ok");
                }
                else await DisplayAlert("Error", "Failed to connect to the robot!", "Ok");
            }
            else
            {
                await bleHandler.Disconnect();
                btnConnect.Source = "disconnected.png";
                await DisplayAlert("", "Disconnected.", "Ok");
                _connected = false;
            }
        }

        private async void btnEN_Clicked(object sender, EventArgs e)
        {
            if (!await vrHandler.IsMicEnabled()) await DisplayAlert("Error", "Please grant permission to the microphone!", "Ok");

            if (indicatorVR.IsRunning)
            {
                vrHandler.StopRecognition();
            }
            else
            {
                vrHandler.StartRecognition();
            }
        }

        private void btnCZ_Clicked(object sender, EventArgs e)
        {
            DisplayAlert("", "Not implemented!", "Ok");
        }

        private async void btnLeft_Clicked(object sender, EventArgs e)
        {
            if (!await bleHandler.Send(FORWARD, new byte[2] { FAST, SLOW }))
            {
                await DisplayAlert("Error", "Failed to send the command", "Ok");
            }
        }

        private async void btnUp_Clicked(object sender, EventArgs e)
        {
            if (!await bleHandler.Send(FORWARD, new byte[2] { FFAST, FFAST }))
            {
                await DisplayAlert("Error", "Failed to send the command", "Ok");
            }
        }

        private async void btnRight_Clicked(object sender, EventArgs e)
        {
            if (!await bleHandler.Send(FORWARD, new byte[2] { SLOW, FAST }))
            {
                await DisplayAlert("Error", "Failed to send the command", "Ok");
            }
        }

        private async void btnDown_Clicked(object sender, EventArgs e)
        {
            if (!await bleHandler.Send(BACK, new byte[2] { FAST, FAST }))
            {
                await DisplayAlert("Error", "Failed to send the command", "Ok");
            }
        }

        private async void btnStop_Clicked(object sender, EventArgs e)
        {
            if (!await bleHandler.Send(STOP))
            {
                await DisplayAlert("Error", "Failed to send the command", "Ok");
            }
        }

        private async void btnFollow_Clicked(object sender, EventArgs e)
        {
            if (!await bleHandler.Send(FOLLOW))
            {
                await DisplayAlert("Error", "Failed to send the command", "Ok");
            }
        }
    }
}
