++++ ns_group.l
++++ en_us: Lamego, Stinger
++++ nl: Stinger, Ldp
++++ pt: Lamego
++++ pt_br: Ricardo
++++ de: jo

+NS_GROUP_SUMMARY(en_us)
Group management functions.
+NS_GROUP_SUMMARY(nl)
Groep management functies.
+NS_GROUP_SUMMARY(pt_br)
Gerenciamento das funções de grupo.
+NS_GROUP_SUMMARY(de)
Funktionen zur Verwaltung von Grguppen.
++++++++++++++++++++++++++++++++++++++++++++
+NS_GROUP_SYNTAX(en_us)
The available sub-commands are:
  ADD     - Add a nick to a group 
  CREATE  - Create a new group
  DROP    - Drop an existing group
  DEL     - Remove an user from a group
  INFO    - Display infomation for a group
  LIST    - List existing groups
  SET     - Change a group setting
  SHOW    - List the groups you belong to
+NS_GROUP_SYNTAX(nl)
De beschikbare subcommando's zijn:
  ADD     - Voeg een bijnaam aan een groep toe
  CREATE  - Maak een nieuwe groep
  DROP    - Maak een bestaande groep ongedaan
  DEL     - Verwijder een gebruiker van een groep
  INFO    - Laat informatie zien van een groep
  LIST    - Laat alle bestaande groepen zien
  SET     - Verander groep instellingen
  SHOW    - Laat alle groepen zien waar u in voor komt.
++++++++++++++++++++++++++++++++++++++++++++
+NS_GROUP_HELP(en_us)
The GROUP command provides all the required functions
to manage nickserv groups.
@NS_GROUP_SYNTAX
To get help on a specific subcommand use 
  // HELP GROUP subcommand
+NS_GROUP_HELP(nl)
Het GROUP commando heeft alle benodigde functies om de groepen
binnen nickserv te beheren.
@NS_GROUP_SYNTAX
Om hulp te krijgen over een specifiek subcommando gebruik
  // HELP GROUP subcommando
++++++++++++++++++++++++++++++++++++++++++++
+NS_GROUP_ADD_SYNTAX(en_us)
Syntax:
  \B// GROUP ADD group nick [+time[d|Y]]\B
+NS_GROUP_ADD_SYNTAX(nl)
Syntax:
  \B// GROUP ADD groep bijnaam [+tijd[dY]]\B
++++++++++++++++++++++++++++++++++++++++++++  
+NS_GROUP_ADD_HELP(en_us)
The GROUP ADD command adds a nick to a group.
If a time parameter is specified then the nick will be removed
from the group after that time has expired.

@NS_GROUP_ADD_SYNTAX

You need to be master for the group to be able to add users to it.
+NS_GROUP_ADD_HELP(nl)
Het GROUP ADD commando voegt een bijnaam toe aan een groep
Als de tijd parameter is aangegeven zal de bijnaam worden verwijderd
wanneer deze tijd is verlopen.

@NS_GROUP_ADD_SYNTAX

U moet de master van de groep zijn om bijnamen toe te kunnen voegen.
++++++++++++++++++++++++++++++++++++++++++++  
+NS_GROUP_CREATE_SYNTAX(en_us)
Syntax:
  \B// GROUP CREATE group[automodes] master_group description\B
+NS_GROUP_CREATE_SYNTAX(nl)
Syntax:
  \B// GROUP CREATE groep[automodes] master_groep beschrijving\B
++++++++++++++++++++++++++++++++++++++++++++
+NS_GROUP_CREATE_HELP(en_us)
The GROUP CREATE command creates a nickserv group.
You can define auto usermodes to be set when group members identify,
to do this put the umodes letters between \"[\" \"]\" after the group name.
The master_group is the group that can add/remove users to the created group.

@NS_GROUP_CREATE_SYNTAX

Only services root can create groups.
+NS_GROUP_CREATE_HELP(nl)
Het GROUP CREATE commando maakt een nieuwe nickserv groep aan.
Er kunnen standaard automodes worden mee gegeven die een member krijgt als
deze zich identificeerd. Om dit te kunnen doen moeten de modes tussen 
de \"[\" \"]\" komen na de groepsnaam. De master_groep is de groep die members kan
toevoegen/verwijderen aan de aangemaakte groep.

