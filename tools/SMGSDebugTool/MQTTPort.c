/*******************************************************************************
 * Copyright (c) 2014, 2017 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Allan Stockdill-Mander - initial API and implementation and/or initial documentation
 *    Ian Craggs - return codes from linux_read
 *******************************************************************************/

#include "MQTTPort.h"

#ifdef WIN32
#include <Winsock2.h>
#include <Windows.h>
#include <time.h>
#include <WS2tcpip.h>
#else
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#endif // WIN32

#ifdef WIN32
int gettimeofday(struct timeval *tp, void *tzp)
{
    time_t clock;
    struct tm tm;
    SYSTEMTIME wtm;
    GetLocalTime(&wtm);
    tm.tm_year   = wtm.wYear - 1900;
    tm.tm_mon   = wtm.wMonth - 1;
    tm.tm_mday   = wtm.wDay;
    tm.tm_hour   = wtm.wHour;
    tm.tm_min   = wtm.wMinute;
    tm.tm_sec   = wtm.wSecond;
    tm. tm_isdst  = -1;
    clock = mktime(&tm);
    tp->tv_sec = clock;
    tp->tv_usec = wtm.wMilliseconds * 1000;
    return (0);
}

void timersub(struct timeval *a, struct timeval *b, struct timeval *res)
{
    res->tv_sec=a->tv_sec-b->tv_sec;
    res->tv_usec=a->tv_usec-b->tv_usec;
    if(res->tv_usec<0 && res->tv_sec>0)
    {
        res->tv_sec--;
        res->tv_usec+=1E6;
    }
}

void timeradd(struct timeval *a, struct timeval *b, struct timeval *res)
{
    res->tv_sec=a->tv_sec+b->tv_sec;
    res->tv_usec=a->tv_usec+b->tv_usec;
    if(res->tv_usec>=1E6)
    {
        res->tv_usec-=1E6;
        res->tv_sec++;
    }
    if(res->tv_usec<0 && res->tv_sec>0)
    {
        res->tv_sec--;
        res->tv_usec+=1E6;
    }

}

#endif

void TimerInit(Timer* timer)
{
    timer->end_time = (struct timeval)
    {
        0, 0
    };
}

char TimerIsExpired(Timer* timer)
{
    struct timeval now, res;
    gettimeofday(&now, NULL);
    timersub(&timer->end_time, &now, &res);
    return res.tv_sec < 0 || (res.tv_sec == 0 && res.tv_usec <= 0);
}


void TimerCountdownMS(Timer* timer, unsigned int timeout)
{
    struct timeval now;
    gettimeofday(&now, NULL);
    struct timeval interval = {timeout / 1000, (timeout % 1000) * 1000};
    timeradd(&now, &interval, &timer->end_time);
}


void TimerCountdown(Timer* timer, unsigned int timeout)
{
    struct timeval now;
    gettimeofday(&now, NULL);
    struct timeval interval = {timeout, 0};
    timeradd(&now, &interval, &timer->end_time);
}


int TimerLeftMS(Timer* timer)
{
    struct timeval now, res;
    gettimeofday(&now, NULL);
    timersub(&timer->end_time, &now, &res);
    //printf("left %d ms\n", (res.tv_sec < 0) ? 0 : res.tv_sec * 1000 + res.tv_usec / 1000);
    return (res.tv_sec < 0) ? 0 : res.tv_sec * 1000 + res.tv_usec / 1000;
}

#ifndef WIN32

int linux_read(Network* n, unsigned char* buffer, int len, int timeout_ms)
{
    struct timeval interval = {timeout_ms / 1000, (timeout_ms % 1000) * 1000};
    if (interval.tv_sec < 0 || (interval.tv_sec == 0 && interval.tv_usec <= 0))
    {
        interval.tv_sec = 0;
        interval.tv_usec = 100;
    }

    setsockopt(n->my_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&interval, sizeof(struct timeval));

    int bytes = 0;
    while (bytes < len)
    {
        int rc = recv(n->my_socket, &buffer[bytes], (size_t)(len - bytes), 0);
        if (rc == -1)
        {
            if (errno != EAGAIN && errno != EWOULDBLOCK)
                bytes = -1;
            break;
        }
        else if (rc == 0)
        {
            bytes = 0;
            break;
        }
        else
            bytes += rc;
    }
    return bytes;
}


