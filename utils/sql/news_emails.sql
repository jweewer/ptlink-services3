SELECT 
	nick, email
FROM 
	nickserv 
WHERE 
	(flags & 0x00000020) <> 0	# Just authenticated
	AND (flags & 0x00000002) = 0	# Skip forbidden
	AND (flags & 0x00000008) = 0  	# Skip nonews flag
	AND NOT IsNull(email)		# Skip null emails
GROUP BY email
