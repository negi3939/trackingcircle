#ifndef READMOV_H_
#define READMOV_H_
#include <opencv2/opencv.hpp>

class ReadMOVfile{
protected:
  std::string readfilename;
  cv::VideoCapture video;
  cv::Mat imag;
  int width, height, fps;
public:
  ReadMOVfile(std::string &s);
  int getimage(cv::Mat &m);
  int getinfo(int &l_width, int &l_height, double &l_fps);
};

#endif
