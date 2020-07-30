/*Author: Adrian González Pardo
  Email: gozapaadr@gmail.com
  Nickname: DevCrack
  Fecha de modificación: 29/07/2020
  GitHub: AdrianPardo99
  Licencia Creative Commons CC BY-SA
*/
#include "socketInit.h"

int createSocketRaw(){
  return socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
}
int isOpenSocket(int packet_socket){
  if(isAnError(packet_socket)){
    perror("Error al abrir socket 7^7\n");
    return 0;
  }else{
    printf("Exito socket abierto correctamente 7u7\n");
    return 1;
  }
}
int closeSocket(int packet_socket){
  close(packet_socket);
  if(isAnError(packet_socket)){
    perror("Ya valio madres :'v el socket no se cerro\n\n");
    return 0;
  }else{
    printf("Socket cerrado correctamente uwu\n\n");
    return 1;
  }
}
