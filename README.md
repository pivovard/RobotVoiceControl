# RobotVoiceControl

Control Robot vehicle (RobotTMR - ZCU) with mobile app.
Communication with robot is via HM-10 BLE module (HMSoft firmware required - [how to flash the firmware](https://circuitdigest.com/node/3688))

### RobotVoiceControlMobileApp
Mobile app for Android and iOS with speech recognition (iOS speech recognition is not tested). Build with Xamarin.Forms.\
Speech recognition requires an Azure Cognitive Services resource ([Create a Cognitive Services resource](https://docs.microsoft.com/cs-cz/azure/cognitive-services/cognitive-services-apis-create-account?tabs=multiservice%2Cwindows)). Set API key and region in [Constants.cs](RobotVoiceControlMobileApp/RobotVoiceControlMobileApp/Constants.cs).

### RobotVMR
Firmware for supervisor MSP430 MCU. Updated original firmware with BLE communication on UART0.
