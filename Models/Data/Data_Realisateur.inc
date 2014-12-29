<?php

/**
 * @author renevier-gonin
 * @package Data_
 * 
 * @abstract Data_Realisateur est une classe uniquement pour typer les personnes (Data_People) en tant que realisateur
 * @see Data_People
 */
class Data_Realisateur extends Data_People {
	
	
	/**
	 * permet de creer un Réalisateur à partir d'une personne (s'il y a  un paramètre)
	 * ou de créer un réalisateur "vide".
	 * @param Data_People $p : optionel. 
	 */
	public function __construct(Data_People $p = null)
	{
		if ($p)
		{
			$this->setNom($p->getNom());
			$this->setPrenom($p->getPrenom());
			$this->setMort($p->getMort());
			$this->setNaissance($p->getNaissance());
			$this->setNationalite($p->getNationalite());
			$this->setSexeFeminin($p->isSexeFeminin());

			if ($p->hasId()) $this->setId($p->getId());
		}
		else $this->setNationalite(Data_NATIONALITE::getEnum()->AMERICAINE);
		
	}


}
?>