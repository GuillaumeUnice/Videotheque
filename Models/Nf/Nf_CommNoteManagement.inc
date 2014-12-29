<?php

//include constant("PATHTOROOT")."includes/connexion.inc";

/**
 * 
 * @author renevier-gonin
 * @package Nf_
 * 
 * @abstract la classe Nf_CommNoteManagement permet de lire, d'ajouter, de modifier ou de supprimer
 * des notes et des commentaires faites par les utilisateurs sur les films dans la BD. Elle s'utilise :
 * <code>
 * $lewis = Nf_UserDvdManagement::getInstance()->getUser("Lewis", "Raymond"); // on obtient l'utilisateur Raymond Lewis
 * // tous les commentaires de Mr Lewis
 * $comms = Nf_CommNoteManagement::getInstance()->getCommentairesParUser($lewis);
 * // toutes les notes de Mr Lewis
 * $notes = Nf_CommNoteManagement::getInstance()->getNotesParuser($lewis);
 * // on utilise Nf_CommNoteManagement::getInstance() mais on peut aussi stocké le singleton dans une variable
 * //  $notcommMgt = Nf_CommNoteManagement::getInstance();
 * // ...
 * </code>
 */
class Nf_CommNoteManagement {

	/**
	 * @ignore
	 * @var Nf_CommNoteManagement
	 */
	protected static $instance;
	/**
	 * @ignore
	 */
	protected $connexion;

	/**
	 * @ignore
	 */
	protected function __construct()
	{
		$this->connexion = constant('CONNEXION');
	}

	/**
	 * pour obtenir l'unique instance de Nf_CommNoteManagement
	 * @return Nf_CommNoteManagement
	 */
	public static function getInstance()
	{
		if (! Nf_CommNoteManagement::$instance) Nf_CommNoteManagement::$instance = new Nf_CommNoteManagement();

		return Nf_CommNoteManagement::$instance;
	}



		
	/***************************************************************************
	 ***************** Partie pour lire les données ************
	 *****************************************************************************/

	/**
	 * Cette méthode permet d'obtenir un Data_Commentaire à partir d'un id<br />
	 * méthode publique pour rendre les échanges via formulaire plus simple
	 * 
	 * @param int $id l'id dans la bd
	 * @return Data_Commentaire : le commenentaire sous forme d'une Data_Commentaire correspondant à l'id... ou null si le commentaire n'existe pas
	 */
	public function idToCommentaire($id) 
	{
		$query = "SELECT * FROM commente where id=\"$id\"";
		$localResults =  mysql_query($query, $this->connexion);
		
		$r = null;
		// on extrait un realisateur
		if (mysql_num_rows($localResults) == 1)
		{
			$rep = mysql_fetch_array($localResults);
			
			$film = Nf_FilmManagement::getInstance()->idToFilm($rep["film"]);
             	$auteur = Nf_UserDvdManagement::getInstance()->idToUser($rep["user"]);

				if ($auteur && $film)
				{
					$r = new Data_Commentaire($auteur, $film, $rep["commentaire"], $rep["date"]);
					$r->setId($rep["id"]);
				}
		}

		return $r;
	}
	
	
	/**
	 * Cette méthode permet d'obtenir un Data_Note à partir d'un id<br />
	 * méthode publique pour rendre les échanges via formulaire plus simple
	 * 
	 * @param int $id l'id dans la bd
	 * @return Data_Note : la note sous forme d'une Data_Note correspondant à l'id... ou null si la note n'existe pas
	 */
	public function idToNote($id) 
	{
		$query = "SELECT * FROM note where id=\"$id\"";
		$localResults =  mysql_query($query, $this->connexion);
		
		$r = null;
		// on extrait un realisateur
		if (mysql_num_rows($localResults) == 1)
		{
			$rep = mysql_fetch_array($localResults);
			
			$film = Nf_FilmManagement::getInstance()->idToFilm($rep["film"]);
             	$auteur = Nf_UserDvdManagement::getInstance()->idToUser($rep["user"]);

				if ($auteur && $film)
				{
					$r = new Data_Note($auteur, $film, $rep["note"], $rep["date"]);
					$r->setId($rep["id"]);
				}
		}

		return $r;
	}
	
	
	/**
	 * permet d'obtenir les derniers commentaires (modifiés ou créés)
	 * @param int $nb : nombre de commentaires voulu... par défaut 5
	 * @return Array : un tableau de Data_Commentaire
	 */
	public function getDerniersCommentaires($nb = 5)
	{
		$n = (int) $nb;
		$query = "SELECT * FROM `commente` ORDER BY `commente`.`date` DESC LIMIT 0 , $n";		
		
		$result = array();
        
		$localResults = mysql_query($query, $this->connexion);
        if($localResults)
            {
            while ($rep = mysql_fetch_array($localResults)) {
              	// pour en prendre un sur deux...          	
              	$film = Nf_FilmManagement::getInstance()->idToFilm($rep["film"]);
             	$auteur = Nf_UserDvdManagement::getInstance()->idToUser($rep["user"]);

				if ($auteur && $film)
				{
					$comm = new Data_Commentaire($auteur, $film, $rep["commentaire"], $rep["date"]);
					$comm->setId($rep["id"]);
					$result[] = $comm;
				}
            	
              }
			}
        return $result;
	}
	
	
	/**
	 * permet d'obtenir les derniers notes (modifiées ou créées)
	 * @param int $nb : nombre de notes voulues... par défaut 5
	 * @return Array : un tableau de Data_Note
	 */
	public function getDernieresNotes($nb = 5)
	{
		$n = (int) $nb;
		$query = "SELECT * FROM `note` ORDER BY `note`.`date` DESC LIMIT 0 , $n";
		
		
		$result = array();
        
		$localResults = mysql_query($query, $this->connexion);
        if($localResults)
            {
            while ($rep = mysql_fetch_array($localResults)) {
              	// pour en prendre un sur deux...          	
              	$film = Nf_FilmManagement::getInstance()->idToFilm($rep["film"]);
             	$auteur = Nf_UserDvdManagement::getInstance()->idToUser($rep["user"]);

				if ($auteur && $film)
				{
					$note = new Data_Note($auteur, $film, $rep["note"], $rep["date"]);
					$note->setId($rep["id"]);
					$result[] = $note;
				}
            	
              }
			}
        return $result;
	}

