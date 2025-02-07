#include "UdpServer.h"
#include <cstring>
#include "DxLib.h"
#include <iostream> // For error messages

#pragma comment(lib, "ws2_32.lib")

const unsigned short SERVER_PORT = 8888;

struct CIRCLE {
    int centerX;
    int centerY;
    int size;
    int color;
};

UdpServer::UdpServer(GameObject* parent)
    : GameObject(parent, "UdpServer"), sock(INVALID_SOCKET) {}

UdpServer::~UdpServer() {
    Close();
}

void UdpServer::Initialize() {
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

    SOCKADDR_IN bindAddr;
    memset(&bindAddr, 0, sizeof(bindAddr));
    bindAddr.sin_family = AF_INET;
    bindAddr.sin_port = htons(SERVER_PORT);
    bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sock, (SOCKADDR*)&bindAddr, sizeof(bindAddr)) == SOCKET_ERROR) {
        std::cerr << "Failed to bind socket." << std::endl;
        Close();
        return;
    }

    SetWindowText("ƒT[ƒo");
    SetGraphMode(800, 600, 32);
    ChangeWindowMode(TRUE);
    if (DxLib_Init() == -1) return;
    SetBackgroundColor(0, 0, 0);
    SetDrawScreen(DX_SCREEN_BACK);
    SetAlwaysRunFlag(1);
}

void UdpServer::Update() {
    ClearDrawScreen();

    circle = { -100, -100, 0, GetColor(255, 255, 255) };
    sockaddr_in clientAddr;
    int fromlen = sizeof(clientAddr);

    CIRCLE value;
    int ret = recvfrom(sock, reinterpret_cast<char*>(&value), sizeof(value), 0,
        reinterpret_cast<sockaddr*>(&clientAddr), &fromlen);
    if (ret > 0) {
        circle.centerX = ntohl(value.centerX);
        circle.centerY = ntohl(value.centerY);
        circle.size = ntohl(value.size);
        circle.color = ntohl(value.color);
    }
    else if (WSAGetLastError() != WSAEWOULDBLOCK) {
        std::cerr << "recvfrom failed." << std::endl;
    }

    ScreenFlip();
    WaitTimer(16);

    if (ProcessMessage() == -1 || CheckHitKey(KEY_INPUT_ESCAPE) == 1) {
        Close();
    }
}

void UdpServer::Draw()
{
    DrawCircle(circle.centerX, circle.centerY, circle.size, circle.color, 1);
}

void UdpServer::Close() {
    if (sock != INVALID_SOCKET) {
        closesocket(sock);
        WSACleanup();
        sock = INVALID_SOCKET;
    }
}
