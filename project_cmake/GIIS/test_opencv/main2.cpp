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

    void AddNoise(double mean, double stddev) {
        cv::Mat noiseR = cv::Mat(image_border.size(), CV_8U);
        cv::Mat noiseG = cv::Mat(image_border.size(), CV_8U);
        cv::Mat noiseB = cv::Mat(image_border.size(), CV_8U);

        cv::randn(noiseR, cv::Scalar::all(mean), cv::Scalar::all(stddev));
        cv::randn(noiseG, cv::Scalar::all(mean), cv::Scalar::all(stddev));
        cv::randn(noiseB, cv::Scalar::all(mean), cv::Scalar::all(stddev));

        std::vector<cv::Mat> channels;
        cv::split(image_border, channels);

        cv::add(channels[0], noiseB, channels[0]);
        cv::add(channels[1], noiseG, channels[1]);
        cv::add(channels[2], noiseR, channels[2]);

        cv::merge(channels, noisy_image);
    }


    void RemoveNoise(int kernel_size = 3, int threshold_noise = 49) {
        cv::Mat result = noisy_image.clone();
        int middle = kernel_size / 2;

        for (int x = 0; x < original_image.rows; ++x) {
            for (int y = 0; y < original_image.cols; ++y) {
                std::vector<cv::Vec3b> values;

                for (int kx = -middle; kx <= middle; ++kx) {
                    for (int ky = -middle; ky <= middle; ++ky) {
                        values.push_back(noisy_image.at<cv::Vec3b>(std::max(0, std::min(x - kx + 1, original_image.rows)), std::max(0, std::min(y - ky + 1, original_image.cols))));
                    }
                }

                cv::Vec3b temp_sum(0, 0, 0);
                for (auto& value : values) {
                    temp_sum += value;
                }
                temp_sum /= (kernel_size * kernel_size);

                std::sort(values.begin(), values.end(), [](const cv::Vec3b& a, const cv::Vec3b& b) {
                    return cv::norm(a, cv::NORM_L2) < cv::norm(b, cv::NORM_L2);
                });

                if ((cv::norm(values[kernel_size * kernel_size / 2], cv::NORM_L2) - cv::norm(temp_sum, cv::NORM_L2) + 50)  > threshold_noise) {
                    result.at<cv::Vec3b>(x, y) = values[kernel_size * kernel_size / 2];
                }
            }
        }
        after_image = result.clone();
    }


    void RemoveBorder(int border_size = 1) {
        cv::Rect roi(border_size, border_size, after_image.cols - 2 * border_size, after_image.rows - 2 * border_size);
        after_noborder = after_image(roi);
    }

    cv::Mat get_original() {
        return original_image;
    }

    cv::Mat get_border() {
        return image_border;
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
    cv::Mat noisy_image;
    cv::Mat after_image;
    cv::Mat after_noborder;
};

int main(int, char**) {
    TImage first("C:/project/project_cmake/GIIS/test_opencv/assets/931213131310.png");
    cv::imshow("name", first.get_original());
    cv::waitKey(0);

    first.AddingBorder(1);
    cv::imshow("name", first.get_border());
    cv::waitKey(0);

    first.AddNoise(0, 10.5);
    cv::imshow("name", first.get_noisy());
    cv::waitKey(0);

    first.RemoveNoise(3, 10);
    cv::imshow("name", first.get_after());
    cv::waitKey(0);

    first.RemoveBorder(1);
    cv::imshow("name", first.get_noborder());
    cv::waitKey(0);
}