	/**
	 * permet d'obtenir la liste de tous les commentaires sur le film donné (faits par un utilisateur )
	 * commentaire = triplet Film, User, Chaine de caractères
	 * @param Data_Film $f est le film dont on veut tous les commentaires
	 * @param Data_User $u est l'auteur des commentaires qui sont recherchés - optionnel, null par defaut
	 * @return array un tableau des commentaires sur le film $f (fait par $u )
	 */
	public function getCommentairesParFilm(Data_Film $f, Data_User $u=null)
	{
		$result = array();


		$fid = Nf_FilmManagement::getInstance()->filmToId($f);
		if ($u) $uid = Nf_UserDvdManagement::getInstance()->userToId($u);


		$query = "SELECT commente.id, commente.user, commente.film, commente.commentaire, commente.date FROM commente JOIN user ON commente.user = user.id ";
		$query .= "WHERE commente.film=\"$fid\"" ;
		if ($u) $query .= "AND commente.user=\"$uid\"";

		$localResults = mysql_query($query, $this->connexion);

		if ($localResults)	while ($rep = mysql_fetch_array($localResults)) {
			$auteur = null;
			if ($u) $auteur = $u;
			else $auteur = Nf_UserDvdManagement::getInstance()->idToUser($rep["user"]);

			if ($auteur)
			{
				$comm = new Data_Commentaire($auteur, $f, $rep["commentaire"], $rep["date"]);
				$comm->setId($rep["id"]);
				$result[] = $comm;
			}
		}


		return $result;

	}

