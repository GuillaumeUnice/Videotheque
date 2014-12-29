<?php

/**
 * 
 * @author renevier-gonin
 * @package Data_
 * 
 * @abstract cette classe représente un utilisateur de la vidéothèque
 * pour les noms et prénoms, certains caractères sont interdits : `!@#$%^&*()=+{}|\:;"/?,|
 * les noms et prénoms constituent l'identificateur de l'utilisateur
 */
class Data_User {
	/**
	 * le nom de l'utilisateur
	 * @var string
	 */
	protected $nom;
	/**
	 * le prenom de l'utilisateur
	 * @var string
	 */
	protected $prenom;
	/**
	 * l'année d'inscription
	 * @var int
	 */
	protected $date; // année d'enregistrement
	/**
	 * le mot de passe
	 * @var string
	 */
	protected $mdp;
	
	/**
	 * 
	 * @ignore
	 */
		protected $id = -1;
	
	/**
	 * 
	 * @ignore
	 */
	protected static $filtre = array("`","!","@","#","$","%","^","&","*","(",")","=","+","{","}","|","\\",":",";","\"","/","?",",","|");
	// protected static $filtredChar = "`!@#$%^&*()=+{}|\\:;\"/?,|";
	
	/**
	 * constructeur, avec des parametres par defauts... non obligatoires...
	 * @param string $nom (par defaut "")
	 * @param string $prenom (par defaut "")
	 * @param int $date (par defaut -1... donc pas affecte)
	 * @param string $mdp (par defaut "")
	 */
	public function __construct($nom ="", $prenom = "" , $date = -1, $mdp ="")
	{
		if ($nom) $this->setNom($nom);
		if ($prenom) $this->setPrenom($prenom);
		if ($date) $this->setDate($date);
		if ($mdp) $this->setMdp($mdp);
	}
	
	
	/**
	 * @return int l'annee d'inscription
	 */
	public function getDate() {
		return $this->date;
	}
	/**
	 * 
	 * @param int $date : l'annee d'inscription
	 */
	public function setDate( $date) {
		if (is_int($date) && ($date > 0) ) $this->date = $date;
		else
			{
				$cast = (int) $date;
				if ($cast > 0) $this->date  = $cast;
			}
	}
	/**
	 * @return string le nom de l'utilisateur
	 */
	public function getNom() {
		return stripslashes($this->nom);
	}
	/**
	 * filtre les !@#$%^&*()=+{}|\:;"/?,| et les espaces du debut et de fin
	 * @param string $nom : pour changer le nom de l'utilisateur
	 * @return string le nom affecte (ou l'ancien)
	 */
	public function setNom( $nom) {
		// on empeche les `!@#$%^&*()=+{}|\:;"/?,| 
		$tmp = str_replace(self::$filtre, "", $nom);
		// les blancs inutiles
		$tmp = trim($tmp);
		$tmp = addslashes($tmp);
		if ($tmp) $this->nom = $tmp;
		
		return $this->getNom();
	}
	
	
	/**
	 * 
	 * @param string $mdp : pour changer le mot de passe de l'utilisateur
	 */
	public function setMdp( $mdp) {
		$this->mdp = $mdp;
	}
	
	/**
	 *  @return string le mote de passe de l'utilisateur
	 */
	public function getMdp() {
		return $this->mdp;
	}
	/**
	 *  @return string le prenom de l'utilisateur
	 */
	public function getPrenom() {
		return stripslashes($this->prenom);
	}
	/**
	 * filtre les !@#$%^&*()=+{}|\:;"/?,| et les espaces du debut et de fin
	 * @param string $prenom : pour changer le prenom de l'utilisateur
	 * @return string  le nom affecte (ou l'ancien)
	 */
	public function setPrenom( $prenom) {
		// on empeche les `!@#$%^&*()=+{}|\:;"/?,| 
		$tmp = str_replace(self::$filtre, "", $prenom);
				// les blancs inutiles
		$tmp = trim($tmp);
		$tmp = addslashes($tmp);
		if ($tmp) $this->prenom  = $tmp;
		
		return $this->getPrenom() ;
	}
	/**
	 * 
	 * @ignore
	 */
	public function __toString() {
		return "Utilisateur : ".$this->getPrenom()." ".$this->getNom()." (depuis ".$this->getDate(). ")";
	}
	
	/** 
	 * test l'égalité, seulement avec un User (renvoie faux sinon)
	 * le test ne porte que sur les noms et prénoms
	 */
	public function equals($obj) {
		if ($obj instanceof Data_User) {
			$result = true;
			
			$result = ($obj->nom == $this->nom) && ($obj->prenom == $this->prenom);
			
			
			return $result;
		}
		else return ($obj == $this);
	}
	
	
	/**
	 * pas tres fiable... car la bd peut changer entre deux utilisations...
	 * mais peu s'averer pratique pour echanger des donnees...
	 * @return boolean vrai s'il y a un id
	 */
	public function hasId()
	{
		return ($this->id > 0);
	}
	
	/**
	 * @param int $id permet de spécifier l'id de l'utilisateur.
	 */
	public function setId( $id) {
		if (is_int($id)) $this->id = $id;
		else {
				$cast = (int) $id;
				if ($cast) $this->id = $cast;
			}
	}
	
	/**
	 * @return int retourne l'id  de l'utilisateur.
	 */
	public function getId() {
		return $this->id;
	}
}
?>