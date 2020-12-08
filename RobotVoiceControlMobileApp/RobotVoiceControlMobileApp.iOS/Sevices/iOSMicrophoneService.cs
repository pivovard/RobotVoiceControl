using AVFoundation;
using RobotVoiceControlMobileApp.iOS.Services;
using RobotVoiceControlMobileApp.Services;
using System.Threading.Tasks;
using Xamarin.Forms;

[assembly: Dependency(typeof(iOSMicrophoneService))]
namespace RobotVoiceControlMobileApp.iOS.Services
{
    public class iOSMicrophoneService : IMicrophoneService
    {
        TaskCompletionSource<bool> tcsPermissions;

        public Task<bool> GetPermissionAsync()
        {
            tcsPermissions = new TaskCompletionSource<bool>();
            RequestMicPermission();
            return tcsPermissions.Task;
        }

        public void OnRequestPermissionResult(bool isGranted)
        {
            tcsPermissions.TrySetResult(isGranted);
        }

        void RequestMicPermission()
        {
            var session = AVAudioSession.SharedInstance();
            session.RequestRecordPermission((granted) =>
            {
                tcsPermissions.TrySetResult(granted);
            });
        }
    }
}