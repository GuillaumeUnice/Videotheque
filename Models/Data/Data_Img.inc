<?php

/**
 * 
 * @author renevier-gonin
 * @package Data_
 * 
 * 
 * @abstract Cette classe encapsule les images contenues dans la BD. 
 *elle permet d'en obtenir le chemin vers l'image (src de la balise img)
 *@use constant("PATHTOROOT")
 *
 */
 class Data_Img {
	
 	/**
 	 * id de l'image dans la bd (table affiche ou portrait)
 	 * @var int 
 	 */
	protected $id ;
	/**
	 * @var boolean  
	 */
	protected $film = true;
	
	/**
	 * @param int $id : l'id - dans la base de donnee - de l'image
	 * @param boolean $isFilm : true par defaut, indique si c'est une affiche (ou sinon un portrait)
	 */
	public function __construct( $id, $isFilm=true)
	{
		
		if (is_int($id)) $this->id = $id;
		else
		{
			$cast = (int) $id;
			if ($cast > 0) $this->id  = $cast;
		}
		$this->film = $isFilm;
	}
	
	/**
	 * pour indiquer que cette image est une affiche de film
	 */
	public function setAfficheFilm()
	{
		$this->film = true;
	}
	
	/**
	 * pour indiquer que cette image est une photo d'une personne
	 */
 	public function setPortrait()
	{
		$this->film = false;
	}
	
	/**
	 * @return boolean : vrai si c'est une affiche de film
	 */
	public function isAffiche()
	{
		return $this->film ;
	}
	/**
	 * @return boolean : vrai si c'est une image de personne (une photo, un portrait)
	 */
	public function isPortrait()
	{
		return (! $this->film) ;
	}
	
	/**
	 * s'il n'y a pas de parametre, utilisera constant("PATHTOROOT")
	 * @param $pathToSiteRoot : (optionel) le chemin depuis l'execution du script php (la page web) vers la racine du site
	 * @return string : le chemin vers l'image... via une page php...
	 */
	public function getSrc($pathToSiteRoot="")
	{
		if (! $pathToSiteRoot) $pathToSiteRoot = constant("PATHTOROOT");
		$sep ="";
		if (strrpos($pathToSiteRoot, "/") != (strlen($pathToSiteRoot)-1) ) $sep ="/";
		$get = "id={$this->id}";
		if ($this->isPortrait()) $get .= "&type=portrait";
		return "{$pathToSiteRoot}{$sep}Image.php?$get";
	}

 	/**
	 * @return int retourne l'id  de l'image.
	 */
	public function getId() {
		return $this->id;
	}
}

?>