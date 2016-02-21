+++ bs_bot.l
++++ en_us: lamego
++++ nl : ^Stinger^

+++++++++++++++++++++++++++++++++++++++++++++
+BS_CREATE_SUMMARY(en_us)
Create services based bots.
+BS_CREATE_SUMMARY(pt)
Cria \"bots\" .
+BS_CREATE_SUMMARY(nl)
Maak een op de services gebaseerde bots.
+++++++++++++++++++++++++++++++++++++++++++++
+BS_CREATE_SYNTAX(en_us)
// CREATE owner_nick time nick username hostname realname

The time parameter defines the bot lifetime.
The time value 0d means the bot will never expire.
+BS_CREATE_SYNTAX(pt)
// CREATE nick_dono tempo nick username hostname realname
+BS_CREATE_SYNTAX(nl)
  // CREATE eigenaar_bijnaam tijd bijnaam gebruikersnaam hostnaam echtenaam
De tijd parameter geeft de levenstijd van de bot aan.
De tijd paramaeter 0d, houd in dat de bot nooit verloopt.
+++++++++++++++++++++++++++++++++++++++++++++
+BS_CREATE_HELP(en_us)
@BS_CREATE_SYNTAX
+BS_CREATE_HELP(nl)
@BS_CREATE_SYNTAX
+++++++++++++++++++++++++++++++++++++++++++++
+BS_CREATE_SYNTAX_INV(en_us)
Invalid syntax !

@BS_CREATE_SYNTAX
+BS_CREATE_SYNTAX_INV(nl)
Ongeldige syntax !

@BS_CREATE_SYNTAX
+++++++++++++++++++++++++++++++++++++++++++++
+BS_CREATE_X_EXISTS(en_us)
The bot \B%s\B already exists !
+BS_CREATE_X_EXISTS(nl)
De bot \B%s\B bestaat al !
+++++++++++++++++++++++++++++++++++++++++++++
+BS_CREATE_NICK_X_IS_REG_X(en_us)
\B%s\B is a registered nick !
+BS_CREATE_NICK_X_IS_REG_X(nl)
\B%s\B is een geregistreerde bijnaam !
+++++++++++++++++++++++++++++++++++++++++++++
+BS_CREATE_CREATED_X(en_us)
Bot \B%s\B was successfully created.
+BS_CREATE_CREATED_X(pt)
O bot \B%s\B foi criado com successo.
+BS_CREATE_CREATED_X(nl)
Bot \B%s\B is succesvol aangemaakt.
+++++++++++++++++++++++++++++++++++++++++++++
+BS_CREATE_INVALID_NICK_X(en_us)
\B%s\B is not a valid nick !
+BS_CREATE_INVALID_NICK_X(nl)
\B%s\B is een ongeldige bijnaam !
+++++++++++++++++++++++++++++++++++++++++++++
+BS_CREATE_INVALID_HOST_X(en_us)
\B%s\B is not a valid hostname !
+BS_CREATE_INVALID_HOST_X(nl)
\B%s\B is een ongeldige hostnaam !
+++++++++++++++++++++++++++++++++++++++++++++
+BS_CREATE_INVALID_USER_X(en_us)
\B%s\B is not a valid username !
+BS_CREATE_INVALID_USER_X(nl)
\B%s\B is een ongeldige usernaam !
