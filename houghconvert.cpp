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
#include "filesave.h"

struct Val_ID{
  double val;
  int id;
  bool operator<(const Val_ID &another) const{
    return val < another.val;
  };
};

struct numposition{
  int num;
  double x;
  double y;
  double r;
};

class Houghconv{
protected:
  std::vector<cv::Vec3f> circles;
  cv::Vec3f true_circle;
  cv::Vec3f prev_circle;
  long count;
  int framenum;
public:
  Houghconv();
  void filter(const cv::Mat &m,cv::Mat &out_m);
  int getpos(const cv::Mat &m,numposition &np);
  int judgetruecircle();
  void drawcircle(cv::Mat &m);
};

Houghconv::Houghconv(){
  count = 0;
  framenum=0;
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

int Houghconv::getpos(const cv::Mat &m,numposition &np){
  cv::Mat filt,gray;
  filter(m,filt);
  cv::blur(filt, filt, cv::Size(5,5));
  cvtColor(filt, gray, cv::COLOR_BGR2GRAY);
  cv::threshold(gray,gray,100,255,cv::THRESH_BINARY);
  cv::Canny(gray, gray, 10, 100 * 2);
  HoughCircles(gray, circles, CV_HOUGH_GRADIENT,2, 1, 200, 100);
  int ret = judgetruecircle();
  if(ret){
    np.num = framenum;
    np.x = true_circle[0];
    np.y = true_circle[1];
    np.r = true_circle[2];
  }
  framenum++;
  return ret;
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
  count++;
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

void filesavenumpos(std::string &fst,std::vector<numposition> &np_his){
  Filesave fs(fst);
  for(int ii=0;ii<np_his.size();ii++){
    fs.write_fc((double)np_his[ii].num);
    fs.write_fc((double)np_his[ii].x);
    fs.write_fc((double)np_his[ii].y);
    fs.write_fn((double)np_his[ii].r);
  }
  std::cout << "saved file name is "<< fst << "." << std::endl;
};


#if defined(HOUGH_IS_MAIN)
int main(int argh, char* argv[]){
  if(argh<2){
    std::cout << "error no argument" << std::endl;
    exit(0);
  }
  int screen_b = 1;
  std::string st(argv[1]);
  std::string noscst("noscreen");
 
  if(argh>2){
    std::string optionst(argv[2]);
    if(optionst==noscst){
      screen_b = 0;
      std::cout << "no screen mode. now converting..." << std::endl;
    }
  }
  ReadMOVfile readmv(st);
  std::string savefilename;
  st.resize(st.size()-4);
  savefilename = st + "_position.csv";
  cv::Mat m;
  numposition np;
  std::vector<numposition> np_history;
  Houghconv hough;
  int judge_fl;
  while(1){
    if(readmv.getimage(m)==0){
      break;
    }
    judge_fl = hough.getpos(m,np);
    if(judge_fl){np_history.push_back(np);}
    if(screen_b){
      hough.drawcircle(m);
      cv::imshow("showing",m);
      cv::waitKey(1);
    }
  }
  filesavenumpos(savefilename,np_history);
  return 0;
}
#endif
