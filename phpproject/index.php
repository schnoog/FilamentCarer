<?php

require_once "phpMQTT/phpMQTT.php";

$server = "192.168.178.27";     // change if necessary
$port = 1883;                     // change if necessary
$username = "";                   // set your username
$password = "";                   // set your password
$client_id = "phpMQTT-subscriber"; // make sure this is unique for connecting to sever - you could use uniqid()

$COMMANDTOPIC="3d/CMD";

$mqtt = new phpMQTT($server, $port, $client_id);

$rfid="";
$balance=0;
$workvalue="";
$DoneItsWork=false;



GetWeigth();
function GetWeigth(){
	global $workvalue;
		$workvalue = "";
		WaitForAnswer("getid","3d/filament/rfid");
		echo "RFID: " . $workvalue . "\n\n" ;
		$rfid = $workvalue;
		$workvalue = "";
		WaitForAnswer("balance","3d/filament/weight");
		echo "Gewichts: "  . $workvalue . "\n\n";
		$balance = $workvalue;
		
		$factor = 344;
		$tara = 214594;
		$wd = $balance - $tara;
		$weight = round($wd / $factor,1);
		
		
		echo "Aktuelles Gewicht: $weight g \n\n";
		
	
}


//------------------------------------------------------
//------------------------------------------------------
//------------------------------------------------------

function WaitForAnswer($command, $waittopic){
	global $mqtt,$username,$password, $DoneItsWork,$COMMANDTOPIC;
	$cnt=0;
	$DoneItsWork = false;
	if(!$mqtt->connect(true, NULL, $username, $password)) {
		exit(1);
	}
	echo "Subscribe to " . $waittopic . "\n";
	$topics[$waittopic] = array("qos" => 0, "function" => "GetRetValue");
	$mqtt->subscribe($topics, 0);


	while($mqtt->proc()){
		if($cnt == 0){
			
			$mqtt->publish($COMMANDTOPIC, $command, 0);
			echo "Publish Command " . $command . "\n";			
		}
		$cnt++;
		if ($DoneItsWork) break;
		if($cnt == 100){
			$mqtt->publish($COMMANDTOPIC, $command, 0);
			echo "Re-Publish Command " . $command . "\n";				
		}
		if($cnt == 200) break;
	}
	$mqtt->close();
}
//------------------------------------------------------
//------------------------------------------------------
//------------------------------------------------------
function GetRetValue($topic,$msg){
		global $workvalue,$DoneItsWork;
		$DoneItsWork = true;
		$workvalue = $msg;
}
//------------------------------------------------------
//------------------------------------------------------
//------------------------------------------------------
function procmsg($topic, $msg){
	global $workvalue,$DoneItsWork;
	$DoneItsWork = true;
		echo "Msg Recieved: " . date("r") . "\n";
		echo "Topic: {$topic}\n\n";
		echo "\t$msg\n\n";
}
//------------------------------------------------------
//------------------------------------------------------
//------------------------------------------------------

function publishCommand($value){
	global $mqtt,$username,$password,$COMMANDTOPIC;
	if ($mqtt->connect(true, NULL, $username, $password)) {
		$mqtt->publish($COMMANDTOPIC, $value, 0);
		$mqtt->close();
	} else {
		echo "Time out!\n";
	}
}
//------------------------------------------------------
//------------------------------------------------------
//------------------------------------------------------