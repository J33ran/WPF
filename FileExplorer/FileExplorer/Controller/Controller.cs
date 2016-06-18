using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections.ObjectModel;
using System.ComponentModel;
using FileExplorer.Model;
using System.IO;
using System.Collections.Specialized;
using FileExplorer.Properties;
using System.Windows.Input;
using FileExplorer.ViewModel;

namespace FileExplorer.Controller
{
    public class Controller
    {
        #region // Private Members
        private DirInfo _currentDirectory;
        //private DirectoryViewModel directoryViewModel;
        private IList<DirInfo> _currentItems; 
        #endregion

        #region Constructors

        public Controller()
        {
        
        } 
        #endregion

        #region // Public Properties
        /// <summary>
        /// Name of the current directory user is in
        /// </summary>
        public DirInfo CurrentDirectory
        {
            get { return _currentDirectory; }
            set
            {
                _currentDirectory = value;
                RefreshCurrentItems();
            }
        }

        /// <summary>
        /// Children of the current directory to show in the right pane
        /// </summary>
        public IList<DirInfo> CurrentItems
        {
            get
            {
                if (_currentItems == null)
                {
                   //_currentItems = new List<DirInfo>();
                    _currentItems = (from rd in DirectoryService.GetDrives()
                            select new DirInfo(rd)).ToList();
                }
                return _currentItems;
            }
            set
            {
                _currentItems = value;
            }
        } 
        #endregion


        #region Methods
        /// <summary>
        /// this method gets the children of current directory and stores them in the CurrentItems Observable collection
        /// </summary>
        protected void RefreshCurrentItems()
        {
            IList<DirInfo> childDirList = new List<DirInfo>();
            IList<DirInfo> childFileList = new List<DirInfo>();

            //If current directory is "My computer" then get the all logical drives in the system
            if (CurrentDirectory.Name.Equals(Resources.My_Computer_String))
            {
                childDirList = (from rd in DirectoryService.GetDrives()
                                select new DirInfo(rd)).ToList();
            }
            else
            {
                //Combine all the subdirectories and files of the current directory
                childDirList = (from dir in DirectoryService.GetDirectories(CurrentDirectory.Path)
                                select new DirInfo(dir)).ToList();

                childFileList = (from fobj in DirectoryService.GetFiles(CurrentDirectory.Path)
                                 select new DirInfo(fobj)).ToList();

                childDirList = childDirList.Concat(childFileList).ToList();
            }

            CurrentItems = childDirList;
        } 
        #endregion
    }
}
