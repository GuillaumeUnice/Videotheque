<?php

//include constant("PATHTOROOT")."includes/connexion.inc";

/**
 * 
 * @author renevier-gonin
 * @package Nf_
 * 
 * @abstract la classe Nf_UserDvdManagement permet de lire, d'ajouter, de modifier ou de supprimer
 * des utilisateurs, les dvs qu'ils possèdes et les dvd qu'ils empruntes/rendent:
 * <code>
 * $lewis = Nf_UserDvdManagement::getInstance()->getUser("Lewis", "Raymond"); // on obtient l'utilisateur Raymond Lewis
 * // tous les dvds de Mr Lewis
 * $dvdsDeLewis = Nf_UserDvdManagement::getInstance()->getDvdsParUser($lewis);
 * // toutes les dvds empruntés par Mr Lewis
 * $dvdsEmpruntesParLewis = Nf_UserDvdManagement::getInstance()->getDvdsEmpruntes($lewis);
 * // on utilise Nf_UserDvdManagement::getInstance() mais on peut aussi stocké le singleton dans une variable
 * //  $usrdvdMgt = Nf_UserDvdManagement::getInstance();
 * // ...
 * </code>
 */
class Nf_UserDvdManagement {

	/**
	 * @ignore
	 * @var Nf_UserDvdManagement
	 */
	protected static $instance;
	/**
	 * @ignore
	 */
	protected $connexion;

	/**
	 *  @ignore
	 */
	protected function __construct()
	{
		$this->connexion = constant('CONNEXION');	
	}
	
	/**
	 * pour obtenir l'unique instance de Nf_UserDvdManagement
	 * @return Nf_UserDvdManagement
	 */
	public static function getInstance()
	{
		if (! Nf_UserDvdManagement::$instance) Nf_UserDvdManagement::$instance = new Nf_UserDvdManagement();
		
		return Nf_UserDvdManagement::$instance;
	}
	
	/**
	 *  @ignore
	 */
 	protected function estUserIncomplet(Data_User $u)
	 {
		 return ($u == null) ||($u->getNom() == null) ||
		 ($u->getPrenom() == null) ||($u->getNom() == "") ||
		 ($u->getPrenom() == "");
	 }
	 
	 
	 
	 /**
	  * // public car requise dans CommNoteManagment
	  * permet d'obtenir un utilisateur à partir d'un id
	  * @param int $id
	  * @return Data_User 
	  */
	public function idToUser($id) 
		{
		$query = "SELECT * FROM user where id=\"$id\"";
		$localResults =  mysql_query($query, $this->connexion);

		if (mysql_num_rows($localResults) >= 1)
		{
			//		 on prend le resultat
			$rep = mysql_fetch_array($localResults);
			$u = $this->fromCompleteRequestToUser($rep);

			return $u;
		}
		else return null;
		
	}

	/**
	 *  @ignore
	 */
	protected function fromCompleteRequestToUser($rep) 
	{
		$nu = $rep["Nom"];
		$pu = $rep["Prenom"];
		$du = $rep["inscription"];
		$mu = $rep["mdp"];
		
		$u = new Data_User($nu, $pu, $du, $mu);
		$u->setId($rep["id"]);
		return $u;
	}

	/**
	 *  @ignore
	 */
	protected function getUsersFromRequest($query) {
		$result = array();
		
			$localResults =  mysql_query($query, $this->connexion);

			//	on prend le resultat
			if ($localResults) while ($rep = mysql_fetch_array($localResults)) {
				$result[] = $this->fromCompleteRequestToUser($rep);
			}
		

		return $result;
	}
	
	
	/**
	 * permet de retrouver un dvd à partir de son id, s'il existe
	 * @param int $id
	 * @return Data_Dvd
	 */
	public function idToDvd($id)
	{
		$result = null;
		$query = "SELECT * FROM possede WHERE id=\"$id\"";
		$localResults = mysql_query($query, $this->connexion);

			if (mysql_num_rows($localResults))	
			{
				$rep = mysql_fetch_array($localResults);
				$f = Nf_FilmManagement::getInstance()->idToFilm($rep["film"]);
				if ($f)
				{
					$u = $this->idToUser($rep["user"]);
					if ($u)
					{
						$nbtot = $rep["nombretotal"];
						$nbdisp = $rep["nombredispo"];
						$date = $rep["dernierachat"];
						$result = new Data_Dvd($u, $f, $nbtot, $nbdisp, $date);
						$result->setId($rep["id"]);
					}
				}
				
			}

		return $result;
	}
	
	
	
/**
	 * permet de retrouver un emprunt à partir de son id, s'il existe
	 * @param int $id
	 * @return Data_Emprunt
	 */
	public function idToEmprunt($id)
	{
		$result = null;
		$query = "SELECT * FROM emprunte WHERE id=\"$id\"";
		$localResults = mysql_query($query, $this->connexion);

			if (mysql_num_rows($localResults))	
			{
				$rep = mysql_fetch_array($localResults);
				$dvd = $this->idToFilm($rep["supportfilm"]);
				if ($dvd)
				{
					$u = $this->idToUser($rep["user"]);
					if ($u)
					{
						$date = $rep["date"];
						$result = new Data_Emprunt($u, $dvd, $date);
						$result->setId($rep["id"]);
					}
				}
				
			}

		return $result;
	}
	
	
	
