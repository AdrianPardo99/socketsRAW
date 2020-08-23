#/*Author: Adrian González Pardo
#  Email: gozapaadr@gmail.com
#  Nickname: DevCrack
#  Fecha de modificación: 29/07/2020
#  GitHub: AdrianPardo99
#  Licencia Creative Commons CC BY-SA
#*/
comp:
	gcc -c C/libs.c -lm -o O/libs.o -I H/
	gcc -c C/socketInit.c -lm -o O/socketInit.o -I H/
	gcc -c C/hardware.c -lm -o O/hardware.o -I H/
	gcc -c C/arpProtocol.c -lm -o O/arpProtocol.o -I H/
	gcc C/main.c O/libs.o O/socketInit.o O/hardware.o O/arpProtocol.o -lm -o main -I H/
clean:
	rm O/*
