#include <iostream>
#include <fstream>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include "opencv2/opencv.hpp"

class ReadMOVfile{
protected:
  std::string readfilename;
  cv::VideoCapture video;
  cv::Mat imag;
public:
  ReadMOVfile(std::string &s);
  int getimage(cv::Mat &m);
};

ReadMOVfile::ReadMOVfile(std::string &s){
  readfilename = s;
  video.open(readfilename);
  if(video.isOpened() == false){
    std::cout << "Cannot open "<< readfilename << ". Check file name." << std::endl;
    return;
  }
}

int ReadMOVfile::getimage(cv::Mat &m){
  video >> imag;
  if(imag.empty() == true){return 0;}
  m = imag;
  return 1;
}

#if defined(READ_IS_MAIN)
int main(int argh, char* argv[]){
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
