<?php

/**
 * 
 * @author renevier-gonin
 * @package Data_
 * 
 * @abstract  Cette classe permet d'encapuler les notes faites par les utilisateurs sur les films. Un note sera donc :
 * - une note comprise entre 0 et 10
 * - un auteur (Data_User)
 * - un film (Data_Film)
 * Seule la note est accessible via un getNote et un setNote (pour controle de la valeur)
 * l'auteur et le film sont en accès direct.
 */
class Data_Note {
	/**
	 * la valeur de la note, initialisee a zero
	 * elle doit etre comprise entre 0 et 10 (inclus), et c'est un entier !
	 * @var int
	 */
	private $note = 0;
	/**
	 * l'auteur de la note 
	 * (techniquement il peut y avoir plusieurs notes de la même personne sur le même film)
	 * en accès direct (public)
	 * @var Data_user
	 */
	public $auteur;
	/**
	 * le film sur lequel porte le commentaire
	 * en accès direct (public)
	 * @var Data_Film
	 */
	public $film;
	/**
	 * @var string YYYY-MM-DD HH:MM:SS
	 */
	protected $date;
	
	/**
	 * @ignore
	 */
	protected $id = -1;
	
	
	
	/**
	 * constructeur avec tous les parametres pour construire un objet...
	 * mais si la note n'est pas valide (0 <= valeur <= 10) alors l'objet 
	 * est cree avec une note nulle (si la valeur est < 0) ou de 10 si 
	 * (la valeur est > 10)
	 * @param Data_User $auteur : l'utilisateur qui a note
	 * @param Data_Film $sujet : le film note
	 * @param int $valeur : la note (0-10)
	 */
	public function __construct(Data_User $auteur, Data_Film $sujet, $valeur, $date="") 
	{
		$this->auteur = $auteur;
		$this->film = $sujet;
		
		if (is_int($valeur))
		{
			if ($valeur < 0) $this->note = 0;
			else if ($valeur > 10)$this->note = 10;
			else $this->note = $valeur;
		}
		else {
				$cast = (int) $valeur;
				if ($cast < 0) $this->note = 0;
				else if ($cast > 10)$this->note = 10;
				else $this->note = $cast;		
			}
			
	if ($date)
		{
			$this->setDate($date);
		}
		else
		{
			$this->setDate(date("Y-m-d H:i:s"));
		}
			
		
	}
	
	
	/**
	 * la date est au format YYYY-MM-DD HH:MM:SS, l'année devant être 19xx ou 200x ou 201x
	 * @param string $date : la date de la dernière modification (ou création) de la note
	 */
	public function setDate($date)
	{
		$d = trim($date);
		$nb = preg_match("/(19[0-9]|20[0-1])[0-9]-(0[1-9]|1[0-2])-([0-2][0-9]|3[0-1]) ([0-1][0-9]|2[0-3]):[0-5][0-9]:[0-5][0-9]/", $d);
				// $nb = $nb + preg_match("/20[0|1][0-9]-[0-1][0-9]-[0-3][0-9]/", $d);
		if ($nb == 1)
		{
			$this->date = $d;
		}		
	}
	
	/**
	 * retourne la date de la dernière modification de la note
	 * @return string
	 */
	public function getDate()
	{
		return $this->date;
	}
	
	/**
	 * @return int la note
	 */
	public function getNote() {
		return $this->note;
	}

	/**
	 * si la note n'est pas valide (0 <= valeur <= 10) alors la note
	 * vaut soit 0 (si la valeur est < 0) soit 10 si 
	 * (la valeur est > 10)
	 * @param int $valeur la note a affecter
	 */
	public function setNote( $valeur) {
		if (is_int($valeur))
			{
				if ($valeur < 0) $this->note = 0;
				else if ($valeur > 10)$this->note = 10;
				else $this->note = valeur;
			}
		else 
		{
			$cast = (int) $valeur;
			if ($cast < 0) $this->note = 0;
			else if ($cast > 10)$this->note = 10;
			else $this->note = $cast;
		}
	}

	/**
	 * @ignore
	 */
	public function __toString() {
		return "Notation de ".$this->film." par ".$this->auteur."  : ".$this->note. " ({$this->date})";
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
	 * @param int $id permet de spécifier l'id de la note.
	 */
	public function setId( $id) {
		if (is_int($id)) $this->id = $id;
		else {
				$cast = (int) $id;
				if ($cast) $this->id = $cast;
			}
	}
	
	/**
	 * @return int retourne l'id  de la note.
	 */
	public function getId() {
		return $this->id;
	}
}
?>