/*Author: Adrian González Pardo
  Email: gozapaadr@gmail.com
  Nickname: DevCrack
  Fecha de modificación: 30/07/2020
  GitHub: AdrianPardo99
  Licencia Creative Commons CC BY-SA
*/
#include "hardware.h"
#include "constantsData.h"

void hardware(socketRaw);

int main(int argc, char *argv[]) {
  if(geteuid()!=0){
    printf(BRED"Esta aplicación debe ser ejecutado con privilegios root \u2665\n"KNRM);
    exit(1);
  }
  int flag;
  if(flag=getFlags(argc,argv)){
    socketRaw sock=createSocketRaw();
    if(isOpenSocket(sock)){
      if(flag==1){
        hardware(sock);
      }
      if(!closeSocket(sock)){exit(1);}
    }else{
      exit(1);
    }
  }
  return 0;
}

void hardware(socketRaw sock){
  int i;
  datos **data=getData(sock),*macOrigen,*ipO,*mascara;
  macOrigen=*data;
  ipO=*(data+1);
  mascara=*(data+2);
  printf("MAC Origin interface: ");
  for(i=0;i<6;i++){
    printf("%.2X",macOrigen[i]);
    if(i<5){
      printf(":");
    }
  }
  printf("\nIP Origin interface: ");
  for(i=0;i<4;i++){
    printf("%d",ipO[i]);
    if(i<3){
      printf(".");
    }
  }
  printf("\nNetmask Origin interface: ");
  for(i=0;i<4;i++){
    printf("%d",mascara[i]);
    if(i<3){
      printf(".");
    }
  }
  printf("\n");
  for(i=0;i<3;i++){
    free(data[i]);
  }
  free(data);
}
