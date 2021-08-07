using CarDriver.Forms.ViewModels;
using System.ComponentModel;
using Xamarin.Forms;

namespace CarDriver.Forms.Views
{
    public partial class ItemDetailPage : ContentPage
    {
        public ItemDetailPage()
        {
            InitializeComponent();
            BindingContext = new ItemDetailViewModel();
        }
    }
}