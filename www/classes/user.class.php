<?php
class User {
	/* local variables */
	var $snid;
	var $nick;
	var $email;
	
	/* constructor */
	function User($snid, $nick, $email) {
		$this->snid = $snid;
		$this->nick = $nick;
		$this->email = $email;
	}
}
?>