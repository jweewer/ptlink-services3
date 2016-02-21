++ **********************************************************************
++ * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
++ *                     http://software.pt-link.net                    *
++ * This program is distributed under GNU Public License               *
++ * Please read the file COPYING for copyright information.            *
++ **********************************************************************
++
++  Desc: os_sline language file
++
++ *  $Id: os_sline.l,v 1.2 2005/08/30 13:46:26 waxweazle Exp $
++

+SLINE_SUMMARY(en_us)
Manages slines.
+SLINE_SUMMARY(nl)
Beheer slines.
+SLINE_SUMMARY(pt_br)
Gerencia as slines.
+SLINE_SUMMARY(de)
Verwaltet Slines.
++++++++++++++++++++++++++++++++++++++++++++
+SLINE_HELP(en_us)
Allows to manage services lines:
The supported line letters are:
  V - SVLINES - Block filenames on DCCs or strings on PRIVMSG
  Q - SQLINES - Do not allow to use a given nick or nick mask

Example:
  // ADD V !forbidden.exe Attempt to send virus file.
  // ADD Q *serv* This is a services reserved nick.
+SLINE_HELP(nl)
Staat toe de slines te beheren:
De ondersteunde lijnletters zijn:
  V - SVLINES - Blokkeer bestandsnamen voor DCC of strings op een PRIVMSG
  Q - SQLINES - Sta een opgegeven bijnaam of bijnaammasker niet toe
  
Voorbeeld:
  // ADD V !forbidden.exe Probeert virus te versturen.
  // ADD Q *serv* Dit is gereserveerd voor de services
+SLINE_HELP(pt_br)
Permite o gerenciamento das Services Lines.
As seguintes letras para Services Lines são suportadas:
  V - SVLINES - Bloqueia nomes de arquivos no DCC ou palavras em PRIVMSG
  Q - SQLINES - Bloqueia o uso de um determinado nick ou máscara de nick

Exemplo:
  // ADD V !forbidden.exe Tentou enviar um arquivo infectado por vírus.
  // ADD Q *serv* Este é um nick reservado para uso dos Services
+SLINE_HELP(de)
Erlaubt die VErwaltung der Services lines:
Folgenede line-Zeichen werden unterstützt:
  V - SVLINES - Blockiere Dateinamen bei DCCs oder Zeichenketten bei PRIVMSG
  Q - SQLINES - Verbiete die Verbindung eines Nicks oder einer Nickmaske

Beispiel:
  // ADD V !forbidden.exe Versuch einen Virus zu senden.
  // ADD Q *serv* Nicks dürfen serv nicht im Namen haben. Reserviert vür Services.
++++++++++++++++++++++++++++++++++++++++++++
+SLINE_SYNTAX(en_us)
Invalid Syntax!
Syntax:
  // SLINE ADD letter mask message
  
  // SLINE DEL id
  
  // SLINE LIST [letter]
+SLINE_SYNTAX(nl)
Ongeldige syntax!
Syntax:
  // SLINE ADD letter masker bericht
  
  // SLINE DEL id
  
  // SLINE LIST [letter]
+SLINE_SYNTAX(pt_br)
Sintaxe inválida!
Sintaxe:
  // SLINE ADD letra máscara mensagem
  
  // SLINE DEL id
  
  // SLINE LIST [letra]
+SLINE_SYNTAX(de)
Ungültige Syntax!
Syntax:
  // SLINE ADD Letter Maske Nachricht
  
  // SLINE DEL ID
  
  // SLINE LIST [Letter]
++++++++++++++++++++++++++++++++++++++++++++
+SLINE_ALREADY_EXISTS_X_X(en_us)
There is already an S%cLINE with mask \B%s\B .
+SLINE_ALREADY_EXISTS_X_X(nl)
Er bestaat al een S%cLINE met masker %s .
+SLINE_ALREADY_EXISTS_X_X(pt_br)
Já existe uma S%cLINE com a máscara \B%s\B.
+SLINE_ALREADY_EXISTS_X_X(de)
Es gibt bereits eine S%cLINE mit der Maske \B%s\B.
++++++++++++++++++++++++++++++++++++++++++++
+ADDED_SLINE_X_X(en_us)
Added S%cLINE with id %d .
+ADDED_SLINE_X_X(nl)
S%cLINE toegevoegt met id %d .
+ADDED_SLINE_X_X(pt_br)
Adicionada S%cLINE com id %d.
+ADDED_SLINE_X_X(de)
S%cLINE mit der ID %d hinzugefügt.
++++++++++++++++++++++++++++++++++++++++++++
+SLINE_X_NOT_FOUND(en_us)
There is no sline with id \B%d\B.
+SLINE_X_NOT_FOUND(nl)
Sline met id %d bestaat niet.
+SLINE_X_NOT_FOUND(pt_br)
Não existe uma sline com id \B%d\B.
+SLINE_X_NOT_FOUND(de)
Es gibt keine Sline mit der ID \B%d\B.
++++++++++++++++++++++++++++++++++++++++++++
+DELETED_SLINE_X(en_us)
Deleted sline \B%d\B .
+DELETED_SLINE_X(nl)
Sline %d verwijderd.
+DELETED_SLINE_X(pt_br)
Sline \B%d\B apagada.
+DELETED_SLINE_X(de)
Sline %d gelöscht.
++++++++++++++++++++++++++++++++++++++++++++
+SLINE_LIST_HEADER_X(en_us)
************* SLINE LIST (\B%d\B) ***************
+SLINE_LIST_HEADER_X(nl)
************** SLINE LIJST (%d) *****************
+SLINE_LIST_HEADER_X(pt_br)
************ Lista de SLINE (\B%d\B) ************
+SLINE_LIST_HEADER_X(de)
************* SLINELISTE (\B%d\B) ***************
++++++++++++++++++++++++++++++++++++++++++++
+SLINE_LIST_FORMAT(en_us)
%5d %c %-20s Reason: %s (by %s on %s)
+SLINE_LIST_FORMAT(nl)
%5d %c %-20s Reden: %s (door: %s op %s)
+SLINE_LIST_FORMAT(pt_br)
%5d %c %-20s Motivo: %s (por %s em %s)
+SLINE_LIST_FORMAT(de)
%5d %c %-20s Grund: %s (von %s am %s)
++++++++++++++++++++++++++++++++++++++++++++
+SLINE_LIST_TAIL(en_us)
*************************************************
+SLINE_LIST_TAIL(nl)
*************************************************
+SLINE_LIST_TAIL(pt_br)
*************************************************
+SLINE_LIST_TAIL(de)
*************************************************
