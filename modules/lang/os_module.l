++ **********************************************************************
++ * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
++ *                     http://software.pt-link.net                    *
++ * This program is distributed under GNU Public License               *
++ * Please read the file COPYING for copyright information.            *
++ **********************************************************************
++
++  Desc: os_sline language file
++
++ *  $Id: os_module.l,v 1.2 2005/08/30 13:46:26 waxweazle Exp $
++

+MODULE_SUMMARY(en_us)
Modules managment functions.
+MODULE_SUMMARY(nl)
Module managementfuncties.
+MODULE_SUMMARY(pt_br)
Funções de gerenciamento dos módulos.
+MODULE_SUMMARY(de)
funktionen zum Modulmanagement.
+++++++++++++++++++++++++++++++++++++++++++
+MODULE_HELP(en_us)
The MODULE command provides the functions required to manage modules.

Syntax:
  // MODULE LIST [mask]
  // MODULE LOAD module
  // MODULE UNLOAD module
+MODULE_HELP(nl)
Het MODULE commando biedt de functies aan om de modules te beheren.

Syntax:
  // MODULE LIST [mask]
  // MODULE LOAD module
  // MODULE UNLOAD module
+MODULE_HELP(pt_br)
O comando MODULE fornece funções necessárias para gerenciar módulos.

Sintaxe:
  // MODULE LIST [mascara]
  // MODULE LOAD módulo
  // MODULE UNLOAD módulo
+MODULE_HELP(de)
Der MODULE-Befehl stellt Funktionen zur Verfügung, 
die zur Modulverwaltung benötigt werden

Syntax:
  // MODULE LIST [Maske]
  // MODULE LOAD Modul
  // MODULE UNLOAD Modul
+++++++++++++++++++++++++++++++++++++++++++
+INVALID_MODULE_SYNTAX(en_us)
Invalid syntax!
Syntax:
  // MODULE LIST [mask]
  // MODULE LOAD module
  // MODULE UNLOAD module
+INVALID_MODULE_SYNTAX(nl)
Ongeldige syntax:
  // MODULE LIST [mask]
  // MODULE LOAD module
  // MODULE UNLOAD module
+INVALID_MODULE_SYNTAX(pt_br)
Sintaxe inválida!
Sintaxe:
  // MODULE LIST [máscara]
  // MODULE LOAD módulo
  // MODULE UNLOAD módulo
+INVALID_MODULE_SYNTAX(de)
Ungültige Syntax!
Syntax:
  // MODULE LIST [Maske]
  // MODULE LOAD Modul
  // MODULE UNLOAD Modul
