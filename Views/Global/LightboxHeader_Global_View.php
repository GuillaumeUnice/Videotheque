<?php

	class LightboxHeader_Global_View {
		
		private $css;
		private $js;
		
		public function LightboxHeader_Global_View($css, $js)
		{
			$this->css = $css;
			$this->js = $js;
		}
		
		public function getHeader() {
			ob_start();
?>
<!doctype html>
<html lang="fr">
	<head>
		<title>POO-IHM 2013-2014 Lightbox</title>
		<meta charset="utf-8">
		
		<?php
			// Traitement des feuilles de styles à intégrer
			foreach($this->css as $cssFileName) {
		?>
				<link rel="stylesheet" type="text/css" href="./public/css/<?php echo $cssFileName; ?>">
		<?php
			}
		?>
		
		<?php
			// Traitement des scripts js à intégrer
			foreach($this->js as $jsFileName) {
		?>
				<script type="text/javascript" src="./public/js/<?php echo $jsFileName; ?>"></script>
		<?php
			}
		?>

	</head>
<body>

	<div id="lightbox_wrapper">
		<section id="lightbox_main">
<?php			
			$head = ob_get_contents();
			ob_end_clean();
			
			return $head;
		}
		
	}
	
	
?>