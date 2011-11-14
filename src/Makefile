##########################################################################
##                                                                      ##
## Archivo make para la compilacion de Simple network library.          ##
##                                                                      ##
## Simple network library Copyright (C) 2009 Jesús Hernández Gormaz     ##
##                                                                      ##
## Fecha de creacion: 22 de marzo del 2009 (Siglo XXI)                  ##
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

CFLAGS=-I../include/SNL/

libSNL.a: SNL.o
	@echo Empaquetando el codigo objeto en la biblioteca estatica
	$(AR) rs $@ $^
	@echo Moviendo la biblioteca de la carpeta del codigo fuente a la carpeta de las bibliotecas estaticas
	mv $@ ../lib/
	@echo La biblioteca $@ ha sido compilada

SNL.o: SNL.c
	@echo Compilando el codigo fuente
	$(CC) -g $(CFLAGS) -c -o $@ $^

.SILENT:

.PHONY: clean

clean:
	echo Borrando los archivos innecesarios de la carpeta src
	$(RM) -f *.o *.a *~ \#* core

