<?php

class Films_Controller {

    /**
     * // public car requise par le parser
     * Permet d'afficher l'ensemble des films ainsi que leur informations annexes : réalisateur, acteurs, affiches, commentaires...
     * @param int $params["limit"] contenant une éventuelle limit pour la requete nombre a afficher
     * @return void et appelant la vue Delete_Films_View($viewparams)
     * $viewparams contenant : $viewparams["films"]
     */
    public function listAll($params) {
        $limit = 5;
        if (isset($params["limit"])) {
            $limit = (int) $params["limit"];
        }

        $films = Nf_FilmManagement::getInstance()->getFilms();

        function filmCmp($film1, $film2) {
            return strcmp($film1->getTitre(), $film2->getTitre());
        }

        uasort($films, 'filmCmp');

        foreach ($films as $key => $film) {
            $films[$key]->affiches = Nf_FilmManagement::getInstance()->getAffiches($film);
        }

        //Extraction des notes de la BDD pour en faire une moyenne
        foreach ($films as $key => $film) {

            $tabNote = Nf_CommNoteManagement::getInstance()->getNotesParFilm($film);

            if (count($tabNote) >= 1) { // si possède des notes alors calcul de la moyenne
                $sum = 0;

                foreach ($tabNote as $key => $data_Note) {

                    $sum += $data_Note->getNote();
                }
                $film->note = $sum / count($tabNote);
            }
            if ($res = Nf_CommNoteManagement::getInstance()->getCommentairesParFilm($film)) {
                $film->com = $res;
            }
            if (isset($_SESSION['nom'])) {
                $user = Nf_UserDvdManagement::getInstance()->idToUser($_SESSION['id']);
                if ($res = Nf_CommNoteManagement::getInstance()->getNotesParFilm($film, $user)) {
                    $film->ANote = $res[0];
                }
            }
        }

        $viewparams["films"] = $films;
        $viewparams["items"] = $this->menuListe();

        $view = new ListAll_Films_View($viewparams);
        $view->display();
    }

    var $nb = 5;

    public function listAutoload($params) {
        $_SESSION["autoload_start"] = 0;
        if (isset($params["nb"])) {
            $this->nb = (int) $params["nb"];
        }
        $films = Nf_FilmManagement::getInstance()->getFilmsAtoB($_SESSION["autoload_start"], $this->nb);
        $_SESSION["autoload_start"] += $this->nb;

        function filmCmp($film1, $film2) {
            return strcmp($film1->getTitre(), $film2->getTitre());
        }

        uasort($films, 'filmCmp');

        foreach ($films as $key => $film) {
            $films[$key]->affiches = Nf_FilmManagement::getInstance()->getAffiches($film);
        }

        //Extraction des notes de la BDD pour en faire une moyenne
        foreach ($films as $key => $film) {

            $tabNote = Nf_CommNoteManagement::getInstance()->getNotesParFilm($film);

            if (count($tabNote) >= 1) { // si possède des notes alors calcul de la moyenne
                $sum = 0;

                foreach ($tabNote as $key => $data_Note) {

                    $sum += $data_Note->getNote();
                }
                $film->note = $sum / count($tabNote);
            }

            if ($res = Nf_CommNoteManagement::getInstance()->getCommentairesParFilm($film)) {
                $film->com = $res;
            }

            if (isset($_SESSION['nom'])) {
                $user = Nf_UserDvdManagement::getInstance()->idToUser($_SESSION['id']);
                if ($res = Nf_CommNoteManagement::getInstance()->getNotesParFilm($film, $user)) {
                    $film->ANote = $res[0];
                }
            }
        }

        $viewparams["films"] = $films;
        $viewparams["items"] = $this->menuListe();

        $view = new ListAutoload_Films_View($viewparams);
        $view->display();
    }

