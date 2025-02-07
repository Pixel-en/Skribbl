#pragma once
#include "Engine/GameObject.h"
#include <WinSock2.h>
#include <WS2tcpip.h>

class UdpServer : public GameObject {
public:
    UdpServer(GameObject* parent);
    ~UdpServer();

    void Initialize(unsigned short serverPort);
    void Update() override;
    void Close();

private:
    SOCKET sock;
    const unsigned int MAX_MESSAGE_LENGTH = 1024;
};
