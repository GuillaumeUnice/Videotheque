<?php

//include constant("PATHTOROOT")."includes/connexion.inc";

/**
 * 
 * @author renevier-gonin
 * @package Nf_
 * 
 * @abstract la classe Nf_ActeurReaManagement permet de lire, d'ajouter, de modifier ou de supprimer
 * des personnes (acteur ou réalisaeur ou ni l'un ni l'autre) du monde du cinéma. Elle s'utilise :
 * <code>
 * $actreaMgt = Nf_ActeurReaManagement::getInstance();
 * $acteurs = $actreaMgt->getActeurs(); // pour obtenir la liste de tous les acteurs
 * $hamills = $actreaMgt->getActeurs("Hamill", "Mark"); // pour obtenir tous les acteurs dont le nom contien Hamill et dont le prénom contient "Mark"
 * // attention au homonyme...
 * if ($hamills) $markhamill = $hamills[0] ; // si le tableau n'est pas vide, on prend le premier Mark Hamill...
 * </code>
 * Les personnes ne deviennent acteurs ou réalisateurs que lorsqu'elles sont associées à un film. 
 * On peut donc ajouter des personnes et ensuite les associer à des films...
 */
class Nf_ActeurReaManagement {

	/**
	 * @ignore
	 * @var Nf_ActeurReaManagement
	 */
	protected static $instance;
	/**
	 * @ignore
	 */
	protected $connexion;

	/**
	 * 
	 * @ignore
	 */
	protected function __construct()
	{
		$this->connexion = constant('CONNEXION');	
	}
	
	/**
	 * pour obtenir l'unique instance de Nf_ActeurReaManagement
	 * @return Nf_ActeurReaManagement
	 */
	public static function getInstance()
	{
		if (! Nf_ActeurReaManagement::$instance) Nf_ActeurReaManagement::$instance = new Nf_ActeurReaManagement();
		
		return Nf_ActeurReaManagement::$instance;
	}
	
	/**
	 * @ignore
	 */
	protected function peopleToId(Data_People $p) {
		$result = -1;

		$query = "SELECT id from acteursrealisateurs WHERE ";
		$query .= "Nom = \"".$p->getNom()."\" ";
		$query .= "AND Prenom = \"".$p->getPrenom()."\" ";
		$query .= "AND Nationalite = \"".$p->getNationalite()."\" ";
		$query .= "AND Naissance = \"".$p->getNaissance()."\" ";
		// test de champ null avec IS
		if ($p->getMort() < 0) $query .= "AND Mort IS NULL ";
		else $query .= "AND Mort = \"".$p->getMort()."\" ";
		if ($p->isSexeFeminin()) $query .= "AND Sexe = \"f\"";
		else $query .= "AND Sexe = \"m\"";		
		
		$results = mysql_query($query, $this->connexion);
		if (mysql_num_rows($results) == 1)
		{
			$rep = mysql_fetch_array($results);
			$result = $rep["id"];
		}	
			
		return $result;
	}
	
	/**
	 * @ignore
	 */
	protected function fromCompleteRequestToPeople($localResults)  {
		$r = new Data_People();
		$r->setNom($localResults["Nom"]);
		$r->setPrenom($localResults["Prenom"]);
		$r->setNaissance($localResults["Naissance"]);
		$r->setNationalite(strtoupper($localResults["Nationalite"]));
		$m = $localResults["Mort"];
		if ($m > 0) $r->setMort($m);
		$sexe = $localResults["Sexe"];
		if ($sexe == "f") $r->setSexeFeminin(true);
		
		
		$r->setId($localResults["id"]);
		
		return $r;
	}
	
	
	/**
	 * Cette méthode permet d'obtenir un Data_People à partir d'un id<br />
	 * méthode publique pour rendre les échanges via formulaire plus simple
	 * 
	 * @param int $id l'id dans la bd
	 * @return Data_People : la personne sous forme d'une Data_People correspondant à l'id
	 */
	public function idToPeople($id)
	{
		$query = "SELECT * FROM acteursrealisateurs where id=\"$id\"";
		$localResults =  mysql_query($query, $this->connexion);

		$p = null;
		// on extrait une personne
		if (mysql_num_rows($localResults) == 1)
		{
			$p = $this->fromCompleteRequestToPeople(mysql_fetch_array($localResults));
		}

		return $p;
	}
	
