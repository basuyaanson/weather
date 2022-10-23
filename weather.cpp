#include <iostream>
#include<stdio.h>
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
//#pragma warning(disable:4996)

//api.openweathermap.org/data/2.5/weather?q=Kaohsiung&appid=c3b68639ba154d1c5a9c0053e9a5819b&lang=zh_tw

const char* host = "api.openweathermap.org";//主機名
const char* key = "c3b68639ba154d1c5a9c0053e9a5819b";//授權碼
const char* loc = "Kaohsiung";//要查詢的城市編號 "ip"
char gurl[100] = { 0 };

//最終請求路徑
char* generateUrl(char* key, char* loc)
{
    sprintf(gurl, "GET /data/2.5/weather?q=%s&appid=%s&lang=zh_tw\r\n\r\n", loc,key );
    return gurl;
}

//初始化網路路徑
void initSocket()
{
    WSADATA data;
    if (0 != WSAStartup(MAKEWORD(2, 2), &data))
    {
        printf("初始化失敗");
    }

}

//連接伺服器
SOCKET connectTohost(const char* host,unsigned short port)
{
    //創建socket
    SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd == INVALID_SOCKET)
    {
        printf("創建socket失敗,%d\n", WSAGetLastError());
        return INVALID_SOCKET;
    }
    //準備ip地址和端口
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
 

    //DNS解析
    HOSTENT  *hostent = gethostbyname(host); 
    if (hostent)
    {
        memcpy(&addr.sin_addr, hostent->h_addr_list[0], sizeof(addr.sin_addr));
    }
    else
    {
        printf("DNS解析失敗");
        return INVALID_SOCKET;
    }

    //連接伺服器
    if(-1 == connect(fd, (struct sockaddr*)&addr, sizeof(addr)));
    {
        printf("連接失敗,%d\n", WSAGetLastError());
        return INVALID_SOCKET;
    }
    return fd;
}



int main()
{    
    
    char* key1 = (char*)key;
    char* loc1 = (char*)loc;
  //  char* url = generateUrl(key1, loc1);
    initSocket();

   // printf("%s", url);

    //連接伺服器
    SOCKET fd = connectTohost(host, 80);
   //SOCKET fd = connectTohost("178.128.122.9", 80); 
    if (fd != INVALID_SOCKET)
    {
        printf("連接成功\n");
    }


    //發送請求
   char * url = generateUrl(key1,loc1);
   if (0 > send(fd, url, strlen(url),0))
   {
       printf("發送失敗,%d\n",WSAGetLastError());
   }
   else
   {
       //接收數據
       char buf[BUFSIZ] = { 0 };
       if (0 >= recv(fd, buf, BUFSIZ, 0))
       {
           printf("接收失敗,%d", WSAGetLastError());
       }
       else
       {
           puts(buf);
       }

   }
   closesocket(fd);
    return 0;

}
