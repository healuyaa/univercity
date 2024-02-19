#include <opencv2/opencv.hpp>
#include <vector>
#include <functional>

#define original 0
#define border 1
#define noisy 2
#define removenoisy 3
#define removeborder 4

class UsessTI {
    public:
    static std::string path_to_image;
    static int timer;
    static std::vector<std::string> pathes_after_save;
    static std::string path_to_folder;
};
std::string UsessTI::path_to_image = "";
int UsessTI::timer = 0;
std::vector<std::string> UsessTI::pathes_after_save(5, "");
std::string UsessTI::path_to_folder = "";

class TImage {
    public:
    TImage(std::string path) {
        original_image = cv::imread(path);
    }

    void AddingBorder(int border_size = 1) {
        cv::copyMakeBorder(original_image, image_border, border_size, border_size, border_size, border_size, cv::BORDER_CONSTANT, cv::Scalar(255, 255, 255));
    }

    void AddNoisy(double mean, double stddev) {
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

    void RemoveNoise(int kernel_size = 3, int threshold_noise = 49, std::function<void(float)> progressCallback = nullptr) {
        cv::Mat result = noisy_image.clone();
        int middle = kernel_size / 2;
        int total_pixels = original_image.rows * original_image.cols;
        int current_pixel = 0;

        for (int x = 0; x < original_image.rows; ++x) {
            for (int y = 0; y < original_image.cols; ++y) {
                std::vector<cv::Vec3b> values;

                for (int kx = -middle; kx <= middle; ++kx) {
                    for (int ky = -middle; ky <= middle; ++ky) {
                        values.push_back(noisy_image.at<cv::Vec3b>(std::max(0, std::min(x - kx + 1, original_image.rows - 1)), 
                                                                    std::max(0, std::min(y - ky + 1, original_image.cols - 1))));
                    }
                }

                cv::Vec3b temp_sum(0, 0, 0);
                for (auto& value : values) {
                    temp_sum += value;
                }
                temp_sum /= (kernel_size * kernel_size);

                std::nth_element(values.begin(), values.begin() + kernel_size * kernel_size / 2, values.end(), [](const cv::Vec3b& a, const cv::Vec3b& b) {
                    return cv::norm(a, cv::NORM_L2) < cv::norm(b, cv::NORM_L2);
                });

                if ((cv::norm(values[kernel_size * kernel_size / 2], cv::NORM_L2) - cv::norm(temp_sum, cv::NORM_L2))  > threshold_noise) {
                    result.at<cv::Vec3b>(x, y) = values[kernel_size * kernel_size / 2];
                }               

                ++current_pixel;
                float progress = ((float)current_pixel) / ((float)(total_pixels + 1));
                progressCallback(progress);
            }
        }
        after_image = result.clone();
    }

    void RemoveBorder(int border_size = 1) {
        cv::Rect roi(border_size, border_size, after_image.cols - 2 * border_size, after_image.rows - 2 * border_size);
        after_noborder = after_image(roi);
    }

    void SaveImages(const std::string path) {
        cv::imwrite(path + "/original_image.jpg", original_image);
        // cv::imwrite(path + "/image_border.jpg", image_border);
        cv::imwrite(path + "/noisy_image.jpg", noisy_image);
        // cv::imwrite(path + "/after_image.jpg", after_image);
        cv::imwrite(path + "/no_noisy_image.jpg", after_noborder);
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

    ~TImage() {}
    
    private:
    cv::Mat original_image;
    cv::Mat image_border;
    cv::Mat noisy_image;
    cv::Mat after_image;
    cv::Mat after_noborder;
};

