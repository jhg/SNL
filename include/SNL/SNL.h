/*
Simple network library Copyright (C) 2009 Jesús Hernández Gormaz

Fecha de creacion: 22 de marzo del 2009

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



/*Compilacion condicionada que evita incluir por error mas de una vez este
 archivo de cabecera*/
#ifndef _SNL_SIMPLE_NETWORK_LIBRARY
#define _SNL_SIMPLE_NETWORK_LIBRARY



/*Definiendo la variable grupo_sockets para poder manejar grupos de sockets
 con los que comprobar si algun socket ha recivido datos o si estan
 disponibles nuevas conexiones en la cola de escucha que como servidor
 devamos aceptar*/
typedef struct grupo_sockets grupo_conexiones;

/*Estructura para el encapsulado de datos para su posterior envio por un
 socket. Permite el envio de informacion entre dos aplicaciones de forma
 facil*/
struct SNL_encapsulado{
	char bandera;
	char encabezado[42];
	char cuerpo[979];
	unsigned int checksum;
};

/*Creando un tipo nuevo a partir de la estructura SNL_encapsulado*/
typedef struct SNL_encapsulado SNL_paqueteSNL;

/*Creando el tipo nuevo SNL_datos*/
typedef void * SNL_datos;

/*Definiendo la macro SIN_CONEXION*/
#define SIN_CONEXION -1



/*Funciones de informacion interna de la libreria y de mantenimiento y
 recuperacion y notificacion de errores de la misma*/

/*añadiendo compatibilidad de las funciones con programas en C++ para que
 puedan ser usadas en los mismos*/ 
