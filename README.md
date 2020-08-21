# Socket RAW application #
Aplicación en construcción

```bash
#For excecute install cmake and make, and run
  make
#Next
  sudo ./main
```
# Tramas de datos físicas de datos #
En la programación y análisis de tramas de datos en la red es necesario saber que existe una estructura básica de como se envian las tramas de datos las cuales inicialmente utilizan una pila básica de como se ordenan los datos
```bash
  |MAC-Destination|MAC-Source|Ethertype/Length|Data|
```
__Donde__

| Parte | Característica |
| ------- | --------- |
| MAC-Destination | Es un conjunto de 6 bytes los cuales especifican la dirección física a la cual se desea llegar |
| MAC-Source | Es un conjunto de 6 bytes los cuales especifican la dirección física de donde es enviada la solicitud |
| Ethertype/Length | Es un conjunto de 2 bytes en el cual se especifica el protocolo o tamaño de la trama de datos |
| Data | Es un conjunto de tamaño N bytes el cual por default tiene un limite de 1500 bytes para datos dependiendo de la interfaz de red especificado como la Unidad de Transmisión Máxima (MTU Maximum Transmission Unit) |


# ARP protocol #
El Protocolo de Resolución de Direcciones por sus siglas (ARP Address Resolution Protocol), es un protocolo cuyo fin de creación es conocer la distribución de direcciones IP/MAC de una red de computadoras las cuales pueden estar dentro o no de una red local, a continuación se muestra la estructura del protocolo.

## Estructura a partir de Ethertype ARP ##

La estructura de la trama ARP en petición es la siguiente:
```bash
  |MAC-B|MAC-S|Ethertype|Hardware-type|Protocol-type|LHardware|LAProtocol|OpCode|
    |MAC-S|IP-S|MAC-D|IP-D|
```

La estructura de la trama de ARP en respuesta es la misma solo cambia OpCode

__Donde__

| Estructura de bytes | Características y como se llena/analiza |
| ------- | --------- |
| MAC-B | Se llena con los datos de una MAC de Broadcast (FF:FF:FF:FF:FF:FF) 6 bytes |
| MAC-S | Se llena con los datos de tu interfaz de red 6 bytes |
| Ethertype | Se llena con los datos del protocolo ARP (0x0806) 2 bytes |
| Hardware-type | Se llena con el código del tipo de hardware 2 bytes |
| Protocol-type | Se llena con el tipo de protocolo que ocupa (0x0800) 2 bytes |
| LHardware | Es la longitud de hardware en la cual se ocupa 1 byte (0x06) |
| LAProtocol | Es la longitud de la dirección IP del protocolo 1 byte (0x04) |
| OpCode | Es el código de operación con el se desea hacer la petición de ARP Normal o Inversa e incluso el código de respuesta 2 bytes |
| MAC-S | Mismo que los primeros bytes de la dirección física de origen 6 bytes |
| IP-S | Es la dirección IP de la interfaz de origen de red 4 bytes |
| MAC-D | Es la MAC de destino de la red 6 bytes |
| IP-D | Es la IP del destino de la red 4 bytes |


## Tipos de Hardware ##

En el apartado de Tipo de Hardware contiene opciones en las cuales se puede seleccionar que es una red bajo Ethernet u otro modelo de red/dispositivo

Dichos códigos son:
| Código de tipo de hardware | Significado |
| ------- | --------- |
| 0x0001 | Ethernet 10 Mbps |
| 0X0006 | Redes IEEE 802.5 TokenRing |
| 0x000F | Frame Relay |
| 0X0010 | ATM |

## Códigos de Operación ##

Es necesario tener un código de operación el cual permite conocer el estatus de la solicitud/respuesta de ARP

Estos códigos son:
| Código de operación | Significado |
| ------- | --------- |
| 0x0001 | Solicitud ARP |
| 0x0002 | Respuesta ARP |
| 0x0008 | Solicitud ARP Inversa (IP a partir de MAC) |
| 0X0009 | Respuesta ARP Inversa |

## ARP Normal Petition ##

### Petición ###