	 /**
	  *  // public car requise dans CommNoteManagment
	  *  permet d'obtenir un id à partir d'un utilisateur
	  * @param Data_User $user
	  * @return int l'id de l'utilisateur (ou -1)
	  */
	public function userToId(Data_User $user) {
		$query = "SELECT DISTINCT id FROM user where Nom=\"".$user->getNom()."\"";
		$query .= " AND Prenom = \"".$user->getPrenom()."\"";

		$userId = -1;

		
			$localResults =  mysql_query($query, $this->connexion);

			if (mysql_num_rows($localResults) >= 1)
			{
				$rep = mysql_fetch_array($localResults);
				$userId = $rep["id"];
			}


		return $userId; 
	}
	

		 
/***************************************************************************
	             ***************** Partie pour lire les données ************
	             *****************************************************************************/	 
	 
	
	
	/**
	 * pour obtenir la liste des utilisateurs de la vidéothèque dont on connait une partie du nom ou une partie du prénom
	 * s'il n'y a pas de parametre, renvoie tous les utilisateurs
	 * USER = auteur de commentaire
	 * @param string $nom : parametre optionel, par defaut à "" : un nom ou un prénom ou un morceau de nom ou de prénom
	 * @return array retourne tous les auteurs des commentaires dans un tableau ayant le nom "nom".
	 */
	public function getUsers($nom=""){
		if ($nom) $query = "SELECT * from user WHERE nom LIKE \"%$nom%\" OR prenom LIKE \"%$nom%\"";
		else $query = "SELECT * from user";
		return $this->getUsersFromRequest($query);
	}

	/**
	 * pour obtenir l'utilisateur de la vidéothèque dont on spécifie le prénom et le nom
	 * USER = auteur de commentaire
	 * @param string $nom : nom du User
	 * @param string $prenom : prenom du User
	 * @return Data_User retourne un utilisateur (le premier trouvé).
	 */
	public function getUser($nom, $prenom){
		$query = "SELECT * from user WHERE nom=\"$nom\" AND prenom=\"$prenom\"";
		$lu =  $this->getUsersFromRequest($query);
		$nbRep = count($lu);
		if ($nbRep >= 1) {
			// au moins une réponse
			return $lu[0];
		}
		else {
			// il n'y a pas de réponse
			return null;
		}
		

	}
	
	
	/**
	 * permet d'obtenir la liste de toutes les possessions de film pour un utilisateur
	 * Possession = quadruplet film (objet de la classe Data_Film), nbExemplaires (int), nbDisponibles (int) et le possesseur (objet de la classe Data_User) 
	 * @param Data_User $u est le possesseur des films 
	 * @return array retourne un tableau de toutes les possessions de l'utilisateur $u
	 */
	// mark
	public function getDvdsParUser(Data_User $u)
	{
		$result =  array();
		$uid = $this->userToId($u);

		if ($uid) {

			$query = "SELECT * FROM possede where user=\"$uid\"";
			$localResults = mysql_query($query, $this->connexion);

		if ($localResults)	while ($rep = mysql_fetch_array($localResults)) {
				$f = Nf_FilmManagement::getInstance()->idToFilm($rep["film"]);
				
				if ($f)
				{
					$nbtot = $rep["nombretotal"];
					$nbdisp = $rep["nombredispo"];
					$date = $rep["dernierachat"];
					$r = new Data_Dvd($u, $f, $nbtot, $nbdisp, $date);
					$r->setId($rep["id"]);
					
					$result[] = $r;
				}
				
				
		}
			


		}
		
		return $result;
	}
	
	
	/**
	 * permet de retrouver une ou les possessions d'un utilisateur (en particulier le nombre d'exemplaire et de dispo)
	 * @param Data_User $u la personne qui doit posseder le dvd
	 * @param Data_Film $f  le film recherche
	 * @return Data_Dvd ou null si l'utilisateur ne l'a pas, le Dvd (instance de la classe Data_Dvd) sinon
	 */
	public function getDvd(Data_User $u, Data_Film $f)
	{
		$result =  null;
		$uid =$this->userToId($u);
		$fid = Nf_FilmManagement::getInstance()->filmToId($f);

		if ($uid && $fid) {
			$query = "SELECT * FROM possede where film=\"$fid\" AND user=\"$uid\"";
			$localResults =  mysql_query($query, $this->connexion);

			if (mysql_num_rows($localResults) >= 1) {
				$rep = mysql_fetch_array($localResults);
				$nbtot = $rep["nombretotal"];
				$nbdisp = $rep["nombredispo"];	
				$date = $rep["dernierachat"];			
				$result= new Data_Dvd($u, $f, $nbtot, $nbdisp, $date);
				$result->setId($rep["id"]);
			}
		
		}

		
		return $result;
	}
	