    public function printNextFilmAutoload($params) {
        if (isset($params["nb"])) {
            $this->nb = (int) $params["nb"];
        }
        $films = Nf_FilmManagement::getInstance()->getFilmsAtoB($_SESSION["autoload_start"], $this->nb);
        $_SESSION["autoload_start"] += $this->nb;

        function filmCmp($film1, $film2) {
            return strcmp($film1->getTitre(), $film2->getTitre());
        }

        uasort($films, 'filmCmp');

        foreach ($films as $key => $film) {
            $films[$key]->affiches = Nf_FilmManagement::getInstance()->getAffiches($film);
        }

        //Extraction des notes de la BDD pour en faire une moyenne
        foreach ($films as $key => $film) {

            $tabNote = Nf_CommNoteManagement::getInstance()->getNotesParFilm($film);

            if (count($tabNote) >= 1) { // si possède des notes alors calcul de la moyenne
                $sum = 0;

                foreach ($tabNote as $key => $data_Note) {

                    $sum += $data_Note->getNote();
                }
                $film->note = $sum / count($tabNote);
            }
            if ($res = Nf_CommNoteManagement::getInstance()->getCommentairesParFilm($film)) {
                $film->com = $res;
            }
            if (isset($_SESSION['nom'])) {
                $user = Nf_UserDvdManagement::getInstance()->idToUser($_SESSION['id']);
                if ($res = Nf_CommNoteManagement::getInstance()->getNotesParFilm($film, $user)) {
                    $film->ANote = $res[0];
                }
            }
        }

        $viewparams["films"] = $films;
        $view = new Print_Films_View($viewparams);
        $view->display();
    }

    /**
     * Effectue les requêtes SQL permettant de récupérer les statistiques
     * <br />Notament utilisée pour génerer le menu
     * @return array
     */
    private function menuListe() {
        $genres = array();
        foreach (Data_STYLE::getValues() as $style) {
            $genre["name"] = $style;
            array_push($genres, $genre);
        };

        return array("genres" => $genres);
    }

    public function addNote($params) {

        $data_Film = Nf_FilmManagement::getInstance()->idToFilm($params["id"]);
        if (isset($_POST['add'])) { // dans ce cas formulaire a traité
            $user = Nf_UserDvdManagement::getInstance()->idToUser($_SESSION['id']);

            $data_Note = new Data_Note($user, $data_Film, $_POST['add'], null);

            if (Nf_CommNoteManagement::getInstance()->addNote($data_Note)) {
                $viewparams["ajouter"] = "La note a pu être ajoué";
            } else {
                $viewparams["erreur"] = "Une erreur c'est produite la note n'a pas pu être enregistrée";
            }
        }

        $viewparams['film'] = $data_Film;

        $view = new AddNote_Films_View($viewparams);
        $view->display();
    }

//addNote()

    public function addCom($params) {

        $data_Film = Nf_FilmManagement::getInstance()->idToFilm($params["id"]);
        if (isset($_POST['com'])) { // dans ce cas formulaire a traité
            $user = Nf_UserDvdManagement::getInstance()->idToUser($_SESSION['id']);
            $data_Com = new Data_Commentaire($user, $data_Film, $_POST['com'], null);
            if (Nf_CommNoteManagement::getInstance()->addCommentaire($data_Com)) {
                $viewparams["ajouter"] = "Le commentaire a &eacute;t&eacute; ajout&eacute;";
            } else {
                $viewparams["erreur"] = "Une erreur c'est produite le commentaire n'a pas pu &ecirc;tre enregistr&eacute;";
            }
        }

        $viewparams['film'] = $data_Film;

        $view = new AddCom_Films_View($viewparams);
        $view->display();
    }