	/**
	 * @ignore
	 */
	protected function fromCompleteRequestToRealisateur($localResults)  {
		$r = new Data_Realisateur($this->fromCompleteRequestToPeople($localResults));
		return $r;
	}
	
	/**
	 * Cette méthode permet d'obtenir un Data_Realisateur à partir d'un id<br />
	 * méthode publique pour rendre les échanges via formulaire plus simple
	 * 
	 * @param int $id l'id dans la bd
	 * @return Data_Realisateur : la personne sous forme d'une Data_Realisateur correspondant à l'id
	 */
	public function idToRealisateur($id) 
	{
		$query = "SELECT * FROM acteursrealisateurs where id=\"$id\"";
		$localResults =  mysql_query($query, $this->connexion);
		
		$r = null;
		// on extrait un realisateur
		if (mysql_num_rows($localResults) == 1)
		{
			$r = $this->fromCompleteRequestToRealisateur(mysql_fetch_array($localResults));
		}

		return $r;
	}
	
	/**
	 * @ignore
	 */
	protected function fromCompleteRequestToActeur($localResults) {
		$a = new Data_Acteur($this->fromCompleteRequestToPeople($localResults));
		return $a;
	}
	
	/**
	 * Cette méthode permet d'obtenir un Data_Acteur à partir d'un id<br />
	 * méthode publique pour rendre les échanges via formulaire plus simple
	 * 
	 * @param int $id l'id dans la bd
	 * @return Data_Acteur : la personne sous forme d'une Data_Acteur correspondant à l'id
	 */
	public function idToActeur($id)
	{
		$query = "SELECT * FROM acteursrealisateurs where id=\"$id\"";
		$localResults =  mysql_query($query, $this->connexion);

		$r = null;
		// on extrait un realisateur
		if (mysql_num_rows($localResults) == 1)
		{
			$r = $this->fromCompleteRequestToActeur(mysql_fetch_array($localResults));
		}

		return $r;
	}
	
	/**
	 * @ignore
	 */
	protected function getRealisateursFromRequest($query) {
		$result = array();
			$localResults =  mysql_query($query, $this->connexion);

			//	on prend le resultat
			 if ( $localResults) while ($rep = mysql_fetch_array($localResults)) {
				$result[] = $this->fromCompleteRequestToRealisateur($rep);
			}

		
		return $result;
	}

	/**
	 * @ignore
	 */
	protected function getActeursFromRequest($query) {
		$result = array();
			$localResults =  mysql_query($query, $this->connexion);

			//	on prend le resultat
			 if ( $localResults) while ($rep = mysql_fetch_array($localResults)) {
				$result[] = $this->fromCompleteRequestToActeur($rep);
			}

		
		return $result;
	}
	
	/**
	 * @ignore
	 */
	protected function estDansLaBaseActeurRealisateur(Data_People $p)
	 {
		 return ($this->peopleToId($p) > 0);
	 }


