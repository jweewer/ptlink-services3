+++++ cs_role.l
+++++ en_us: Lamego
+++++ nl: ^Stinger^, Ldp
+++++ pt_br: openglx (revisado por Ricardo)
+++++ de: jo

+ROLE_SUMMARY(en_us)
Manages channel roles.
+ROLE_SUMMARY(pt)
Gere as funções num canal.
+ROLE_SUMMARY(nl)
Beheer kanaal-rollen.
+ROLE_SUMMARY(pt_br)
Gerencia as funções em um canal.
+ROLE_SUMMARY(de)
Verwaltet Rollen/Funktionen im Channel.
+++++++++++++++++++++++++++++++++++++++++++++
+ROLE_HELP(en_us)
The ROLE command provides all the required functions
to manage channels roles.
The available sub-commands are:
    CREATE - Create a role
    DROP   - Drop a role
    VIEW   - List roles
    LIST   - List users with roles
    ADD    - Add a user with  roles
    DEL    - Delete user from roles
    SETMSG - Set a user role message
    SET    - Set a role master/properties
For help on an command type:
  // HELP ROLE command
+ROLE_HELP(pt)
O comando ROLE disponibiliza todas as funções
necessárias para gerir as funções nos canais.
Os subcomandos disponíveis são:
    CREATE - Cria uma função
    DROP   - Elimina uma função
    VIEW   - Lista funções disponíveis
    LIST   - Lista utilizadores com funções no canal
    ADD    - Adiciona utilizador com uma dada função
    DEL    - Retira um utilizador de qualquer função no canal
    SETMSG - Altera a mensagem associada à função do utilizador
    SET    - Define a função mestre/propriedades de uma função    
Para mais ajuda digite:
  // HELP ROLE comando
+ROLE_HELP(nl)
Het ROLE commando biedt alle functies aan om de
kanaal-rollen te kunnen beheren.
De aangeboden functies zijn:
    CREATE - Maak een rol aan
    DROP   - Verwijder een rol
    VIEW   - Geef een lijst van alle rollen
    LIST   - Geef een lijst van gebruikers met een rol
    ADD    - Voeg een gebruiker toe aan een rol
    DEL    - Verwijder een gebruiker van een rol
    SETMSG - Geeft een gebruiker-rol bericht
    SET    - Stelt een rol master/functies in
Voor meer informatie // HELP ROLE commando
+ROLE_HELP(pt_br)
O comando ROLE disponibiliza todas as funções
necessárias para gerenciar as funções nos canais.
Os subcomandos disponíveis são:
    CREATE - Cria uma função
    DROP   - Elimina uma função
    VIEW   - Lista funções disponíveis
    LIST   - Lista usuários com funções no canal
    ADD    - Adiciona usuário a uma função
    DEL    - Retira um usuário de qualquer função no canal
    SETMSG - Altera a mensagem associada à função do usuário
    SET    - Define a função mestre/propriedades de uma função    
Para mais ajuda digite:
  // HELP ROLE comando
+ROLE_HELP(de)
Der ROLE Befehl bietet alle benötigten Funktionen, um die 
Rollen/Funktionen in ein Channel zu verwalten.
Gültige Unterbefehle sind:
    CREATE - Erstelle eine Rolle
    DROP   - Lösche eine Rolle
    VIEW   - Zeige die Rollen an
    LIST   - Zeige Bentuzer mit Rollen an
    ADD    - Füge Benutzer mit Rolle hinzu
    DEL    - Lösche einen Benutzer aus einer Rolle
    SETMSG - Setze eine Nachricht für einen Benutzer mit Rollenfunktion 
    SET    - Setze Master und/oder Eigenschaften für eine Rolle
Für weitere Hilfe verwende bitte:
  // HELP ROLE Befehl
+++++++++++++++++++++++++++++++++++++++++++++
+CHAN_ROLE_SYNTAX(en_us)
Invalid syntax
Syntax:
  // ROLE command
+CHAN_ROLE_SYNTAX(pt)
Sintaxe inválida.
Syntaxe:
  // ROLE comando
+CHAN_ROLE_SYNTAX(nl)
Ongeldige syntax
Syntax: 
    // ROLE commando
+CHAN_ROLE_SYNTAX(pt_br)
Sintaxe inválida!
Sintaxe:
  // ROLE comando
+CHAN_ROLE_SYNTAX(de)
Ungültige Syntax
Syntax: 
    // ROLE command
+++++++++++++++++++++++++++++++++++++++++++++
+CHAN_ROLE_X_X_ALREADY_EXISTS(en_us)
Role \B%s\B already exists on \B%s\B!
+CHAN_ROLE_X_X_ALREADY_EXISTS(pt)
A função \B%s\B já existe no \B%s\B!
+CHAN_ROLE_X_X_ALREADY_EXISTS(nl)
Rol \B%s\B bestaat al op kanaal \B%s\B!
+CHAN_ROLE_X_X_ALREADY_EXISTS(pt_br)
A função \B%s\B já existe no \B%s\B!
+CHAN_ROLE_X_X_ALREADY_EXISTS(de)
Die Rolle/Funktion \B%s\B gibt es bereits in \B%s\B!
+++++++++++++++++++++++++++++++++++++++++++++
+CHAN_ROLE_X_X_NOT_FOUND(en_us)
There is no role \B%s\B on \B%s\B!
+CHAN_ROLE_X_X_NOT_FOUND(pt)
Não existe a função \B%s\B no \B%s\B!
+CHAN_ROLE_X_X_NOT_FOUND(nl)
Er is geen rol \B%s\B op kanaal \B%s\B!
+CHAN_ROLE_X_X_NOT_FOUND(pt_br)
Não existe a função \B%s\B no \B%s\B!
+CHAN_ROLE_X_X_NOT_FOUND(de)
Die Rolle/Funktion \B%s\B gibt es in \B%s\B nicht!
+++++++++++++++++++++++++++++++++++++++++++++
+CHAN_ROLE_X_X_CREATED(en_us)
Role \B%s\B was created on \B%s\B.
+CHAN_ROLE_X_X_CREATED(pt)
A função \B%s\B foi criada no \B%s\B.
+CHAN_ROLE_X_X_CREATED(nl)
Rol \B%s\B is aangemaakt op kanaal \B%s\B.
+CHAN_ROLE_X_X_CREATED(pt_br)
A função \B%s\B foi criada no \B%s\B.
+CHAN_ROLE_X_X_CREATED(de)
Die Rolle \B%s\B wurde für \B%s\B erstellt.
+++++++++++++++++++++++++++++++++++++++++++++
+CHAN_ROLE_CREATE_ERROR_X_X(en_us)
Unable to create role \B%s\B on \B%s\B!
+CHAN_ROLE_CREATE_ERROR_X_X(pt)
Não foi possível criar a função \B%s\B no \B%s\B!
Por favor tente mais tarde.
+CHAN_ROLE_CREATE_ERROR_X_X(nl)
Kan rol \B%s\B niet aanmaken op \B%s\B!
+CHAN_ROLE_CREATE_ERROR_X_X(pt_br)
Não foi possível criar a função \B%s\B no \B%s\B!
+CHAN_ROLE_CREATE_ERROR_X_X(de)
Die Rolle \B%s\B konnte in \B%s\B nicht erstellt werden!
+++++++++++++++++++++++++++++++++++++++++++++
+CHAN_ROLE_DROP_SYNTAX(en_us)
Invalid syntax!
Syntax:
  \B// ROLE #chan DROP role \B
+CHAN_ROLE_DROP_SYNTAX(pt)
Sintaxe inválida!
Syntaxe:
 // ROLE #chan DROP role
+CHAN_ROLE_DROP_SYNTAX(nl)
Ongeldige syntax!
syntax:
  // ROLE #kanaal DROP rol
+CHAN_ROLE_DROP_SYNTAX(pt_br)
Sintaxe inválida!
Sintaxe:
  \B// ROLE #Canal DROP função \B
+CHAN_ROLE_DROP_SYNTAX(de)
Ungültige Syntax!
Syntax:
  \B// ROLE #chan DROP role \B