    public function deleteNote($params) {
        $data_Note = Nf_CommNoteManagement::getInstance()->idToNote($params['id']);
        $data_Film = new Data_Film();
        $data_Film = Nf_FilmManagement::getInstance()->idToFilm($params["idFilm"]);
        $data_Film->note = $data_Note;
        if (isset($_POST['delete'])) { // dans ce cas formulaire a traité


            /* $viewparams['id'] = $_POST['delete'];
              $data_Film = Nf_FilmManagement::getInstance()->idToFilm($_POST['delete']); */
            if (Nf_CommNoteManagement::getInstance()->removeNote($data_Note)) {
                $viewparams["delete"] = "La note a été supprimée";
            } else {
                $viewparams["erreur"] = "Une erreur c'est produite la note n'a pas pu être supprimée";
            }
        }

        $viewparams['film'] = $data_Film;
        $view = new DeleteNote_Films_View($viewparams);

        $view->display();
    }

//deleteNote()

    /**
     * Permet de n'afficher que les films de l'année et les 9 années suivantes de celle donnée en paramètre
     * @param $params["annee"] le parametre GET de l'année de départ sur la tranche de 10 ans
     * @return array
     */
    public function printFilmsAnnee($params) {
        $films = array();
        $annee = 2000 + (date("Y") % 2000);
        if (isset($params["annee"])) {
            $annee = (int) $params["annee"];
        }
        for ($i = $annee; $i < $annee + 9; $i++) {
            $films = array_merge($films, Nf_FilmManagement::getInstance()->getFilmsParAnnee($i));
        }
        foreach ($films as $key => $film) {
            $films[$key]->affiches = Nf_FilmManagement::getInstance()->getAffiches($film);
        }

        $viewparams["films"] = $films;
        $view = new Print_Films_View($viewparams);
        $view->display();
    }

    /**
     * Permet de n'afficher que les films de l'année donnée en paramètre
     * @param $params["annee"] le parametre GET de l'année
     * @return array
     */
    public function printFilmsGenre($params) {
        $films = array();
        if (isset($params["genre"])) {
            $genre = $params["genre"];
            $films = Nf_FilmManagement::getInstance()->getFilmsParStyle($genre);
        }

        foreach ($films as $key => $film) {
            $films[$key]->affiches = Nf_FilmManagement::getInstance()->getAffiches($film);
        }

        $viewparams["films"] = $films;
        $view = new Print_Films_View($viewparams);
        $view->display();
    }

    /**
     * Permet d'enregistrer un film ainsi qu'éventuellement ses données annexes : réalisateur, acteurs et affiches liés
     * @param Data_Film $data_Film le film a enregistrer en BDD
     * @return $viewparams contenant : $viewparams["ajouter"], $viewparams["erreur"]
     */
    private function registerFilm(Data_Film $data_Film) {
        if (Nf_FilmManagement::getInstance()->addFilm($data_Film)) { // ajout en BDD du film
            $viewparams["ajouter"] = "Le film a pu être ajouté";
        } else {
            $viewparams["erreur"] = "Une erreur c'est produite vérifié que le film n'est pas déjà présent ou que l'ensemble des champs ont été correctement rempli";
        }
        return $viewparams;
    }

// registerFilm()

    /**
     * Permet d'ajouter un réalisateur a un film
     * @param Data_Film data_Film le film auquel il faut lier le réalisateur
     * @param int $realisateur l'id du realisateur
     * @return $viewparams contenant : $viewparams["ajouter"], $viewparams["erreur"]
     */
    private function registerRea(Data_Film $data_Film, $realisateur) {
        $data_People = Nf_FilmManagement::getInstance()->idToPeople($realisateur); //Va chercher la personne a lié en BDD
        $data_Rea = new Data_Realisateur($data_People);

        if (Nf_FilmManagement::getInstance()->addRealisateurAuFilm($data_Film, $data_Rea)) { // Lien du réalisateur et du film précédement ajouté
            $viewparams["ajouter"] = "<br /> le réalisateur a pu être lié au film";
        } else {
            $viewparams["erreur"] = "Le réalisateur n'a pu être lié au film";
        }

        return $viewparams;
    }

// registerRea()

