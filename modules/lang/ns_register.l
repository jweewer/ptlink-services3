++++ ns_register.l
++++ en_us: Lamego, Stinger
++++ nl: Stinger, Ldp
++++ pt: Lamego
++++ pt_br: Ricardo
++++ de: jo

+REGISTER_SUMMARY(en_us)
Nick registration.
+REGISTER_SUMMARY(nl)
Bijnaam-registratie.
+REGISTER_SUMMARY(pt_br)
Registra um nick.
+REGISTER_SUMMARY(de)
Nickregistrierung.
++++++++++++++++++++++++++++++++++++++++++++
+REGISTER_HELP(en_us)
Nick registration.
+REGISTER_HELP(nl)
Bijnaam-registratie
+REGISTER_HELP(pt_br)
Registra um nick.
+REGISTER_HELP(de)
Nickregistrierung.
++++++++++++++++++++++++++++++++++++++++++++
+NICK_ALREADY_REGISTERED(en_us)
This nick is already registered.
+NICK_ALREADY_REGISTERED(pt)
Este nick já está registado.
+NICK_ALREADY_REGISTERED(nl)
Deze bijnaam is al geregistreerd.
+NICK_ALREADY_REGISTERED(pt_br)
Este nick já está registrado.
+NICK_ALREADY_REGISTERED(de)
Dieser Nick ist bereits registriert.
+++++++++++++++++++++++++++++++++++++++++++
+NICK_REGISTER_SYNTAX(en_us)
Invalid syntax 
Syntax: 
  // REGISTER password email
+NICK_REGISTER_SYNTAX(pt)
Sintaxe inválida
Sintaxe: 
  // REGISTER password email
+NICK_REGISTER_SYNTAX(nl)
Ongeldige syntax
Syntax:
  // REGISTER wachtwoord Email
+NICK_REGISTER_SYNTAX(pt_br)
Sintaxe inválida
Sintaxe: 
  // REGISTER senha email
+NICK_REGISTER_SYNTAX(de)
Ungültige Syntax 
Syntax: 
  // REGISTER Passwort Email
