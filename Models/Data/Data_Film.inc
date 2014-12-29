<?php

/**
 * 
 * @author renevier-gonin
 * @package Data_
 * 
 *  @abstract la classe Data_Film permet de représenter les films :
 * - un titre
 * - une année de sortie
 * - un seul et unique réalisateur
 * - un tableau de rôles
 * - un résumé
 * - un style (c.f. Data_STYLE)
 * - une langue (c.f. Data_LANGUE)
 * @see Data_STYLE, Data_LANGUE
 */
class Data_Film {
	/**
	 * 
	 * @var array : tableu de Data_Role
	 * @see Data_Role 
	 */
	protected $roles = array();
	/**
	 * le (seul et unique) realisateur du film
	 * @var Data_Realisateur
	 */
	protected $director;
	/**
	 * @var string
	 */
	protected $resume;
	/**
	 * 
	 * @var string
	 */
	protected $titre;
	/**
	 * 
	 * @var string : valeur parmis celles de Data_LANGUE
	 * @see Data_LANGUE
	 */
	protected $langue;
	/**
	 * 
	 * @var int, l'année de sortie du film
	 */
	protected $annee;
	/**
	 * 
	 * @var string : valeur parmis celles de Data_STYLE
	 * @see Data_STYLE
	 */
	protected $style ;

	/**
	 * 
	 * @ignore
	 */
	protected $id = -1;
	
	/**
	 * constructeur sans paramètre : ne fait rien, sauf mettre un style par defaut : FICTON
	 *
	 * duplication de film (tout sauf le realisateur et les roles)
	 * @param Data_Film $f : film qui sert à initialiser le nouveau film (peut etre null)
	 */
	public function __construct(Data_Film $f=null) {
		if ($f)
		{
			$this->resume = $f->resume;
			$this->titre = $f->titre;
			$this->langue = $f->langue;
			$this->annee = $f->annee;
			$this->style = $f->style;
		}
		else
		{
			$this->style = Data_STYLE::getEnum()->FICTION;
		}
		
		
	}
	
	/** 
	 * test l'égalité sur resume, titre, langue, annee et style
	 * @param Object : a tester... (il y a un test $obj instanceof Data_Film ) 
	 * @return boolean
	 */
	public function equals($obj) {
		if ($obj instanceof Data_Film) {
			$result = true;
			
			$result = ($obj->resume == $this->resume) && ($obj->titre == $this->titre);
			$result = $result && ($obj->annee ==   $this->annee) ;
			$result = $result && ($obj->langue == $this->langue);
			$result = $result && ($obj->style  == $this->style);
			
			return $result;
		}
		else return ($obj == $this);
	}
	
	/**
	 * @return int : retourne l'année de sortie du film.
	 */
	public function getAnnee() {
		return $this->annee;
	}

	/**
	 * @param Data_Acteur $acteur est l'acteur dont on veut savoir s'il joue dans le film
	 * @return boolean : vrai ou faux, selon si l'acteur joue dans le film ou non
	 */
	public function joueDedans(Data_Acteur $acteur)
	{
		$jouededans = false;
		foreach($this->roles as $r)
		{
			if ($r->acteur->equals($acteur)) return true;
		}
		return $jouededans;
	}
	
	/**
	 * @param int $annee permet de spécifier l'année de sortie du film.
	 */
	public function setAnnee( $annee) {
		if (is_int($annee)) $this->annee = $annee;
		else {
				$cast = (int) $annee;
				if ($cast > 0) $this->annee = $cast;
			}
	}

	/**
	 * ajoute le role à la liste des personnages du film.
	 * @param Data_Role $r est un Role (couple Acteur et String [nom du personnage]) du film.
	 */
	public function addPersonnage(Data_Role $r)
	{
		if (! in_array($r, $this->roles)) $this->roles[] = $r;
	}
	
	/**
	 * @return array : retourne la liste des personnages (couple Acteur et String [nom du personnage]) du film sous forme d'un tableau.
	 * @see Data_Role
	 */
	public function getPersonnages()
	{
		// Role [] r = new Role[roles.size()];
		return $this->roles;
	}
	
	/**
	 * permet de spécifier le réalisateur du film.
	 * @param Data_Realisateur $d est le réalisateur du film.
	 */
	public function setRealisateur(Data_Realisateur $d)
	{
		$this->director = $d;
	}
	
	/**
	 * @return Data_Realisateur retourne le réalisateur du film.
	 */
	public function getRealisateur()
	{
		return $this->director ;
	}
	
	/**
	 * @param string txt est un résumé du film.
	 * permet de préciser le résumé du film.
	 */
	public function setResume( $txt)
	{
		$this->resume = $txt;
	}
	
	/**
	 * @return string retourne le résumé du film.
	 */
	public function getResume()
	{
		return $this->resume ;
	}

	/**
	 * @return string retourne la langue (type ENUM LANGUE) du film.
	 */
	public function getLangue() {
		return $this->langue;
	}

	/**
	 * @param string $langue est la langue officielle du film.
	 */
	public function setLangue( $langue) {
				if (Data_LANGUE::isInEnum($langue)) {
						$this->langue = $langue;
				}
	}

	/**
	 * @return string retourne le titre du film.
	 */
	public function getTitre() {
		return $this->titre;
	}

	/**
	 * @param string $titre est le titre officiel du film.
	 */
	public function setTitre( $titre) {
		$this->titre = $titre;
	}
	
	
	/**
	 * description est assez complete et longue
	 * @return string une description complete du film
	 */
	public function toStringLong() {
		$result =  "Titre = ".$this->titre." langue = ".$this->langue." ; style = ".$this->style." de ".$this->annee." realise par ".$this->director. "\nAvec : ";
		foreach($this->roles as $r) $result.= "\n \t - $r";
		$result.="\n resume : ".$this->resume;
		return $result;
	}
	
	
	/**
	 * @ignore
	 */
	public function __toString() {
		$result =  $this->titre." (".$this->annee.")";
		return $result;
	}

	/**
	 * @return string retourne le style (type ENUM STYLE) du film->
	 */
	public function getStyle() {
		return $this->style;
	}

	/**
	 * @param string $style (type ENUM STYLE) est le style (genre) du film->
	 */
	public function setStyle( $style) {
		if (Data_STYLE::isInEnum($style)) {
						$this->style = $style;
		}
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
	 * @param int $id permet de spécifier l'id du film.
	 */
	public function setId( $id) {
		if (is_int($id)) $this->id = $id;
		else {
				$cast = (int) $id;
				if ($cast) $this->id = $cast;
			}
	}
	
	/**
	 * @return int retourne l'id  du film.
	 */
	public function getId() {
		return $this->id;
	}
}
?>