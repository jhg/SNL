##########################################################################
##                                                                      ##
## Archivo make para la instalacion de Simple network library.          ##
##                                                                      ##
## Simple network library Copyright (C) 2009 Jesús Hernández Gormaz     ##
##                                                                      ##
## Fecha de creacion: 22 de marzo del 2009                              ##
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
INSTALL = /usr
INSTALL_LIB = $(INSTALL)/lib
INSTALL_INCLUDE = $(INSTALL)/include/SNL
#### End of system configuration section.   ####

all:
	@echo Compilando Simple Network Library version 0.0.1
	@$(MAKE) -C ./src/
	@echo SNL compilada
	@echo Encontrara la biblioteca estatica en la carpeta lib y el archivo de cabecera en la carpeta include

install: all
	@echo Copiando los archivos de SNL a las carpetas del sistema apropiadas
	@echo Copiando libSNL.a
	cp ./lib/libSNL.a $(INSTALL_LIB)/
	@echo Creando el directorio SNL
	mkdir $(INSTALL_INCLUDE)
	@echo Copiando SNL.h
	cp ./include/SNL/SNL.h $(INSTALL_INCLUDE)/
	@echo Se copiaron los archivos de SNL a la carpeta $(INSTALL) del sistema apropiadas

uninstall:
	@echo Eliminando los archivos de SNL del sistema
	@echo Eliminando libSNL.a
	$(RM) $(INSTALL)/lib/libSNL.a
	@echo Eliminando SNL.h
	$(RM) $(INSTALL)/include/SNL/SNL.h
	@echo Eliminando el directorio SNL
	rmdir $(INSTALL)/include/SNL
	@echo Se eliminaron los archivos de SNL de la carpeta $(INSTALL) del sistema

.PHONY: clean all install uninstall ejemplo

.SILENT:

ejemplo:
	echo Compilando un programa de ejemplo
	$(MAKE) -C ./test/
	echo Programa de ejemplo compilado
	echo Encontrara el programa de ejemplo en la carpeta test

clean:
	echo Borrando los archivos innecesarios
	$(RM) -f *.o *.a *~ \#* core
	$(MAKE) -C ./src/ clean
	$(MAKE) -C ./test/ clean
	echo Borrando los archivos innecesarios de la carpeta lib
	cd lib ; $(RM) -f *.o *.a *~ \#* core

