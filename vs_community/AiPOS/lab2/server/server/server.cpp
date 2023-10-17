#include <iostream>
#include <fstream>
#include <string>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Error init Winsock" << endl;
        return 1;
    } // Инициализация сокета

    SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "Error create socket" << endl;
        WSACleanup();
        return 1;
    } // Создание сокета

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8192);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        cerr << "error bind socket" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    } // привязка сокета к адресному порту

    cout << "server rdy..." << endl;

    while (true) {
        sockaddr_in clientAddress;
        int clientAddrLen = sizeof(clientAddress);
        char buffer[1024];
        int bytesRead;

        bytesRead = recvfrom(serverSocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddress, &clientAddrLen);
        if (bytesRead == SOCKET_ERROR) {
            cerr << "error receiving data from client" << endl;
            continue;
        }

        buffer[bytesRead] = '\0';
        string command(buffer);

        if (command.find("load ") == 0) { // по "load " открывает файл
            string fileName = command.substr(5);

            ifstream file(fileName);
            if (file.good()) {
                string fileContent((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
                file.close();

                sendto(serverSocket, fileContent.c_str(), fileContent.length(), 0,
                    (struct sockaddr*)&clientAddress, sizeof(clientAddress));
            }
            else {
                string errorMessage = "error file message";
                sendto(serverSocket, errorMessage.c_str(), errorMessage.length(), 0,
                    (struct sockaddr*)&clientAddress, sizeof(clientAddress));
            }
        }
        else {
            string errorMessage = "undefined command";
            sendto(serverSocket, errorMessage.c_str(), errorMessage.length(), 0,
                (struct sockaddr*)&clientAddress, sizeof(clientAddress));
        }
    }

    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
