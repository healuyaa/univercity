#include "imgui.h"

#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx9.h"

#include <d3d9.h>
#include <tchar.h>

#include "HospitalSQLite.h"



static LPDIRECT3D9              g_pD3D = nullptr;
static LPDIRECT3DDEVICE9        g_pd3dDevice = nullptr;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);



const char* dbPath = "hospital.db";
HospitalDatabase hospitalDB(dbPath);
int InitSqlite3() {
    if (!hospitalDB.openDatabase()) {
        std::cerr << "Failed to open the database." << std::endl;
        return 1;
    }

    if (!hospitalDB.createTable()) {
        std::cerr << "Failed to create the 'Contacts' table." << std::endl;
        return 1;
    }
    return 0;
}


void StyleTableSqlite3() {
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
    
    ImGui::SetNextWindowSize(ImVec2(750, 800));
    ImGui::SetNextWindowPos(ImVec2(580, 10));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
}

void WindowTableSqlite3Top() {
    static int sortColumn = -1;
    static bool sortAscending = true;

    std::vector<std::vector<std::string>> allData = hospitalDB.selectAllPatients();
    std::vector<std::vector<std::string>> sortedData = allData;

    if (sortColumn >= 0 && sortColumn < static_cast<int>(sortedData[0].size())) {
        std::sort(sortedData.begin(), sortedData.end(), [=](const auto& a, const auto& b) {
            return a[sortColumn] < b[sortColumn];
        });
        if (!sortAscending)
            std::reverse(sortedData.begin(), sortedData.end());
    }

    ImGui::Columns(6, "ContactColumns", true);

    ImGui::SetColumnWidth(0, 130.5f);
    if (ImGui::Selectable("Full Name")) {
        sortColumn = 0;
        sortAscending = !sortAscending;
    }
    ImGui::NextColumn();

    ImGui::SetColumnWidth(1, 130.5f);
    if (ImGui::Selectable("Address")) {
        sortColumn = 1;
        sortAscending = !sortAscending;
    }
    ImGui::NextColumn();

    ImGui::SetColumnWidth(2, 130.5f);
    if (ImGui::Selectable("Phone number")) {
        sortColumn = 2;
        sortAscending = !sortAscending;
    }
    ImGui::NextColumn();

    ImGui::SetColumnWidth(3, 130.5f);
    if (ImGui::Selectable("Birthday")) {
        sortColumn = 3;
        sortAscending = !sortAscending;
    }
    ImGui::NextColumn();

    ImGui::SetColumnWidth(4, 130.5f);
    if (ImGui::Selectable("Date visit")) {
        sortColumn = 4;
        sortAscending = !sortAscending;
    }
    ImGui::NextColumn();

    ImGui::SetColumnWidth(5, 50.f);
    if (ImGui::Selectable("ID")) {
        sortColumn = 5;
        sortAscending = !sortAscending;
    }
    ImGui::NextColumn();

    ImGui::Separator();

    for (const auto& rowData : sortedData) {
        ImGui::Text("%s", rowData[0].c_str());
        ImGui::NextColumn();

        ImGui::Text("%s", rowData[1].c_str());
        ImGui::NextColumn();

        ImGui::Text("%s", rowData[2].c_str());
        ImGui::NextColumn();

        ImGui::Text("%s", rowData[3].c_str());
        ImGui::NextColumn();

        ImGui::Text("%s", rowData[4].c_str());
        ImGui::NextColumn();

        ImGui::Text("%s", rowData[5].c_str());
        ImGui::NextColumn();
    }

    ImGui::Columns(1);
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
    
    ImGui::SetNextWindowSize(ImVec2(560, 800));
    ImGui::SetNextWindowPos(ImVec2(10, 10));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
}

