#pragma once
#include "OpenCVIncludes.h"
#include <iostream>

/*!< \file ColorDetectionAlgorithms
	This file contains set of algorithms for detecting objects in video stream 
	using color data. 
*/

namespace Algorithms
{
	enum DefaultColors
	{
		YELLOW = (1 << 0),
		PURPLE = (1 << 1),
		BROWN = (1 << 2)
	};

	class HSVColorDetection
	{
	public:
		HSVColorDetection();
		~HSVColorDetection();

		bool RunColorDetection(cv::Mat& inputImage);
		void GetDefaultColorValues(unsigned colorChoice);

	private:
		void RunMorphologicalOperations(cv::Mat& binaryMask);
		void TrackObject(const cv::Mat& detectionMask, cv::Mat& inputImage);

	private:
		std::vector<std::string> colorNames;
		std::vector<cv::Scalar> colorUpperBounds;
		std::vector<cv::Scalar> colorLowerBounds;
		std::vector<cv::Scalar> colors;
	};
}//namespace Algorithms