	/**
	 * permet d'obtenir la liste de toutes les possessions de film de  tous les utilisateurs
	 * Possession = quadruplet film (objet de la classe Data_Film), nbExemplaires (int), nbDisponibles (int) et le possesseur (objet de la classe Data_User) 
	 * @return array retourne un tableau de toutes les possessions de lous les utilisateurs
	 */
	public function getDvds()
	{
		$result = array();

		
			$query = "SELECT * FROM possede ";
			$localResults = mysql_query($query, $this->connexion);

			if ($localResults)	while ($rep = mysql_fetch_array($localResults)) {
				$f = Nf_FilmManagement::getInstance()->idToFilm($rep["film"]);
				if ($f)
				{
					$u = $this->idToUser($rep["user"]);
					if ($u)
					{
						$nbtot = $rep["nombretotal"];
						$nbdisp = $rep["nombredispo"];
						$date = $rep["dernierachat"];
						$r = new Data_Dvd($u, $f, $nbtot, $nbdisp, $date);
						$r->setId($rep["id"]);
					
						$result[] = $r;
					}
				}
				
			}
			


		
		return $result;
	}

	
	/**
	 * permet d'obtenir la liste des possesseurs du film $f
	 * Possession = quadruplet film (objet de la classe Data_Film), nbExemplaires (int), nbDisponibles (int) et le possesseur (objet de la classe Data_User) 
	 * @param Data_Film $f est le film recherche 
	 * @return array retourne un tableau de toutes les possessions liees au film $f
	 */
	public function getDvdsParFilm(Data_Film $f)
	{
		$result = array();
		$fid = Nf_FilmManagement::getInstance()->filmToId($f);

		if ($fid) {

			$query = "SELECT * FROM possede where film=\"$fid\"";
			$localResults = mysql_query($query, $this->connexion);

			if ($localResults)	while ($rep = mysql_fetch_array($localResults)) {
				$u = $this->idToUser($rep["user"]);
				
				if ($u)
				{
					$nbtot = $rep["nombretotal"];
					$nbdisp = $rep["nombredispo"];
					$date = $rep["dernierachat"];
					$r = new Data_Dvd($u, $f, $nbtot, $nbdisp, $date);
					$r->setId($rep["id"]);
					
					$result[] = $r;
				}
			}
		}
		
		return $result;
	}
	
	
	
	
	/**
	 * pour obtenir la liste des films (dvd) qu'un utilisateur a emprunte
	 * @param Data_User $u : l'utilisateur qui emprunte
	 * @return array un tableau de dvd (qui peut etre vide)
	 */
	public function getDvdsEmpruntes(Data_User $u)
	{
		$result = array();

		$uid = $this->userToId($u);

		if ($uid) {

			$query = "SELECT * FROM emprunte where user=\"$uid\"";
			$localResults = mysql_query($query, $this->connexion);

			if ($localResults)	while ($rep = mysql_fetch_array($localResults)) {
				
				$dvdId = $rep["supportfilm"];
				$result[] = $this->idToDvd($dvdId);
			}
		}
		
		return $result;
	}
	
	
	/**
	 * 
	 * @param $nb
	 * @return 
	 */
	public function getDerniersEmprunts($nb = 5)
	{
		$n = (int) $nb;
		$query = "SELECT * FROM `emprunte` ORDER BY `emprunte`.`date` DESC, `emprunte`.`id` DESC LIMIT 0 , $n";
		
		
		$result = array();
        
		$localResults = mysql_query($query, $this->connexion);
        if($localResults)
            {
            while ($rep = mysql_fetch_array($localResults)) {
				$dvd = $this->idToDvd($rep["supportfilm"]);
				if ($dvd)
				{
					$u = $this->idToUser($rep["user"]);
					if ($u)
					{
						$date = $rep["date"];
						$emprunt = new Data_Emprunt($u, $dvd, $date);
						$emprunt->setId($rep["id"]);
						$result[] = $emprunt;
					}
				}
            	
              }
			}
        return $result;
	}
	
	
	
/**
	 * 
	 * @param $nb
	 * @return 
	 */
	public function getDerniersDvds($nb = 5)
	{
		$n = (int) $nb;
		$query = "SELECT * FROM `possede` ORDER BY `possede`.`dernierachat` DESC, `possede`.`id` DESC LIMIT 0 , $n";
		
		
		$result = array();
        
		$localResults = mysql_query($query, $this->connexion);
        if($localResults)
            {
            while ($rep = mysql_fetch_array($localResults)) {
            	$f = Nf_FilmManagement::getInstance()->idToFilm($rep["film"]);
				if ($f)
				{
					$u = $this->idToUser($rep["user"]);
					if ($u)
					{
						$nbtot = $rep["nombretotal"];
						$nbdisp = $rep["nombredispo"];
						$date = $rep["dernierachat"];
						$r = new Data_Dvd($u, $f, $nbtot, $nbdisp, $date);
						$r->setId($rep["id"]);
					
						$result[] = $r;
					}
				}
            	
              }
			}
        return $result;
	}
	
/***************************************************************************
	             ***************** Partie pour ajouter les données ************
	             *****************************************************************************/		
	
