<?php
	class Membres_Controller {
	
			/**
			* // public car requise par le parser
			* Permet de se connecter
			* @param void
			* @return void et appelant la vue Connect_Films_View($viewparams)
			* $viewparams contenant : $viewparams['erreur']
			*/
			public function connect() {
				
				if(isset($_POST['nom']))
				{
					$UserInstance = Nf_UserDvdManagement::getInstance();

					$res = $UserInstance->getUser($_POST["nom"], $_POST["prenom"]);
					if(isset($res)) {
					
						if($res->getMdp() == $_POST["mdp"]) {
							//$res[] = $UserInstance::getUsersFromRequest('SELECT id, Nom, Prenom, inscription, mdp FROM user WHERE Nom = \'' . $_POST['nom'] . '\' AND mdp = \'' . $_POST['mdp'] . '\'');
				
							$_SESSION["id"] = $res->getId();
							$_SESSION["nom"] = $res->getNom();
							$_SESSION["prenom"] = $res->getPrenom();
							$_SESSION["mdp"] = $res->getMdp();
							$_SESSION["date"] = $res->getDate();
							$viewparams["connect"] = true;

						} else { $viewparams["erreur"] = "Erreur : mot de passe incorrect";}
					} else { $viewparams["erreur"] = "Erreur : Nom ou Prenom incorrect";}
				}
				
				$view = new Connect_Membres_View($viewparams);
				$view->display();
			}
			
			/**
			* // public car requise par le parser
			* Permet de créer un compte pour se connecter
			* @param void
			* @return void et appelant la vue AddUser_Films_View($viewparams)
			* $viewparams contenant : $viewparams['erreur'], $viewparams['ajouter']
			*/
			public function addUser() {
				
				if(isset($_POST['nom']))
				{
					$UserInstance = Nf_UserDvdManagement::getInstance();
					
					$res = $UserInstance->getUser($_POST["nom"], $_POST["prenom"]);
					if(!isset($res)) {
						if(strcmp($_POST["mdp1"], $_POST["mdp2"]) == 0) {
						
							$user = new Data_User($_POST["nom"], $_POST["prenom"], null, $_POST["mdp1"]);
							if($UserInstance->addUser($user))
							{
								$viewparams["ajouter"] = "L'enregistrement est efffectué";
							} else {
								$viewparams["erreur"] = "Erreur : lors de requete SQL";
							}
							
						} else { $viewparams["erreur"] = "Erreur : la confirmation du mot de passe est incorrect";}
					} else { $viewparams["erreur"] = "Erreur : Il existe déjà une personne ayant ces informations, vous ne pouvez vous réinscrire plusieur fois!!!";}
				}
				
				$view = new AddUser_Membres_View($viewparams);
				$view->display();
			}
			
			/**
			* // public car requise par le parser
			* Permet de se déconnecter
			* @param void
			*/
			public function disconnect() {
				
				$_SESSION = array();
				session_destroy();
				$oldPage = $_SERVER['HTTP_REFERER'];
				header('Location: ' . $oldPage);
			}
			
			/**
			* // public car requise par le parser
			* Permet d'afficher le profil/info d'un compte
			* @param int $params["id"] contient l'id de l'user
			* @return void et appelant la vue InfoUser_Films_View($viewparams)
			* $viewparams contenant : 
			*/
			public function infoUser($params) {
				
				
				$UserInstance = Nf_UserDvdManagement::getInstance();
				$user = $UserInstance->idToUser($params["id"]);

				//recuperation des notes de l'utilisateur
				$note = Nf_CommNoteManagement::getInstance()->getNotesParUser($user);
				
				$viewparams["note"] = $note;
				$viewparams["nom"] = $user->getNom();
				$viewparams["prenom"] = $user->getPrenom();
				$viewparams["date"] = $user->getDate();
				
				$view = new InfoUser_Membres_View($viewparams);
				$view->display();
				
			}
	}
?>