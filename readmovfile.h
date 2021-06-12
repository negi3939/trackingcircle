#ifndef READMOV_H_
#define READMOV_H_
#include <opencv2/opencv.hpp>

class ReadMOVfile{
protected:
  std::string readfilename;
  cv::VideoCapture video;
  cv::Mat imag;
public:
  ReadMOVfile(std::string &s);
  int getimage(cv::Mat &m);
};

#endif
