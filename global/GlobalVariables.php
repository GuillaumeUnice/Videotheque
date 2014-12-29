<?php
/**
 * @author brel
 */
 
/**
 * @abstract pour définir la racine du site (chemin à partir duquel est exécuté le script)
 *
 * @var @ignore
 */ 
define("PATHTOROOT", "./");

/**
 * @abstract pour définir le suffixe des classes Controlleurs
 *
 * @var @ignore
 */
define('CONTROLLER_SUFFIXE', '_Controller');

/**
 * @abstract pour définir le suffixe des classes Vues
 *
 * @var @ignore
 */
define('VIEW_SUFFIXE', '_View');

/**
 * @abstract pour définir l'URL de la page d'accueil
 *
 * @var @ignore
 */
define('INDEX_PAGE', './index.php?controller=Accueil');

/**
 * @author renevier-gonin
 * @abstract pour définir (une seule fois) la connexion à la base de données.  
 *
 * 
 * @var @ignore
 */
$test = @constant("CONNEXION");
if (! $test)
{
	define("CONNEXION", mysql_connect("localhost", "root", ""));
	mysql_select_db("poo-ihm2014", constant("CONNEXION"));
}

?>