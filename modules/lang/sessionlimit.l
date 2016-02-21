++++ operserv.l
++++ en_us: Lamego, Stinger
++++ nl: Stinger, Ldp
++++ pt: Lamego
++++ pt_br: Ricardo

+OS_SESSION_SUMMARY(en_us)
Session management
+OS_SESSION_SUMMARY(nl)
Sessie beheer
+++++++++++++++++++++++++++++++++++++++++++++
+OS_SESSION_SYNTAX(en_us)
Syntax:
  // SESSION LIST number
  // SESSION VIEW host
+OS_SESSION_SYNTAX(nl)
Syntax:
  // SESSION LIST nummer
  // SESSION VIEW host
+++++++++++++++++++++++++++++++++++++++++++++  
+OS_SESSION_HELP(en_us)
On the irc services context, a session is defined as the set of
client connections from the a same host.
The // SESSION commands allows to view/manage client sessions.
The supported subcommands are:
  LIST - List sessions with connections above number
  VIEW  - View users on a session
You can get more help on these commands with:
  // HELP SESSION subcommand.
+OS_SESSION_HELP(nl)
Binnen de irc services is een sessie aangegeven als het aantal client
connecties vanaf een zelfde host.
De // SESSION commando's stellen u in staat de client sessies te 
bekijken/beheren.
De ondersteunde subcommando's zijn:
  LIST  - Lijst van connecties die meer zijn dan het opgegeven nummer
  VIEW  - Bekijk de gebruikers van een sessie
Meer informatie hier over kunt u vinden met:
  // HELP SESSION subcommando. 
+++++++++++++++++++++++++++++++++++++++++++++  
+OS_SESSION_LIST_SYNTAX(en_us)
Syntax:
  // SESSION LIST number
+OS_SESSION_LIST_SYNTAX(nl)
Syntax:
  // SESSION LIST nummer
+++++++++++++++++++++++++++++++++++++++++++++
+OS_SESSION_LIST_HEADER_X(en_us)
***** Sessions with more than \B%d\B connections *****
+OS_SESSION_LIST_HEADER_X(nl)
****** Sessies met meer dan \B%d\B connecties *******
+++++++++++++++++++++++++++++++++++++++++++++
+OS_SESSION_LIST_TAIL(en_us)
********************************
+OS_SESSION_LIST_TAIL(nl)
*************************************
+++++++++++++++++++++++++++++++++++++++++++++
+OS_SESSION_LIST_ITEM_X_X_X_X(en_us)
  %s %d/%d - \B%d\B Hit(s)
+OS_SESSION_LIST_ITEM_X_X_X_X(nl)
  %s %d/%d - \B%d\B overeenkomst(en)
+++++++++++++++++++++++++++++++++++++++++++++
+OS_SESSION_LIST_ITEM_X_X_X(en_us)
  %s %d/%d
+OS_SESSION_LIST_ITEM_X_X_X(nl)
  %s %d/%d
+++++++++++++++++++++++++++++++++++++++++++++
+OS_SESSION_VIEW_SYNTAX(en_us)
Syntax:
 // SESSION VIEW host
+OS_SESSION_VIEW_SYNTAX(nl)
Syntax:
  // SESSION VIEW host
+++++++++++++++++++++++++++++++++++++++++++++
+OS_SESSION_VIEW_HEADER_X(en_us)
***** Users connected from \B%s\B *****
+OS_SESSION_VIEW_HEADER_X(nl)
**** Gebruikers verbonden van \B%s\B *****
+++++++++++++++++++++++++++++++++++++++++++++
+OS_SESSION_VIEW_TAIL(en_us)
********************************
+OS_SESSION_VIEW_TAIL(nl)
****************************************
+++++++++++++++++++++++++++++++++++++++++++++
+OS_SESSION_VIEW_X_NOT_FOUND(en_us)
There are no users connected from \B%s\B !
+OS_SESSION_VIEW_X_NOT_FOUND(nl)
Er zijn geen gebruikers verbonden vanaf \B%s\B !
+++++++++++++++++++++++++++++++++++++++++++++
+OS_SESSION_VIEW_ITEM_X_X_X(en_us)
  %-20s %s :%s
+OS_SESSION_VIEW_ITEM_X_X_X(nl)
  %-20s %s :%s
+++++++++++++++++++++++++++++++++++++++++++++
+OS_EXCEPTION_SYNTAX(en_us)
Syntax:
  // EXCEPTION ADD owner time host limit reason
  // EXCEPTION DEL host
  // EXCEPTION LIST [mask]
  // EXCEPTION INFO mask
  // EXCEPTION CHANGE old_host new_host
+OS_EXCEPTION_SYNTAX(nl)
Syntax:
  // EXCEPTION ADD eigenaar tijd host limiet reden
  // EXCEPTION DEL host
  // EXCEPTION LIST [host]
  // EXCEPTION INFO host
