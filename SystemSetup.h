#pragma once
#include <filesystem>
#include <iostream>
#include "OpenCVIncludes.h"

namespace Setup
{
	
	/*! \enum DetectionAlgrotihms
		Enum for setting algorithm used for color based object detection.
	*/
	enum class DetectionAlgrotihms
	{
		HSVDetection = 0, //!< HSV hue channel thresholding based method
		DEV				  //!< Method developed by Mateusz Zduniuk
	};

	/*! \enum InputMode
		Enum used for setting type of input data that will be used for color based object detection.
	*/
	enum class InputMode
	{
		STREAM = 0,		//!< Use feed from web cam connected to the system.
		VIDEO,			//!< Use pre-recorded video file.
		IMAGE			//!< Use single image.
	};
}//namespace Setup