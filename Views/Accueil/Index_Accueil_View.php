<?php
	class Index_Accueil_View extends Main_Global_View {
		
		public function Index_Accueil_View() {
			$accueilcss = array("accueil.css");
			$this->setCSS($accueilcss);
		}
		
		public function mainContent() {
			ob_start();
?>
<h1>Accueil</h1>
<article id="welcome">
	<header>Bienvenue dans la biblioth&egrave;que de films!</header>

	<p>
		Des renseignements à propos de films, acteurs, réalisateurs c'est ici!!!<br />
		Participer à l'essor du site en créeant un compte dans le but de rajouter vos meilleurs films, acteurs mais aussi de les commenter.<br />
		A travers ce site faite la connaissance d'autres cinéphiles!
	</p>
</article>


<?php
			
			$content = ob_get_contents();
			ob_end_clean();
			
			return $content;	
		}
	}
?>