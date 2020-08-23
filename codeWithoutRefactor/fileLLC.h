/*Author: Adrian González Pardo
  Email: gozapaadr@gmail.com
  Nickname: DevCrack
  Fecha de modificación: 14/04/2019
  GitHub: AdrianPardo99
  Licencia Creative Commons CC BY-SA
*/

/*Funcion que obtiene la información del control del LLC*/
char *getControl(int crtl){
  switch (crtl) {
    case 0:case 2:return "Trama de información\n";break;
    case 1:return "Trama de supervisión\n";break;
    case 3:return "Trama no numerada\n";break;
  }
}

/*Funcion que escribe el protocolo en un archivo de texto*/
void fgetProtocol(datos tram,FILE *out){
  switch ((tram&0xFE)) {
    case 0x04:
      fprintf(out,"IBM SNA");
      break;
    case 0x06:
      fprintf(out,"IP");
      break;
    case 0x80:
      fprintf(out,"3COM");
      break;
    case 0xAA:
      fprintf(out,"SNAP");
      break;
    case 0xBC:
      fprintf(out,"BANYAN");
      break;
    case 0xE0:
      fprintf(out,"NOVELL");
      break;
    case 0xFA:
      fprintf(out,"LAN MANAGER FE-CLNS");
      break;
    case 0x42:
      fprintf(out,"SPANNING TREE BPDU");
      break;
    case 0xF0:
      fprintf(out,"NETBIOS");
      break;
  }
  fprintf(out,"\n");
}

/*Funcion que obtiene la información del protocolo que usa el LLC*/
void getProtocol(datos tram){
  switch ((tram&0xFE)) {
    case 0x04:
      printf("IBM SNA");
      break;
    case 0x06:
      printf("IP");
      break;
    case 0x80:
      printf("3COM");
      break;
    case 0xAA:
      printf("SNAP");
      break;
    case 0xBC:
      printf("BANYAN");
      break;
    case 0xE0:
      printf("NOVELL");
      break;
    case 0xFA:
      printf("LAN MANAGER FE-CLNS");
      break;
    case 0x42:
      printf("SPANNING TREE BPDU");
      break;
    case 0xF0:
      printf("NETBIOS");
      break;
  }
  printf("\n");
}

