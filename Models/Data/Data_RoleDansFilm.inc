<?php

/**
 * 
 * @author renevier-gonin
 * @package Data_
 * 
 * 
 * @abstract Cette classe est une classe de convenance, pour associer plus facilement un acteur
 * avec ses roles. La classe est faite de : 
 * - un titre de film , pour un affichage plus "rapide"
 * - un id de film, pour eventuellement retrouver le film (ou mettre un lien vers le film)
 * - un role (Data_Role)
 *
 */
class Data_RoleDansFilm {
	
	/**
	 * 
	 * @var int
	 */
	protected $film ;
	/**
	 * 
	 * @var string
	 */
	protected $titreFilm ;
	/**
	 * 
	 * @var Data_Role
	 */
	protected $role ;
	
	
	/**
	 * les elements peuvent etre null (ou sans valeur)
	 * classe de convenance, pour ne pas a trop fouiller la bd.
	 * @param Data_Role $r : le role (couple acteur - nom de personnage)
	 * @param int $f : id du film 
	 * @param string $t : titre du film 
	 */
	public function __construct(Data_Role $r=null, $f=-1, $t="")
	{
		$this->film = $f;
		$this->titreFilm = $t;
		$this->role = $r;
	}
	/**
	 * @return int l'id du film a associe avec un role
	 */
	public function getFilm() {
		return $this->film;
	}
	/**
	 * @return Data_Role le role
	 */
	public function getRole() {
		return $this->role;
	}
	/**
	 * @param int $film l'id du film
	 */
	public function setFilm($film) {
		if (is_int($film) && ($film > 0)) $this->film = $film;
		else
			{
				$cast = (int) $film;
				if ($cast > 0) $this->film  = $cast;
			}
	}
	/**
	 * @param Data_Role $role : le role a associer avec un film
	 */
	public function setRole(Data_Role $role) {
		$this->role = $role;
	} 
	
	/**
	 * @ignore
	 */
	public function __toString()
	{
		return "film (".$this->film.") = ".$this->titreFilm." et role = ".$this->role;
	}

	/**
	 * @return  string le titre du film 
	 */
	public function getTitreFilm() {
		return $this->titreFilm;
	}

	/**
	 * @param string $titreFilm le titre du film... convenance pour l'afficher
	 */
	public function setTitreFilm($titreFilm) {
		$this->titreFilm = $titreFilm;
	}

}
?>