    /**
     * Permet d'ajouter des acteurs a un film
     * @param Data_Film data_Film le film auquel il faut lier les roles
     * @param array(Data_Role) $data_Role les roles a lier au film
     * @return $viewparams contenant : $viewparams["ajouter"], $viewparams["erreur"]
     */
    private function registerActeur($data_Film, $data_Role) {
        foreach ($data_Role as $key => $value) {
            if (Nf_FilmManagement::getInstance()->addPersonnagesAuFilm($data_Film, $value)) {
                $viewparams["ajouter"] .= '<br />' . $value . ' a pu être lié au film';
            } else {
                $viewparams["erreur"] .= '<br />' . $value . 'n\'a pas pu etre lié une erreur c\'est produite';
            }
        }
        return $viewparams;
    }

// registerActeur()

    /**
     * Permet d'ajouter des affiches a un film
     * @param Data_Film data_Film le filmauquel il faut lier les affiches
     * @param string $data_Affiche contenant le lien absolue du fichier sur le bureau ou l\'url du fichier
     * @return $viewparams contenant : $viewparams["ajouter"], $viewparams["erreur"]
     */
    private function registerAffiche($data_Film, $data_Affiche) {
        foreach ($data_Affiche as $key => $value) {
            if (Nf_FilmManagement::getInstance()->addAfficheAuFilm($data_Film, $value)) {
                $viewparams["ajouter"] .= '<br />L\'affiche : ' . $value . ' a pu être ajouté';
            } else {
                $viewparams["erreur"] .= '<br />L\'affiche : ' . $value . 'n\'a pas pu être enregistré une lors du transfert du fichier a eu lieu vérifier que ce soit bien une image!';
            }
        }

        return $viewparams;
    }

// registerAffiche()

    /**
     * // public car requise par le parser
     * Permet d'ajouter un film ainsi qu'éventuellement ses données annexes : réalisateur, acteurs et affiches liés
     * @param void
     * @return void et appelant la vue Add_Films_View($viewparams)
     * $viewparams contenant : $viewparams["ajouter"], $viewparams["erreur"], $viewparams["acteur"][$value->getID()]
     * $viewparams["realisateur"][$value->getID()]
     */
    public function addFilm() {

        if (isset($_POST['titre'])) { // dans ce cas formulaire a traité
            //création de l'objet film
            $data_Film = $this->createFilm($_POST['annee'], $_POST['titre'], $_POST['style'], $_POST['langue'], $_POST['resume']);
            $viewparams = $this->registerFilm($data_Film);

            if (isset($_POST['realisateur']) && strcmp($_POST['realisateur'], 'null') != 0) { // si réalisateur a traité
                $param = $this->registerRea($data_Film, $_POST['realisateur']);
                $viewparams["ajouter"] .= $param["ajouter"];
                $viewparams["erreur"] = $param["erreur"];
            }

            // traitrement des éventuelle acteur
            $nb = 1;
            $data_Role = array();
            while (isset($_POST['acteur' . $nb])) { // acteur a traité
                if (strcmp($_POST['acteur' . $nb], 'null') != 0) { // si acteur selectionné
                    $data_Acteur = new Data_Acteur($data_People = Nf_FilmManagement::getInstance()->idToPeople($_POST['acteur' . $nb])); // Va chercher la personne a lié en BDD
                    if (isset($_POST['role' . $nb]) && strcmp($_POST['role' . $nb], '') != 0) { // si role rempli
                        array_push($data_Role, new Data_Role($_POST['role' . $nb], $data_Acteur)); // création du Data_Role
                    } else {
                        $viewparams["erreur"] = '<br />L\'acteur : ' . $data_Acteur->getNom() . ' ' . $data_Acteur->getPrenom() . 'n\'a pas pu être lié étant donné qu\'aucun role a été entré !';
                    }
                }
                ++$nb;
            }

            //ajout des acteurs
            $param = $this->registerActeur($data_Film, $data_Role);
            $viewparams["ajouter"] .= $param["ajouter"];
            $viewparams["erreur"] .= $param["erreur"];

            // traitement des éventuelles, affiches
            $nbAffiche = 1;
            $data_Affiche = array();
            while (isset($_POST['affiche' . $nbAffiche])) { // affiche a traité
                if (strcmp($_POST['affiche' . $nbAffiche], '') != 0) { // si affiche selectionné
                    array_push($data_Affiche, $_POST['affiche' . $nbAffiche]);
                }
                ++$nbAffiche;
            }

            // ajout des affiches
            $param = $this->registerAffiche($data_Film, $data_Affiche);
            $viewparams["ajouter"] .= $param["ajouter"];
            $viewparams["erreur"] .= $param["erreur"];
        } // fin de traitement du POST
        //Affichage des différents acteur a modifier pour pouvoir ajouter vraiment tout le monde a décommenté
        //$acteur = Nf_ActeurReaManagement::getInstance()->getPersonnes();
        $acteur = Nf_ActeurReaManagement::getInstance()->getActeurs();
        foreach ($acteur as $value) {
            $viewparams["acteur"][$value->getID()] = $value;
        }


        //Affichage des différents realisateur a modifier pour pouvoir ajouter vraiment tout le monde a décommenté
        //$realisateur = Nf_ActeurReaManagement::getInstance()->getPersonnes();
        $realisateur = Nf_ActeurReaManagement::getInstance()->getRealisateurs();
        foreach ($realisateur as $value) {
            $viewparams["realisateur"][$value->getID()] = $value;
        }

        $view = new Add_Films_View($viewparams);
        $view->display();
    }

// addFilm()