/*Funcion que imprimer el analisis del LLC en un archivo*/
void writeOutputLLC(datos *trama){
  FILE *out;
  out=fopen("outLLC.txt","a");
  if(out==NULL){
    perror("Error al crear o abrir el archivo de salida: ");
  }else{
    int i,crtl=trama[16]&0x03;
    fprintf(out,"MAC Destino: ");
    for(i=0;i!=6;i++){
      fprintf(out,"%.2x%s",trama[i],(i!=5)?(":"):("\n"));
    }
    fprintf(out,"MAC Origen: ");
    for(i=6;i!=12;i++){
      fprintf(out,"%.2x%s",trama[i],(i!=11)?(":"):("\n"));
    }
    fprintf(out,"Longitud: %d\n",trama[12]*256+trama[13]);
    fprintf(out,"DSAP: %s\t\t",(((trama[14]&0x01))==1)?("Trama de grupo"):
      ("Trama individual"));
    fprintf(out,"Protocolo: "),fgetProtocol(trama[14],out);
    fprintf(out,"SSAP: %s\t\t",(((trama[15]&0x01))==1)?("Trama de respuesta")
      :("Trama de comando"));
    fprintf(out,"Protocolo: "),fgetProtocol(trama[15],out);
    fprintf(out,"Control: %s",getControl(crtl));
    if(crtl==0||crtl==2){
      fprintf(out,"Extendida\nNúmero de secuencia de envio: %d\n",(trama[16]&0xFE)>>1);
      fprintf(out,"Número de secuencia que se espera recibir: %d\n",(trama[17]&0xFE)>>1);
      fprintf(out,"POLL/FINAL: %d ",trama[17]&0x01);
      //Vemos el estado del POLL/FINAL
      if(((trama[16]&0x10)>>4)==1){
        if((trama[15]&0x01)==1){
          fprintf(out,"FINAL ");
        }else{
          fprintf(out,"PULL ");
        }
        fprintf(out,"Requiere respuesta inmediata\n");
      }else{
        fprintf(out,"P/F Trama de informacion\n");
      }
    }else if(crtl==1){
      fprintf(out,"Extendida\nNúmero de secuencia que se espera recibir: %d\n",(trama[17]&0xFE)>>1);
      //Determinamos que tipo de Trama de Supervision es
      switch((trama[16]&0x0C)){
        case 0:fprintf(out,"Receiver ready (RR)");break;
        case 4:fprintf(out,"Receiver not ready (RNR)");break;
        case 8:fprintf(out,"Retransmicion (REJ)");break;
        case 12:fprintf(out,"Retransmicion selectiva (SREJ)");break;
      }
      printf("POLL/FINAL: %d ",trama[17]&0x01);
      //Vemos el estado del POLL/FINAL
      if(((trama[16]&0x10)>>4)==1){
        if((trama[15]&0x01)==1){
          fprintf(out,"FINAL ");
        }else{
          fprintf(out,"PULL ");
        }
        fprintf(out,"Requiere respuesta inmediata\n");
      }else{
          fprintf(out,"P/F Trama de informacion\n");
      }
    }
    if(crtl==3){
      //No Numerada
      fprintf(out,"Normal/Extendida\n");
      fprintf(out,"POLL/FINAL: %d ",(trama[16]&0x10)>>4);
      //Vemos el estado del POLL/FINAL
      if(((trama[16]&0x10)>>4)==1){
        if((trama[15]&0x01)==1){//Trama de Respuesta es FINAL
          fprintf(out,"FINAL Requiere respuesta inmediata\n");
          //Imprimimos acorde a lo que se plantea
          fprintf(out,"\n\tNombre\t\t\t    Descripcion\t\t\tInformacion\n");
          switch((trama[16]&0xEC)){
            case 0x60:
              fprintf(out,"|Unnumbered Acknowledgment UA|\t"
                "|Reconoce la conexion|\t"
                "|De uno de los comandos de modo de configuracion|\t");
            break;
            case 0x0C:
              fprintf(out,"|Disconnect Mode DM|\t"
                "|Responder en modo de desconexión|\t"
                "|Modo de configuración requerido|\t");
            break;
            case 0x40:
              fprintf(out,"|Request Disconnect RD|\t"
                "|Solicitud por el comando DISC|\t"
                "\t");
            break;
            case 0x04:
              fprintf(out,"|Request Inialization Mode RIM|\t"
                "|Inicialización necesaria| \t"
                "|Solicitud de comando SIM|\t");
            break;
            case 0x84:
              fprintf(out,"|Frame Reject FRMR|\t"
                "|Informe de Recepción|\t"
                "|De trama inaceptable|\t");
            break;
            default:
              fprintf(out,"MMM MM INCORRECTO");
            break;
          }
        }else{//Trama de Comando es PULL
          fprintf(out,"PULL Requiere respuesta inmediata\n");
          //Imprimimos acorde a lo que se plantea
          fprintf(out,"\n\tNombre\t\t\t    Descripcion\t\t\tInformacion\n");
          switch((trama[16]&0xEC)){
            case 0x80:
              fprintf(out,"|Set normal response SNRM|\t"
                "|Modo de Configuracion|\t"
                "|Usa 3 bits para números de secuencia|\t");
            break;
            case 0xCC:
              fprintf(out,"|Set normal response extended mode SNRME|\t"
                "|Modo de Configuracion Extendido|\t"
                "|Usa 7 bits para números de secuencia|\t");
            break;
            case 0x0C:
              fprintf(out,"|Set asynchronous response SARM|\t"
                "|Modo de Configuracion Asincrono|\t"
                "|Usa 3 bits para números de secuencia|\t");
            break;
            case 0x4C:
              fprintf(out,"|Set asynchronous response extended mode SARME|\t"
                "|Modo de Configuracion Asincrono Extendido|\t"
                "|Usa 7 bits para números de secuencia|\t");
            break;
            case 0x2C:
              fprintf(out,"|Set asynchronous balanced mode SABM|\t"
                "|Modo de Configuracion Asincrono Balanceadp|\t"
                "|Usa 3 bits para números de secuencia|\t");
            break;
            case 0x6C:
              fprintf(out,"|Set asynchronous balanced extended mode SABME|\t"
                "|Modo de Configuracion Asincrono Balanceado Extendido|\t"
                "|Usa 7 bits para números de secuencia|\t");
            break;
            case 0x04:
              fprintf(out,"|Set inicialitation mode SIM|\t"
                "|Iniciar funcion de control de enlace|\t"
                "|Dirigido a la estacion|\t");
            break;
            case 0x40:
              fprintf(out,"|Disconnect DISC|\t"
                "|Terminar la conexion de enlace lógico|\t"
                "|Para futuras tramas de I y S regresa DM|\t");
            break;
            case 0x20:
              fprintf(out,"|Unnumbered Poll UP|\t"
                "|Se utiliza para solicitar|\t"
                "|Información de control|\t");
            break;
            case 0x8C:
              fprintf(out,"|Reset RSET|\t"
                "|Utilizado para la recuperación|\t"
                "|Reestablece N(R), pero no N(S)|\t");
            break;
            default:
              fprintf(out,"MMM MM INCORRECTO");
            break;
          }
        }
      }else{
        fprintf(out,"P/F Trama de informacion\n");
        //Imprimimos acorde a lo que se plantea
        fprintf(out,"\n\tNombre\t\t\t    Descripcion\t\t\tInformacion\n");
        switch((trama[16]&0xEC)){
          case 0x00:
            fprintf(out,"|Unnumbered Information UI|\t"
              "|Datos no reconocidos|\t"
              "|Tiene una carga util|\t");
          break;
          case 0xAC:
            fprintf(out,"|Exchange Identification XID|\t"
              "|Se utiliza para solicitar|\t"
              "|Información de control|\t");
          break;
          case 0xE0:
            fprintf(out,"|Test TEST|\t"
              "|Intercambio de información idéntica|\t"
              "|Campos para la realización de pruebas|\t");
          break;
          default:
            fprintf(out,"MMM MM INCORRECTO");
          break;
        }
      }
      fprintf(out,"\n");
    }
    fprintf(out,"\n");
    fclose(out);
  }
}