+++++++++++++++++++++++++++++++++++++++++++++
+CHAN_ROLE_X_X_DROP(en_us)
Role \B%s\B was dropped on \B%s\B.
+CHAN_ROLE_X_X_DROP(pt)
A função \B%s\B foi apagada do canal \B%s\B.
+CHAN_ROLE_X_X_DROP(nl)
Rol \B%s\B is verwijderd op kanaal \B%s\B.
+CHAN_ROLE_X_X_DROP(pt_br)
A função \B%s\B foi apagada do canal \B%s\B.
+CHAN_ROLE_X_X_DROP(de)
Die Rolle \B%s\B wurde gelöscht für \B%s\B.
+++++++++++++++++++++++++++++++++++++++++++++
+INVALID_ACTION_X(en_us)
Invalid action: \B%s\B !
To list available actions type:
  \B// HELP ACTIONLIST\B .
+INVALID_ACTION_X(pt)
Acção inválida: \B%s\B !
Para listar as acções disponíveis digite:
  \B// HELP ACTIONLIST\B \B
+INVALID_ACTION_X(nl)
Ongeldige actie: \B%s\B !
Voor een lijst van geldige acties, typ
   \B// HELP ACTIONLIST\B .
+INVALID_ACTION_X(pt_br)
Ação inválida: \B%s\B!
Para listar as ações disponíveis, digite:
  \B// HELP ACTIONLIST\B
+INVALID_ACTION_X(de)
Ungültige Aktion: \B%s\B !
Eine Liste mit gültigen Aktionen erhälst du mit:
  \B// HELP ACTIONLIST\B .
+++++++++++++++++++++++++++++++++++++++++++++
+INVALID_PRIVILEGE_X(en_us)
Invalid permission: \B%s\B
To list available permissions type \B// HELP PERMLIST\B.
+INVALID_PRIVILEGE_X(pt)
Privilégio inválido: \B%s\B
Para listar os permissões disponíveis digite:
   \B// HELP PERMLIST\B\B
+INVALID_PRIVILEGE_X(nl)
Ongeldige privilege: \B%s\B
Voor een lijst van alle geldige permissies, typ
\B// HELP PERMLIST\B.
+INVALID_PRIVILEGE_X(pt_br)
Permissão inválida: \B%s\B
Para listar as permissões disponíveis, digite:
   \B// HELP PERMLIST\B
+INVALID_PRIVILEGE_X(de)
Ungültige Rechtevergabe: \B%s\B
Eine Liste mit möglichen Rechten erhälst du unter \B// HELP PERMLIST\B.
+++++++++++++++++++++++++++++++++++++++++++++
+CHAN_ROLE_ACTIONLIST(en_us)
*** Available role actions
  aadm - auto admin on join
  aop - auto op on join
  avoice - auto voice on join
  notice - send notice to the channel on join
  msg - send private message to the channel on join
***
+CHAN_ROLE_ACTIONLIST(pt)
*** Ações disponíveis
  aadm - auto admin
  aop - auto op
  avoice - auto voice
  notice - envia notice para o canal
  msg - envia mensagem para o canal
***
+CHAN_ROLE_ACTIONLIST(nl)
*** Beschikbare rol acties
  aadm - auto-admin bij binnenkomst
  aop - auto-op bij binnenkomst
  avoice - auto-voice bij binnenkomst
  notice - stuur notice naar kanaal bij binnenkomst
  msg - stuur prive bericht naar kanaal bij binnenkomst
***
+CHAN_ROLE_ACTIONLIST(pt_br)
*** Ações disponíveis
  aadm - auto admin
  aop - auto op
  avoice - auto voice
  notice - envia notice para o canal
  msg - envia mensagem para o canal
***
+CHAN_ROLE_ACTIONLIST(de)
*** Verfügbare Aktionen sind
  aadm - setzte admin Status beim Betreten des Channels
  aop - setze op Status  beim Betreten
  avoice - setze +voice beim Betreten
  notice - sende Nachricht an den Channel beim Betreten
  msg - sende private Nachricht an den Channel beim Betreten
***
+++++++++++++++++++++++++++++++++++++++++++++
+CHAN_ROLE_PERMLIST(en_us)
*** Available role permissions
  set - can use SET
  kick - can use KICK
  opdeop - can use OP/DEOP
  voice - can use VOICE/DEVOICE
  invite - can use the INVITE command
  unban - can use the UNBAN command
  clear - can use the CLEAR command
  view - can use the ROLE VIEW command
  list - can use the ROLE LIST command
  akick - can use the AKICK command
***
+CHAN_ROLE_PERMLIST(pt)
*** Privilégios disponíveis
  set - pode utilizar o comando SET
  kick - pode utilizar o comando KICK
  opdeop - pode utilizar o comando OP/DEOP
  voice - pode utilizar o commando VOICE/DEVOICE
  invite - pode utilizar o comando INVITE
  unban - pode utilizar o comando UNBAN
  clear - pode utilizar o comando CLEAR
  view - pode utilizar o commando ROLE VIEW
  list - pode utilizar o commando ROLE LIST
  akick - pode utilizar o comando AKICK
***
+CHAN_ROLE_PERMLIST(nl)
*** Beschikbare rol permissies
  set - gebruiker kan SET gebruiken
  kick - gebruiker kan KICK gebruiken
  opdeop - gebruiker kan OP/DEOP gebruiken
  voice - gebruiker kan VOICE/DEVOICE gebruiken
  invite - gebruiker kan INVITE gebruiken
  unban - gebruiker kan UNBAN gebruiken
  clear - gebruiker kan CLEAR gebruiken
  view - gebruiker kan ROLE VIEW gebruiken
  list - gebruiker kan ROLE LIST gebruiken
  akick - gebruiker kan AKICK gebruiken
+CHAN_ROLE_PERMLIST(pt_br)
*** Privilégios disponíveis
  set - pode utilizar o comando SET
  kick - pode utilizar o comando KICK
  opdeop - pode utilizar o comando OP/DEOP
  voice - pode utilizar o comando VOICE/DEVOICE
  invite - pode utilizar o comando INVITE
  unban - pode utilizar o comando UNBAN
  clear - pode utilizar o comando CLEAR
  view - pode utilizar o comando ROLE VIEW
  list - pode utilizar o comando ROLE LIST
  akick - pode utilizar o comando AKICK
***
+CHAN_ROLE_PERMLIST(de)
*** Mögliche Rollenrechte sind:
  set - Benutzer kann den SET - Befehl benutzen
  kick - ... kann kicken (KICK)
  opdeop - ... kann OP/DEOP verwenden
  voice - ... kann VOICE/DEVOICE verwenden
  invite - ... kann INVITE Befehl verwenden
  unban - ... kann UNBAN Befehl verwenden
  clear - ... kann CLEAR Befehl verwenden
  view - ... kann den ROLE VIEW Befehl benutzen
  list - ... darf den ROLE LIST Befehl verwenden
  akick - ... darf den AKICK Befehl verwenden
***
+++++++++++++++++++++++++++++++++++++++++++++
+CHAN_ROLE_CREATE_SYNTAX(en_us)
Invalid syntax !
Syntax:
   \B// ROLE #chan CREATE role master_role actions:permissions\B
+CHAN_ROLE_CREATE_SYNTAX(pt)
Sintaxe inválida:
Sintaxe:
   \B// ROLE #chan CREATE função função_mestre acções:permissões\B
+CHAN_ROLE_CREATE_SYNTAX(nl)
Ongeldige syntax !
   \B// ROLE #kanaal CREATE rol_naam master_rol acties:permissies\B
+CHAN_ROLE_CREATE_SYNTAX(pt_br)
Sintaxe inválida!
Sintaxe:
   \B// ROLE #Canal CREATE função função_mestre ações:permissões\B
+CHAN_ROLE_CREATE_SYNTAX(de)
Ungültige Syntax !
Syntax:
   \B// ROLE #Channel CREATE role master_role actions:permissions\B
