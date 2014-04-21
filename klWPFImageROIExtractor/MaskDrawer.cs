
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Shapes;
using System.Collections;

namespace MaskDrawingApp
{
    public enum Interpolation
    {
        None = 0,
        Quadratic = 1,
        Cubic = 2
    }

    public class MaskDrawer
    {
        public int downsampleRate;
        public Interpolation bezierType { get;  set; }
        public FillRule fillRule
        {
            get
            {
                return myGeometryGroup.FillRule;

            }
            set
            {
                myGeometryGroup.FillRule = value;
            }
        }
        
        private SolidColorBrush myPathFillColorBrush;
        
        protected PathFigure myPathFigure;
        protected PathGeometry myPathGeometry;
        protected GeometryGroup myGeometryGroup;
        public    Path myPath;

        private Canvas _canvas;
        
        public PointCollection RawPathPoints;
        public PointCollection DownsampledPathPoints;

        //initialization
        public MaskDrawer(Canvas canvas)
        {
            _canvas = canvas;

            RawPathPoints = new PointCollection();
            
            DownsampledPathPoints=new PointCollection();

            myPathFillColorBrush = new SolidColorBrush();
            myPathFillColorBrush.Color = Color.FromArgb(94, 94, 94, 0);

            //Create a new path figure, set its start point and create a path segment collection
            myPathFigure = new PathFigure();
            myPathFigure.Segments = new PathSegmentCollection();

            //Create path geometry and add path figure to it
            myPathGeometry = new PathGeometry();
            myPathGeometry.Figures.Add(myPathFigure);

            //Create path geometry group and add path geometry to it
            myGeometryGroup = new GeometryGroup();
            myGeometryGroup.Children.Add(myPathGeometry);

            //create a new path and assign the geometry group to its Data
            myPath = new Path();
            myPath.Stroke = Brushes.Yellow;
            myPath.StrokeDashArray=new DoubleCollection(new double[]{1,2});
            
            myPath.Fill = myPathFillColorBrush;
            myPath.StrokeThickness = 2;

            myPath.Data = myGeometryGroup;

            //Set default values
            bezierType = Interpolation.Cubic;
            fillRule = FillRule.EvenOdd;

            }
    
        //some initialization goes on. This function will be called in event handler OnLeftDown()
        public void StartDrawingFreehandPath(Point startpt)
        {
            
            //RawPathPoints.Add(startpt);
            myPathFigure.StartPoint = startpt;
            
            //add path to canvas
            _canvas.Children.Add(myPath);

        }

        //drawing freehand path segment by segment. This function will be called by OnMouseMove() and OnLeftUp()
        public void DrawFreehandPath(Point currpt)
        {
            //add current point
            RawPathPoints.Add(currpt);

            //draw straight line between current point and previous point
            PolyLineSegment s = new PolyLineSegment();
            s.Points = new PointCollection();
            int count = RawPathPoints.Count;
            if (count >= 2)
            {
                s.Points.Add(RawPathPoints[count - 2]);
                s.Points.Add(RawPathPoints[count - 1]);
            }

            //add current line section to segment collection
            myPathFigure.Segments.Add(s);

        }

        private void DoUniformDownsampling(PointCollection points)
        {
            int DownsampledPtsNumber = RawPathPoints.Count / downsampleRate;

            for (int i = 0; i < DownsampledPtsNumber; i++)
            {
                points.Add(RawPathPoints[i * downsampleRate]);

            }
        }