@NS_GROUP_CREATE_SYNTAX

Alleen services root kan groepen aanmaken.
++++++++++++++++++++++++++++++++++++++++++++
+NS_GROUP_DEL_SYNTAX(en_us)
Syntax:
  \B// GROUP DEL group nick\B
+NS_GROUP_DEL_SYNTAX(nl)
Syntax:
  \B// GROUP DEL groep bijnaam\B
++++++++++++++++++++++++++++++++++++++++++++
+NS_GROUP_DEL_HELP(en_us)
The GROUP DEL command deletes an user from a group.

@NS_GROUP_DEL_SYNTAX

You need to be master for the group to be able to delete users from it.
+NS_GROUP_DEL_HELP(nl)
Het GROUP DEL commando verwijderd een bijnaam van een groep.

@NS_GROUP_DEL_SYNTAX

U moet master van de groep zijn om een bijnaam te kunnen verwijderen.
++++++++++++++++++++++++++++++++++++++++++++
+NS_GROUP_DROP_SYNTAX(en_us)
Syntax:
  \B// GROUP DROP group\B
+NS_GROUP_DROP_SYNTAX(nl)
Syntax:
  \B// GROUP DROP groep\B
++++++++++++++++++++++++++++++++++++++++++++
+NS_GROUP_DROP_HELP(en_us)
The GROUP DROP command deletes a group.
The members of the group are removed before the group deletation.

@NS_GROUP_DROP_SYNTAX

Only services root can drop groups.
+NS_GROUP_DROP_HELP(nl)
Het GROUP DROP commando maakt een groep ongedaan.
De members van de groep worden verwijderd voor dat de groep wordt
verwijderd.

@NS_GROUP_DROP_SYNTAX

Alleen dervices root kan een groep ongedaan maken.
++++++++++++++++++++++++++++++++++++++++++++  
+NS_GROUP_INFO_SYNTAX(en_us)
  \B// GROUP INFO group\B
+NS_GROUP_INFO_SYNTAX(nl)
  \B// GROUP INFO groep\B
++++++++++++++++++++++++++++++++++++++++++++
+NS_GROUP_INFO_HELP(en_us)
The GROUP INFO command displays information for a group.

@NS_GROUP_INFO_SYNTAX
+NS_GROUP_INFO_HELP(nl)
Het GROUP LIST commando geeft de informatie van een groep weer.

@NS_GROUP_INFO_SYNTAX
++++++++++++++++++++++++++++++++++++++++++++
+NS_GROUP_LIST_SYNTAX(en_us)
Syntax:
  \B// GROUP LIST [pattern]\B
+NS_GROUP_LIST_SYNTAX(nl)
Syntax:
  \B// GROUP LIST [patroon]\B
++++++++++++++++++++++++++++++++++++++++++++
+NS_GROUP_LIST_HELP(en_us)
The GROUP LIST command lists the existing groups.
A pattern option can be used to filter the list.

@NS_GROUP_LIST_SYNTAX

Only services root can list groups.
+NS_GROUP_LIST_HELP(nl)
Het GROUP LIST commando geeft een lijst van de aanwezige groepen
Een patroon optie kan gebruikt worden als filter voor de lijst.

@NS_GROUP_LIST_SYNTAX

Alleen services root kan een lijst opvragen van de groepen.
++++++++++++++++++++++++++++++++++++++++++++
+NS_GROUP_SET_SYNTAX(en_us)
Syntax:
  \B// GROUP SET option value\B
Available options are:
  autoumodes - usermodes to be set when user identifies
  desc       - group description
  maxusers   - max number of users that can be added to the group  
+NS_GROUP_SET_SYNTAX(nl)
Syntax: 
  \B// GROUP SET optie waarde\B
Beschikbare opties zijn:
  autoumodes - gebruikermodes die worden gezet bij identificatie
  maxusers   - maximaal aantal members voor een groep
++++++++++++++++++++++++++++++++++++++++++++  
+NS_GROUP_SET_HELP(en_us)
The GROUP SET command changes group settings.

@NS_GROUP_SET_SYNTAX

Only services root can change group settings.
+NS_GROUP_SET_HELP(nl)
Het GROUP SET commando veranderd groepsinstellingen

@NS_GROUP_SET_SYNTAX