++++++++++++++++++++++++++++++++++++++++++++
+NICK_REGISTER_SUCCESS(en_us)
Nick successfully registered.
+NICK_REGISTER_SUCCESS(pt)
Nick registado com sucesso.
+NICK_REGISTER_SUCCESS(nl)
Bijnaam sucesvol geregistreerd.
+NICK_REGISTER_SUCCESS(pt_br)
Nick registrado com sucesso.
+NICK_REGISTER_SUCCESS(de)
Der Nick wurde erfolgreich registriert.
++++++++++++++++++++++++++++++++++++++++++++
+WELCOME_SENT(en_us)
Nick security code was sent to the specified email.
Please check your email to complete the nick registration.
+WELCOME_SENT(pt)
O código de segurança foi enviado para o seu email.
Verifique o seu email para completar o registo.
+WELCOME_SENT(nl)
Bijnaam-beveiligingscode verstuurd naar het opgegeven email adres.
+WELCOME_SENT(pt_br)
O código de segurança do nick foi enviado para o email especificado.
Verifique o seu email para completar o registro do nick.
+WELCOME_SENT(de)
Der Sicherheitscode für den Nick wurde an die angegebene Emailadresse geschickt.
Checke bitte deine Emails um die Registrierung des Nicks abzuschliessen.
++++++++++++++++++++++++++++++++++++++++++++
+WEAK_PASSWORD(en_us)
Your password must be at least 8 chars long and 
use some non-alfabetic char.
+WEAK_PASSWORD(pt)
A password tem que ter pelo menos 8 caracteres,
e um caracter numérico ou símbolo.
+WEAK_PASSWORD(nl)
Uw wachtwoord moet minstens 8 karakters lang zijn en
ook niet-alfabetische karakters bevatten.
+WEAK_PASSWORD(pt_br)
A senha deve ter pelo menos 8 caracteres,
e usar algum caracter numérico ou símbolo.
+WEAK_PASSWORD(de)
Das Passwort muss mindetens 8 Zeichen lang sein und muss auch
Zeichen ausserhalb des Alphabets enthalten.
++++++++++++++++++++++++++++++++++++++++++++
+NICK_REGISTER_FAIL(en_us)
There was a problem with your nick registration.
Please try again later.
+NICK_REGISTER_FAIL(pt)
Ocorreu um problema durante o registo.
Por favor tente mais tarde.
+NICK_REGISTER_FAIL(nl)
Er heeft zich een probleem voorgedaan tijdens het registreren
van uw bijnaam.
Probeer het later nog eens.
+NICK_REGISTER_FAIL(pt_br)
Ocorreu um problema durante o registro do seu nick.
Por favor, tente novamente mais tarde.
+NICK_REGISTER_FAIL(de)
Bei der Registrierung des Nicks trat ein Problem auf.
Versuche es bitte später nachmal.
++++++++++++++++++++++++++++++++++++++++++++
+WELCOME_ERROR(en_us)
There was an error trying to send an email with the 
security code for your nick.
Please use the SET EMAIL command later.
+WELCOME_ERROR(pt)
Ocorreu um erro a tentar enviar o email com o codigo
de segurança do seu nick.
Por favor utilize o comando SET EMAIL mais tarde.
+WELCOME_ERROR(nl)
Er heeft zich een probleem voor gedaan tijdens het versturen
van de beveiligingscode van uw bijnaam.
Probeer het later nog eens met het SET EMAIL commando.
+WELCOME_ERROR(pt_br)
Ocorreu um erro ao tentar enviar o email com o código
de segurança do seu nick.
Por favor, utilize o comando SET EMAIL mais tarde.
+WELCOME_ERROR(de)
Beim Senden der Email mit dem Sicherheitscode trat ein Fehler auf.
Bitter hole das später mit dem Befehl SET EMAIL nach.
++++++++++++++++++++++++++++++++++++++++++++
+ALREADY_X_WITH_EMAIL(en_us)
There are already \B%d\B nicks registered with this email address.
You can not register more nicks.
+ALREADY_X_WITH_EMAIL(nl)
Er zijn al \B%d\B bijnamen geregistreerd met dit Email adres.
U kunt geen bijnaam meer registreren.
+ALREADY_X_WITH_EMAIL(pt_br)
Já existe \B%d\B nick(s) registrado(s) com este email.
Você não pode registrar mais nicks.
+ALREADY_X_WITH_EMAIL(de)
Es gibt bereits \B%d\B Nicks die auf diese Emailadresse registriert sind.
Du kannst keine weiteren Nicks registrieren.
++++++++++++++++++++++++++++++++++++++++++++
+NICK_CANNOT_BE_REGISTERED(en_us)
That nick can not be registered.
Choose another by typing /NICK nick .
+NICK_CANNOT_BE_REGISTERED(pt)
Este nick não pode ser registrado.
Escolha outro através do comando /NICK nick .
+NICK_CANNOT_BE_REGISTERED(nl)
Deze bijnaam mag niet worden geregistreerd.
Kies een andere bijnaam met /NICK bijnaam .
+NICK_CANNOT_BE_REGISTERED(pt_br)
Este nick não pode ser registrado.
Escolha outro através do comando /NICK nick
+NICK_CANNOT_BE_REGISTERED(de)
Dieser Nick kann nicht registriert werden.
Bitte wähle einen anderen Nick. Dazu tippe: /NICK nick
++++++++++++++++++++++++++++++++++++++++++++
+LINK_EXCHANGE_X_X(en_us)
The following link is recommended by the user %s :
  %s
If you want to suggest a site, on IRC use:
  // SET FAVLINK your_site
+LINK_EXCHANGE_X_X(pt)
A seguinte página foi sugerida pelo utilizador %s :
  %s
Se quizeres sugerir um site, no IRC utiliza:
  // SET FAVLINK teu_site
+LINK_EXCHANGE_X_X(nl)
De volgende link is aanbevolen door gebruiker %s :
 %s
Als u een leuke site suggestie heeft, op IRC gebruik:
  // SET FAVLINK uw_site