void WindowSettingUI() {
    static bool flag_add = false;
    static bool flag_remove = false;
    static bool flag_edit = false;
    static bool flag_search = false;
    static bool flag_export = false;

    static std::vector<char> fullNameBuffer;
    static std::vector<char> phoneNumberBuffer;
    static std::vector<char> addressBuffer;
    static std::vector<char> birthdayBuffer;
    static std::vector<char> dateVisitBuffer;
    static std::vector<char> searchBuffer;

    static int idBuffer;

    const size_t bufferSize = 256;
    fullNameBuffer.resize(bufferSize);
    phoneNumberBuffer.resize(bufferSize);
    addressBuffer.resize(bufferSize);
    birthdayBuffer.resize(bufferSize);
    searchBuffer.resize(bufferSize);
    dateVisitBuffer.resize(bufferSize);

    if(ImGui::Button("Add")) {
        flag_add = true;
    }

    ImGui::SameLine();
    if(ImGui::Button("Remove")) {
        flag_remove = true;
    }

    ImGui::SameLine();
    if(ImGui::Button("Edit")) {
        flag_edit = true;
    }

    ImGui::SameLine();
    if(ImGui::Button("Find")) {
        flag_search = true;
    }

    if(flag_add) {
        ImGui::BeginChild("Adding data", ImVec2(300, 170), true, ImGuiWindowFlags_HorizontalScrollbar);
        ImGui::Text("Adding data");

        ImGui::InputText("Full Name", fullNameBuffer.data(), bufferSize);
        ImGui::InputText("Phone number", phoneNumberBuffer.data(), bufferSize);
        ImGui::InputText("Address", addressBuffer.data(), bufferSize);
        ImGui::InputText("Birthday", birthdayBuffer.data(), bufferSize);
        ImGui::InputText("Date visit", dateVisitBuffer.data(), bufferSize);
        
        if(ImGui::Button("Confirm")) {
            if(!fullNameBuffer.empty() || !phoneNumberBuffer.empty() || !addressBuffer.empty() || !birthdayBuffer.empty() || !dateVisitBuffer.empty()) {   
                hospitalDB.insertPatient(fullNameBuffer.data(), phoneNumberBuffer.data(), addressBuffer.data(), birthdayBuffer.data(), dateVisitBuffer.data());

                fullNameBuffer.clear();
                phoneNumberBuffer.clear();
                addressBuffer.clear();
                birthdayBuffer.clear();
                dateVisitBuffer.clear();
            }
        }

        ImGui::SameLine();
        if(ImGui::Button("Close")) {
            flag_add = false;
        }

        ImGui::EndChild();
    }

    if(flag_remove) {
        ImGui::BeginChild("Remove data", ImVec2(300, 80), true, ImGuiWindowFlags_HorizontalScrollbar);
        ImGui::Text("Remove data");

        ImGui::InputInt("Type id", &idBuffer, 1, INT_MAX);
        if(idBuffer <= 0) {
            idBuffer = 1;
        }

        if(ImGui::Button("Confirm")) { 
            if(true) {
                hospitalDB.deletePatient(idBuffer);                

                idBuffer = 1;
            }
        }

        ImGui::SameLine();
        if(ImGui::Button("Close")) {
            flag_remove = false;
        }

        ImGui::EndChild();
    }

    if(flag_edit) {
        ImGui::BeginChild("Edding data", ImVec2(300, 190), true, ImGuiWindowFlags_HorizontalScrollbar);
        ImGui::Text("Edit data");

        ImGui::InputInt("Type id", &idBuffer, 1, INT_MAX);
        if(idBuffer <= 0) {
            idBuffer = 1;
        }
        ImGui::InputText("Full Name", fullNameBuffer.data(), bufferSize);
        ImGui::InputText("Phone number", phoneNumberBuffer.data(), bufferSize);
        ImGui::InputText("Address", addressBuffer.data(), bufferSize);
        ImGui::InputText("Birthday", birthdayBuffer.data(), bufferSize);
        ImGui::InputText("Date visit", dateVisitBuffer.data(), bufferSize);
        
        if(ImGui::Button("Confirm")) {
            if(!fullNameBuffer.empty() || !phoneNumberBuffer.empty() || !addressBuffer.empty() || !birthdayBuffer.empty() || !dateVisitBuffer.empty()) {   
                hospitalDB.updatePatientInfo(idBuffer, fullNameBuffer.data(), phoneNumberBuffer.data(), addressBuffer.data(), birthdayBuffer.data(), dateVisitBuffer.data());

                fullNameBuffer.clear();
                phoneNumberBuffer.clear();
                addressBuffer.clear();
                birthdayBuffer.clear();
                dateVisitBuffer.clear();
                idBuffer = 1;
            }
        }

        ImGui::SameLine();
        if(ImGui::Button("Close")) {
            flag_edit = false;
        }

        ImGui::EndChild();
    }

    if (flag_search) {
        ImGui::BeginChild("Find Window");
        ImGui::Text("Find data");

        ImGui::InputText("Search data", searchBuffer.data(), bufferSize);
        ImGui::SameLine();
        if (ImGui::Button("Close")) {
            flag_search = false;
        }

        ImGui::BeginChild("Finding data", ImVec2(540, 200), true, ImGuiWindowFlags_HorizontalScrollbar);

        ImGui::Columns(6, "ContactColumns", true);

        ImGui::SetColumnWidth(0, 100.0f);
        ImGui::Text("Full Name");
        ImGui::NextColumn();

        ImGui::SetColumnWidth(1, 100.0f);
        ImGui::Text("Phone number");
        ImGui::NextColumn();

        ImGui::SetColumnWidth(2, 100.0f);
        ImGui::Text("Address");
        ImGui::NextColumn();

        ImGui::SetColumnWidth(3, 100.0f);
        ImGui::Text("Birthday");
        ImGui::NextColumn();

        ImGui::SetColumnWidth(4, 100.0f);
        ImGui::Text("Date visit");
        ImGui::NextColumn();

        ImGui::SetColumnWidth(5, 40.0f);
        ImGui::Text("ID");
        ImGui::NextColumn();

        std::vector<std::vector<std::string>> searchResults = hospitalDB.searchPatients(searchBuffer.data());
        for (const auto& result : searchResults) {
            ImGui::Text("%s", result[0].c_str());
            ImGui::NextColumn();

            ImGui::Text("%s", result[1].c_str());
            ImGui::NextColumn();

            ImGui::Text("%s", result[2].c_str());
            ImGui::NextColumn();

            ImGui::Text("%s", result[3].c_str());
            ImGui::NextColumn();

            ImGui::Text("%s", result[4].c_str());
            ImGui::NextColumn();

            ImGui::Text("%s", result[5].c_str());
            ImGui::NextColumn();
        }

        ImGui::Columns(1);

        ImGui::EndChild();

        ImGui::EndChild();
    }
}


int main(int, char**){
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"Dear ImGui DirectX9" , nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX9", WS_OVERLAPPEDWINDOW, 200, 50, 1350, 900, nullptr, nullptr, wc.hInstance, nullptr);

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

        InitSqlite3();

        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

       
        StyleSettingsWindow();
        ImGui::Begin("Window settings", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

        WindowSettingUI();

        ImGui::PopStyleVar();
        ImGui::End();

        StyleTableSqlite3();
        ImGui::Begin("Table Window", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

        WindowTableSqlite3Top();

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
    hospitalDB.~HospitalDatabase();
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