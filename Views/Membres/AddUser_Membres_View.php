<?php
	class AddUser_Membres_View extends Lightbox_Global_View {
		
		
		public function AddUser_Membres_View($viewparams) {
			
			$usercss = array("form.css");
			$this->setCSS($usercss);
			
			if(isset($viewparams["ajouter"])) {
					$this->ajouter = $viewparams["ajouter"];
			}
			if(isset($viewparams["erreur"])) {
					$this->erreur = $viewparams["erreur"];
			}
			
		}
		public function mainContent() {
		echo '<h1 >Inscription</h1>';
		if(isset($this->ajouter)) {
		?>
			
			<div id="correct"><p><?php echo utf8_encode($this->ajouter) ?></p></div>
			<a id="iframe" href="./index.php?controller=Membres&action=Connect">Connection</a>
		<?php
		} else {	
			if(isset($this->erreur)) {
				echo '<div id="erreur"><p>' . utf8_encode($this->erreur) . '</p></div>';
			}
		
?>

				<form action="./index.php?controller=Membres&action=addUser" method="post">
					
					
					<div class="border_form">
						<p>
							<label for="nom">Nom : </label><input type="text" id="nom" name="nom" placeholder="Tapez ici!" maxlength="50" autofocus required/>
						</p>
						<p>
							<label for="prenom">Prénom : </label><input type="text" id="prenom" name="prenom" placeholder="Tapez ici!" maxlength="50" autofocus required/>
						</p>
						<p>
							<label for="mdp1">Mot de passe : </label><input type="password" id="mdp1" name="mdp1" placeholder="Tapez ici!" maxlength="50" autofocus required/>
						</p>
						
						<p>
							<label for="mdp2">Confirmation mot de passe : </label><input type="password" id="mdp2" name="mdp2" placeholder="Tapez ici!" maxlength="50" autofocus required/>
						</p>
					</div>
					<p>
						Valider : <input type="submit" value="Validez" />
					</p>
				</form>
				
				<a id="iframe" href="./index.php?controller=Membres&action=Connect">Déjà inscris ?</a>
<?php
}
		}
	}
?>