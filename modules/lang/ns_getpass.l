++++ ns_getpass.l
++++ en_us: Lamego, Stinger
++++ nl: Stinger
++++ pt: Lamego
++++ pt_br: Ricardo
++++ de: jo

+GETPASS_SUMMARY(en_us)
Recover a lost/stolen nick password.
+GETPASS_SUMMARY(pt)
Recupera a password perdida/roubada de um nick.
+GETPASS_SUMMARY(nl)
Herstellen van een verloren/gestolen bijnaam wachtwoord
+GETPASS_SUMMARY(de)
Wiederherstellen eines verlorenen/gestohlenen Nickpassworts.
+++++++++++++++++++++++++++++++++++++++++++++
+GETSEC_SUMMARY(en_us)
Recover the nick password.
+GETSEC_SUMMARY(pt)
Recupera o código de segurança do nick.
+GETSEC_SUMMARY(nl)
Herstellen van bijnaam wachtwoord.
+GETSEC_SUMMARY(de)
Wiederherstellen eines Nickpassworts.
+++++++++++++++++++++++++++++++++++++++++++++
+GETPASS_HELP(en_us)
The command GETPASS allows a user to recover is nick password.
You need to know the nick security code!
A recuperation code will be sent to the nick's email.

Syntax:
  // GETPASS nick securitycode
+GETPASS_HELP(pt)
O comando GETPASS permite a um utilizador recuperar a password do seu nick.
Tem que indicar o código de segurança do nick!
Sintaxe:
  // GETPASS nick código_de_segurança
+GETPASS_HELP(nl)
Het GETPASS commando stelt de gebruiker in staat het wachtwoord van de bijnaam
terug te krijgen. De beveiligingscode is hier voor nodig!
Een herstelcode wordt naar het E-mail adres van de bijnaam verzonden.

Syntax:
  // GETPASS bijnaam beveiligingscode
+GETPASS_HELP(de)
Der GETPASS Befehl erlaubt es einem Benutzer sein verlorenes/gestohlenes
Passwort wiederherzustellen.
Dazu wird der Sicherheitscode der Authentifizierung benötigt.
Ein Code zur Wiedererlangung des Passowrts wird an diese Email geschickt.

Syntax:
  // GETPASS Nick Sicherheitscode
+++++++++++++++++++++++++++++++++++++++++++++
+GETSEC_HELP(en_us)
The command GETSEC allows a user to recover is nick security code.
Syntax:
  // GETSEC
+GETSEC_HELP(pt)
O comando GETSEC permite ao utilizador recuperar o código de segurança
do seu nick.
Sintaxe:
  // GETSEC
+GETSEC_HELP(nl)
Het GETSEC commando stel de gebruiker in staat de beveiligingscode van de bijnaam
terug te krijgen.
+GETSEC_HELP(de)
Der GETSEC Befehl erlaubt einem Benutzer die Wiederhestellung des Sicherheitscodes.
Syntax:
  // GETSEC
+++++++++++++++++++++++++++++++++++++++++++++
+INVALID_GETPASS_SYNTAX(en_us)
Invalid syntax!
Syntax:
  // GETPASS nick securitycode
+INVALID_GETPASS_SYNTAX(pt)
Sintaxe inválida!
Sintaxe:
  // GETPASS nick código_de_segurança
+INVALID_GETPASS_SYNTAX(nl)
Ongeldige syntax!
Syntax:
  // GETPASS bijnaam beveiligingscode
+INVALID_GETPASS_SYNTAX(de)
Ungültige Syntax!
Syntax:
  // GETPASS Nick Sicherheitscode
+++++++++++++++++++++++++++++++++++++++++++++
+GETPASS_SUBJECT(en_us)
Password recovery
+GETPASS_SUBJECT(pt)
Recuperação da password
+GETPASS_SUBJECT(nl)
Wachtwoord herstel
+GETPASS_SUBJECT(de)
Passwortwiederherstellung
+++++++++++++++++++++++++++++++++++++++++++++
+GETPASS_BODY_X_X(en_us)
To recover your nick's password type:
  /NickServ RECOVER %s %s
And then change the nick password with:
  /NickServ SET PASSWORD new_password
+GETPASS_BODY_X_X(pt)
Para recuperar a password do seu nick digite:
  /NickServ RECOVER %s %s
