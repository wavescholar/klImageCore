using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using Microsoft.Win32;
using Path=System.Windows.Shapes.Path;
using System.Text;
using System.Windows.Controls;
using System.Collections;


namespace MaskDrawingApp
{
    public partial class Window1
    {
        private BitmapImage bitmapsrc;
        private Interpolation  beziertype;
        private FillRule fillrule;
        private int downsampleRate;
        private MaskDrawer maskDrawer;

        string imageFileName;

        string afRemovedImageFile;
        string baseName;

        string marker1;
        string marker2; 

        public ArrayList AllPoints;

        private double afThresh;//Range is 0,2 step 1/10


        public Window1()
        {
            InitializeComponent();

            AllPoints = new ArrayList();

            MyCanvas.MouseLeftButtonDown += OnLeftDown;
            MyCanvas.MouseLeftButtonUp += OnLeftUp;
            MyCanvas.MouseMove += OnMouseMove;

            afThresh = 1;
            
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            string [] args = Environment.GetCommandLineArgs();
            if (args.Length > 1)
            {
                imageFileName = args[1];
                //marker1 = args[2];
                //marker2 = args[3];
                //baseName = args[4];
            }
            else
            {
                imageFileName = Directory.GetCurrentDirectory() +"\\fused.jpg";
                marker1 = "AF_488.tif";
                marker2 = "AF_555.tif";
                baseName = "AF";
            }

            //LoadImage(imageFileName);
           
        }

        private void OnMouseMove(object sender, MouseEventArgs e)
        {
            if (MyCanvas.IsMouseCaptured)
            {
                Point currentPoint = e.GetPosition(MyCanvas);
                maskDrawer.DrawFreehandPath(currentPoint);
            }
        }

        private void OnLeftUp(object sender, MouseButtonEventArgs e)
        {

            if (MyCanvas.IsMouseCaptured)
            {
                maskDrawer.DoneDrawingFreehandPath();

                AllPoints.Add(maskDrawer.RawPathPoints);

                MyCanvas.ReleaseMouseCapture();
            }
        }

        private void OnLeftDown(object sender, MouseButtonEventArgs e)
        {
            if (!MyCanvas.IsMouseCaptured)
            {
                Point startPoint = e.GetPosition(MyCanvas);

                maskDrawer=new MaskDrawer(MyCanvas);
                maskDrawer.bezierType = beziertype;
                maskDrawer.fillRule = fillrule;
                maskDrawer.downsampleRate = downsampleRate;
                maskDrawer.StartDrawingFreehandPath(startPoint);

                MyCanvas.CaptureMouse();
            }
        }

        private void LoadImage(string imageName)
        {
            bitmapsrc = new BitmapImage();
            bitmapsrc.BeginInit();
            bitmapsrc.UriSource = new Uri(imageName);
            bitmapsrc.EndInit();
            image1.Source = bitmapsrc; 
            imageFileName = imageName;
        }

        private void Browse_Click(object sender, RoutedEventArgs e)
        {
            var dlg = new OpenFileDialog
            {
                InitialDirectory = "",
                Filter = "Image files (*.jpg)|*.jpg|All Files (*.*)|*.*",
                RestoreDirectory = true
            };

            if (dlg.ShowDialog() != true) return; 
            var selectedFileName = dlg.FileName;

            LoadImage(selectedFileName);
             
        }

        private void ClearAllAnnotations_Click(object sender, RoutedEventArgs e)
        {
            List<UIElement> elementsToRemove = new List<UIElement>();
            foreach (var child in MyCanvas.Children)
            {
                if (child is Path)
                {
                    elementsToRemove.Add(child as UIElement);
                }
            }

            foreach (var element in elementsToRemove)
            {
                MyCanvas.Children.Remove(element);
            }

            AllPoints.Clear();

        }

        private void Bezier_None(object sender, RoutedEventArgs e)
        {
            beziertype = Interpolation.None;
        }

        private void Bezier_Quadratic(object sender, RoutedEventArgs e)
        {
            beziertype = Interpolation.Quadratic;
        }

        private void Bezier_Cubic(object sender, RoutedEventArgs e)
        {
            beziertype = Interpolation.Cubic;
        }

        private void Even_Odd(object sender, RoutedEventArgs e)
        {
            fillrule = FillRule.EvenOdd;
        }

        private void None_Zero(object sender, RoutedEventArgs e)
        {
            fillrule = FillRule.Nonzero;
        }

        private void DownsampleOne(object sender, RoutedEventArgs e)
        {
            downsampleRate = 1;
        }

        private void DownsampleThree(object sender, RoutedEventArgs e)
        {
            downsampleRate = 3;
        }

        private void DownsampleFive(object sender, RoutedEventArgs e)
        {
            downsampleRate = 5;
        }

        private void DownsampleAdaptive(object sender, RoutedEventArgs e)
        {
            downsampleRate = 9999; //this number will just be used as a flag for adaptive downsampling
        }

        private void ZoomControl_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            //Clear Annotations forst
            List<UIElement> elementsToRemove = new List<UIElement>();
            foreach (var child in MyCanvas.Children)
            {
                if (child is Path)
                {
                    elementsToRemove.Add(child as UIElement);
                }
            }

            foreach (var element in elementsToRemove)
            {
                MyCanvas.Children.Remove(element);
            }

            double newVal = 1/ e.NewValue;
            image1.LayoutTransform = new ScaleTransform(newVal, newVal);
            //MyCanvas.LayoutTransform = new ScaleTransform(newVal, newVal);
            if(AllPoints !=null)
                AllPoints.Clear();
            
        }

        private void SaveROI_Click(object sender, RoutedEventArgs e)
        {
            StringBuilder sb = new StringBuilder(imageFileName);
            sb.Replace(".jpg","");
            sb.Replace(".tif", "");
            sb.Replace("Fused_HighRes", "");

 
            //We have to scale the points by the value of the ZoomControl
            Transform transform = image1.LayoutTransform;
            Matrix mat = transform.Value;
            double scaleX = mat.M11;
            double scaleY = mat.M22;
            if (scaleX != scaleY)
            {
                throw new Exception("scaleX is not equal to scale y in save spline method");
            }
            
            ArrayList blobs = AllPoints;
            for (int blobCount = 0; blobCount < blobs.Count; blobCount++)
            {
                StreamWriter sw = new StreamWriter(sb.ToString() + "-ROIMaskPoints.csv");
                PointCollection blobPoints = (PointCollection)blobs[blobCount];          
                for (int i = 0; i < blobPoints.Count; i++)
                {
                    Point pt = new Point(blobPoints[i].X * (1 / scaleX), blobPoints[i].Y * (1 / scaleY));
                    sw.WriteLine(pt);
                }
                sw.Close();
            }

        }

        private void slider1_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            double newVal = e.NewValue;
            afThresh = newVal;
        }

        private void RemoveAF_Click(object sender, RoutedEventArgs e)
        {
            System.Diagnostics.Process proc = new System.Diagnostics.Process();
            proc.StartInfo.FileName = "DriveAFMask.exe";
            proc.StartInfo.UseShellExecute = false;
            proc.StartInfo.Arguments =   marker1 + " " + marker2 + " " +"fused.jpg" + " " + Convert.ToString(afThresh) + " " + baseName;
            proc.Start();
            proc.WaitForExit();

            afRemovedImageFile = Directory.GetCurrentDirectory() + "\\AF_Removed.jpg";
            LoadImage(afRemovedImageFile);
        }
    }
}
