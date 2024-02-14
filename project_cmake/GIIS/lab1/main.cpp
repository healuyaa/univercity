#include <iostream>
#include <Windows.h>
#include <filesystem>

#include "imgui.h"

#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"

#include "GLFW/glfw3.h"

#include <opencv2/opencv.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#pragma comment(lib, "legacy_stdio_definitions.lib")

class UsessTI {
    public:
    static std::string path_to_image;
    static int timer;
};
std::string UsessTI::path_to_image = "";
int UsessTI::timer = 0;

class TImage {
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

    void AddNoisy(double mean, double stddev) {
        cv::Mat noise = cv::Mat(image2wb.size(), CV_8U);
        cv::randn(noise, cv::Scalar::all(mean), cv::Scalar::all(stddev));
        cv::add(image2wb, noise, noisy_image);
    }

    void RemoveNoise(int kernel_size = 3, int threshold_noise = 49) {
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
            ++UsessTI::timer;
        }
        after_image = result.clone();
    }

    void RemoveBorder(int border_size = 1) {
        cv::Rect roi(border_size, border_size, after_image.cols - 2 * border_size, after_image.rows - 2 * border_size);
        after_noborder = after_image(roi).clone();
    }

    void SaveImages(const std::string path) {
        cv::imwrite(path + "/original_image.jpg", original_image);
        cv::imwrite(path + "/image_border.jpg", image_border);
        cv::imwrite(path + "/image2wb.jpg", image2wb);
        cv::imwrite(path + "/noisy_image.jpg", noisy_image);
        cv::imwrite(path + "/after_image.jpg", after_image);
        cv::imwrite(path + "/after_noborder.jpg", after_noborder);
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

    ~TImage() {}
    
    private:
    cv::Mat original_image;
    cv::Mat image_border;
    cv::Mat image2wb;
    cv::Mat noisy_image;
    cv::Mat after_image;
    cv::Mat after_noborder;
};

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void OpenFileButton() { 
    OPENFILENAME ofn; // open dialog with filesystem
    char szFileName[MAX_PATH] = ""; // temporary!!!!!

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFilter = "Images\0*.bmp;*.jpg;*.png\0All Files\0*.*\0";
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = sizeof(szFileName);
    ofn.lpstrTitle = "Chouse image";
    ofn.Flags = OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE)
        UsessTI::path_to_image = szFileName;          
}

void do_transform_image(TImage &first, int threeshold2wb, float threeshold_add_noisy, int threeshold_remove_noisy, int border = 1, int kernel_size = 3) {
    first.AddingBorder(border);
    first.ColorChange2wb(threeshold2wb);
    first.AddNoisy(0.0, threeshold_add_noisy);
    first.RemoveNoise(kernel_size, threeshold_remove_noisy);
    first.RemoveBorder(border);

    // first.SaveImages("C:/project/project_cmake/GIIS/lab1/build/Debug/out");
    // first.~TImage();
}

void displayImage(const cv::Mat& image) {
    const void* imgData = static_cast<const void*>(image.data);
    ImVec2 imageSize(static_cast<float>(image.cols), static_cast<float>(image.rows));
    ImGui::Image((void *)imgData, imageSize);
}

int main() {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    GLFWwindow* window = glfwCreateWindow(1280, 800, "remove-noisy-window", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    if (!ImGui_ImplOpenGL3_Init("#version 330 core")) {
        std::cerr << "Failed to initialize ImGui OpenGL backend!" << std::endl;
        return -1;
    }

    if (!ImGui_ImplGlfw_InitForOpenGL(window, true)) {
        std::cerr << "Failed to initialize ImGui GLFW backend!" << std::endl;
        return -1;
    }

    ImGui::StyleColorsDark();
    ImVec4 clear_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    bool show_demo_window = true;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        {
            ImGui::SetNextWindowSize(ImVec2(520, 350));
            ImGui::SetNextWindowPos(ImVec2(10, 10));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);

            static int threeshold2wb = 100;
            static float threeshold_add_noisy = 10.5;
            static int threeshold_remove_noisy = 10;

            static int border = 1;
            static int kernel_size = 1;

            static bool check_button = false;

            ImGui::Begin("window settings", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
            
            {
                if(ImGui::Button("Open file")) {
                    OpenFileButton();
                }                

                ImGui::SliderInt("Threeshold black-white", &threeshold2wb, 0, 255);
                ImGui::SliderFloat("Threeshold add noisy", &threeshold_add_noisy, 0.0, 20.0);
                ImGui::SliderInt("Threeshold remove noisy", &threeshold_remove_noisy, 0, 20);

                TImage first(UsessTI::path_to_image);
                if(ImGui::Button("Confirm")) {
                    if(UsessTI::path_to_image != "") {
                        do_transform_image(first, threeshold2wb, threeshold_add_noisy, threeshold_remove_noisy, border, kernel_size);
                    }
                }
                ImGui::SameLine();

                if(ImGui::Button("View")) {
                    check_button = true;
                }
                
                if(check_button) {
                    {
                        // ImGui::Begin("original");
                        // ImGui::Image(MatToTexture(first.get_original()), ImVec2(first.get_original().cols, first.get_original().rows));
                        // ImGui::End();

                        // OpenCVImageDisplay(first.get_original());
                        // OpenCVImageDisplay(first.get_border());
                        // OpenCVImageDisplay(first.get2wb());
                        // OpenCVImageDisplay(first.get_noisy());
                        // OpenCVImageDisplay(first.get_after());
                        // OpenCVImageDisplay(first.get_noborder());
                        ImGui::Begin("original");
                        displayImage(first.get_original());
                        ImGui::End();
                    }

                    // {
                    //     ImGui::Begin("border");
                    //     ImGui::Image(MatToTexture(first.get_border()), ImVec2(first.get_border().cols, first.get_border().rows));
                    //     ImGui::End();
                    // }

                    // {
                    //     ImGui::Begin("wb");
                    //     ImGui::Image(MatToTexture(first.get2wb()), ImVec2(first.get2wb().cols, first.get2wb().rows));
                    //     ImGui::End();
                    // } 
                }
            }
            ImGui::PopStyleVar();
            ImGui::End();
        }

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}