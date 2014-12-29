<?php

function _existController($controller) {
	return file_exists("./Controllers/".$controller.CONTROLLER_SUFFIXE.".php");
}

function _redirect($page) {
	echo '<meta http-equiv="refresh" content="0;'.$page.'">';
	session_write_close();
	exit;
}

?>