	/**
	 * @ignore
	 */
	 protected function estPeopleIncomplet(Data_People $a)
	 {
		 $result = ($a == null) || ($a->getNaissance() <= 0) 
		 || ($a->getNationalite() == null) 
		 || ($a->getNom() == null) || ($a->getNom() == "")
		 || ($a->getPrenom() == null) || ($a->getPrenom() == "");
		 return  $result;
	 }
	 
	 
	 
/***************************************************************************
	             ***************** Partie pour lire les données ************
	             *****************************************************************************/	 
	 
	 
	 /**
	  * permet d'avoir la liste des acteurs et realisateur potentiel, dont on ne sait pas ce qu'ils ont fait
	  * @return array : un tableau des acteurs ou realisateurs dont on n'a pas renseigné de film
	  */
	 public function getNiActeurNiRealisateurs()
	 {
		 /*
		  * requete qui prend tous les acteurs et realisateur
		  * SELECT acteursrealisateurs.* FROM acteursrealisateurs JOIN jouedans on 
		  * jouedans.acteur = acteursrealisateurs.id UNION SELECT acteursrealisateurs.* 
		  * FROM acteursrealisateurs JOIN realise on 
		  * realise.realisateur = acteursrealisateurs.id 
		  * 
		  * idem sur une ligne :
		  * SELECT acteursrealisateurs.* FROM acteursrealisateurs JOIN jouedans on jouedans.acteur = acteursrealisateurs.id UNION SELECT acteursrealisateurs.* FROM acteursrealisateurs JOIN realise on realise.realisateur = acteursrealisateurs.id 
		  * 
		  * EXCEPT ne semble pas marche...
		  */

		 // liste des acteurs / realisateur
		$result = array(); 
	 	$pui = array();
		 $query = "SELECT acteursrealisateurs.* FROM acteursrealisateurs JOIN jouedans on jouedans.acteur = acteursrealisateurs.id UNION SELECT acteursrealisateurs.* FROM acteursrealisateurs JOIN realise on realise.realisateur = acteursrealisateurs.id";

		 
		 $localResults =  mysql_query($query);

			  if ( $localResults) while ($rep =mysql_fetch_array($localResults)) {
				 $pui[] = $rep["id"];
			 }
		 

		 $result = array();
		 $query = "SELECT * FROM acteursrealisateurs";

		 $localResults =  mysql_query($query);

			  if ( $localResults) while ($rep =mysql_fetch_array($localResults)) {
				 $id = $rep["id"];
				 if (! in_array($id, $pui))
				 {
					 $p = $this->fromCompleteRequestToPeople($rep);
					 $result[] = $p;
				 }
			 }
			 


		 return $result;
	 }
	 
	 
	 
   

	/**
	 * si pas de paramètre : pour obtenir la liste des réalisateurs (ceux qui ont réalisé au moins un film)
	 * si un seul paramètre : pour obtenir la liste des réalisateurs en fonction d'un morceau du nom ou du prénom
	 * si les deux paramètres : pour obtenir la liste des réalisateurs en fonction d'un morceau du nom et d'un morceau du prénom<br />
	 * par exemple avec Jarre et Michel, vous pourriez avoir Jean-Michel Jarre et Michel Jarre.<br /> 
	 * @param string nom est un nom ou un morceau de nom
	 * @param string prenom est un prénom ou un morceau de prénom
	 * @return array : retourne tous les réalisateurs dont le nom et le prénom contient "nom" dans un tableau.
	 */
	public function getRealisateurs($nom = "" , $prenom ="" ){
		$query = "SELECT DISTINCT acteursrealisateurs.* FROM realise JOIN acteursrealisateurs ON realise.realisateur = acteursrealisateurs.id";
		
		if ($nom && $prenom)
		{
			$query = "SELECT DISTINCT acteursrealisateurs.* FROM realise JOIN acteursrealisateurs ON realise.realisateur = acteursrealisateurs.id ";
			$query .= " WHERE acteursrealisateurs.nom LIKE \"%$nom%\" AND acteursrealisateurs.prenom LIKE \"%$prenom%\"";
		}
		else if ($nom)
		{
			$query = "SELECT DISTINCT acteursrealisateurs.* FROM realise JOIN acteursrealisateurs ON realise.realisateur = acteursrealisateurs.id ";
			$query .= " WHERE acteursrealisateurs.nom LIKE \"%".$nom."%\" OR acteursrealisateurs.prenom LIKE \"%".$nom."%\"";
			
		}
		return $this->getRealisateursFromRequest($query);
	}


	/**
	 * pour obtenir la liste des personnes en fonction d'un morceau du nom et d'un morceau du prénom<br />
	 * par exemple avec Jarre et Michel, vous pourriez avoir Jean-Michel Jarre et Michel Jarre.<br /> 
	 * pour obtenir la liste de tous les personnes (acteurs, réalisateurs, ceux qui ne sont pas encore affectés à un film) : ne pas mettre de parametre
	 * @return array retourne tous les personnes (People) dans un tableau.
	 */
	public function getPersonnes($nom = "" , $prenom ="" ){		
		$query = "SELECT DISTINCT acteursrealisateurs.* FROM acteursrealisateurs";
		
	
		if ($nom && $prenom)
		{
			$query .= " WHERE acteursrealisateurs.nom LIKE \"%$nom%\" AND acteursrealisateurs.prenom LIKE \"%$prenom%\"";
		}
		else if ($nom)
		{
			$query .= " WHERE acteursrealisateurs.nom LIKE \"%".$nom."%\" OR acteursrealisateurs.prenom LIKE \"%".$nom."%\"";
			
		}

		$result = array();
		
			$localResults =  mysql_query($query);

			//	on prend le resultat
			 if ( $localResults) while ($rep = mysql_fetch_array($localResults)) {
				$result[] = $this->fromCompleteRequestToPeople($rep);
			}


		return $result;
	}