    /**
     * Permet de créer un object Data_Film
     * @param int $annee
     * @param string $titre
     * @param Data_STYLE $style
     * @param Data_LANGUE $langue
     * @param string $resume
     * @return Data_Film $data_Film l'objet film crée
     */
    private function createFilm($annee, $titre, $style, $langue, $resume) {
        //création de l'objet film
        $data_Film = new Data_Film();
        $data_Film->setAnnee($annee);
        $data_Film->setTitre($titre);
        $data_Film->setStyle($style);
        $data_Film->setLangue($langue);
        $data_Film->setResume($resume);

        return $data_Film;
    }

// createFilm()

    /**
     * Permet l'update d'un film
     * @param Data_Film $old_Film le film dont qui doit potentiellement être modifié
     * @param Data_Film $new_Film le nouveau film envoyer contenant d'éventuelles différences
     * @return $viewparams contenant notifications : $viewparams["update"], $viewparams["erreur"]
     */
    private function updateFilm($old_Film, $new_Film) {

        //Creation du nouveau film potentiellment différent
        //$new_Film = createFilm($_POST['annee'], $_POST['titre'], $_POST['style'], $_POST['langue'], $_POST['resume']);
        if (Nf_FilmManagement::getInstance()->updateFilm($old_Film, $new_Film)) {
            $viewparams["update"] = "Update du film réussie";
        } else {
            $viewparams["erreur"] = 'Erreur lors de l\'update du film';
        }

        return $viewparams;
    }

// updateFilm()