Cabe resaltar que si bien podemos conocer estos datos es necesario saber como esta estructurada la solicitud de la petición ARP, ahora bien a continuación se muestra una forma de cómo se estructura en valores hexadecimales la petición de forma separada:

```bash
  FF FF FF FF FF FF       # 0-5 bytes   MAC Broadcast
  XX XX XX XX XX XX       # 6-11 bytes  MAC de la Interfaz de red
  06 08                   # 12-13 bytes Ethertype ARP
  00 01                   # 14-15 bytes Type Hardware (Ethernet)
  08 00                   # 16-17 bytes Protocol type
  06                      # 18 byte     Hardware Length
  04                      # 19 byte     Protocol Length
  00 01                   # 20-21 bytes Op Code Request ARP
  XX XX XX XX XX XX       # 22-27 bytes MAC de la Interfaz de red
  XZ XZ XZ XZ             # 28-31 bytes IP de origen de la interfaz de red
  00 00 00 00 00 00       # 32-37 bytes MAC destino inicio en 0's
  XY XY XY XY             # 38-41 bytes IP a la cual se desea obtener la MAC
```

### Respuesta ###
```bash
  XX XX XX XX XX XX       # 0-5 bytes   MAC de nuestra interfaz
  YY YY YY YY YY YY       # 6-11 bytes  MAC de la interfaz de red que nos responde
  06 08                   # 12-13 bytes Ethertype ARP
  00 01                   # 14-15 bytes Type Hardware (Ethernet)
  08 00                   # 16-17 bytes Protocol type
  06                      # 18 byte     Hardware Length
  04                      # 19 byte     Protocol Length
  00 02                   # 20-21 bytes Op Code Answer ARP
  YY YY YY YY YY YY       # 22-27 bytes MAC de la interfaz de red que nos responde
  XY XY XY XY             # 28-31 bytes IP a la cual se obtuvo MAC
  XX XX XX XX XX XX       # 32-37 bytes MAC de la Interfaz de red
  XZ XZ XZ XZ             # 38-41 bytes IP de origen de la interfaz de red
```

## ARP Inverse Petition ##

### Petición ###

Si bien la petición de ARP normal es tener conocimiento de la IP y buscar el a quien pertenece la dirección MAC, este tipo de petición es hecha cuando se conoce la dirección física pero no se conoce la dirección IP, la cual esta estructurada de la siguiente forma:

```bash
  FF FF FF FF FF FF       # 0-5 bytes   MAC Broadcast
  XX XX XX XX XX XX       # 6-11 bytes  MAC de la Interfaz de red
  06 08                   # 12-13 bytes Ethertype ARP
  00 01                   # 14-15 bytes Type Hardware (Ethernet)
  08 00                   # 16-17 bytes Protocol type
  06                      # 18 byte     Hardware Length
  04                      # 19 byte     Protocol Length
  00 08                   # 20-21 bytes Op Code Request ARP Inverse
  XX XX XX XX XX XX       # 22-27 bytes MAC de la Interfaz de red
  XZ XZ XZ XZ             # 28-31 bytes IP de origen de la interfaz de red
  YY YY YY YY YY YY       # 32-37 bytes MAC destino
  00 00 00 00             # 38-41 bytes IP al inicio en 0's
```

### Respuesta ###

```bash
  XX XX XX XX XX XX       # 0-5 bytes   MAC de nuestra interfaz
  YY YY YY YY YY YY       # 6-11 bytes  MAC de la interfaz de red que nos responde
  06 08                   # 12-13 bytes Ethertype ARP
  00 01                   # 14-15 bytes Type Hardware (Ethernet)
  08 00                   # 16-17 bytes Protocol type
  06                      # 18 byte     Hardware Length
  04                      # 19 byte     Protocol Length
  00 09                   # 20-21 bytes Op Code Answer ARP Inverse
  YY YY YY YY YY YY       # 22-27 bytes MAC de la interfaz de red que nos responde
  XY XY XY XY             # 28-31 bytes IP de que se obtuvo la MAC
  XX XX XX XX XX XX       # 32-37 bytes MAC de la Interfaz de red
  XZ XZ XZ XZ             # 38-41 bytes IP de origen de la interfaz de red
```
