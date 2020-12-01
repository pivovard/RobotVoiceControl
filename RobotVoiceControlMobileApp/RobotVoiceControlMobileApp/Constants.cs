using System;
using System.Collections.Generic;
using System.Text;

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

        public const byte SLOW  = 0x0D; //13
        public const byte FAST  = 0x17; //23
        public const byte FFAST = 0x66; //102

        public const string BLEName = "HMSoft";

        public static readonly Guid BLE_SERVICE = Guid.Parse("0000ffe0-0000-1000-8000-00805f9b34fb");
        public static readonly Guid BLE_CHARACTERISTIC = Guid.Parse("0000ffe1-0000-1000-8000-00805f9b34fb");

        public const string CognitiveServicesApiKey = "94ab57fb8b6444b38d2ce92857e4dbdb";
        public const string CognitiveServicesRegion = "westeurope";
    }
}
