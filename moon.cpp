#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>

#define RADIUS 150
#define CENTER_Y (IMAGE_HEIGHT/2)
#define CENTER_X (IMAGE_WIDTH/2)
#define IMAGE_HEIGHT 400
#define IMAGE_WIDTH 400
#define WINDOW_NAME "SHADER (ARTIFICIAL MOON) USING C++"

using namespace std;
using namespace cv;

typedef struct
{
  double cos_alpha;
  double cos_beta;
  double cos_gamma;
}cosines;

typedef struct
{
  double x;
  double y;
  double z;
}vect;

Mat image(IMAGE_WIDTH, IMAGE_HEIGHT, CV_8UC1, Scalar(0));
int slider;
const int SLIDER_MAX = 180;
cosines shader[IMAGE_HEIGHT][IMAGE_WIDTH] = { 0 };

double cosz(double, double);
int assign_shader_vector(cosines(*)[IMAGE_WIDTH]);

void slider_function(int, void*);


int main()
{
  //int i, j;
  namedWindow(WINDOW_NAME);
  //imshow(WINDOW_NAME, image);
  //radius of the circle is 150 pixels
  //Creating the Look up array for the 3D shade effect
  assign_shader_vector(shader);
  slider = 0;
  
  createTrackbar("ADJUST", WINDOW_NAME, &slider, SLIDER_MAX, slider_function);
  while (char(waitKey(1)) != 'q');
  return 0;
}

double cosz(double cosx, double cosy)
{
  double temp = 0;
  temp = 1.00 - cosx*cosx - cosy*cosy;
  temp = sqrt(temp);
  return temp;
}

int assign_shader_vector(cosines(*shader)[IMAGE_WIDTH])
{
  int i = 0;
  int j = 0;
  for (j = 0;j < IMAGE_HEIGHT;j++)
  {
    for (i = 0;i < IMAGE_WIDTH;i++)
    {
      if ((RADIUS * RADIUS) >((i - CENTER_X)*(i - CENTER_X) + (j - CENTER_Y)*(j - CENTER_Y))) // means that the point is inside the circle
      {
        (*(*(shader + j) + i)).cos_alpha = ((double)i - CENTER_X) / RADIUS;
        (*(*(shader + j) + i)).cos_beta = ((double)j - CENTER_Y) / RADIUS;
        (*(*(shader + j) + i)).cos_gamma = cosz(((double)i - CENTER_X) / RADIUS, ((double)j - CENTER_Y) / RADIUS);
      }
    }
  }
  return 0;
}


void slider_function(int position, void*)
{
  double temp;
  int i = 0;
  int j = 0;
  vect v = { 0,0,0 };
  v.x = (double)SLIDER_MAX / 2 - (double)position;
  v.y = 0.00;
  v.z = -(double)30; //this decides the region upto which the artificial light is adjustable
  temp = sqrt(v.x*v.x + v.z*v.z);
  v.x = v.x / temp;
  v.z = v.z / temp;
  for (j = 0;j<IMAGE_HEIGHT;j++)
  {
    for (i = 0;i<IMAGE_WIDTH;i++)
    {
      if ((*(*(shader + j) + i)).cos_alpha != 0.00 || (*(*(shader + j) + i)).cos_beta != 0.00 || (*(*(shader + j) + i)).cos_gamma != 0.00)
      {
        temp = (-v.x * ((*(*(shader + j) + i)).cos_alpha) - v.y*((*(*(shader + j) + i)).cos_beta) - v.z*((*(*(shader + j) + i)).cos_gamma));
        if (temp > 0.00)
        {
          image.at<uchar>(j, i) = temp * 255.0;
        }
        else
        {
          image.at<uchar>(j, i) = 0;
        }
      }
    }
  }
  imshow(WINDOW_NAME, image);
  return;
}