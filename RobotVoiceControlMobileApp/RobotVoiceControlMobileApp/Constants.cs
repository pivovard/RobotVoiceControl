using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Text;
using Xamarin.Essentials;

namespace RobotVoiceControlMobileApp
{
    public class Constants
    {
        public const byte START = 0x0A;
        public const byte END   = 0x0E;

        //commands
        public const byte STOP      = 0x00;
        public const byte FORWARD   = 0x01;
        public const byte BACK      = 0x02;
        public const byte FOLLOW    = 0x03;

        public static byte SLOW  //13
        {
            get
            {
                return (byte)Preferences.Get("slow", 0x0D);
            }
}
        public static byte FAST  //23
        {
            get
            {
                return (byte) Preferences.Get("fast", 0x17);
            }
        }
        public static byte FFAST //102
        {
            get
            {
                return (byte)Preferences.Get("ffast", 0x66);
            }
        }
        
        public const string BLEName = "HMSoft";

        public static readonly Guid BLE_SERVICE = Guid.Parse("0000ffe0-0000-1000-8000-00805f9b34fb");
        public static readonly Guid BLE_CHARACTERISTIC = Guid.Parse("0000ffe1-0000-1000-8000-00805f9b34fb");

        //public static string CognitiveServicesApiKey = "94ab57fb8b6444b38d2ce92857e4dbdb";
        //public static string CognitiveServicesRegion = "westeurope";

        public static string CognitiveServicesApiKey
        {
            get
            {
                return Preferences.Get("apiId", "94ab57fb8b6444b38d2ce92857e4dbdb");
            }
        }
        public static string CognitiveServicesRegion
        {
            get
            {
                var t = Preferences.Get("region", "westeurope");
                return t;
            }
        }

        public static bool UsePhrase
        {
            get
            {
                return Preferences.Get("usePhrase", false);
            }
        }
        public static string Phrase
        {
            get
            {
                return Preferences.Get("phrase", "Hey Robot");
            }
        }

    }
}
