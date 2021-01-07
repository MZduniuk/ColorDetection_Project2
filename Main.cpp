#include <iostream>
#include "SystemSetup.h"
#include "ColorDetectionAlgorithms.h"

namespace Options
{
	Setup::DetectionAlgrotihms algorithmId;
	Setup::InputMode inputMode;
	int cameraPort;
	std::string inputFilePath;
	unsigned colorSelection;
}

bool InitializePipeline(int argc, char* argv[])
{
	const std::string keys =
		"{help h           |     | Print out help and info on the program}"
		"{input-mode i     |1    | Set type of input data (0 - web-cam feed, 1 - recorded video, 2 - single image)}"
		"{video-feed-index |0    | Set port from which web-cam feed should be streamed}"
		"{input-file-path  |     | Path to image or video file on which detection will be performed}"
		"{algorithm-mode a |0    | Set type of the color based object detection}"
		"{select-color     |7    | Select which of three default colors to detect (1 - yellow, 2 - purple, 4 - brown, 7 - all)"
		"[Sum of two number will result in using both options, e.g. 3 = yellow + purple]}"
		;

	cv::CommandLineParser commandParser(argc, argv, keys);
	commandParser.about("This program has been created as a part of CMP0 laboratory project"
		"at Warsaw University of Technology by Mateusz Zduniuk");

	if (commandParser.has("help"))
	{
		commandParser.printMessage();
		return false;
	}

	Options::inputMode = commandParser.get<Setup::InputMode>("input-mode");
	Options::cameraPort = commandParser.get<int>("video-feed-index");
	Options::algorithmId = commandParser.get<Setup::DetectionAlgrotihms>("algorithm-mode");

	if (Options::inputMode != Setup::InputMode::STREAM)
	{
		Options::inputFilePath = commandParser.get<std::string>("input-file-path");

		if (Options::inputFilePath.empty())
		{
			printf("No input path has been given");
			return false;
		}
		if (!std::filesystem::exists(Options::inputFilePath))
		{
			std::cout << "Given input path does not exist " << Options::inputFilePath << std::endl;
			return false;
		}
	}

	if (Options::algorithmId == Setup::DetectionAlgrotihms::HSVDetection)
	{
		printf("Using HSV based color detection");
		Options::colorSelection = commandParser.get<unsigned>("select-color");

		if (Options::colorSelection < 0 || Options::colorSelection > 7)
		{
			printf("Wrong color has been selected. Number out of range %d", Options::colorSelection);
			return false;
		}
	}

	return true;
}

int main(int argc, char* argv[])
{
	if (!InitializePipeline(argc, argv))
	{
		printf("Pipeline failed while initializing input data");
		return EXIT_FAILURE;
	}

	cv::VideoCapture capture;
	Algorithms::HSVColorDetection colorDetector;

	if(Options::inputMode == Setup::InputMode::STREAM)
	{
		printf("[INFO]Using live video stream");
		capture.open(Options::cameraPort, cv::CAP_ANY);
		if (!capture.isOpened())
		{
			printf("[ERROR]Failed to capture video stream from device ID: %d", Options::cameraPort);
			return EXIT_FAILURE;
		}
	}
	else
	{
		printf("[INFO]Using recorded video file");
		capture.open(Options::inputFilePath);
		if (!capture.isOpened())
		{
			std::cout << "[ERROR]Failed to load data from path:" << Options::inputFilePath << std::endl;
			return EXIT_FAILURE;
		}
	}

	if (Options::algorithmId == Setup::DetectionAlgrotihms::HSVDetection)
	{
		colorDetector.GetDefaultColorValues(Options::colorSelection);
	}

	while (true)
	{
		cv::Mat currentFrame;
		capture.read(currentFrame);

		if (currentFrame.empty())
		{
			std::cout << "[WARNING] Failed to read frame" << std::endl;
			break;
		}

		if (Options::algorithmId == Setup::DetectionAlgrotihms::HSVDetection)
		{
			colorDetector.RunColorDetection(currentFrame);
		}

		cv::imshow("Test", currentFrame);

		cv::waitKey(-1);
		char c = (char)cv::waitKey(25);
		if (c == 27)
			break;
	}

	capture.release();
	cv::destroyAllWindows();
	return EXIT_SUCCESS;
}