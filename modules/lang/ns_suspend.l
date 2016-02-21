++++ ns_register.l
++++ en_us: Lamego, Stinger
++++ nl: Stinger, Ldp
++++ pt: Lamego
++++ pt_br: Ricardo
++++ de: jo

++++++++++++++++++++++++++++++++++++++++++++
+NS_SUSPEND_SUMMARY(en_us)
Forbid a nick to be used for an amount of time.
+NS_SUSPEND_SUMMARY(nl)
Verbied een bijnaam van gebruik voor een bepaalde tijd
++++++++++++++++++++++++++++++++++++++++++++
+NS_SUSPEND_SYNTAX(en_us)
Syntax:
  // SUSPEND [+time(h|d|M|Y)] nick reason
  // SUSPEND LIST
+NS_SUSPEND_SYNTAX(nl)
Syntax:
  // SUSPEND [tijd(h|d|M|Y)] bijnaam reden
  // SUSPEND LIST
+NS_SUSPEND_SYNTAX(pt)
Syntax:
  // SUSPEND [+tempo(h|d|M|Y)] nick motivo
  // SUSPEND LIST
++++++++++++++++++++++++++++++++++++++++++++
+NS_SUSPEND_HELP(en_us)
The // SUSPEND command forbids a nick from be used for an amount of time

@NS_SUSPEND_SYNTAX
Example:
  // SUSPEND [+10d] smoker No smokers allowed.
Nick smoker can't be used on the next 10d .
+NS_SUSPEND_HELP(nl)
Het // SUSPEND commando verbied een naam van gebruik voor een bepaalde tijd 

@NS_SUSPEND_SYNTAX
Voorbeeld:
  // SUSPEND [+15d] blaat Stop met blaten
Bijnaam blaat kan 15 dagen niet worden gebruikt.
++++++++++++++++++++++++++++++++++++++++++++
+NICK_X_SUSPENDED(en_us)
Nick \B%s\B is now suspended.
+NICK_X_SUSPENDED(pt)
O nick \B%s\B foi suspenso.
+NICK_X_SUSPENDED(nl)
Bijnaam \B%s\B is nu geschorst.
++++++++++++++++++++++++++++++++++++++++++++
+NS_SUSPEND_X_ALREADY_SUSPENDED(en_us)
Nick \B%s\B is already suspended !
+NS_SUSPEND_X_ALREADY_SUSPENDED(pt)
O nick \B%s\B já está suspenso !
+NS_SUSPEND_X_ALREADY_SUSPENDED(nl)
Bijnaam \B%s\B is al geschorst.
++++++++++++++++++++++++++++++++++++++++++++
+NS_UNSUSPEND_SUMMARY(en_us)
Remove a nick suspension.
+NS_UNSUSPEND_SUMMARY(pt)
Remove a suspensão de um nick.
+NS_UNSUSPEND_SUMMARY(nl)
Verwijder bijnaam schorsing.
++++++++++++++++++++++++++++++++++++++++++++
+NS_UNSUSPEND_SYNTAX(en_us)
Syntax:
  // UNSUSPEND nick
+NS_UNSUSPEND_SYNTAX(pt)
Sintaxe:
  // UNSUSPEND nick  
+NS_UNSUSPEND_SYNTAX(nl)
Syntax:
  // UNSUSPEND bijnaam
++++++++++++++++++++++++++++++++++++++++++++  
+NS_UNSUSPEND_HELP(en_us)
The // UNSUSPEND command removes a nick suspension.

@NS_UNSUSPEND_SYNTAX
+NS_UNSUSPEND_HELP(pt)
O comando // UNSUSPEND remove a suspensão a um nick.

@NS_UNSUSPEND_SYNTAX
+NS_UNSUSPEND_HELP(nl)
Het // UNSUSPEND commando verwijderd de schorsing van een bijnaam.

@NS_UNSUSPEND_SYNTAX
++++++++++++++++++++++++++++++++++++++++++++
+NS_SUSPEND_X_NOT_SUSPENDED(en_us)
Nick \B%s\B is not suspended !
+NS_SUSPEND_X_NOT_SUSPENDED(pt)
O nick \B%s\B não está suspendo !
+NS_SUSPEND_X_NOT_SUSPENDED(nl)
Bijnaam \B%s\B is niet geschorst !
++++++++++++++++++++++++++++++++++++++++++++
+NS_UNSUPEND_REMOVED_X(en_us)
Nick \B%s\B suspension was removed.
+NS_UNSUPEND_REMOVED_X(pt)
A suspensão do nick \B%s\B foi removida.
+NS_UNSUPEND_REMOVED_X(nl)
Schorsing van bijnaam \B%s\B verwijderd. 
++++++++++++++++++++++++++++++++++++++++++++
+NS_SUSPEND_LIST_ITEM_X_X_X_X_X(en_us)
%s by %s on %s , reason: %s [expires in \B%d\B day(s)]
+NS_SUSPEND_LIST_ITEM_X_X_X_X_X(pt)
%s por %s em %s, motivo: %s [expira em \B%d\B dia(s)]
+NS_SUSPEND_LIST_ITEM_X_X_X_X_X(nl)
%s door %s op %s , reden: %s [verloopt over \B%d\B dag(en)]
++++++++++++++++++++++++++++++++++++++++++++
+NS_SUSPEND_LIST_ITEM_X_X_X_X_FOREVER(en_us)
%s by %s on %s , reason: %s - \BForever\B
+NS_SUSPEND_LIST_ITEM_X_X_X_X_FOREVER(en_us)
%s por %s em %s , motivo: %s - \BPermanente\B
+NS_SUSPEND_LIST_ITEM_X_X_X_X_FOREVER(nl)
%s door %s op %s, reden: %s - \BPermanent\B
++++++++++++++++++++++++++++++++++++++++++++
+NS_SUSPEND_LIST_HEADER(en_us)
************ Suspensions list ****************
+NS_SUSPEND_LIST_HEADER(pt)
************ Lista de suspensï¿½s  ************
+NS_SUSPEND_LIST_HEADER(nl)
************** Schorsings lijst **************
++++++++++++++++++++++++++++++++++++++++++++
+NS_SUSPEND_LIST_TAIL(en_us)
**********************************************
++++++++++++++++++++++++++++++++++++++++++++
+NS_SUSPEND_LIST_TAIL(pt)
**********************************************
+NS_SUSPEND_LIST_TAIL(nl)
**********************************************
