<?php

class ListAutoload_Films_View extends Main_Global_View {

    var $films;

    public function ListAutoload_Films_View($viewparams) {
        $filmscss = array("films.css", "list.css");
        $this->setCSS($filmscss);

        $this->setFilms($viewparams["films"]);
        $this->maincontent = new Print_Films_View($viewparams);
        $this->menu = new Menu_Films_View($viewparams);
    }

    public function setFilms($films) {
        $this->films = $films;
    }

    public function mainContent() {
        ob_start();
        ?>
        <div id="nav_left">
            <?php $this->menu->mainContent(); ?>
        </div>
        <articles id="liste">
            <?php $this->maincontent->mainContent(); ?>
        </articles>
        <input id="smb" type="button" value="Plus de résultats" />
        <script type="text/javascript">
            $(document).ready(function() {
                $("#smb").click(function() {
                    $.ajax({
                        type: 'GET',
                        url: './index.php?controller=Films&action=printNextFilmAutoload',
                        timeout: 3000,
                        success: function(data) {
                            $("#liste").append(data);
                        },
                        error: function() {
                            alert('La requête n\'a pas abouti');
                        }
                    })
                });
                $("ul#menu_annee li").click(function(e) {
                    $("#nav_left li").removeClass('selected');
                    $(this).addClass('selected');
                    $.ajax({
                        type: 'GET',
                        url: './index.php?controller=Films&action=printFilmsAnnee&annee=' + $(this).attr("value"),
                        timeout: 3000,
                        success: function(data) {
                            $("#liste").html(data);
                        },
                        error: function() {
                            alert('La requête n\'a pas abouti');
                        }
                    })
                });
                $("ul#menu_genres li").click(function() {
                    if (!$(this).attr('value'))
                        return;
                    $("#nav_left li").removeClass('selected');
                    $(this).addClass('selected');
                    $.ajax({
                        type: 'GET',
                        url: './index.php?controller=Films&action=printFilmsGenre&genre=' + $(this).attr('value'),
                        timeout: 3000,
                        success: function(data) {
                            $("#liste").html(data);
                        },
                        error: function() {
                            alert('La requête n\'a pas abouti');
                        }
                    })
                });
                $('#search').keyup(function(e) {
                    if (e.keyCode == 13 && $('#search').val().length != 0) {
                        $.ajax({
                            type: 'GET',
                            url: './index.php?controller=Films&action=rechercheTitreFilm&recherche=' + $('#search').val(),
                            timeout: 3000,
                            success: function(data) {
                                $("#liste").html(data);
                            },
                            error: function() {
                                alert('La requête n\'a pas abouti');
                            }
                        })
                    }
                });
            });
        </script>
        <?php
        $content = ob_get_contents();
        ob_end_clean();

        return $content;
    }

}
?>