        private void DoAdaptiveDownsampling(PointCollection points)
        {

            points.Add(RawPathPoints[0]);

            DownsampledPathPoints = RawPathPoints.Clone(); //don't use assign!!!


            //check every three pixels A, B, C. If the angle between AB and BC is larger than a threshold (ex., 178 degree)
            //then we drop pixel B.
            for (int i = 0; i < DownsampledPathPoints.Count - 2; i++)
            {
                Point A = DownsampledPathPoints[i];
                Point B = DownsampledPathPoints[i + 1];
                Point C = DownsampledPathPoints[i + 2];

                double c = Math.Sqrt((A.X - B.X) * (A.X - B.X) + (A.Y - B.Y) * (A.Y - B.Y));
                double a = Math.Sqrt((B.X - C.X) * (B.X - C.X) + (B.Y - C.Y) * (B.Y - C.Y));
                double b = Math.Sqrt((A.X - C.X) * (A.X - C.X) + (A.Y - C.Y) * (A.Y - C.Y));

                double ang;
                if (a * c != 0)
                    ang = Math.Acos((a * a + c * c - b * b) / (2 * a * c));
                else
                {
                    ang = Math.PI;
                }

                double s_tri = (a + b + c) / 2.0;
                double area = Math.Sqrt(s_tri * (s_tri - a) * (s_tri - b) * (s_tri - c));


                //If the angle is small that means the curvature is large, we keep the point B
                if (ang < 175.0 / 180.0 * Math.PI && area > 10)
                {
                   points.Add(DownsampledPathPoints[i + 1]);
                }
                else
                {
                    DownsampledPathPoints.RemoveAt(i + 1);
                    if (i > 0) //roll back i
                        i--;
                }

            }            
            //add rest points not checked
            points.Add(RawPathPoints[RawPathPoints.Count - 2]);
            points.Add(RawPathPoints[RawPathPoints.Count - 1]);
         }


        public void DoneDrawingFreehandPath()
        {
            //always force closing by adding last point as the first one
            int ptcount = RawPathPoints.Count;
            if(ptcount>0)
            {
                DrawFreehandPath(RawPathPoints[0]);
            }

            PathFigure pf = new PathFigure();
            pf.StartPoint = myPathFigure.StartPoint;
            pf.Segments = new PathSegmentCollection();

            if (bezierType == Interpolation.None)
            {
                var s = new PolyLineSegment();
                s.Points = new PointCollection();
                //if (downsampleRate > 0 && downsampleRate != 9999)
                //     DoUniformDownsampling(s.Points);  
                //else 
                //     DoAdaptiveDownsampling(s.Points);

                //do this once for polyline
                s.Points.Add(RawPathPoints[0]);
                DownsampledPathPoints = s.Points;
                pf.Segments.Add(s);


            }
            
            if (bezierType == Interpolation.Quadratic)
            {
                PolyQuadraticBezierSegment s = new PolyQuadraticBezierSegment();
                s.Points = new PointCollection();

                if (downsampleRate > 0 && downsampleRate != 9999)
                    DoUniformDownsampling(s.Points);
                else
                    DoAdaptiveDownsampling(s.Points);

                //Do this twice for quadratic Bezier
                s.Points.Add(RawPathPoints[0]);
                s.Points.Add(RawPathPoints[0]);

                //assign s.Points to DownsampledPathPoints
                DownsampledPathPoints = s.Points;
                pf.Segments.Add(s);
            }

            if (bezierType == Interpolation.Cubic)
            {
                PolyBezierSegment s = new PolyBezierSegment();
                s.Points = new PointCollection();

                //if (downsampleRate > 0 && downsampleRate != 9999)
                //    DoUniformDownsampling(s.Points);
                //else
                //    DoAdaptiveDownsampling(s.Points);
                
                //do this two time to ensure closing for quadratic Bezier
                s.Points.Add(RawPathPoints[0]);
                s.Points.Add(RawPathPoints[0]);
                s.Points.Add(RawPathPoints[0]);

                //assign s.Points to DownsampledPathPoints
                DownsampledPathPoints = s.Points;
                pf.Segments.Add(s);
            }
            PathGeometry pg = new PathGeometry();
            pg.Figures.Add(pf);

            //Create path geometry group and add path geometry to it
            GeometryGroup gg = new GeometryGroup();
            gg.Children.Add(pg);

            //create a new path and assign the geometry group to its Data
            Path ExtraPath = new Path();

            if (bezierType == Interpolation.None)
                ExtraPath.Stroke = Brushes.Azure;

            if (bezierType == Interpolation.Quadratic)
                ExtraPath.Stroke = Brushes.Aquamarine;

            if (bezierType == Interpolation.Cubic)
                ExtraPath.Stroke = Brushes.Bisque;

            ExtraPath.StrokeThickness = 2;
            ExtraPath.Data = gg;
            
            _canvas.Children.Add(ExtraPath);

        }
    }
    
}