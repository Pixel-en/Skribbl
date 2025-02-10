#include "UDPServer.h"

inline bool operator == (const IPDATA& a, const IPDATA& b) {
    if (a.d1 == b.d1 && a.d2 == b.d2 && a.d3 == b.d3 && a.d4 == b.d4)return  true;
    return false;

};

UDPServer::UDPServer(GameObject* parent)
    :GameObject(parent,"UDPServer")
{
    circle0_ = { -10,-10,5,GetColor(255,0,0) };
    UDPHandle_[0] = MakeUDPSocket(8888);
    UDPHandle_[1] = MakeUDPSocket(8888);
    IP_[0] = { 0,0,0,0 };
    IP_[1] = { 0,0,0,0 };

    assert(UDPHandle_ >= 0);
}

UDPServer::~UDPServer()
{
}

void UDPServer::Initialize()
{
   
 

}

void UDPServer::Update()
{
 int port;
    IPDATA ip;
    GetMousePoint(&circle0_.x,&circle0_.y);
    for (int i; i < 2; i++) {
        if (CheckNetWorkRecvUDP(UDPHandle_[i]) == TRUE)
        {
            if(IP_[i]==IPDATA{0,0,0,0})
            {
                for (int j; j < 2; j++) {
                    if(IP_[j]==IPDATA{})
           }
            }
            NetWorkRecvUDP(UDPHandle_[i], &ip, &port, &circle_, sizeof(circle_), FALSE);

            NetWorkSendUDP(UDPHandle_[i], ip, port, &circle0_, sizeof(circle0_));
        }
    }
}

void UDPServer::Draw()
{
    ClearDrawScreen();
    for (int i; i < 2; i++) {
        DrawCircle(circle_[i].x, circle_[i].y, circle_[i].size, circle_[i].color);
    }
    DrawCircle(circle0_.x, circle0_.y, circle0_.size, circle0_.color);

}

void UDPServer::Release()
{    // Clean up resources
    for(int i;i<2;i++)
    DeleteUDPSocket(UDPHandle_[i]);
}
