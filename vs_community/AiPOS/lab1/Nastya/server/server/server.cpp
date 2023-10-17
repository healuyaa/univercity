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

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
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
        cerr << "error link socket" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    } // привязка сокета к адресному порту

    if (listen(serverSocket, 5) == SOCKET_ERROR) {
        cerr << "error listen socket" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    cout << "server rdy..." << endl;

    while (true) {
        SOCKET clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) {
            cerr << "error connect to socket" << endl;
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }

        char buffer[1024];
        int bytesRead;

        bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead == SOCKET_ERROR) {
            cerr << "error complete command from client" << endl;
            closesocket(clientSocket);
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
                send(clientSocket, fileContent.c_str(), fileContent.length(), 0);
            }
            else {
                string errorMessage = "error file message";
                send(clientSocket, errorMessage.c_str(), errorMessage.length(), 0);
            }
        }
        else {
            string errorMessage = "undefined command";
            send(clientSocket, errorMessage.c_str(), errorMessage.length(), 0);
        }

        closesocket(clientSocket);
    }

    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
