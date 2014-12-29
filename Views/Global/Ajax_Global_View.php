<?php

	abstract class Ajax_Global_View {
		

		
		/**
		* Effectue l'affichage de la vue.
		*
		* @return void
		*/
		public function display() {
			
			
			//echo $header->getHeader();
			echo $this->mainContent();
			//echo $footer->getFooter();
			

		}
		
		/**
		 * Permet d'obtenir le contenu principal de la page.
		 *
		 * @return string
		 */
		abstract protected function mainContent();
		
	}
?>