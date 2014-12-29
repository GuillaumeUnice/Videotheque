<?php
/**
 * 
 * @author renevier-gonin, Franck Berthelon
 * @package Data_
 * 
 * 
 * @abstract valeur possible pour l'état d'une demande d'amitié
 * 'ATTENTE', 'DEMANDE','ACCEPTE'
 * les valeurs s'utilisent comme des champs de classes :
 * <code>
 * Data_STATUS::getEnum()->ATTENTE // pour une demande d'amitié faite en attente de réponse
 * Data_STATUS::getEnum()->DEMANDE // pour une demande d'amitié qui attent d'une réponse de la part de la personne contactée
 * Data_STATUS::getEnum()->ACCEPTE // pour une demande d'amiité confirmée
 * </code>
 * 
 */
class Data_STATUS extends Libs_Enum {
	    
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
		$status = array( 
		'ATTENTE', 'DEMANDE','ACCEPTE'
		);
		foreach($status as $s) {
			$this->add($s);
		}
	}

	/**
	 * @return Data_STATUS 
	 */
	public static function getEnum()
    {
    	if (! Data_STATUS::$instance) Data_STATUS::$instance = new Data_STATUS();
    	return Data_STATUS::$instance;
    }
    
    /**
     * 
     * @param string $name : pour savoir si cette valeur est dans l'enum
     * @return boolean vrai si $name est une valeur possible
     */
 	public static function isInEnum($name)
    {
    	$result = false;
    	
    	$enum = Data_STATUS::getEnum();
    	$result = array_key_exists($name, $enum->self);
    	    	
    	return $result;
    }

    /**
     * @return array : un tableau avec toutes les valeurs de l'enum
     */
  	public static function getValues()
        {
        	$enum = Data_STATUS::getEnum();
    		$result = array_keys($enum->self);
    	    	
    		return $result; 
        }
}
?>