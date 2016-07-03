using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using SeismicGraphDatabase.Commands;
using Microsoft.Win32;
using LMKR.Windows.Controls.Dialogs.Seismic;
using LMKR.Windows.GeoGraphix;
using LMKR.Entities.Seismic;



using System.IO;
//using log4net;
    

namespace SeismicGraphDatabase
{   
    class MainViewModel : ISeisVisionLicenseHandler
    {
        public void CheckInSeisVisionLicense()
        { }

        public void CheckoutSeisVisionLicense()
        { }
        //private static readonly ILog log = LogManager.GetLogger(typeof(MainViewModel));

        //private SeismicTraceReaderBase _selectedVolume;
        //private SeismicExtents _activeSurveyExtents;
        //private Segy3DTraceHeaderMapping _segyHeaderOffsets;
        private RelayCommand<object> _browseFilesCommand;
        private RelayCommand<object> _loadSeismicCommand; 


        public MainViewModel()
        {
            //var dlg = new SeismicInputDialog(256, 1, this);           
            
        }

        public ICommand LoadSeismicCommand
        {
            get
            {
                if (_loadSeismicCommand == null)
                {
                    _loadSeismicCommand = new RelayCommand<object>(this.LoadSeismic);
                }

                return _loadSeismicCommand;
            }
        }

        public ICommand BrowseFilesCommand 
        { 
            get 
            { 
                if (_browseFilesCommand == null)
                { 
                    _browseFilesCommand = new RelayCommand<object>(this.BrowseFiles); 
                }

                return _browseFilesCommand; 
            } 
        }


        void BrowseFiles(Object o)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = @"Seismic files (*.segy, *.sgy, *.3dx, *.3ds)|*.sgy;*.segy;*.3dx;*.3ds";

            if (openFileDialog.ShowDialog() == true)
            {
                var InputFilePath = openFileDialog.FileName;
            }
        
        }
        void LoadSeismic(object o)
        {
            var dlg = new SeismicInputDialog(0, 0, this);
            //{
            //        PreviousSelectedVolume = null,
            //        PreviousSelectedTimesliceVolume = null,
            //        ApplicationName = "Test"
            //};
            
            
            if (dlg.ShowDialog() == true)
            {
                SeismicTraceReaderBase reaader = dlg.SelectedVolume;
            };
            
        }

        //private void LoadSeismic(string _inputFilePath)
        //{
        //    //try
        //    //{
        //    //    SeismicFileBase file = null;
        //    //    if (SeismicFileUtility.IsBrickFile(_inputFilePath) == true)
        //    //    {
        //    //        var brickFile = new GGXBrickFile(File.OpenRead(_inputFilePath));
        //    //        brickFile.ReadMetadata().FilePath = _inputFilePath;
                    
        //    //        file = brickFile;
        //    //    }
        //    //    else
        //    //    {
        //    //        file = new SegyFile(File.OpenRead(_inputFilePath));
        //    //    }

        //    //    //file.ReadFileHeader();
        //    //    //_segyHeaderOffsets = file.TraceHeaderOffsets.Clone();

        //    //    //var metaData = file.ScanTraceHeaders();
        //    //    //metaData.GridInfo.CreateSeismicSurvey();

        //    //    //metaData.FilePath = _inputFilePath;
        //    //    var _selectedVolume = file.OpenTraceReader();

        //    //    //_activeSurveyExtents = _selectedVolume.WorkingGridInfo.Extents;

        //    //    var crossLines = _selectedVolume.WorkingGridInfo.Extents.NumberCrossline;
        //    //    var inLines = _selectedVolume.WorkingGridInfo.Extents.NumberInline;

        //    //    for (int i = 0; i < inLines; i++)
        //    //    {
        //    //        for (int j = 0; j < crossLines; j++)
        //    //        {
        //    //            var data = _selectedVolume.ReadTrace(i, j);
        //    //            //log.Info(data.ToString());
                        
        //    //        }
        //    //    }

        //    //    return true;
        //    //}
        //    //catch (InvalidDataException)
        //    //{
        //    //    return false;
        //    //}
        //}
    }
}
