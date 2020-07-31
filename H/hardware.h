/*Author: Adrian González Pardo
  Email: gozapaadr@gmail.com
  Nickname: DevCrack
  Fecha de modificación: 30/07/2020
  GitHub: AdrianPardo99
  Licencia Creative Commons CC BY-SA
*/
#include "socketInit.h"

typedef int socketRaw;
typedef struct ifreq interfaces;
typedef unsigned char datos;
typedef char name;
typedef int dato;
typedef short tinyDato;
typedef struct sockaddr_ll tramas;
typedef struct timeval timer;
typedef struct ifconf confInt;

void getMetric(socketRaw,interfaces);
void getMTU(socketRaw,interfaces);
void getMask(socketRaw,interfaces,datos*);
void getIP(socketRaw,interfaces,datos*);
void getMAC(socketRaw,interfaces,datos*);
void enumInterface(socketRaw,interfaces*);
datos **getData(socketRaw);
