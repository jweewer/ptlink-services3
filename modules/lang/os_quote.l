++ **********************************************************************
++ * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
++ *                     http://software.pt-link.net                    *
++ * This program is distributed under GNU Public License               *
++ * Please read the file COPYING for copyright information.            *
++ **********************************************************************
++
++  Desc: os_quote language file
++
++ *  $Id: os_quote.l,v 1.2 2005/08/30 13:46:26 waxweazle Exp $
++

+QUOTE_SUMMARY(en_us)
Quotes management functions.
+QUOTE_SUMMARY(nl)
Quote beheer functies.
+QUOTE_SUMMARY(pt_br)
Funções de gerenciamento de cotas.
+QUOTE_SUMMARY(de)
Verwaltung der Quote Funktionen.
+++++++++++++++++++++++++++++++++++++++++++
+QUOTE_HELP(en_us)
The QUOTE command provides the functions required to manage quotes.

Syntax:
  // QUOTE ADD quote
  // QUOTE DEL id
  // QUOTE LIST [sqlmask]
+QUOTE_HELP(nl)
Het QUOTE commando verzorgt de functies om quotes te kunnen beheren.

Syntax:
  // QUOTE ADD quote
  // QUOTE DEL id
  // QUOTE LIST [sqlmask]
+QUOTE_HELP(pt_br)
O comando QUOTE fornece as funções necessárias para gerenciar cotas.

Sintaxe:
  // QUOTE ADD cota
  // QUOTE DEL id
  // QUOTE LIST [máscaraSQL]
+QUOTE_HELP(de)
Der QUOTE Befehl stellt die Funktionen zur Verwaltung von Quotes zur Verfügung.

Syntax:
  // QUOTE ADD Quote
  // QUOTE DEL ID
  // QUOTE LIST [Sqlmaske]
+++++++++++++++++++++++++++++++++++++++++++
+QUOTE_NEEDS_OPER(en_us)
You need to be a services operator to use this command!
+QUOTE_NEEDS_OPER(nl)
U moet services operator zijn om dit commando te gebruiken!
+QUOTE_NEEDS_OPER(pt_br)
Você precisa ser um Operador dos Services para usar este comando!
+QUOTE_NEEDS_OPER(de)
Du musst ServiceOperator sein, um diesen Befehl benutzen zu können!
+++++++++++++++++++++++++++++++++++++++++++
+INVALID_QUOTE_SYNTAX(en_us)
Invalid syntax !
Syntax:
  // QUOTE ADD quote
  // QUOTE DEL id
  // QUOTE LIST [sqlmask]
+INVALID_QUOTE_SYNTAX(nl)
Ongeldige syntax !
Syntax:
  // QUOTE ADD quote
  // QUOTE DEL id
  // QUOTE LIST [sqlmask]
+INVALID_QUOTE_SYNTAX(pt_br)
Sintaxe Inválida!
Sintaxe:
  // QUOTE ADD cota
  // QUOTE DEL id
  // QUOTE LIST [máscaraSQL]
+INVALID_QUOTE_SYNTAX(de)
Ungültige Syntax !
Syntax:
  // QUOTE ADD Quote
  // QUOTE DEL ID
  // QUOTE LIST [Sqlmaske]
++++++++++++++++++++++++++++++++++++++++++++
+QUOTE_X_INSERTED(en_us)
Quote inserted with id \B%d\B .
+QUOTE_X_INSERTED(nl)
Quote toegevoegd met id \B%d\B .
+QUOTE_X_INSERTED(pt_br)
Cota inserida com id \B%d\B.
+QUOTE_X_INSERTED(de)
Quote mit der ID \B%d\B eingefügt.
++++++++++++++++++++++++++++++++++++++++++++
+QUOTE_STR(en_us)
##########
%s
##########
+QUOTE_STR(pt)
##########
%s
##########
+QUOTE_STR(nl)
##########
%s
##########
+QUOTE_STR(pt_br)
##########
%s
##########
+QUOTE_STR(de)
##########
%s
##########
+++++++++++++++++++++++++++++++++++++++++++
+QUOTE_X_DELETED(en_us)
Quote \B%d\B was deleted.
+QUOTE_X_DELETED(nl)
Quote \B%d\B is verwijderd.
+QUOTE_X_DELETED(pt_br)
A cota \B%d\B foi apagada.
+QUOTE_X_DELETED(de)
Quote \B%d\B wurde gelöscht.
+++++++++++++++++++++++++++++++++++++++++++
+QUOTE_X_NOT_FOUND(en_us)
Quote \B%d\B was not found.
+QUOTE_X_NOT_FOUND(nl)
Kan quote \B%d\B niet vinden.
+QUOTE_X_NOT_FOUND(pt_br)
A cota \B%d\B não foi encontrada.
+QUOTE_X_NOT_FOUND(de)
Quote \B%d\B wurde nicht gefunden.
+++++++++++++++++++++++++++++++++++++++++++
+QUOTE_LIST_HEADER(en_us)
******** List of quotes
+QUOTE_LIST_HEADER(nl)
******** Quote lijst
+QUOTE_LIST_HEADER(pt_br)
******** Lista de cotas
+QUOTE_LIST_HEADER(de)
******** Liste der Quotes
+++++++++++++++++++++++++++++++++++++++++++
+QUOTE_LIST_TAIL(en_us)
******** End of list
+QUOTE_LIST_TAIL(nl)
******** Einde van de lijst
+QUOTE_LIST_TAIL(pt_br)
******** Fim da lista
+QUOTE_LIST_TAIL(de)
******** Ende der Liste
+++++++++++++++++++++++++++++++++++++++++++
+QUOTE_ITEM_X_X_X(en_us)
\B%d\B by %s : %s
+QUOTE_ITEM_X_X_X(nl)
\B%d\B door %s : %s
+QUOTE_ITEM_X_X_X(pt_br)
\B%d\B por %s : %s
+QUOTE_ITEM_X_X_X(de)
\B%d\B von %s : %s
