SELECT 
	nick, email
FROM 
	nickserv 
WHERE 
        (flags & 0x00000002) = 0    # Skip forbidden
	AND (flags & 0x00000004) = 0  	# Skip noxpire flags
        AND (flags & 0x00000020) <> 0       # Just authenticated
        AND NOT IsNull(email)           # Skip null emails
	AND FLOOR((t_expire - UNIX_TIMESTAMP(NOW()))/86400) = 5
GROUP BY email
