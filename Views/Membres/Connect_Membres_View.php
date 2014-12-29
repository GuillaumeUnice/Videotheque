<?php
	class Connect_Membres_View extends Lightbox_Global_View { //Main_Global_View {
		

		public function Connect_Membres_View($viewparams) {
			$usercss = array("form.css");
			$this->setCSS($usercss);
			
			if(isset($viewparams["connect"])) {
				
				echo '<script type="text/javascript">';
						echo 'parent.jQuery.fancybox.close();';
						echo 'parent.location.reload(true);';
						
					echo '</script>';
					$this->connect = $viewparams["connect"];
			}
			if(isset($viewparams["erreur"])) {
					$this->erreur = $viewparams["erreur"];
			}
			
		}
		public function mainContent() {
		
			echo '<h1>Connexion</h1>';
			if(isset($this->erreur)) {
				echo '<div id="erreur"><p>' . utf8_encode($this->erreur) . '</p></div>';
			}		
?>
						
			<form id="form" action="./index.php?controller=Membres&action=connect" method="post">
				
				
				<div class="border_form">
					<p>
						<label for="nom">Nom : </label><input type="text" id="nom" name="nom" placeholder="Tapez ici!" maxlength="50" autofocus required/>
					</p>
					<p>
						<label for="prenom">Prénom : </label><input type="text" id="prenom" name="prenom" placeholder="Tapez ici!" maxlength="50" autofocus required/>
					</p>
					<p>
						<label for="mdp">Mot de passe : </label><input type="password" id="mdp" name="mdp" placeholder="Tapez ici!" maxlength="50" autofocus required/>
					</p>
					
				</div>
				<p>
					Valider : <input type="submit" value="Validez" />
				</p>
			</form>
			
			<a id="iframe" href="./index.php?controller=Membres&action=addUser">Créer un compte ?</a>
				
<?php
		}
	}
?>