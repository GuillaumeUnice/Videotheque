<?php

//include constant("PATHTOROOT")."includes/connexion.inc";

/**
 * 
 * @author Franck Berthelon
 * @package Nf_
 * 
 * @abstract 
 * extension de Nf_UserDvdManagement pour l'ajout des relations sociales
 * Chaque demande sera enregistrée deux fois : 
 * - demandeur - demandé (état = attente)
 * - demandé - demandeur (état = demande)
 * - puis, si c'est confirmé par le demandé demandeur - demandé et  demandé - demandeur avec l'état ACCEPTE pour les deux
 * - en cas de refus par le demandé, la demande est détruite
 * <code>
 * $friendMgt = Nf_FriendManagement::getInstance();
 * $friends = $friendMgt->getAmis($utilisateur); // pour obtenir la liste des amis
 * </code>
 * 
 */
class Nf_FriendManagement extends Nf_UserDvdManagement {

	/**
	 * @ignore
	 * @var Nf_FriendManagement
	 */
	protected static $instance;

	/**
	 * @ignore
	 */
	protected function __construct()
	{
		$this->connexion = constant('CONNEXION');	
	}
	
	/**
	 * pour obtenir l'unique instance de Nf_FriendManagement
	 * @return Nf_FriendManagement
	 */
	public static function getInstance()
	{
		if (! Nf_FriendManagement::$instance) Nf_FriendManagement::$instance = new Nf_FriendManagement();
		
		return Nf_FriendManagement::$instance;
	}
	
	
	/**
	 * pour obtenir les dernières amitiés, quelques soient leurs statuts.	
	 * @param int $nb le nombre des dernières demandes (confirmées ou non) d'amitié
	 * @return Array un tableau de Data_Friend
	 */
	public function getDernieresAmities($nb = 5)
	{
		$n = (int) $nb;
		$n = $n*2;
		$query = "SELECT * FROM `estami` ORDER BY `estami`.`date` DESC LIMIT 0 , $n";
		
		
		$result = array();
        
		$localResults = mysql_query($query, $this->connexion);
        if($localResults)
            {
            $odd = false;
            while ($rep = mysql_fetch_array($localResults)) {
              	// pour en prendre un sur deux...          	
            	if ($odd) 
              	{
              		$ami = $this->idToUser($rep["idami"]);
              		$u = $this->idToUser($rep["iduser"]);
              		$friend = new Data_Friend($u, $ami, $rep["status"], $rep["date"]);
            		$result[] = $friend;
             	 }
              	
             	 $odd = ! $odd;
            	
              }
			}
        return $result;
	}
	
	
	/**
	 * pour obtenir toutes les amitiés et demande d'amitié
	 * il y a un filre, pour prendre qu'une entrée sur deux dans la table, 
	 * car chaque demande est entrée deux fois : demandeur - demandé et demandé - demandeur
	 * @return Array un tableau de Data_Friend
	 */
	public function getAmities()
	{
	
		$query = "SELECT * FROM `estami` ORDER BY `estami`.`date` DESC ";
		
		
		$result = array();
        
		$localResults = mysql_query($query, $this->connexion);
        if($localResults)
            {
            $odd = false;
            while ($rep = mysql_fetch_array($localResults)) {
              	// pour en prendre un sur deux...          	
            	if ($odd) 
              	{
              		$ami = $this->idToUser($rep["idami"]);
              		$u = $this->idToUser($rep["iduser"]);
              		$friend = new Data_Friend($u, $ami, $rep["status"], $rep["date"]);
            		$result[] = $friend;
             	 }
              	
             	 $odd = ! $odd;
            	
              }
			}
        return $result;
	}
	
	
		/**
         * pour obtenir la liste des amis d'un utilisateur
         * @param Data_User $u : l'utilisateur
         * @param string $status : paramètre optionnel permettant de filtrer les resultats en fonction du statut
         * @return array un tableau  de Data_Friend (qui peut etre vide)
         */
        public function getAmis(Data_User $u, $status="") {
          $result = array();
         
          if ($status && !Data_STATUS::isInEnum($status)) return $result;

          $uid = $this->userToId($u);

          if ($uid) {
          	$u->setId($uid);
            $query = "SELECT * FROM estami where iduser=\"$uid\"".(($status)?" and status=\"$status\"":"");
         
            $localResults = mysql_query($query, $this->connexion);
            if($localResults)
              while ($rep = mysql_fetch_array($localResults)) {
              	$ami = $this->idToUser($rep["idami"]);
              	$friend = new Data_Friend($u, $ami, $rep["status"], $rep["date"]);
            	$result[] = $friend;
              }
              
          }
          return $result;
        }
        
		/**
         * pour obtenir la liste des amis d'un utilisateur (ici $u sera l'instagateur de la demande d'amitié)
         * @param Data_User $u : l'utilisateur
         * @param string $status : paramètre optionnel permettant de filtrer les resultats en fonction du statut
         * @return array un tableau  de Data_Friend (qui peut etre vide)
         */
        public function getAmisEnAttente(Data_User $u) {
          $result = array();
         
          $status= Data_STATUS::getEnum()->ATTENTE;

          $uid = $this->userToId($u);

          if ($uid) {
          	$u->setId($uid);
            $query = "SELECT * FROM estami where iduser=\"$uid\"  and status=\"$status\" ";
         
            $localResults = mysql_query($query, $this->connexion);
            if($localResults)
              while ($rep = mysql_fetch_array($localResults)) {
              	$ami = $this->idToUser($rep["idami"]);
              	$friend = new Data_Friend($u, $ami, $rep["status"], $rep["date"]);
            	$result[] = $friend;
              }
              
          	
          }
          return $result;
        }