	/**
	 * sans paramètre : pour obtenir la liste de tous les acteurs (ceux qui jouent dans au moins un film)
	 * avec un paramètre : pour obtenir la liste de tous les acteurs (ceux qui jouent dans au moins un film) dont 
	 * avec deux paramètres :pour obtenir la liste des acteurs en fonction d'un morceau du nom et d'un morceau du prénom<br />
	 * par exemple avec Jarre et Michel, vous pourriez avoir Jean-Michel Jarre et Michel Jarre.<br /> 
	 * @param string nom est un nom ou un morceau de nom
	 * @param string prenom est un prénom ou un morceau de prénom
	 * @return array retourne tous les acteurs dont le nom et le prénom contient "nom" dans une arraylist.
	 */
	public function getActeurs($nom="", $prenom=""){
		$query = "SELECT DISTINCT acteursrealisateurs.* FROM jouedans JOIN acteursrealisateurs ON jouedans.acteur = acteursrealisateurs.id";
		
		if ($nom && $prenom)
		{
			$query = "SELECT DISTINCT acteursrealisateurs.* FROM jouedans JOIN acteursrealisateurs ON jouedans.acteur = acteursrealisateurs.id ";
			$query .= " WHERE acteursrealisateurs.nom LIKE \"%$nom%\" AND acteursrealisateurs.prenom LIKE \"%$prenom%\"";
		}
		else if ($nom)
		{
			$query = "SELECT DISTINCT acteursrealisateurs.* FROM jouedans JOIN acteursrealisateurs ON jouedans.acteur = acteursrealisateurs.id";
			$query .= " WHERE acteursrealisateurs.nom LIKE \"%$nom%\" OR acteursrealisateurs.prenom LIKE \"%$nom%\"";
			
		}
		return $this->getActeursFromRequest($query);
	}

	
	/**
	  * retourne une liste d'image (sous forme de tableau de Data_Img a utiliser avec getSrc() )à partir d'une personne
	  * s'il n'y a pas d'image ou un probleme avec la base de donnees, retourne une liste vide (ou partielle)
	  * @param Data_People $p la personne (acteur ou realisateur)
	  * @return array toutes les images liees a cette personne 
	  */
	 public function getPortraits(Data_People $p) {
		 $result = array();

		
			 $pid = $this->peopleToId($p);

			 $query = "SELECT portraitId  from portrait WHERE personne = \"$pid\"";

			 $localResults =  mysql_query($query, $this->connexion);

			 //	on prend le resultat
			 if ( $localResults) while ($rep = mysql_fetch_array($localResults)) {
				 $id  = $rep["portraitId"];
				 $result[] =new Data_Img($id, false);
			 }

		

		 return $result;
	 }
	 
	/**
	 * pour obtenur tous les rôles (Data_RoleDansFilm) d'un acteur
	 * @param Data_Acteur $a : l'acteur dont on veut la liste de role
	 * @return array la liste si elle existe, null si a n'est pas complet, une liste vide si l'acteur n'a pas de role
	 * enregistre dans la base
	 */
	 public function getRoles(Data_Acteur $a)
	 {
		 if ($this->estPeopleIncomplet($a)) return null;

		 $result = array();

		 $aId = $this->peopleToId($a);
		 $query = "SELECT film.Titre, jouedans.role, jouedans.film, jouedans.rid FROM jouedans JOIN film ON film.id = jouedans.film where acteur = '$aId';";
		
			 $localResults =  mysql_query($query, $this->connexion);
			 
			 if ($localResults) while ($rep = mysql_fetch_array($localResults)) {
				 $fId = $rep["film"];
				 $nomRole = $rep["role"];
				 $titre = $rep["Titre"];
				 $r = new Data_Role($nomRole, $a);
				 $r->setId($rep["rid"]);
				 $result[] = new Data_RoleDansFilm($r, $fId, $titre);
			 }
		
		 return $result;
	 }
	 
	 
	 /***************************************************************************
	             ***************** Partie pour ajouter les données ************
	             *****************************************************************************/	 
	 
