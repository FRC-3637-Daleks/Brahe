#pragma once
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <vector>
#include <string>
#include <math.h>

namespace grip {

/**
* GripPipeline class.
* 
* An OpenCV pipeline generated by GRIP.
*/
class GripPipeline {
	private:
		cv::Mat source0;
		cv::Mat rgbThresholdOutput;
		cv::Mat cvErodeOutput;
		cv::Mat cvDilateOutput;
		std::vector<std::vector<cv::Point> > findContoursOutput;
		void rgbThreshold(cv::Mat &, double [], double [], double [], cv::Mat &);
		void cvErode(cv::Mat &, cv::Mat &, cv::Point &, double , int , cv::Scalar &, cv::Mat &);
		void cvDilate(cv::Mat &, cv::Mat &, cv::Point &, double , int , cv::Scalar &, cv::Mat &);
		void findContours(cv::Mat &, bool , std::vector<std::vector<cv::Point> > &);

	public:
		GripPipeline();
		void process(cv::Mat source0);
		void setsource0(cv::Mat &source0);
		cv::Mat* getrgbThresholdOutput();
		cv::Mat* getcvErodeOutput();
		cv::Mat* getcvDilateOutput();
		std::vector<std::vector<cv::Point> >* getfindContoursOutput();
};


} // end namespace grip


