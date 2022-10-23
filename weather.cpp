#include <iostream>
#include<stdio.h>
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
//#pragma warning(disable:4996)

//api.openweathermap.org/data/2.5/weather?q=Kaohsiung&appid=c3b68639ba154d1c5a9c0053e9a5819b&lang=zh_tw

const char* host = "api.openweathermap.org";//�D���W
const char* key = "c3b68639ba154d1c5a9c0053e9a5819b";//���v�X
const char* loc = "Kaohsiung";//�n�d�ߪ������s�� "ip"
char gurl[100] = { 0 };

//�̲׽ШD���|
char* generateUrl(char* key, char* loc)
{
    sprintf(gurl, "GET /data/2.5/weather?q=%s&appid=%s&lang=zh_tw\r\n\r\n", loc,key );
    return gurl;
}

//��l�ƺ������|
void initSocket()
{
    WSADATA data;
    if (0 != WSAStartup(MAKEWORD(2, 2), &data))
    {
        printf("��l�ƥ���");
    }

}

//�s�����A��
SOCKET connectTohost(const char* host,unsigned short port)
{
    //�Ы�socket
    SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd == INVALID_SOCKET)
    {
        printf("�Ы�socket����,%d\n", WSAGetLastError());
        return INVALID_SOCKET;
    }
    //�ǳ�ip�a�}�M�ݤf
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
 

    //DNS�ѪR
    HOSTENT  *hostent = gethostbyname(host); 
    if (hostent)
    {
        memcpy(&addr.sin_addr, hostent->h_addr_list[0], sizeof(addr.sin_addr));
    }
    else
    {
        printf("DNS�ѪR����");
        return INVALID_SOCKET;
    }

    //�s�����A��
    if(-1 == connect(fd, (struct sockaddr*)&addr, sizeof(addr)));
    {
        printf("�s������,%d\n", WSAGetLastError());
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

    //�s�����A��
    SOCKET fd = connectTohost(host, 80);
   //SOCKET fd = connectTohost("178.128.122.9", 80); 
    if (fd != INVALID_SOCKET)
    {
        printf("�s�����\\n");
    }


    //�o�e�ШD
   char * url = generateUrl(key1,loc1);
   if (0 > send(fd, url, strlen(url),0))
   {
       printf("�o�e����,%d\n",WSAGetLastError());
   }
   else
   {
       //�����ƾ�
       char buf[BUFSIZ] = { 0 };
       if (0 >= recv(fd, buf, BUFSIZ, 0))
       {
           printf("��������,%d", WSAGetLastError());
       }
       else
       {
           puts(buf);
       }

   }
   closesocket(fd);
    return 0;

}