Alleen services root kan groepsinstellingen veranderen.
++++++++++++++++++++++++++++++++++++++++++++
+NS_GROUP_SHOW_HELP(en_us)
The GROUP SHOW command lists the groups you belong to.
+NS_GROUP_SHOW_HELP(nl)
Het GROUP SHOW commando geeft een lijst van groepen waar u bij zit.
++++++++++++++++++++++++++++++++++++++++++++
+NICK_NOT_ROOT(en_us)
Only services root can change/create groups.
+NICK_NOT_ROOT(pt)
Apenas superadministradors dos serviços podem criar grupos.
+NICK_NOT_ROOT(nl)
Alleen services-root kan groepen maken.
+NICK_NOT_ROOT(pt_br)
Apenas Services Root podem criar/alterar grupos.
+NICK_NOT_ROOT(de)
Nur Services-Root kann Gruppen erstellen und/oder ändern.
++++++++++++++++++++++++++++++++++++++++++++
+NS_GROUP_ALREADY_EXISTS(en_us)
A group with that name already exists!
+NS_GROUP_ALREADY_EXISTS(pt)
Já existe um grupo com esse nome!
+NS_GROUP_ALREADY_EXISTS(nl)
Een groep met deze naam bestaat al!
+NS_GROUP_ALREADY_EXISTS(pt_br)
Já existe um grupo com esse nome!
+NS_GROUP_ALREADY_EXISTS(de)
Eine Gruppe mit dem Namen existiert bereits!
++++++++++++++++++++++++++++++++++++++++++++
+NS_GROUP_MASTER_NOT_FOUND(en_us)
The nick/group \B%s\B is not registered.
+NS_GROUP_MASTER_NOT_FOUND(pt)
O nick/grupo \B%s\B não está registado.
+NS_GROUP_MASTER_NOT_FOUND(nl)
De bijnaam/groep \B%s\B is niet geregistreerd.
+NS_GROUP_MASTER_NOT_FOUND(pt_br)
O nick/grupo \B%s\B não está registrado.
+NS_GROUP_MASTER_NOT_FOUND(de)
Der Nick und/oder die Gruppe \B%s\B ist nicht registriert.
++++++++++++++++++++++++++++++++++++++++++++
+NS_GROUP_CREATE_ERROR(en_us)
There was a fatal error creating the group. 
Please try again later.
+NS_GROUP_CREATE_ERROR(pt)
Ocorrou um erro durante a criação do grupo.
Por favor tente mais tarde.
+NS_GROUP_CREATE_ERROR(nl)
Er was een fatale fout tijdens het aanmaken van de groep.
Probeer het later nog eens.
+NS_GROUP_CREATE_ERROR(pt_br)
Ocorreu um erro durante a criação do grupo.
Por favor, tente novamente mais tarde.
+NS_GROUP_CREATE_ERROR(de)
Bei der Erstellung der Gruppe ist ein schwerer Fehler aufgetreten.
Bitte versuche es später nochmal.
++++++++++++++++++++++++++++++++++++++++++++
+NS_GROUP_CREATE_OK(en_us)
Group \B%s\B successfully created.
+NS_GROUP_CREATE_OK(pt)
Grupo \B%s\B foi criado com sucesso.
+NS_GROUP_CREATE_OK(nl)
Groep \B%s\B is succesvol aangemaakt.
+NS_GROUP_CREATE_OK(pt_br)
Grupo \B%s\B foi criado com sucesso.
+NS_GROUP_CREATE_OK(de)
Die Gruppe \B%s\B wure erfolgreich erstellt.
++++++++++++++++++++++++++++++++++++++++++++
+NO_SUCH_GROUP_X(en_us)
The group \B%s\B does not exist.
+NO_SUCH_GROUP_X(pt)
O grupo \B%s\B  não existe.
+NO_SUCH_GROUP_X(nl)
De groep \B%s\B bestaat niet.
+NO_SUCH_GROUP_X(pt_br)
O grupo \B%s\B  não existe.
+NO_SUCH_GROUP_X(de)
Die Gruppe \B%s\B existiert nicht.
++++++++++++++++++++++++++++++++++++++++++++
+NOT_MASTER_OF_X(en_us)
You are not a master of group \B%s\B.
+NOT_MASTER_OF_X(pt)
Não és \"master\" do grupo \B%s\B.
+NOT_MASTER_OF_X(nl)
U bent niet de master van groep \B%s\B.
+NOT_MASTER_OF_X(pt_br)
Você não é \"master\" do grupo \B%s\B.
+NOT_MASTER_OF_X(de)
Du bist nicht \"Master\" der Gruppe \B%s\B.
++++++++++++++++++++++++++++++++++++++++++++
+NICK_X_ALREADY_ON_X(en_us)
Nick \B%s\B is already member on \B%s\B.
+NICK_X_ALREADY_ON_X(pt)
O nick \B%s\B já está no grupo \B%s\B.
+NICK_X_ALREADY_ON_X(nl)
Bijnaam \B%s\B is al lid van groep \B%s\B.
+NICK_X_ALREADY_ON_X(pt_br)
O nick \B%s\B já é membro do grupo \B%s\B.
+NICK_X_ALREADY_ON_X(de)
Der Nick \B%s\B ist breits Mitglied in \B%s\B.
++++++++++++++++++++++++++++++++++++++++++++
+NICK_ADDED_X_X(en_us)
The nick \B%s\B was added to group \B%s\B.
+NICK_ADDED_X_X(pt)
O nick \B%s\B foi adicionado ao grupo \B%s\B.
+NICK_ADDED_X_X(nl)
De bijnaam \B%s\B is toegevoegd aan groep \B%s\B.
+NICK_ADDED_X_X(pt_br)
O nick \B%s\B foi adicionado ao grupo \B%s\B.
+NICK_ADDED_X_X(de)
Der Nick \B%s\B wurde der Gruppe \B%s\B hinzugefügt.
++++++++++++++++++++++++++++++++++++++++++++
+NO_SUCH_NICK_X(en_us)
The nick \B%s\B is not a registered nick.
+NO_SUCH_NICK_X(pt)
O nick \B%s\B nao está registado.
+NO_SUCH_NICK_X(nl)
De bijnaam \B%s\B is niet geregistreerd.
+NO_SUCH_NICK_X(pt_br)
O nick \B%s\B não está registrado.
+NO_SUCH_NICK_X(de)
Der Nick \B%s\B ist nicht registriert.
++++++++++++++++++++++++++++++++++++++++++++
+NICK_X_NOT_ON_GROUP_X(en_us)
The nick \B%s\B is not a member of group \B%s\B.
+NICK_X_NOT_ON_GROUP_X(nl)
De bijnaam \B%s\B is geen member van de groep \B%s\B.
+NICK_X_NOT_ON_GROUP_X(pt_br)
O nick \B%s\B não é membro do grupo \B%s\B.
+NICK_X_NOT_ON_GROUP_X(de)
Der Nick \B%s\B ist nicht Mitglied der Gruppe \B%s\B.
++++++++++++++++++++++++++++++++++++++++++++
+NICK_DEL_X_X(en_us)
The nick \B%s\B was removed from group \B%s\B.
+NICK_DEL_X_X(nl)
De bijnaam \B%s\B is verwijderd van groep \B%s\B.
+NICK_DEL_X_X(pt_br)
O nick \B%s\B foi removido do grupo \B%s\B.
+NICK_DEL_X_X(de)
Der Nick \B%s\B wurde aus der Gruppe \B%s\B entfernt.
++++++++++++++++++++++++++++++++++++++++++++
+NS_GROUP_INFO_X(en_us)
***** Group info for \B%s\B *****
+NS_GROUP_INFO_X(nl)
***** Groep informatie voor \B%s\B *****
+NS_GROUP_INFO_X(pt_br)
***** Informações do grupo para \B%s\B *****
+NS_GROUP_INFO_X(de)
***** Gruppeninfo für \B%s\B *****
++++++++++++++++++++++++++++++++++++++++++++
+NS_GROUP_INFO_MASTER_X(en_us)
Master.......: %s
+NS_GROUP_INFO_MASTER_X(nl)
Master.......: %s
+NS_GROUP_INFO_MASTER_X(pt_br)
Master.......: %s
+NS_GROUP_INFO_MASTER_X(de)
Master.......: %s
++++++++++++++++++++++++++++++++++++++++++++
+NS_GROUP_INFO_DESC_X(en_us)
Description..: %s
+NS_GROUP_INFO_DESC_X(nl)
Beschrijving.: %s
+NS_GROUP_INFO_DESC_X(pt_br)
Descrição....: %s
+NS_GROUP_INFO_DESC_X(de)
Beschreibung.: %s
++++++++++++++++++++++++++++++++++++++++++++
+NS_GROUP_MEMBERS(en_us)
Group members:
+NS_GROUP_MEMBERS(nl)
Groep members:
+NS_GROUP_MEMBERS(pt_br)
Membros grupo:
+NS_GROUP_MEMBERS(de)
Mitglieder...:
++++++++++++++++++++++++++++++++++++++++++++
+NS_GROUP_MEMBER_X(en_us)
    %s
