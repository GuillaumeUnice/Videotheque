<?php

/**
 * @author renevier-gonin
 * @package Data_
 * 
 * @abstract Data_Dvd est une classe pour encapsuler les possessions (dvd) des utilisateurs (Data_User). Les dvds seront composes de
 * - d'un propriétaire 
 * - d'un film 
 * - d'un nombre d'exemplaire total
 * - d'un nombre d'exemplaire disponible au pret
 * Ces éléments sont accessibles via les getters / setters
 * 
 * @see Data_User, Data_Film
 */
 class Data_Dvd {

 	/**
 	 * le film contenu sur le dvd
 	 * @var Data_Film
 	 */
	protected $filmPossede;
	/**
	 * le nombre d'exemplaire total
	 * @var int
	 */
	protected $nbExemplaires;
	/**
	 * le nombre d'exemplaire disponible au pret
	 * @var int
	 */
	protected $nbDisponibles;
	/**
	 * le propriétaire du(des) dvd(s)
	 * @var Data_User
	 */
	protected $possesseur;
	
	/**
	 * date du dernier achat
	 * @var string YYYY-MM-DD 
	 */
	protected $date;
	
	/**
	 * 
	 * @ignore
	 */
		protected $id = -1;
	
	/**
	 * 
	 * @param Data_User $possesseur : celui qui a achete le(s) dvd(s)
	 * @param Data_Film $film : le film sur le dvd
	 * @param  $nbExemplaires (int) : le nombre de dvds possedes
	 * @param  $nbDispo (int) : le nombre de dvds que le possesseur peut preter
	 */
	public function __construct(Data_User $possesseur, Data_Film $film, $nbExemplaires = 1, $nbDispo = 1, $date="")
	{
		$this->possesseur = $possesseur;
		$this->filmPossede = $film;
		$this->nbExemplaires = $nbExemplaires;
		$this->nbDisponibles = $nbDispo;
		
		if ($date)
			{
				$this->setDate($date);
			}
			else
			{
				$this->setDate(date("Y-m-d"));
			}
	}
	
 	/**
	 * la date est au format YYYY-MM-DD, l'année devant être 19xx ou 200x ou 201x
	 * @param string $date : la date du dernier (ou premier) achat
	 */
	public function setDate($date)
	{
		$d = trim($date);
		$nb = preg_match("/(19[0-9]|20[0-1])[0-9]-(0[1-9]|1[0-2])-([0-2][0-9]|3[0-1])/", $d);
		if ($nb == 1)
		{
			$this->date = $d;
		}		
	}
	
	/**
	 * retourne la date de la dernière date d'achat
	 * @return string
	 */
	public function getDate()
	{
		return $this->date;
	}
	
	/**
	 * pour savoir quel est le film sur le dvd
	 * @return Data_Film
	 */
	public function getFilm( ) {return $this->filmPossede; }
	
 	/**
	 * pour savoir quel est le possesseur du dvd
	 * @return Data_User
	 */
	public function getPossesseur( ) {return $this->possesseur; }
	
	/**
	 * pour donner un dvd a quelqu'un..
	 * @param $u : le nouveau possesseur du dvd
	 */
	public function setPossesseur(Data_User $u) { $this->possesseur = $u; }
	
	/**
	 * @return int : le nombre d'exemplaire du dvd disponible (pour ce possesseur...)
	 */
	public function getNbDispo() {return $this->nbDisponibles ;}
	/**
	 * 
	 * @param int $nb : pour changer le nombre d'exemplaire disponible
	 */
	public function setNbDispo($nb) 
		{
			if (is_int($nb)) $this->nbDisponibles = $nb ;
			else {
				$cast = (int) $nb;
				if ($cast >= 0) $this->nbDisponibles = $cast;
			}
		}
	
	/**
	 * @return int : le nombre d'exemplaire du dvd possedes (par ce possesseur...)
	 */
	public function getNbExemplaires() {return $this->nbExemplaires ;}
	/**
	 * 
	 * @param int $nb pour changer le nombre d'exemplaire possedes
	 */
	public function setNbExemplaires($nb) 
	{
		if (is_int($nb)) $this->nbExemplaires = $nb ;
		else {
				$cast = (int) $nb;
				if ($cast > 0) $this->nbExemplaires = $cast;
			}
	}
	
	
	/**
	 * @ignore
	 */
	public function __toString() {
		return "Dvd de ".$this->possesseur." du film ".$this->filmPossede." (".$this->nbDisponibles."/".$this->nbExemplaires." - {$this->date})";
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
	 * @param int $id permet de spécifier l'id du dvd.
	 */
	public function setId( $id) {
		if (is_int($id)) $this->id = $id;
		else {
				$cast = (int) $id;
				if ($cast) $this->id = $cast;
			}
	}
	
	/**
	 * @return int retourne l'id  du dvd.
	 */
	public function getId() {
		return $this->id;
	}
	
}
?>