+++++++++++++++++++++++++++++++++++++++++++++
+FOUNDER_ROLE_CREATED_X(en_us)
Founder role was created on \B%s\B.
+FOUNDER_ROLE_CREATED_X(pt)
A regra founder foi criada no \B%s\B.
+FOUNDER_ROLE_CREATED_X(nl)
Eigenaar-rol is aangemaakt op kanaal \B%s\B.
+FOUNDER_ROLE_CREATED_X(pt_br)
A regra founder foi criada no \B%s\B.
+FOUNDER_ROLE_CREATED_X(de)
Gründerrolle für \B%s\B wurde erstellt.
+++++++++++++++++++++++++++++++++++++++++++++
+FOUNDER_ROLE_CREATE_ERROR_X(en_us)
Error creating founder role on \B%s\B.
+FOUNDER_ROLE_CREATE_ERROR_X(pt)
Erro criando a regra founder no \B%s\B.
+FOUNDER_ROLE_CREATE_ERROR_X(nl)
Fout bij maken eigenaar-rol op kanaal \B%s\B.
+FOUNDER_ROLE_CREATE_ERROR_X(pt_br)
Erro criando a regra founder no \B%s\B.
+FOUNDER_ROLE_CREATE_ERROR_X(de)
Fehler beim Erstellen der Gründerrolle für \B%s\B.
+++++++++++++++++++++++++++++++++++++++++++++
+CHAN_ROLE_ADD_SYNTAX(en_us)
Syntax:
  \B// ROLE #chan ADD role nick [message]\B
+CHAN_ROLE_ADD_SYNTAX(pt)
Sintaxe:
  \B// ROLE #chan ADD função nick [mensagem]\B  
+CHAN_ROLE_ADD_SYNTAX(nl)
Syntax:
  \B// ROLE #kanaal ADD role bijnaam [bericht]\B
+CHAN_ROLE_ADD_SYNTAX(pt_br)
Sintaxe:
  \B// ROLE #Canal ADD função nick [mensagem]\B  
+CHAN_ROLE_ADD_SYNTAX(de)
Syntax:
  \B// ROLE #Channel ADD role nick [message]\B
+++++++++++++++++++++++++++++++++++++++++++++  
+NICK_X_IS_X_ON_X(en_us)
Nick \B%s\B is already \B%s\B on \B%s\B.
+NICK_X_IS_X_ON_X(pt)
Nick \B%s\B já tem a função \B%s\B no \B%s\B.
+NICK_X_IS_X_ON_X(nl)
Bijnaam \B%s\B is \B%s\B op kanaal \B%s\B.
+NICK_X_IS_X_ON_X(pt_br)
Nick \B%s\B já tem a função \B%s\B no \B%s\B.
+NICK_X_IS_X_ON_X(de)
Der Benutzer \B%s\B ist bereits \B%s\B in \B%s\B.
+++++++++++++++++++++++++++++++++++++++++++++
+NICK_X_ADDED_TO_ROLE_X_ON_X(en_us)
Nick \B%s\B was added with role \B%s\B on \B%s\B.
+NICK_X_ADDED_TO_ROLE_X_ON_X(pt)
Nick \B%s\B adicionado com a função \B%s\B no \B%s\B.
+NICK_X_ADDED_TO_ROLE_X_ON_X(nl)
Bijnaam \B%s\B is toegevoegd met rol \B%s\B op kanaal \B%s\B.
+NICK_X_ADDED_TO_ROLE_X_ON_X(pt_br)
Nick \B%s\B foi adicionado com a função \B%s\B no \B%s\B.
+NICK_X_ADDED_TO_ROLE_X_ON_X(de)
Benutzer \B%s\B wurde mit Rolle \B%s\B in \B%s\B hinzugefügt.
+++++++++++++++++++++++++++++++++++++++++++++
+CHAN_ROLE_DEL_SYNTAX(en_us)
Syntax:
  \B// ROLE #chan DEL nick | ALL \B
+CHAN_ROLE_DEL_SYNTAX(pt)
Sintaxe:
  \B// ROLE #canal DEL nick | ALL\B  
+CHAN_ROLE_DEL_SYNTAX(nl)
Syntax:
  \B// ROLE #kanaal DEL bijnaam | ALL\B
+CHAN_ROLE_DEL_SYNTAX(pt_br)
Sintaxe:
  \B// ROLE #Canal DEL nick | ALL\B  
+CHAN_ROLE_DEL_SYNTAX(de)
Syntax:
  \B// ROLE #Channel DEL nick | ALL\B
+++++++++++++++++++++++++++++++++++++++++++++
+NICK_X_HAS_NO_ROLE_ON_X(en_us)
Nick \B%s\B has no role on \B%s\B.
+NICK_X_HAS_NO_ROLE_ON_X(pt)
Nick \B%s\B não tem nenhuma função no \B%s\B.
+NICK_X_HAS_NO_ROLE_ON_X(nl)
Bijnaam \B%s\B heeft geen rol op \B%s\B.
+NICK_X_HAS_NO_ROLE_ON_X(pt_br)
Nick \B%s\B não tem nenhuma função no \B%s\B.
+NICK_X_HAS_NO_ROLE_ON_X(de)
Benutzer \B%s\B hat keine Rolle in \B%s\B.
+++++++++++++++++++++++++++++++++++++++++++++
+NICK_X_DELETED_FROM_ROLE_X_ON_X(en_us)
Nick \B%s\B was deleted from role \B%s\B on \B%s\B.
+NICK_X_DELETED_FROM_ROLE_X_ON_X(pt)
Nick \B%s\B foi removido da função \B%s\B no \B%s\B.
+NICK_X_DELETED_FROM_ROLE_X_ON_X(nl)
Bijnaam \B%s\B is verwijderd van rol \B%s\B op \B%s\B.
+NICK_X_DELETED_FROM_ROLE_X_ON_X(pt_br)
Nick \B%s\B foi removido da função \B%s\B no \B%s\B.
+NICK_X_DELETED_FROM_ROLE_X_ON_X(de)
Benutzer \B%s\B wurde aus der Rolle \B%s\B in \B%s\B gelöscht.
+++++++++++++++++++++++++++++++++++++++++++++
+REACHED_MAX_ROLES_X(en_us)
You can't add more roles, you reached the limit of \B%d\B roles.
+REACHED_MAX_ROLES_X(pt)
Não pode adicionar mais funções.
Atingiu o limite de \B%d\B funções.
+REACHED_MAX_ROLES_X(nl)
Er kunnen geen rollen meer worden toegevoegd, limiet van \B%d\B rollen bereikt.
+REACHED_MAX_ROLES_X(pt_br)
Você não pode adicionar mais funções.
Atingiu o limite de \B%d\B funções por canal.
+REACHED_MAX_ROLES_X(de)
Du kannst keine weiteren Rollen erstellen. Das Maximum von \B%d\B Rollen ist erreicht.
+++++++++++++++++++++++++++++++++++++++++++++
+REACHED_MAX_USERS_X(en_us)
You can't assign more roles, you reached the limit of \B%d\B role users.
+REACHED_MAX_USERS_X(pt)
Não pode atribuir mais funções.
Atingiu o limite de \B%d\B utilizadores com funções no canal !
+REACHED_MAX_USERS_X(nl)
Kan geen rollen meer toewijzen, limiet van \B%d\B rol-gebruikers bereikt.
+REACHED_MAX_USERS_X(pt_br)
Não pode atribuir mais funções.
Atingiu o limite de \B%d\B usuários com funções no canal!
+REACHED_MAX_USERS_X(de)
Du kannst keine weiteren Rollen zuweisen. Das Limit von \B%d\B Benutzern mit Rolle ist erreicht.
+++++++++++++++++++++++++++++++++++++++++++++
+ROLE_LIST_HEADER_X(en_us)
*** User with roles on \B%s\B
+ROLE_LIST_HEADER_X(pt)
*** Utilizadores com funções no \B%s\B
+ROLE_LIST_HEADER_X(nl)
*** Bijnaam met rollen op \B%s\B
+ROLE_LIST_HEADER_X(pt_br)
*** Usuários com funções no \B%s\B
+ROLE_LIST_HEADER_X(de)
*** Benutzer mit Rolle/Funktion für \B%s\B
+++++++++++++++++++++++++++++++++++++++++++++
+ROLE_LIST_X_X_X(en_us)
  \B%s\B is \B%s\B %s