int linux_write(Network* n, unsigned char* buffer, int len, int timeout_ms)
{
    struct timeval tv;

    tv.tv_sec = 0;  /* 30 Secs Timeout */
    tv.tv_usec = timeout_ms * 1000;  // Not init'ing this can cause strange errors

    setsockopt(n->my_socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv,sizeof(struct timeval));
    int	rc = write(n->my_socket, buffer, len);
    return rc;
}


void NetworkInit(Network* n)
{
    n->my_socket = 0;
    n->mqttread = linux_read;
    n->mqttwrite = linux_write;
    n->disconnect=NetworkDisconnect;
}


int NetworkConnect(Network* n, char* addr, int port)
{
    int type = SOCK_STREAM;
    struct sockaddr_in address;
    int rc = -1;
    sa_family_t family = AF_INET;
    struct addrinfo *result = NULL;
    struct addrinfo hints = {0, AF_UNSPEC, SOCK_STREAM, IPPROTO_TCP, 0, NULL, NULL, NULL};

    if ((rc = getaddrinfo(addr, NULL, &hints, &result)) == 0)
    {
        struct addrinfo* res = result;

        /* prefer ip4 addresses */
        while (res)
        {
            if (res->ai_family == AF_INET)
            {
                result = res;
                break;
            }
            res = res->ai_next;
        }

        if (result->ai_family == AF_INET)
        {
            address.sin_port = htons(port);
            address.sin_family = family = AF_INET;
            address.sin_addr = ((struct sockaddr_in*)(result->ai_addr))->sin_addr;
        }
        else
            rc = -1;

        freeaddrinfo(result);
    }

    if (rc == 0)
    {
        n->my_socket = socket(family, type, 0);
        if (n->my_socket != -1)
            rc = connect(n->my_socket, (struct sockaddr*)&address, sizeof(address));
        else
            rc = -1;
    }

    return rc;
}


void NetworkDisconnect(Network* n)
{
    close(n->my_socket);
}

#else

int win32_read(Network* n, unsigned char* buffer, int len, int timeout_ms)
{
    //TODO 处理超时
    return recv(n->my_socket,buffer,len,0);
}

int win32_write(Network* n, unsigned char* buffer, int len, int timeout_ms)
{
    //TODO 处理超时
    return send(n->my_socket,buffer,len,0);
}

static bool IsWSAStartUp=false;
static WSADATA wsaData;
static WORD wVersionRequested;
void NetworkInit(Network* n)
{
    if(!IsWSAStartUp)
    {
        wVersionRequested = MAKEWORD( 2, 2 );
        IsWSAStartUp=(0==WSAStartup( wVersionRequested, &wsaData ));
    }
    n->my_socket = INVALID_SOCKET;
    n->mqttread = win32_read;
    n->mqttwrite = win32_write;
    n->disconnect=NetworkDisconnect;
}

int NetworkConnect(Network* n, char* host, int port)
{
    int rc=-1;


    struct addrinfo hints;
    struct addrinfo *res, *cur;
    struct sockaddr_in *addr=NULL;

    //初始化 hints
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;	//IPv4
    hints.ai_flags = AI_PASSIVE; //匹配所有 IP 地址
    hints.ai_protocol = 0;        //匹配所有协议
    hints.ai_socktype = SOCK_STREAM; //流类型

    if(getaddrinfo(host, NULL, &hints, &res) < 0)
    {
        return rc;
    }

    //输出获取的信息
    for (cur = res; cur != NULL; cur = cur->ai_next)
    {
        addr = (struct sockaddr_in *) cur->ai_addr; //获取当前 address
    }

    if(addr==NULL)
    {
        return rc;
    }

    struct sockaddr_in serveraddr= {0};

    serveraddr.sin_port=htons(port);
    serveraddr.sin_addr=addr->sin_addr;
    serveraddr.sin_family=AF_INET;

    freeaddrinfo(res);

    n->my_socket=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(n->my_socket==INVALID_SOCKET)
    {
        return rc;
    }

    rc=((SOCKET_ERROR==connect(n->my_socket,(struct sockaddr *)&serveraddr,sizeof(serveraddr)))?-1:0);


    return rc;
}

void NetworkDisconnect(Network* n)
{
    closesocket(n->my_socket);
}


#endif
