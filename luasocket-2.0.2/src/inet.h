#ifndef INET_H 
#define INET_H 
/*=========================================================================*\
* Internet domain functions
* LuaSocket toolkit
*
* This module implements the creation and connection of internet domain
* sockets, on top of the socket.h interface, and the interface of with the
* resolver. 
*
* The function inet_aton is provided for the platforms where it is not
* available. The module also implements the interface of the internet
* getpeername and getsockname functions as seen by Lua programs.
*
* The Lua functions toip and tohostname are also implemented here.
*
* RCS ID: $Id: inet.h,v 1.16 2005/10/07 04:40:59 diego Exp $
\*=========================================================================*/
#include "lua.h"
#include "socket.h"
#include "timeout.h"

#ifdef _WIN32
#define INET_ATON
#endif

int inet_open(lua_State *L);

const char *inet_trycreate(p_socket ps, int type);
const char *inet_tryconnect(p_socket ps, const char *address, 
        unsigned short port, p_timeout tm);
const char *inet_trybind(p_socket ps, const char *address, 
        unsigned short port);

int inet_meth_getpeername(lua_State *L, p_socket ps);
int inet_meth_getsockname(lua_State *L, p_socket ps);

#ifdef INET_ATON
int inet_aton(const char *cp, struct in_addr *inp);
#endif



typedef unsigned short int uint16;  
typedef unsigned long int uint32; 

// 短整型大小端互换  
#define BigLittleSwap16(A)  ((((uint16)(A) & 0xff00) >> 8) | (((uint16)(A) & 0x00ff) << 8))  

// 长整型大小端互换  
#define BigLittleSwap32(A)  ((((uint32)(A) & 0xff000000) >> 24) | (((uint32)(A) & 0x00ff0000) >> 8) | (((uint32)(A) & 0x0000ff00) << 8) | (((uint32)(A) & 0x000000ff) << 24)) 

// 本机大端返回1，小端返回0  
static int checkCPUendian()  
{  
    union{  
        unsigned long int i;  
        unsigned char s[4];  
    }c;  
      
    c.i = 0x12345678;  
    return (0x12 == c.s[0]);  
} 

// 模拟htonl函数，本机字节序转网络字节序  
static unsigned long int t_htonl(unsigned long int h)  
{  
    // 若本机为大端，与网络字节序同，直接返回  
    // 若本机为小端，转换成大端再返回  
    return checkCPUendian() ? h : BigLittleSwap32(h);  
} 

// 模拟ntohl函数，网络字节序转本机字节序  
static unsigned long int t_ntohl(unsigned long int n)  
{  
    // 若本机为大端，与网络字节序同，直接返回  
    // 若本机为小端，网络数据转换成小端再返回  
    return checkCPUendian() ? n : BigLittleSwap32(n);  
} 

// 模拟htons函数，本机字节序转网络字节序  
static unsigned short int t_htons(unsigned short int h)  
{  
    // 若本机为大端，与网络字节序同，直接返回  
    // 若本机为小端，转换成大端再返回  
    return checkCPUendian() ? h : BigLittleSwap16(h);  
}  
  
// 模拟ntohs函数，网络字节序转本机字节序  
static unsigned short int t_ntohs(unsigned short int n)  
{  
    // 若本机为大端，与网络字节序同，直接返回  
    // 若本机为小端，网络数据转换成小端再返回  
    return checkCPUendian() ? n : BigLittleSwap16(n);  
} 


#endif /* INET_H */
