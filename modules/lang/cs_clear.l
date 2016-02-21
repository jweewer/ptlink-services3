/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************  

  Description: cs_clear lang file

 *  $Id: cs_clear.l,v 1.1.1.1 2005/08/27 15:44:52 jpinto Exp $
*/
+++++ cs_clear.l
+++++ en_us: Lamego
+++++ nl: ^Stinger^, Ldp
+++++ pt_br: openglx, Ricardo
+++++ de: dp

+CLEAR_SUMMARY(en_us)
Clears all ops, voices, bans, users or modes from a channel
+CLEAR_SUMMARY(pt)
Remove todos os ops, voices, bans ou utilizadores de um canal.
+CLEAR_SUMMARY(nl)
Verwijdert alle ops, voices, bans, gebruikers of modes van een kanaal.
+CLEAR_SUMMARY(pt_br)
Remove todos os ops, voices, bans, usuários ou modos de um canal.
+CLEAR_SUMMARY(de)
Alle Ops, Voices, Bans, Benutzer und Modi von einem Channel entfernen.
+++++++++++++++++++++++++++++++++++++++++++++
+CLEAR_HELP(en_us)
The CLEAR command clears all ops, voices, bans, users of modes from a channel.

Syntax:
  // CLEAR #Chan OPS
  // CLEAR #Chan VOICES
  // CLEAR #Chan BANS
  // CLEAR #Chan USERS [message]
  // CLEAR #Chan MODES
+CLEAR_HELP(pt)
O comando CLEAR remove todos os ops, voices, bans ou utilizadores de um canal.

Sintaxe:
  // CLEAR #Canal OPS
  // CLEAR #Canal VOICES
  // CLEAR #Canal BANS
  // CLEAR #Canal USERS [mensagem]
  // CLEAR #Canal MODES
+CLEAR_HELP(nl)
Het CLEAR commando verwijdert alle ops, voices, bans, gebruikers of modes van een kanaal.

Syntax:
  // CLEAR #kanaal OPS
  // CLEAR #kanaal VOICES
  // CLEAR #kanaal BANS
  // CLEAR #kanaal USERS [bericht]
  // CLEAR #kanaal MODES
+CLEAR_HELP(pt_br)
O comando CLEAR remove todos os ops, voices, bans, usuários ou modos de um canal.

Sintaxe:
  // CLEAR #Canal OPS
  // CLEAR #Canal VOICES
  // CLEAR #Canal BANS
  // CLEAR #Canal USERS [mensagem]
  // CLEAR #Canal MODES
+CLEAR_HELP(de)
Der Befehl CLEAR entfernt alle Ops, Voices, Bans, Benutzer und Modi von einem
Channel.

Syntax:
  // CLEAR #Channel OPS
  // CLEAR #Channel VOICES
  // CLEAR #Channel BANS
  // CLEAR #Channel USERS [Nachricht]
  // CLEAR #Channel MODES
+++++++++++++++++++++++++++++++++++++++++++++
+CLEAR_SYNTAX(en_us)
Invalid Syntax
Syntax:
  // CLEAR #Chan OPS
  // CLEAR #Chan VOICES
  // CLEAR #Chan BANS
  // CLEAR #Chan USERS [message]
  // CLEAR #Chan MODES
+CLEAR_SYNTAX(pt)
Sintaxe inválida.
Sintaxe:
  // CLEAR #Canal OPS
  // CLEAR #Canal VOICES
  // CLEAR #Canal BANS
  // CLEAR #Canal USERS [mensagem]
  // CLEAR #Canal MODES
+CLEAR_SYNTAX(nl)
Ongeldige syntax.
Syntax:
  // CLEAR #kanaal OPS
  // CLEAR #kanaal VOICES
  // CLEAR #kanaal BANS
  // CLEAR #kanaal USERS [bericht]
  // CLEAR #kanaal MODES
+CLEAR_SYNTAX(pt_br)
Sintaxe inválida.
Sintaxe:
  // CLEAR #Canal OPS
  // CLEAR #Canal VOICES
  // CLEAR #Canal BANS
  // CLEAR #Canal USERS [mensagem]
  // CLEAR #Canal MODES
+CLEAR_SYNTAX(de)
Ungültige Syntax.
Syntax:
  // CLEAR #Channel OPS
  // CLEAR #Channel VOICES
  // CLEAR #Channel BANS
  // CLEAR #Channel USERS [Nachricht]
  // CLEAR #Channel MODES
