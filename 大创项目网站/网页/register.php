<?php

$login=@$_POST['tijiao'];
if($login=="提交"){
$newpassword1 = @$_POST['password1'];
$checkpassword1 = @$_POST['password2'];	
if($newpassword1 == $checkpassword1){
	$newsno = @$_POST['user'];	
	include "conn.php";
	$querysno = "select *from {$db_name}.snoandpassword where sno = '{$newsno}'";
	$check_query = $link ->query($querysno);
	if(mysqli_num_rows($check_query)!=0){echo"===您输入的用户名已经存在===";}
	else{
		$insert = "insert into {$db_name}.snoandpassword(sno,password) value('$newsno','$newpassword1')";
		if($link->query($insert)){echo"===注册成功！===";}
	}


}
else{echo"<script> alert('两次密码不相同'); window.location.href='register.html'; </script>";}
}
else{echo"<script> alert('按钮错误'); window.location.href='register.html'; </script>";}

?>