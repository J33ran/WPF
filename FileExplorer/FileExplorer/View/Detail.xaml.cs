using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using FileExplorer.Model;

namespace FileExplorer.View
{
    /// <summary>
    /// Interaction logic for Detail.xaml
    /// </summary>
    public partial class Detail : UserControl
    {
        public Detail()
        {
            InitializeComponent();
        }

        protected void HandleDoubleClick(object sender, MouseButtonEventArgs e)
        {
            var dir = ((ListViewItem)sender).Content as DirInfo;
       
            Controller.Controller controller = DataContext as Controller.Controller;
            controller.SelectedDirectory = dir;
        }
    }
}
