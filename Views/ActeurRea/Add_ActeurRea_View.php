<?php
/**
 * Created by IntelliJ IDEA.
 * User: Quentin
 * Date: 21/05/14
 * Time: 12:56
 */

class Add_ActeurRea_View  extends Lightbox_Global_View {


    public function Add_ActeurRea_View($viewparams) {
        $acteurReacss = array("form.css", "chosen.css");
        $this->setCSS($acteurReacss);

        $acteurReajs = array("chosen.jquery.js");
        $this->setJS($acteurReajs);


        if(isset($viewparams["erreur"]) && strcmp($viewparams["erreur"], "") != 0) {
            $this->erreur = $viewparams["erreur"];
        }
        if (isset($viewparams["ajouter"])&& strcmp($viewparams["ajouter"], "") != 0) {
            $this->ajouter = $viewparams["ajouter"];
        }

        $this->acteurRea = $viewparams["film"];
    }




    /**
     * Permet d'obtenir le contenu principal de la page.
     *
     * @return string
     */
    public function mainContent() {
        ob_start();
        ?>

        <form action="./index.php?controller=ActeurRea&action=addActeurRea" method="post" >

            <h1>Formulaire Acteur / Realisateur</h1>
            <?php
            if(isset($this->ajouter)) {
                echo '<div id="correct"><p>' . utf8_encode($this->ajouter) . '</p></div>';

            }
            if(isset($this->erreur))
                echo '<div id="erreur"><p>' . utf8_encode($this->erreur) . '</p></div>';
            ?>
            <div class="border_form">
                <p>
                    <label for="titre">Nom : </label><input type="text" id="nom" name="nom" placeholder="Tapez ici!" maxlength="255" autofocus required/>
                    <label for="titre">Prenom : </label><input type="text" id="prenom" name="prenom" placeholder="Tapez ici!" maxlength="255" autofocus required/>
                    Fonction :
                    <label for="acteur">Acteur</label>
                    <input id="acteur" type="radio" name="rolePeople" value="a" checked>
                    <label for="real">Réalisateur</label>
                    <input id="real" type="radio" name="rolePeople" value="r">

                </p>
                <p>
                    <label for="nationalite">Nationalite : </label>
                    <select name="nationalite" id="nationalite" >
                        <?php
                        // Affichage des nationnalite
                        $tabNationalite = Data_NATIONALITE::getValues();
                        foreach($tabNationalite as $key => $value) {
                            echo '<option value="' . $value . '" >' . utf8_encode(strtolower($value)) . '</option>';
                        }
                        ?>
                    </select>
                    Date de Naissance : <input type="number" name="annee" min="1900" max="<?php echo (date('Y') + 10); ?>" value="<?php echo date('Y'); ?>">
                </p>
                Sexe :
                <label for="Homme">Homme</label>
                <input id="Homme" type="radio" name="sexPeople" value="m" checked>
                <label for="Femme">Femme</label>
                <input id="Femme" type="radio" name="sexPeople" value="f">

                <p  class="film">
                    <label for="film1">Film 1 : </label>
                    <select name="film1" id="film1" placeholder="Selectionner des films" ><!--name="acteur[]" id="acteur" data-placeholder="Selectionner des acteurs" multiple class="chosen-select" >-->
                        <option value="null" >Aucun</option>
                        <?php
                        foreach ($this->acteurRea as $key => $value) {
                            echo '<option value ="' . $key . '">' . utf8_encode($value) . '</option>';
                        }
                        ?>

                    </select>
                    <button type="button" id="addFilm">Ajouter</button>
                </p>

                <p  class="portrait">
                    <label for="portrait1">Portrait 1 : </label><input type="text" id="portrait1" name="portrait1" placeholder="url/nom de depuis la racine du fichier"/>
                    <button type="button" id="addPortrait">Ajouter</button>
                </p>
                <!--<label for="affiche">Affiche 1 : </label><input id="affiche" type="file" name="affiche" />-->
            </div>
            <p style="margin-left: 25px;">
                Envoyer : <input type="submit" value="Validez" />
            </p>
            <p>
                Effacer tout : <input type="reset" value="Effacer"/>
            </p>
        </form>

        <script>
            $(document).ready(function() {
                var c = 1;
                $("#addFilm").on('click',function(){

                    //ajout du select film
                    var $orginal = $('#film1');
                    var $cloned = $orginal.clone();
                    $cloned//.attr('id', 'film'+(++c) );
                        .attr({
                            id: 'film'+(++c),
                            name: 'film'+ c
                        });

                    //$cloned.appendTo('.film');
                    $( "#addFilm" ).before("<br /><label for=\"film" + c + "\">film " + c + " : </label>");
                    $("#addFilm").before($cloned);
;
                });
            });

            var nbAffich = 1;
            var nameId = "portrait";
            $("#addPortrait").on('click',function(){

                //ajout de l'input text
                var $orginal = $('#portrait1');
                var $cloned = $orginal.clone();
                $cloned.attr({
                    id: nameId + (++nbAffich),
                    name: nameId + nbAffich
                });
                $cloned.val('');

                $( "#addPortrait" ).before("<br /><label for=\"" + nameId + nbAffich + "\">" + nameId + " " + nbAffich + " : </label>");
                //$( ".portrait" ).append("<label for=\"" + nameId + nbAffich + "\">" + nameId + " " + nbAffich + " :</label>");
                $("#addPortrait").before($cloned);


            }); // $("#addAffich").on('click')

        </script>
        <?php

        $content = ob_get_contents();
        ob_end_clean();

        return $content;
    }
}
?>
