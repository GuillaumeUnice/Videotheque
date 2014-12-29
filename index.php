<?php
	session_start();
	error_reporting(0);
	require ('./global/GlobalVariables.php');
	require ('./global/GlobalFunctions.php');

	function __autoload($name) {
		if (@constant("PATHTOROOT")) $path = constant("PATHTOROOT");
		else
		{
			echo "VOUS N AVEZ PAS DEFINI PATHTOROOT :(par exemple define(\"PATHTOROOT\", \"../\"); ";
			die();
		}
		if (strrpos($path, "/") != (strlen($path)-1) ) $path .="/";
		
		if(strstr($name, CONTROLLER_SUFFIXE)) {//User_Controller
			require ($path."Controllers/".$name.".php");
		} else if(strstr($name, VIEW_SUFFIXE)){//Login_User_View
				$view = explode('_', $name);
				
				require ($path."Views/".$view[1]."/".$name.".php");//./views/User/Login_User_View.php
		} else {
			//require ("./Models/".$name.".php");//User
			
			$rep = substr($name, 0, 5);
			// **** CAS Data_ ou Libs_
			if (($rep == "Data_") || ($rep == "Libs_"))
			{
				$rep = substr($rep, 0, 4);
	
				// include constant("RACINE")."$rep/$class_name.inc";
				require ($path."Models/".$rep."/".$name.".inc");
			}
			else
			{
				$rep = substr($name, 0, 3);
				// **** CAS Nf_
				if ($rep =="Nf_")
				{
					$rep = substr($rep, 0, 2);
					require ($path."Models/".$rep."/".$name.".inc");
				}
			}
		}
	}
	
	if(!empty($_GET['controller'])) {
		if(_existController($_GET['controller'])) {
			$controllerClass = new ReflectionClass($_GET['controller'].CONTROLLER_SUFFIXE);
		} else {
			_redirect(INDEX_PAGE);
		}
		if($controllerClass->isInstantiable()) {
			$controllerObj = $controllerClass->newInstance();
			
			if(!empty($_GET['action'])) {
					$action = $_GET['action'];
					if(count($_GET) > 2) {
						$params = array();
						foreach($_GET as $getKey => $getValue) {
							if($getKey != "controller" && $getKey != "action") {
								$params[$getKey] = $getValue;
							}
						}
					} else {
						$params = null;
					}
					$action = new ReflectionMethod($_GET['controller'].CONTROLLER_SUFFIXE, $action);
					$action->invoke($controllerObj, $params);
			} else {
					$action = new ReflectionMethod($_GET['controller'].CONTROLLER_SUFFIXE, 'index');
					$action->invoke($controllerObj, null);
			}
		} else {
			_redirect(INDEX_PAGE);//si c'est pas instanciable (ou redirection vers page d'erreur)
		}
	} else {
		_redirect(INDEX_PAGE);//si aucun controller n'est defini
	}
	
	session_write_close();
?>
