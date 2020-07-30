/*Author: Adrian González Pardo
  Email: gozapaadr@gmail.com
  Nickname: DevCrack
  Fecha de modificación: 29/07/2020
  GitHub: AdrianPardo99
  Licencia Creative Commons CC BY-SA
*/
typedef int socketRaw;
typedef struct ifreq interfaces;
typedef unsigned char datos;
typedef char name;
typedef int dato;
typedef short tinyDato;
typedef struct sockaddr_ll tramas;
typedef struct timeval timer;
typedef struct ifconf confInt;

datos macBroadcast[]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
