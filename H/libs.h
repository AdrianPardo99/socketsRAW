/*Author: Adrian González Pardo
  Email: gozapaadr@gmail.com
  Nickname: DevCrack
  Fecha de modificación: 30/07/2020
  GitHub: AdrianPardo99
  Licencia Creative Commons CC BY-SA
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ifaddrs.h>
#include <signal.h>
#include <netdb.h>
#include <ctype.h>
#include <asm/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h>
//#include <linux/if_packet.h>
#include <net/ethernet.h> /* the L2 protocols */
#include <net/if.h>
#include <net/ethernet.h>
#include <netinet/in.h>
#include <netpacket/packet.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#define BUFFER_SIZE 4096
#define TTLPATH "/proc/sys/net/ipv4/ip_default_ttl"
#define ICMP_PROT 1
#define PINGTOTLEN 84
/*Creación de cabecera de colores para distintos usos*/
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define BRED  "\x1B[91m"
#define BGRN  "\x1B[92m"
#define BYEL  "\x1B[93m"
#define BBLU  "\x1B[94m"
#define BMAG  "\x1B[95m"
#define BCYN  "\x1B[96m"
#define BWHT  "\x1B[97m"



int isAnError(int);
int getFlags(int,char**);
