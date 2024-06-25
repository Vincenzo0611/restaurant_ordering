#include "Polaczenie.h"


Polaczenie::Polaczenie()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "B³¹d inicjalizacji biblioteki Winsock." << std::endl;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "B³¹d tworzenia gniazda." << std::endl;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Mo¿e nas³uchiwaæ na dowolnym adresie IP
    serverAddress.sin_port = htons(12345); // Dowolny numer portu

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        wxMessageBox("B³¹d podczas przypisywania adresu do gniazda.");
        closesocket(serverSocket);
    }

    if (listen(serverSocket, 1) == SOCKET_ERROR) {
        wxMessageBox("B³¹d nas³uchiwania na gnieŸdzie.");
        closesocket(serverSocket);
    }


    char hostname[256];
    gethostname(hostname, sizeof(hostname));
    struct hostent* host;
    host = gethostbyname(hostname);

    if (host == nullptr) {
        std::cerr << "B³¹d uzyskiwania adresu IP." << std::endl;
    }

    struct in_addr** addr_list = (struct in_addr**)host->h_addr_list;

    if (addr_list[0] == nullptr) {
        std::cerr << "Brak dostêpnych adresów IP." << std::endl;
    }

    // ZnajdŸ ostatni adres IP w liœcie
    struct in_addr* last_addr = nullptr;
    for (int i = 0; addr_list[i] != nullptr; ++i) {
        last_addr = addr_list[i];
    }

    if (last_addr == nullptr) {
        std::cerr << "Nie mo¿na znaleŸæ ostatniego adresu IP." << std::endl;
    }

    wxMessageBox(inet_ntoa(*last_addr));
    sockaddr_in clientAddress;
    int clientAddressSize = sizeof(clientAddress);

    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressSize);
    if (clientSocket == INVALID_SOCKET) {
        wxMessageBox("B³¹d podczas akceptowania po³¹czenia.");
        closesocket(serverSocket);
    }
    closesocket(serverSocket);
}

Polaczenie::~Polaczenie() {
    closesocket(clientSocket);
    WSACleanup();
}
