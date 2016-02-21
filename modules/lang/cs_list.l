++++ cs_list.l
++++ en_us: Lamego, Stinger
++++ nl: Stinger, Ldp
++++ pt_br: Ricardo
++++ de: jo

+LIST_SUMMARY(en_us)
List alls channels matching a given mask.
+LIST_SUMMARY(nl)
Geeft een lijst van kanalen die aan het masker voldoen.
+LIST_SUMMARY(de)
Listet alle Channel, die einer Maske entsprechen.
+++++++++++++++++++++++++++++++++++++++++++++
+LIST_HELP(en_us)
The list command allows to list all channels matching a given mask:

Syntax:
  // LIST *channel* [noexpire|forbidden]

Examples:
  List all channels containing the word \"love\":
  // LIST *love*
  
To prevent flooding the output is limited to 50 entries.
+LIST_HELP(nl)
het LIST commando geeft een lijst van alle kanalen die overeenkomen
met het opgegeven masker:

Syntax:
  // LIST *kanaal* [noexpire|forbidden]
  
Voorbeelden:
  Alle bijnamen die het woord \"blaat\" bevatten:
  // LIST *blaat*
Om flooden te voorkomen is de output gelimiteerd tot 50 regels.
+LIST_HELP(de)
Der list-Befehl erlaubt das Anzeigen einer Liste alle Channels, die einer gegeben Masken entsprechen.

Syntax:
  // LIST *Channel* [noexpire|forbidden]

Beispiele:
  Zeige ann Channels, die das Wort \"love\" enthalten:
  // LIST *love*
  
Um Fluten zu verhindern stoppt die Ausgabe der Liste nach 50 Einträgen.
+++++++++++++++++++++++++++++++++++++++++++++
+LIST_SYNTAX(en_us)
Syntax:
  // LIST *channel* [noexpire|forbidden]
+LIST_SYNTAX(nl)
Syntax:
  // LIST *kanaal* [noexpire|forbidden]
+LIST_SYNTAX(de)
Syntax:
  // LIST *Channel* [noexpire|forbidden]
+++++++++++++++++++++++++++++++++++++++++++++
+CHAN_LIST_HEADER_X(en_us)
*** List with \B%d\B channel(s)
+CHAN_LIST_HEADER_X(nl)
*** Lijst met \B%d\B kana(a)l(en)
+CHAN_LIST_HEADER_X(de)
*** Liste mit \B%d\B Channel(s)
+++++++++++++++++++++++++++++++++++++++++++++
+CHAN_LIST_X_X(en_us)
  %-35s %s
+CHAN_LIST_X_X(pt)
  %-35s %s  
+CHAN_LIST_X_X(nl)
  %-35s %s 
+CHAN_LIST_X_X(pt_br)
  %-35s %s 
+CHAN_LIST_X_X(de)
  %-35s %s 
+++++++++++++++++++++++++++++++++++++++++++++
+CHAN_LIST_TAIL(en_us)
** End of list
+CHAN_LIST_TAIL(nl)
*** Eind van de lijst  
+CHAN_LIST_TAIL(de)
** Ende der Liste
+++++++++++++++++++++++++++++++++++++++++++++
+LIST_STOPPED(en_us)
List stopped after 50 entries.
+LIST_STOPPED(pt)
Lista terminada após 50 resultados.
+LIST_STOPPED(nl)
Lijst gestopt na 50 regels.
+LIST_STOPPED(de)
Liste nach 50 Einträgen angehalten.