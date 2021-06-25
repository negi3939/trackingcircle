#include <iostream>
#include <fstream>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <opencv2/opencv.hpp>
#include "readmovfile.h"


ReadMOVfile::ReadMOVfile(std::string &s){
  readfilename = s;
  video.open(readfilename);
  if(video.isOpened() == false){
    std::cout << "Cannot open "<< readfilename << ". Check file name." << std::endl;
    return;
  }
  width  = (int)video.get(cv::CAP_PROP_FRAME_WIDTH);
	height = (int)video.get(cv::CAP_PROP_FRAME_HEIGHT);
	fps    = video.get(cv::CAP_PROP_FPS);
}

int ReadMOVfile::getimage(cv::Mat &m){
  video >> imag;
  if(imag.empty() == true){return 0;}
  m = imag;
  return 1;
}

int ReadMOVfile::getinfo(int &l_width, int &l_height, double &l_fps){
  l_width = width;
  l_height = height;
  l_fps = fps;
}

#if defined(READ_IS_MAIN)
int main(int argh, char* argv[]){
  if(argh<2){
    std::cout << "error no argument" << std::endl;
    exit(0);
  }
  std::string st(argv[1]);
  ReadMOVfile readmv(st);
  cv::Mat m;
  while(1){
    if(readmv.getimage(m)==0){
      break;
    }
    cv::imshow("showing",m);
    cv::waitKey(1);
  }
  return 0;
}
#endif
