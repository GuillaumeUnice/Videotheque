<?php
class Update_ActeurRea_View extends Lightbox_Global_View {


    public function Update_ActeurRea_View($viewparams) {
        $acteurReacss = array("films.css", "form.css", "chosen.css", "lightbox.css");
        $this->setCSS($acteurReacss);

        $filmsjs = array("chosen.jquery.js");
        $this->setJS($filmsjs);


        if(isset($viewparams["erreur"]) && strcmp($viewparams["erreur"], "") != 0) {
            $this->erreur = $viewparams["erreur"];
        }
        if (isset($viewparams["update"])) {
            $this->update = $viewparams["update"];
        }

        $this->personne = $viewparams['personne'];
        $this->portrait = $viewparams['portrait'];
    }

    public function mainContent() {
        ob_start();
        ?>

        <h1>Formulaire d'Edition d'acteur / réalisateur</h1>

        <?php
        if(isset($this->update)) {
            ?>
            <script type="text/javascript">
                $(document).ready(function() {
                    $.ajax({
                        type: 'GET',
                        url: './index.php?controller=ActeurRea&action=printFilm&id=' + <?php echo $_GET['id']; ?>,
                        timeout: 3000,
                        success: function(data) {
                            parent.$("#" + <?php echo $_GET['id']; ?>).replaceWith(data);
                        },
                        error: function() {
                            alert('La requête n\'a pas abouti'); }
                    });
                });
            </script>
            <?php
            echo '<div id="correct"><p>' . utf8_encode($this->update) . '</p></div>';
        }
        if(isset($this->erreur))
            echo '<div id="erreur"><p>' . utf8_encode($this->erreur) . '</p></div>';

        if(! $this->personne) {
            echo '<div id="erreur"><p>L\'acteur / réalisateur n\'a pas pu être trouvé une erreur c\'est produite</p><div>';
        } else {
            ?>
            <form action="./index.php?controller=ActeurRea&action=edit<?php echo '&id=' . $_GET['id']?>" method="post">
                <div class="border_form">

                    <p>
                        <label for="nom">Nom : </label><input type="text" id="nom" name="nom" placeholder="Tapez ici!" maxlength="255" autofocus value="<?php echo $this->personne->getNom(); ?>" required/>
                        <label for="prenom">Prenom : </label><input type="text" id="prenom" name="prenom" placeholder="Tapez ici!" maxlength="255" autofocus value="<?php echo $this->personne->getPrenom(); ?>"required/>
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
                            $tabNationalite = Data_NATIONALITE::getValues();
                            foreach($tabNationalite as $key => $value) {
                            echo '<option value="' . $value . '" ';
                            if($this->personne->getNationalite() == $value) echo 'selected';
                            echo ' >' . utf8_encode(strtolower($value)) . '</option>';
                            }
                            ?>
                        </select>
                        Date de Naissance : <input type="number" name="annee" min="1900" max="<?php echo (date('Y') + 10); ?>" value="<?php echo $this->personne->getNaissance(); ?>">
                    </p>
                    Sexe :
                    <label for="Homme">Homme</label>
                    <input id="Homme" type="radio" name="sexPeople" value="m" <?php if($this->personne->isSexeFeminin()) { } else {echo('checked');} ?> >
                    <label for="Femme">Femme</label>
                    <input id="Femme" type="radio" name="sexPeople" value="f" <?php if($this->personne->isSexeFeminin()) { echo('checked');}?> >



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
}
?>