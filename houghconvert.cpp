#include <iostream>
#include <fstream>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <math.h>
#include <vector>
#include <opencv2/opencv.hpp>
#include "readmovfile.h"

struct Val_ID{
  double val;
  int id;
  bool operator<(const Val_ID &another) const{
    return val < another.val;
  };
};



class Houghconv{
protected:
  std::vector<cv::Vec3f> circles;
  cv::Vec3f true_circle;
  cv::Vec3f prev_circle;
  long count;
  
public:
  Houghconv();
  void filter(const cv::Mat &m,cv::Mat &out_m);
  void getpos(const cv::Mat &m);
  int judgetruecircle();
  void drawcircle(cv::Mat &m);
};

Houghconv::Houghconv(){
  count = 0;
}

void Houghconv::filter(const cv::Mat &m,cv::Mat &out_m){
  out_m = m.clone();
  for(int ii = 0; ii<m.rows;ii++){
    for(int jj = 0;jj<m.cols;jj++){
      if(m.data[ii * m.step + jj*m.channels() + 0]>100){
	for(int ch=0;ch<m.channels();ch++){
	  out_m.data[ii * m.step + jj*m.channels() + ch] = 255;
	}
      }else{
	for(int ch=0;ch<m.channels();ch++){
	  out_m.data[ii * m.step + jj*m.channels() + ch] = 0;
	}
      }
    }
  }
}

void Houghconv::getpos(const cv::Mat &m){
  cv::Mat filt,gray;
  filter(m,filt);
  cv::blur(filt, filt, cv::Size(5,5));
  cvtColor(filt, gray, CV_BGR2GRAY);
  cv::threshold(gray,gray,100,255,cv::THRESH_BINARY);
  cv::Canny(gray, gray, 10, 100 * 2);
  HoughCircles(gray, circles, CV_HOUGH_GRADIENT,2, 1, 200, 100);
  judgetruecircle();
}

int Houghconv::judgetruecircle(){
  std::vector<Val_ID> errval;
  Val_ID vlid;
  double pre_absval;
  if(circles.size()==0){return 0;}
  if(count==0){
    true_circle[0]=0;
    true_circle[1]=0;
    true_circle[2]=0;
    for(int ii=0;ii<circles.size();ii++){
      true_circle[0] += circles[ii][0];
      true_circle[1] += circles[ii][1];
      true_circle[2] += circles[ii][2];
    }
    true_circle[0] /= circles.size();
    true_circle[1] /= circles.size();
    true_circle[2] /= circles.size();
  }else{
    pre_absval = true_circle[0]*true_circle[0] + true_circle[1]*true_circle[1] + true_circle[2]*true_circle[2];
    for(int ii=0;ii<circles.size();ii++){
      vlid.val = circles[ii][0]*circles[ii][0] + circles[ii][1]*circles[ii][1]+circles[ii][2]*circles[ii][2] - pre_absval;
      vlid.id = ii;
      errval.push_back(vlid); 
    }
    std::sort(errval.begin(),errval.end());
    true_circle = circles[errval[0].id];
  }
  prev_circle = true_circle;
}

void Houghconv::drawcircle(cv::Mat &m){
  for(int ii=0;ii<circles.size();ii++){
    cv::Point center(cvRound(circles[ii][0]), cvRound(circles[ii][1]));
    int radius = cvRound(circles[ii][2]);
    circle( m, center, 3, cv::Scalar(0,125,60), -1, 8, 0 );
    circle( m, center, radius, cv::Scalar(60,0,125), 3, 8, 0 );
  }
  cv::Point center(cvRound(true_circle[0]), cvRound(true_circle[1]));
  int radius = cvRound(true_circle[2]);
  circle( m, center, 3, cv::Scalar(0,255,0), -1, 8, 0 );
  circle( m, center, radius, cv::Scalar(0,0,255), 3, 8, 0 );
}
#if defined(HOUGH_IS_MAIN)
int main(int argh, char* argv[]){
  if(argh<2){
    std::cout << "error no argument" << std::endl;
    exit(0);
  }
  std::string st(argv[1]);
  ReadMOVfile readmv(st);
  cv::Mat m;
  Houghconv hough;
  while(1){
    if(readmv.getimage(m)==0){
      break;
    }
    hough.getpos(m);
    hough.drawcircle(m);
    cv::imshow("showing",m);
    cv::waitKey(1);
  }
  return 0;
}
#endif
