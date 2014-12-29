<?php
/**
 * 
 * @author renevier-gonin
 * @package Data_
 * 
 * @abstract Cette classe permet d'encapuler les rôles. Un rôle sera donc :
 * - un nom de personnage
 * - un acteur (Data_Acteur)
 * l'auteur et le nom  sont en accès direct.
 */
class Data_Role {
	/**
	 * nom est le nom du personnage dans le film
	 * en accès direct.
	 * @var string
	 */
	public $nom ;
	/**
	 * acteur est l'acteur qui tient le role du personnage 'nom'
	 * en accès direct.
	 * @var Data_Acteur
	 */
	public $acteur;
	
	/**
	 * @ignore
	 */
	protected $id = -1;
	
	/**
	 * constructeur parametre
	 * @param $n : nom du role
	 * @param $a : Data_Acteur
	 */
	public function __construct($n, Data_Acteur $a)
	{
		$this->nom = $n;
		$this->acteur = $a;
	}
	/**
	 * @ignore
	 */
	public function __toString() {
		return "(Acteur : ".$this->acteur.", Personnage : ".$this->nom. ")";
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
	 * @param int $id permet de spécifier l'id du rôle.
	 */
	public function setId( $id) {
		if (is_int($id)) $this->id = $id;
		else {
				$cast = (int) $id;
				if ($cast) $this->id = $cast;
			}
	}
	
	/**
	 * @return int retourne l'id  du rôle.
	 */
	public function getId() {
		return $this->id;
	}
	
}
?>