using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Xamarin.Essentials;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace RobotVoiceControlMobileApp
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class SettingsPage : ContentPage
    {
        public SettingsPage()
        {
            InitializeComponent();
        }

        private void EntryString_Completed(object sender, EventArgs e)
        {
            Entry entry = sender as Entry;
            Preferences.Set(entry.AutomationId, entry.Text);
        }

        private void EntryByte_Completed(object sender, EventArgs e)
        {
            Entry entry = sender as Entry;

            byte val;
            if(byte.TryParse(entry.Text, out val))
            {
                Preferences.Set(entry.AutomationId, val);
            }
            else
            {
                DisplayAlert("Error", "Value is not valid! Value must be byte.", "Ok");
            }
        }

        private void CheckBox_CheckedChanged(object sender, CheckedChangedEventArgs e)
        {
            CheckBox ch = sender as CheckBox;
            Preferences.Set("usePhrase", ch.IsChecked);
        }

        private void Reset_Clicked(object sender, EventArgs e)
        {
            Preferences.Clear();
            Navigation.PopAsync();
        }
    }
}