#pragma once
#include "Engine/GameObject.h"
#include <WinSock2.h>
#include <WS2tcpip.h>

class UdpClient : public GameObject {
public:
    UdpClient(GameObject* parent);
    ~UdpClient();

    void Initialize(const char* serverAddress, unsigned short serverPort);
    void Update() override;
    void SendCircleData(int x, int y, int size, int color);
    void Close();

private:
    SOCKET sock;
    SOCKADDR_IN serverAddr;
    const unsigned int MAX_MESSAGE_LENGTH = 1024;
};
