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
    class Relation
    {
        public string name { get; set; }
        public double? inline { get; set; }
        public double? crossline { get; set; }
        //public float? timeline { get; set; }
    };


    class Brick
    {
        // Copy constructor.
        public Brick(Brick rhs)
        {
            inline = rhs.inline;
            crossline = rhs.crossline;
            timeline = rhs.timeline;

            samples = rhs.samples;

            // Specfically done for time relation
            parentCrossline = rhs.parentCrossline;
            parentInline = rhs.parentInline;
            parentTime = rhs.parentTime;

        }

        public Brick()
        {
        }

        public double inline { get; set; }
        public double crossline { get; set; }
        public float timeline { get; set; }
        public float[] samples { get; set; }
        public double? parentInline { get; set; }
        public double? parentCrossline { get; set; }
        public float? parentTime { get; set; }
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


        //private RelayCommand<object> _browseFilesCommand;
        private RelayCommand<object> _loadSeismicCommand;

        private static void CreateRelation(Brick brick, Relation relation)
        {
//            var cypher =
//                        String.Format(@"MATCH (a:Brick),(b:Brick)
//                        WHERE   a.inline = {0} AND b.inline = {1}                
//                        AND     a.crossline = {2} AND b.crossline = {3}
//                        AND     a.timeline = {4} AND b.timeline = {5}
//                            CREATE (a)-[:{6}]->(b)"
//                        , new object[] 
//                        { 
//                              relation.inline
//                            , brick.inline
//                            , relation.crossline
//                            , brick.crossline
//                            , relation.timeline
//                            , brick.timeline
//                            , relation.name
//                        }
//                    );

            var cypher =
                        String.Format(@"MATCH (a:Brick),(b:Brick)
                        WHERE   a.inline = {0} AND b.inline = {1}                
                        AND     a.crossline = {2} AND b.crossline = {3}
                            CREATE (a)-[:{4}]->(b)"
                        , new object[] 
                        { 
                              relation.inline
                            , brick.inline
                            , relation.crossline
                            , brick.crossline
                            , relation.name
                        }
                    );

            session.Run(cypher);
        }
       
        public static void CreateBrick(Object o)
        {
            var brick = (Brick)o;
            Relation timeLineRelation = new Relation
            {
                name = @"Timeline"
                ,
                inline = brick.inline
                ,
                crossline = brick.crossline
                //,
                //timeline = null
            };

            Relation crossLineRelation = new Relation
            {
                name = @"Crossline"
                ,
                inline = brick.inline
                ,
                crossline = brick.parentCrossline
                //,
                //timeline = null
            };

            Relation inLineRelation = new Relation
            {
                name = @"Inline"
                ,
                inline = brick.parentInline
                ,
                crossline = brick.crossline
                //,
                //timeline = null
            };

            //for (int i = 0; i < brick.samples.Length; i++)
            {
                //brick.timeline = i;

                // Create a single brick
                var createProps =
                     new Dictionary<string, object> { 
                                                { "inline", brick.inline }
                                                , {"crossline", brick.crossline}
                                                //, {"timeline", brick.timeline}
                                                , {"samples", brick.samples}
                                                //, {"sample", brick.samples[i]}
                     };

//                var result = session.Run(@"CREATE (brick:Brick {
//                                            inline: {inline}
//                                            , crossline: {crossline}
//                                            , timeline: {timeline}
//                                            , samples: {samples} 
//                                            })",
//                                            createProps);

                var result = session.Run(@"CREATE (brick:Brick {
                                            inline: {inline}
                                            , crossline: {crossline}
                                            , samples: {samples} 
                                            })",
                            createProps);

                ////Timeline Relation
                //if (timeLineRelation.timeline != null) 
                //{
                //   CreateRelation(brick, timeLineRelation);
                //}
                
                //timeLineRelation.timeline = brick.timeline;

                // Crossline Relation
                if (crossLineRelation.crossline != null)
                {
                    //crossLineRelation.timeline = brick.timeline;
                    CreateRelation(brick, crossLineRelation);

                }

                // Inline Relation
                if (inLineRelation.inline != null)
                {
                    //inLineRelation.timeline = brick.timeline;
                    CreateRelation(brick, inLineRelation);
                }
              
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
                        var data = reader.ReadResampledTrace((int)inline, (int)crossline, 100);
                            //(float)samplingInterval);
                        Brick n = new Brick
                        {
                            inline = inline
                            , crossline = crossline
                            , samples = data
                            , parentInline = parentInline
                            , parentCrossline = parentCrossline
                            , parentTime  = null
                        };

                        CreateBrick(n);
                        parentCrossline = crossline;

                    }

                    parentInline = inline;
                }
            }
        }
    }
}
