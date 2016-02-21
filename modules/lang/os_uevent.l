++++ $Id: sessionlimit.l,v 1.5 2005/11/03 20:24:05 waxweazle Exp $
++++ operserv.l
++++ en_us: Lamego, Stinger
++++ nl: Stinger, Ldp
++++ pt: Lamego
++++ pt_br: Ricardo

+OS_UEVENT_SUMMARY(en_us)
User events managment
+OS_UEVENT_SUMMARY(nl)
User event beheer
+++++++++++++++++++++++++++++++++++++++++++++
+OS_UEVENT_SYNTAX(en_us)
Syntax:
  // UEVENT ADD [+time] event_type(parameter) action_type action_params
+OS_UEVENT_SYNTAX(nl)
Syntax:
  // UEVENT ADD [+tijd] event_type(parameter) actie_type actie_parameters
+++++++++++++++++++++++++++++++++++++++++++++  
+OS_UEVENT_HELP(en_us)
The OS UEVENT command allows to program actions to be executed when
specific user actions are executed.

To add an event:
// UEVENT ADD [+time] ON_event(parameter) action action_param

To list events
// UEVENT LIST

To delete an event:
// UEVENT DEL id

To list the available events type:
  // HELP UEVENT EVENTS
To list the available actions type:
  // HELP UEVENT ACTIONS
+OS_UEVENT_HELP(nl)
Het OS UEVENT commando stelt u in staat een actie te programeren die worden
uitgevoerd als een specifieke gebruiker verbinding maakt

Toevoegen van een event
  // UEVENT ADD [+tijd] ON_event(parameter) actie actie_parameter

Voor een lijst van events
  // UEVENT LIST

Verwijderen van een events
  // UEVENT DEL id

Voor een lijst van aanwezige events:
  // HELP UEVENT EVENTS

Voor een lijst van aanwezige acties:
  // HELP UEVENT ACTIONS
+++++++++++++++++++++++++++++++++++++++++++++
+OS_UEVENT_EVENT_LIST(en_us)
Availale event types:
  ON_CONNECT(mask)  - User connects matching mask
  ON_JOIN(chan)     - User joins a channel (masks are allowed)
  ON_LOGIN(mask)    - User logins/identifies matching mask
  ON_REGISTER(mask) - User registers nick matching mask
+OS_UEVENT_EVENT_LIST(nl)
Beschikbare event types:
  ON_CONNECT(masker)  - Gebruiker maakt verbinding met overeenkomend masker
  ON_JOIN(kanaal)     - Gebruiker gaat kanaal binnen (maskers toegestaan)
  ON_LOGIN(masker)    - Gebruiker login/identify met overeenkomend masker
  ON_REGISTER(masker) - Gebruiker registreerd bijnaam met overeenkomend masker
+++++++++++++++++++++++++++++++++++++++++++++
+OS_UEVENT_ACTION_LIST(en_us)
Available actions:
  MESSAGE message   - Send private message to user
  NOTICE messsage   - Send notice to user
  GLINE reason      - Gline user's host for 1 day, with reason
  UMODE modes       - Set umodes on user
  JOIN chan         - Make the user join channel
  KILL reason       - Kill the user with a reason
+OS_UEVENT_ACTION_LIST(nl)
beschikbare actie types:
  MESSAGE bericht   - Stuur prive bericht naar gebruiker
  NOTICE bericht    - Stuur prive bericht naar gebruiker
  GLINE             - GLINE gebruikershost voor 1 dag, met reden
  UMODE modes       - Zet umodes op een gebruiker
  JOIN kanaal       - laat een gebruiker een kanaal ingaan
  KILL reden        - Kill de gebruiker met een reden
+++++++++++++++++++++++++++++++++++++++++++++
+OS_UEVENT_ADD_SYNTAX(en_us)
// UEVENT ADD [+time] ON_event(parameter) action action_param

@OS_UEVENT_EVENT_LIST

@OS_UEVENT_ACTION_LIST
+OS_UEVENT_ADD_SYNTAX(nl)
// UEVENT ADD [+tijd] ON_event(parameter) actie actie_parameter
@OS_UEVENT_EVENT_LIST

@OS_UEVENT_ACTION_LIST
+++++++++++++++++++++++++++++++++++++++++++++
+OS_UEVENT_ADD_INVALID_SYNTAX(en_us)
Invalid syntax!
@OS_UEVENT_ADD_SYNTAX
+OS_UEVENT_ADD_INVALID_SYNTAX(nl)
Ongeldige syntax!
@OS_UEVENT_ADD_SYNTAX
+++++++++++++++++++++++++++++++++++++++++++++
+OS_UEVENT_INVALID_EVENT_X(en_us)
Invalid user event \B%s\B .
@OS_UEVENT_EVENT_LIST
+OS_UEVENT_INVALID_EVENT_X(nl)
Invalid user event \B%s\B .
@OS_UEVENT_EVENT_LIST
+++++++++++++++++++++++++++++++++++++++++++++
+OS_UEVENT_INVALID_ACTION_X(en_us)
Invalid action \B%s\B .
@OS_UEVENT_ACTION_LIST
+OS_UEVENT_INVALID_ACTION_X(nl)
Ongeldige actie \B%s\B .
@OS_UEVENT_ACTION_LIST
+++++++++++++++++++++++++++++++++++++++++++++
+OS_UEVENT_ADD_OK(en_us)
Event was successfully added.
+OS_UEVENT_ADD_OK(nl)
Event is succesvol toegevoegd.
+++++++++++++++++++++++++++++++++++++++++++++
+OS_UEVENT_LIST_HEADER(en_us)
*** List of user events ***
+OS_UEVENT_LIST_HEADER(nl)
*** lijst van user events ***
+++++++++++++++++++++++++++++++++++++++++++++
+OS_UEVENT_LIST_X_X_X_X(en_us)
%i %s(%s) %s %s
+OS_UEVENT_LIST_X_X_X_X(nl)
%i %s(%s) %s %s
+++++++++++++++++++++++++++++++++++++++++++++
+OS_UEVENT_LIST_TAIL(en_us)
*** End of List ***
+OS_UEVENT_LIST_TAIL(nl)
*** Einde van de lijst ***
+++++++++++++++++++++++++++++++++++++++++++++
+OS_UEVENT_DEL_SYNTAX(en_us)
// UEVENT DEL id
+OS_UEVENT_DEL_SYNTAX(nl)
// UEVENT DEL id
+++++++++++++++++++++++++++++++++++++++++++++
+OS_UEVENT_DEL_NOT_FOUND_X(en_us)
There is no user event with id \B%d\B !
+OS_UEVENT_DEL_NOT_FOUND_X(nl)
Er bestaat geen user event met \B%d\B als id !
+++++++++++++++++++++++++++++++++++++++++++++
+OS_UEVENT_DEL_DELETED_X(en_us)
User event \B%d\B was deleted.
+OS_UEVENT_DEL_DELETED_X(nl)
User event \B%d\B is verwijderd.
+++++++++++++++++++++++++++++++++++++++++++++
