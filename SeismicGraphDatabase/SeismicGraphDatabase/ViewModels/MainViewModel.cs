using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using SeismicGraphDatabase.Commands;
using Microsoft.Win32;
using LMKR.IO.Seismic;
using LMKR.Entities.Seismic;
using LMKR.Internal.IO.GeoGraphix.Seismic;
using LMKR.Entities.GeoGraphix.Seismic;

using System.IO;
using log4net;
    

namespace SeismicGraphDatabase
{   
    class MainViewModel
    {
        //private static readonly ILog log = LogManager.GetLogger(typeof(MainViewModel));

        //private SeismicTraceReaderBase _selectedVolume;
        //private SeismicExtents _activeSurveyExtents;
        //private Segy3DTraceHeaderMapping _segyHeaderOffsets;
        private RelayCommand<object> _openCommand; 

        public MainViewModel()
        {
            //_activeSurveyExtents = new SeismicExtents();
            //_segyHeaderOffsets = new Segy3DTraceHeaderMapping();
            //_segyHeaderOffsets.Line.StartByte = 9;
            //_segyHeaderOffsets.Crossline.StartByte = 13;
            //_segyHeaderOffsets.X.StartByte = 73;
            //_segyHeaderOffsets.Y.StartByte = 77;
            //_segyHeaderOffsets.SamplingInterval.StartByte = 117;
            //_segyHeaderOffsets.SamplesCount.StartByte = 115;
            //_segyHeaderOffsets.Line.ByteSize = 4;
            //_segyHeaderOffsets.Crossline.ByteSize = 4;
        }
        
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
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = @"Seismic files (*.segy, *.sgy, *.3dx, *.3ds)|*.sgy;*.segy;*.3dx;*.3ds";

            if (openFileDialog.ShowDialog() == true)
            {
                LoadSeismic(openFileDialog.FileName);
            }
            
        }

        private bool LoadSeismic(string _inputFilePath)
        {
            try
            {
                SeismicFileBase file = null;
                if (SeismicFileUtility.IsBrickFile(_inputFilePath) == true)
                {
                    var brickFile = new GGXBrickFile(File.OpenRead(_inputFilePath));
                    brickFile.ReadMetadata().FilePath = _inputFilePath;
                    
                    file = brickFile;
                }
                else
                {
                    file = new SegyFile(File.OpenRead(_inputFilePath));
                }

                //file.ReadFileHeader();
                //_segyHeaderOffsets = file.TraceHeaderOffsets.Clone();

                //var metaData = file.ScanTraceHeaders();
                //metaData.GridInfo.CreateSeismicSurvey();

                //metaData.FilePath = _inputFilePath;
                var _selectedVolume = file.OpenTraceReader();

                //_activeSurveyExtents = _selectedVolume.WorkingGridInfo.Extents;

                var crossLines = _selectedVolume.WorkingGridInfo.Extents.NumberCrossline;
                var inLines = _selectedVolume.WorkingGridInfo.Extents.NumberInline;

                for (int i = 0; i < inLines; i++)
                {
                    for (int j = 0; j < crossLines; j++)
                    {
                        var data = _selectedVolume.ReadTrace(i, j);
                        //log.Info(data.ToString());
                        
                    }
                }

                return true;
            }
            catch (InvalidDataException)
            {
                return false;
            }
        }
    }
}