	/**
	 * permet d'obtenir la liste de tous les commentaires faits par un utilisateur
	 * commentaire = triplet Film, User, Chaine de caractères
	 * @param Data_User $u est l'auteur des commentaires qui sont recherchés
	 * @return array retourne un tableau de tous les commentaires fait par l'utilisateur $u
	 */
	public function getCommentairesParUser(Data_User $u)
	{
		$result = array();
		$uid = Nf_UserDvdManagement::getInstance()->userToId($u);

		if ($uid)
		{
				

			$query = "SELECT commente.id, commente.user, commente.film, commente.commentaire, commente.date FROM commente where user=\"$uid\"";
			$localResults = mysql_query($query, $this->connexion);

			if ($localResults)	while ($rep = mysql_fetch_array($localResults)) {
				$f = Nf_FilmManagement::getInstance()->idToFilm($rep["film"]);
				if ($f)
				{
					$comm = new Data_Commentaire($u, $f, $rep["commentaire"], $rep["date"]);
					$comm->setId($rep["id"]);
					$result[] = $comm;
				}
			}


		}


		return $result;
	}



	/**
	 * permet d'obtenir un tableau de toutes les notes sur le film donné ( faites par un utilisateur )
	 * Data_Note = triplet film (objet de la classe Data_Film), auteur (objet de la classe Data_User) et la note (int)
	 * @param Data_Film $f est le film dont on veut toutes les notes
	 * @param Data_User $u (optionel) est l'auteur des notes qui sont recherchées
	 * @return array un tableau des notes  sur le film $f (faites par $u)
	 */
	public function getNotesParFilm(Data_Film $f, Data_User $u=null)
	{
		$result = array();


		$fid = Nf_FilmManagement::getInstance()->filmToId($f);
		if ($u) $uid = Nf_UserDvdManagement::getInstance()->userToId($u);

		$query = "SELECT note.id, note.film, note.user, note.note, note.date FROM note  ";
		$query .= " WHERE note.film=\"$fid\" ";
		if ($u) $query .= "AND note.user=\"$uid\"";

		$localResults = mysql_query($query, $this->connexion);

		if ($localResults)	while ($rep = mysql_fetch_array($localResults)) {
			$auteur = null;
			if ($u) $auteur = $u;
			else $auteur = Nf_UserDvdManagement::getInstance()->idToUser($rep["user"]);
			if ($auteur)
			{
				$n = new Data_Note($auteur, $f, $rep["note"], $rep["date"]);
				$n->setId($rep["id"]);	

				$result[] = $n;
			}
		}


		return $result;
	}

	/**
	 * permet d'obtenir la liste de toutes les notes faites par un utilisateur
	 * Data_Note = triplet film (objet de la classe Data_Film), auteur (objet de la classe Data_User) et la note (int)
	 * @param Data_Use$u est l'auteur des notes qui sont recherchées
	 * @return array retourne un tableau de toutes les notes fait par l'utilisateur $u
	 */
	public function getNotesParUser(Data_User $u)
	{
		$result = array();
		$uid = Nf_UserDvdManagement::getInstance()->userToId($u);

		if ($uid)
		{

			$query = "SELECT * FROM note where user=\"$uid\"";
			$localResults = mysql_query($query, $this->connexion);

			if ($localResults)	while ($rep = mysql_fetch_array($localResults)) {
				$f = Nf_FilmManagement::getInstance()->idToFilm($rep["film"]);
				if ($f)
				{
					$n = new Data_Note($u, $f, $rep["note"], $rep["date"]);
					$n->setId($rep["id"]);
					$result[] = $n;
				}
			}

		}
		return $result;
	}


	/**
	 * permet d'obtenir la derniere note faite par un utilisateur sur le film donné
	 * Data_Note = triplet film (objet de la classe Data_Film), auteur (objet de la classe Data_User) et la note (int)
	 * @param Data_Film $f est le film dont on veut la derniere notes
	 * @param Data_User $u est l'auteur de la note qui sont recherchée
	 * @return Data_Note la derniere note fait par $u sur le film $f
	 */
	public function getDerniereNote(Data_Film $f, Data_User $u)
	{
		$result = null;


		$fid = Nf_FilmManagement::getInstance()->filmToId($f);
		$uid = Nf_UserDvdManagement::getInstance()->userToId($u);
			
		if ($fid && $uid)
		{
			$query = "SELECT * FROM note  ";
			$query .= " WHERE note.film=\"$fid\" AND note.user=\"$uid\" ORDER BY note.id DESC";

			$localResults = mysql_query($query, $this->connexion);

			if (mysql_num_rows($localResults)) {
				$rep = mysql_fetch_array($localResults);
				$result = new Data_Note($u, $f, $rep["note"], $rep["date"]);
				$result->setId($rep["id"]);
			}

		}

		return $result;
	}
	
	
	
