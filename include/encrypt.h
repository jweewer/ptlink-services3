/* Include file for high-level encryption routines.
 *
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999 
 *                http://software.pt-link.net       
 * This program is distributed under GNU Public License
 * Please read the file COPYING for copyright information.
 *
 * These services are based on Andy Church Services 
 */

extern void* encrypted_password(char *passwd);
extern int check_password(const char *plaintext, const char *password, int encrypt_method);
void encrypt_md5(const char *src, int len, char *dest);
