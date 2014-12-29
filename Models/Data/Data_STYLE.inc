<?php
/**
 * 
 * @author renevier-gonin
 * @package Data_
 * 
 * 
 * @abstract classe qui permet de contraindre les valeurs possibles pour les styles de film.
 *les valeurs s'utilisent comme des champs de classes :
 *<code>
 *Data_STYLE::getEnum()->SCIENCEFICTION // pour un film de science fiction
 *Data_STYLE::getEnum()->HORREUR // pour un film d'horreur
 *</code>
 *les valeurs possibles sont : "FICTION", "SCIENCEFICTION", "COMEDIE", "DRAME", "HORREUR", "AVENTURE", "HISTORIQUE"
 */
class Data_STYLE extends Libs_Enum {
	    
	/**
	 * 
	 * @ignore
	 */
	protected static $instance;
	
	/**
	 * 
	 * @ignore
	 */
	protected function __construct( /*...*/ ) {
		$langs = array( 
		"FICTION", "SCIENCEFICTION", "COMEDIE", "DRAME", "HORREUR", "AVENTURE", "HISTORIQUE"
		);
		foreach($langs as $langage) {
			$this->add($langage);
		}
	}

	/**
	 * @return Data_STYLE Permet d'obtenir l'enumeration pour les styles de films possibles  compatible avec la BD
	 */
	public static function getEnum()
    {
    	if (! Data_STYLE::$instance) Data_STYLE::$instance = new Data_STYLE();
    	return Data_STYLE::$instance;
    }
    
    /**
     * 
     * @param string $name : pour savoir si cette valeur est dans l'enum
     * @return boolean vrai si $name est une valeur possible
     */
 public static function isInEnum($name)
    {
    	$result = false;
    	
    	$enum = Data_STYLE::getEnum();
    	$result = array_key_exists($name, $enum->self);
    	    	
    	return $result;
    }

    /**
     * @return array : un tableau avec toutes les valeurs de l'enum
     */
  public static function getValues()
        {
        	$enum = Data_STYLE::getEnum();
    		$result = array_keys($enum->self);
    	    	
    		return $result; 
        }
}
?>