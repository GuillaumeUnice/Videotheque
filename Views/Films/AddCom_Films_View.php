<?php

class AddCom_Films_View extends Lightbox_Global_View {

    private $form;

    public function AddCom_Films_View($viewparams) {

        $filmscss = array("films.css", "form.css", "chosen.css");
        $this->setCSS($filmscss);

        $filmsjs = array("chosen.jquery.js");
        $this->setJS($filmsjs);

        if (isset($viewparams["erreur"]) && strcmp($viewparams["erreur"], "") != 0) {
            $this->erreur = $viewparams["erreur"];
        }
        if (isset($viewparams["ajouter"]) && strcmp($viewparams["ajouter"], "") != 0) {
            $this->ajouter = $viewparams["ajouter"];
        }

        $this->film = $viewparams['film'];
    }

    public function mainContent() {

        ob_start();
        ?>

        <h1>Formulaire Note</h1>
        <?php
        if (isset($this->ajouter)) {
            echo '<div id="correct"><p>' . utf8_encode($this->ajouter) . '</p></div>';
            ?>					
            <script type="text/javascript">
                $(document).ready(function() {
                    $.ajax({
                        type: 'GET',
                        url: './index.php?controller=Films&action=printFilm&id=' + <?php echo $_GET['id']; ?>,
                        timeout: 3000,
                        success: function(data) {
                            parent.$("#" + <?php echo $_GET['id']; ?>).replaceWith(data);
                        },
                        error: function() {
                            alert('La requête n\'a pas abouti');
                        }
                    });
                });
            </script>
            <?php
        }
        if (isset($this->erreur)) {
            echo '<div id="erreur"><p>' . utf8_encode($this->erreur) . '</p></div>';
        } else if (!isset($this->film)) {
            ?>
            <div id="erreur"><p>erreur film introuvable!</p></div>
            <?php
        } else {
            ?>			
            <form action="./index.php?controller=Films&action=addCom&id=<?php echo $this->film->getId(); ?>" method="post" >
                <div class="border_form">
                    <p>
                        <label for="newcom">Commentaire : </label>

                        <textarea id="newcom" name="com" rows=4 cols=50 onclick="this.focus();
                                this.select()">Ajouter un commentaire...</textarea>

                </div>
                <p style="margin-left: 25px;">
                    Envoyer : <input type="submit" value="Validez" />
                </p>

            </form>
            <?php
        }
        $content = ob_get_contents();
        ob_end_clean();

        return $content;
    }

}
?>