/*Funcion que analiza la trama LLC con todos las operaciones que salen*/
void analiceLCC(datos *trama){
  int i,crtl=trama[16]&0x03;
  printf("%sMAC Destino: ",KGRN);
  for(i=0;i!=6;i++){
    printf("%.2x%s",trama[i],(i!=5)?(":"):("\n"));
  }
  printf("%sMAC Origen: ",KYEL);
  for(i=6;i!=12;i++){
    printf("%.2x%s",trama[i],(i!=11)?(":"):("\n"));
  }
  printf("%sLongitud: %d\n",KBLU,trama[12]*256+trama[13]);
  printf("%sDSAP: %s\t\t",BRED,(((trama[14]&0x01))==1)?("Trama de grupo"):
    ("Trama individual"));
  printf("Protocolo: "),getProtocol(trama[14]);
  printf("%sSSAP: %s\t\t",BYEL,(((trama[15]&0x01))==1)?("Trama de respuesta")
    :("Trama de comando"));
  printf("Protocolo: "),getProtocol(trama[15]);
  printf("%sControl: %s%s",KRED,getControl(crtl),BRED);
  if(crtl==0||crtl==2){
    printf("Extendida\nNúmero de secuencia de envio: %d\n",(trama[16]&0xFE)>>1);
    printf("Número de secuencia que se espera recibir: %d\n",(trama[17]&0xFE)>>1);
    printf("POLL/FINAL: %d ",trama[17]&0x01);
    //Vemos el estado del POLL/FINAL
    if(((trama[16]&0x10)>>4)==1){
      if((trama[15]&0x01)==1){
        printf("FINAL ");
      }else{
        printf("PULL ");
      }
      printf("Requiere respuesta inmediata\n");
    }else{
      printf("P/F Trama de informacion\n");
    }
  }else if(crtl==1){
    printf("Extendida\nNúmero de secuencia que se espera recibir: %d\n",(trama[17]&0xFE)>>1);
    //Determinamos que tipo de Trama de Supervision es
    switch((trama[16]&0x0C)){
      case 0:printf("Receiver ready (RR)");break;
      case 4:printf("Receiver not ready (RNR)");break;
      case 8:printf("Retransmicion (REJ)");break;
      case 12:printf("Retransmicion selectiva (SREJ)");break;
    }
    printf("POLL/FINAL: %d ",trama[17]&0x01);
    //Vemos el estado del POLL/FINAL
    if(((trama[16]&0x10)>>4)==1){
      if((trama[15]&0x01)==1){
        printf("FINAL ");
      }else{
        printf("PULL ");
      }
      printf("Requiere respuesta inmediata\n");
    }else{
        printf("P/F Trama de informacion\n");
    }
  }
  if(crtl==3){
    //No Numerada
    printf("Normal/Extendida\n");
    printf("POLL/FINAL: %d ",(trama[16]&0x10)>>4);
    //Vemos el estado del POLL/FINAL
    if(((trama[16]&0x10)>>4)==1){
      if((trama[15]&0x01)==1){//Trama de Respuesta es FINAL
        printf("FINAL Requiere respuesta inmediata\n");
        //Imprimimos acorde a lo que se plantea
        printf("\n\tNombre\t\t\t    Descripcion\t\t\tInformacion\n");
        switch((trama[16]&0xEC)){
          case 0x60:
            printf("|Unnumbered Acknowledgment UA|\t"
              "|Reconoce la conexion|\t"
              "|De uno de los comandos de modo de configuracion|\t");
          break;
          case 0x0C:
            printf("|Disconnect Mode DM|\t"
              "|Responder en modo de desconexión|\t"
              "|Modo de configuración requerido|\t");
          break;
          case 0x40:
            printf("|Request Disconnect RD|\t"
              "|Solicitud por el comando DISC|\t"
              "\t");
          break;
          case 0x04:
            printf("|Request Inialization Mode RIM|\t"
              "|Inicialización necesaria| \t"
              "|Solicitud de comando SIM|\t");
          break;
          case 0x84:
            printf("|Frame Reject FRMR|\t"
              "|Informe de Recepción|\t"
              "|De trama inaceptable|\t");
          break;
          default:
            printf("MMM MM INCORRECTO");
          break;
        }
      }else{//Trama de Comando es PULL
        printf("PULL Requiere respuesta inmediata\n");
        //Imprimimos acorde a lo que se plantea
        printf("\n\tNombre\t\t\t    Descripcion\t\t\tInformacion\n");
        switch((trama[16]&0xEC)){
          case 0x80:
            printf("|Set normal response SNRM|\t"
              "|Modo de Configuracion|\t"
              "|Usa 3 bits para números de secuencia|\t");
          break;
          case 0xCC:
            printf("|Set normal response extended mode SNRME|\t"
              "|Modo de Configuracion Extendido|\t"
              "|Usa 7 bits para números de secuencia|\t");
          break;
          case 0x0C:
            printf("|Set asynchronous response SARM|\t"
              "|Modo de Configuracion Asincrono|\t"
              "|Usa 3 bits para números de secuencia|\t");
          break;
          case 0x4C:
            printf("|Set asynchronous response extended mode SARME|\t"
              "|Modo de Configuracion Asincrono Extendido|\t"
              "|Usa 7 bits para números de secuencia|\t");
          break;
          case 0x2C:
            printf("|Set asynchronous balanced mode SABM|\t"
              "|Modo de Configuracion Asincrono Balanceadp|\t"
              "|Usa 3 bits para números de secuencia|\t");
          break;
          case 0x6C:
            printf("|Set asynchronous balanced extended mode SABME|\t"
              "|Modo de Configuracion Asincrono Balanceado Extendido|\t"
              "|Usa 7 bits para números de secuencia|\t");
          break;
          case 0x04:
            printf("|Set inicialitation mode SIM|\t"
              "|Iniciar funcion de control de enlace|\t"
              "|Dirigido a la estacion|\t");
          break;
          case 0x40:
            printf("|Disconnect DISC|\t"
              "|Terminar la conexion de enlace lógico|\t"
              "|Para futuras tramas de I y S regresa DM|\t");
          break;
          case 0x20:
            printf("|Unnumbered Poll UP|\t"
              "|Se utiliza para solicitar|\t"
              "|Información de control|\t");
          break;
          case 0x8C:
            printf("|Reset RSET|\t"
              "|Utilizado para la recuperación|\t"
              "|Reestablece N(R), pero no N(S)|\t");
          break;
          default:
            printf("MMM MM INCORRECTO");
          break;
        }
      }
    }else{
      printf("P/F Trama de informacion\n");
      //Imprimimos acorde a lo que se plantea
      printf("\n\tNombre\t\t\t    Descripcion\t\t\tInformacion\n");
      switch((trama[16]&0xEC)){
        case 0x00:
          printf("|Unnumbered Information UI|\t"
            "|Datos no reconocidos|\t"
            "|Tiene una carga util|\t");
        break;
        case 0xAC:
          printf("|Exchange Identification XID|\t"
            "|Se utiliza para solicitar|\t"
            "|Información de control|\t");
        break;
        case 0xE0:
          printf("|Test TEST|\t"
            "|Intercambio de información idéntica|\t"
            "|Campos para la realización de pruebas|\t");
        break;
        default:
          printf("MMM MM INCORRECTO");
        break;
      }
    }
    printf("\n");
  }
  printf("\n%s",KNRM);
}

