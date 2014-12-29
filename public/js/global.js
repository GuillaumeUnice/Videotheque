$(document).ready(function(){
    $("#menu").sticky({topSpacing:0});
	
	function duplicateInput (element, namePlace, nameInput) {
		var $nb = 1;
		
		var $orginal = $('#' + nameInput + $nb);
		var $cloned = $orginal.clone();
		$cloned.attr({ id: nameInput + (++$nb), name: nameInput + $nb });
		$cloned.val('');
		$('#' + namePlace).before('<label for=\"' + nameInput + $nb + '\"> Rôle de l\'acteur : </label>');
		$('#' + namePlace).before($cloned);
	}
	
});