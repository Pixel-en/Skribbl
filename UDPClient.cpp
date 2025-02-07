#include "UdpClient.h"
#include <cstring>
#include <iostream> // For error messages

#pragma comment(lib, "ws2_32.lib")

UdpClient::UdpClient(GameObject* parent)
    : GameObject(parent, "UdpClient"), sock(INVALID_SOCKET) {}

UdpClient::~UdpClient() {
    Close();
}

void UdpClient::Initialize(const char* serverAddress, unsigned short serverPort) {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa)) {
        std::cerr << "WSAStartup failed." << std::endl;
        return;
    }

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Failed to create socket." << std::endl;
        WSACleanup();
        return;
    }

    unsigned long cmdarg = 0x01;
    ioctlsocket(sock, FIONBIO, &cmdarg);

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    inet_pton(AF_INET, serverAddress, &serverAddr.sin_addr.s_addr);
}

void UdpClient::Update() {
    // Placeholder for any updates required during the game loop
}

void UdpClient::SendCircleData(int x, int y, int size, int color) {
    struct CIRCLE {
        int centerX;
        int centerY;
        int size;
        int color;
    };

    CIRCLE circle = { htonl(x), htonl(y), htonl(size), htonl(color) };
    int ret = sendto(sock, reinterpret_cast<char*>(&circle), sizeof(circle), 0,
        reinterpret_cast<SOCKADDR*>(&serverAddr), sizeof(serverAddr));

    if (ret == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK) {
        std::cerr << "sendto failed." << std::endl;
    }
}

void UdpClient::Close() {
    if (sock != INVALID_SOCKET) {
        closesocket(sock);
        WSACleanup();
        sock = INVALID_SOCKET;
    }
}