	/**
	  * pour entrer un nouvel utilisateur
	  * test sur nom et prenom, ajout automatique de l'annee.
	  * @param Data_User $user l'utilisateur à ajouter
	  * @return boolean vrai si l'ajout a été correct, faux sinon (attention à l'exception)
	  */
	 public function addUser(Data_User $user) 
	 {
		 if ( $this->estUserIncomplet($user) ) 
		 {
			 return false;
		 }
		 
		 /*
		  $u = $this->getUser($user->getNom(), $user->getPrenom());



		 if ($u != null) 
		 {
			 return false;
		 }
		*/
		 if ($user->getDate() <= 0) 
		 {
			 $user->setDate(date("Y"));
		 }

		$insert = "INSERT INTO user VALUES (id, \"".$user->getNom()."\", \"".$user->getPrenom()."\", \"".$user->getDate()."\", \"".$user->getMdp()."\")";
		return mysql_query($insert, $this->connexion);
	 }
	
	
	/**
	 * Pour prendre en compte l'achat d'un nouvel exemplaire d'un film
	 * Ajoute une nouvelle possession ou incremente une ancienne possession
	 * @param Data_User $u l'acheteur du film
	 * @param Data_Film $f le film achete
	 * @return boolean vrai si l'action a réussie, faux sinon
	 */
	public function buy(Data_User $u, Data_Film $f) 
	{
		$result = false;

		if ($this->estUserIncomplet($u))
		{
			return false;
		}

		if (Nf_FilmManagement::getInstance()->estFilmIncomplet($f))
		{
			return false;
		}

		$uid = $this->userToId($u);
		$fid = Nf_FilmManagement::getInstance()->filmToId($f);

		// d'abord la recherche


		if($uid && $fid) {
			$query = "SELECT * FROM possede where film=\"$fid\" AND user=\"$uid\"";
			
			$localResults =  mysql_query($query, $this->connexion);

			if (mysql_num_rows($localResults) >= 1)
			{
				$rep = mysql_fetch_array($localResults);
				
				$nbposs = $rep["nombretotal"]+1;
				$nbdispo = $rep["nombredispo"]+1;

				$id = $rep["id"];
				// dans d'un update
				$date=date("Y-m-d");
				$update = "UPDATE possede SET nombretotal =\"$nbposs\", nombredispo=\"$nbdispo\", `dernierachat` =\"$date\" WHERE id=\"$id\"";
				$result = mysql_query($update, $this->connexion);
			}
			else {
				// cas d'un insert
				$date=date("Y-m-d");
				$insert = "INSERT INTO possede VALUES (id, \"$fid\", \"$uid\", \"1\", \"1\", \"$date\")";
				$result = mysql_query($insert, $this->connexion);
			}
		}

		return $result;
	}
	 
	
	
