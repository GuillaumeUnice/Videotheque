<?php

/**
 * 
 * @author renevier-gonin
 * @package Data_
 * 
 * 
 * @abstract classe qui permet de contraindre les valeurs possibles pour les Langues.
 *les valeurs s'utilisent comme des champs de classes :
 *<code>
 *Data_LANGUE::getEnum()->FRANCAIS // pour obtenir le français
 *Data_LANGUE::getEnum()->CHINOIS // pour obtenir le chinois
 *</code>
 *les valeurs possibles sont "FRANCAIS","ANGLAIS","ALLEMAND","ESPAGNOL","ITALIEN","GREC","TCHEQUE","PORTUGAIS",
 *"POLONAIS","DANOIS","ISLANDAIS","SUEDOIS","NORVEGIEN","FILANDAIS","ESTONIEN","LETTON","LITUANIEN",
 *"GEORGIEN","ROUMAIN","MOLDAVE","SLOVENE","SLOVAQUE","CROATE","SERBE","UKRAINIEN","RUSSE","HOLLANDAIS",
 *"JAPONAIS","INDIEN","CHINOIS","ARABE"," MONGOLE"
 */
class Data_LANGUE extends Libs_Enum {
	
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
		$langs = array( "FRANCAIS","ANGLAIS","ALLEMAND","ESPAGNOL",
	 "ITALIEN","GREC","TCHEQUE","PORTUGAIS",
	"POLONAIS","DANOIS","ISLANDAIS","SUEDOIS","NORVEGIEN","FILANDAIS","ESTONIEN","LETTON","LITUANIEN",
	"GEORGIEN","ROUMAIN","MOLDAVE","SLOVENE","SLOVAQUE","CROATE","SERBE","UKRAINIEN","RUSSE","HOLLANDAIS",
	"JAPONAIS","INDIEN","CHINOIS","ARABE"," MONGOLE"
		);
		foreach($langs as $langage) {
			$this->add($langage);
		}
	}

	/**
	 * @return Data_LANGUE l'unique instance qui permet d'obtenir les valeurs de l'enumeration, sous forme de propriete 
	 */
	public static function getEnum()
    {
    	if (! Data_LANGUE::$instance) Data_LANGUE::$instance = new Data_LANGUE();
    	return Data_LANGUE::$instance;
    }
    
    /**
     * 
     * @param string $name : pour savoir si cette valeur est dans l'enum
     * @return boolean vrai si $name est une valeur possible
     */
 	public static function isInEnum($name)
    {
    	$result = false;
    	
    	$enum = Data_LANGUE::getEnum();
    	$result = array_key_exists($name, $enum->self);
    	    	
    	return $result;
    }
    
    /**
     * @return array : un tableau avec toutes les valeurs de l'enum
     */
  public static function getValues()
        {
        	$enum = Data_LANGUE::getEnum();
    		$result = array_keys($enum->self);
    	    	
    		return $result; 
        }
}
?>