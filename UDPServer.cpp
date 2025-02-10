#include "UDPServer.h"

inline bool operator == (const IPDATA& a, const IPDATA& b) {
    if (a.d1 == b.d1 && a.d2 == b.d2 && a.d3 == b.d3 && a.d4 == b.d4)return  true;
    return false;

};

UDPServer::UDPServer(GameObject* parent)
    :GameObject(parent,"UDPServer")
{
}

UDPServer::~UDPServer()
{
}

void UDPServer::Initialize()
{   
    // UDP通信用のソケットハンドルを作成
    NetUDPHandle = MakeUDPSocket(8888);

    // 受信待ち表示
    IPDATA ip, ip2;
    int count = 0;
    int port;
    while (true) {
        DrawString(0, 0, "受信待ち", GetColor(255, 255, 255));
        if (NetWorkRecvUDP(NetUDPHandle, &ip2, &port, &ip, sizeof(ip), true) >= 0) {
            net[count].IPAddr = ip2;
            net[count].port = port;
            count++;
            if (count == 2)
                break;
        }
    }
    int num = 1;
    for (int i = 0; i < 2; i++) {
        NetWorkSendUDP(NetUDPHandle, net[i].IPAddr, net[i].port, &num, sizeof(num));
    }

}

void UDPServer::Update()
{
    if (CheckNetWorkRecvUDP(NetUDPHandle) == TRUE) {
        IPDATA ip;
        int port;
        circle cir;
        for (int i = 0; i < 2; i++) {
            NetWorkRecvUDP(NetUDPHandle, &ip, &port, &cir, sizeof(cir), false);
            if (ip == net[i].IPAddr) {
                receivedCircle[i] = cir;
            }
        }
    }
    else {
        //if (ProcessMessage() < 0) {
        //    DeleteUDPSocket(NetUDPHandle);
        //}
    }
}

void UDPServer::Draw()
{    // Clear the screen
    ClearDrawScreen();

    // Check if circle data is received

    for (int i = 0; i < 1; i++) {

        DrawCircle(receivedCircle[i].x, receivedCircle[i].y, receivedCircle[i].size, receivedCircle[i].color, true);

    }
    // Flip the screen
    ScreenFlip();
}

void UDPServer::Release()
{    // Clean up resources
    DeleteUDPSocket(NetUDPHandle);
}