    /**
	 * recherche le dvd à partir de $owner et $film
	 * et s'il le trouve revient a appeler borrowDvd($dvd, $borrower)
	 * @param Data_Film $film le film a emprunter
	 * @param Data_User $owner le possesseur du dvd
	 * @param Data_User $borrower l'emprunteur
	 * @return boolean vrai si l'emprunt a fonctionne, faux sinon
	 */
	public function borrowFilm(Data_Film $film, Data_User $owner, Data_User $borrower)
	{
		$dvd = $this->getDvd($owner, $film);
		
		if ($dvd) {
			return $this->borrowDvd($dvd, $borrower);
		}
		else return false;
	}
	
	
	/**
	 * vérifie que le dvd existe, qu'il est dispo et que l'utilisateur emprunter existe 
	 * et alors permet de l'emprunter.
	 * @param Data_Dvd dvd : le dvd a emprunter (mais les nb d'exemplaire ou dispo ne sont pas pris en compte)
	 * @param Data_User borrower : l'emprunter
	 * @return boolean vrai si l'emprunt a fonctionne, faux sinon
	 */
	public function borrowDvd(Data_Dvd $dvd, Data_User $borrower)
	{
		$result = false;
		$realDvd = $this->getDvd($dvd->getPossesseur(), $dvd->getFilm());   
		$buid = $this->userToId($borrower);
		
		
		if (($realDvd->hasId()) && ($realDvd->getNbDispo() > 0) && $buid)
			{
				// il faut faire un update de la possession pour le nbde dispo
				$update = "UPDATE possede SET  nombredispo=\"".($realDvd->getNbDispo()-1)."\" WHERE id=\"".$realDvd->getId()."\"";
				$result = mysql_query($update, $this->connexion);
				if ($result)
				{
					// a voir si c'est un nouvel emprunt 
					// on a besoin de la date :
					$date = date("Y-m-d");
					// il n'y a plus qu'a inserer
					$insert = "INSERT INTO emprunte VALUES (id, \"".$realDvd->getId()."\", \"$buid\", \"$date\")";
					$result = $result && mysql_query($insert, $this->connexion);
				}
			}
		
		return $result;
	}

/***************************************************************************
	             ***************** Partie pour supprimer les données ************
	             *****************************************************************************/	 
	 
	
	
	/**
	 * efface un utilisateur $u. <br />
	 * forcer a rendre les dvds qu'il a empruntes et ceux qu'il a pretes.<br />
	 * Avec le boolean $effacerToutesTraces, on precise si on veut effacer egalement
	 * ses notes, commentaires, possessions
	 * @param Data_User $u : l'utilateur a efface
	 * @param boolean effacerToutesTraces (optionnel, true par défaut) si vrai, alors on efface toutes les traces de l'utilisateur dans le systeme
	 * @return boolean vrai si l'utilisateur est effectivement effacé
	 */
	public function removeUser(Data_User $u, $effacerToutesTraces=true)
	{
		$result = true;
		$uid = $this->userToId($u);
		
		if ($uid) {

			$query ="";

			$this->returnEmpruntsParUser($u);
			if ($effacerToutesTraces)
			{
				// on efface les commentaires, les notes
				Nf_CommNoteManagement::getInstance()->removeCommentairesParUser($u);
				Nf_CommNoteManagement::getInstance()->removeNotesParUser($u) ;
				// on rend tous le dvds empruntes

				// suppression des possessions : 1) on obtient la liste des possessions
				$dvds = $this->getDvdsParUser($u);
				
				// suppression des possessions : 2) recuperation de ses films...
				foreach($dvds as $dvd )
				{
					$delete = "DELETE FROM `emprunte` WHERE supportfilm = \"".$dvd->getId()."\"";
					mysql_query($delete, $this->connexion) ;
				}
				// suppression des possessions : 3) suppression dans possede
				$delete = "DELETE FROM `possede` WHERE user = \"$uid\"" ;
				mysql_query($delete, $this->connexion) ;
			}

			$delete= "DELETE FROM `user` WHERE id = \"$uid\"" ;
			$result = mysql_query($delete, $this->connexion) ;
		}
		

		return $result;
	}
	
	
	
