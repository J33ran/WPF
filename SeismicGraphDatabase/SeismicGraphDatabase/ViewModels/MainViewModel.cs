using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using SeismicGraphDatabase.Commands;
    

namespace SeismicGraphDatabase
{   
    class MainViewModel
    {
        
        RelayCommand<object> _openCommand; 
        public ICommand OpenCommand 
        { 
            get 
            { 
                if (_openCommand == null)
                { 
                    _openCommand = new RelayCommand<object>(this.Open); 
                }

                return _openCommand; 
            } 
        }

        void Open(object o)
        {
            int i = 0;
        }
 
    }
}