+++++++++++++++++++++++++++++++++++++++++++++
+OS_EXCEPTION_SUMMARY(en_us)
Manage session limit exceptions
+OS_EXCEPTION_SUMMARY(nl)
Beheer sessie limiet uitzonderingen
+++++++++++++++++++++++++++++++++++++++++++++
+OS_EXCEPTION_HELP(en_us)
The command // EXCEPTION allows to add/del/list session limit exceptions.

@OS_EXCEPTION_SYNTAX
+OS_EXCEPTION_HELP(nl)
Het commando EXCEPTION kunt u sessie uitzonderingen 
toevoegen/verwijderen/bekijken.

@OS_EXCEPTION_SYNTAX
+++++++++++++++++++++++++++++++++++++++++++++
+OS_EXCEPTION_LIST_HEADER(en_us)
***** List of exceptions *****
+OS_EXCEPTION_LIST_HEADER(nl)
***** Lijst van uitzonderingen *****
+++++++++++++++++++++++++++++++++++++++++++++
+OS_EXCEPTION_LIST_TAIL(en_us)
******************************
+OS_EXCEPTION_LIST_TAIL(nl)
*************************************
+++++++++++++++++++++++++++++++++++++++++++++
+OS_EXCEPTION_LIST_X_X_X(en_us)
  %s %s %s
+OS_EXCEPTION_LIST_X_X_X(nl)
  %s %s %s
+++++++++++++++++++++++++++++++++++++++++++++
+OS_EXCEPTION_LIST_X_X_X_X(en_us)
  %s %s (\B%d\B in use) %s
+OS_EXCEPTION_LIST_X_X_X_X(pt)  
  %s %s (\B%d\B em uso) %s
+OS_EXCEPTION_LIST_X_X_X_X(nl)
  %s %s (\B%d\B in gebruik) %s
+++++++++++++++++++++++++++++++++++++++++++++
+OS_EXCEPTION_ADD_ALREADY_EXISTS_X(en_us)
There is already an exception for \B%s\B !
+OS_EXCEPTION_ADD_ALREADY_EXISTS_X(nl)
Er isal een uitzondering voor \B%s\B
+++++++++++++++++++++++++++++++++++++++++++++
+OS_EXCEPTION_ADD_OK_X(en_us)
Exception for \B%s\B was succesfully added.
+OS_EXCEPTION_ADD_OK_X(nl)
Uitzondering voor \B%s\B is succesvol toegevoegd.
+++++++++++++++++++++++++++++++++++++++++++++
+OS_EXCEPTION_NOT_FOUND_X(en_us)
Exception for \B%s\B was not found !
+OS_EXCEPTION_NOT_FOUND_X(nl)
Uitzondering voor \B%s\B niet gevonden !
+++++++++++++++++++++++++++++++++++++++++++++
+OS_EXCEPTION_DELETED_X(en_us)
Deleted exception for \B%s\B .
+OS_EXCEPTION_DELETED_X(nl)
Uitzondering voor \B%s\B verwijderd.
+++++++++++++++++++++++++++++++++++++++++++++
+OS_EXCEPTION_CHANGE_SYNTAX(en_us)
Syntax:

  // EXCEPTION CHANGE old_host new_host
+OS_EXCEPTION_CHANGE_SYNTAX(nl)
Syntax:
   
  // EXCEPTION CHANGE oude_host nieuwe_host
+++++++++++++++++++++++++++++++++++++++++++++
+OS_EXCEPTION_CHANGE_DENIED(en_us)
You are not allowed to change that exception !
+OS_EXCEPTION_CHANGE_DENIED(pt)
Não lhe é permitido alterar essa excepção !
+OS_EXCEPTION_CHANGE_DENIED(nl)
U heeft geen rechten om de exceptielijst aan te passen!
+++++++++++++++++++++++++++++++++++++++++++++
+OS_EXCEPTION_CHANGED(en_us)
The exception host was changed.
+OS_EXCEPTION_CHANGED(nl)
De exceptie van de host is veranderd
+++++++++++++++++++++++++++++++++++++++++++++
+OS_EXCEPTION_ALREADY_X_EXISTS(en_us)
There is already an exception for \B%s\B !
+OS_EXCEPTION_ALREADY_X_EXISTS(nl)
Er bestaat al een exceptie voot \B%s\B !
+++++++++++++++++++++++++++++++++++++++++++++
+OS_EXCEPTION_INVALID_HOST_X(en_us)
\B%s\B is not a valid hostname !
+OS_EXCEPTION_INVALID_HOST_X(pt)
\B%s\B não é um hostname válido !
+OS_EXCEPTION_INVALID_HOST_X(nl)
\B%s\B is geen geldige hostnaam !