		/***************************************************************************
	 ***************** Partie pour ajouter les données ************
	 *****************************************************************************/

	 /**
	 * @ignore
	 */
	private function estNoteIncomplet(Data_Note $note)
	 {
		 $n = $note->getNote();
		 return ($note == null) || ($note->film == null) || ($note->auteur == null)  
		 || ($n < 0) || ($n > 10);
	 }
	
	/**
	  * Pour ajouter une nouvelle note
	  * Data_Note = triplet film (objet de la classe Data_Film), auteur (objet de la classe Data_User) et la note (int)
	  * @param Data_Note $note la note a ajouter
	  * @return boolean vrai si l'action a réussie, faux sinon (déjà présent dans la base ou note incomplete )
	  */
	 public function addNote(Data_Note $note) 
	 {
		 if ($this->estNoteIncomplet($note))
		 {
			 return false;
		 }

		 
		 $fid = Nf_FilmManagement::getInstance()->filmToId($note->film);
		 $uid = Nf_UserDvdManagement::getInstance()->userToId($note->auteur);

		 if ($fid && $uid)
		 {
		 	$date = date("Y-m-d H:i:s");
		 	$insert = "INSERT INTO note VALUES (id, \"$fid\", \"$uid\", \"".$note->getNote()."\", \"$date\")";
			 return mysql_query($insert, $this->connexion);
		 }
		else return false;	
		 
	 }
	
	
	 /**
	 * @ignore
	 */
	private function estCommIncomplet(Data_Commentaire $comm)
	{
		return ($comm == null) || ($comm->getFilm() == null) || ($comm->getAuteur() == null)  
		|| ($comm->getCommentaire() == null) || ($comm->getCommentaire() == "");
	}
	/**
	 * Pour ajouter un nouveau commentaire
	 * commentaire = triplet Data_Film, Data_User, Chaine de caractères (tout doit être renseigné)
	 * @param Data_Commentaire $comm
	 * @return boolean vrai si l'action a réussie, faux sinon (déjà présent dans la base)
	 */
	public function addCommentaire(Data_Commentaire $comm) 
	{
		if ($this->estCommIncomplet($comm))
		{
			return false;
		}

		$fid = Nf_FilmManagement::getInstance()->filmToId($comm->getFilm());
		$uid = Nf_UserDvdManagement::getInstance()->userToId($comm->getAuteur());

		$txt = str_replace("\"", "'", $comm->getCommentaire());

		 if ($fid && $uid)
		 {
		 	$date = date("Y-m-d H:i:s");
			$insert = "INSERT INTO commente VALUES (id, \"$fid\", \"$uid\", \"$txt\", \"$date\")";
			 return mysql_query($insert, $this->connexion);
		 }
		else return false;
	}
	 

/***************************************************************************
	 ***************** Partie pour supprimer les données ************
	 *****************************************************************************/
	
	/**
	 * Pour effacer un commentaire (et tous ceux qui lui ressemble i.e. même auteur, même film, même texte)
	 * @param Data_Commentaire $c : le commentaire a effacer
	 * @return boolean vrai si cela a fonctionne, faux sinon
	 */
	public function removeCommentaire(Data_Commentaire $comm)
	{
		$result = false;

		$fid = Nf_FilmManagement::getInstance()->filmToId($comm->getFilm());
		$uid = Nf_UserDvdManagement::getInstance()->userToId($comm->getAuteur());
		$txt = str_replace("\"", "'", $comm->getCommentaire());
		
		if ($fid && $uid) {
			$remove = "DELETE FROM `commente` WHERE film = \"$fid\" AND user = \"$uid\" AND commentaire=\"$txt\"";
			$result = mysql_query($remove, $this->connexion);
		}
		

		return $result;
	}
	
	
	/**
	 * permet d'effacer tous les commentaires d'un utilisateur sur un film donne
	 * @param Data_User $u : l'utilisateur dont on veut supprimer tous les commentaires (sur un film)
	 * @param Data_Film $f : le film (optionel) dont on veut supprimer tous les commentaires d'un utilisateur
	 * @return boolean  vrai si cela a fonctionne, faux sinon
	 */
	public function removeCommentairesParUser(Data_User $u, Data_Film $f=null)
	{
		$result = false;

		$uid = Nf_UserDvdManagement::getInstance()->userToId($u);

		if ($uid)
		{
			$remove = "DELETE FROM `commente` WHERE  user = \"$uid\"";
			
			if ($f) 
			{
				$fid = Nf_FilmManagement::getInstance()->filmToId($f);
				if ($fid) $remove .= " AND film = \"$fid\" ";
			}

			$result = mysql_query($remove, $this->connexion);
		}
		
		
		return $result;
	}
	
	
	/**
	 * permet d'effacer tous les commentaires sur un film donne
	 * @param Data_Film $f : le film dont on veut supprimer tous les commentaires 
	 * @return boolean  vrai si cela a fonctionne, faux sinon
	 */
	public function removeCommentairesParFilm(Data_Film $f)
	{
		$result = false;

		$fid = Nf_FilmManagement::getInstance()->filmToId($f);
		
		if ($fid) {
			$remove = "DELETE FROM `commente` WHERE film = \"$fid\"";
			$result = mysql_query($remove, $this->connexion);
		}
		
		return $result;
	}
	
	
	
