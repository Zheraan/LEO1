<?php
	$time = microtime(true);
	$time_dt = date("Y-m-d H:i:s", round($time));

	$log_str = sprintf("%.3f\t%s\t%s", $time, $time_dt, $_SERVER['REMOTE_ADDR']);

	if (isset($_POST["log"]))
	{
		$log = $_POST["log"];
		$log_str = $log_str . sprintf("\t%s\n", $log); 
		echo ("POST_OK\n");
	}

	$file_name = 'log.txt';
	$myfile = fopen($file_name, 'a') or die('Cannot open file: '.$file_name); 
	fwrite($myfile, $log_str);
	fclose($myfile);		
?>