	/**
	 * permet de supprimer un des Dvd de la collection
	 * condition : il faut qu'il y ait assez de dvd et on ne peut pas enlever un dvd prete
	 * revient à appeler removeDvdParUserFilm($dvd->getPossesseur(), $dvd->getFilm());
	 * @param Data_Dvd $dvd est le film a enleve des possessions
	 * @return boolean vrai si la suppression a ete effective, faux sinon
	 */
	public  function removeDvd(Data_Dvd $dvd)
	{
		return $this->removeDvdParUserFilm($dvd->getPossesseur(), $dvd->getFilm());
	}

	/**
	 * permet de supprimer un des Dvd de la collection
	 * condition : il faut qu'il y ait assez de dvd et on ne peut pas enlever un dvd prete
	 * @param Data_User $oldOwner : l'ancien possesseur qui se separe d'un dvd
	 * @param Data_Film $f : le film sur le dvd
	 * @return boolean vrai si la suppression a ete effective, faux sinon
	 */
	public  function removeDvdParUserFilm(Data_User $oldOwner, Data_Film $f)
	{	 
		$result = false;
		// on commence par retrouver le dvd
		$realDvd = $this->getDvd($oldOwner, $f);  
		if ($realDvd)
		{
				// ensuite, soit on décroit les nombres, a condition... que le nombre soit suffisant
				if ($realDvd->getNbDispo() >= 1)
				{
					if ($realDvd->getNbExemplaires() > 1)
					{
						// c'est un update
						$update = "UPDATE possede SET nombretotal =\"".($realDvd->getNbExemplaires()-1)."\", nombredispo=\"".($realDvd->getNbDispo()-1)."\" WHERE id=\"{$realDvd->getId()}\"";
						$result = mysql_query($update, $this->connexion);
					}
					else
					{
						// c'est une suppression
						$delete = "DELETE FROM `possede` WHERE id=\"{$realDvd->getId()}\"";
						$result = mysql_query($delete, $this->connexion);
					}
				}
		}

		return $result;
	}
	
	
	/**
	 * fait rendre tous les emprunts sur un dvd  
	 * @param Data_Dvd $dvd : le dvd emprunte
	 * @return boolean
	 */
	public function returnEmpruntsParDvd(Data_Dvd $dvd)
	{
		$result = false;
		$realDvd = $this->getDvd($dvd->getPossesseur(), $dvd->getFilm()); 

		if ($realDvd) {

			// ensuite, on retrouve l'emprunt (le plus ancien)
			$query = "SELECT * FROM emprunte where supportfilm=\"{$realDvd->getId()}\"";
			$localResults = mysql_query($query, $this->connexion);

			$result =$localResults;
			if ($localResults) while ($rep = mysql_fetch_array($localResults)) {
				// il pourrait y avoir plusieurs emprunts... on en elimine un
				$borrowId = $rep["id"];
				// le remove se fait a la fin

				// on efface l'emprunt
				$delete = "DELETE FROM `emprunte` WHERE id=\"$borrowId\"";
				$result = $result && mysql_query($delete, $this->connexion);
				
				$realDvd->setNbDispo($realDvd->getNbDispo()+1);

				
			}
			// on update la possession
			$update = "UPDATE possede SET  nombredispo=\"{$realDvd->getNbDispo()}\" WHERE id=\"{$realDvd->getId()}\"";
			$result = $result && mysql_query($update, $this->connexion);
		}
		return $result;
	}

	/**
	 * fait rendre l'emprunt le plus vieux 
	 * @param Data_Dvd $dvd : le dvd emprunte
	 * @param Data_User $borrower : l'emprunteur
	 * @return boolean vrai si un retour d'emprunt a ete fait, faux sinon
	 */
	
