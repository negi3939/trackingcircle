#ifndef CAMERA_H_
#define CAMERA_H_
#include "opencv2/opencv.hpp"

class Camera{
	private:
	protected:
		std::string outpname;//出力用ファイル名
		cv::VideoCapture *cap;//カメラ
		cv::Mat frame,diff;//取得用とdiff用
	public:
		Camera();//カメラ0番でopen
		Camera(int c_n);//カメラc_n番でopen カメラ使わない場合は負の引数
		~Camera();//カメラを閉じる
		int read();//カメラから取得
		int read(std::string &imname);//画像取り込み
		int read(std::string &imname1,std::string &imname2);//画像差分取得用
		void setoutpname(std::string &imname);//出力するファイル名の設定
		virtual void show();//表示
		virtual void write();//保存
		int kbhit();//キーボード割り込み判定
};

#endif
