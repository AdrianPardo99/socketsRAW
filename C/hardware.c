/*Author: Adrian González Pardo
  Email: gozapaadr@gmail.com
  Nickname: DevCrack
  Fecha de modificación: 30/07/2020
  GitHub: AdrianPardo99
  Licencia Creative Commons CC BY-SA
*/
#include "hardware.h"

void getMetric(socketRaw ds,interfaces nic){
  if(ioctl(ds,SIOCGIFMETRIC,&nic) == -1){
    perror("Error al obtener la Metrica ");
    exit(1);
  }else{
    printf("\nLa Metrica es: %d",nic.ifr_metric);
  }
}

void getMTU(socketRaw ds,interfaces nic){
  if(ioctl(ds,SIOCGIFMTU,&nic) == -1){
    perror("Error al obtener el MTU ");
    exit(1);
  }else{
    printf("\nMTU es: %d",nic.ifr_mtu);
  }
}

void getMask(socketRaw ds,interfaces nic,datos *mascara){
  if(ioctl(ds,SIOCGIFNETMASK,&nic) == -1){
    perror("Error al obtener la Macara de subred ");
    exit(1);
  }else{
    memcpy(mascara,nic.ifr_hwaddr.sa_data+2,4);
  }
}

void getIP(socketRaw ds,interfaces nic,datos *ipO){
  if(ioctl(ds,SIOCGIFADDR,&nic) == -1){
    perror("Error al obtener la IP ");
    exit(1);
  }else{
    memcpy(ipO,nic.ifr_hwaddr.sa_data+2,4);
  }
}

void getMAC(socketRaw ds,interfaces nic,datos *macOrigen){
  if(ioctl(ds,SIOCGIFHWADDR,&nic)==-1){
    perror("Error al obtener la MAC ");
    exit(1);
  }else{
    memcpy(macOrigen,nic.ifr_hwaddr.sa_data+0,6);
  }
}

void enumInterface(socketRaw ds,interfaces *nic){
  confInt conf;
  name cpHardware[1000][IFNAMSIZ];
  char buf[16384];
	unsigned i;
  int t=0,indice;
  interfaces *nics;
  size_t len;
  conf.ifc_len=sizeof buf;
  conf.ifc_buf=buf;
  if(ioctl(ds, SIOCGIFCONF,&conf)!=0){
    perror("ioctl(SIOCGIFCONF)");
		exit(EXIT_FAILURE);
  }
  nics=conf.ifc_req;
  printf(KYEL"Interfaces de red disponibles: \n"KNRM);
  for(i=0;i<conf.ifc_len;){
    len=sizeof *nics;
    printf(BCYN"\tInterface[%d]: %s\n"KNRM,t,nics->ifr_name);
    strcpy(cpHardware[t]+0,nics->ifr_name);
    nics=(struct ifreq*)((char*)nics+len);
		i+=len;
    t++;
  }
  printf(BGRN"Ingresa el numero de interfaz con la que deseas trabajar: "KNRM);
  scanf("%d",&indice);
  strcpy(nic->ifr_name+0,cpHardware[indice]+0);
}

datos **getData(socketRaw sock){
  int i;
  datos **data=NULL;
  interfaces nic;
  data=(datos**)malloc(sizeof(datos*)*3);
  for(i=0;i<3;i++){
    *(data+i)=(datos*)malloc(sizeof(datos)*40);
  }
  enumInterface(sock,&nic);
  getMAC(sock,nic,&**(data));
  getIP(sock,nic,&*(*(data+1)));
  getMask(sock,nic,&*(*(data+2)));
  /*getMTU(sock,nic);
  getMetric(sock,nic);*/
  return data;
}
