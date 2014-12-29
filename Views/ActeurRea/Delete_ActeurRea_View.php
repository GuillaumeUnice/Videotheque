<?php
class Delete_ActeurRea_View extends Lightbox_Global_View {


    public function Delete_ActeurRea_View($viewparams) {
        $filmscss = array("films.css", "form.css", "lightbox.css");
        $this->setCSS($filmscss);


        if(isset($viewparams["erreur"])) {
            $this->erreur = $viewparams["erreur"];
        }
        if (isset($viewparams["delete"])) {
            $this->id = $viewparams['id'];
            $this->delete = $viewparams["delete"];
        }

        $this->people = $viewparams['acteurRea'];

    }

    public function mainContent() {
        ob_start();
        ?>

        <h1>Suppression d'acteur / réalisateur</h1>
        <?php

        if(isset($this->erreur)) {
            echo '<div id="erreur"><p>' . utf8_encode($this->erreur) . '</p></div>';
        }
        if(isset($this->delete)) {
            ?>
            <script type="text/javascript">
                $(document).ready(function() {
                    parent.$("#" + <?php echo $this->id; ?>).remove();
                });
            </script>
            <?php
            echo '<div id="correct"><p>' . utf8_encode($this->delete) . '</p></div>';
        } elseif(! $this->people) {
            echo '<div id="erreur"><p>L\'acteur réalisteur  n\'a pas pu être trouvé une erreur c\'est produite il ne peut donc être supprimé!!!</p><div>';
        } else {
            ?>

            <div style="text-align: center;">
                <p>
                    Etes vous certains de supprimer le film : <?php echo utf8_encode($this->people->getPrenom()); ?>  <?php echo utf8_encode($this->people->getNom()); ?> ? Cette action est irréversible!
                </p>
                <form action="./index.php?controller=ActeurRea&action=delete" method="post">
                    <input type="hidden" name="delete" value="<?php echo $this->people->getId(); ?>" />
                    <input type="submit" value="Oui" />
                    <button onclick="parent.jQuery.fancybox.close()" type="button" id="annuler">Annuler</button>

                </form>
            </div>
            <?php

            $content = ob_get_contents();
            ob_end_clean();

            return $content;
        }

    }
}
?>