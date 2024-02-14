#include <iostream>
#include <Windows.h>
#include <filesystem>
#include <vector>
#include <algorithm>

#include "imgui.h"

#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx9.h"

#include <d3d9.h>
#include <d3dx9.h>
#include <D3dx9tex.h>

#include <tchar.h>

#include <opencv2/opencv.hpp>

#include "TImage/TImage.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"



static LPDIRECT3D9              g_pD3D = nullptr;
static LPDIRECT3DDEVICE9        g_pd3dDevice = nullptr;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


static float progress_bar = 0;
float UpdateProgressBar(float progress) {
    progress_bar = progress;
    return progress;
}

template<class T>
const T& clamp(const T& value, const T& low, const T& high) {
    return (value < low) ? low : (value > high) ? high : value;
}


void StyleSettingsWindow() {
    ImGuiStyle& style_settings_window = ImGui::GetStyle();
    style_settings_window.Colors[ImGuiCol_TitleBg] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
    style_settings_window.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
    style_settings_window.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);

    style_settings_window.Colors[ImGuiCol_Button] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
    style_settings_window.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
    style_settings_window.Colors[ImGuiCol_ButtonActive] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);

    style_settings_window.Colors[ImGuiCol_SliderGrab] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
    style_settings_window.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);

    style_settings_window.FrameRounding = 4.0f;
    style_settings_window.GrabRounding = 4.0f;
    
    ImGui::SetNextWindowSize(ImVec2(300, 300));
    ImGui::SetNextWindowPos(ImVec2(10, 10));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
}

void StyleWindowImages() {
    ImGuiStyle& style_images_window = ImGui::GetStyle();
    style_images_window.Colors[ImGuiCol_TitleBg] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
    style_images_window.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
    style_images_window.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
}


