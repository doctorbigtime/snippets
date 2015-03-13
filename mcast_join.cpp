#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define check(expr) {if(0>(expr))perror(#expr),exit(1);}

extern int main( int argc, char** argv )
{
   int sock;
   check( sock = ::socket( AF_INET, SOCK_DGRAM, 0 ) );

   int reuse = 1;
   check( setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,(char*)&reuse,sizeof(reuse)) );

   sockaddr_in local;
   ::memset( &local, 0, sizeof(local) );
   local.sin_family = AF_INET;
   local.sin_addr.s_addr = INADDR_ANY;
   local.sin_port = htons( atoi(argv[2]) );
   check( bind(sock,reinterpret_cast<sockaddr*>(&local),sizeof(local)) );

   ip_mreq group;
   group.imr_multiaddr.s_addr = inet_addr( argv[1] );
   group.imr_interface.s_addr = argc > 3 ? inet_addr(argv[3]) : INADDR_ANY;
   check( setsockopt(sock,IPPROTO_IP,IP_ADD_MEMBERSHIP,(char*)&group,sizeof(group)) );

   char buf[ 1500 ];
   while( true )
   {
      sockaddr_in addr;
      ssize_t bytes;
      socklen_t addrsize;
      check( bytes = ::recvfrom(sock,buf,sizeof(buf),0,reinterpret_cast<sockaddr*>(&addr),&addrsize) );
      std::cout << "Received " << bytes << " bytes from " << inet_ntoa(addr.sin_addr) << std::endl;
   }
   return 0;
}