E depois altere a password com:
  /NickServ SET PASSWORD nova_password
+GETPASS_BODY_X_X(nl)
Om het wachtwoord van uw bijnaam te herstellen typ:
  /NickServ RECOVER %s %s
En verander het wachtwoord van uw bijnaam met:
  /NickServ SET PASSWORD nieuw_wachtwoord
+GETPASS_BODY_X_X(de)
Zur Wiederherstellung des Passworts für deinen Nick tippe:
  /NickServ RECOVER %s %s
Anschliessend setze für den Nick ein neues Passwort. Dazu verwende:
  /NickServ SET PASSWORD neuesPasswort
+++++++++++++++++++++++++++++++++++++++++++++
+GETPASS_CHECK_EMAIL_X(en_us)
An email was sent to \B%s\B with instructions to recover the password.
+GETPASS_CHECK_EMAIL_X(pt)
Um email foi enviado para \B%s\B com instruções pare recuperar a password.
+GETPASS_CHECK_EMAIL_X(nl)
Er is een E-mail verzonden naar \B%s\B met de instructies hoe u het wachtwoord kunt
herstellen
+GETPASS_CHECK_EMAIL_X(de)
An \B%s\B wurde ein Email geschickt, die Anweisungen zur Wiedererlangung des Passworts enthält.
+++++++++++++++++++++++++++++++++++++++++++++
+GETSEC_BODY_X_X(en_us)
Someone using your nick has request to get a new security code.
If this was request by you just type:
  /NickServ SET EMAIL %s %s
to get a new security code.
If someone else did the request then probably your nick password is compromised
and you should change it with /NickServ SET PASSWORD new_password.
Do not forget to reauthenticate the email afterwards.
+GETSEC_BODY_X_X(pt)
Alguem utlizando o seu nick pediu a recuperação do código de segurança.
Se foi você pode utilizar o comando:
  /NickServ SET EMAIL %s %s
para receber um novo código de segurança.
Em seguida não se esqueça de reautenticar o email.
+GETSEC_BODY_X_X(nl)
Iemand ie uw bijnaam heeft gebruikt heeft een nieuwe beveiligingscode aangevraagd
Als dit uw verzoek was typ dan:
  /NickServ SET EMAIL %s %s
om een nieuwe beveilgingscode te krijgen.
Als iemand anders toch dit heeft aangevraagt is het wachtwoord van uw bijnaam bekend bij
iemand anders, het advies is om het wachtwoord te veranderen met 
  /NickServ SET PASSWORD nieuw_wachtwoord
Vergeet niet het E-mail adres opnieuw te authenticeren.
+GETSEC_BODY_X_X(de)
Jemand der deinen Nick benutzt hat, hat einen neuen Sciherheitscode beantragt.
Wenn die Anfrage von dir kam, tippe:
  /NickServ SET EMAIL %s %s
um einen neuen Sicherheitscode zu erhalten.
Wenn jemand anderes die Anfrage gestellt hat, ist dein Passwort wahrscheinlich bekannt geworden und du
solltest es ändern. Dazu verwende:
  /NickServ SET PASSWORD neuesPasswort
Vergiss nicht, hinterher deine Email erneut zu authentifizieren.
+++++++++++++++++++++++++++++++++++++++++++++
+GETSEC_SUBJECT(en_us)
Security code recovery
+GETSEC_SUBJECT(pt)
Recuperação do código de segurança
+GETSEC_SUBJECT(nl)
Beveiligingscode herstel
+GETSEC_SUBJECT(de)
Wiedererlangung des Sicherheitscodes
+++++++++++++++++++++++++++++++++++++++++++++
+GETSEC_CHECK_EMAIL_X(en_us)
An email was sent to \B%s\B with instructions to recover the security code.
+GETSEC_CHECK_EMAIL_X(pt)
Um email foi enviado para \B%s\B com instruções pare recuperar o
código de segurança.
+GETSEC_CHECK_EMAIL_X(nl)
Er is een E-Mail verzonden naar \B%s\B met de instructies hoe u de beveiligingscode
kunt herstellen.
+GETSEC_CHECK_EMAIL_X(de)
An \B%s\B wurde ein Email geschickt, die Anweisungen zur Wiedererlangung des Sicherheitscodes enthält.
+++++++++++++++++++++++++++++++++++++++++++++