/*Funcion que escucha si hay una trama LLC en el buffer*/
void recibeLLC(socketRaw sock,datos *trama){
  int tam;
  while(1){
    tam=recvfrom(sock,trama,1514,MSG_DONTWAIT,NULL,0);
    if(!isAnError(tam)){
      if((trama[12]*256+trama[13])<=1500){
        printf("\t\t%sTrama LLC\n\t   %sTamaño de trama: %d%s\n",
          BRED,BBLU,tam,KNRM);
        imprimeTramaLLC(trama,tam);
        analiceLCC(trama);
        writeOutputLLC(trama);
      }
    }
  }
}

/*Funcion que inicializa la interfaz de red para escuchar si hay una trama
  LLC*/
void initLLC(){
  createSocket();
  openSocket();
  if(!isAnError(packet_socket)){
    obtainHardwareData();
    if(isAnError(indice)){exit(1);}
    printf("%sPor favor preciona enter para continuar...%s",BGRN,KNRM);
    getchar();
    printf("%sInit scanner LLC%s\n",BBLU,KNRM);
    recibeLLC(packet_socket,tramaGet);
    closeSocket();
  }else{
    perror("Error al abrir la aplicación en funcion initLLC: ");
    exit(1);
  }
}

/*Funcion inicializa el analizador LLC apartir de un archivo de texto*/
void initFileLLC(char *nameFile){
  FILE *fl;
  fl=fopen(nameFile,"r");
  if(fl==NULL){
    perror("Error al leer archivo: ");
    exit(1);
  }
  clearScreen();
  datos buffer[500],*trama;
  char hexbyte[3];
  int linea=1,val=0,i;
  while(fgets(buffer,sizeof(buffer),fl)){
    i=0;
    memset(&trama, 0, sizeof(trama));
    trama=malloc((int)(strlen(buffer)-2)/2);
    for(int a = 0; a < strlen(buffer); a+=2){
      hexbyte[0]=buffer[a];
      hexbyte[1]=buffer[a+1];
      sscanf(hexbyte, "%x", &val);
      trama[i]=(unsigned char)val;
      i++;
    }
    printf("\t%sTrama LLC %d\t%s   Tamaño de trama: %d%s\n",
      BRED,linea,BBLU,(int)(strlen(buffer)-2)/2,KNRM);
    imprimeTramaLLC(trama,(int)(strlen(buffer)-2)/2);
    analiceLCC(trama);
    writeOutputLLC(trama);
    printf("%s",KNRM);
    stop(4000000);
    clearScreen();
    linea++;
  }
  fclose(fl);
}