    /**
	 * Pour effacer une note (ou plusieurs s'il y en a plusieurs avec la meme valeur) 
	 * @param Data_Note $note : la note a effacer
	 * @return boolean vrai si cela a fonctionne, faux sinon
	 */
	public function removeNote(Data_Note $note)
	{
		$result = false;

		$fid = Nf_FilmManagement::getInstance()->filmToId($note->film);
		$uid = Nf_UserDvdManagement::getInstance()->userToId($note->auteur);
		$n =  $note->getNote();
		
		if ($fid && $uid) {
			$remove = "DELETE FROM `note` WHERE film = \"$fid\" AND user = \"$uid\" AND note=\"$n\"";
			$result = mysql_query($remove, $this->connexion);
		}
		

		return $result;
	}
	
	
	/**
	 * permet d'effacer tous les notes d'un utilisateur (sur un film donne)
	 * @param Data_User $u : l'utilisateur dont on veut supprimer toutes les notes (sur un film)
	 * @param Data_Film $f : le film (optionel) dont on veut supprimer toutes les notes d'un utilisateur
	 * @return boolean vrai si cela a fonctionne, faux sinon
	 */
	public function removeNotesParUser(Data_User $u, Data_Film $f=null)
	{
		$result = false;

		$uid = Nf_UserDvdManagement::getInstance()->userToId($u);

		if ($uid)
		{
			$remove = "DELETE FROM `note` WHERE  user = \"$uid\"";
			
			if ($f) 
			{
				$fid = Nf_FilmManagement::getInstance()->filmToId($f);
				if ($fid) $remove .= " AND film = \"$fid\" ";
			}

			$result = mysql_query($remove, $this->connexion);
		}
				
		return $result;
	}
	
	
	/**
	 * permet d'effacer toutes les notes sur un film donne
	 * @param Data_Film $f : le film dont on veut supprimer toutes les notes 
	 * @return boolean vrai si cela a fonctionne, faux sinon
	 */
	public function removeNotesParFilm(Data_Film $f)
	{
		$result = false;

		$fid = Nf_FilmManagement::getInstance()->filmToId($f);
		
		if ($fid) {
			$remove = "DELETE FROM `note` WHERE film = \"$fid\"";
			$result = mysql_query($remove, $this->connexion);
		}
		
		return $result;
	}
	
	/**
	 * pour effacer la derniere note entree par $u sur $f	  
	 * Note = triplet film (objet de la classe Film), auteur (objet de la classe User) et la note (int)
	 * @param Data_User $u : l'utilisateur auteur de la note
	 * @param Data_Film $f : le film note
	 * @return boolean vrai si on a pu effacer la note, faux sinon
	 */
	public function removeDerniereNote(Data_User $u, Data_Film $f)
	{
		$result = false;

		$uid = Nf_UserDvdManagement::getInstance()->userToId($u);
		$fid = Nf_FilmManagement::getInstance()->filmToId($f);
		

		if ($uid && $fid) {
			
			$query = "SELECT * FROM `note` WHERE film = \"$fid\" AND user = \"$uid\" ORDER BY id DESC";

			$localResults =  mysql_query($query, $this->connexion);

			if (mysql_num_rows($localResults)) {
				$rep = mysql_fetch_array($localResults);
				$noteId = $rep["id"];
				
				if ($noteId > 0) 
				{
					$remove = "DELETE FROM note WHERE id=\"$noteId\"";
					$result = mysql_query($remove, $this->connexion);
				}
			}
		}
		
		return $result;
	}

/***************************************************************************
	 ***************** Partie pour mettre a jour les données ************
	 *****************************************************************************/
	
