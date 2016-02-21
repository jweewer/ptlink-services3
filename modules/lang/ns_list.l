++++ ns_list.l
++++ en_us: Lamego, Stinger
++++ nl: Stinger, Ldp
++++ pt_br: Ricardo
++++ de: jo

+LIST_SUMMARY(en_us)
List alls nicks matching a given mask.
+LIST_SUMMARY(pt)
Lista todos os nicks de acordo com uma dada máscara.
+LIST_SUMMARY(nl)
Geeft een lijst van bijnamen die aan het masker voldoen.
+LIST_SUMMARY(de)
Listet alle Nicks, die einer Suchmaske entsprechen.
+++++++++++++++++++++++++++++++++++++++++++++
+LIST_HELP(en_us)
The list command allows to list all nicks matching a given mask:

Syntax:
  // LIST ( nick | @email | !user@host | #*IP* ) [auth|noexpire|forbidden|nonews]

Examples:
  List all nicks contaning the word \"love\":
  // LIST *love*
  List all nicks with emails from gmail:
  // LIST @*gmail.com
  List all nicks with \"bot\" on the mask userhost mask:
  // LIST !*bot*
  
To prevent flooding the output is limited to 50 entries.
+LIST_HELP(pt)
O comand LIST lista todos os nicks de acordo com uma dada máscara.

Sintaxe:
  // LIST nick | @email  | !user@host | #*IP* [auth|noexpire|forbidden|nonews]

Exemplos:
  Listar todos os nicks que contenham a palavra \"gato\":
  // LIST *gato*
  Listar todos os nicks com email do gmail:
  // LIST @*gmail.com
  Listar todos os nicks com \"bot\" na ultima máscara:
  // LIST !*bot*
Para prevenir o excesso de informação a lista é limitada e 50 entrada
+LIST_HELP(nl)
het LIST commando geeft een lijst van alle bijnamen die overeenkomen
met het opgegeven masker:

Syntax:
  // LIST nick | @email | !user@host | #*IP* [auth|noexpire|forbidden|nonews]
  
Voorbeelden:
  Alle bijnamen die het woord \"blaat\" bevatten:
  // LIST *blaat*
  Alle bijnamen met een email van gmail:
  // LIST @*gmail.com
  Alle bijnamen met \"bot\" als userhost
  // LIST !*bot*
Om flooden te voorkomen is de output gelimiteerd tot 50 regels.
+LIST_HELP(de)
Der LIST-Befehl erlaubt die Auflistung aller Nicks, die einer Maske entsprechen:

Syntax:
  // LIST ( Nick | @email | !user@host | #*IP* ) [auth|noexpire|forbidden|nonews]

Beispiele:
  Listet alle Nicks, die das Wort \"love\" enthalten:
  // LIST *love*
  Listet alle Nicks die eine Emailadresse bei gmail haben:
  // LIST @*gmail.com
  Listet alle Nicks mit  \"bot\" in der Userhost Maske:
  // LIST !*bot*
  
Um Flooding (zu grosse Liste/Datenmenge) zu verhindern, ist die Liste auf max. 50 Einträge beschränkt.
+++++++++++++++++++++++++++++++++++++++++++++
+LIST_SYNTAX(en_us)
Syntax:
  // LIST nick | @email | !user@host | #*IP*
+LIST_SYNTAX(pt)
Sintaxe:
  // LIST nick | @email | !user@host | #*IP*
+LIST_SYNTAX(nl)
Syntax:
  // LIST nick | @email | !user@host | #*IP*
+LIST_SYNTAX(de)
Syntax:
  // LIST Nick | @email | !user@host | #*IP*
+++++++++++++++++++++++++++++++++++++++++++++
+NICK_LIST_HEADER_X(en_us)
*** List with \B%d\B nick(s)
+NICK_LIST_HEADER_X(pt)
*** Lista com \B%d\B nick(s)
+NICK_LIST_HEADER_X(nl)
*** Lijst met \B%d\B bijna(a)nm(en)
+NICK_LIST_HEADER_X(de)
*** Liste mit \B%d\B Nick(s)
+++++++++++++++++++++++++++++++++++++++++++++
+NICK_LIST_X_X(en_us)
  %s [%s]
+NICK_LIST_X_X(pt)
  %s [%s]
+NICK_LIST_X_X(nl)
  %s [%s]
+NICK_LIST_X_X(pt_br)
  %s [%s]
+NICK_LIST_X_X(de)
  %s [%s]
+++++++++++++++++++++++++++++++++++++++++++++
+NICK_LIST_TAIL(en_us)
** End of list
+NICK_LIST_TAIL(pt)
** Fim da lista
+NICK_LIST_TAIL(nl)
*** Eind van de lijst  
+NICK_LIST_TAIL(de)
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
