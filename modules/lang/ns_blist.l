++++ ns_blist.l
++++ en_us: Lamego, Stinger
++++ nl: Stinger, Ldp
++++ pt_br: Ricardo

+BLIST_SUMMARY(en_us)
Manages the email black list.
+BLIST_SUMMARY(pt)
Gere a lista de demails proibidos.
+BLIST_SUMMARY(nl)
Beheer de E-mail blacklist.
+++++++++++++++++++++++++++++++++++++++++++++
+BLIST_HELP(en_us)
The blist command allows to manage the email black list:

Syntax:
  // BLIST ADD ( user@email | @email )
  // BLIST DEL ( user@email | @email )
  // BLIST mask

Examples:
  Add all emails from @myemail.com to the black list
  // ADD @myemail.com
  Detele @myemail.com from the black list
  // DEL @myemail.com
  List all nicks with \"my\" on the blacklist.
  // LIST %my%  
++++BLIST_HELP(nl)
++++Het blist commando stelt u in staat de E-mail blacklist te beheren:
++++
++++Syntax:
++++  // BLIST ADD (user@email | @email )
++++  // BLIST DEL (user@email | @email )
++++  // BLIST mask
  
++++Voorbeelden:
++++  Voeg alle E-mail adressen toe aan de blacklist van @mijnemail.com
++++  // ADD @mijnemail.com
++++  Verwijder @mijnemail.com van de blacklist
++++  // DEL @mijnemail.com
++++  Geef een lijst van alle bijnamen met \"mijn\" die aanwezig zijn op de blacklist
++++  // LIST %mijn%