	/**
	  * pour ajouter un nouvel acteur
	  * @param Data_Acteur $a est l'acteur à ajouter
	  * @return boolean vrai si l'action a réussie, faux sinon (déjà présent dans la base)
	  */
	 public function addActeur(Data_Acteur $a) 
	 {
		 /*
		  * INSERT INTO `acteursrealisateurs` ( `id` , `Nom` , `Prenom` , `Nationalite` , `Naissance` , `Mort` , `Sexe` )
		  * VALUES (
		  * '', 'Poelvoorde', 'Benoît', 'BELGE', '1964', NULL , 'm'
		  * );
		  */
		 if ($this->estPeopleIncomplet($a))
		 {
			 return false;
		 }
		 // on vérifie si c'est dans la base : acteur (jouedans) ou realisateur (realise)
		 if ($this->estDansLaBaseActeurRealisateur($a))
		 {
			 return false;
		 }

		 $mort = "NULL";
		 $sexe = "\"m\"";
		 if ($a->isSexeFeminin()) $sexe = "\"f\"";
		 if ($a->getMort() > 0) $mort = "\"".$a->getMort()."\"";
		 $update = "INSERT INTO acteursrealisateurs VALUES (id, \"".$a->getNom()."\", \"".$a->getPrenom()."\", " .
		 "\"".$a->getNationalite()."\", \"".$a->getNaissance()."\", ".$mort.", ".$sexe.")";

		
		return mysql_query($update);
		
	 }
	 
 	/**
	  * pour ajouter un nouveau réalisateur
	  * @param Data_Realisateur $r le réalisateur à ajouter à la base
	  * @return boolean vrai si l'action a réussie, faux sinon (déjà présent dans la base)
	  */
	 public function addRealisateur(Data_Realisateur $r) 
	 {
		 if ($this->estPeopleIncomplet($r))
		 {
			 return false;
		 }
		 if ($this->estDansLaBaseActeurRealisateur($r))
		 {
			 return false;
		 }

		 $mort = "NULL";
		 $sexe = "\"m\"";
		 if ($r->isSexeFeminin()) $sexe = "\"f\"";
		 if ($r->getMort() > 0) $mort = "\"".$r->getMort()."\"";
		 $update = "INSERT INTO acteursrealisateurs VALUES (id, \"".$r->getNom()."\", \"".$r->getPrenom()."\", " .
		 "\"".$r->getNationalite()."\", \"".$r->getNaissance()."\", ".$mort.", ".$sexe.")";

		
		return mysql_query($update);
	 }
	 
	 
	 
	/**
	  * ajoute une photo de la personne
	  * Le type mime (image/png, image/gif, image/jpeg) est déterminé automatiquement
	  * en fonction du nom de l'image (.png, .gif, .jpg ou .jpeg)
	  * @param Data_People $p la personne
	  * @param string image : le nom de fichier ou l'url
	  * @return boolean vrai si cela a fonctionné
	  */
	 public function addPortraitAUnePersonne(Data_People $p, $image)
	 {
		 $retour = false;

			$pid = $this->peopleToId($p);

		 $content = addslashes(file_get_contents($image));
		 
		 $ext3 = substr($file, -4); 
		 $ext4 = substr($file, -5); 
		 $type = "image/png";
		 if (($ext3 == ".jpg") || ($ext4 == ".jpeg")) $type = "image/jpg";
		 else if ($ext3 == ".gif") $type = "image/jpg";

		if ($content)
		{
		 $query = "insert into portrait (portraitId, personne, image, type) values ('','$pid','$content', '$type')";
		 
		 $retour = mysql_query($query, $this->connexion);
		}
		

		 return $retour;
	 }
	 
	 
	 /***************************************************************************
	             ***************** Partie pour supprimer les données ************
	             *****************************************************************************/	 
	 
	 
	 
