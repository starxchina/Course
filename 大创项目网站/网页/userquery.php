
<?php 
header("Content-type:text/html; charset=gb2312");
session_start();	
?>
<HTML>
	<!-- 用户查询界面 -->
	<HEAD>

		<TITLE>教务查询页面</TITLE>
	</HEAD>

	<BODY>

		用户名:
		<?php 
		include "conn.php";  
		$snoandsname_sno = @$_SESSION['querysno'];
		$select_snoandsname = "select * from {$db_name}.snoandsname where sno_b= '{$snoandsname_sno}' ";
		$check_query = mysqli_query($link,$select_snoandsname) or die(mysqli_error($link));
		$row = mysqli_fetch_assoc($check_query);
		echo $snoandsname_sno;
		echo " ";
		echo $row['sname'] = iconv('UTF-8', 'GBK', $row['sname'])."<br/>";
		echo "您的成绩为:".$row['sgrade'];
		?>
		<br>
		欢迎您的登录!<br/>
		<br><br><br><br><br><br>
		<div id="div1">
		<div id="id1" style="width:200px;height:20px;float:left">
		<?php
		$sql="select * from snoandsname";
		$result=mysqli_query($link,$sql) or die (mysqli_error());
		echo "记录有：(数据均为随机)";
		echo "<p>";
		echo"<table border=1>";
		echo"<tr><td>sno</td><td>sname</td><td>sgrade</td></tr>";
		while($row=mysqli_fetch_array($result)){
			echo"<tr>";
			echo"<td>".$row['sno_b']."</td>";
			echo"<td>".$row['sname'] = iconv('UTF-8', 'GBK', $row['sname'])."</td>";
			echo"<td>".$row['sgrade']."</td>";
			echo"</tr>";

		}
		echo"</table>";
		?>
		</div>

		<div id="id2" style="width:200px;height:20px;margin-left:300px">
		<?php
		$sql="select * from datapublishing";
		$result=mysqli_query($link,$sql) or die (mysqli_error());
		echo "统计:";
		echo "<p>";
		echo"<table border=1>";
		echo"<tr><td>value</td><td>count</td></tr>";
		while($row=mysqli_fetch_array($result)){
			echo"<tr>";
			echo"<td>".$row['value']."</td>";
			echo"<td>".$row['count']."</td>";
			echo"</tr>";

		}
		echo"</table>";
		?>
		</div>

		<div id="id3" style="width:200px;height:20px;margin-left:500px ;">
		<?php

		function Sign($value){
			if($value > 0) return 1;
			else if($value == 0) return 0;
			else return -1;
		}

		function noisyCount($sensitivity,$epsilon){
			$RAND_MIN=0.0;
			$RAND_MAX=10.0;
			$d = rand($RAND_MIN,$RAND_MAX);
			if($d == $RAND_MIN) $d++;
			else if($d == $RAND_MAX) $d--;
			$uniform = $d/$RAND_MAX - 0.5;
			$s = $sensitivity/$epsilon;
			return $s*Sign($uniform)*log(1.0-2.0*abs($uniform));
		}

		$sql="select * from datapublishing";
		$result=mysqli_query($link,$sql) or die (mysqli_error());
		echo "统计:(差分隐私)";
		echo "<p>";
		echo"<table border=1>";
		echo"<tr><td>value</td><td>count</td></tr>";
		while($row=mysqli_fetch_array($result)){
			echo"<tr>";
			echo"<td>".$row['value']."</td>";
			$a = $row['count']+noisyCount(1.0,1.0);
			echo"<td>".round($a,2)."</td>";
			echo"</tr>";

		}
		echo"</table>";
		mysqli_close($link);
		?>
		</div>

		</div>
	</BODY>
</HTML>