void OpenFileButton() { 
    OPENFILENAME ofn;
    char szFileName[MAX_PATH] = "";

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

std::string CreateDirectoryPathOut() {
    std::string documentsPath = std::getenv("USERPROFILE");
    documentsPath += "/Documents/out";
    CreateDirectoryA(documentsPath.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS;
    return documentsPath;
}

void DoTransformImage(TImage &first, float threeshold_add_noisy, int threeshold_remove_noisy, int border_size = 1, int kernel_size = 3) {
    first.AddingBorder(border);
    first.AddNoisy(0.0, threeshold_add_noisy);
    first.RemoveNoise(kernel_size, threeshold_remove_noisy, UpdateProgressBar);
    first.RemoveBorder(border);

    std::string documentsPath = CreateDirectoryPathOut();
    first.SaveImages(documentsPath);

    UsessTI::pathes_after_save[original] += documentsPath + "/original_image.jpg";
    // UsessTI::pathes_after_save[border] += documentsPath + "/image_border.jpg";
    UsessTI::pathes_after_save[noisy] += documentsPath + "/noisy_image.jpg";
    // UsessTI::pathes_after_save[removenoisy] += documentsPath + "/after_image.jpg";
    UsessTI::pathes_after_save[removeborder] += documentsPath + "/no_noisy_image.jpg";
}

bool LoadTextureFromFile(const char* filename, PDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height)
{
    PDIRECT3DTEXTURE9 texture;
    HRESULT hr = D3DXCreateTextureFromFileA(g_pd3dDevice, filename, &texture);
    if (hr != S_OK)
        return false;

    D3DSURFACE_DESC my_image_desc;
    texture->GetLevelDesc(0, &my_image_desc);
    *out_texture = texture;
    *out_width = (int)my_image_desc.Width;
    *out_height = (int)my_image_desc.Height;
    return true;
}

void RenderImagesWindow(std::string name, int &my_image_width, int &my_image_height, PDIRECT3DTEXTURE9 &my_texture) {
    ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoResize);
    ImGui::Text("size = %d x %d", my_image_width, my_image_height);
    ImGui::Image((void*)my_texture, ImVec2(static_cast<float>(my_image_width), static_cast<float>(my_image_height)));
    ImGui::End();
}

void WindowSettingUI() {
    static float threeshold_add_noisy = 23;
    static int threeshold_remove_noisy = 9;

    static int border_size = 1;
    static int kernel_size = 3;

    static bool check_button = false;
    
    if(ImGui::Button("Open file")) {
        OpenFileButton();
    }

    ImGui::SameLine();
    if(ImGui::Button("Save file")) {

    }
    ImGui::Spacing();

    ImGui::PushItemWidth(250);
    ImGui::Text("Kernel size");
    if(ImGui::InputInt("##Kernel size(min: 3, max: 19)", &kernel_size, 2)) {
        kernel_size = (kernel_size % 2 == 0) ? kernel_size + 1 : kernel_size;
        kernel_size = clamp(kernel_size, 3, 19);
        border_size = kernel_size / 2;
    }
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::Text("min: 3, max 19");
        ImGui::EndTooltip();
    }
    ImGui::Spacing();

    ImGui::Text("Border size");
    if(ImGui::InputInt("##Border size(min: 1, max: 9)", &border_size , 1, 9)) {
        border_size = clamp(border_size, 3, 9);
        kernel_size = border_size * 2 + 1;
    }
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::Text("min: 1, max: 9");
        ImGui::EndTooltip();
    }
    ImGui::Spacing();

    ImGui::Text("Change the amount of noise");
    if(ImGui::InputFloat("##Increase noise", &threeshold_add_noisy, 2)) {
        threeshold_add_noisy = clamp(threeshold_add_noisy, (float)0.0, (float)100.0);
    }
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::Text("min: 0.0, max: 100.0");
        ImGui::EndTooltip();
    }
    ImGui::Spacing();

    ImGui::Text("Change noise smoothing threshold");
    if(ImGui::InputInt("##Reduce noise", &threeshold_remove_noisy, 2)) {
        threeshold_remove_noisy = clamp(kernel_size, 0, 50);
    }
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::Text("min: 0, max: 50");
        ImGui::EndTooltip();
    }
    ImGui::Spacing();
    ImGui::PopItemWidth();

    TImage first(UsessTI::path_to_image);
    if(ImGui::Button("Confirm")) {
        if(UsessTI::path_to_image != "") {
            DoTransformImage(first, threeshold_add_noisy, threeshold_remove_noisy, border_size, kernel_size);
        }
    }

    ImGui::SameLine();
    if(ImGui::Button("View")) {
        if(UsessTI::path_to_image != "") {
            check_button = true;
            UsessTI::path_to_image = "";
        }
    }
    ImGui::Spacing();

    ImGui::ProgressBar(progress_bar, ImVec2(0.0f, 0.0f));

    if(check_button) {
        {
            StyleWindowImages();
            
            int my_image_width4 = 0;
            int my_image_height4 = 0;
            PDIRECT3DTEXTURE9 my_texture4 = NULL;
            bool ret4;
            ret4 = LoadTextureFromFile(UsessTI::pathes_after_save[removeborder].c_str(), &my_texture4, &my_image_width4, &my_image_height4);
            IM_ASSERT(ret4);
            std::string name = "Remove border";

            RenderImagesWindow(name, my_image_width4, my_image_height4, my_texture4);
        }

        // {
        //     StyleWindowImages();
            
        //     int my_image_width3 = 0;
        //     int my_image_height3 = 0;
        //     PDIRECT3DTEXTURE9 my_texture3 = NULL;
        //     bool ret3;
        //     ret3 = LoadTextureFromFile(UsessTI::pathes_after_save[removenoisy].c_str(), &my_texture3, &my_image_width3, &my_image_height3);
        //     IM_ASSERT(ret3);
        //     std::string name = "Remove noisy";

        //     RenderImagesWindow(name, my_image_width3, my_image_height3, my_texture3);
        // }

        {
            StyleWindowImages();
            
            int my_image_width2 = 0;
            int my_image_height2 = 0;
            PDIRECT3DTEXTURE9 my_texture2 = NULL;
            bool ret2;
            ret2 = LoadTextureFromFile(UsessTI::pathes_after_save[noisy].c_str(), &my_texture2, &my_image_width2, &my_image_height2);
            IM_ASSERT(ret2);
            std::string name = "Noisy";

            RenderImagesWindow(name, my_image_width2, my_image_height2, my_texture2);
        }        

        // {
        //     StyleWindowImages();
            
        //     int my_image_width1 = 0;
        //     int my_image_height1 = 0;
        //     PDIRECT3DTEXTURE9 my_texture1 = NULL;
        //     bool ret1;
        //     ret1 = LoadTextureFromFile(UsessTI::pathes_after_save[border].c_str(), &my_texture1, &my_image_width1, &my_image_height1);
        //     IM_ASSERT(ret1);
        //     std::string name = "Border";

        //     RenderImagesWindow(name, my_image_width1, my_image_height1, my_texture1);
        // }

        {
            StyleWindowImages();
            
            int my_image_width0 = 0;
            int my_image_height0 = 0;
            PDIRECT3DTEXTURE9 my_texture0 = NULL;
            bool ret0;
            ret0 = LoadTextureFromFile(UsessTI::pathes_after_save[original].c_str(), &my_texture0, &my_image_width0, &my_image_height0);
            IM_ASSERT(ret0);
            std::string name = "Original";

            RenderImagesWindow(name, my_image_width0, my_image_height0, my_texture0);
        }

    }
}



int main(int, char**){
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"Dear ImGui DirectX9" , nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX9", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, nullptr, nullptr, wc.hInstance, nullptr);

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;    
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsClassic();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    ImVec4 clear_color = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);


    bool done = false;
    while (!done)
    {
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            g_d3dpp.BackBufferWidth = g_ResizeWidth;
            g_d3dpp.BackBufferHeight = g_ResizeHeight;
            g_ResizeWidth = g_ResizeHeight = 0;
            ResetDevice();
        }

        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

    
        StyleSettingsWindow();
        ImGui::Begin("Window settings", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);        

        WindowSettingUI();

        ImGui::PopStyleVar();
        ImGui::End();
        

        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x*clear_color.w*255.0f), (int)(clear_color.y*clear_color.w*255.0f), (int)(clear_color.z*clear_color.w*255.0f), (int)(clear_color.w*255.0f));
        g_pd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(nullptr, nullptr, nullptr, nullptr);

        // Handle loss of D3D9 device
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);    
}



bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == nullptr)
        return false;

    // Create the D3DDevice
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = nullptr; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
