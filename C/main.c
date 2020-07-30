/*Author: Adrian González Pardo
  Email: gozapaadr@gmail.com
  Nickname: DevCrack
  Fecha de modificación: 29/07/2020
  GitHub: AdrianPardo99
  Licencia Creative Commons CC BY-SA
*/
#include "hardware.h"
#include "constantsData.h"

int main(int argc, char *argv[]) {
  if(geteuid()!=0){
    printf(BRED"Esta aplicación debe ser ejecutado con privilegios root \u2665\n"KNRM);
    exit(1);
  }
  socketRaw sock=createSocketRaw();
  if(isOpenSocket(sock)){
    printf("Positivo %02X:%02X:%02X:%02X:%02X:%02X\n",macBroadcast[0],macBroadcast[1],macBroadcast[2],macBroadcast[3],macBroadcast[4],macBroadcast[5]);
    if(closeSocket(sock)){

    }else{
      exit(1);
    }
  }else{
    exit(1);
  }
  return 0;
}
