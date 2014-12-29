<?php
	class Accueil_Controller {
			public function index() {
				$view = new Index_Accueil_View();
				$view->display();
			}
	}
?>