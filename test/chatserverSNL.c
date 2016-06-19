/*
chatserverSNL Copyright (C) 2009 Jesús Hernández Gormaz

Fecha de creacion: 8 de Junio del 2009

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
#include <SNL.h>

/*tipo de variable enumeracion para especificar un color de algo que el
 programa debe comunicar al usuario*/
typedef enum color {NoColor, normal, rojo, cyan, verde, azul, negro, amarillo, \
blanco, gris_claro, marron, purpura, gris_oscuro, verde_claro, cyan_claro, \
rojo_claro, purpura_claro} color;

/*funcion que manda a la terminal secuencias de escape ANSI para borrar la pantalla y colocar el cursor al comienzo de la misma*/
int secuencia_borrar_terminal();
/*funcion que manda a la terminal secuencias de escape ANSI para dar formato de color al texto en la terminal (el cambio se mantiene hasta que se vuelva a cambiar)*/
int secuencia_color_terminal(color c);

int main(int argc, char *argv[]){
/*iniciamos las variables*/
int conexion, conexion_escucha, puerto, e=0, i=0, p=1;
unsigned long int numero_clientes=0;
char buffer[1537], direccion[21];
/* Esta sera la publicidad que mostrara el servidor*/
char publicidad[]="\n\nPublicidad del servidor:\nVisita www.snl.ya.st\n\n\n\0";
/*el grupo de conexiones, con uno es suficiente*/
grupo_conexiones *clientes;
/*esto es puramente por decoracion y que quede bonito,
 solo borra la ventana de la terminal*/
secuencia_borrar_terminal();
/*escribimos en pantalla*/
printf("Conectando el servidor en el puerto 1500, espere.");
/*iniciamos una conexion de escucha (este ejemplo es usando IPv4,
 pero con IPv6 es practicamente igual pero usando las funciones que no
 acaban con _IPv4 el nombre de la funcion, consulte la documentacion para
 más información*/
conexion_escucha= SNL_escuchar_TCP_IPv4(NULL, 1500, 20);
/*comprobamos que el servidor este conectado*/
if(conexion == SIN_CONEXION){
	/*esta es otra funcion que simplemente es para que quede bonito el
	 ejemplo, imprime en pantalla una secuencia de escape ANSI que hace
	 que todo lo que se escriba despues se vea en rojo claro*/
	secuencia_color_terminal(rojo_claro);
	printf("Error iniciando la conexion del servidor en el puerto 1500.\n");
	/*con esto hacemos que al escribir no se escriba en rojo en pantalla
	 sino en el color predeterminado*/
	secuencia_color_terminal(NoColor);
	return -1;
}else{
	printf("Servidor conectado y aceptando clientes en el puerto ");
	secuencia_color_terminal(cyan);
	printf("1500");
	secuencia_color_terminal(NoColor);
	printf(".\n");
	/*esto si nos interesa, iniciamos un nuevo grupo de conexiones
	 pasando como argumento la conexion que usaremos para escuchar
	 conexiones entrantes*/
	clientes= SNL_nuevo_grupo_conexiones(conexion_escucha);
	/*comprobamos que se creara el nuevo grupo*/
	if(clientes == NULL){
		/*esto es que no se a creado, lo indicamos y acabamos el
		 programa*/
		secuencia_color_terminal(rojo_claro);
		printf("Error de memoria insuficiente.\n");
		secuencia_color_terminal(NoColor);
		return -1;
	}else{
		/*se creo correctamente el grupo, cambiamos el tiempo de espera
		 a que suceda algo en los sockets del grupo a un segundo y
		 500000 microsegundos (un segundo y medio)*/
		SNL_tiempo_espera_grupo(clientes, 1, 500000);
		/*esto es un bucle con el que estar atendiendo a los clientes
		continuamente sin que acabe el programa, deberemos acabarlo
		pulsando CONTROL+C o cerrando la terminal*/
		while(e == 0){
			printf("Comprobando conexiones entrantes, espere.\n");
			/*comprobamos si hay alguna conexion entrante*/
			while(SNL_conexion_entrante_grupo(clientes) == 1){
				printf("Nueva conexion entrante, espere.\n");
				/*aceptamos la conexion entrante como un buen
				 servidor*/
				conexion= SNL_aceptar_conexion_IPv4(conexion_escucha, &direccion[0], &puerto);
				/*comprobamos que se ha podido establecer la
				 conexion*/
				if(conexion == SIN_CONEXION){
					secuencia_color_terminal(rojo_claro);
					printf("Error aceptando conexion entrante.\n");
					secuencia_color_terminal(NoColor);
				}else{
					printf("Conexion entrante nueva aceptada:\n");
					secuencia_color_terminal(amarillo);
					printf("%s %i\n", &direccion[0], puerto);
					secuencia_color_terminal(NoColor);
					/*añadimos la conexion al grupo de
					 conexiones y comprobamos que no
					 hubo errores*/
					if(SNL_nueva_conexion_grupo(conexion, clientes) != 0){
						secuencia_color_terminal(rojo_claro);
						printf("Error de memoria en la conexion entrante.\n");
						secuencia_color_terminal(NoColor);
					}else{
						printf("La nueva conexion entrante ha sido aceptada sin problemas.\n");
						/*aumentamos el numero de clientes*/
						numero_clientes++;
						strcpy(&buffer[0], "Un cliente se ha conectado al servidor.\n");
						strcat(&buffer[0], "IP del cliente que se ha conectado: ");
						strcat(&buffer[0], &direccion[0]);
						strcat(&buffer[0], "\n\0");
						/*enviamos a todos los clientes
						 la direccion IP del nuevo
						 cliente y comprobamos que
						 no hubo ningun error enviando*/
						if(SNL_enviar_grupo_TCP(clientes, &buffer[0], 1536, 0) == 0){
							printf("Datos reenviados a todos los clientes sin problemas.\n");
						}else{
							secuencia_color_terminal(rojo_claro);
							printf("Error reenviando datos a todos los clientes.\n");
							secuencia_color_terminal(NoColor);
						}
					}
				}
			}
			printf("Conexiones entrantes comprobadas.\n");
			printf("Comprobando clientes conectados en busca de actividad en la conexion, espere.\n");
			conexion= SIN_CONEXION;
			/*comprobamos si hay alguna conexion activa en el
			 grupo de conexiones*/
			if(SNL_comprobar_grupo_conexiones(clientes) == 0){
				/*ahora obtenemos la conexion activa del grupo*/
				conexion= SNL_conexion_activa_grupo_conexiones(clientes);
				/*este bucle es para despues intentar obtener
				 otra conexion activa hasta atender a todas*/
				while(conexion > SIN_CONEXION){
					/*vaciamos el buffer, esto no es
					 relevante para el uso de SNL, pero
					 como este programa lo probare usando
					 telnet para conectarme a el consigo
					 asi que al enviar un mensaje no
					 muestre fragmentos de mensajes
					 anteriores*/
					for(i=0; i<1537; i++){
						buffer[i]= '\0';
					}
					/*recibimos los datos del cliente y
					 guardamos el valor retornado*/
					i= SNL_recibir_TCP(conexion, &buffer[0], 1536, 0);
					/*comprobamos que sucedio con el valor
					 retornado*/
					if(i == 0){
						printf("Desconexion del cliente.\n");
						/*quitamos la conexion del grupo
						 como la funcion de recivir datos
						 ya la cerro automaticamente*/
						if(SNL_quitar_conexion_grupo(conexion, clientes) == 0){
							printf("Conexion cerrada sin problemas.\n");
							/*ahora tenemos un cliente menos
							 conectado*/
							numero_clientes--;
							strcpy(&buffer[0], "Un cliente se ha desconectado del servidor.\n\0");
							/*informamos de esto a los
							 demas clientes*/
							if(SNL_enviar_grupo_TCP(clientes, &buffer[0], 1536, 0) == 0){
								printf("Datos reenviados a todos los clientes sin problemas.\n");
							}else{
								secuencia_color_terminal(rojo_claro);
								printf("Error reenviando datos a todos los clientes.\n");
								secuencia_color_terminal(NoColor);
							}
						}else{
							secuencia_color_terminal(rojo_claro);
							printf("Error cerrando la conexion.\n");
							secuencia_color_terminal(NoColor);
							numero_clientes--;
							strcpy(&buffer[0], "Un cliente se ha desconectado del servidor.\nOcurrio un error en la desconexion.\n\0");
							if(SNL_enviar_grupo_TCP(clientes, &buffer[0], 1536, 0) == 0){
								printf("Datos reenviados a todos los clientes sin problemas.\n");
							}else{
								secuencia_color_terminal(rojo_claro);
								printf("Error reenviando datos a todos los clientes.\n");
								secuencia_color_terminal(NoColor);
							}
						}
					}else if(i == -1){
						secuencia_color_terminal(rojo_claro);
						printf("Error recibiendo datos del cliente.\n");
						secuencia_color_terminal(NoColor);
						strcpy(&buffer[0], "Ocurrio un error reciviendo datos de un cliente.\n\0");
						/*informamos a todos los clientes
						 del error reciviendo los datos*/
						if(SNL_enviar_grupo_TCP(clientes, &buffer[0], 1536, 0) == 0){
							printf("Datos reenviados a todos los clientes sin problemas.\n");
						}else{
							secuencia_color_terminal(rojo_claro);
							printf("Error reenviando datos a todos los clientes.\n");
							secuencia_color_terminal(NoColor);
						}
					}else{
						printf("Datos recividos del cliente:\n");
						secuencia_color_terminal(verde);
						printf("%s\n", &buffer[0]);
						secuencia_color_terminal(NoColor);
						printf("Reenviando datos a todos los clientes, espere.\n");
						/*reenviamos los datos recividos
						 a todos los clientes*/
						if(SNL_enviar_grupo_TCP(clientes, &buffer[0], 1536, 0) == 0){
							printf("Datos reenviados a todos los clientes sin problemas.\n");
							/*esto es para despues
							 mostrar publicidad*/
							p++;
						}else{
							secuencia_color_terminal(rojo_claro);
							printf("Error reenviando datos a todos los clientes.\n");
							secuencia_color_terminal(NoColor);
						}
					}
					/*este es el motivo del while dentro del
					 que estamos, comprobamos nuevamente
					 las conexiones activas para atender a
					 todas*/
					conexion= SIN_CONEXION;
					SNL_comprobar_grupo_conexiones(clientes);
					conexion= SNL_conexion_activa_grupo_conexiones(clientes);
				}
			}
			printf("Actividad en las conexiones comprobadas.\n");
			/*esto se encarga de la publicidad*/
			if(p >= 20){
				p= 1;
				strcpy(&buffer[0], &publicidad[0]);
				strcat(&buffer[0], "Datos del servidor:\n - Ultima IP conectada: ");
				strcat(&buffer[0], &direccion[0]);
				strcat(&buffer[0], "\n\n\n\0");
				if(SNL_enviar_grupo_TCP(clientes, &buffer[0], 1536, 0) == 0){
					printf("Publicidad enviada a todos los clientes sin problemas.\n");
				}else{
					secuencia_color_terminal(rojo_claro);
					printf("Error enviando publicidad a todos los clientes.\n");
					secuencia_color_terminal(NoColor);
				}
			}
			/*esto es tambien para que quede bonito, y para evitar
			 consumir demasiada CPU, para el programa 1 segundo,
			 esto en GNU/Linux funciona, pero en otros sistemas no
			 puedo asegurarlo, como no es relevante puede eliminarse
			 para compilar si da error solo que la pantalla de
			 la terminal parpadeara al borrar el texto y
			 escribir nuevamente*/
			sleep(1);
			secuencia_borrar_terminal();
			printf("Servidor conectado y aceptando clientes en el puerto ");
			secuencia_color_terminal(cyan);
			printf("1500");
			secuencia_color_terminal(NoColor);
			printf(".\n");
			printf("Total de clientes conectados: ");
			secuencia_color_terminal(cyan);
			printf("%lu", numero_clientes);
			secuencia_color_terminal(NoColor);
			printf(".\n");
			printf("Ultimos datos recividos del cliente:\n");
			secuencia_color_terminal(verde);
			printf("%s", &buffer[0]);
			secuencia_color_terminal(NoColor);
			printf("\n");
		}
		return 0;
	}
}
}

