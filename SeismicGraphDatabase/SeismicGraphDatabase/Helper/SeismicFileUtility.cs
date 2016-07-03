using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text;
using LMKR.Entities.Seismic;

namespace SeismicGraphDatabase.Helper
{
    /// <summary>
    ///  A utility class for determining filetypes from extensions
    /// </summary>
    public static class SeismicFileUtility
    {
        /// <summary>
        /// Segy format
        /// </summary>
        public const string Segy = "segy";
        /// <summary>
        /// Sgy format
        /// </summary>
        public const string Sgy = "sgy";
        /// <summary>
        /// 3dx format
        /// </summary>
        public const string Brick3DX = "3dx";
        /// <summary>
        /// 3dh format
        /// </summary>
        public const string Brick3DH = "3dh";
        /// <summary>
        /// 3ds format
        /// </summary>
        public const string Brick3DS = "3ds";

        /// <summary>
        /// Returns File type by looking at the file extension
        /// </summary>
        /// <param name="fileExtension"></param>
        /// <returns></returns>
        public static SeismicFileFormat GetSeismicFileFormat(string fileExtension)
        {
            if (String.IsNullOrWhiteSpace(fileExtension))
                throw new ArgumentNullException("fileExtension");

            switch (fileExtension.ToLower(CultureInfo.CurrentCulture))
            {
                case Segy:
                case Sgy:
                    return SeismicFileFormat.Segy;
                case Brick3DS:
                case Brick3DH:
                case Brick3DX:
                    return SeismicFileFormat.GGXBrick;
                default:
                    return SeismicFileFormat.Unknown;
            }

        }

        /// <summary>
        /// Checks whether the file is brick file.
        /// </summary>
        /// <param name="path"></param>
        /// <returns></returns>
        public static bool IsBrickFile(string path)
        {
            var fileType = GetSeismicFileFormat((new FileInfo(path).Extension).Substring(1));
            return fileType == SeismicFileFormat.GGXBrick;
        }

    }
}