+ROLE_LIST_X_X_X(pt)
  \B%s\B é \B%s\B %s
+ROLE_LIST_X_X_X(nl)
  \B%s\B is \B%s\B %s
+ROLE_LIST_X_X_X(pt_br)
  \B%s\B é \B%s\B %s
+ROLE_LIST_X_X_X(de)
  \B%s\B ist \B%s\B %s
+++++++++++++++++++++++++++++++++++++++++++++
+ROLE_LIST_X_X_X_X(en_us)
  \B%s\B is \B%s\B %s with message: %s
+ROLE_LIST_X_X_X_X(pt)
  \B%s\B é \B%s\B %s com mensagem: %s  
+ROLE_LIST_X_X_X_X(nl)
  \B%s\B is \B%s\B %s met bericht: %s
+ROLE_LIST_X_X_X_X(pt_br)
  \B%s\B é \B%s\B %s com mensagem: %s  
+ROLE_LIST_X_X_X_X(de)
  \B%s\B ist \B%s\B %s mit Nachricht: %s
+++++++++++++++++++++++++++++++++++++++++++++
+ROLE_LIST_TAIL(en_us)
*** End of role list
+ROLE_LIST_TAIL(pt)
*** Fim da lista de utilizadores com funções
+ROLE_LIST_TAIL(nl)
*** Einde van rollen-lijst
+ROLE_LIST_TAIL(pt_br)
*** Fim da lista de usuários com funções
+ROLE_LIST_TAIL(de)
*** Ende der Rollenliste
+++++++++++++++++++++++++++++++++++++++++++++
+ROLE_VIEW_HEADER_X(en_us)
*** Roles on \B%s\B
+ROLE_VIEW_HEADER_X(pt)
*** Funções do \B%s\B
+ROLE_VIEW_HEADER_X(nl)
*** Rollen op \B%s\B
+ROLE_VIEW_HEADER_X(pt_br)
*** Funções do \B%s\B
+ROLE_VIEW_HEADER_X(de)
*** Rollen für \B%s\B
+++++++++++++++++++++++++++++++++++++++++++++
+ROLE_VIEW_X_X_X_X(en_us)
 %s [%s] %s:%s
+ROLE_VIEW_X_X_X_X(pt)
 %s [%s] %s:%s 
+ROLE_VIEW_X_X_X_X(nl)
 %s [%s] %s:%s
+ROLE_VIEW_X_X_X_X(pt_br)
 %s [%s] %s:%s
+ROLE_VIEW_X_X_X_X(de)
 %s [%s] %s:%s
+++++++++++++++++++++++++++++++++++++++++++++
+ROLE_VIEW_TAIL(en_us)
*** End of role list
+ROLE_VIEW_TAIL(pt)
*** Fim da lista de funções
+ROLE_VIEW_TAIL(nl)
*** Einde van rollen-lijst
+ROLE_VIEW_TAIL(pt_br)
*** Fim da lista de funções
+ROLE_VIEW_TAIL(de)
*** Ende der Rollenliste
+++++++++++++++++++++++++++++++++++++++++++++
+CHAN_ROLE_ADD_HELP(en_us)
The ROLE ADD command will add a nick to the channel user list
binding it to a given role.

Syntax: 
  \B// ROLE #chan ADD role nick [message]\B
  
The optional message will be used by some role actions.

To list the roles available on the channel type:
 \B// ROLE #chan VIEW \B
 
Example:
  // ROLE #Portugal ADD founder Lamego Welcome lazy.
+CHAN_ROLE_ADD_HELP(pt)
O comando ROLE ADD adiciona um nick com uma determinada função
num canal.

Sintaxe:
  \B// ROLE #canal ADD função nick [mensagem]\B
  
A mensagem que é opcional será enviada para o canal quando o utilizador
entre caso a sua função tenha esta acção definida.

Para listar as funções disponíveis num canal digite:
  \B// ROLE #canal VIEW\B
Exemplo:
  // ROLE #Portugal ADD founder Lamego Never say never.
+CHAN_ROLE_ADD_HELP(nl)
Het ROLE ADD commando zal een bijnaam toevoegen aan de kanaal-
gebruikerslijst met de opgegeven rol.

Syntax: 
  \B// ROLE #kanaal ADD rol bijnaam [bericht]\B
  
Bericht wordt naar het kanaal gestuurd wanneer de bijnaam het kanaal
binnenkomt als de rol een notice of msg actie bevat.
Voor een lijst van beschikbare rollen op een kanaal
  \B// ROLE #kanaal VIEW\B
Voorbeeld:
  // ROLE #blaat ADD founder Stinger Zeg nooit nooit. 
+CHAN_ROLE_ADD_HELP(pt_br)
O comando ROLE ADD adiciona um nick com uma determinada função
num canal.

Sintaxe:
  \B// ROLE #Canal ADD função nick [mensagem]\B
  
A mensagem, que é opcional, será enviada para o canal quando o utilizador
entrar (caso a sua função tenha esta ação definida).

Para listar as funções disponíveis num canal digite:
  \B// ROLE #Canal VIEW\B
Exemplo:
  // ROLE #PTC ADD founder openglx Nunca diga nunca.
+CHAN_ROLE_ADD_HELP(de)
Der ROLE ADD Befehl fügt einen Benutzer zur Rollenliste hinzu und weist ihm eine Rolle zu

Syntax: 
  \B// ROLE #Channel ADD role nick [Nachricht]\B
  
Die optionale Nachricht wird bei einigen Aktionen benutzt.

Eine Liste der vorhandenen Rollen für einen Channel erhälst du mit:
 \B// ROLE #Channel VIEW \B
 
Beispiel:
  // ROLE #Portugal ADD admin Lamego Welcome lazy.
- fügt den Benutzer Lamego zur Rolle der Admins des Cahnnels #Portugal hinzu.
+++++++++++++++++++++++++++++++++++++++++++++
+NOT_MASTER_OF_X_ON_X(en_us)
You are not master of the role \B%s\B on \B%s\B!
+NOT_MASTER_OF_X_ON_X(pt)
Não é mestre da regra \B%s\B no \B%s\B!
+NOT_MASTER_OF_X_ON_X(nl)
U bent niet de master van rol \B%s\B op \B%s\B!
+NOT_MASTER_OF_X_ON_X(pt_br)
Você não é mestre da função \B%s\B no \B%s\B!
+NOT_MASTER_OF_X_ON_X(de)
Du bist Master/Chef der Rolle \B%s\B in \B%s\B!
+++++++++++++++++++++++++++++++++++++++++++++
+NOT_MASTER_OF_NICK_X_ON_X(en_us)
You are not master of nick \B%s\B on \B%s\B!
+NOT_MASTER_OF_NICK_X_ON_X(pt)
Não é mestre do nick \B%s\B no \B%s\B!
+NOT_MASTER_OF_NICK_X_ON_X(nl)
U bent geen master van bijnaam \B%s\B op \B%s\B!
+NOT_MASTER_OF_NICK_X_ON_X(pt_br)
Você não é mestre do nick \B%s\B no \B%s\B!
+NOT_MASTER_OF_NICK_X_ON_X(de)
Du bist nicht Master/Chef des Benutzers \B%s\B in \B%s\B!
+++++++++++++++++++++++++++++++++++++++++++++
+CHAN_ROLE_SETMSG_SYNTAX(en_us)
Invalid Syntax.
Syntax: 
  \B// ROLE #chan SETMSG nick [message]\B
+CHAN_ROLE_SETMSG_SYNTAX(pt)
Sintaxe inválida.
Syntaxe:
  \B// ROLE #canal SETMSG nick [mensagem]\B  
