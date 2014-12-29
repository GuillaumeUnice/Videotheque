<?php

/**
 * 
 * @author renevier-gonin
 * @package Data_
 * 
 * @abstract classe qui permet de contraindre les valeurs possibles pour les nationalités.
 *les valeurs s'utilisent comme des champs de classes :
 *<code>
 *Data_NATIONALITE::getEnum()->FRANCAISE // pour obtenir la nationalité française
 *Data_NATIONALITE::getEnum()->CHINOISE // pour obtenir la nationalité chinoise
 *</code>
 *les valeurs possibles sont "FRANCAISE","AMERICAINE","ANGLAISE","BELGE","ITALIENNE","ESPAGNOLE",
 *"GRECQUE","TCHEQUE","ALLEMANDE","SUISSE","PORTUGAISE","MONEGASQUE","POLONAISE",
 *"AUTRICHIENNE","DANOISE","IRLANDAISE","ISLANDAISE","SUEDOISE","NORVEGIENNE",
 *"FILANDAISE","ESTONIENNE","LETTONNE","LITUANIENNE","GEORGIENNE","ROUMAINE",
 *"MOLDAVE","SLOVENE","SLOVAQUE","CROATE","SERBE","UKRAINIENNE","BIELORUSSE",
 *"RUSSE","LUXEMBOURGEOISE","HOLLANDAISE","CANADIENNE","JAPONAISE","INDIENNE",
 *"CHINOISE","EGYPTIENNE","MAROCAINE","ALGERIENNE","TUNISIENNE","BRESILIENNE",
 *"MEXICAINE","MONGOLE","AUSTRALIENNE","NEOZELANDAISE","ECOSSAISE","GALLOISE",
 *"ARGENTINE","CHILIENNE","PERUVIENNE","BOLIVIENNE","COLOMBIENNE"
 */
class Data_NATIONALITE extends Libs_Enum {
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
		$nat = array( "FRANCAISE","AMERICAINE","ANGLAISE","BELGE","ITALIENNE","ESPAGNOLE",
					"GRECQUE","TCHEQUE","ALLEMANDE","SUISSE","PORTUGAISE","MONEGASQUE","POLONAISE",
					"AUTRICHIENNE","DANOISE","IRLANDAISE","ISLANDAISE","SUEDOISE","NORVEGIENNE",
					"FILANDAISE","ESTONIENNE","LETTONNE","LITUANIENNE","GEORGIENNE","ROUMAINE",
					"MOLDAVE","SLOVENE","SLOVAQUE","CROATE","SERBE","UKRAINIENNE","BIELORUSSE",
					"RUSSE","LUXEMBOURGEOISE","HOLLANDAISE","CANADIENNE","JAPONAISE","INDIENNE",
					"CHINOISE","EGYPTIENNE","MAROCAINE","ALGERIENNE","TUNISIENNE","BRESILIENNE",
					"MEXICAINE","MONGOLE","AUSTRALIENNE","NEOZELANDAISE","ECOSSAISE","GALLOISE",
					"ARGENTINE","CHILIENNE","PERUVIENNE","BOLIVIENNE","COLOMBIENNE" );
		foreach($nat as $pays) {
			$this->add($pays);
		}
	}

	/**
	 * @return Data_NATIONALITE l'unique instance qui permet d'obtenir les valeurs de l'enumeration, sous forme de propriete 
	 */
	public static function getEnum()
    {
    	if (! Data_NATIONALITE::$instance) Data_NATIONALITE::$instance = new Data_NATIONALITE();
    	return Data_NATIONALITE::$instance;
    }
    
    /**
     * 
     * @param string $name : pour savoir si cette valeur est dans l'enum
     * @return boolean vrai si $name est une valeur possible
     */
 	public static function isInEnum($name)
    {
    	$result = false;
    	
    	$enum = Data_NATIONALITE::getEnum();
    	$result = array_key_exists($name, $enum->self);
    	    	
    	return $result;
    }
    
     /**
     * @return array : un tableau avec toutes les valeurs de l'enum
     */
     public static function getValues()
        {
        	$enum = Data_NATIONALITE::getEnum();
    		$result = array_keys( $enum->self);
    	    	
    		return $result; 
        }
    
}
?>