 	/**
	  * permet d'effacer un acteur ou un realisateur (ou une personne enregistree qui n'est 
	  * pas encore affectee a un film). Ceci effacera aussi toutes eventuelles realisations, tous 
	  * eventuels portraits, tous eventuels roles.
	  * @param Data_People $p : la personne à supprimer de la base de données
	  * @return boolean true en cas de succes
	  */
	 public function removePersonne(Data_People $p)
	 {
		 $result = true;
		 
		 $pid = $this->peopleToId($p);
		 		 
		 if ($pid > 0)
		 	{
			 $query = "DELETE FROM `realise` WHERE `realisateur`='$pid';";
			 $result = mysql_query($query) ;
			 
			 $query = "DELETE FROM `portrait` WHERE `personne`='$pid';";
			 $result = mysql_query($query) || $result;

			 $query = "DELETE FROM `jouedans` WHERE `acteur`='$pid';";
			 $result = mysql_query($query) || $result;
			 
			 $query = "DELETE FROM `acteursrealisateurs` WHERE `id`='$pid';";
			 $result = mysql_query($query)||  $result;
		 	}
		 
		 return $result;
	 }
	 
	 
     /**
	  * supprime une occurence de l'image dans la table "portrait"
	  * (en general, la derniere occurence). 
	  * idee initiale : requete sql : DELETE FROM `portrait` WHERE CONVERT(`image` USING utf8) = CAST("+blob+" AS BINARY) LIMIT 1;
	  * ou blob est le tableau d'octet convertit en chaine de caractere (0xAB00055...) <br />
	  * ceci ne marchant pas, la requete est du style : WHERE 'portraitID' = 'id';
	  * @param int $id : est l'id d'une image dans la table (champ de la classe Img)
	  * @return boolean vrai si la suppression a été effectuée
	  */
	 public function removePortraitAUnePersonne($id) 
	 {
		 $query = "DELETE FROM `portrait` WHERE `portraitId`  = '$id'  LIMIT 1;";

		 $retour = false;
		 $retour = mysql_query($query, $this->connexion);

		 return $retour;
	 }
	 
	 
	/**
	  * efface toutes les images d'une personne
	  * @param Data_People $p dont on veut effacer tous les portraits
	  * @return boolean vrai si des donnees ont ete effacees
	  */
	 public function removeTousLesPortraitsDUnePersonne(Data_People $p) 
	 {
		 $pid = $this->peopleToId($p);
		 $query = "DELETE FROM `portrait` WHERE personne = \"$pid\"";

		 $retour = false;
		
			 $retour = mysql_query($query, $this->connexion);
		 

		 return $retour;
	 }
	 
	 /***************************************************************************
	             ***************** Partie pour mettre a jour les données ************
	             *****************************************************************************/	 
	 
	 /**
	  * pour mettre à jour une personne
	  * condition : retrouver la personne dans la base. La gestion des différences entre
	  * l'ancienne et la nouvelle valeur est laissée aux programmes clients
	  * @param Data_People $oldp est l'ancienne valeur de l'acteur à mettre à jour
	  * @param Data_People $p la nouvelle valeur
	  * @return boolean vrai si l'update a été effectuée
	  */
	public function updatePersonne(Data_People $oldp, Data_People $p) 
	 {
		 if ($this->estPeopleIncomplet($oldp) || $this->estPeopleIncomplet($p)) 
		 {
			 return false; 
		 }

		 $pid = $this->peopleToId($oldp);

		 if ($pid < 0)
		 {
			 return false;
		 }
		 /* UPDATE `acteursrealisateurs` SET `Nom` = 'Berléand',
		`Prenom` = 'François',
		`Nationalite` = 'FRANCAISE',
		`Naissance` = '1952',
		`Mort` = NULL WHERE `id` =17 LIMIT 1 ;
		  */

		 $mort=" Mort = NULL ";
		 $sexe=($p->isSexeFeminin())?'f':'m';
		 
		 if ($p->getMort() > 0) $mort = " Mort =\"".$p->getMort()."\" ";
		 $query = "UPDATE acteursrealisateurs SET Nom=\"".$p->getNom()."\", Prenom=\"".$p->getPrenom()."\", Nationalite=\"".$p->getNationalite()."\", Naissance=\"".$p->getNaissance()."\", ".$mort.", Sexe=\"".$sexe."\" WHERE id=\"".$pid."\"";
		 $result = false;
		 $result = mysql_query($query, $this->connexion);
		
		 return $result;
	 }
	 
}
?>