+NS_GROUP_MEMBER_X(nl)
    %s
+NS_GROUP_MEMBER_X(pt_br)
    %s
+NS_GROUP_MEMBER_X(de)
    %s
++++++++++++++++++++++++++++++++++++++++++++    
+NS_GROUP_EMPTY(en_us)
This group is empty.
**************
+NS_GROUP_EMPTY(nl)
Deze groep is leeg.
**************
+NS_GROUP_EMPTY(pt_br)
Este grupo está vazio.
**************
+NS_GROUP_EMPTY(de)
Diese Gruppe ist leer.
**************
++++++++++++++++++++++++++++++++++++++++++++
+NS_GROUP_MEMBERS_TAIL_X(en_us)
************** \B%d\B member(s)
+NS_GROUP_MEMBERS_TAIL_X(nl)
************** \B%d\B member(s)
+NS_GROUP_MEMBERS_TAIL_X(pt_br)
************** \B%d\B membro(s)
+NS_GROUP_MEMBERS_TAIL_X(de)
************** \B%d\B Mitglied(er)
++++++++++++++++++++++++++++++++++++++++++++
+NS_GROUP_INFO_TAIL(en_us)
************* End Of Group Info
+NS_GROUP_INFO_TAIL(nl)
************* Einde van groepsinfo
+NS_GROUP_INFO_TAIL(pt_br)
************* Fim das informações do grupo
+NS_GROUP_INFO_TAIL(de)
************* Gruppeninfoende
++++++++++++++++++++++++++++++++++++++++++++
+NOT_MASTER_OR_MEMBER_X(en_us)
You need to be master or member to see \B%s\B info.
+NOT_MASTER_OR_MEMBER_X(nl)
U moet master of member zijn om de info van groep ^B%s^B te kunnen zien
+NOT_MASTER_OR_MEMBER_X(pt_br)
Você precisa ser master ou membro para ver as informações de \B%s\B.
+NOT_MASTER_OR_MEMBER_X(de)
Du musst Master oder Mitglied von \B%s\B sein, um die Gruppeninfo zu einzusehen.
++++++++++++++++++++++++++++++++++++++++++++
+NS_GROUP_LIST_HEADER(en_us)
********* List of groups *********
+NS_GROUP_LIST_HEADER(nl)
******** Lijst van groepen ********
+NS_GROUP_LIST_HEADER(pt_br)
********* Lista de grupos *********
+NS_GROUP_LIST_HEADER(de)
********** Gruppenliste **********
++++++++++++++++++++++++++++++++++++++++++++
+NS_GROUP_LIST_X_X_X(en_us)
  %-20s %-20s %4d %s
