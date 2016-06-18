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

namespace FileExplorer.View
{
    /// <summary>
    /// Interaction logic for Directory.xaml
    /// </summary>
    public partial class Directory : UserControl
    {
        public Directory()
        {
            InitializeComponent();
        }

        private void SelectionChanged(object sender, RoutedPropertyChangedEventArgs<Object> e)
        {
        }

        private void TreeView_Expanded(object sender, RoutedEventArgs e)
        {
        }
    }
}
