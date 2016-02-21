function checkRegister(msg) 
{
  var ret = true
  var doc = document.login

  if (doc.nick.value == "")
  {
    ret = false
  }
  else if (doc.password.value == "")
  {
    ret = false
  }
  else if (doc.passwd.value != doc.repasswd.value)
  {
    ret = false
  }
  if (!ret)     
    alert(msg)
  return ret
}