#ifdef _cplusplus
extern "C"
{
#endif

/*Recive un puntero a una cadena de caracteres y la longitud que admite
 y copia en el una cadena de caracteres con informacion sobre la libreria
 como version, fecha de compilacion, nombre de la biblioteca y otra
 informacion. Devuelve 0 si todo fue bien y la longitud necesaria para copiar
 la cadena de caracteres si no es lo suficiente larga*/
extern unsigned long int informacion_biblioteca_SNL(char *informacion, unsigned long int longitud);



/*Funciones criptograficas*/

/*Encripta un texto con una clave haciendo un XOR y al dicho entre cada
 byte de ambas cadenas un NOT. Para desencriptar se usa esta misma
 funcion con la misma clave. Esta es una funcion de criptografia simetrica*/
extern int EncriptarTexto_CifradoBitsIguales(unsigned char *texto, unsigned char *clave);



/*Funciones de suma de verificacion y digitos de verificación*/

/*Checksum de los bytes datos pasados*/
extern unsigned int checksum(SNL_datos datos, unsigned long longitud);



/*Funciones para trabajar con grupos de conexiones (de forma que se puede
 atender a varias conexiones sin quedar bloqueado)*/

/*Crea un nuevo grupo de conexiones y añade la conexion pasada como
 argumento como la conexion de escucha (esta conexion solo se ha de
 especificar para trabajar como servidor, si se trabaja como cliente
 especificar la conexion con la macro SIN_CONEXION*/
extern grupo_conexiones *SNL_nuevo_grupo_conexiones(int conexion_escucha);

/*Añade una conexion a un grupo de conexiones*/
extern int SNL_nueva_conexion_grupo(int conexion, grupo_conexiones *grupo);

/*Quita una conexion de un grupo de conexiones pero sin cerrarla*/
extern int SNL_quitar_conexion_grupo(int conexion, grupo_conexiones *grupo);

/*Quita una conexion de un grupo de conexiones y la desconecta*/
extern int SNL_desconectar_conexion_grupo(int conexion, grupo_conexiones *grupo);

/*Establece los segundos y microsegundos de espera a que algun socket de un
 grupo de conexiones haya recivido datos o haya una nueva conexion en la
 cola de escucha*/
extern int SNL_tiempo_espera_grupo(grupo_conexiones *grupo, int segundos, int microsegundos);

/*Establece el byte id del grupo de conexiones el cual puede ser usado de
 forma arbitraria por el programador para, por ejemplo, diferenciar en las
 funciones que programe unos determinados grupos de conexiones de otros*/
extern int SNL_cambiar_id_grupo(grupo_conexiones *grupo, char id);

/*Devuelve el byte id de un grupo de conexiones*/
extern char SNL_id_grupo(grupo_conexiones *grupo);

/*Devuelve 0 si una conexion forma parte de un grupo de conexiones dado o 1
 en caso de no formar parte del grupo de conexiones, si devuelve -1 es que
 ocurrio un error durante la comprobacion*/
extern int SNL_pertenece_a_grupo_conexiones(int conexion, grupo_conexiones *grupo);

/*Envia un mismo buffer de datos a todos los sockets de un grupo (ecepto al
 socket de escucha)*/
extern int SNL_enviar_grupo_TCP(grupo_conexiones *grupo, const SNL_datos datos, int bytes, int flags);

/*Comprueba el estado de las conexiones de un grupo esperando el tiempo que
 tenga establecido dicho grupo de forma que despues se pueda saber que
 conexiones han enviado datos o si hay nuevas conexiones en cola esperando
 ser aceptadas*/
extern int SNL_comprobar_grupo_conexiones(grupo_conexiones *grupo);

/*Informa sobre si hay conexiones pendientes conexiones entrantes en cola
 esperando ser aceptadas (para esto es necesario que el grupo se iniciara
 con una conexion de escucha), devuelve 0 si no hay ninguna conexion
 entrante y 1 en caso de que haiga alguna conexion esperando ser aceptada*/
extern int SNL_conexion_entrante_grupo(grupo_conexiones *grupo);

/*Devuelve una conexion que haya enviado datos de forma que podamos
 realizar las operaciones pertinentes como recivir los datos y enviar
 una respuesta. Hay que llamar antes a la funcion comprobar_grupo_conexiones
 y devuelve -1 en caso de no haber ninguna conexion que haiga enviado datos*/
extern int SNL_conexion_activa_grupo_conexiones(grupo_conexiones *grupo);

/*Devuelve la conexion de escucha de un grupo de conexiones o -1 si el grupo
 no tiene asignada la conexion de escucha*/
extern int SNL_conexion_escucha_grupo(grupo_conexiones *grupo);

/*Cambia la conexion de escucha del grupo por la pasada como argumento
 (no cierra la conexion que tenga el grupo como conexion de escucha antes
 del cambio)*/
extern int SNL_cambiar_escucha_grupo(grupo_conexiones *grupo, int conexion);

/*Libera un grupo de conexiones pero sin cerrar las conexiones del grupo*/
extern int SNL_liberar_grupo_conexiones(grupo_conexiones *grupo);

/*Libera un grupo de conexiones y cierra todas las conexiones del grupo*/
extern int SNL_cerrar_grupo_conexiones(grupo_conexiones *grupo);



/*Funciones para IPv4*/

/*Conectar a una IP del tipo IPv4 en el puerto indicado usando sockets TCP*/
extern int SNL_conectar_TCP_IPv4(char *direccion, unsigned short int puerto);

/*Comienza una conexion con sockets UDP en un puerto determinado (para
 servidores) o en cualquier puerto que decida el sistema si ponemos como
 puerto 0 (para clientes)*/
extern int SNL_conectar_UDP_IPv4(unsigned short int puerto);

/*Recive un buffer de datos de un socket UDP IPv4*/
extern int SNL_recibir_UDP_IPv4(int conexion, SNL_datos datos, int bytes, \
int flags, char *direccion_remota, unsigned short int *puerto_remoto, \
unsigned long int longitud_d);

/*Envia un buffer de datos por un socket UDP IPv4*/
extern int SNL_enviar_UDP_IPv4(int conexion, SNL_datos datos, int bytes, \
int flags, char *direccion_remota, unsigned short int puerto_remoto);

/*Poner un socket TCP a la escucha de conexiones con la direccion IP del
 tipo IPv4 que se indique (NULL para que se escoja automaticamente) en el
 puerto indicado y con el numero de conexiones entrantes en cola indicado*/
extern int SNL_escuchar_TCP_IPv4(char *direccion, unsigned short int puerto, int cola);

/*Aceptar la siguiente conexion entrante en cola de una conexion de escucha
 del tipo de sockets TCP y del tipo de IP IPv4*/
extern int SNL_aceptar_conexion_IPv4(int conexion_escucha, char *direccion_remota, int *puerto_remoto);

/*Obtiene por DNS la IP del tipo IPv4 del host indicado*/
extern int SNL_DNS_IPv4(char *direccion, char *host);



/*Funciones para IPv6*/

/*Conectar a una IP del tipo IPv6 en el puerto indicado usando sockets TCP*/
extern int SNL_conectar_TCP(char *direccion, unsigned short int puerto);

/*Comienza una conexion con sockets UDP en un puerto determinado (para
 servidores) o en cualquier puerto que decida el sistema si ponemos como
 puerto 0 (para clientes)*/
extern int SNL_conectar_UDP(unsigned short int puerto);

/*Recive un buffer de datos de un socket UDP IPv4*/
extern int SNL_recibir_UDP(int conexion, SNL_datos datos, int bytes, \
int flags, char *direccion_remota, unsigned short int *puerto_remoto, \
unsigned long int longitud_d);

/*Envia un buffer de datos por un socket UDP IPv4*/
extern int SNL_enviar_UDP(int conexion, SNL_datos datos, int bytes, int flags, \
char *direccion_remota, unsigned short int puerto_remoto);

/*Poner un socket TCP a la escucha de conexiones con la direccion IP del
 tipo IPv6 que se indique (NULL para que se escoja automaticamente) en el
 puerto indicado y con el numero de conexiones entrantes en cola indicado*/
extern int SNL_escuchar_TCP(char *direccion, unsigned short int puerto, int cola);

/*Aceptar la siguiente conexion entrante en cola de una conexion de escucha
 del tipo de sockets TCP y del tipo de IP IPv6*/
extern int SNL_aceptar_conexion(int conexion_escucha, char *direccion_remota, int *puerto_remoto, unsigned long int longitud_d);

/*Finaliza una conexion TCP o UDP ya sea con IPv4 o IPv6*/
extern int SNL_desconectar(int conexion);

/*Envia por una conexion ya establecida de sockets TCP e IPv6 el buffer de
 datos que se le pasa como puntero de la longitud de bytes indicada y con
 las flags indicados*/
extern int SNL_enviar_TCP(int conexion, const SNL_datos datos, int bytes, int flags);

/*Recive el buffer de datos de una conexion de sockets TCP IPv6 ya
 establecida y lo coloca en el buffer con capacidad para el numero de bytes
 indicado, con las flags que se indique*/
extern int SNL_recibir_TCP(int conexion, SNL_datos datos, int bytes, int flags);

/*Obtiene por DNS la IP del tipo IPv6 del host indicado*/
extern int SNL_DNS(char *direccion, char *host, char *servicio, unsigned long int longitud_d, unsigned long int longitud_s);



/*Funciones para la compatibilidad con windows*/

#ifdef WIN32
/*Iniciar la biblioteca para windows*/
extern int SNL_iniciar_W(void);

/*Acaba con el uso de la biblioteca para windows*/
extern int SNL_acabar_W(void);
#endif

/*Fin de la compatibilidad con programas escritos en C++*/
#ifdef _cplusplus
}
#endif

/*Fin de la compilacion condicional para evitar incluir este archivo de
 cabecera mas de una vez por error*/
#endif