+NS_GROUP_LIST_X_X_X(nl)
 %-20s %-20s %4d %s
+NS_GROUP_LIST_X_X_X(pt_br)
  %-20s %-20s %4d %s
+NS_GROUP_LIST_X_X_X(de)
  %-20s %-20s %4d %s
++++++++++++++++++++++++++++++++++++++++++++
+NS_GROUP_LIST_TAIL(en_us)
*********** End of list **********
+NS_GROUP_LIST_TAIL(nl)
******** Einde van lijst ********
+NS_GROUP_LIST_TAIL(pt_br)
*********** Fim da lista **********
+NS_GROUP_LIST_TAIL(de)
*********** Listenende **********
++++++++++++++++++++++++++++++++++++++++++++
+NS_GROUP_DROPPED_X(en_us)
Group \B%s\B has been dropped.
+NS_GROUP_DROPPED_X(nl)
De groep \B%s\B is ongedaan gemaakt.
+NS_GROUP_DROPPED_X(pt_br)
Grupo \B%s\B foi cancelado.
+NS_GROUP_DROPPED_X(de)
Gruppe \B%s\B ist gelöscht.
++++++++++++++++++++++++++++++++++++++++++++
+NS_GROUP_ITEM_X(en_us)
%s
+NS_GROUP_ITEM_X(nl)
%s
+NS_GROUP_ITEM_X(pt_br)
%s
+NS_GROUP_ITEM_X(de)
%s
++++++++++++++++++++++++++++++++++++++++++++
+NS_GROUP_ITEM_X_X(en_us)
%s (expires on %s)
+NS_GROUP_ITEM_X_X(pt_br)
%s (expira em %s)
+NS_GROUP_ITEM_X_X(nl)
%s (verloopt op %s)
++++++++++++++++++++++++++++++++++++++++++++
+NS_GROUP_LIST_X_X_X(en_us)
%s [%s] %s
+NS_GROUP_LIST_X_X_X(nl)
%s [%s] %s
+NS_GROUP_LIST_X_X_X(pt_br)
%s [%s] %s
+NS_GROUP_LIST_X_X_X(de)
%s [%s] %s
++++++++++++++++++++++++++++++++++++++++++++
+CANT_DROP_ROOT(en_us)
You can't drop the Root group !
+CANT_DROP_ROOT(nl)
U kan de registratie van de Root groep niet
ongedaan maken !
+CANT_DROP_ROOT(pt_br)
Você não pode cancelar o grupo de Root!
+CANT_DROP_ROOT(de)
Die Root-Gruppe kann nicht gelöscht werden.
++++++++++++++++++++++++++++++++++++++++++++
+NO_GROUPS(en_us)
You do not belong to any group.
+NO_GROUPS(pt)
Não pertence a nenhum grupo.
+NO_GROUPS(nl)
U behoort niet tot een groep.
+NO_GROUPS(de)
Du gehörst keiner Gruppe an.
++++++++++++++++++++++++++++++++++++++++++++
+NS_GROUP_SHOW_HEADER(en_us)
*** Your groups:
+NS_GROUP_SHOW_HEADER(pt)
*** Os seus grupos:
+NS_GROUP_SHOW_HEADER(nl)
*** Uw groep(en):
+NS_GROUP_SHOW_HEADER(de)
*** Deine Gruppe(n):
++++++++++++++++++++++++++++++++++++++++++++
+NS_GROUP_SHOW_TAIL(en_us)
*** End of list
+NS_GROUP_SHOW_TAIL(pt)
*** Fim da lista
+NS_GROUP_SHOW_TAIL(nl)
** Einde van de lijst
+NS_GROUP_SHOW_TAIL(de)
*** Listenende
++++++++++++++++++++++++++++++++++++++++++++
+NS_GROUP_SHOW_X_X(en_us)
  %s : %s
