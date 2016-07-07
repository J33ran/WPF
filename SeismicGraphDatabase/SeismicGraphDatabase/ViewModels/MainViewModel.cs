using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using SeismicGraphDatabase.Commands;
using Microsoft.Win32;

using LMKR.Geometry;
using LMKR.Windows.Controls.Dialogs.Seismic;
using LMKR.Windows.GeoGraphix;
using LMKR.Entities.Seismic;



using System.IO;
using log4net;
using Neo4j.Driver.V1;


    

namespace SeismicGraphDatabase
{
    struct node
    {
        public double inline { get; set; }
        public double crossline { get; set; }
        public float[] time { get; set; }
        public double? parentInline { get; set; }
        public double? parentCrossline { get; set; }
    };

    class MainViewModel : ISeisVisionLicenseHandler
    {
        public void CheckInSeisVisionLicense()
        { }

        public void CheckoutSeisVisionLicense()
        { }

        private static readonly ILog log;
        private static readonly IDriver driver;
        private static readonly ISession session;


        private RelayCommand<object> _browseFilesCommand;
        private RelayCommand<object> _loadSeismicCommand;

        private static void CreateRelation(string relname, double? parent, double child)
        {
            if (parent != null)
            {
                var relation = String.Format(@"MATCH (a:Brick),(b:Brick)
                                WHERE a.{0} = '{1}' AND b.{0} = '{2}'               
                                CREATE (a)-[r:RELTYPE {{ {0} : a.{1} + '<->' + b.{2} }}]->(b)"
                                    , new object[] {relname, parent, child}
                                );

                session.Run(relation);
            }
        }

        public static void CeateNode(Object o)
        {
            var n = (node)o;

            double? parentTime = null;
            
            for (int i = 0; i < n.time.Length; i++)
            {
                var createProps =
                     new Dictionary<string, object> { 
                                                { "inline", n.inline }
                                                , {"crossline", n.crossline}
                                                , {"time", n.time[i]}
                     };

                var result = session.Run(@"CREATE (brick:Brick {
                                            inline: {inline}
                                            , crossline: {crossline}
                                            , time: {time} 
                                            })",
                                            createProps);

                //Time Relation
                CreateRelation("time", parentTime, i);
                parentTime = i;
            }

        }


        static MainViewModel()
        {
            log = LogManager.GetLogger(typeof(MainViewModel));
            
            driver = GraphDatabase.Driver("bolt://localhost", AuthTokens.Basic("neo4j", "123098"));
            session = driver.Session();

        }
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


        void LoadSeismic(object o)
        {
            var dlg = new SeismicInputDialog(0, 0, this);
            //{
            //        PreviousSelectedVolume = null,
            //        PreviousSelectedTimesliceVolume = null,%
            //        ApplicationName = "Test"
            //};

            var show = dlg.ShowDialog();

            if (show == true)
            {
                SeismicTraceReaderBase reader = dlg.SelectedVolume;
                var gridInfo = reader.WorkingGridInfo;

                var dimension1 = gridInfo.Extents.InlineAxis.End;
                var dimension2 = gridInfo.Extents.CrosslineAxis.End;



                var samplingInterval = gridInfo.Extents.TimeAxis.Increment;
                double? parentInline = null;
                double? parentCrossline = null;


                for (var inline = gridInfo.Extents.InlineAxis.Start; inline < dimension1; inline++)
                {
                    for (var crossline = gridInfo.Extents.CrosslineAxis.Start; crossline < dimension2; crossline++)
                    {
                        var data = reader.ReadResampledTrace((int)inline, (int)crossline, (float)samplingInterval);
                        node n = new node
                        {
                            inline = inline
                            , crossline = crossline
                            , time = data
                            , parentInline = parentInline
                            , parentCrossline = parentCrossline
                        };

                        CeateNode(n);

                        CreateRelation(@"Crossline", parentCrossline, crossline);
                        parentCrossline = crossline;

                    }

                    CreateRelation(@"Inline", parentInline, inline);
                    parentInline = inline;

                }
            }
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
