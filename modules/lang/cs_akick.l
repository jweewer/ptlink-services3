++++ cs_akick.l
++++ en_us: Lamego, Stinger
++++ nl: Stinger, Ldp
++++ pt: Lamego
++++ pt_br: Ricardo
++++ de: dp, jo

+AKICK_SUMMARY(en_us)
Manage akick list.
+AKICK_SUMMARY(pt)
Gestão da list de akicks
+AKICK_SUMMARY(nl)
Beheer de akick lijst.
+AKICK_SUMMARY(de)
Verwalten der akick Liste.
+++++++++++++++++++++++++++++++++++++++++++++
+AKICK_HELP(en_us)
The AKICK command allows to manage the channel akick list.
+AKICK_HELP(pt)
O comand AKICK permite gerir a lista de akicks do canal.
+AKICK_HELP(nl)
Het AKICK commando stelt u in staat de akick lijst van uw kanaal
te beheren.
+AKICK_HELP(de)
Der AKICK Befehl erlaubt die Verwaltung der akick Liste des Channels.
+++++++++++++++++++++++++++++++++++++++++++++
+CHAN_AKICK_SYNTAX(en_us)
Invalid Syntax!
Syntax:
  // AKICK #Chan ADD mask [+time[h|d|m]] [reason]
  // AKICK #Chan DEL mask | ALL
  // AKICK #Chan LIST [mask]
+CHAN_AKICK_SYNTAX(pt)
Sintaxe inválida!
Sintaxe:
  // AKICK #Canal ADD máscara [+tempo[h|d|m] [motivo]
  // AKICK #Chan DEL máscara | ALL
  // AKICK #Chan LIST [máscara
+CHAN_AKICK_SYNTAX(nl)
Ongeldige syntax!
Syntax:
  // AKICK #kanaal ADD masker [+tijd[h|d|m]] [reden]
  // AKICK #kanaal DEL masker | ALL
  // AKICK #kanaal LIST [masker]
+CHAN_AKICK_SYNTAX(de)
Ungültige Syntax!
Syntax:
  // AKICK #Channel ADD Maske [+Dauer[h|d|m]] [Grund]
  // AKICK #Channel DEL Maske | ALL
  // AKICK #Channel LIST [Maske]
+++++++++++++++++++++++++++++++++++++++++++++
+CHAN_AKICK_ADD_SYNTAX(en_us)
Invalid Syntax!
Syntax:
  // AKICK #Chan ADD mask [+time[h|d|m]] [reason]
+CHAN_AKICK_ADD_SYNTAX(pt)
Sintaxe inválida!
Sintaxe:
  // AKICK #Canal ADD máscara +tempo[h|d|m]] [motivo]
+CHAN_AKICK_ADD_SYNTAX(nl)
Ongeldige syntax!
Syntax:
  // AKICK #kanaal ADD masker [+tijd[h|d|m]] [reden]
+CHAN_AKICK_ADD_SYNTAX(de)
Ungültige Syntax!
Syntax:
  // AKICK #Channel ADD Maske [+Dauer[h|d|m]] [Grund]
++++++++++++++++++++++++++++++++++++++++++++
+ALREADY_AKICK_X_X(en_us)
There is already an akick for \B%s\B on \B%s\B.
+ALREADY_AKICK_X_X(nl)
Er bestaat al een akick voor \B%s\B op \B%s\B.
+ALREADY_AKICK_X_X(de)
Es gibt bereits einen akick für \B%s\B in \B%s\B.
++++++++++++++++++++++++++++++++++++++++++++
+ADDED_AKICK_X_X(en_us)
Added akick for \B%s\B on \B%s\B.
+ADDED_AKICK_X_X(nl)
Akick voor \B%s\B op \B%s\B is toegevoegd.
+ADDED_AKICK_X_X(de)
Akick für \B%s\B in \B%s\B hinzugefügt.
++++++++++++++++++++++++++++++++++++++++++++
+CHAN_AKICK_DEL_SYNTAX(en_us)
Invalid syntax!
Syntax:
  // AKICK #Chan DEL mask | ALL
+CHAN_AKICK_DEL_SYNTAX(pt)
Sintaxe inválida
Sintaxe:
  // AKICK #Chan DEL mask | ALL
