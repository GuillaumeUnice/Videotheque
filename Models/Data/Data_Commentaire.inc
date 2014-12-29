<?php

/**
 *  @author renevier-gonin
 * @package Data_
 * 
 *  @abstract Data_Commentaire est une classe pour encapsuler les commentaires. Ceux-ci seront composes de
 * - d'un auteur 
 * - d'un film 
 * - d'un texte
 * Ces éléments sont accessibles via les getters / setters
 * 
 * @see Data_User, Data_Film
 */
class Data_Commentaire {
	/**
	 * le texte du commentaire
	 * @var string
	 */
	protected $commentaire;
	/**
	 * l'auteur du commentaire 
	 * (il peut y avoir plusieurs commentaire de la même personne sur le même film)
	 * @var Data_User
	 */
	protected $auteur;
	/**
	 * le film sur lequel porte le commentaire
	 * @var Data_Film
	 */
	protected $film;
	/**
	 * @var string YYYY-MM-DD HH:MM:SS
	 */
	protected $date;
	
	/**
	 * @ignore
	 */
	protected $id = -1;
	
	
	/**
	 * @param Data_User $u (null ou l'auteur du commentaire)
	 * @param Data_Film $f (null ou le film sur lequel porte le commentaire)
	 * @param string $txt ("" ou le commentaire)
	 * @param string $date ("" ou la date au format YYYY-MM-JJ) - par défaut la date du jour sera affecté
	 */
	public function __construct(Data_User $u= null, Data_Film $f= null, $txt = "", $date="") 
	{
		if ($u)
		{
			$this->auteur = $u;
		}
		
		if ($f)
			{
				$this->film = $f;
			}
			
		if ($txt)
			{
				$this->commentaire = $txt;
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
	 * @param string $date : la date de la dernière modification (ou création) du commentaire
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
	 * retourne la date de la dernière modification du commentaire
	 * @return string
	 */
	public function getDate()
	{
		return $this->date;
	}
	
	/**
	 * @ignore
	 */
	public function __toString() {
		return "critique de ".$this->film." par ".$this->auteur." ({$this->date}): ".$this->commentaire;
	}
	
	/**
	 * @return Data_User : l'auteur du commentaire
	 */
	public function getAuteur() { return $this->auteur ; }
	/**
	 * @return Data_Film : le film commenté
	 */
	public function getFilm() { return $this->film ; }
	/**
	 * @return string : le commentaire
	 */
	public function getCommentaire() { return $this->commentaire ; }
	
	/**
	 * pour determiner qui a fait le commentaire
	 * @param Data_User $u : l'utilisateur auteur de la critique
	 */
	public function setAuteur(Data_User $u) { $this->auteur = $u; }
	/**
	 * pour determiner le sujet de la critique, le film sur lequel porte le commentaire
	 * @param Data_Film $f : le film qui est commente
	 */
	public function setFilm(Data_Film $f) { $this->film = $f; }
	/**
	 * pour definir le commentaire lui meme
	 * @param $txt : le texte du commentaire
	 */
	public function setCommentaire($txt) { $this->commentaire = $txt; }
	
	
	
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
	 * @param int $id permet de spécifier l'id du commentaire.
	 */
	public function setId( $id) {
		if (is_int($id)) $this->id = $id;
		else {
				$cast = (int) $id;
				if ($cast) $this->id = $cast;
			}
	}
	
	/**
	 * @return int retourne l'id  du commentaire.
	 */
	public function getId() {
		return $this->id;
	}
}	
?>