	public function returnDvd(Data_Dvd $dvd, Data_User $borrower)
	{
		$result = false;

		$realDvd = $this->getDvd($dvd->getPossesseur(), $dvd->getFilm()); 
		$buid = $this->userToId($borrower);
		
		if ($buid && $realDvd) {

			// il faut retrouver l'emprunt (le premier)
			// puis l'effacer 
			// puis incrementer nbDispo dans possede...


			// on retrouve l'emprunt (le plus ancien)
			$query = "SELECT * FROM emprunte where supportfilm=\"{$realDvd->getId()}\" AND user=\"$buid\" ORDER BY date";
			$localResults = mysql_query($query, $this->connexion);

			$borrowId = -1; 
			if ($localResults) {
				// il pourrait y avoir plusieurs emprunts... on en elimine un
				$rep = mysql_fetch_array($localResults);
				$borrowId = $rep["id"];
				// le remove se fait a la fin
			}
			

			
			$result = ($borrowId > 0);

			if ($result)
			{
				// on efface l'emprunt
				$delete = "DELETE FROM `emprunte` WHERE id=\"$borrowId\"";
				$result = $result &&  mysql_query($delete, $this->connexion);

				// on update la possession
				$update = "UPDATE possede SET nombredispo=\"".($realDvd->getNbDispo()+1)."\" WHERE id=\"{$realDvd->getId()}\"";
				$result = $result && mysql_query($update, $this->connexion);
			}
		}
		
		return $result;
	}

	
	
	
    /**
     * permet de faire rendre tous les emprunts d'un utilisateur passé en parametre
     * @param Data_User $borrower : celui qui rend tous les dvds qu'il a empruntés.
     * @return boolean
     */
	public function returnEmpruntsParUser(Data_User $borrower)
	{
		$result = false;
		$buid = $this->userToId($borrower);
		if ($buid) {

			$query = "SELECT * from emprunte where user=\"$buid\"";
			$localResults = mysql_query($query, $this->connexion);
			$result = (mysql_num_rows($localResults) > 0);
			if ($result) while ($rep = mysql_fetch_array($localResults)) {
				$dvdId = $rep["supportfilm"];
				$borrowId = $rep["id"];
				
				$realDvd = $this->idToDvd($dvdId);
				$update = "UPDATE possede SET nombredispo=\"".($realDvd->getNbDispo()+1)."\" WHERE id=\"{$realDvd->getId()}\"";
				$result = $result && mysql_query($update, $this->connexion);
				// on efface l'emprunt
				$delete = "DELETE FROM `emprunte` WHERE id=\"$borrowId\"";
				$result = $result &&  mysql_query($delete, $this->connexion);
			}
		}
		return $result;
	}

/***************************************************************************
	             ***************** Partie pour mettre à jour les données ************
	             *****************************************************************************/	 
	
	
    /**
	 * permet de donner un (des) dvd (dont le possesseur est dans $dvd->possesseur) a une 
	 * nouvelle personne ($newOwner)
	 * s'il y a des dvds prêtés, alors il faudra les rendre au nouveau propriétaire
	 * @param Data_Dvd $dvd que l'on veut donner
	 * @param Data_User $newOwner le nouveau proprietaire
	 * @return boolean vrai si le don a marche, faux sinon
	 */
	public  function giveDvd(Data_Dvd $dvd, Data_User $newOwner)
	{
		$result = false;

		$realDvd = $this->getDvd($dvd->getPossesseur(), $dvd->getFilm()); 
		$nuid = $this->userToId($newOwner);
		
		if($realDvd && $nuid)
		{
			$update = "UPDATE `possede` SET `user` = '$nuid' WHERE `id` ={$realDvd->getId()};";
			if (mysql_query($update, $this->connexion)) $result = true; // pour ne pas renvoyer une resource
		}

		return $result;
	}
	
	

	/**
	 * pour changer de mot de passe
	 * @param Data_User $u l'utilisateur qui veut changer de mot de passe, le nouveau mot de passe est disponible via $u->getMdp()
	 * @return boolean : vrai en cas de succès dans la mise à jour
	 */
	public  function updateMDP(Data_User $u)
	{
		$result = false;
		$uid = $this->userToId($u);
		
		if($uid)
		{
			$update = "UPDATE `user` SET `mdp` = '{$u->getMdp()}' WHERE `id` =$uid;";
			if (mysql_query($update, $this->connexion)) $result = true; // pour ne pas renvoyer une resource
		}

		return $result;
	}
}
	
?>