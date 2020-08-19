#include <unistd.h>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <vector>

using namespace std;
using namespace cv;

void CreateAlphMat(Mat mat) {
  for (int i = 0; i < mat.rows; i++) {
    for (int j = 0; j < mat.cols; j++) {
      Vec4b& rgba = mat.at<Vec4b>(i, j);
      rgba[0] = UCHAR_MAX;
      rgba[1] = saturate_cast<uchar>((float(mat.cols - j)) / ((float)mat.cols) *
                                     UCHAR_MAX);
      rgba[2] = saturate_cast<uchar>((float(mat.rows - i)) / ((float)mat.cols) *
                                     UCHAR_MAX);
      rgba[3] = saturate_cast<uchar>(0.5 * (rgba[1] + rgba[2]));
    }
  }
}

int main(int argc, char* argv[]) {
  Mat src_image = imread("224727-15637204471cad.jpg");
  namedWindow("origin");
  imshow("origin", src_image);

  // create Mat with alpha channel
  Mat mat(480, 640, CV_8UC4);
  CreateAlphMat(mat);
  vector<int> compression_params;
  // compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
  // compression_params.push_back(9);

  VideoCapture capature(0);
  if (!capature.isOpened()) {
    cout << "open camera failed" << endl;
    return -1;
  }
  while (true) {
    Mat frame;
    capature >> frame;
    if (!frame.empty()) {
      imshow("camera", frame);
    }
    if (waitKey(30) > 0) {
      break;
    }
  }

#if 0
  try {
    imwrite("透明 Alpha 值图.png", mat, compression_params);
    imshow("生成的 PNG 图", mat);
    fprintf(stdout,
            "PNG图片文件的 alpha 数据保存完毕\n 可以在工程目录下查看\n");
  } catch (runtime_error& er) {
    fprintf(stderr, "图片转换成PNG格式发生错误：%s\n", er.what());
    return 1;
  }
#endif
  waitKey(0);
  return 0;
}