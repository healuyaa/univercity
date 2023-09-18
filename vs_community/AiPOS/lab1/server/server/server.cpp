#include <iostream>
#include <string>
#include <fstream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    // Инициализация библиотеки Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Ошибка инициализации Winsock" << endl;
        return 1;
    }

    // Создание сокета
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "Ошибка создания сокета" << endl;
        WSACleanup();
        return 1;
    }

    // Установка параметров сервера
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080); // Здесь можно указать желаемый порт

    // Привязка сокета к адресу и порту
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Ошибка привязки сокета" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Ожидание клиентских подключений
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        cerr << "Ошибка ожидания клиентских подключений" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    cout << "Сервер ожидает подключений на порту 12345" << endl;

    while (true) {
        // Принятие клиентского подключения
        SOCKET clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) {
            cerr << "Ошибка приема клиентского подключения" << endl;
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }

        char buffer[256];
        int bytesRead;

        // Получение команды от клиента
        bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead == SOCKET_ERROR) {
            cerr << "Ошибка приема данных от клиента" << endl;
            closesocket(clientSocket);
            continue;
        }

        buffer[bytesRead] = '\0';

        // Разбор команды от клиента
        string command(buffer);
        size_t spacePos = command.find(' ');
        if (spacePos != string::npos) {
            string cmd = command.substr(0, spacePos);
            string filename = command.substr(spacePos + 1);

            if (cmd == "load") {
                ifstream file(filename);
                if (file.is_open()) {
                    string fileContent((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
                    file.close();

                    // Отправка содержимого файла клиенту
                    if (send(clientSocket, fileContent.c_str(), fileContent.size(), 0) == SOCKET_ERROR) {
                        cerr << "Ошибка отправки файла клиенту" << endl;
                    }
                }
                else {
                    // Файл не найден, отправляем сообщение об ошибке
                    string errorMessage = "Файл не найден.";
                    if (send(clientSocket, errorMessage.c_str(), errorMessage.size(), 0) == SOCKET_ERROR) {
                        cerr << "Ошибка отправки сообщения об ошибке клиенту" << endl;
                    }
                }
            }
        }

        // Закрытие клиентского соединения
        closesocket(clientSocket);
    }

    // Закрытие серверного сокета и очистка Winsock
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
