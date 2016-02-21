++++ cs_drop.l 
++++ en_us: Lamego, Stinger
++++ nl: Stinger, Ldp
++++ pt_br: openglx, Ricardo
++++ de: dp

+DROP_SUMMARY(en_us)
Drop command.
+DROP_SUMMARY(pt)
Elimina um canal.
+DROP_SUMMARY(nl)
Verzorgt het drop commando.
+DROP_SUMMARY(pt_br)
Desregistra um canal.
+DROP_SUMMARY(de)
Löscht einen Channel.
+++++++++++++++++++++++++++++++++++++++++++++
+DROP_HELP(en_us)
Provides drop command.
+DROP_HELP(pt)
Ajuda para o comando DROP
+DROP_HELP(nl)
Verzorgt het drop commando.
+DROP_HELP(pt_br)
O comando DROP cancela o registro de um canal.
+DROP_HELP(de)
Löscht einen Channel.
+++++++++++++++++++++++++++++++++++++++++++++
+CHAN_DROP_SYNTAX(en_us)
Syntax:
  // DROP #channel
+CHAN_DROP_SYNTAX(pt)
Sintaxe:
  // DROP #canal
+CHAN_DROP_SYNTAX(nl)
  // DROP #kanaal
+CHAN_DROP_SYNTAX(pt_br)
Sintaxe:
  // DROP #canal
+CHAN_DROP_SYNTAX(de)
Syntax:
  // DROP #channel
+++++++++++++++++++++++++++++++++++++++++++++
+CHAN_X_DROPPED(en_us)
Channel \B%s\B has been dropped.
+CHAN_X_DROPPED(pt)
O registo do canal \B%s\B foi cancelado.
+CHAN_X_DROPPED(nl)
Registratie van kanaal \B%s\B is ongedaan gemaakt.
+CHAN_X_DROPPED(pt_br)
O registro do canal \B%s\B foi cancelado.
+CHAN_X_DROPPED(de)
Der Channel \B%s\B wurde gelöscht.
++++++++++++++++++++++++++++++++++++++++++++
+DROP_SECURITY_REQUIRED(en_us)
Your current email is authenticated.
You need to provide your nick security code to drop the channel.
+DROP_SECURITY_REQUIRED(pt)
O seu nick está autenticado.
Tem que fornecer o código de segurança para executar o drop.
+DROP_SECURITY_REQUIRED(nl)
Het huidige email adres is geauthenticeerd.
U heeft de beveiligingscode van uw bijnaam nodig die u heeft gekregen
om de kanaal registratie ongedaan te maken.
+DROP_SECURITY_REQUIRED(pt_br)
O seu email atual está autenticado.
Você precisa fornecer o código de segurança do seu nick para desregistrar o canal.
+DROP_SECURITY_REQUIRED(de)
Deine E-Mail ist authentifziert. Du musst den Sicherheitscode für deinen
Nick angeben, um den Channel löschen zu können.
++++++++++++++++++++++++++++++++++++++++++++
+SDROP_SUMMARY(en_us)
Cancels a registration of a given channelname (For SAdmin only)
+SDROP_SUMMARY(nl)
Maakt een kanaal registratie ongedaan van een opgegeven kanaal (Alleen SAdmin)
+SDROP_SUMMARY(de)
Löscht einen Channel ohne Eingabe des Sicherheitscodes (Nur für SAdmin)
++++++++++++++++++++++++++++++++++++++++++++
+SDROP_HELP(en_us)
Cancel a registered channel without using the securitycode
Syntax:
  // SDROP #channel
+SDROP_HELP(nl)
Maakt een kanaal registratie ongedaan van een opgegeven kanaal
Syntax:
  // SDROP #kanaal
+SDROP_HELP(de)
Löscht einen Channel ohne Eingabe des Sicherheitscodes (Nur für SAdmin)
Syntax:
  // SDROP #channel
++++++++++++++++++++++++++++++++++++++++++++
+CHAN_SDROP_SYNTAX(en_us)
Syntax:
  // SDROP #channel
+CHAN_SDROP_SYNTAX(nl)
Syntax:
  // SDROP #kanaal
+CHAN_SDROP_SYNTAX(de)
Syntax:
  // SDROP #channel