﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using FileExplorer.Model;
using System.Windows;


namespace FileExplorer.ViewModel
{
    public class DetailViewModel : DependencyObject
    {
        #region Properties
        #endregion

        #region Constructor

        public DetailViewModel()
        {
        }

        #endregion
    
        #region // Dependency Properties
        public static readonly DependencyProperty propertyChildren
            = DependencyProperty.Register("Children", typeof(IList<DirInfo>), typeof(DetailViewModel));

        public IList<DirInfo> Children
        {
            get { return (IList<DirInfo>)GetValue(propertyChildren); }
            set { SetValue(propertyChildren, value); }
        }

        //public static readonly DependencyProperty propertyIsExpanded = 
        //    DependencyProperty.Register("IsExpanded", typeof(bool), typeof(DirectoryViewModel));

        //public static readonly DependencyProperty propertyIsSelected =
        //    DependencyProperty.Register("IsSelected", typeof(bool), typeof(DirectoryViewModel));

        //public bool IsExpanded
        //{
        //    get { return (bool)GetValue(propertyIsExpanded); }
        //    set { SetValue(propertyIsExpanded, value); }
        //}

        //public bool IsSelected
        //{
        //    get { return (bool)GetValue(propertyIsSelected); }
        //    set { SetValue(propertyIsSelected, value); }
        //} 
        #endregion
    }
}
