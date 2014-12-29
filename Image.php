<?php
/**
 * @author renevier-gonin
 * @package utilities
 * 
 * 
 * @abstract pour fournir une image (src de img) stockée dans la bd à partir de son id et de son type (affiche ou portrait)
 * <code>
 * <img alt="affiche de starwars 4" src="./Image.php?id=1" />
 * </code>
 * le src est donnée via la classe Data_Img
 * @see Data_Img
 */

/**
 * la connexion à la bd est requise...
 */
require "./global/GlobalVariables.php";



if ( isset($_GET['id']) )
{
	$id = intval ($_GET['id']);
	$table = " afficheId, image, type FROM affiche WHERE afficheId = ";
	if (isset($_GET['type']))
	{
		if ($_GET['type'] == "portrait") $table = " portraitId, image, type FROM portrait WHERE portraitId =";
	}
	$req = "SELECT $table  ".$id;
	$ret = mysql_query ($req) or die (mysql_error ());
	$col = mysql_fetch_row ($ret);
	if ( !$col[0] )
	{
		echo "Id d'image inconnu";
	}
		else
	{
		header ("Content-type: {$col[2]}");
		echo $col[1];
	}
}
else
{
	echo "Mauvais id d'image";
}
?>
