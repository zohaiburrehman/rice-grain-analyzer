#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    Mat src,src_gray,threshold_output;
    int thresh = 100;
    int max_thresh = 255;

  /// Task # 1
  /// Read image
  src = imread( "scan006.jpg" );

  /// Convert image to gray and blur it
   cvtColor( src, src_gray, CV_BGR2GRAY );
   imwrite("b_and_w.jpg", src_gray);

   blur( src_gray, src_gray, Size(6,6) );
   imwrite("blur.jpg", src_gray);

   threshold( src_gray, threshold_output, thresh, max_thresh, THRESH_BINARY );
   imwrite("threshold_output.jpg", threshold_output);

  /// //////////////////////////////////////////////////////////////////////////////////////////////////////////

  /// Task Separate each rice
  /// Detect Each rice
  /// Find contours

  vector<vector<Point> > contours;

  findContours( threshold_output, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

  /// used to crop rice from main image
  vector<Rect> boundRect( contours.size() );

  /// used to find minimum rectangle aroun area for length
  vector<RotatedRect> minRect( contours.size() );

  /// for green color
  Scalar c=Scalar(0,255,0);
  for( int i = 0; i < contours.size(); i++ )
     {

         minRect[i] = minAreaRect( Mat(contours[i]) );
         boundRect[i] = boundingRect( Mat(contours[i]) );
         drawContours( src, contours, i, c, 2, 8, vector<Vec4i>(), 0, Point() );
     }
    imwrite("contours.jpg",src);


  /// //////////////////////////////////////////////////////////////////////////////////////////////////////////


   int num=0;
   string str=" ";
   double temp=0;
   double len=0;
   double wid=0;
   double area=0;

  for( int i = 0; i< contours.size(); i++ )
     {
       area=contourArea(contours[i]);

       if(area>1000){
        num++;

        Point2f rect_points[4];
        minRect[i].points( rect_points );

       for( int j = 0; j < 2; j++ )
       {

       double dist =norm(Mat(rect_points[j]),Mat(rect_points[(j+1)]) );
                if(j==0)
                    len=dist;
                else
                    wid=dist;
       }
        if(wid>len)
        {
            temp=len;
            len=wid;
            wid=temp;
        }
       cout<<"\n===============Rice # "<<num<<"===============\n\n";
       cout<<"Length : "<<len<<endl;
       cout<<"Width  : "<<wid<<endl;
       cout<<"Area   : "<<area<<endl<<endl;

       stringstream ss ;
       ss << num;
       str = ss.str();

       Mat Roi = src(boundRect[i]);
       imwrite("rice_"+str+".jpg",Roi);

       }
     }
       cout<<"Total number of rice = "<<num;

  /// Show in a window
  namedWindow( "Contours", CV_WINDOW_NORMAL );
  resizeWindow("Contours",600,600);
  imshow( "Contours", src );


  waitKey(0);
  return(0);
}


