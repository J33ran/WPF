using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;


namespace FileExplorer.ViewModel
{
    class DirectoryViewModel : ViewModelBase
    {
        public IList<DirInfo> Directories
        {
            get
            {
                return ((App)Application.Current).Controller.CurrentItems;
            }
        }  
        
        public DirectoryViewModel()
        {
        }
    }
}
