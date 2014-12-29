<?php
	class Add_Films_View extends Lightbox_Global_View {
		
		private $form;
		
		public function Add_Films_View($viewparams) {
			$filmscss = array("films.css", "form.css", "chosen.css");
			$this->setCSS($filmscss);
			
			$filmsjs = array("chosen.jquery.js", "film.js");
			$this->setJS($filmsjs);
			
			
			if(isset($viewparams["erreur"]) && strcmp($viewparams["erreur"], "") != 0) {
					$this->erreur = $viewparams["erreur"];
			}
			if (isset($viewparams["ajouter"])&& strcmp($viewparams["ajouter"], "") != 0) {
					$this->ajouter = $viewparams["ajouter"];
			}
			
			$this->acteur = $viewparams["acteur"];
			$this->realisateur = $viewparams["realisateur"];
		}
		
		public function mainContent() {
			ob_start();		
?>

			<form action="./index.php?controller=Films&action=addFilm" method="post" >
					
				<h1>Formulaire Films</h1>
<?php
				if(isset($this->ajouter)) {
					echo '<div id="correct"><p>' . utf8_encode($this->ajouter) . '</p></div>';
				
				}
				if(isset($this->erreur))
					echo '<div id="erreur"><p>' . utf8_encode($this->erreur) . '</p></div>';
?>					
					<div class="border_form">
						<p>
							<label for="titre">Titre : </label><input type="text" id="titre" name="titre" placeholder="Tapez ici!" maxlength="255" autofocus required/>
							<label for="style">Style : </label>
							<select name="style" id="style" >
<?php
								// Affichage des Styles
								$tabStyle = Data_STYLE::getValues();
								foreach($tabStyle as $key => $value) {
									echo '<option value="' . $value . '" >' . utf8_encode(strtolower($value)) . '</option>';
								}
?>
							</select>
							<label for="langue">Langue : </label>
							<select name="langue" id="langue" >
<?php
								// Affichage des Langues
								$tabLangue = Data_LANGUE::getValues();
								foreach($tabLangue as $key => $value) {
									echo '<option value="' . $value . '" >' . utf8_encode(strtolower($value)) . '</option>';
								}
?>
							</select>
							Date: <input type="number" name="annee" min="1900" max="<?php echo (date('Y') + 10); ?>" value="<?php echo date('Y'); ?>">
						</p>
						<p>
							<label for="resume">Résumé : </label><br />
							<textarea name="resume" id="resume" placeholder="Tapez ici!" required></textarea>
						</p>
						<p>
							<label for="realisateur">Réalisateur : </label>
							<select name="realisateur" id="realisateur" >
								<option value="null" >Aucun</option>
								<?php
									foreach ($this->realisateur as $key => $value) {
										echo '<option value ="' . $key . '">' . utf8_encode($value) . '</option>';
									}
								?>
							</select>
						</p>
						
						<p  class="acteur">
							<label for="acteur1">Acteur 1 : </label>
							<select name="acteur1" id="acteur1" placeholder="Selectionner des acteurs" ><!--name="acteur[]" id="acteur" data-placeholder="Selectionner des acteurs" multiple class="chosen-select" >-->
								<option value="null" >Aucun</option>
								<?php
									foreach ($this->acteur as $key => $value) {
										echo '<option value ="' . $key . '">' . utf8_encode($value) . '</option>';
									}
								?>
								
							</select>
							<label for="role1">Rôle de l'acteur : </label><input type="text" id="role1" name="role1" placeholder="Rôle de l'acteur" maxlength="255" />
							<button type="button" id="addActeur">Ajouter</button>
						</p>
						
						<p  class="affiche">
							<label for="affiche1">Affiche 1 : </label><input type="text" id="affiche1" name="affiche1" placeholder="url/nom de depuis la racine du fichier"/>
							<button type="button" id="addAffiche">Ajouter</button>
						</p>
						<!--<label for="affiche">Affiche 1 : </label><input id="affiche" type="file" name="affiche" />-->
					</div>
					<p style="float: left; margin-left: 25px;">
						Envoyer : <input type="submit" value="Validez" />
					</p>
					<p style="float: right; margin-right: 25px;">
						Effacer tout : <input type="reset" value="Effacer"/>
					</p>
				</form>
<?php
		
			$content = ob_get_contents();
			ob_end_clean();
			
			return $content;	
		}
	}
?>