    /**
     * Permet l'update du réalisateur d'un film
     * @param Data_Film $data_Film le film dont on doit modifier le réalisateur
     * @param Data_Realisateur $new_Realisateur le nouveau réalisateur éventuellment différent de l'ancien
     * @param Data_Realisateur $old_Realisateur l'ancien réalisateur
     * @return $viewparams contenant notifications : $viewparams["update"], $viewparams["erreur"]
     */
    private function updateRealisateur($data_Film, $new_Realisateur, $old_Realisateur) {

        if (isset($old_Realisateur)) { // si le film avait un réalisateur
            if ($old_Realisateur->getId() != $new_Realisateur) { // si réalisateur différent
                $data_Realisateur = new Data_Realisateur($data_People = Nf_FilmManagement::getInstance()->idToPeople($old_Realisateur->getId())); // Va chercher la personne a lié en BDD
                //suppression de l\'ancien réalisateur
                if (Nf_FilmManagement::getInstance()->removeRealisateurDuFilm($data_Realisateur, $data_Film)) { // une fois supprimé on ajoute le nouveau réalisateur
                } else {
                    $viewparams["erreur"] = 'Erreur lors de l\'edition du realisateur';
                    return $viewparams;
                }
            } else {
                return;
            }
        }
        if (strcmp($new_Realisateur, 'null') != 0) {
            $params = $this->registerRea($data_Film, $new_Realisateur);
            if (isset($params["ajouter"]))
                $viewparams["update"] = $params["ajouter"];
            if (isset($params["erreur"]))
                $viewparams["erreur"] = $params["erreur"];
        }

        return $viewparams;
    }

// updateRealisateur

    /**
     * // public car requise par le parser
     * Permet d'editer un film, si ce dernier existe...
     * @param int $params["id"] l'id du film
     * @return void et appelant la vue Update_Films_View($viewparams)
     * $viewparams contenant : $viewparams['affiches'], $viewparams['film']
     * $viewparams["realisateur"][$value->getID()], $viewparams["acteur"][$value->getID()],
     * $viewparams["update"], $viewparams["erreur"]
     */
    public function edit($params) {


        // recuperation des données du films
        $data_Film = new Data_Film();
        $data_Film = Nf_FilmManagement::getInstance()->idToFilm($params["id"]);
        $viewparams['film'] = $data_Film;

        if (isset($_POST['titre'])) { // dans ce cas formulaire a traité
            $new_Film = $this->createFilm($_POST['annee'], $_POST['titre'], $_POST['style'], $_POST['langue'], $_POST['resume']);
            $params = $this->updateFilm($data_Film, $new_Film);
            if (isset($params["update"]))
                $viewparams["update"] = $params["update"];
            if (isset($params["erreur"]))
                $viewparams["erreur"] = $params["erreur"];

            //traitement du réalisateur
            $params = $this->updateRealisateur($new_Film, $_POST['realisateur'], $data_Film->getRealisateur());
            if (isset($params["update"]))
                $viewparams["update"] .= $params["update"];
            if (isset($params["erreur"]))
                $viewparams["erreur"] .= $params["erreur"];

            //traitement des acteurs existant
            $nbUpdateActeur = 1;
            $data_Role = array();
            while (isset($_POST['updateActeur' . $nbUpdateActeur])) { // acteur a traiter
                if ($_POST['updateActeur' . $nbUpdateActeur] != 0) { // alors supprimer Acteur
                    array_push($data_Role, $_POST['updateActeur' . $nbUpdateActeur]); // Va chercher la personne a lié en BDD
                }
                ++$nbUpdateActeur;
            }

            $params = $this->deleteActeur($data_Role, $data_Film);
            if (isset($params["delete"]))
                $viewparams["update"] .= '<br />' . $params["delete"];
            if (isset($params["erreur"]))
                $viewparams["erreur"] .= '<br />' . $params["erreur"];

            //traitement des affiches existantes
            $nbUpdateAffiche = 1;
            $data_Affiche = array();
            while (isset($_POST['updateAffiche' . $nbUpdateAffiche])) { // acteur a traiter
                if ($_POST['updateAffiche' . $nbUpdateAffiche] != 0) { // alors supprimer Acteur
                    array_push($data_Affiche, $_POST['updateAffiche' . $nbUpdateAffiche]); // Va chercher la personne a lié en BDD
                }
                ++$nbUpdateAffiche;
            }


            $params = $this->deleteAffiche($data_Affiche);
            if (isset($params["delete"]))
                $viewparams["update"] .= '<br />' . $params["delete"];
            if (isset($params["erreur"]))
                $viewparams["erreur"] .= '<br />' . $params["erreur"];

            // traitrement des éventuelles acteurs
            $nb = 1;
            $data_Role = array();
            while (isset($_POST['acteur' . $nb])) { // acteur a traité
                if (strcmp($_POST['acteur' . $nb], 'null') != 0) { // si acteur selectionné
                    $data_Acteur = new Data_Acteur($data_People = Nf_FilmManagement::getInstance()->idToPeople($_POST['acteur' . $nb])); // Va chercher la personne a lié en BDD
                    if (isset($_POST['role' . $nb]) && strcmp($_POST['role' . $nb], '') != 0) { // si role rempli
                        array_push($data_Role, new Data_Role($_POST['role' . $nb], $data_Acteur)); // création du Data_Role
                    } else {
                        $viewparams["erreur"] = '<br />L\'acteur : ' . $data_Acteur->getNom() . ' ' . $data_Acteur->getPrenom() . 'n\'a pas pu être lié étant donné qu\'aucun role a été entré !';
                    }
                }
                ++$nb;
            }

            //ajout des acteurs
            $param = $this->registerActeur($data_Film, $data_Role);
            $viewparams["update"] .= $param["ajouter"];
            $viewparams["erreur"] .= $param["erreur"];

            // traitement des éventuelles affiches
            $nbAffiche = 1;
            $data_Affiche = array();
            while (isset($_POST['affiche' . $nbAffiche])) { // affiche a traité
                if (strcmp($_POST['affiche' . $nbAffiche], '') != 0) { // si affiche selectionné
                    array_push($data_Affiche, $_POST['affiche' . $nbAffiche]);
                }
                ++$nbAffiche;
            }

            // ajout des affiches
            $param = $this->registerAffiche($data_Film, $data_Affiche);
            $viewparams["update"] .= $param["ajouter"];
            $viewparams["erreur"] .= $param["erreur"];

            //rafraichissement du film
            $data_Film = Nf_FilmManagement::getInstance()->idToFilm($data_Film->getId());
            $viewparams['film'] = $data_Film;
        } // fin du traitement du formulaire
        //recupération des affiches du film
        $data_Affiches = Nf_FilmManagement::getInstance()->getAffiches($data_Film);
        $viewparams['affiches'] = $data_Affiches;

        // tableau de l'ensemble des acteurs clé => id de l'acteur value => toString
        $acteur = Nf_ActeurReaManagement::getInstance()->getActeurs();
        foreach ($acteur as $value) {
            $viewparams["acteur"][$value->getID()] = $value;
        }

        // tableau de l'ensemble des réalisateurs clé => id de l'acteur value => toString
        $realisateur = Nf_ActeurReaManagement::getInstance()->getRealisateurs();
        foreach ($realisateur as $value) {
            $viewparams["realisateur"][$value->getID()] = $value;
        }

        $view = new Update_Films_View($viewparams);
        $view->display();
    }

// edit()

