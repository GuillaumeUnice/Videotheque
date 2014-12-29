<?php

/**
 * Created by IntelliJ IDEA.
 * User: Quentin
 * Date: 15/05/14
 * Time: 13:19
 */
class ListAll_ActeurRea_View extends Main_Global_View
{

    private $people = array();

    public function ListAll_ActeurRea_View($viewparams)
    {
        $filmscss = array("acteurRea.css");
        $this->setCSS($filmscss);

        $this->setPeople($viewparams["acteurRea"]);
        $this->maincontent = new Print_ActeurRea_View($viewparams);
    }

    public function setPeople($people)
    {
        $this->people = $people;
    }

    public function mainContent()
    {
        ob_start();
        if (!$this->people) {
            ?>
            <article>
                Aucun acteur/r&eacute;alisteur n'est pr&eacute;sent dans la base de donn&eacute;es.
            </article>
        <?php } ?>
        <div id="liste">
            <?php $this->maincontent->mainContent(); ?>
        </div>
        <script type="text/javascript">
            $(document).ready(function () {
                $('#search').keyup(function (e) {
                    if (e.keyCode == 13 && $('#search').val().length != 0) {
                        $.ajax({
                            type: 'GET',
                            url: './index.php?controller=ActeurRea&action=recherche&recherche=' + $('#search').val(),
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
             });
        </script>
        <?php
        $content = ob_get_contents();
        ob_end_clean();

        return $content;
    }
} 