	/**
	 * mettre a jour la derniere note... ou inserer une note s'il n'y pas encore de note de l'utilsateur
	 *  Data_Note = triplet film (objet de la classe Data_Film), auteur (objet de la classe  Data_User) et la note (int)
	 * @param  Data_Note n : la nouvelle note
	 * @return boolean vrai si la mise a jour a ete effective, faux sinon
	 */
	public function updateDerniereNote( Data_Note $n)
	{
		$result = false;


		$uid = Nf_UserDvdManagement::getInstance()->userToId($n->auteur);
		$fid = Nf_FilmManagement::getInstance()->filmToId($n->film);
		

		if ($uid && $fid && ($n->getNote() > 0) ) {
			
			$query = "SELECT * FROM `note` WHERE film = \"$fid\" AND user = \"$uid\" ORDER BY id DESC";

			$localResults =  mysql_query($query, $this->connexion);
			$noteId = -1;
			if (mysql_num_rows($localResults)) {
				$rep = mysql_fetch_array($localResults);
				$noteId = $rep["id"];
			}

			if ($noteId <= 0) $result = $this->addNote($n);
			else
			{
				$date=date("Y-m-d H:i:s");
				$update = "UPDATE note SET  `note` =\"".$n->getNote()."\", `date` =\"$date\" WHERE id=\"$noteId\"";
				$result = mysql_query($update, $this->connexion);
			}
		}
		

		return $result;
	}	
	
	
	/**
	 * pour mettre à jour un commentaire
	 * restriction : il doit déjà exister et seul le texte peut changer
	 * le vieux paramètre est en paramètre, car il peut y avoir plusieurs commentaires 
	 * d'un utilisateur par film
	 * @param Data_Commentaire $oldc l'ancienne valeur du commentaire
	 * @param Data_Commentaire $c la nouvelle valeur
	 * @return boolean vrai si l'update a été effectuée
	 */
	public function updateCommentaire(Data_Commentaire $oldc, Data_Commentaire $c) 
	{
		if ($this->estCommIncomplet($c) || $this->estCommIncomplet($oldc) )
		{
			return false;
		}
		// verification d'egalite de user
		if (! $oldc->getAuteur()->equals($c->getAuteur())) 
		{
			return false;
		}
		// verification d'egalite de film
		if (! $oldc->getFilm()->equals($c->getFilm()))
		{
			return false;
		}	

		$result = false;

		$uid = Nf_UserDvdManagement::getInstance()->userToId($c->getAuteur());
		$fid = Nf_FilmManagement::getInstance()->filmToId($c->getFilm());
		$cid = -1;

		$oldtxt =  str_replace("\"", "'", $oldc->getCommentaire());
		$txt = str_replace("\"", "'", $c->getCommentaire());

		$query = "SELECT * FROM commente ";
		$query .= "WHERE commente.film=\"$fid\" AND commente.user=\"$uid\" AND commente.commentaire=\"$oldtxt\"";

		if ($uid && $fid) {
			$localResults =  mysql_query($query, $this->connexion);

			// normalement résultat unique
			if (mysql_num_rows($localResults)) {
				$rep = mysql_fetch_array($localResults);
				$cid = $rep["id"];
			}

			if ($cid <=0) return false ;

			
			// todo : l'update
			$date=date("Y-m-d H:i:s");
			$update = "UPDATE commente SET `commentaire` =\"$txt\", `date` =\"$date\" WHERE id=\"$cid\" LIMIT 1";
			$result = mysql_query($update, $this->connexion);
		}
		return $result;
	}
	
	
	
	
}
?>