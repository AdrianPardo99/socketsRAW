/*Author: Adrian González Pardo
  Email: gozapaadr@gmail.com
  Nickname: DevCrack
  Fecha de modificación: 30/07/2020
  GitHub: AdrianPardo99
  Licencia Creative Commons CC BY-SA
*/
#include "libs.h"

int isAnError(int e){
  return e == -1;
}

int getFlags(int argc,char **argv){
  int aflag=0,sflag=0,oaflag=0,index,c,nflag=0;
  opterr=0;
  while((c=getopt(argc, argv, "aosnh"))!=-1){
    switch(c){
      case 'a':
        aflag=1;
      break;
      case 'o':
        oaflag=1;
      break;
      case 's':
        sflag=1;
      break;
      case 'n':
        nflag=1;
      break;
      case 'h':
        printf("Welcome to the Socket Raw application, the next flags can be "
          "use for working"
          "\n\t-a\tEnable an scanner network with ARP"
          "\n\t-h\tDisplay the help options in the application"
          "\n\t-n\tOpen a socket, enum and get the network hardware information"
          "\n\t-s\tEnable the SQL store for ARP scanner or the ARP single scan"
          "\n\t-o\tEnable the ARP single scan\n");
          break;
      default:
        printf("Error\n");
      break;
    }
  }
  if(nflag){
    return 1;
  }else if(oaflag){
    return 2;
  }else if(aflag){
    return 4;
  }else if(sflag && aflag){
    return 5;
  }else if(sflag && oaflag){
    return 3;
  }else{
    return 0;
  }
}
