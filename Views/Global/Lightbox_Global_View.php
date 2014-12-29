<?php

	abstract class Lightbox_Global_View {
		
		/**
		* Tableau contenant les fichiers CSS à inclure.
		*
		* @var array
		*/
		protected $css = array();
		
		/**
		* Tableau contenant les fichiers JS à inclure.
		*
		* @var array
		*/
		protected $js = array();
		
		/**
		* Met à jour le tableau $css.
		*
		* @param array $css
		* @return void
		*/
		protected function setCSS($css) {
			$this->css = $css;
 		}
		
		/**
		* Met à jour le tableau $js.
		*
		* @param array $js
		* @return void
		*/
		protected function setJS($js) {
			$this->js = $js;
 		}
		
		/**
		* Effectue l'affichage de la vue.
		*
		* @return void
		*/
		public function display() {
			
			$cssGlobal = array("global.css", "lightbox.css");
			
			$jsGlobal = array("jquery-2.1.0.min.js", "jquery.sticky.js", "global.js" );
			
			$cssTotal = array_merge($cssGlobal, $this->css);
			$jsTotal = array_merge($jsGlobal, $this->js);
			
			
			$header = new LightboxHeader_Global_View($cssTotal, $jsTotal);
			$footer = new LightboxFooter_Global_View();
			
			echo $header->getHeader();
			echo $this->mainContent();
			echo $footer->getFooter();
			

		}
		
		/**
		 * Permet d'obtenir le contenu principal de la page.
		 *
		 * @return string
		 */
		abstract protected function mainContent();
		
	}
?>