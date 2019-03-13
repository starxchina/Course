<?php
//连接数据库验证登录名与密码
header("Content-type:text/html; charset=utf-8");


$login=@$_POST['submit'];//post的是name
$yanzhengma_write = @$_POST['inputRandom'];
$yanzhengma_right = @$_POST['hidetext'];


//mysqli_query("set names utf-8");


	if($yanzhengma_write == $yanzhengma_right){
	if ($login=="login"  ){
		$username = $_POST['user'];  
		$password = $_POST['password'];

		if ($username!="" and $password!=""){
		include "conn.php";	//包含数据库连接文件 
		//检测用户名及密码是否正确
			$select_snoandpassword = "select * from {$db_name}.snoandpassword where sno= '{$username}' ";
$check_query = mysqli_query($link,$select_snoandpassword);  //mysqli_query（数据库服务器连接对象，SQL语句）

$result = mysqli_fetch_array($check_query);


if($result['password'] == $password){
	//登录成功
	session_start();
	$_SESSION['querysno'] = $username; 
	//header("Location:userquery.php");
	echo "<meta http-equiv=refresh content='0; url=userquery.php'>";
	mysqli_free_result($check_query);
	mysqli_close($link);}

else { 
	echo "<script> 
	alert('Incorrect password');
	window.location.href='index.html';
	</script>";
	mysqli_free_result($check_query);		
	mysqli_close($link);}

} 
}
else{echo "<script>  window.location.href='index.html'; </script>";}
}
else{	echo "<script> 
	alert('验证码错误');
	window.location.href='index.html';
	</script>";}
?>