        /**
         * Permet de verifier si deux utilisateurs sont amis
         * @param Data_User $u : l'utilisateur
         * @param Data_User $ami : l'ami potentiel
         * @param $status : paramètre optionnel permettant de specifier les resultats en fonction du statut
         * @return boolean : la valeur de retour est false ou le status de l'amitié
         */
        public function isAmi(Data_User $u, Data_User $ami, $status="") {

          if ($status && !Data_STATUS::isInEnum($status)) return false;

          $uid = $this->userToId($u);
          $uidami = $this->userToId($ami);

          if ($uid && $uidami) {
            $query = "SELECT * FROM estami where iduser=\"$uid\" and idami=\"$uidami\"".(($status)?" and status=\"$status\"":"");
            
            $localResults = mysql_query($query, $this->connexion);
            if($localResults) if ($rep = mysql_fetch_array($localResults)) return $rep["status"];
              
          }
          return false;
        }
        
        
//		/**
//         * Permet de verifier si deux utilisateurs sont en relation : l'un ou l'autre peuvent être initiateur de l'amitié
//         * @param Data_User $u : l'utilisateur
//         * @param Data_User $ami : l'ami potentiel
//         * @param $status : paramètre optionnel permettant de specifier les resultats en fonction du statut
//         * @return boolean
//         */
//        public function isEnRelation(Data_User $u, Data_User $ami, $status="") {
//
//          if ($status && !Data_STATUS::isInEnum($status)) return false;
//
//          $uid = $this->userToId($u);
//          $uidami = $this->userToId($ami);
//
//          if ($uid && $uidami) {
//            $query = "SELECT * FROM estami where (iduser=\"$uid\" and idami=\"$uidami\"".(($status)?" and status=\"$status\"":"").") OR (iduser=\"$uidami\" and idami=\"$uid\"".(($status)?" and status=\"$status\"":"").")";
//            
//            $localResults = mysql_query($query, $this->connexion);
//            if($localResults) if ($rep = mysql_fetch_array($localResults)) return true;
//              
//          }
//          return false;
//        }

    	/**
         * Envoie d'une invitation a un utilisateur pour devenir ami
         * @param Data_User $u : l'utilisateur qui envoie la demande
         * @param Data_User $ami : l'utilisateur qui recoit la demande
         * @return boolean vrai si l'ajout a fonctionne, faux sinon
         */
        public function sendInvitationAmi(Data_User $u, Data_User $ami)
        {
          $result = false;
          if ($u == $ami || $this->isAmi($u, $ami)) return $result;
         
          $uid = $this->userToId($u);
          $uidami = $this->userToId($ami);

          if ($uid && $uidami && ($uid != $uidami)) {
          	$date = date("Y-m-d");
            $insert = "INSERT INTO estami VALUES (\"$uid\", \"$uidami\", \"".Data_STATUS::getEnum()->ATTENTE."\", \"$date\"), (\"$uidami\", \"$uid\", \"".Data_STATUS::getEnum()->DEMANDE."\", \"$date\")";
            $result = mysql_query($insert, $this->connexion);
          }

          return $result;
        }

    	/**
         * permet de supprimer une invitation/demande/ami d'un utilisateur
         * @param Data_User $u : celui qui souhaite supprimer
         * @param Data_User $ami : l'utilisateur a supprimer
         * @return boolean
         */
        public function deleteAmi(Data_User $u, Data_User $ami)
        {
          $result = false;
          if (!$this->isAmi($u, $ami)) return $result;

          $uid = $this->userToId($u);
          $uidami = $this->userToId($ami);
         
          if ($uid && $uidami) {
            $delete = "DELETE FROM `estami` WHERE (iduser=\"$uid\" and idami=\"$uidami\") or (iduser=\"$uidami\" and idami=\"$uid\")";
            $result = mysql_query($delete, $this->connexion);
          }

          return $result;
        }

    	/**
         * pour accepter une invitation d'ami
         * @param Data_User $u l'utilisateur connecté
         * @param Data_User $ami l'utilisateur demandeur
         * @return boolean : vrai en cas de succès dans la mise à jour
         */
        public function acceptInvitationAmi(Data_User $u, Data_User $ami)
        {
          $result = false;
          if (!$this->isAmi($u, $ami, Data_STATUS::getEnum()->DEMANDE)) return $result;
         
          $uid = $this->userToId($u);
          $uidami = $this->userToId($ami);

          if ($uid && $uidami) {
            $insert = "UPDATE estami SET status=\"".Data_STATUS::getEnum()->ACCEPTE."\" where (iduser=\"$uid\" and idami=\"$uidami\") or (iduser=\"$uidami\" and idami=\"$uid\")";
            $result = mysql_query($insert, $this->connexion);
          }
         
          return $result;
        }
	
}
?>