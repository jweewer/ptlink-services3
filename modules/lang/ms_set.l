++++ ms_set.l
++++ en_us: Lamego, Stinger
++++ nl: Stinger, Ldp
++++ pt: Lamego
++++ pt_br: Ricardo
++++ de: jo

+MS_SET_SUMMARY(en_us)
Changes your memoserv options.
+MS_SET_SUMMARY(pt)
Altera as suas opções do memoserv.
+MS_SET_SUMMARY(nl)
Verander uw memoserv instellingen
++++++++++++++++++++++++++++++++++++++++++++
+MS_SET_HELP(en_us)
This commands allows you set your memoserv options:
  Available info:
  
  Switch options, ON/OFF values only
  AUTOSAVE  - automatically save new receveid memos
  FORWARD   - forward new memos to email
  NOMEMOS   - disable memos reception

+MS_SET_HELP(pt)
O comando SET permite definir as opções de memoserv para um nick
  Opções dísponíveis

  Opções para activar/desactivar, o valor tem que ser ON/OFF
  AUTOSAVE - grava os novos memos automáticamente
  FORWARD  - encaminha novos memos para o email
  NOMEMOS  - desactiva a recepção de memos
+MS_SET_HELP(nl)
Dit commando stelt u instaat uw memoserv opties 
te veranderen:
  Beschikbare informatie:
  
  Switch opties, alleen ON/OFF waarden
++++++++++++++++++++++++++++++++++++++++++++
+MS_SET_SYNTAX(en_us)
Syntax:
  // SET option value
  For a list of available options use // HELP SET
+MS_SET_SYNTAX(pt)
Sintaxe:
  // SET opção valor
  Para listar as opções disponíveis digite: 
  // HELP SET
+MS_SET_SYNTAX(nl)
Syntax:
  // SET optie waarde
  Voor een lijst van beschikbare opties gebruik // HELP SET
+MS_SET_SYNTAX(pt_br)
Sintaxe:
  // SET opção valor
  Para listar as opções disponíveis, digite: 
  // HELP SET
+MS_SET_SYNTAX(de)
Syntax:
  // SET Option Wert
  Eine Liste der möglichen Optionen erhälst du mit // HELP SET
++++++++++++++++++++++++++++++++++++++++++++
++++++++++++++++++++++++++++++++++++++++++++
+UNKNOWN_OPTION_X(en_us)
\B%s\B is not a valid option, please check the available
options with // HELP SET.
+UNKNOWN_OPTION_X(pt)
\B%s\B não é uma opção válida!
Veja as opções disponíveis digitando:
  // HELP SET.
