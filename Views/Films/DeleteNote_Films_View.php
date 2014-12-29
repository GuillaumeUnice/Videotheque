<?php
	class DeleteNote_Films_View extends Lightbox_Global_View {
		
		//private $films;
		
		public function DeleteNote_Films_View($viewparams) {
			$filmscss = array("films.css", "form.css", "lightbox.css");
			$this->setCSS($filmscss);
			
			
			if(isset($viewparams["erreur"])) {
				$this->erreur = $viewparams["erreur"];
			}
			if (isset($viewparams["delete"])) {
				$this->id = $viewparams['id'];
				$this->delete = $viewparams["delete"];
			}
			
			$this->film = $viewparams['film'];
			
		}
		
		public function mainContent() {
			ob_start();
?>

			<h1>Suppression de Films</h1>
<?php
				
				if(isset($this->erreur)) {
					echo '<div id="erreur"><p>' . utf8_encode($this->erreur) . '</p></div>';	
				}
				if(isset($this->delete)) {
				
				?>	
					<script type="text/javascript">
					$(document).ready(function() {
						$.ajax({
							type: 'GET',
							url: './index.php?controller=Films&action=printFilm&id=' + <?php echo $_GET['idFilm']; ?>,
							timeout: 3000,
							success: function(data) {
								parent.$("#" + <?php echo $_GET['idFilm']; ?>).replaceWith(data);
							},
							error: function() {
							alert('La requête n\'a pas abouti'); }
						}); 
					});
					</script>
					
<?php
					echo '<div id="correct"><p>' . utf8_encode($this->delete) . '</p></div>';		
				} else if((!isset($this->film)) || (!isset($this->film->note))) {
				
					echo '<div id="erreur"><p>erreur affichage note ou film introuvable!</p></div>';

				} else {		
?>
	
			<div style="text-align: center;">
				<p>
					Etes vous certains de vouloir supprimer votre note de : <?php echo utf8_encode($this->film->note->getNote()); ?>?
				</p>
				<?php //die('lol : ' . $this->film->note->getId()); ?>
				<form action="./index.php?controller=Films&action=deleteNote&id=<?php  
				echo $this->film->note->getId(); ?>&idFilm=<?php 
				echo $this->film->getId();?>" method="post">		
					<input type="hidden" name="delete" value="<?php echo $this->film->note->getId(); ?>" />
					<input type="submit" value="Oui" />
					<button onclick="parent.jQuery.fancybox.close()" type="button" id="annuler">Annuler</button>
					
				</form>
			</div>
<?php
		
			$content = ob_get_contents();
			ob_end_clean();
			
			return $content;	
			}
		
		}
	}
?>