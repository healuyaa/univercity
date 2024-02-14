#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

class Timer {
    public:

    int get_timer() {
        return timer; 
    }

    static int timer;
};
int Timer::timer = 0;

class TImage : Timer{
    public:
    TImage(std::string path) {
        original_image = cv::imread(path);
    }

    void AddingBorder(int border_size = 1) {
        cv::copyMakeBorder(original_image, image_border, border_size, border_size, border_size, border_size, cv::BORDER_CONSTANT, cv::Scalar(255, 255, 255));
    }

    void ColorChange2wb(int threeshold_value) {
        cv::Mat image2gr;
        cv::cvtColor(image_border, image2gr, cv::COLOR_BGR2GRAY);
        cv::threshold(image2gr, image2wb, threeshold_value, 255, cv::THRESH_BINARY);
    }

    void AddNoise(double mean, double stddev) {
        cv::Mat noise = cv::Mat(image2wb.size(), CV_8U);
        cv::randn(noise, cv::Scalar::all(mean), cv::Scalar::all(stddev));
        cv::add(image2wb, noise, noisy_image);
    }

    void RemoveNoise(int kernel_size = 3, int threshold_noise = 11) {
        cv::Mat result = noisy_image.clone();
        int middle = kernel_size / 2;

        for (int x{0}; x < original_image.rows; ++x) {
            for (int y{0}; y < original_image.cols; ++y) {
                std::vector<uchar> values;

                for (int kx{-middle}; kx <= middle; ++kx) {
                    for (int ky{-middle}; ky <= middle; ++ky) {
                        values.push_back(noisy_image.at<uchar>(std::max(0, std::min(x - kx + 1, original_image.rows)), std::max(0, std::min(y - ky + 1, original_image.cols))));                     
                    }
                }
                
                int temp_sum = 0;
                for (auto a: values) {
                    temp_sum += a;
                }
                temp_sum /= 8;

                std::sort(values.begin(), values.end());
                if ((values[kernel_size * kernel_size / 2] - temp_sum + 50) > threshold_noise) {
                    result.at<uchar>(x, y) = values[kernel_size * kernel_size / 2];   
                }
            }
            ++timer;
        }
        after_image = result.clone();
    }

    void RemoveBorder(int border_size = 1) {
        cv::Rect roi(border_size, border_size, after_image.cols - 2 * border_size, after_image.rows - 2 * border_size);
        after_noborder = after_image(roi).clone();
    }

    cv::Mat get_original() {
        return original_image;
    }

    cv::Mat get_border() {
        return image_border;
    }

    cv::Mat get2wb() {
        return image2wb;
    }

    cv::Mat get_noisy() {
        return noisy_image;
    }

    cv::Mat get_after() {
        return after_image;
    }

    cv::Mat get_noborder() {
        return after_noborder;
    }
    
    private:
    cv::Mat original_image;
    cv::Mat image_border;
    cv::Mat image2wb;
    cv::Mat noisy_image;
    cv::Mat after_image;
    cv::Mat after_noborder;
};

int main(int, char**) {
    TImage first("C:\\project\\project_cmake\\GIIS\\lab1\\assets\\w480.jpg");
    cv::imshow("name", first.get_original());
    cv::waitKey(0);

    first.AddingBorder(1);
    cv::imshow("name", first.get_border());
    cv::waitKey(0);

    first.ColorChange2wb(100);
    cv::imshow("name", first.get2wb());
    cv::waitKey(0);

    first.AddNoise(0.0, 10.5);
    cv::imshow("name", first.get_noisy());
    cv::waitKey(0);

    first.RemoveNoise(3, 10);
    cv::imshow("name", first.get_after());
    cv::waitKey(0);

    first.RemoveBorder(1);
    cv::imshow("name", first.get_noborder());
    cv::waitKey(0);
}