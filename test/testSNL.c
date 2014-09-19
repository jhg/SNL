/*
testSNL Copyright (C) 2009 Jesús Hernández Gormaz

Fecha de creacion: 5 de abril del 2009

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 3, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

Este programa es software libre. Puede redistribuirlo y/o
modificarlo bajo los términos de la Licencia Pública General
de GNU según es publicada por la Free Software Foundation,
bien de la versión 3 de dicha Licencia o bien (según su
elección) de cualquier versión posterior.
Este programa se distribuye con la esperanza de que sea
útil, pero SIN NINGUNA GARANTÍA, incluso sin la garantía
MERCANTIL implícita o sin garantizar la CONVENIENCIA PARA UN
PROPÓSITO PARTICULAR. Para más detalles, véase la Licencia
Pública General de GNU.

Debería haber recibido una copia de la Licencia Pública
General junto con este programa. En caso contrario, escriba
a la Free Software Foundation, Inc., en 675 Mass Ave,
Cambridge, MA 02139, EEUU.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SNL/SNL.h>

int main(int argc, char *argv[]){
if(argc < 2){
	printf("Para usar este ejemplo de SNL debe de indicar un dominio");
	printf(" despues del comando con el que ejecuta el programa, por");
	printf(" ejemplo:\n %s www.fsf.org\n", argv[0]);
	exit(-1);
}else{
	/*la variable conexión guardara el identificador de la conexión
	el cual deberemos de pasarlo a las funciones cuando enviemos
	orecivamos datos, etc, etc, etc.*/
	int conexion;
	/*texto es el array que usaremos como buffer para enviar
	y recibir datos, con medio kilobyte es suficiente ya que
	normalmente las paginas pesan unos pocos kilobytes (sin
	contar las imagenes) y asi nuestro ejemplo leera varias
	veces el socket para poder recibir toda la pagina*/
	char texto[512];
	/*las dos variables siguientes son respectivamente para guardar
	la direccion del host, la IP, y para guardar un texto con la
	informacion de SNL*/
	char direccion[512], informacion[501];
	/*para trabajar con la conexión usaremos un grupo de conexiones
	por ser mejor que trabajar con la conexión directamente
	incluso cuando solo necesitamos una conexión, el grupo nos
	permite comprobar si hay datos esperando que los leamos en
	algun socket sin que la aplicación se quede a la espera de que
	haya datos en el socket*/
	grupo_conexiones *a;
	/*mostramos en terminal un texto que indica que el programa
	esta ejecutandose y que vamos a empezar con la conexion*/
	printf("\nEjecucion\n");
	/*empezamos a llenar texto con la peticion HTTP que enviaremos
	despues*/
	strcpy(&texto[0], "GET ");
	/*mostramos en pantalla el host al que nos conectaremos*/
	printf("Host: %s\n", argv[1]);
	/*usamos la funcion de DNS para IPv6 de forma que si se
	obtiene una direccion parecida a 1F::F0:AB es que la maquina en
	la que ejecutamos este ejemplo ya funciona con IPv6, pero este
	ejemplo es de IPv4, por lo que esta es la unica funcion de Ipv6
	que usaremos en este ejemplo por ser la que más difiere de su
	equivalente para IPv4, el primer argumento es la cadena donde
	queremos guardar la direccion IP, el segundo el host, el tercero
	es el servicio pero lo dejamos como null ya que no necesitamos
	conocer el puerto del servicio ni el servicio de un puerto
	en este ejemplo además de no haber modificado los ficheros de
	configuracion del sistema para configurar este ejemplo como un
	servicio (esto limita los puertos que podemos usar), el cuarto
	la longitud maxima de la cadena de caracteres de la direccion
	(igual que antes al obtener el texto de la informacion de SNL)
	y el ultimo es la longitud del servicio, 0 como le pasamos NULL*/
	SNL_DNS(&direccion[0], argv[1], NULL, 1023, 0);
	/*imprimimos en la terminal el host y la Ipv6 obtenida*/
	printf("DNS IPv6 obtenido para %s: %s\n", argv[1], &direccion[0]);
	/*usamos la funcion de DNS para IPv4, el primer argumento es
	donde se copiara la direccion IPv4 y el segundo el host*/
	SNL_DNS_IPv4(&direccion[0], argv[1]);
	/*igual que antes, mostramos el host y la IPv4 en pantalla*/
	printf("DNS IPv4 obtenido para%s: %s\n", argv[1], &direccion[0]);
	/*continuamos rellenando el buffer con la peticion GET de HTTP
	que enviaremos*/
	strcat(&texto[0], "http://");
	strcat(&texto[0], argv[1]);
	strcat(&texto[0], "/");
	strcat(&texto[0], " HTTP/1.1\n\rHost: ");
	strcat(&texto[0], argv[1]);
	strcat(&texto[0], "\n\rAccept: text/html\n\rKeep-Alive: 300\n\rProxy-Connection: keep-alive\n\r\n\r");
	/*iniciamos una conexión de TCP sobre IPv4 en la direccion IP
	que obtuvimos antes en el puerto 80, el puerto usado para el
	protocolo HTTP*/
	conexion= SNL_conectar_TCP_IPv4(&direccion[0], 80);
	/*comprobamos si conseguimos conectarnos*/
	if(conexion == SIN_CONEXION){
		printf("problemas conectando\n");
		exit(-1);
	}
	printf("conectado\n");
	/*creamos un nuevo grupo de conexiones indicando como conexión
	de escucha SIN_CONEXION ya que en este ejemplo no aceptaremos
	conexiones como si fuese un servidor*/
	a= SNL_nuevo_grupo_conexiones(SIN_CONEXION);
	/*añadimos la conexión abierta al grupo*/
	SNL_nueva_conexion_grupo(conexion, a);
	/*enviamos el buffer que preparamos con la peticion HTTP
	y comprobamos que todo el buffer se enviara*/
	if(SNL_enviar_grupo_TCP(a, &texto[0], strlen(&texto[0]), 0) == 0){
		printf("enviado:\n\n");
		printf("%s\n\n", &texto[0]);
	}else{
		exit(-1);
	}
	/*cambiamos el tiempo de espera del grupo a que suceda algo en
	los sockets*/
	SNL_tiempo_espera_grupo(a, 2, 0);
	/*esta funcion se encargara de comprobar todas las conexiones
	del grupo menos la conexión de escucha*/
	SNL_comprobar_grupo_conexiones(a);
	/*volvemos a cambiar el tiempo de espera del grupo para dar
	mas tiempo al servidor para enviar la respuesta*/
	SNL_tiempo_espera_grupo(a, 5, 500000);
	/*imprimimos texto en la terminal*/
	printf("Esperando respuesta del servidor:\n\n");
	/*iniciamos un bucle while en el que la condicion es que
	la conexión en la que ha sucedido algo del grupo sea
	la conexión que iniciamos antes (esto siempre se cumplira
	ya que es la unica del grupo, a no ser que no haya ninguna
	y el retorno sea SIN_CONEXION*/
	while(SNL_conexion_activa_grupo_conexiones(a) == conexion){
		/*intentamos leer datos recividos en el socket y
		comprobamos si da error al leer datos recividos lo
		cual puede ser por que el servidor cerrase la conexión
		si devuelve 0 o por error en la recepcion de los datos
		o error en la conexión si es -1*/
		if(SNL_recibir_TCP(conexion, &texto[0], 512, 0) <= 0){
			printf("\n\nproblemas reciviendo datos del servidor,");
			printf(" puede que el servidor hubiese cerrado la conexion");
			/*salimos del bucle*/
			break;
		}else{
			/*como conseguimos recibir datos los mostramos en
			la terminal*/
			printf("%s", &texto[0]);
		}
		/*comprobamos si no hay ninguna conexión que este aun
		activa*/
		if(SNL_comprobar_grupo_conexiones(a) == -1){
			/*salimos del bucle*/
			break;
		}
	}
	printf("\n\nsi no se muestran datos recibidos desde el servidor puede ser");
	printf(" que el tiempo de espera se excedio del esperado");
	/*cerramos todas las conexiones del grupo y eliminamos el grupo
	liberando la memoria ocupada por este*/
	SNL_cerrar_grupo_conexiones(a);
	printf("\n\ndesconectado\n");
	/*copiamos a informacion el texto con la informacion de SNL
	y el 500 indica que el array puede guardar como maximo una
	cadena de 500 caracteres, sin contar el carácter nulo de fin
	de cadena de caracteres*/
	informacion_biblioteca_SNL(&informacion[0], 500);
	/*mostramos la informacion de SNL obtenida al principio*/
	printf("\n%s\n\n", &informacion[0]);
	printf("El texto anterior cifrado:\n");
	/*encriptamos el texto usando la funcion de cifrado de bits
	iguales que tiene SNL, esto es solo para mostrar un ejemplo de
	esta funcion*/
	EncriptarTexto_CifradoBitsIguales(&informacion[0], "Free software as in freedom");
	printf("\n%s\n\n", &informacion[0]);
	printf("Fin\n");
}
exit(0);
}