/*funcion que manda a la terminal secuencias de escape ANSI para borrar la pantalla y colocar el cursor al comienzo de la misma*/
int secuencia_borrar_terminal(){
printf("\n");
printf("%c[2J", 27);
printf("%c[0;0f", 27);
secuencia_color_terminal(normal);
return 0;
}

/*funcion que manda a la terminal secuencias de escape ANSI para dar formato de color al texto en la terminal (el cambio se mantiene hasta que se vuelva a cambiar)*/
int secuencia_color_terminal(color c){
	register int r=0;
	printf("%c[", 27);
	switch(c){
		case rojo: printf("0;31");
			break;
		case rojo_claro: printf("1;31");
			break;
		case verde: printf("0;32");
			break;
		case verde_claro: printf("1;32");
			break;
		case amarillo: printf("1;33");
			break;
		case marron: printf("0;33");
			break;
		case gris_oscuro: printf("1;30");
			break;
		case gris_claro: printf("0;37");
			break;
		case azul: printf("0;34");
			break;
		case purpura: printf("0;35");
			break;
		case purpura_claro: printf("1;35");
			break;
		case cyan: printf("0;36");
			break;
		case cyan_claro: printf("1;36");
			break;
		case blanco: printf("1;37");
			break;
		default: printf("0;00");
			break;
	}
	printf("m");
	return r;
}
