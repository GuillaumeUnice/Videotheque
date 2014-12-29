$(document).ready(function() {

$("ul#menu_annee li").click(function(e) {
                            $("#nav_left li").removeClass('selected');
                            $(this).addClass('selected');
                            $.ajax({
                                type: 'GET',
                                url: './index.php?controller=Films&action=printFilmsAnnee&annee=' + $(this).attr("value"),
                                timeout: 3000,
                                success: function (data) {
                                    $("#liste").html(data);
                                },
                                error: function () {
                                    alert('La requête n\'a pas abouti');
                                }
                            })
                    });
                    $("ul#menu_genres li").click(function() {
                        if(!$(this).attr('value')) return;
                        $("#nav_left li").removeClass('selected');
                        $(this).addClass('selected');
                        $.ajax({
                            type: 'GET',
                            url: './index.php?controller=Films&action=printFilmsGenre&genre='+$(this).attr('value'),
                            timeout: 3000,
                            success: function (data) {
                                $("#liste").html(data);
                            },
                            error: function () {
                                alert('La requête n\'a pas abouti');
                            }
                        })
                    });
                    $('#search').keyup(function (e) {
                        if (e.keyCode == 13 && $('#search').val().length != 0) {
                            $.ajax({
                                type: 'GET',
                                url: './index.php?controller=Films&action=rechercheTitreFilm&recherche='+$('#search').val(),
                                timeout: 3000,
                                success: function (data) {
                                    $("#liste").html(data);
                                },
                                error: function () {
                                    alert('La requête n\'a pas abouti');
                                }
                            })
                        }
                    });
				
	var c = 1;
	$("#addActeur").on('click',function(){
	
			//ajout du select acteur
			var $orginal = $('#acteur1');
			var $cloned = $orginal.clone();
			$cloned
			.attr({
				id: 'acteur'+(++c),
				name: 'acteur'+ c
			});
			
			$( "#addActeur" ).before("<br /><label for=\"acteur" + c + "\">Acteur " + c + " : </label>");
			///$( ".acteur" ).append("<label for=\"acteur" + c + "\">Acteur " + c + " :</label>");
			$("#addActeur").before($cloned);//$cloned.appendTo('.acteur');
			
			
			//ajout du role lié au select
			var $orginal = $('#role1');
			var $cloned = $orginal.clone();
			$cloned.attr({ id: 'role'+ c, name: 'role'+ c });
			$cloned.val('');
			$( "#addActeur" ).before("<label for=\"role" + c + "\"> Rôle de l'acteur : </label>");
			$("#addActeur").before($cloned);
			
	}); // $("#addActeur").on('click')
					
	var nbAffich = 1;
	var nameId = "affiche";
	$("#addAffiche").on('click',function(){
	
			//ajout de l'input text
			var $orginal = $('#affiche1');
			var $cloned = $orginal.clone();
			$cloned.attr({
				id: nameId + (++nbAffich),
				name: nameId + nbAffich
			});
			$cloned.val('');
			
			$( "#addAffiche" ).before("<br /><label for=\"" + nameId + nbAffich + "\">" + nameId + " " + nbAffich + " : </label>");
			$("#addAffiche").before($cloned);
			
			
	}); // $("#addAffich").on('click')
});