/*Funcion que envia tramas LLC a partir de un txt*/
void initSentFileLLC(char *nameFile){
  createSocket();
  openSocket();
  if(!isAnError(packet_socket)){
    obtainHardwareData();
    if(isAnError(indice)){exit(1);}
    printf("%sPor favor preciona enter para continuar...%s",BGRN,KNRM);
    getchar();
    printf("%sInit sent LLC%s\n",BBLU,KNRM);
    FILE *fl;
    fl=fopen(nameFile,"r");
    if(fl==NULL){
      perror("Error al leer archivo: ");
      exit(1);
    }
    clearScreen();
    datos buffer[500],*trama;
    char hexbyte[3];
    int linea=1,val=0,i;
    while(fgets(buffer,sizeof(buffer),fl)){
      i=0;
      memset(&trama, 0, sizeof(trama));
      trama=malloc((int)(strlen(buffer)-2)/2);
      for(int a = 0; a < strlen(buffer); a+=2){
        hexbyte[0]=buffer[a];
        hexbyte[1]=buffer[a+1];
        sscanf(hexbyte, "%x", &val);
        trama[i]=(unsigned char)val;
        i++;
      }
      printf("\t%sTrama LLC %d\t%s   Tamaño de trama: %d%s\n",
        BRED,linea,BBLU,(int)(strlen(buffer)-2)/2,KNRM);
      imprimeTramaLLC(trama,(int)(strlen(buffer)-2)/2);
      enviaTrama(packet_socket,trama,indice);
      stop(50000000);
      linea++;
    }
    printf("Exito al enviar todas las tramas del archivo\n");
  }else{
    perror("Error al abrir la aplicación en funcion initLLC: ");
    exit(1);
  }
}