+CHAN_ROLE_SETMSG_SYNTAX(nl)
Ongeldige Syntax.
Syntax:
  \B// ROLE #kanaal SETMSG bijnaam [bericht]\B
+CHAN_ROLE_SETMSG_SYNTAX(pt_br)
Sintaxe inválida!
Sintaxe:
  \B// ROLE #Canal SETMSG nick [mensagem]\B  
+CHAN_ROLE_SETMSG_SYNTAX(de)
Ungültige Syntax.
Syntax: 
  \B// ROLE #Channel SETMSG nick [message]\B
+++++++++++++++++++++++++++++++++++++++++++++
+MESSAGE_FOR_X_ON_X_CHANGED_TO_X(en_us)
Message for \B%s\B on \B%s\B changed to \B%s\B.
+MESSAGE_FOR_X_ON_X_CHANGED_TO_X(pt)
Mensagem para \B%s\B no \B%s\B foi alterada para \B%s\B.
+MESSAGE_FOR_X_ON_X_CHANGED_TO_X(nl)
Bericht voor \B%s\B op \B%s\B veranderd naar \B%s\B.
+MESSAGE_FOR_X_ON_X_CHANGED_TO_X(pt_br)
Mensagem para \B%s\B no \B%s\B foi alterada para \B%s\B.
+MESSAGE_FOR_X_ON_X_CHANGED_TO_X(de)
Nachricht für \B%s\B in \B%s\B geändert auf \B%s\B.
+++++++++++++++++++++++++++++++++++++++++++++
+MESSAGE_FOR_X_ON_X_REMOVED(en_us)
Message for \B%s\B on \B%s\B was removed.
+MESSAGE_FOR_X_ON_X_REMOVED(pt)
Mensgame para \B%s\B no \B%s\B foi removida.
+MESSAGE_FOR_X_ON_X_REMOVED(nl)
Bericht voor \B%s\B voor \B%s\B is verwijderd.
+MESSAGE_FOR_X_ON_X_REMOVED(pt_br)
Mensagem para \B%s\B no \B%s\B foi removida.
+MESSAGE_FOR_X_ON_X_REMOVED(de)
Nachricht für \B%s\B in \B%s\B wurde entfernt.
+++++++++++++++++++++++++++++++++++++++++++++
+CANT_DROP_ADMIN_ROLE_ON_X(en_us)
You can't drop admin role on \B%s\B!
+CANT_DROP_ADMIN_ROLE_ON_X(pt)
Não pode remover a role de administrador no \B%s\B!
+CANT_DROP_ADMIN_ROLE_ON_X(nl)
U kunt de admin role op \B%s\B niet verwijderen!
+CANT_DROP_ADMIN_ROLE_ON_X(pt_br)
Você não pode remover a função de administrador no \B%s\B!
+CANT_DROP_ADMIN_ROLE_ON_X(de)
Da kannst die Administrator Rolle nicht löschen für \B%s\B!
+++++++++++++++++++++++++++++++++++++++++++++
+CHAN_ROLE_CREATE_HELP(en_us)
The ROLE CREATE command creates a  channel role.
A channel role defines a set of actions and/or permissions 
which you can assign to users.
Syntax:
  // ROLE #chan CREATE role master_role action_list:permissions_list
  
Where:
  master_role: is the role that will manage users from the created role
    master members can ADD/DEL/SETMSG on the created role.
The actions list must be comma separated, list available actions with:
  \B// HELP ACTIONLIST\B 
The permissions list must be comma separated, list available permissions with:
  \B// HELP PERMLIST\B     

Example:
  // ROLE #Zoo CREATE Lion founder amsg,aop
+CHAN_ROLE_CREATE_HELP(pt)
O comando ROLE CREATE cria uma função(ROLE) no canal.
As funções permitem atribuir um conjunto de acções/permissões 
aos utilizadores. As funções são geridas pela função mestre
que pode adicionar/remover utilizadores na respectiva função.

Sintaxe:
  // ROLE #chan CREATE função função_mestre lista_acções:lista_permissões
  
Onde:
  função_mestre: é a função que pode adicionar/remover utilizadores à função.
A lista de ações é separada por vírgulas, para mais detalhes digite:
  \B// HELP ACTIONLIST\B 
A lista de permissões é separada por vírgulas, para mais detalhes digite:
  \B// HELP PERMLIST\B     

Exemplo:
  // ROLE #Zoo CREATE Macaco founder amsg,aop
+CHAN_ROLE_CREATE_HELP(nl)
Het ROLE CREATE commando geeft de mogelijkheid om kanaal-rollen
aan te maken. Een kanaal-rol geeft een bepaalde set permissies en/of
acties aan een aangewezen gebruiker.
Syntax:
  // ROLE #kanaal CREATE rol master_rol acties:permissies

Waar:
  master_rol: is de rol die het beheer heeft over de aangemaakte rol
    de master members kunnen ADD/DEL/DROP/SETMSG doen op de aangemaakte rol
  actie lijst: lijst van acties, voor een lijst van alle beschikbare acties
    \B// HELP ACTIONLIST\B
  permissielijst: lijst van permissies, voor een lijst van alle beschikbare
    permissies
    \B// HELP PERMLIST\B
Voorbeeld:
  // ROLE #blaat CREATE blater founder amsg,aop  
+CHAN_ROLE_CREATE_HELP(pt_br)
O comando ROLE CREATE cria uma função(ROLE) no canal.
As funções permitem atribuir um conjunto de ações/permissões 
aos usários. As funções são gereciadass pela função mestre
que pode adicionar/remover usuários na respectiva função.

Sintaxe:
  // ROLE #Canal CREATE função função_mestre lista_ações:lista_permissões
  
Onde:
  função_mestre: é a função que pode adicionar/remover utilizadores à função.
A lista de ações é separada por vírgulas, para mais detalhes digite:
  \B// HELP ACTIONLIST\B 
A lista de permissões é separada por vírgulas, para mais detalhes digite:
  \B// HELP PERMLIST\B     

Exemplo:
  // ROLE #Zoo CREATE Macaco founder amsg,aop
+CHAN_ROLE_CREATE_HELP(de)
Der Befehl ROLE CREATE erstellt eine Rolle für einen Channel.
Eine Rolle/Funktion definiert eine Reihe von Aktionen und/oder Rechten,
die du dann Benutzern zuweisen kannst.

Syntax:
  // ROLE #Channel CREATE Rolle Oberrolle Aktionsliste:Rechteliste
  
Dabei ist:
  Oberrolle: Die übergerdnete Rolle. Benutzer in der Oberrolle verwalten
  die Mitglieder in der erstellten Rolle. Mitglieder in der Oberrolle können
  die Befehle ADD/DEL/SETMSG für die Rolle anwenden

Die Liste der Aktionen muss durch Kommata getrennt werden.
Eine Liste der gültigen Aktionen erhälst du unter:
  \B// HELP ACTIONLIST\B 
Die Liste Der Rechte muss ebenfalls durch Kommata getrennt sein. 
Eine Liste der verfügbaren Rechte gibt es unter:
  \B// HELP PERMLIST\B

Beispiel:
  // ROLE #Zoo CREATE Lion founder amsg,aop
+++++++++++++++++++++++++++++++++++++++++++++
+CHAN_ROLE_VIEW_HELP(en_us)
The ROLE VIEW command will list roles available on the channel.
Syntax:
  // ROLE #chan VIEW

The list will appear with this format:
  role [master_role] actions:permissions
+CHAN_ROLE_VIEW_HELP(pt)
O comando ROLE VIEW permite listas as funções disponíveis num canal.
Sintaxe:
  // ROLE #canal VIEW

A lista aparecerá com o seguinte formato:  
  função [função_mestre] acções:permissões
+CHAN_ROLE_VIEW_HELP(nl)
Het ROLE VIEW commando geeft een lijst met beschikbare rollen
op het kanaal.
Syntax:
  // ROLE #kanaal VIEW
  
De lijst komt in het volgende formaat:
  rol [master_rol] acties:permissies
