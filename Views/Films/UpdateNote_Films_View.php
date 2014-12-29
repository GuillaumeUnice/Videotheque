<?php
	class Update_Films_View extends Lightbox_Global_View {

		
		public function UpdateNote_Films_View($viewparams) {
			$filmscss = array("films.css", "form.css", "chosen.css", "lightbox.css");
			$this->setCSS($filmscss);
			
			$filmsjs = array("chosen.jquery.js");
			$this->setJS($filmsjs);
			
			
			if(isset($viewparams["erreur"]) && strcmp($viewparams["erreur"], "") != 0) {
					$this->erreur = $viewparams["erreur"];
			}
			if (isset($viewparams["update"])) {
					$this->update = $viewparams["update"];
			}
			
			$this->film = $viewparams['film'];
		}
		
		public function mainContent() {
			ob_start();
?>

			<h1>Formulaire d'Edition de Note</h1>
			
<?php
				if(isset($this->update)) {
?>
					<script type="text/javascript">
					$(document).ready(function() {
						$.ajax({
							type: 'GET',
							url: 'http://localhost/film/mvc-poo-ihm-2014/index.php?controller=Films&action=printFilm&id=' + <?php echo $_GET['idFilm']; ?>,
							timeout: 3000,
							success: function(data) {
								parent.$("#" + <?php echo $_GET['idFilm']; ?>).replaceWith(data);
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