    /**
     * Permet de supprimer une affiche d\'un film
     * @param array(int) $data_Affiche contenant les id des affiches en BDD
     * @return $viewparams contenant : $viewparams["delete"], $viewparams["erreur"]
     */
    private function deleteAffiche($data_Affiche) {
        foreach ($data_Affiche as $value) {
            $img = new Data_Img($value);
            if (Nf_FilmManagement::getInstance()->removeAffiche($img)) {
                $viewparams["delete"] .= '<br />Affiche supprimé du film';
            } else {
                $viewparams["erreur"] .= '<br />Affiche n\'a pas pu être supprimé du film';
            }
        }
        return $viewparams;
    }

// deleteAffiche()

    /**
     * Permet de supprimer des roles d\'un film
     * @param array(int) $data_Role contenant les id des roles en BDD
     * @return $viewparams contenant : $viewparams["delete"], $viewparams["erreur"]
     */
    private function deleteActeur($data_Role, Data_Film $data_Film) {
        foreach ($data_Role as $value) {
            foreach ($data_Film->getPersonnages() as $role) {
                if ($role->getId() == $value) {
                    if (Nf_FilmManagement::getInstance()->removePersonnageDuFilm($role, $data_Film)) {
                        $viewparams["delete"] .= '<br />Le role : ' . $role . 'a pas pu être supprimé du film';
                    } else {
                        $viewparams["erreur"] .= '<br />L\'acteur : ' . $role . 'n\'a pas pu être supprimé du film';
                    }
                }
            }
        }
        return $viewparams;
    }

// deleteActeur()

