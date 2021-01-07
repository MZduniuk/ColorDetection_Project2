#include "ColorDetectionAlgorithms.h"

namespace Algorithms
{
    HSVColorDetection::HSVColorDetection()
    {
    }

    HSVColorDetection::~HSVColorDetection()
    {
    }

    bool HSVColorDetection::RunColorDetection(cv::Mat& inputImage)
    {
        const size_t howManyColors = colorNames.size();

        for (size_t i = 0; i < howManyColors; i++)
        {
            cv::Mat hsvImage;
            cv::Mat detectionMask;

            cv::cvtColor(inputImage, hsvImage, cv::COLOR_BGR2HSV);
            cv::inRange(hsvImage, colorLowerBounds[i], colorUpperBounds[i], detectionMask);
            RunMorphologicalOperations(detectionMask);
            TrackObject(detectionMask, inputImage);

            cv::imshow("Test", inputImage);
        }

        return true;
    }

    void HSVColorDetection::GetDefaultColorValues(unsigned colorChoice)
    {
        if (colorChoice & (DefaultColors::YELLOW))
        {
            colorLowerBounds.emplace_back(cv::Scalar(20, 124, 200));
            colorUpperBounds.emplace_back(cv::Scalar(30, 256, 256));
            colors.emplace_back(cv::Scalar(0, 255, 255));
            colorNames.emplace_back("YELLOW");
        }

        if (colorChoice & (DefaultColors::PURPLE))
        {
            colorLowerBounds.emplace_back(cv::Scalar(130, 120, 150));
            colorUpperBounds.emplace_back(cv::Scalar(150, 215, 201));
            colors.emplace_back(cv::Scalar(128, 128, 128));
            colorNames.emplace_back("GRAY");
        }

        if (colorChoice & (DefaultColors::BROWN))
        {
            colorLowerBounds.emplace_back(cv::Scalar(0, 100, 50));
            colorUpperBounds.emplace_back(cv::Scalar(10, 130, 150));
            colors.emplace_back(cv::Scalar(0, 0, 0));
            colorNames.emplace_back("BLACK");
        }

        for (size_t i = 0; i < colorNames.size(); i++)
        {
            std::cout << colorNames[i] << "color objects detection enabled" << std::endl;
        }
    }

    void HSVColorDetection::RunMorphologicalOperations(cv::Mat& binaryMask)
    {
        cv::Mat erodeKernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
        cv::Mat dilateKernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));

        //Run image opening
        cv::erode(binaryMask, binaryMask, erodeKernel, cv::Point(-1, -1), 2);
        cv::dilate(binaryMask, binaryMask, dilateKernel, cv::Point(-1, -1), 2);
        //Run image closing
        cv::dilate(binaryMask, binaryMask, dilateKernel, cv::Point(-1, -1), 2);
        cv::erode(binaryMask, binaryMask, erodeKernel, cv::Point(-1, -1), 2);
    }

    void HSVColorDetection::TrackObject(const cv::Mat& detectionMask, cv::Mat& inputImage)
    {
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        const int maxContourArea = 1;

        if (cv::countNonZero(detectionMask) == 0)
        {
            printf("Empty mask");
            return;
        }

        cv::findContours(detectionMask, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

        if (contours.empty())
        {
            printf("[WARNING] No contours were found");
            return;
        }

        for (size_t i = 0; i < contours.size(); i++)
        {
            cv::Rect boundingBox;
            double currentArea = cv::contourArea(contours.at(i));

            if (currentArea < maxContourArea)
            {
                continue;
            }

            boundingBox = cv::boundingRect(contours[i]);
            cv::rectangle(inputImage, boundingBox.tl(), boundingBox.br(), cv::Scalar(0, 0, 255), 2);
        }
    }
}