+CHAN_ROLE_VIEW_HELP(pt_br)
O comando ROLE VIEW permite listar as funções disponíveis num canal.
Sintaxe:
  // ROLE #Canal VIEW

A lista aparecerá com o seguinte formato:  
  função [função_mestre] ações:permissões
+CHAN_ROLE_VIEW_HELP(de)
der Befehl ROLE VIEW zeigte eine Liste der erstellten Rollen für den Channel.
Syntax:
  // ROLE #Channel VIEW

Die Liste erscheint dabei im Format:
  Rolle [übergeordnete Rolle] Aktionen:Rechte
+++++++++++++++++++++++++++++++++++++++++++++
+CHAN_ROLE_DROP_HELP(en_us)
The ROLE DROP command will drop a role.

Syntax:
  // ROLE #chan DROP role
  
You must be the channel founder or role master to drop the role.
+CHAN_ROLE_DROP_HELP(pt)
O comando ROLE DROP apaga uma função de um canal bem como
os utilizadores que lhe estão associados.

Sintaxe,
  // ROLE #Canal DROP função

Apenas o fundador do canal pode utilizar este comando.
+CHAN_ROLE_DROP_HELP(nl)
Het ROLE DROP commando verwijderd een rol

Syntax:
  // ROLE #kanaal DROP rol
  
U moet hiervoor kanaal-eigenaar of rol-master zijn
+CHAN_ROLE_DROP_HELP(pt_br)
O comando ROLE DROP apaga uma função de um canal bem como
os usuários que estão associados a esta função.

Sintaxe:
  // ROLE #Canal DROP função

Você precisa ser o fundador do canal ou um master da função
pode poder apagá-la.
+CHAN_ROLE_DROP_HELP(de)
Der Befehl ROLE DROP entfernt eine Rolle.

Syntax:
  // ROLE #Channel DROP Rolle
  
Um den Befehl verwenden zu können, musst du Gründer des Channels sein oder Miglied der übergeordneten Rolle.
+++++++++++++++++++++++++++++++++++++++++++++
+CHAN_ROLE_LIST_HELP(en_us)
The ROLE LIST command lists all users with assigned roles.

Syntax:
  // ROLE #chan LIST [role]
  
The list will appear with this format:
  nick is role with message: role message
  
If you specify a role on the command it will only show users
assigned to that role.
NOTE: Only users with LIST permission can use this command.
+CHAN_ROLE_LIST_HELP(pt)
O comando ROLE LIST lista todos os utilizadores com função no canal.

Sintaxe:
  // ROLE #chan LIST [função]
  
A lista aparece com o seguinte formato:  
  nick é função com mensagem: mensagem
  
Se for indicada uma função, apenas serão listados os utilizadores com essa função.
Apenas utilizadores com permissão LIST podem utilizar este comando.
+CHAN_ROLE_LIST_HELP(nl)
Het ROLE LIST commando zal een lijst geven van alle gebruikers met
toegewezen rollen.

Syntax:
  // ROLE #kanaal LIST [rol]
  
De lijst komt in het volgende formaat:
  bijnaam is rol met bericht: rol bericht
Indien een rol wordt aangegeven zal alleen een lijst worden gegeven
met alle gebruikers van deze rol. 
+CHAN_ROLE_LIST_HELP(pt_br)
O comando ROLE LIST lista todos os usuários com função no canal.

Sintaxe:
  // ROLE #Canal LIST [função]
  
A lista aparece com o seguinte formato:  
  nick é função com mensagem: mensagem
  
Se for indicada uma função, apenas serão listados os utilizadores com essa função.
Apenas utilizadores com permissão LIST podem utilizar este comando.
+CHAN_ROLE_LIST_HELP(de)
Der Befehl ROLE LIST listet alle Benutzer mit ihren Funktionen/Rollen auf.

Syntax:
  // ROLE #Channel LIST [Rolle]
  
Die Liste erscheint im Format:
  Benutzer ist \"Rolle\" mit der Nachricht: \"role message\"
  
Wenn du eine optionale Rolle mit angigst, werden nur die Mitglieder dieser Rolle angezeigt.
HINWEIS: Nur Benutzer mit \"LIST\"-Recht können den Befehl verwenden.
+++++++++++++++++++++++++++++++++++++++++++++
+CHAN_ROLE_DEL_HELP(en_us)
The ROLE DEL command will delete a nick from any assigned role
on the given channel.

Syntax:
  // ROLE #chan DEL nick | ALL
+CHAN_ROLE_DEL_HELP(pt)
O comando ROLE DEL apaga um nick de qualquer função que este tenha
no canal.

Sintaxe:
  // ROLE #Canal DEL nick | ALL
+CHAN_ROLE_DEL_HELP(nl)
Het ROLE DEL commando zel de gegeven bijnaam verwijderen van alle rollen
van het opgegeven kanaal.

Syntax:
  // ROLE #kanaal DEL bijnaam | ALL
+CHAN_ROLE_DEL_HELP(pt)
O comando ROLE DEL apaga um nick de qualquer função que este tenha
no canal.

Sintaxe:
  // ROLE #Canal DEL nick | ALL
+CHAN_ROLE_DEL_HELP(de)
Der Befehl ROLE DEL löscht einen Benutzer aus der Rollenliste des Channels.

Syntax:
  // ROLE #Channel DEL nick | ALL
+++++++++++++++++++++++++++++++++++++++++++++
+CHAN_ROLE_SETMSG_HELP(en_us)
The ROLE SETMSG command sets/removes the message associated to
an user role.

Syntax:
  // ROLE #chan SETMSG nick [message]

If no message is specified the message is deleted.
+CHAN_ROLE_SETMSG_HELP(pt)
O comando ROLE SETMSG define/remove a mensagem associada à função de
um utilizador num canal.

Sintaxe:
  // ROLE #canal SETMSG nick [mensagem]

Se a mensagem não for indicada então será removida.
+CHAN_ROLE_SETMSG_HELP(nl)
Het ROLE SETMSG commando zal een bericht neerzetten/verwijderen voor de
opgegeven bijnaam op elke rol

Syntax:
  // ROLE #kanaal SETMSG bijnaam [bericht]

Als er geen bericht wordt opgegeven, wordt het bericht verwijderd.
+CHAN_ROLE_SETMSG_HELP(pt_br)
O comando ROLE SETMSG define/remove a mensagem associada à função de
um utilizador do canal.

Sintaxe:
  // ROLE #Canal SETMSG nick [mensagem]

Se a mensagem não for indicada então será removida.
+CHAN_ROLE_SETMSG_HELP(de)
Der Befehl ROLE SETMSG setzt oder entfernt die Nachricht die einem Mitglied einer Rolle zugewiesen ist.

Syntax:
  // ROLE #Channel SETMSG Nick [Nachricht]

Wenn keine Nachricht angegeben wird, wird die bisherige Nachricht gelöscht.
+++++++++++++++++++++++++++++++++++++++++++++
+CHAN_ROLE_ACCEPT_HELP(en_us)
The ROLE ACCEPT command allows you to accept a role that was assigned 
to you on some channel.

Syntax:
  // ROLE #Chan ACCEPT
+CHAN_ROLE_ACCEPT_HELP(pt)  
O comando ROLE ACCEPT permite-lhe aceitar uma função que lhe foi atribuida
num canal.

Sintaxe:
  // ROLE #Chan ACCEPT
+CHAN_ROLE_ACCEPT_HELP(nl)
Het ROLE ACCEPT commando stelt u instaat een kanaal rol te accepteren als
deze aan u gegeven wordt.

Syntax:
  // ROLE #kanaal ACCEPT
+CHAN_ROLE_ACCEPT_HELP(pt_br)
O comando ROLE ACCEPT permite-lhe aceitar uma função que lhe foi atribuída
num canal.

Sintaxe:
  // ROLE #Canal ACCEPT
+CHAN_ROLE_ACCEPT_HELP(de)
Der ROLE ACCEPT Befehl dient dazu eine dir für den Channnel
zugewiesene Rolle anzunehmen.