+++++++++++++++++++++++++++++++++++++++++++
+MODULE_LIST_HEAD(en_us)
Module          Version   Description
-------------------------------------------
+MODULE_LIST_HEAD(pt)
Módulo          Versão   Descrição
-------------------------------------------
+MODULE_LIST_HEAD(nl)
Module          Versie    Omschrijving
-------------------------------------------
+MODULE_LIST_HEAD(pt_br)
Módulo          Versão   Descrição
-------------------------------------------
+MODULE_LIST_HEAD(de)
Modul           Version   Beschreibung
+++++++++++++++++++++++++++++++++++++++++++
+MODULE_LIST_X_X_X(en_us)
%-16s %-5s %s
+MODULE_LIST_X_X_X(nl)
%-16s %-5s %s
+MODULE_LIST_X_X_X(pt_br)
%-16s %-5s %s
+MODULE_LIST_X_X_X(de)
%-16s %-5s %s
+++++++++++++++++++++++++++++++++++++++++++
+MODULE_LIST_TAIL(en_us)
-------------------------------------------
+MODULE_LIST_TAIL(pt)
-------------------------------------------
+MODULE_LIST_TAIL(nl)
-------------------------------------------
+MODULE_LIST_TAIL(pt_br)
-------------------------------------------
+MODULE_LIST_TAIL(de)
-------------------------------------------
+++++++++++++++++++++++++++++++++++++++++++
+MODULE_NEED_ROOT(en_us)
You need to be services root for the MODULE command.
+MODULE_NEED_ROOT(nl)
U moet services root zijn voor het MODULE commando.
+MODULE_NEED_ROOT(pt_br)
Você precisa ser um Services Root para utilizar o comando MODULE.
+MODULE_NEED_ROOT(de)
Du musst Services Root sein, um den MODULE - BEFEHL verwenden zu dürfen.
+++++++++++++++++++++++++++++++++++++++++++
+MODULE_X_NOT_LOADED(en_us)
The module \B%s\B is not loaded !
+MODULE_X_NOT_LOADED(nl)
De module \B%s\B is niet geladen !
+MODULE_X_NOT_LOADED(pt_br)
O módulo \B%s\B não está carregado!
+MODULE_X_NOT_LOADED(de)
Das Modul \B%s\B ist nicht geladen!
+++++++++++++++++++++++++++++++++++++++++++
+MODULE_EVENT_IN_USE_X_X(en_us)
The event \B%s\B is in use by module \B%s\B !
+MODULE_EVENT_IN_USE_X_X(nl)
Event \B%s\B is in gebruik door module \B%s\B !
+MODULE_EVENT_IN_USE_X_X(pt_br)
O evento \B%s\B está em uso pelo módulo \B%s\B!
+MODULE_EVENT_IN_USE_X_X(de)
Das Ereignis \B%s\B wird vom Modul \B%s\B verwendet!
+++++++++++++++++++++++++++++++++++++++++++
+MODULE_FUNC_IN_USE_X_X(en_us)
The function \B%s\B is in use by module \B%s\B !
+MODULE_FUNC_IN_USE_X_X(nl)
Functie \B%s\B is ingebruik door module \B%s\B !
+MODULE_FUNC_IN_USE_X_X(pt_br)
A função \B%s\B está em uso pelo módulo \B%s\B!
+MODULE_FUNC_IN_USE_X_X(de)
Die Funktion \B%s\B wird vom Modul \B%s\B verwendet!
+++++++++++++++++++++++++++++++++++++++++++
+MODULE_UNLOADING_X(en_us)
Unloading module \B%s\B.
+MODULE_UNLOADING_X(nl)
Uitladen module \B%s\B.
+MODULE_UNLOADING_X(pt_br)
Descarregando módulo \B%s\B.
+MODULE_UNLOADING_X(de)
Das Modul \B%s\B wird entladen.
+++++++++++++++++++++++++++++++++++++++++++
+MODULE_UNLOADED(en_us)
Module unloaded.
+MODULE_UNLOADED(nl)
Module uitgeladen.
+MODULE_UNLOADED(pt_br)
Módulo descarregado.
+MODULE_UNLOADED(de)
Das Modul ist entladen.
+++++++++++++++++++++++++++++++++++++++++++
+MODULE_ALREADY_LOADED_X(en_us)
Module \B%s\B is already in memory.
+MODULE_ALREADY_LOADED_X(nl)
Module \B%s\B aanwezig in geheugen.
+MODULE_ALREADY_LOADED_X(pt_br)
Módulo \B%s\B já está na memória.
+MODULE_ALREADY_LOADED_X(de)
Das Module \B%s\B befindet sich bereits im Speicher.
+++++++++++++++++++++++++++++++++++++++++++
+CANT_UNLOAD_SELF(en_us)
You can't unload the \"module\" module !
+CANT_UNLOAD_SELF(pt)
Não pode fazer unload do modulo \"module\" !
+CANT_UNLOAD_SELF(nl)
De module \"module\" kan niet worden uitgeladen !
+CANT_UNLOAD_SELF(pt_br)
Você não pode descarregar o módulo \"module\"!
+CANT_UNLOAD_SELF(de)
Das Modul \"module\" kann nicht entladen werden!
+++++++++++++++++++++++++++++++++++++++++++