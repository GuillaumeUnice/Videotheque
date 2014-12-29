<?php

/**
 * 
 * @author renevier-gonin, Franck Berthelon
 * @package Data_
 * 
 * @abstract pour manipuler les réseaux de connaissance.
 * 
 * 
 */
class Data_Friend {
	/**
	 * l'utilisateur
	 * @var Data_User
	 */
	protected $user;
	/**
	 * l'ami
	 * @var Data_User
	 */
	protected $ami;
	/**
	 * le status, cf DATA_STATUS
	 * @var string
	 */
	protected $status;
	/**
	 * la date d'enregistrement de "l'amitié"
	 * @var string
	 */
	protected $date; // année d'enregistrement
	
	/**
	 * 
	 * @param Data_User $u
	 * @param Data_User $f
	 * @param string $status
	 * @param string $date
	 */
	public function __construct(Data_User $u = null, Data_User $f = null , $status ="ATTENTE", $date = "")
	{
		if ($u) $this->setUser($u);
		if ($f) $this->setAmi($f);
		if ($date) {
			$this->setDate($date);
		}
		else { 
			$this->setDate(date("Y-m-d"));
		}
		if ($status) $this->setStatus($status);
	}

	

	/**
	 * @param string $status (type ENUM STATUS) est le status de l'amitié
	 */
	public function setStatus( $status) {
		if (Data_STATUS::isInEnum($status)) {
						$this->status = $status;
		}
	}
	
	/**
	 * 
	 * @param Data_User $u :la personne qui a fait la demande de lien d'amitié 
	 */
	public function setUser(Data_User $u) { $this->user = $u; }
	
	/**
	 * 
	 * @param Data_User $f :la personne qui le "user" est ami (a fait se demande d'amitié) 
	 */
	public function setAmi(Data_User $f) { $this->ami = $f; }
	
	
	/**
	 * la date est au format AAAA-MM-JJ, l'année devant être 19xx ou 200x ou 201x
	 * @param string $date : la date de la dernière modification (ou création) de l'amitié
	 */
	public function setDate($date)
	{
		$d = trim($date);
		$nb = preg_match("/(19[0-9]|20[0-1])[0-9]-(0[1-9]|1[0-2])-([0-2][0-9]|3[0-1])/", $d);
				if ($nb == 1)
		{
			$this->date = $d;
		}
		
	}
	
	/**
	 * retourne la date de la dernière modification de l'amitié
	 * @return string
	 */
	public function getDate()
	{
		return $this->date;
	}
	
	
	/**
	 * @return Data_User
	 */
	public function getUser()
	{
		return $this->user;
	}
	
	/**
	 * @return Data_User
	 */
	public function getAmi()
	{
		return $this->ami;
	}
	
	/**
	 * @return string
	 */
	public function getStatus()
	{
		return $this->status;
	}
	
	
	
	/** 
	 * test l'égalité, sur les utilisateurs et le status... pas sur la date.
	 */
	public function equals($obj) {
		if ($obj instanceof Data_Friend) {
			$result = true;
			
			$result = ($this->user->equals($obj->user)) && ($this->ami->equals($obj->ami));
			$result = $result || (($this->user->equals($obj->ami)) && ($this->ami->equals($obj->user)));
			$result = $result && ($this->status == $obj->status);
			
			
			return result;
		}
		else return ($obj == $this);
	}
	
	/**
	 * @ignore
	 */
	public function __toString() {
		$result =  $this->user." est ami (".$this->status.") avec ".$this->ami." depuis ".$this->date;
		return $result;
	}
}
?>