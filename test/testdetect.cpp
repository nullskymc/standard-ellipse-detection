/*
 *Copyright: Copyright (c) 2019
 *Created on 2019-6-31
 *Author:zhengaohong@zgheye.cc
 *Version 1.0.1
*/

#include <iostream>
#include <vector>
#include <string>

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "ellipse_detection/types.hpp"
#include "ellipse_detection/detect.h"

using namespace std;
using namespace zgh;

int main(int argc, char* argv[]) {
  if (argc <= 1) {
    std::cout << "[Usage]: testdetect [image_dir1] [image_dir2] [image_dir3] ..." << std::endl;
  }
#if 0
  VideoCapture cap(argv[1]);
  Mat board;
  // int cnt = 0;
  while (1) {
    cap >> board;
    // resize(board, board, Size(board.cols / 3, board.rows / 3));
    if (board.empty()) {
      break;
    }
    Mat image;
    cvtColor(board, image, CV_RGB2GRAY);
    // cv::imwrite("test" + to_string(cnt++) + ".jpg", image);
    vector<shared_ptr<Ellipse> > ells;
    int row = image.rows;
    int col = image.cols;
    double width = 2.0;
    FuncTimerDecorator<int>("detectEllipse")(detectEllipse, image.data, row, col, ells, NONE_POL, width);
    cout << "Find " << ells.size() << " ellipse" << endl;
    for (int i = 0; i < (int)ells.size(); ++i) {
      auto ell = ells[i];
      std::cout << "coverangle : " << ell->coverangle << ",\tgoodness : " << ell->goodness << ",\tpolarity : " << ell->polarity << endl;
      ellipse(board,
              Point(ell->o.y, ell->o.x),
              Size(ell->a, ell->b),
              rad2angle(PI_2 - ell->phi),
              0,
              360,
              Scalar(0, 255, 0),
              width,
              8,
              0);

    }
    imshow("origin", image);
    imshow("detect", board);
    waitKey(0);

  }
#else
  std::string output_dir = "/mnt/m/scientificProject/output/";
  for (int i = 1; i < argc; ++i) {
    std::cout << "Trying to read image from: " << argv[i] << std::endl;
    cv::Mat board = cv::imread(argv[i]);
    if (board.empty()) {
      std::cerr << "Error: Could not read image from " << argv[i] << std::endl;
      continue;
    }
    cv::Mat image = cv::imread(argv[i], cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
      std::cerr << "Error: Could not read grayscale image from " << argv[i] << std::endl;
      continue;
    }
    // cv::resize(image, image, cv::Size(image.cols / 2, image.rows / 2));
    // cv::resize(board, board, cv::Size(board.cols / 2, board.rows / 2));
    // cv::imshow("origin image", image);
    vector<shared_ptr<Ellipse> > ells;
    int row = image.rows;
    int col = image.cols;
    double width = 2.0;
    FuncTimerDecorator<int>("detectEllipse")(detectEllipse, image.data, row, col, ells, NONE_POL, width);
    cout << "Find " << ells.size() << " ellipse(s)" << endl;
    for (int i = 0; i < (int)ells.size(); ++i) {
      auto ell = ells[i];
      std::cout << "coverangle : " << ell->coverangle
                << ",\tgoodness : " << ell->goodness
                << ",\tpolarity : "
                << ell->polarity
                << endl;
      cv::ellipse(board,
              cv::Point(ell->o.y, ell->o.x),
              cv::Size(ell->a, ell->b),
              rad2angle(PI_2 - ell->phi),
              0,
              360,
              cv::Scalar(0, 255, 0),
              width,
              8,
              0);

    }
    std::string output_filename = output_dir + "output_" + std::to_string(i) + ".jpg";
    cv::imwrite(output_filename, board);
    std::cout << "Saved result to " << output_filename << std::endl;
  }
  std::cout << "Processing complete. Please check the output images in " << output_dir << std::endl;
#endif
  return 0;
}

