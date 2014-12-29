<?php


/**
 * Data_People est un classe qui encapulse les personnes liees au cinema
 * @package Data_
 * 
 * @see Data_Acteur, Data_Realisateur
 */
class Data_People {
	/**
	 * nom de la personne
	 * @var string
	 */
	protected $nom;
	/**
	 * prénom de la personne
	 * @var string
	 */
	protected $prenom;
	/**
	 * année de naissance de la personne
	 * @var int
	 */
	protected $naissance;
	/**
	 * nom de la mort de la personne (-1 si elle est encore en vie)
	 * @var int
	 */
	protected $mort = -1;
	/**
	 * nationalité de la personne, les valeurs possibles sont dans la pseudo enum Data_NATIONALITE
	 * @var string
	 * @see Data_NATIONALITE
	 */
	protected $nationalite;
	/**
	 * sexe de la personne : si vrai, alors la personne est de sexe féminin
	 * @var boolean
	 */
	protected $femme = false;
	
	/**
	 * @ignore
	 */
	protected $id = -1;
	
	/**
	 * 
	 * @param string $nom : nom de la personne (par defaut "")
	 * @param string $prenom : prenom de la personne (par defaut "")
	 * @param int $naissance : annee de naissance (par defaut 0)
	 * @param string $nationalite : une chaine parmis celle de Data_NATIONALITE (par defaut AMERICAINE)
	 * @param boolean $femme : par defaut faux (un garcon)
	 */
	public function __construct($nom = "", $prenom ="", $naissance = 0, $nationalite = "", $femme = false)
	{
		if ($nom) $this->setNom($nom);
		if ($prenom) $this->setPrenom($prenom);
		if ($naissance) $this->setNaissance($naissance);
		if ($nationalite) $this->setNationalite($nationalite);
		else $this->setNationalite(Data_NATIONALITE::getEnum()->AMERICAINE);
		$this->setSexeFeminin($femme);
	}
	
	
	
	/**
	 * @return boolean retourne vrai si c'est une femme.
	 */
	public function isSexeFeminin() {
		return $this->femme;
	}
	
	/**
	 * @param  string $femme précise si c'est une femme (valeur à vrai) ou un homme (valeur à faux).
	 */
	public function setSexeFeminin( $femme) {
		if (is_bool($femme)) $this->femme = (boolean) $femme;
	}
	
	/**
	 * @return int retourne l'année (4 chiffres) de la mort de la personne ou -1 s'il est en vie.
	 */
	public function getMort() {
		return $this->mort;
	}

	/**
	 * @param int $mort est l'année (sur 4 chiffres) de sa mort (ou -1 s'il est encore en vie).
	 */
	public function setMort( $mort) {
		if (is_int($mort)) $this->mort = (int) $mort;
		else {
				$cast = (int) $mort;
				if ($cast >= -1) $this->mort = $cast;
			}
	}

	/**
	 * @return int retourne l'année (4 chiffres) de la naissance de l'acteur.
	 */
	public function getNaissance() {
		return $this->naissance;
	}

	/**
	 * @param int $naissance est l'année (sur 4 chiffres) de sa naissance.
	 */
	public function setNaissance( $naissance) {
		if (is_int($naissance)) $this->naissance = (int) $naissance;
		else {
			$cast = (int) $naissance;
			if ($cast > 0) $this->naissance = $cast;
		}
	}

	/**
	 * @return string retourne le nom de la personne.
	 */
	public function getNom() {
		return $this->nom;
	}

	/**
	 * @param string $nom précise le nom de la personne.
	 */
	public function setNom( $nom) {
		$this->nom = $nom;
	}

	/**
	 * @return string retourne le prénom de la personne.
	 */
	public function getPrenom() {
		return $this->prenom;
	}

	/**
	 * @param string $prenom précise le prénom de la personne.
	 */
	public function setPrenom( $prenom) {
		$this->prenom = $prenom;
	}

	/**
	 * @return string retourne la nationalité (valeur dans Data_NATIONALITE) de la personne.
	 */
	public function getNationalite() {
		return $this->nationalite;
	}

	/**
	 * @param string $nationalite (valeur dans Data_NATIONALITE) fixe la nationalité de la personne.
	 */
	public function setNationalite( $nationalite) {
		if (Data_NATIONALITE::isInEnum($nationalite)) {
			$this->nationalite = $nationalite;
		}
	}
	


	/**
	 * @ignore
	 */
	public function __toString() {
		$am = ($this->mort < 0) ? " - n.a." : " - ".$this->mort;
		$sexe = ($this->femme) ? " [F] " : " [M] ";
		return "".$this->prenom." ".$this->nom." $sexe (".$this->naissance. $am . ")";
	}
	
	
	/** 
	 * test l'égalité, seulement avec un Acteur ou Réalisateur (renvoie faux sinon)
	 * @return boolean : vrai en cas d'égalité entre deux personnes 
	 */
	public function equals($obj) {
		if (($obj instanceof Data_Acteur) || ($obj instanceof Data_Realisateur)) {
			$result = true;			
									
			$result = ($obj->nom == $this->nom) && ($obj->prenom == $this->prenom);
			
			$result = $result && ($obj->naissance == $this->naissance) && ($obj->mort == $this->mort);
			$result = $result && ($obj->femme == $this->femme);
			$result = $result && ($obj->nationalite == $this->nationalite);
			
			return $result;
		}
		
		else return ($obj == $this);
	}
	
	
	/**
	 * pas tres fiable... car la bd peut changer entre deux utilisations...
	 * mais peu s'averer pratique pour echanger des donnees...
	 * @return boolean  vrai s'il y a un id
	 */
	public function hasId()
	{
		return ($this->id > 0);
	}
	
	/**
	 * @param int $id permet de spécifier l'id de la personne (acteur ou réa ou ni l'un ni l'autre).
	 */
	public function setId( $id) {
		if (is_int($id)) $this->id = $id;
		else {
				$cast = (int) $id;
				if ($cast) $this->id = $cast;
			}
	}
	
	/**
	 * @return int retourne l'id  de la personne (acteur ou réa ou ni l'un ni l'autre).
	 */
	public function getId() {
		return $this->id;
	}
}
?>