Syntax:
  // ROLE #Channel ACCEPT
+++++++++++++++++++++++++++++++++++++++++++++
+CHAN_ROLE_REJECT_HELP(en_us)
The ROLE REJECT command allows you to reject a role that was assigned 
to you on some channel.

Sintax:
  // ROLE #Chan REJECT
+CHAN_ROLE_REJECT_HELP(pt)  
O comando ROLE REJECT permite-lhe recusar uma função que lhe foi atribuida
num canal.

Sintaxe:
  // ROLE #Chan REJECT
+CHAN_ROLE_REJECT_HELP(nl)
Het ROLE REJECT commando stelt u instaat een kanaal rol te weigeren als
deze aan u gegeven wordt.

Syntax:
  // ROLE #kanaal REJECT
+CHAN_ROLE_REJECT_HELP(pt_br)
O comando ROLE REJECT permite-lhe recusar uma função que lhe foi atribuída
num canal.

Sintaxe:
  // ROLE #Canal REJECT
+CHAN_ROLE_REJECT_HELP(de)
Der ROLE REJECT Befehl erlaubt es dir, eine für einen Channel zugewiesene Rolle abzulehnen.

Syntax:
  // ROLE #Channel REJECT
+++++++++++++++++++++++++++++++++++++++++++++
+CHAN_ROLE_SET_SYNTAX(en_us)
Syntax:
  // ROLE #Chan SET role MASTER master_role
  // ROLE #Chan SET role PROP actions_list:permissions_list
+CHAN_ROLE_SET_SYNTAX(pt)
Syntax:
  // ROLE #canal SET função MASTER função_mestre
  // ROLE #canal SET função PROP acções:permissões
+CHAN_ROLE_SET_SYNTAX(nl)
Syntax:
  // ROLE #kanaal SET rol MASTER master_rol
  // ROLE #kanaal SET rol PROP acties:permissies
+CHAN_ROLE_SET_SYNTAX(pt_br)
Sintaxe:
  // ROLE #Canal SET função MASTER função_mestre
  // ROLE #Canal SET função PROP ações:permissões
+CHAN_ROLE_SET_SYNTAX(de)
Syntax:
  // ROLE #Channel SET Rolle MASTER übergeordnete_Rolle
  // ROLE #Channel SET Rolle PROP Aktionenliste:Rechteliste
+++++++++++++++++++++++++++++++++++++++++++++
+CHAN_ROLE_SET_HELP(en_us)
The ROLE SET command changes a role master or actions/permissions.

Syntax:
  // ROLE #Chan SET role MASTER master_role
  // ROLE #Chan SET role PROP actions_list:permissions_list
  
The actions list must be coma separated, list availabe actions with:
  \B// HELP ACTIONLIST\B 
The permissions list must be comma separated, list available permissions with:
  \B// HELP PERMLIST\B 
NOTE: Only the channel founder can use this function.
+CHAN_ROLE_SET_HELP(pt)
O comando ROLE SET altera a função mestre ou acções/permissões
de uma função.

Sintaxe:
  // ROLE #Chan SET role MASTER master_role
  // ROLE #Chan SET role PROP actions_list:permissions_list

A lista de acções é separada por vírgulas, para mais detalhes digite: 
  \B// HELP ACTIONLIST\B
A lista de permissões é separada por vírgulas, para mais detalhes digite:
  \B// HELP PERMLIST\B
  
NOTA: Apenas o fundador de um canal pode utilizar este comando.
+CHAN_ROLE_SET_HELP(nl)
Het ROLE SET commando kan een rol-master of acties/permissies veranderen

Syntax:
  // ROLE #kanaal SET rol MASTER master_rol
  // ROLE #kanaal SET rol PROP acties:permissies

De acties moeten gescheiden worden door een komma, voor een lijst van beschikbare
acties:
  \B// HELP ACTIONLIST\B 
De permissies worden gescheiden door een komma, voor een lijst van beschikbare
permissies:
  \B// HELP PERMLIST\B
Notitie: Alleen de eigenaar kan dit commando gebruiken.
+CHAN_ROLE_SET_HELP(pt_br)
O comando ROLE SET altera a função mestre ou ações/permissões
de uma função.

Sintaxe:
  // ROLE #Canal SET role MASTER função_mestre
  // ROLE #Canal SET role PROP ações:permissões

A lista de ações é separada por vírgulas, liste as ações disponíveis com: 
  \B// HELP ACTIONLIST\B
A lista de permissões é separada por vírgulas, liste as permissões disponíveis com:
  \B// HELP PERMLIST\B
  
NOTA: Apenas o fundador de um canal pode utilizar este comando.
+CHAN_ROLE_SET_HELP(de)
Der ROLE SET Befehl ändert den Master einer Rolle oder die Aktionen/Rechte für eine Rolle.

Syntax:
  // ROLE #Channel SET role MASTER übergerdnete_Rolle
  // ROLE #Channel SET role PROP Aktionsliste:Rechteliste
  
Die Liste der Aktionen muss durch Kommata getrennt werden.
Eine Liste der gültigen Aktionen erhälst du unter:
  \B// HELP ACTIONLIST\B 
Die Liste Der Rechte muss ebenfalls durch Kommata getrennt sein. 
Eine Liste der verfügbaren Rechte gibt es unter:
  \B// HELP PERMLIST\B
