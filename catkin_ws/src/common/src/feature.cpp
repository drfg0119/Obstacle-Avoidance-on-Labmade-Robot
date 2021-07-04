#include<iostream>
#include<algorithm>
#include<fstream>
#include<chrono>

#include<ros/ros.h>
#include <cv_bridge/cv_bridge.h>

#include<opencv2/core/core.hpp>
void OpenCVManager::testSiftFeatureDetector()
{
    QString fileName1 = "16.jpg";
    int width = 400;
    int height = 300;

    cv::Mat srcMat = cv::imread(fileName1.toStdString());
    cv::resize(srcMat, srcMat, cv::Size(width, height));

    cv::String windowName = _windowTitle.toStdString();
    cvui::init(windowName);

    cv::Mat windowMat = cv::Mat(cv::Size(srcMat.cols * 2, srcMat.rows * 3),
                                srcMat.type());

    cv::Ptr<cv::xfeatures2d::SIFT> _pSift = cv::xfeatures2d::SiftFeatureDetector::create();

    int k1x = 0;
    int k1y = 0;
    int k2x = 100;
    int k2y = 0;
    int k3x = 100;
    int k3y = 100;
    int k4x = 0;
    int k4y = 100;
    while(true)
    {
        windowMat = cv::Scalar(0, 0, 0);

        cv::Mat mat;

        // 原图先copy到左边
        mat = windowMat(cv::Range(srcMat.rows * 1, srcMat.rows * 2),
                        cv::Range(srcMat.cols * 0, srcMat.cols * 1));
        cv::addWeighted(mat, 0.0f, srcMat, 1.0f, 0.0f, mat);

        {
            std::vector<cv::KeyPoint> keyPoints1;
            std::vector<cv::KeyPoint> keyPoints2;

           cvui::printf(windowMat, 0 + width * 1, 10 + height * 0, "k1x");
           cvui::trackbar(windowMat, 0 + width * 1, 20 + height * 0, 165, &k1x, 0, 100);
           cvui::printf(windowMat, 0 + width * 1, 70 + height * 0, "k1y");
           cvui::trackbar(windowMat, 0 + width * 1, 80 + height * 0, 165, &k1y, 0, 100);

           cvui::printf(windowMat, width / 2 + width * 1, 10 + height * 0, "k2x");
           cvui::trackbar(windowMat, width / 2 + width * 1, 20 + height * 0, 165, &k2x, 0, 100);
           cvui::printf(windowMat, width / 2 + width * 1, 70 + height * 0, "k2y");
           cvui::trackbar(windowMat, width / 2 + width * 1, 80 + height * 0, 165, &k2y, 0, 100);

           cvui::printf(windowMat, 0 + width * 1, 10 + height * 0 + height / 2, "k3x");
           cvui::trackbar(windowMat, 0 + width * 1, 20 + height * 0 + height / 2, 165, &k3x, 0, 100);
           cvui::printf(windowMat, 0 + width * 1, 70 + height * 0 + height / 2, "k3y");
           cvui::trackbar(windowMat, 0 + width * 1, 80 + height * 0 + height / 2, 165, &k3y, 0, 100);

           cvui::printf(windowMat, width / 2 + width * 1, 10 + height * 0 + height / 2, "k4x");
           cvui::trackbar(windowMat, width / 2 + width * 1, 20 + height * 0 + height / 2, 165, &k4x, 0, 100);
           cvui::printf(windowMat, width / 2 + width * 1, 70 + height * 0 + height / 2, "k4y");
           cvui::trackbar(windowMat, width / 2 + width * 1, 80 + height * 0 + height / 2, 165, &k4y, 0, 100);

           std::vector<cv::Point2f> srcPoints;
           std::vector<cv::Point2f> dstPoints;

           srcPoints.push_back(cv::Point2f(0.0f, 0.0f));
           srcPoints.push_back(cv::Point2f(srcMat.cols - 1, 0.0f));
           srcPoints.push_back(cv::Point2f(srcMat.cols - 1, srcMat.rows - 1));
           srcPoints.push_back(cv::Point2f(0.0f, srcMat.rows - 1));

           dstPoints.push_back(cv::Point2f(srcMat.cols * k1x / 100.0f, srcMat.rows * k1y / 100.0f));
           dstPoints.push_back(cv::Point2f(srcMat.cols * k2x / 100.0f, srcMat.rows * k2y / 100.0f));
           dstPoints.push_back(cv::Point2f(srcMat.cols * k3x / 100.0f, srcMat.rows * k3y / 100.0f));
           dstPoints.push_back(cv::Point2f(srcMat.cols * k4x / 100.0f, srcMat.rows * k4y / 100.0f));

           cv::Mat M = cv::getPerspectiveTransform(srcPoints, dstPoints);
           cv::Mat srcMat2;
           cv::warpPerspective(srcMat,
                               srcMat2,
                               M,
                               cv::Size(srcMat.cols, srcMat.rows),
                               cv::INTER_LINEAR,
                               cv::BORDER_CONSTANT,
                               cv::Scalar::all(0));

           mat = windowMat(cv::Range(srcMat.rows * 1, srcMat.rows * 2),
                           cv::Range(srcMat.cols * 1, srcMat.cols * 2));
           cv::addWeighted(mat, 0.0f, srcMat2, 1.0f, 0.0f, mat);

           //特征点检测
           _pSift->detect(srcMat, keyPoints1);
           //绘制特征点(关键点)
           cv::Mat resultShowMat;
           cv::drawKeypoints(srcMat,
                             keyPoints1,
                             resultShowMat,
                             cv::Scalar(0, 0, 255),
                             cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
           mat = windowMat(cv::Range(srcMat.rows * 2, srcMat.rows * 3),
                           cv::Range(srcMat.cols * 0, srcMat.cols * 1));
           cv::addWeighted(mat, 0.0f, resultShowMat, 1.0f, 0.0f, mat);

           //特征点检测
           _pSift->detect(srcMat2, keyPoints2);
           //绘制特征点(关键点)
           cv::Mat resultShowMat2;
           cv::drawKeypoints(srcMat2,
                             keyPoints2,
                             resultShowMat2,
                             cv::Scalar(0, 0, 255),
                             cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
           mat = windowMat(cv::Range(srcMat.rows * 2, srcMat.rows * 3),
                           cv::Range(srcMat.cols * 1, srcMat.cols * 2));
           cv::addWeighted(mat, 0.0f, resultShowMat2, 1.0f, 0.0f, mat);

           cv::imshow(windowName, windowMat);
        }
        // 更新
        cvui::update();
        // 显示
        // esc键退出
        if(cv::waitKey(25) == 27)
        {
            break;
        }
    }
}