+NS_GROUP_SHOW_X_X(pt)
  %s : %s
+NS_GROUP_SHOW_X_X(nl)
  %s : %s
+NS_GROUP_SHOW_X_X(de)
  %s : %s
++++++++++++++++++++++++++++++++++++++++++++  
+NS_GROUP_SHOW_X_X_X(en_us)
  %s : %s  (Expires on %s)
+NS_GROUP_SHOW_X_X_X(pt)
  %s : %s  (Expira em %s)
+NS_GROUP_SHOW_X_X_X(nl)
  %s : %s (verloopt op %s)
++++++++++++++++++++++++++++++++++++++++++++
+AUTOMODES_X_UNSET(en_us)
Automodes for group \B%s\B were removed.
+AUTOMODES_X_UNSET(pt)
Os modos automáticos para o grupo \B%s\B foram removidos.
+AUTOMODES_X_UNSET(nl)
Automodes voor groep \B%s\B zijn verwijderd.
+AUTOMODES_X_UNSET(de)
Automodi der Gruppe \B%s\B wurden entfernt.
++++++++++++++++++++++++++++++++++++++++++++
+AUTOMODES_X_CHANGED_TO_X(en_us)
Automodes for group \B%s\B where changed to \b%s\B .
+AUTOMODES_X_CHANGED_TO_X(pt)
Os modos automáticos do grupo \B%s\B foram alterados para \b%s\B .
+AUTOMODES_X_CHANGED_TO_X(nl)
Automodes voor groep \B%s\B vijn veranderd naar \B%s\B.
+AUTOMODES_X_CHANGED_TO_X(de)
Automodi für die Gruppe \B%s\B geändert in \b%s\B .
++++++++++++++++++++++++++++++++++++++++++++
+DESC_X_UNSET(en_us)
Description for group \B%s\B was removed.
+DESC_X_UNSET(pt)
A descrição do grupo \B%s\B foi removida.
+DESC_X_UNSET(nl)
Omschrijving voor groep \B%s\B is verwijderd.
+DESC_X_UNSET(de)
Die Beschreibung für die Gruppe \B%s\B wurde gelöscht.
++++++++++++++++++++++++++++++++++++++++++++
+DESC_X_CHANGED_TO_X(en_us)
Description for group \B%s\B was changed to:
%s
+DESC_X_CHANGED_TO_X(pt)
A descrição do grupo \B%s\B foi alterada para:
%s
+DESC_X_CHANGED_TO_X(nl)
Beschrijving voor groep \B%s\B is veranderd naar:
%s
+DESC_X_CHANGED_TO_X(de)
Die Beschreibung für die Gruppe \B%s\B wurde geändert in:
%s
++++++++++++++++++++++++++++++++++++++++++++
+SET_INVALID_OPTION_X(en_us)
Option \B%s\B does not exist !
Existing options are:
  DESC        Group description
  AUTOMODES   User modes to be set on identify.
