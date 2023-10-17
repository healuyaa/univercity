﻿#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <winsock2.h>
#include <conio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")

using namespace std;

void LogEvent(const string& event, ofstream& logfile) {
    time_t currentTime = time(0);
    char* timestamp = ctime(&currentTime);
    timestamp[strlen(timestamp) - 1] = '\0';

    logfile << "[" << timestamp << "] " << event << endl;
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Error init Winsock" << endl;
        return 1;
    }

    string serverAddress = "127.0.0.1";
    int serverPort = 8192;

    cout << "input command (connect <ip> <port>): ";
    string input;
    getline(cin, input);

    if (input.find("connect ") == 0) {
        size_t spacePos1 = input.find(" ");
        if (spacePos1 != string::npos) {
            size_t spacePos2 = input.find(" ", spacePos1 + 1);
            if (spacePos2 != string::npos) {
                serverAddress = input.substr(spacePos1 + 1, spacePos2 - spacePos1 - 1);
                serverPort = stoi(input.substr(spacePos2 + 1));
            }
        }
    }
    else {
        cerr << "Error input" << endl;
        WSACleanup();
        return 1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "Error create socket" << endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddressInfo;
    serverAddressInfo.sin_family = AF_INET;
    serverAddressInfo.sin_port = htons(serverPort);
    serverAddressInfo.sin_addr.s_addr = inet_addr(serverAddress.c_str());

    cout << "Input string to send: ";

    input = "";
    while (true) {
        unsigned char c = _getch();
        if (c == 224 || c == 13)
            break;
        if (c > 31)
        {
            input += c;
            std::cout << c;
        }
    }

    if (sendto(clientSocket, input.c_str(), input.length(), 0, (struct sockaddr*)&serverAddressInfo, sizeof(serverAddressInfo)) == SOCKET_ERROR) {
        cerr << "error send" << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    ofstream logfile("log.txt", ios::app);
    if (!logfile.is_open()) {
        cerr << "Error open log file" << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    LogEvent("send string to server: " + input, logfile);

    char buffer[1024];
    int bytesRead;
    int serverAddrLen = sizeof(serverAddressInfo);
    bytesRead = recvfrom(clientSocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&serverAddressInfo, &serverAddrLen);
    if (bytesRead == SOCKET_ERROR) {
        cerr << "error receive" << endl;
    }
    else {
        std::cout << std::endl;
        for (size_t i{}; i < bytesRead; ++i)
        {
            std::cout << buffer[i];
        }
        std::cout << std::endl;

        buffer[bytesRead] = '\0';

        LogEvent("receive from server: " + string(buffer), logfile);
    }

    closesocket(clientSocket);
    WSACleanup();
    logfile.close();

    return 0;
}