+CHAN_AKICK_DEL_SYNTAX(nl)
Ongeldige syntax!
Syntax:
  // AKICK #kanaal DEL masker | ALL
+CHAN_AKICK_DEL_SYNTAX(de)
Ungültige Syntax!
Syntax:
  // AKICK #Channel DEL Maske | ALL
++++++++++++++++++++++++++++++++++++++++++++
+AKICK_X_X_NOT_FOUND(en_us)
There is no akick for \B%s\B on \B%s\B !
+AKICK_X_X_NOT_FOUND(pt)
Não há nenhum akick para \B%s\B no \B%s\B !
+AKICK_X_X_NOT_FOUND(nl)
Er is geen akick aanwezig voor \B%s\B op \B%s\B !
+AKICK_X_X_NOT_FOUND(de)
Es gibt keinen akick für \B%s\B in \B%s\B !
++++++++++++++++++++++++++++++++++++++++++++
+DELETED_AKICK_X_X(en_us)
Akick for \B%s\B was deleted on \B%s\B .
+DELETED_AKICK_X_X(pt)
A akick para \B%s\B no \B%s\B foi eliminada.
+DELETED_AKICK_X_X(nl)
Akick voor \B%s\B is verwijderd op \B%s\B .
+DELETED_AKICK_X_X(de)
Akick für \B%s\B wurde was aus \B%s\B gelöscht.
++++++++++++++++++++++++++++++++++++++++++++
+AKICK_LIST_HEADER_X(en_us)
*** List with \B%d\B akick(s)
+AKICK_LIST_HEADER_X(pt)
*** List with \B%d\B akick(s)
+AKICK_LIST_HEADER_X(nl)
*** Lijst bevat \B%d\B akick(s)
+AKICK_LIST_HEADER_X(de)
*** Liste mit \B%d\B akick(s)
++++++++++++++++++++++++++++++++++++++++++++
+AKICK_LIST_X_X_X(en_us)
 %s [%s] (expires on %s)
+AKICK_LIST_X_X_X(pt)
 %s [%s] (expira em %s)
+AKICK_LIST_X_X_X(nl)
 %s [%s] (verloopt op %s)
+AKICK_LIST_X_X_X(pt_br)
 %s [%s] (expires em %s)
+AKICK_LIST_X_X_X(de)
 %s [%s] (gültig bis %s)
++++++++++++++++++++++++++++++++++++++++++++ 
+AKICK_LIST_TAIL(en_us)
*** End of akick list
+AKICK_LIST_TAIL(pt)
*** Fim da lista de akicks
+AKICK_LIST_TAIL(nl)
** Einde van de akick lijst
+AKICK_LIST_TAIL(de)
*** Ende der akick Liste
++++++++++++++++++++++++++++++++++++++++++++
+NO_AKICK_PERM_ON_X(en_us)
You don't have permission to use the AKICK command on \B%s\B !
+NO_AKICK_PERM_ON_X(pt)
Não tem permissão para utilizar o comando AKICK no \B%s\B !
+NO_AKICK_PERM_ON_X(nl)
U heeft geen AKICK permissie op \B%s\B !
+NO_AKICK_PERM_ON_X(de)
Da hast nicht das Recht, den Akick Befehl für \B%s\B zu verwenden!
++++++++++++++++++++++++++++++++++++++++++++
+REACHED_MAX_AKICK_X(en_us)
You have reached the maximum of \B%d\B akicks !
+REACHED_MAX_AKICK_X(pt)
Atingiu o valor máximo de \B%d\B akicks !
+REACHED_MAX_AKICK_X(nl)
U heeft het maximale aantal van \B%d\B akicks bereikt !
+REACHED_MAX_AKICK_X(de)
Die Maximalzahl von \B%d\B akicks ist erreicht!
++++++++++++++++++++++++++++++++++++++++++++
+DELETED_AKICK_X_ALL(en_us)
All akicks on \B%s\B were deleted.
+DELETED_AKICK_X_ALL(pt)
Todas as akicks no \B%s\B foram removidas.
+DELETED_AKICK_X_ALL(nl)
Alle akicks op \B%s\B zijn verwijderd.