    /**
     * // public car requise par le parser
     * Permet de supprimer définitivement un film ainsi que ses données annexes : réalisateur, acteurs et affiches liés
     * @param int $params["id"] l'id du film
     * @return void et appelant la vue Delete_Films_View($viewparams)
     * $viewparams contenant : $viewparams['film'], $viewparams["delete"], $viewparams["erreur"]
     */
    public function delete($params) {
        if (isset($_POST['delete'])) { // dans ce cas formulaire a traité
            $viewparams['id'] = $_POST['delete'];
            $data_Film = Nf_FilmManagement::getInstance()->idToFilm($_POST['delete']);
            if (Nf_FilmManagement::getInstance()->removeFilm($data_Film)) {
                $viewparams["delete"] = "Le film a été supprimé";
            } else {
                $viewparams["erreur"] = "Une erreur c'est produite le film n'a pas pu être supprimé";
            }
        }

        $data_Film = Nf_FilmManagement::getInstance()->idToFilm($params["id"]);

        $viewparams['film'] = $data_Film;
        $view = new Delete_Films_View($viewparams);
        $view->display();
    }

// delete()

    /**
     * // public car requise par le parser
     * Permet d'afficher un film ainsi que ses données annexes : réalisateur, acteurs, affiches liés et notes
     * @param int $params["id"] l'id du film
     * @return void et appelant la vue Print_Films_View($viewparams)
     * $viewparams contenant : $viewparams['film'], $viewparams["affiches"]
     */
    public function printFilm($params) {

        $id = $params['id'];

        $data_Film = Nf_FilmManagement::getInstance()->idToFilm($id);
        $data_Film->affiches = Nf_FilmManagement::getInstance()->getAffiches($data_Film);

        //Extraction des notes de la BDD pour en faire une moyenne                
        $tabNote = Nf_CommNoteManagement::getInstance()->getNotesParFilm($data_Film);

        if (count($tabNote) >= 1) { // si possède des notes alors calcul de la moyenne
            $sum = 0;

            foreach ($tabNote as $key => $data_Note) {

                $sum += $data_Note->getNote();
            }
            $data_Film->note = $sum / count($tabNote);
        }

        if (isset($_SESSION['id'])) {

            $user = Nf_UserDvdManagement::getInstance()->idToUser($_SESSION['id']);
            if ($res = Nf_CommNoteManagement::getInstance()->getNotesParFilm($data_Film, $user)) {
                $data_Film->ANote = $res[0];
            }
        }

        $viewparams["films"] = array($data_Film);
        $view = new Print_Films_View($viewparams);
        $view->display();
    }

// printFilm

    /**
     * Permet d'afficher les films dont le titre est approximativement ce qui est passé en paramètre
     */
    public function rechercheTitreFilm($param) {
        if (!$param["recherche"] or ( strlen($param["recherche"]) == 0)) {
            return;
        }
        // récupére les films qui correspondent (presque à un des mots clés)
        $films = Nf_FilmManagement::getInstance()->getFilmsParTitre($param["recherche"]);

        foreach ($films as $key => $film) {
            $films[$key]->affiches = Nf_FilmManagement::getInstance()->getAffiches($film);
        }

        $viewparams["films"] = $films;
        $view = new Print_Films_View($viewparams);
        $view->display();
    }

}
?>