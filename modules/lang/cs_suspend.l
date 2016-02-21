++++ cs_suspend.l
++++ en_us: Lamego, Stinger
++++ nl: Stinger, Ldp
++++ pt: Lamego
++++ pt_br: Ricardo
++++ de: jo

++++++++++++++++++++++++++++++++++++++++++++
+CS_SUSPEND_SUMMARY(en_us)
Forbid a chan to be used for an amount of time.
+CS_SUSPEND_SUMMARY(nl)
Verbied een kanaal van gebruik voor een bepaalde tijd
++++++++++++++++++++++++++++++++++++++++++++
+CS_SUSPEND_SYNTAX(en_us)
Syntax:
  // SUSPEND [+time(h|d|M|Y)] chan reason
  // SUSPEND LIST
+CS_SUSPEND_SYNTAX(nl)
Syntax:
  // SUSPEND [+tijd(h|d|M|Y)] kanaal reden
  // SUSPEND LIST
++++++++++++++++++++++++++++++++++++++++++++
+CS_SUSPEND_HELP(en_us)
The // SUSPEND command forbids a chan from be used for an amount of time

@CS_SUSPEND_SYNTAX
Example:
  // SUSPEND +10d #smoker No smokers allowed.
Chan #smoker can't be used on the next 10d .
+CS_SUSPEND_HELP(nl)
Het // SUSPEND commando verbied een naam van gebruik voor een bepaalde tijd 

@CS_SUSPEND_SYNTAX
Voorbeeld:
  // SUSPEND +15d #blaat Stop met blaten
Kanaal blaat kan 15 dagen niet worden gebruikt.
++++++++++++++++++++++++++++++++++++++++++++
+CHAN_X_SUSPENDED(en_us)
Chan \B%s\B is now suspended.
+CHAN_X_SUSPENDED(pt)
O canal \B%s\B foi suspenso.
+CHAN_X_SUSPENDED(nl)
Kanaal \B%s\B is nu geschorst.
++++++++++++++++++++++++++++++++++++++++++++
+CS_SUSPEND_X_ALREADY_SUSPENDED(en_us)
The chan \B%s\B is already suspended !
+CS_SUSPEND_X_ALREADY_SUSPENDED(pt)
O canal \B%s\B já está suspenso !
+CS_SUSPEND_X_ALREADY_SUSPENDED(nl)
Kanaal \B%s\B is al geschorst.
++++++++++++++++++++++++++++++++++++++++++++
+CS_UNSUSPEND_SUMMARY(en_us)
Remove a chan suspension.
+CS_UNSUSPEND_SUMMARY(pt)
Remove a suspensão de um canal.
+CS_UNSUSPEND_SUMMARY(nl)
Verwijder kanaal schorsing.
++++++++++++++++++++++++++++++++++++++++++++
+CS_UNSUSPEND_SYNTAX(en_us)
Syntax:
  // UNSUSPEND chan
+CS_UNSUSPEND_SYNTAX(pt)
Sintaxe:
  // UNSUSPEND chan  
+CS_UNSUSPEND_SYNTAX(nl)
Syntax:
  // UNSUSPEND kanaal
++++++++++++++++++++++++++++++++++++++++++++  
+CS_UNSUSPEND_HELP(en_us)
The // UNSUSPEND command removes a chan suspension.

@CS_UNSUSPEND_SYNTAX
+CS_UNSUSPEND_HELP(pt)
O comando // UNSUSPEND remove a suspensão a um canal.

@CS_UNSUSPEND_SYNTAX
+CS_UNSUSPEND_HELP(nl)
Het // UNSUSPEND commando verwijderd de schorsing van een kanaal.

@CS_UNSUSPEND_SYNTAX
++++++++++++++++++++++++++++++++++++++++++++
+CS_SUSPEND_X_NOT_SUSPENDED(en_us)
Chan \B%s\B is not suspended !
+CS_SUSPEND_X_NOT_SUSPENDED(pt)
O canal \B%s\B não está suspendo !
+CS_SUSPEND_X_NOT_SUSPENDED(nl)
kanaal \B%s\B is niet geschorst !
++++++++++++++++++++++++++++++++++++++++++++
+CS_UNSUPEND_REMOVED_X(en_us)
Chan \B%s\B suspension was removed.
+CS_UNSUPEND_REMOVED_X(pt)
A suspensão do canal \B%s\B foi removida.
+CS_UNSUPEND_REMOVED_X(nl)
Schorsing van kanaal \B%s\B verwijderd. 
++++++++++++++++++++++++++++++++++++++++++++
+CS_SUSPEND_LIST_ITEM_X_X_X_X_X(en_us)
%s by %s on %s , reason: %s [expires in %d day(s)]
+CS_SUSPEND_LIST_ITEM_X_X_X_X_X(pt)
%s por %s em %s, motivo: %s [expira em %d dia(s)]
+CS_SUSPEND_LIST_ITEM_X_X_X_X_X(nl)
%s door %s op %s , reden: %s [verloopt over %d dag(en)]
++++++++++++++++++++++++++++++++++++++++++++
+CS_SUSPEND_LIST_ITEM_X_X_X_X_FOREVER(en_us)
%s by %s on %s , reason: %s - \BForever\B
+CS_SUSPEND_LIST_ITEM_X_X_X_X_FOREVER(pt)
%s por %s em %s, motivo: %s - \BPermanente\B
+CS_SUSPEND_LIST_ITEM_X_X_X_X_FOREVER(nl)
%s door %s op %s, reden: %s - \BPermanent\B
++++++++++++++++++++++++++++++++++++++++++++
+CS_SUSPEND_LIST_HEADER(en_us)
************ Suspensions list ****************
+CS_SUSPEND_LIST_HEADER(pt)
************ Lista de suspensões  ************
+CS_SUSPEND_LIST_HEADER(nl)
************** Schorsings lijst **************
++++++++++++++++++++++++++++++++++++++++++++
+CS_SUSPEND_LIST_TAIL(en_us)
**********************************************
+CS_SUSPEND_LIST_TAIL(pt)
**********************************************
+CS_SUSPEND_LIST_TAIL(nl)
**********************************************