+UNKNOWN_OPTION_X(nl)
\B%s\B is geen geldige optie. Voor een lijst van beschikbare
opties, bekijk // HELP SET
+UNKNOWN_OPTION_X(pt_br)
\B%s\B não é uma opção válida. Por favor, veja as opções
disponíveis, digitando: // HELP SET
+UNKNOWN_OPTION_X(de)
\B%s\B ist keine gültige Option. Eine Liste der möglichen Optionen
erhälst du unter // HELP SET
++++++++++++++++++++++++++++++++++++++++++++
+OPTION_X_ON(en_us)
The %s option is now \BON\B.
+OPTION_X_ON(pt)
A opção %s foi \Bactivada\B.
+OPTION_X_ON(nl)
De optie %s is nu \BON\B (aan).
+OPTION_X_ON(pt_br)
A opção %s foi \Bativada\B.
+OPTION_X_ON(de)
Die Option %s ist jetzt \BON\B (Ein).
++++++++++++++++++++++++++++++++++++++++++++
+OPTION_X_OFF(en_us)
The %s option is now \BOFF\B.
+OPTION_X_OFF(pt)
A opção %s foi \Bdesactivada\B.
+OPTION_X_OFF(nl)
De optie %s is nu \BOFF\B (uit).
+OPTION_X_OFF(pt_br)
A opção %s foi \Bdesativada\B.
+OPTION_X_OFF(de)
Die Option %s ist jetzt \BOFF\B (Aus).
++++++++++++++++++++++++++++++++++++++++++++
+VALUE_ON_OR_OFF(en_us)
The value for this option must be \BON\B or \BOFF\B.
+VALUE_ON_OR_OFF(pt)
O valor para esta opção tem que ser \BON\B ou \BOFF\B.
+VALUE_ON_OR_OFF(nl)
De waarde voor deze optie moet \BON\B (aan) of \BOFF\B (uit) zijn.
+VALUE_ON_OR_OFF(pt_br)
O valor para esta opção deve ser \BON\B ou \BOFF\B.
+VALUE_ON_OR_OFF(de)
Der Wert für diese Option kann nur \BON\B (Ein) oder \BOFF\B (Aus) sein.
++++++++++++++++++++++++++++++++++++++++++++
+MS_SSET_SUMMARY(en_us)
Super SET (For SAdmins only)
+MS_SSET_SUMMARY(pt)
Super SET (Apenas para SAdmins)
+MS_SSET_SUMMARY(nl)
Super SET (Alleen SAdmins)
+MS_SSET_SUMMARY(pt_br)
Super SET (Apenas para SAdmins)
+MS_SSET_SUMMARY(de)
Super SET (Nur für SAdmins)
++++++++++++++++++++++++++++++++++++++++++++
+MS_SSET_HELP(en_us)
Allows to change settings for a specified nick.
Syntax:
  // SSET nick option value
Available options:
  MAXMEMOS  - Max numer of memos the user can receive

  All options from // HELP SET
+MS_SSET_HELP(pt)
Permite alterar as configurações do nick especificado.
Sintaxe:
  // SSET nick opção valor 
Opções disponíveis:
  MAXMEMOS  - Max numer of memos the user can receive
 
  Todas as opções do // HELP SET  
+MS_SSET_HELP(nl)
Stelt u in staat instellingen te veranderen van een 
gespecificeerde bijnaam.
Syntax:
  // SSET bijnaam optie waarde
  
Beschikbare opties:
  MAXMEMOS  - Max. aantal memo's een bijnaam kan hebben
  
  Alle opties van // HELP SET
++++++++++++++++++++++++++++++++++++++++++++
+NICK_X_NOT_FOUND(en_us)
Nick \B%s\B is not a registered nick.
+NICK_X_NOT_FOUND(pt)
O nick \B%s\B não está registrado.
+NICK_X_NOT_FOUND(nl)
De bijnaam \B%s\B is niet geregistreerd
+NICK_X_NOT_FOUND(pt_br)
O nick \B%s\B não está registrado.
+NICK_X_NOT_FOUND(de)
Der Nick \B%s\B ist nicht registriert.
++++++++++++++++++++++++++++++++++++++++++++
+MS_SSET_SYNTAX(en_us)
Syntax:
  // SSET nick option value
  For a list of available options use /NickServ HELP SSET  
+MS_SSET_SYNTAX(pt)
Sintaxe:
  // SSET nick opção valor
  Para uma lista das opções disponíveis utilize /NickServ HELP SSET  
+MS_SSET_SYNTAX(nl)
Syntax:
  // SET nick optie waarde
  Voor een lijst van beschikbare opties bekijk /NickServ HELP SSET
+MS_SSET_SYNTAX(pt_br)
Sintaxe:
  // SSET nick opção valor
  Para uma lista das opções disponíveis use /NickServ HELP SSET
+MS_SSET_SYNTAX(de)
Syntax:
  // SSET Nick Option Wert
  Die Liste der möglichen Optionen erhälst du unter /NickServ HELP SSET  
++++++++++++++++++++++++++++++++++++++++++++
+MS_SET_MAXMEMOS_TO_X(en_us)
The max number of memos was changed to \B%d\B.
+MS_SET_MAXMEMOS_TO_X(pt)
O número máximo de memos foi alterado para \B%d\B.
+MS_SET_MAXMEMOS_TO_X(nl)
Het maximale aantal memo's is veranderd naar \B%d\B.
