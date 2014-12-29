<?php
	class Update_Films_View extends Lightbox_Global_View {

		
		public function Update_Films_View($viewparams) {
			$filmscss = array("films.css", "form.css", "chosen.css", "lightbox.css");
			$this->setCSS($filmscss);
			$filmsjs = array("film.js");
			$this->setJS($filmsjs);
			
			$filmsjs = array("chosen.jquery.js");
			$this->setJS($filmsjs);
			
			
			if(isset($viewparams["erreur"]) && strcmp($viewparams["erreur"], "") != 0) {
					$this->erreur = $viewparams["erreur"];
			}
			if (isset($viewparams["update"])) {
					$this->update = $viewparams["update"];
			}
			
			$this->film = $viewparams['film'];
			$this->acteur = $viewparams["acteur"];
			$this->realisateur = $viewparams["realisateur"];
			$this->affiches = $viewparams['affiches'];
		}
		
		public function mainContent() {
			ob_start();
?>

			<h1>Formulaire d'Edition de Films</h1>
			
<?php
				if(isset($this->update)) {
?>
					<script type="text/javascript">
					$(document).ready(function() {
						$.ajax({
							type: 'GET',
							url: './index.php?controller=Films&action=printFilm&id=' + <?php echo $_GET['id']; ?>,
							timeout: 3000,
							success: function(data) {
								parent.$("#" + <?php echo $_GET['id']; ?>).replaceWith(data);
							},
							error: function() {
							alert('La requête n\'a pas abouti'); }
						}); 
					});
					</script>
<?php				echo '<div id="correct"><p>' . utf8_encode($this->update) . '</p></div>';	
				}
				if(isset($this->erreur))
					echo '<div id="erreur"><p>' . utf8_encode($this->erreur) . '</p></div>';
				
				if(! $this->film) {
					echo '<div id="erreur"><p>Le film \'a pas pu être trouvé une erreur c\'est produite</p><div>';
				} else {		
?>
			<form action="./index.php?controller=Films&action=edit<?php echo '&id=' . $_GET['id']?>" method="post">		
					<div class="border_form">
						<p>
							<label for="titre">Titre : </label><input type="text" id="titre" name="titre" placeholder="Tapez ici!" maxlength="255" value="<?php echo $this->film->getTitre(); ?>" required/>
							<label for="style">Style : </label>
							<select name="style" id="style" >
<?php
								// Affichage des Styles
								$tabStyle = Data_STYLE::getValues();
								foreach($tabStyle as $key => $value) {
									echo '<option value="' . $value . '" ';
										if($this->film->getStyle() == $value) echo 'selected';
									echo ' >' . utf8_encode(strtolower($value)) . '</option>';
								}
?>
							</select>

						<label for="langue">Langue : </label>
							<select name="langue" id="langue" >
<?php
								// Affichage des Langues
								$tabLangue = Data_LANGUE::getValues();
								foreach($tabLangue as $key => $value) {
									echo '<option value="' . $value . '" ';
										if($this->film->getLangue() == $value) echo 'selected';
									echo ' >' . utf8_encode(strtolower($value)) . '</option>';
								}
?>
							</select>
							Date: <input type="number" name="annee" min="1900" max="<?php echo (date('Y') + 10); ?>" value="<?php echo $this->film->getAnnee(); ?>">
						</p>
						<p>
							<label for="resume">Résumé : </label><br />
							<textarea name="resume" id="resume" placeholder="Tapez ici!" required><?php echo $this->film->getResume(); ?></textarea>
						</p>
						<p>
							<label for="realisateur">Réalisateur : </label>
							<select name="realisateur" id="realisateur" >
								<option value="null" >Aucun</option>
								<?php
									foreach ($this->realisateur as $key => $value) {
										echo '<option value ="' . $key . '" ';
										$hasRea = $this->film->getRealisateur();
										if(isset($hasRea)) if($this->film->getRealisateur()->getID() == $key) echo 'selected';
										echo '>' . utf8_encode($value) . '</option>';
									}
								?>
							</select>
						</p>
<?php
						$nbActeur = 1;
						foreach ($this->film->getPersonnages() as $key => $value) {
							//die('valeur : ' . $value . $value->getId());
							echo '<p>';
							echo utf8_encode($value);
							echo ' Supprimer? ';
							echo '<label for="supAct' . $nbActeur . '">Oui</label>';
							echo '<input id="supAct' . $nbActeur . '" type="radio" name="updateActeur' . $nbActeur . '" value="' . $value->getId() . '"> ';
							echo '<label for="noSupAct' . $nbActeur . '">Non</label>';
							echo '<input id="noSupAct' . $nbActeur . '" type="radio" name="updateActeur' . $nbActeur . '" value="0" checked>';
							echo '</p>';
							++$nbActeur;
						}

?>
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
						
<?php
						$nbAffiche = 1;
						foreach ($this->affiches as $key => $value) {
							echo '<p class=film_miniature>';
							echo '<img src ="' . $value->getSrc() . '" />';
							echo 'Supprimer?';
							echo '<label for="supAff' . $nbAffiche . '">Oui</label>';
							echo '<input id="supAff' . $nbAffiche . '" type="radio" name="updateAffiche' . $nbAffiche . '" value="' . $value->getId() . '"> ';
							echo '<label for="noSupAff' . $nbAffiche . '">Non</label>';
							echo '<input id="noSupAff' . $nbAffiche . '" type="radio" name="updateAffiche' . $nbAffiche . '" value="0" checked>';
							echo '</p>';
							//die("valeur : " . $key->getSrc());
							++$nbAffiche;
						}
						
						
?>
						<p  class="affiche">
							<label for="affiche1">Affiche 1 : </label><input type="text" id="affiche1" name="affiche1" placeholder="url/nom de depuis la racine du fichier"/>
							<button type="button" id="addAffiche">Ajouter</button>
						</p>
						
						
						
					</div>
					<p style="float: left; margin-left: 25px;">
						Envoyer : <input type="submit" value="Editez" />
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
	}
?>