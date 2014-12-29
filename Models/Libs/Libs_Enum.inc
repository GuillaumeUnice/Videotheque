<?php
/**
 * 
 * @author renevier-gonin, inspiré de redrik@gmail.com pour faire une énumération en php
 * @package Libs_
 * 
 * @link http://www.php.net/manual/en/language.oop5.php#87942 
 */
abstract class Libs_Enum {

	/**
	 * 
	 * @ignore
	 */
    protected $self = array();
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
        
    }
    
    /**
	 * @return ?? l'unique instance qui permet d'obtenir les valeurs de l'enumeration, sous forme de propriete 
	 */
    public  static function getEnum() 
    {
    }
    
    
    /**
     * @return array : un tableau avec toutes les valeurs de l'enum
     */
    public  static function getValues() {}
    
    
     /**
     * 
     * @param string $name : pour savoir si cette valeur est dans l'enum
     * @return boolean vrai si $name est une valeur possible
     */
    public  static function isInEnum($name) {}
   
    /**
	 * 
	 * @ignore
	 */
    public function __get( /*string*/ $name = null ) {
        return $this->self[$name];
    }
   
    /**
	 * 
	 * @ignore
	 */
    public function __isset($name)
    {
    	return isset($this->self[$name]);
    }
    
    /**
     * fonction pour ajouter des valeurs possibles à une énumération
     * à ne pas utiliser de préférence (uniquement au moment de le définition de l'enum)
     * @param string $name : le nom de la valeur
     * @param  $enum : la valeur elle-même, par défaut à null (la valeur est alors $name) 
     */
    public function add( /*string*/ $name = null, /*int*/ $enum = null ) {
        if( isset($enum) )
            $this->self[$name] = $enum;
        else
            $this->self[$name] = "$name";
    }
    
   
}
?>