+SET_INVALID_OPTION_X(pt)
A opção \B%s\B não existe !
As opções existentes são:
  DESC        Descrição do grupo
  AUTOMODES   Modos a serem colocandos no utilizador após identificação.
+SET_INVALID_OPTION_X(nl)
Optie \B%s\B bestaat niet !
Geldige opties zijn:
  DESC        Groep omschrijving
  AUTOMODES   User modes die worden gezet bij de identificatie.
+SET_INVALID_OPTION_X(de)
Die Option \B%s\B existiert nicht!
Folgende Optionen gibt es:
  DESC        Beschreibung der Gruppe
  AUTOMODES   Automodi für die Mitglieder bei Identifizierung.
++++++++++++++++++++++++++++++++++++++++++++  
+NS_GROUP_IS_FULL_X(en_us)
You can't add more members, group is full !
+NS_GROUP_IS_FULL_X(pt)
Não pode adicionar mais membros, o grupo está cheio !
+NS_GROUP_IS_FULL_X(nl)
U kunt geen nieuwe groep members toevoegen, groep is vol !
++++++++++++++++++++++++++++++++++++++++++++
+NS_GROUP_SET_MAXUSERS_SET_X_X(en_us)
The max number of users allowed on group \B%s\B was changed to \B%d\B .
+NS_GROUP_SET_MAXUSERS_SET_X_X(pt)
O numero máximo de utilizadores no gupo \B%s\B foir alterado para \B%d\B .
+NS_GROUP_SET_MAXUSERS_SET_X_X(nl)
Het maximale aantal gebruikers van groep \B%s\B us veranderd naar \B%d\B .
++++++++++++++++++++++++++++++++++++++++++++
+NS_GROUP_CANT_DEFINE_TIME_X(en_us)
You are not a permanent master of \B%s\B.
You can't use the time option !
+NS_GROUP_CANT_DEFINE_TIME_X(pt)
Não é um master permanente do grupo \B%s\B.
Não pode utilizar a opção de duração !
+NS_GROUP_CANT_DEFINE_TIME_X(nl)
U bent geen permanente master van \B%s\B.
U kunt de tijd optie niet gebruiken !
++++++++++++++++++++++++++++++++++++++++++++
+NS_GROUP_X_EXPIRING_X(en_us)
Your membership on group \B%s\B will expire in \B%d\B day(s) !
+NS_GROUP_X_EXPIRING_X(pt)
A sua participação no grupo \B%s\B expira dentro de \B%d\B dia(s) !
+NS_GROUP_X_EXPIRING_X(nl)
Uw lidmaatschap van groep \B%s\B verloopt over \B%d\B dag(en) !
