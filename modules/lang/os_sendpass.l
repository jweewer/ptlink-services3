++ **********************************************************************
++ * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
++ *                     http://software.pt-link.net                    *
++ * This program is distributed under GNU Public License               *
++ * Please read the file COPYING for copyright information.            *
++ **********************************************************************
++
++  Desc: os_sline language file
++
++ *  $Id: os_sendpass.l,v 1.4 2005/09/30 17:56:02 waxweazle Exp $
++

+SENDPASS_SUMMARY(en_us)
Reset and send a nick password.
+SENDPASS_SUMMARY(pt)
Envia uma nova password para um nick.
+SENDPASS_SUMMARY(nl)
Reset een wachtwoord en stuurt deze naar de bijnaam.
+SENDPASS_SUMMARY(pt_br)
Cria e envia uma nova senha para um nick.
+SENDPASS_SUMMARY(de)
Setzt das Pssswort zurück und sendet es an den Nick.
++++++++++++++++++++++++++++++++++++++++++++
+SENDPASS_HELP(en_us)
The SENDPASS command will reset a nick password and send it
to the email set for that nick.

Syntax:
  \B// SENDPASS nick\B
+SENDPASS_HELP(pt)
O comando SENPASS faz reset à password e envia-a 
para o email definido no registo do nick.

Syntaxe:
  \B// SENDPASS nick\B  
+SENDPASS_HELP(nl)
Het SENDPASS commando zal het wachtwoord voor een bijnaam resetten en 
versturen naar het opgegeven E-mail adres.

syntax:
  \B// SENDPASS bijnaam\B  
+SENDPASS_HELP(pt_br)
O comando SENDPASS cria uma nova senha para um nick e a envia
para o email ajustado no nick.

Sintaxe:
  \B// SENDPASS nick\B
+SENDPASS_HELP(de)
Der SENDPASS-Befehl setzt das Passwort für einen Nick zurück und sendet es an die Emailadresse des Nicks.

Syntax:
  \B// SENDPASS Nick\B
++++++++++++++++++++++++++++++++++++++++++++
+SENDPASS_SYNTAX(en_us)
Invalid Syntax!
Syntax:
  \B// SENDPASS nick\B
+SENDPASS_SYNTAX(pt)
Sintaxe Inválida.
Sintaxe:
  \B// SENDPASS nick\B  
+SENDPASS_SYNTAX(nl)
Ongeldige syntax!
syntax:
  \B// SENDPASS bijnaam\B
+SENDPASS_SYNTAX(pt_br)
Sintaxe Inválida!
Sintaxe:
  \B// SENDPASS nick\B  
+SENDPASS_SYNTAX(de)
Ungültige Syntax!
Syntax:
  \B// SENDPASS Nick\B
++++++++++++++++++++++++++++++++++++++++++++
+SENDPASS_X_SENT_X(en_us)
Password for nick \B%s\B has been reset and sent to \B%s\B .
+SENDPASS_X_SENT_X(pt)
A password para o nick \B%s\B foi alterada e enviada para \B%s\B .
+SENDPASS_X_SENT_X(nl)
Wachtwoord voor bijnaam \B%s\B is gereset en verzonden naar \B%s\B .
+SENDPASS_X_SENT_X(pt_br)
A senha para o nick \B%s\B foi recriada e enviada para \B%s\B.
+SENDPASS_X_SENT_X(de)
Das Passwort für den Nick \B%s\B wurde neu gesetzt und an \B%s\B geschickt.
++++++++++++++++++++++++++++++++++++++++++++
+MAIL_LATER(en_us)
There was an error trying to send the email, try again later.
+MAIL_LATER(pt)
Ocorreu um erro ao tentar enviar o email, tente de novo mais tarde.
+MAIL_LATER(nl)
Er heeft zich een fout voorgedaan bij het versturen van de E-mail,
probeer het later nog eens.
+MAIL_LATER(pt_br)
Ocorreu um erro ao tentar enviar o email, tente novamente mais tarde.
+MAIL_LATER(de)
Beim Versuch das Passort zu senden trat ein Fehler auf. Versuche es später nochmal.
++++++++++++++++++++++++++++++++++++++++++++
+SENDPASS_X_X(en_us)
The password for your nick %s was changed to %s .
+SENDPASS_X_X(pt)
A password do seu nick %s foi alterada para %s .
+SENDPASS_X_X(nl)
Het wachtwoord voor uw bijnaam %s is veranderd naar %s .
+SENDPASS_X_X(pt_br)
A senha do seu nick %s foi alterada para %s
+SENDPASS_X_X(de)
Das Passwort für deinen Nick %s wurde geändert in %s
++++++++++++++++++++++++++++++++++++++++++++
+OS_SENDPASS_NO_EMAIL_X(en_us)
Nick %s does not have an email defined !
+OS_SENDPASS_NO_EMAIL_X(pt)
O nick %s não tem um email definido !
+OS_SENDPASS_NO_EMAIL_X(nl)
Nick %s heeft geen E-mail adres opgegeven !
