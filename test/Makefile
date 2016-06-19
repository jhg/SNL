##########################################################################
##                                                                      ##
## Archivo make para la compilacion de ejemplos de programas usando SNL ##
##                                                                      ##
## testSNL Copyright (C) 2009 Jesús Hernández Gormaz                    ##
## chatserverSNL Copyright (C) 2009 Jesús Hernández Gormaz              ##
##                                                                      ##
## Fecha de creacion: 8 de Junio del 2009 (Siglo XXI)                   ##
##                                                                      ##
## This program is free software; you can redistribute it and/or        ##
## modify it under the terms of the GNU General Public License as       ##
## published by the Free Software Foundation; either version 3, or      ##
## (at your option) any later version.                                  ##
## This program is distributed in the hope that it will be useful,      ##
## but WITHOUT ANY WARRANTY; without even the implied warranty of       ##
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the         ##
## GNU General Public License for more details.                         ##
## You should have received a copy of the GNU General Public License    ##
## along with this program; if not, write to the Free Software          ##
## Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.            ##
##                                                                      ##
## Este programa es software libre. Puede redistribuirlo y/o            ##
## modificarlo bajo los términos de la Licencia Pública General         ##
## de GNU según es publicada por la Free Software Foundation,           ##
## bien de la versión 3 de dicha Licencia o bien (según su              ##
## elección) de cualquier versión posterior.                            ##
## Este programa se distribuye con la esperanza de que sea              ##
## útil, pero SIN NINGUNA GARANTÍA, incluso sin la garantía             ##
## MERCANTIL implícita o sin garantizar la CONVENIENCIA PARA UN         ##
## PROPÓSITO PARTICULAR. Para más detalles, véase la Licencia           ##
## Pública General de GNU.                                              ##
## Debería haber recibido una copia de la Licencia Pública              ##
## General junto con este programa. En caso contrario, escriba          ##
## a la Free Software Foundation, Inc., en 675 Mass Ave,                ##
## Cambridge, MA 02139, EEUU.                                           ##
##                                                                      ##
##########################################################################

#### Start of system configuration section. ####
srcdir = .
VPATH = .
CC=gcc
#### End of system configuration section.   ####

CFLAGS=-L/usr/lib -L../lib -I/usr/include/SNL -I../include/SNL/

all: testSNL chatserverSNL

testSNL: testSNL.o
	@echo Enlazando el programa y la biblioteca estatica SNL
	$(CC) $(CFLAGS) -o $@ $^ -lSNL

testSNL.o: testSNL.c
	@echo Compilando el programa de ejemplo
	$(CC) $(CFLAGS) -c -o $@ $^

chatserverSNL: chatserverSNL.o
	@echo Enlazando el programa y la biblioteca estatica SNL
	$(CC) $(CFLAGS) -o $@ $^ -lSNL

chatserverSNL.o: chatserverSNL.c
	@echo Compilando el programa de ejemplo de un servidor de chat
	$(CC) -g $(CFLAGS) -c -o $@ $^

.SILENT:

.PHONY: clean

clean:
	echo Borrando los archivos innecesarios de la carpeta test
	$(RM) -f *.o *.a *~ \#* core

