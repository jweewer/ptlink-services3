++++ op_deop.l
++++ en_us: Lamego, Stinger
++++ nl: Stinger, Ldp
++++ pt: Lamego
++++ de: dp

+OP_SUMMARY(en_us)
Command to give channel oper status to an user
+OP_SUMMARY(pt)
Comando para dar op a um utilizador num canal.
+OP_SUMMARY(nl)
Commando om een gebruiker operator-status 
te geven.
+OP_SUMMARY(pt_br)
Comando para dar o status de operador a um usuário em um canal.
+OP_SUMMARY(de)
Befehl um Op-Status an Benutzer zu vergeben.
+++++++++++++++++++++++++++++++++++++++++++++
+DEOP_SUMMARY(en_us)
Command to remove channel operator from an user
+DEOP_SUMMARY(pt)
Comando para remover op de um utilizador
+DEOP_SUMMARY(nl)
Commando om de operator-status van een gebruiker
af te nemen.
+DEOP_SUMMARY(pt_br)
Comando para remover o status de operador de um usuário em um canal.
+DEOP_SUMMARY(de)
Befehl um einem Benutzer den Op-Status zu nehmen.
+++++++++++++++++++++++++++++++++++++++++++++
+OP_HELP(en_us)
The OP command will give oper status to a user
using chanserv.
Syntax:
  // OP #Chan nick
+OP_HELP(pt)
O comando OP permite dar op a um utilizador
utilizando o chanserv.
Sintaxe:
  // OP #Canal nick
+OP_HELP(nl)
Het OP commando geeft operator-status aan een
gebruiker met behulp van chanserv.
Syntax:
  // OP #kanaal bijnaam
+OP_HELP(pt_br)
O comando OP permite dar o status de operador a um usuário
utilizando o chanserv.
Sintaxe:
  // OP #Canal nick
+OP_HELP(de)
Der OP Befehl gibt einem Benutzer Op-Status mittels ChanServ.
Syntax:
  // OP #channel nick
+++++++++++++++++++++++++++++++++++++++++++++
+DEOP_HELP(en_us)
The DEOP command will remove oper status from a user
using chanserv.
Syntax:
  // DEOP #Chan nick
+DEOP_HELP(pt)
O comando DEOP permite remover o op a um utilizador
utilizando o chanserv.
Sintaxe:
  // DEOP #Canal nick
+DEOP_HELP(nl)
Het DEOP commando verwijdert de operator-status 
van een gebruiker met behulp van chanserv.
Syntax:
 // DEOP #kanaal bijnaam
+DEOP_HELP(pt_br)
O comando DEOP permite remover de um usuário o status de operador,
utilizando o chanserv.
Sintaxe:
  // DEOP #Canal nick
+DEOP_HELP(de)
Der DEOP Befehl nimmt einem Benutzer mittels ChanServ den Op-Status.
Syntax:
  // DEOP #channel nick
+++++++++++++++++++++++++++++++++++++++++++++
+CHAN_OP_SYNTAX(en_us)
Invalid Syntax!
Syntax:
  // OP #Chan nick
+CHAN_OP_SYNTAX(pt)
Sintaxe inválida!
Sintaxe:
  // OP #Canal nick
+CHAN_OP_SYNTAX(nl)
Ongeldige syntax!
Syntax:
  // OP #kanaal bijnaam
+CHAN_OP_SYNTAX(pt_br)
Sintaxe inválida!
Sintaxe:
  // OP #Canal nick
+CHAN_OP_SYNTAX(de)
Ungültige Syntax.
Syntax:
  // OP #channel nick
+++++++++++++++++++++++++++++++++++++++++++++
+CHAN_DEOP_SYNTAX(en_us)
Invalid Syntax!
Syntax:
  // DEOP #Chan nick
+CHAN_DEOP_SYNTAX(pt)
Sintaxe inválida!
Sintaxe:
  // DEOP #Canal nick
+CHAN_DEOP_SYNTAX(nl)
Ongeldige syntax!
Syntax:
  // DEOP #kanaal bijnaam    
+CHAN_DEOP_SYNTAX(pt_br)
Sintaxe inválida!
Sintaxe:
  // DEOP #Canal nick
+CHAN_DEOP_SYNTAX(de)
Ungültige Syntax.
Syntax:
  // DEOP #channel nick
+++++++++++++++++++++++++++++++++++++++++++++  
+NO_OPDEOP_PERM_ON_X(en_us)
You don't have OP/DEOP permission on \B%s\B !
+NO_OPDEOP_PERM_ON_X(pt)
Não tem permissão de OP/DEOP kick no \B%s\B !
+NO_OPDEOP_PERM_ON_X(nl)
U heeft geen OP/DEOP permissie op \B%s\B !
+NO_OPDEOP_PERM_ON_X(pt_br)
Você não tem permissão para utilizar OP/DEOP no \B%s\B!
+NO_OPDEOP_PERM_ON_X(de)
Du hast keine Berechtigung für OP/DEOP in \B%s\B!
+++++++++++++++++++++++++++++++++++++++++++++
+CANT_DEOP_ADMIN_X_ON_X(en_us)
You can't deop \B%s\B, he/she has admin status on \B%s\B.
+CANT_DEOP_ADMIN_X_ON_X(pt)
Não pode remover o op a \B%s\B, ele está com administrar no \B%s\B.
+CANT_DEOP_ADMIN_X_ON_X(nl)
U kunt de operator status van \B%s\B niet verwijderen,
want deze heeft admin-status op %s.
+CANT_DEOP_ADMIN_X_ON_X(pt_br)
Você não pode remover o op de \B%s\B, ele possui status de administrador no \B%s\B.
+CANT_DEOP_ADMIN_X_ON_X(de)
DEOP nicht möglich. \B%s\B hat Admin-Status in \B%s\B.
+++++++++++++++++++++++++++++++++++++++++++++
+NICK_X_ALREADY_OP_ON_X(en_us)
\B%s\B is already op on \B%s\B !
+NICK_X_ALREADY_OP_ON_X(pt)
\B%s\B já está com op no \B%s\B !
+NICK_X_ALREADY_OP_ON_X(nl)
\B%s\B is al operator op \B%s\B !
+NICK_X_ALREADY_OP_ON_X(pt_br)
\B%s\B já está com op no \B%s\B!
+NICK_X_ALREADY_OP_ON_X(de)
\B%s\B ist bereits OP in \B%s\B!
+++++++++++++++++++++++++++++++++++++++++++++
+NICK_X_NOT_OP_ON_X(en_us)
\B%s\B is not op on \B%s\B !
+NICK_X_NOT_OP_ON_X(pt)
\B%s\B nao esta com op no \B%s\B !
+NICK_X_NOT_OP_ON_X(nl)
\B%s\B is geen operator op \B%s\B !
+NICK_X_NOT_OP_ON_X(pt_br)
\B%s\B não está com op no \B%s\B!
+NICK_X_NOT_OP_ON_X(de)
\B%s\B ist kein Op in \B%s\B!