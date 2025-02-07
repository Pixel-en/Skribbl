//#include "UdpClient.h"
//#include <cstring>
//#include <iostream> // For error messages
//
//#pragma comment(lib, "ws2_32.lib")
//
//
//const char* SERVER_ADDRESS = "192.168.43.64";		// 接続先なんか適当にクラスメートとやって！
//// サーバのポート番号
//const unsigned short SERVER_PORT = 8888;
//
//UdpClient::UdpClient(GameObject* parent)
//    : GameObject(parent, "UdpClient"), sock(INVALID_SOCKET) {}
//
//UdpClient::~UdpClient() {
//    Close();
//}
//
//void UdpClient::Initialize() {
//    WSADATA wsa;
//    if (WSAStartup(MAKEWORD(2, 2), &wsa)) {
//        std::cerr << "WSAStartup failed." << std::endl;
//        return;
//    }
//
//    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
//    if (sock == INVALID_SOCKET) {
//        std::cerr << "Failed to create socket." << std::endl;
//        WSACleanup();
//        return;
//    }
//
//    unsigned long cmdarg = 0x01;
//    ioctlsocket(sock, FIONBIO, &cmdarg);
//
//    memset(&serverAddr, 0, sizeof(serverAddr));
//    serverAddr.sin_family = AF_INET;
//    serverAddr.sin_port = htons(SERVER_PORT);
//    inet_pton(AF_INET, SERVER_ADDRESS, &serverAddr.sin_addr.s_addr);
//}
//
//void UdpClient::Update() {
//    // Placeholder for any updates required during the game loop
//    XMINT2 cursor;
//    GetMousePoint(&cursor.x, &cursor.y);
//    SendCircleData(cursor.x, cursor.y, 3, GetColor(255, 152, 0));
//}
//
//void UdpClient::Draw()
//{
//
//}
//
//void UdpClient::SendCircleData(int x, int y, int size, int color) {
//    struct CIRCLE {
//        int centerX;
//        int centerY;
//        int size;
//        int color;
//    };
//
//    CIRCLE circle = { htonl(x), htonl(y), htonl(size), htonl(color) };
//    int ret = sendto(sock, reinterpret_cast<char*>(&circle), sizeof(circle), 0,
//        reinterpret_cast<SOCKADDR*>(&serverAddr), sizeof(serverAddr));
//
//    if (ret == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK) {
//        std::cerr << "sendto failed." << std::endl;
//    }
//}
//
//void UdpClient::Close() {
//    if (sock != INVALID_SOCKET) {
//        closesocket(sock);
//        WSACleanup();
//        sock = INVALID_SOCKET;
//    }
//}