+++++++++++++++++++++++++++++++++++++++++++++
+NO_CLEAR_PERM_ON_X(en_us)
You don't have CLEAR permission on \B%s\B !
+NO_CLEAR_PERM_ON_X(pt)
Não tem permissão para utilizar o comando CLEAR no \B%s\B
+NO_CLEAR_PERM_ON_X(nl)
U heeft heeft geen CLEAR permissie op \B%s\B
+NO_CLEAR_PERM_ON_X(pt_br)
Você não tem permissão para utilizar o comando CLEAR no \B%s\B
+NO_CLEAR_PERM_ON_X(de)
Du hast keine Berechtigung für CLEAR in \B%s\B!
+++++++++++++++++++++++++++++++++++++++++++++
+INVALID_CLEAR_TYPE_X(en_us)
\B%s\B is not a valid type for the // CLEAR command.
Available types are:
  OPS - Clear all channel operators
  VOICES - Clear all channel voices
  BANS - Clear all channel bans
  USERS - Clear all channel users
  MODES - Clear all channel modes
+INVALID_CLEAR_TYPE_X(pt)
\B%s\B não é um tipo válido para o comando // CLEAR 
Os tipos disponíveis são:
  OPS - Remove os ops do canal
  VOICES - Remove os voices do canal
  BANS - Remove todos os bans do canal
  USERS - Remove todos os utilizadores do canal
+INVALID_CLEAR_TYPE_X(nl)
\B%s\B is een ongeldig type voor het // CLEAR commando.
Beschikbare types zijn:
  OPS - Verwijder alle kanaal-operators
  VOICES - erwijder alle kanaal-voices
  BANS - Verwijder alle kanaal-verbanningen
  USERS - Verwijder alle gebruikers van het kanaal
  MODES - Verwijder alle modes van het kanaal
+INVALID_CLEAR_TYPE_X(pt_br)
\B%s\B não é um tipo válido para o comando // CLEAR
Os tipos disponíveis são:
  OPS - Remove os ops do canal
  VOICES - Remove os voices do canal
  BANS - Remove todos os bans do canal
  USERS - Remove todos os usuários do canal
  MODES - Remove todos os modos do canal
+INVALID_CLEAR_TYPE_X(de)
\B%s\B ist ein ungültiger Typ für den // CLEAR Befehl.
Gültige Typen sind:
  OPS - Alle Channel-Ops entfernen.
  VOICES - Alle Channel-Voices entfernen.
  BANS - Alle Channel-Bans entfernen.
  USERS - Alle Channel-Benutzer entfernen.
  MODES - Alle Channel-Modi entfernen.
+++++++++++++++++++++++++++++++++++++++++++++
+OPS_CLEARED_X(en_us)
All other chanops on \B%s\B were removed.
+OPS_CLEARED_X(pt)
Todos os outros ops no \B%s\B foram removidos.
+OPS_CLEARED_X(nl)
Alle andere operators van kanaal \B%s\B zijn verwijderd.
+OPS_CLEARED_X(pt_br)
Todos os outros ops no \B%s\B foram removidos.
+OPS_CLEARED_X(de)
Alle anderen Channel-Ops in \B%s\B wurden entfernt.
+++++++++++++++++++++++++++++++++++++++++++++
+VOICES_CLEARED_X(en_us)
All other voices on \B%s\B were removed.
+VOICES_CLEARED_X(pt)
Todos os outros voices no \B%s\B foram removidos.
+VOICES_CLEARED_X(nl)
Alle andere voices van kanaal \B%s\B zijn verwijderd.
+VOICES_CLEARED_X(pt_br)
Todos os outros voices no \B%s\B foram removidos.
+VOICES_CLEARED_X(de)
Alle anderen Channel-Voices in \B%s\B wurden entfernt.
+++++++++++++++++++++++++++++++++++++++++++++
+BANS_CLEARED_X(en_us)
All bans on \B%s\B were removed.
+BANS_CLEARED_X(pt)
Todos os bans no \B%s\B foram removidos.
+BANS_CLEARED_X(nl)
Alle verbanningen van kanaal \B%s\B zijn verwijderd.
+BANS_CLEARED_X(pt_br)
Todos os bans no \B%s\B foram removidos.
+BANS_CLEARED_X(de)
Alle Bans in \B%s\B wurden entfernt.
+++++++++++++++++++++++++++++++++++++++++++++
+USERS_CLEARED_X(en_us)
All other users were removed from \B%s\B .
+USERS_CLEARED_X(pt)
Todos os outros utilizadores foram removidos do \B%s\B .
+USERS_CLEARED_X(nl)
Alle andere gebruikers van kanaal \B%s\B zijn verwijderd.
+USERS_CLEARED_X(pt_br)
Todos os outros usuários foram removidos do \B%s\B.
+USERS_CLEARED_X(de)
Alle anderen Benutzer in \B%s\B wurden entfernt.
++++++++++++++++++++++++++++++++++++++++++++
+MODES_CLEARED_X(en_us)
All channel modes where removed from \B%s\B .
+MODES_CLEARED_X(pt)
Todos os \"modes\" foram retirados do canal \B%s\B .
+MODES_CLEARED_X(nl)
Alle kanaal modes van \B%s\B zijn verwijderd.
+MODES_CLEARED_X(pt_br)
Todos os modos foram retirados do canal \B%s\B.
+MODES_CLEARED_X(de)
Alle Channel-Modi in \B%s\B wurden entfernt.