Hinweis: Nur der Gründer eines Channels kann diesen Befehl verwenden.
+++++++++++++++++++++++++++++++++++++++++++
+ONLY_FOUNDER_X(en_us)
You are not the founder of \B%s\B, this command is not allowed!
+ONLY_FOUNDER_X(pt)
Não é o fundador do \B%s\B, este comando não é permitido!
+ONLY_FOUNDER_X(nl)
U bent niet de eigenaar van \B%s\B, dit commando is niet toegestaan!
+ONLY_FOUNDER_X(pt_br)
Você não é o fundador do \B%s\B, este comando não lhe é permitido!
+ONLY_FOUNDER_X(de)
Du bist nicht der Gründer von \B%s\B. Dieser Befehl ist nur dem Gründer erlaubt!
+++++++++++++++++++++++++++++++++++++++++++
+CHAN_ROLE_X_ON_X_IS_MASTER_OF_X(en_us)
Role \B%s\B on \B%s\B is master or above of \B%s\B!
+CHAN_ROLE_X_ON_X_IS_MASTER_OF_X(pt)
A função \B%s\B no \B%s\B é mestre ou está acima de \B%s\B!
+CHAN_ROLE_X_ON_X_IS_MASTER_OF_X(nl)
Rol \B%s\B op \B%s\B is master of hoger van \B%s\B!
+CHAN_ROLE_X_ON_X_IS_MASTER_OF_X(pt_br)
A função \B%s\B no \B%s\B é mestre ou está acima de \B%s\B!
+CHAN_ROLE_X_ON_X_IS_MASTER_OF_X(de)
Die Rolle \B%s\B in \B%s\B ist übergeordnete Rolle oder höher als \B%s\B!
+++++++++++++++++++++++++++++++++++++++++++++
+CHAN_ROLE_MASTER_X_ON_X_TO_X(en_us)
Master role of \B%s\B on \B%s\B changed to \B%s\B.
+CHAN_ROLE_MASTER_X_ON_X_TO_X(pt)
Função mestre de \B%s\B no \B%s\B alterada para \B%s\B.
+CHAN_ROLE_MASTER_X_ON_X_TO_X(nl)
Master-rol van \B%s\B op \B%s\B veranderd naar \B%s\B.
+CHAN_ROLE_MASTER_X_ON_X_TO_X(pt_br)
Função mestre de \B%s\B no \B%s\B alterada para \B%s\B.
+CHAN_ROLE_MASTER_X_ON_X_TO_X(de)
Übergeordnete Rolle für \B%s\B in \B%s\B wurde geändert in \B%s\B.
+++++++++++++++++++++++++++++++++++++++++++++
+NO_VIEW_PERM_ON_X(en_us)
You don't have VIEW permission on \B%s\B!
+NO_VIEW_PERM_ON_X(pt)
Não tem permissão para VIEW no \B%s\B!
+NO_VIEW_PERM_ON_X(nl)
U heeft geen VIEW permissie op \B%s\B!
+NO_VIEW_PERM_ON_X(pt_br)
Você não tem permissão para usar o comando VIEW no \B%s\B!
+NO_VIEW_PERM_ON_X(de)
Der VIEW Befehl ist dir in \B%s\B nicht erlaubt!
+++++++++++++++++++++++++++++++++++++++++++++
+NO_LIST_PERM_ON_X(en_us)
You don't have LIST permission on \B%s\B!
+NO_LIST_PERM_ON_X(pt)
Não tem permissão para LIST no \B%s\B!
+NO_LIST_PERM_ON_X(nl)
U heeft geen LIST permissie op \B%s\B!
+NO_LIST_PERM_ON_X(pt_br)
Você não tem permissão para usar o comando LIST no \B%s\B!
+NO_LIST_PERM_ON_X(de)
Der LIST Befehl ist dir in \B%s\B nicht erlaubt!
+++++++++++++++++++++++++++++++++++++++++++++
+CHAN_ROLE_NO_SELF_MASTER(en_us)
A role can't become master of itself!
+CHAN_ROLE_NO_SELF_MASTER(pt)
Uma função não pode ser mestre de si mesma!
+CHAN_ROLE_NO_SELF_MASTER(nl)
Een rol kan geen master van zichzelf zijn!
+CHAN_ROLE_NO_SELF_MASTER(pt_br)
Uma função não pode ser mestre de si mesma!
+CHAN_ROLE_NO_SELF_MASTER(de)
Eine Rolle kann nicht sich selbst übergeordnet sein!
+++++++++++++++++++++++++++++++++++++++++++++
+CANT_SET_ADMIN_MASTER(en_us)
The master for the \Badmin\B role can't be set!
+CANT_SET_ADMIN_MASTER(pt)
Não é possível definir uma função mestre para a função \Badmin\B .
+CANT_SET_ADMIN_MASTER(nl)
De master voor de \Badmin\B role kan niet veranderd worden.
+CANT_SET_ADMIN_MASTER(pt_br)
Não é possível definir uma função mestre para a função \Badmin\B!
+CANT_SET_ADMIN_MASTER(de)
Die übergeordnete Rolle für die \Badmin\B Rolle kann nicht geändert werden!
+++++++++++++++++++++++++++++++++++++++++++++
+ROLE_X_PROP_SET_X(en_us)
Role \B%s\B properties set to \B%s\B
+ROLE_X_PROP_SET_X(pt)
Propriedas da função \B%s\B alteradas para \B%s\B
+ROLE_X_PROP_SET_X(nl)
Rol \B%s\B functies veranderd naar \B%s\B 
+ROLE_X_PROP_SET_X(pt_br)
Propriedades da função \B%s\B alteradas para \B%s\B
+ROLE_X_PROP_SET_X(de)
Die Eigenschaften der Rolle \B%s\B wurden auf \B%s\B geändert.
+++++++++++++++++++++++++++++++++++++++++++++
+ROLE_PENDING(en_us)
This role will be pending until the user accepts it.
+ROLE_PENDING(pt)
Esta função ficará pendente até que o utilizador a aceite.
+ROLE_PENDING(nl)
Deze rol staat in de wachtrij tot de gebruiker deze accepteert.
+ROLE_PENDING(pt_br)
Esta função ficará pendente até que o utilizador aceite-a.
+ROLE_PENDING(de)
Diese Rolle hat schwebenden Status bis der Benutzer seine Rolle akzeptiert hat.
+++++++++++++++++++++++++++++++++++++++++++++
+ROLE_ON_X_ACCEPTED(en_us)
The role pending on \B%s\B was accepted.
+ROLE_ON_X_ACCEPTED(pt)
A função pendente no \B%s\B foi aceite
+ROLE_ON_X_ACCEPTED(nl)
De rol die in de wachtrij stond voor kanaal \B%s\B is geaccepteert.
+ROLE_ON_X_ACCEPTED(pt_br)
A função pendente no \B%s\B foi aceita.
+ROLE_ON_X_ACCEPTED(de)
Die schwebende Rolle für \B%s\B wurde akzeptiert.
+++++++++++++++++++++++++++++++++++++++++++++
+ROLE_ON_X_REJECTED(en_us)
The role pending on \B%s\B was rejected.
+ROLE_ON_X_REJECTED(pt)
A função pendente no \B%s\B foi rejeitada
+ROLE_ON_X_REJECTED(nl)
De rol die in de wachtrij stond voor kanaal \B%s\B is geweigerd.
+ROLE_ON_X_REJECTED(pt_br)
A função pendente no \B%s\B foi rejeitada.
+ROLE_ON_X_REJECTED(de)
Die schwebende Rolle für \B%s\B wurde NICHT akzeptiert!
+++++++++++++++++++++++++++++++++++++++++++++
+NO_ROLE_PENDING_ON_X(en_us)
You have no role pending on \B%s\B.
+NO_ROLE_PENDING_ON_X(pt)
Não tem nenhuma função pendente no \B%s\B.
+NO_ROLE_PENDING_ON_X(nl)
U heeft geen rol in de wachtrij staan voor kanaal \B%s\B.
+NO_ROLE_PENDING_ON_X(pt_br)
Você não tem nenhuma função pendente no \B%s\B.
+NO_ROLE_PENDING_ON_X(de)
Es gibt keine schwebenden Rollen für \B%s\B.
+++++++++++++++++++++++++++++++++++++++++++++
+ROLE_X_X_X_X_PENDING(en_us)
You have been added to role \B%s\B on \B%s\B.
Accept or reject it with // ROLE \B%s\B ACCEPT or // ROLE \B%s\B REJECT.
+ROLE_X_X_X_X_PENDING(pt)
Foi-te atribuida a função \B%s\B no \B%s\B.
Aceita ou rejeita com // ROLE \B%s\B ACCEPT ou // ROLE \B%s\B REJECT.
+ROLE_X_X_X_X_PENDING(nl)
U bent toegevoegd aan rol \B%s\B op \B%s\B.
Accepteer of weiger deze met // ROLE \B%s\B ACCEPT of // ROLE \B%s\B REJECT.
+ROLE_X_X_X_X_PENDING(pt_br)
Você foi adicionado à função \B%s\B no \B%s\B.
Aceite-a ou rejeite-a com // ROLE \B%s\B ACCEPT ou // ROLE \B%s\B REJECT.
+ROLE_X_X_X_X_PENDING(de)
Du wurdest der Rolle \B%s\B für \B%s\B hinzugefügt.
Akzeptiere oder weise die Rolle zurück durch // ROLE \B%s\B ACCEPT um zu akzeptieren
oder eben durch // ROLE \B%s\B REJECT um sie zurückzuweisen.
+++++++++++++++++++++++++++++++++++++++++++++
+REJECTED_ROLE(en_us)
\B(Rejected)\B
+REJECTED_ROLE(pt)
\B(Rejeite)\B
+REJECTED_ROLE(nl)
\B(Geweigerd)\B
+REJECTED_ROLE(pt_br)
\B(Rejeite)\B
+REJECTED_ROLE(de)
\B(Zurückgewiesen)\B
+++++++++++++++++++++++++++++++++++++++++++++
+PENDING_ROLE(en_us)
\B(Pending)\B
+PENDING_ROLE(pt)
\B(Pendente)\B
+PENDING_ROLE(nl)
\B(In de wacht)\B
+PENDING_ROLE(pt_br)
\B(Pendente)\B
+PENDING_ROLE(de)
\B(Schwebend)\B
+++++++++++++++++++++++++++++++++++++++++++++
+DELETED_ALL_ROLES_X(en_us)
Deleted all roles on \B%s\B .
+DELETED_ALL_ROLES_X(pt)
Removeu todas as funções no \B%s\B .
+DELETED_ALL_ROLES_X(nl)
Alle roles op \B%s\B zijn verwijderd.

