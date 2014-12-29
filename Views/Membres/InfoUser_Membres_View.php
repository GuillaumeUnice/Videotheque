<?php
	class InfoUser_Membres_View extends Lightbox_Global_View {
		
		
		public function InfoUser_Membres_View($viewparams) {
			
			$usercss = array("film.css","form.css");
			$this->setCSS($usercss);

			$this->nom = $viewparams["nom"];
			$this->prenom = $viewparams["prenom"];
			$this->date = $viewparams["date"];
			$this->note = $viewparams["note"];

			
		}
		
		public function mainContent() {
?>
			<article class="film" style="padding: 10px; background-color: gray;">
			<h1>Profil</h1>
			<h2>Description</h2>
			<p>
				Nom : <?php echo utf8_encode($this->nom); ?> <br />
				Prenom <?php echo  utf8_encode($this->prenom); ?> <br />
				Année d'inscription : <?php echo  utf8_encode($this->date); ?> <br />
			</p>
			
			<h2>Dernière activité</h2>
			<p>
				Ajouter en ami :
                <br />-UNAVAILABLE-
			</p>
			
			<h2>Films Notée</h2>
			<p>
			<?php
				foreach($this->note as $value) {
					echo $value . '<br /><br />';
				}
			?>
			</p>
            </article>
<?php		
		}
}
?>