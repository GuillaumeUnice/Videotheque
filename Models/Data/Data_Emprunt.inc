<?php

/**
 * @author renevier-gonin
 * @package Data_
 * 
 * @abstract Data_Emprunt
 * pour pouvoir associer une date à l'emprunt
 * 
 * @see Data_User, Data_Film, Data_Dvd
 */
 class Data_Emprunt {

 	/**
 	 * le dvd emprunte
 	 * @var Data_Dvd
 	 */
	protected $dvd;
	
	/**
	 * l'emprunteur
	 * @var Data_User
	 */
	protected $emprunteur;
	
	/**
	 * date d'emprunt
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
	public function __construct(Data_User $emprunteur, Data_Dvd $dvd, $date="")
	{
		$this->emprunteur = $emprunteur;
		$this->dvd = $dvd;
		
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
	 * @param string $date : la date de l'emprunt
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
	 * retourne la date de la dernière modification de l'emprunt
	 * @return string
	 */
	public function getDate()
	{
		return $this->date;
	}
	
	
	
	
	
	/**
	 * pour savoir quel dvd est emprunte
	 * @return Data_Dvd
	 */
	public function getDvd( ) {return $this->dvd; }
	
 	/**
	 * pour savoir quel est l'emprunteur du dvd
	 * @return Data_User
	 */
	public function getEmprunteur( ) {return $this->emprunteur; }
	
	/**
	 * pour passer l'emprunt d'un dvd à quelqu'un..
	 * @param $u : le nouvel emprunteur du dvd
	 */
	public function setEmprunteur(Data_User $u) 
	{ 
		$this->emprunteur = $u; 
		$this->setDate(date("Y-m-d"));
	}
	

	
	
	/**
	 * @ignore
	 */
	public function __toString() {
		return "emprunt par ".$this->emprunteur." du dvd ".$this->dvd." (".$this->date.")";
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
	 * @param int $id permet de spécifier l'id de l'emprunt.
	 */
	public function setId( $id) {
		if (is_int($id)) $this->id = $id;
		else {
				$cast = (int) $id;
				if ($cast) $this->id = $cast;
			}
	}
	
	/**
	 * @return int retourne l'id  de l'emprunt.
	 */
	public function getId() {
		return $this->id;
	}
	
}
?>