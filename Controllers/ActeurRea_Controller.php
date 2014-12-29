<?php
/**
 * Created by IntelliJ IDEA.
 * User: Quentin
 * Date: 14/05/14
 * Time: 16:12
 */

class ActeurRea_Controller {

    /**
     * // public car requise par le parser
     * Permet d'afficher l'ensemble des films ainsi que leur informations annexes : réalisateur, acteurs, affiches, commentaires...
     * @param int $params["limit"] contenant une éventuelle limit pour la requete nombre a afficher
     * @return void et appelant la vue Delete_Films_View($viewparams)
     * $viewparams contenant : $viewparams["films"]
     */
    public function listAll($params) {
        $limit = 0;
        if(isset($params["limit"])) {
            $limit = (int) $params["limit"];
        }

        $personnes = Nf_ActeurReaManagement::getInstance()->getPersonnes();

        function personneCmp($personne1, $personne2) {
            return strcmp($personne1->getNom(), $personne2->getNom());
        }

        uasort($personnes, 'personneCmp');


        foreach($personnes as $key => $personne) {
            $personnes[$key]->portrait = Nf_ActeurReaManagement::getInstance()->getPortraits($personne);
        }

        $viewparams["acteurRea"] = $personnes;

        $view = new ListAll_ActeurRea_View($viewparams);
        $view->display();
    }

    /**
     * Permet d'afficher les films dont le titre est approximativement ce qui est passé en paramètre
     */
    public function recherche($param) {
        if(!$param["recherche"] or (strlen($param["recherche"]) == 0)) {
            return;
        }
        // récupére les acteur réalisateurs qui correspondent (presque à un des mots clés)
        $people = Nf_ActeurReaManagement::getInstance()->getPersonnes($param["recherche"]);

        foreach($people as $key => $personne) {
            $people[$key]->portrait = Nf_ActeurReaManagement::getInstance()->getPortraits($personne);
        }

        $viewparams["acteurRea"] = $people;
        $view = new Print_ActeurRea_View($viewparams);
        $view->display();
    }

    /**
     * Charge la vue pour afficher le détail d'un réalisateur/acteur
     * @param $param
     */
    public function show($param) {
        if(isset($param['id'])) {
            $id = $param['id'];
        }
        $personne = Nf_ActeurReaManagement::getInstance()->idToPeople($id);
        if(!$personne) {
            $personne = new Data_People("Aucun acteur sous cet identifiant","",date("Y"),"-",false);
        }
        $personne->portrait = Nf_ActeurReaManagement::getInstance()->getPortraits($personne);

        $roles = Nf_ActeurReaManagement::getInstance()->getRoles(new Data_Acteur($personne));

        $films_roles = Nf_FilmManagement::getInstance()->getFilmsParActeur(new Data_Acteur($personne));
        $productions = Nf_FilmManagement::getInstance()->getFilmsParRea(new Data_Realisateur($personne));

        $params['personne'] = $personne;
        $params['roles'] = $roles;
        $params['films_roles'] = $films_roles;
        $params['productions'] = $productions;

        foreach($productions as $key => $film) {
            $productions[$key]->affiches = Nf_FilmManagement::getInstance()->getAffiches($film);
        }
        foreach($films_roles as $key => $film) {
            $films_roles[$key]->affiches = Nf_FilmManagement::getInstance()->getAffiches($film);
        }

        $view = new Show_ActeurRea_View($params);
        $view->display();
    }

    /**
     * // public car requise par le parser
     * Permet d'ajouter un film ainsi qu'éventuellement ses données annexes : réalisateur, acteurs et affiches liés
     * @param void
     * @return void et appelant la vue Add_Films_View($viewparams)
     * $viewparams contenant : $viewparams["ajouter"], $viewparams["erreur"], $viewparams["acteur"][$value->getID()]
     * $viewparams["realisateur"][$value->getID()]
     */
    public function addActeurRea() {

        $data_people = new Data_People();
        if(isset($_POST['nom']) && isset($_POST['prenom'])) {
            if(strcmp($_POST['rolePeople'],'acteur')){
                $data_acteur = $this->createActeur($_POST['nom'],$_POST['prenom'],$_POST['annee'],$_POST['nationalite'],$_POST['sexe']);
                $data_people = $this->createActeur($_POST['nom'],$_POST['prenom'],$_POST['annee'],$_POST['nationalite'],$_POST['sexe']);
                $viewparams = $this->registerActeur($data_acteur);
            } else {
                $data_rea = $this->createRea($_POST['nom'],$_POST['prenom'],$_POST['annee'],$_POST['nationalite'],$_POST['sexe']);
                $data_people = $this->createRea($_POST['nom'],$_POST['prenom'],$_POST['annee'],$_POST['nationalite'],$_POST['sexe']);
                $viewparams = $this->registerRea($data_rea);
            }
        }

        $nbPortrait = 1;
        $data_portrait = array();
        while(isset($_POST['portrait' . $nbPortrait])) {
            if(strcmp($_POST['portrait' . $nbPortrait],'') != 0) {

                array_push($data_portrait, $_POST['portrait' . $nbPortrait]);
            }
            ++$nbPortrait;
        }

        $param = $this->registerPortrait($data_people, $data_portrait);
        $viewparams["ajouter"] .= $param["ajouter"];
        $viewparams["erreur"] .= $param["erreur"];

        $view = new Add_ActeurRea_View($viewparams);
        $view->display();

    }


    public function delete($params) {
        if(isset($_POST['delete']))
        {
            $viewparams['id'] = $_POST['delete'];
            $data_People = Nf_ActeurReaManagement::getInstance()->idToPeople($_POST['delete']);
            if(Nf_ActeurReaManagement::getInstance()->removePersonne($data_People)) {
                $viewparams["delete"] = "L'acteur / réalisateur a été supprimé";
            } else {
                $viewparams["erreur"] = "Une erreur c'est produite l'acteur / réalisteur n'a pas pu être supprimé";
            }
        }

        $data_People = Nf_ActeurReaManagement::getInstance()->idToPeople($params["id"]);

        $viewparams['acteurRea'] = $data_People;
        $view = new Delete_ActeurRea_View($viewparams);
        $view->display();
    }

    /**
     * @param $nom
     * @param $prenom
     * @param $naissance
     * @param $nationalite
     * @param $sexe
     * @return Data_Acteur
     */
    private function createActeur($nom, $prenom, $naissance, $nationalite, $sexe) {
        $data_Acteur = new Data_Acteur();
        $data_Acteur->setNom($nom);
        $data_Acteur->setPrenom($prenom);
        $data_Acteur->setNaissance($naissance);
        $data_Acteur->setNationalite($nationalite);
        $data_Acteur->setSexeFeminin($sexe);

        return $data_Acteur;
    }

    /**
     * @param $nom
     * @param $prenom
     * @param $naissance
     * @param $nationalite
     * @param $sexe
     * @return Data_Acteur
     */
    private function createRea($nom, $prenom, $naissance, $nationalite, $sexe) {
        $data_Acteur = new Data_Acteur();
        $data_Acteur->setNom($nom);
        $data_Acteur->setPrenom($prenom);
        $data_Acteur->setNaissance($naissance);
        $data_Acteur->setNationalite($nationalite);
        $data_Acteur->setSexeFeminin($sexe);

        return $data_Acteur;
    }

    /**
     * @param Data_Acteur $data_Acteur
     * @return mixed
     */
    private function registerActeur(Data_Acteur $data_Acteur) {
        if(Nf_ActeurReaManagement::getInstance()->addActeur($data_Acteur)) { // ajout en BDD du film
            $viewparams["ajouter"] = "L'acteur a pu être ajouté";
        } else {
            $viewparams["erreur"] = "Une erreur c'est produite vérifié que l'acteur n'est pas déjà présent ou que l'ensemble des champs ont été correctement rempli";
        }
        return $viewparams;
    } // registerFilm()

    /**
     * @param Data_Realisateur $data_Rea
     * @return mixed
     */
    private function registerRea(Data_Realisateur $data_Rea) {
        if(Nf_ActeurReaManagement::getInstance()->addActeur($data_Rea)) { // ajout en BDD du film
            $viewparams["ajouter"] = "Le réalisateur a pu être ajouté";
        } else {
            $viewparams["erreur"] = "Une erreur c'est produite vérifié que le réalisateur n'est pas déjà présent ou que l'ensemble des champs ont été correctement rempli";
        }
        return $viewparams;
    } // registerFilm()


    /**
     * @param Data_Film $data_Film
     * @param Data_Realisateur $data_Rea
     * @return mixed
     */
    private function registerReaFilm(Data_Film $data_Film,Data_Realisateur $data_Rea) {
        foreach($data_Film as $key => $value) {
            if(Nf_FilmManagement::getInstance()->addRealisateurAuFilm($value, $data_Rea)) {
                $viewparams["ajouter"] .= '<br />' . $value . ' a pu être lié au film';
            } else {
                $viewparams["erreur"] .= '<br />' . $value . 'n\'a pas pu etre lié une erreur c\'est produite';
            }
        }
        return $viewparams;

    }


    private function registerActeurFilm(Data_Film $data_Film, Data_Acteur $data_Acteur) {
        foreach($data_Film as $key => $value) {
            if(Nf_FilmManagement::getInstance()->addActeur($value, $data_Acteur)) {
                $viewparams["ajouter"] .= '<br />' . $value . ' a pu être lié au film';
            } else {
                $viewparams["erreur"] .= '<br />' . $value . 'n\'a pas pu etre lié une erreur c\'est produite';
            }
        }
        return $viewparams;
    }


    private function registerPortrait($data_people, $data_portrait) {
        foreach($data_portrait as $key => $value) {
            if (Nf_ActeurReaManagement::getInstance()->addPortraitAUnePersonne($data_people, $value)) {
                $viewparams["ajouter"] .= '<br />L\'affiche : ' . $value . ' a pu être ajouté';
            } else {
                $viewparams["erreur"] .= '<br />L\'affiche : ' . $value . 'n\'a pas pu être enregistré une lors du transfert du fichier a eu lieu vérifier que ce soit bien une image!';
            }
        }

        return $viewparams;
    }


    /**
     * @param $params
     */
    public function printFilm($params) {

        $id = $params['id'];

        $data_People = Nf_ActeurReaManagement::getInstance()->idToPeople($id);
        $data_People->portrait = Nf_ActeurReaManagement::getInstance()->getPortraits($data_People);


        $viewparams["acteurRea"] = array($data_People) ;
        $view = new Print_ActeurRea_View($viewparams);
        $view->display();
    } // printFilm

    public function edit($params) {

        $data_people = new Data_People();
        $data_Personne = Nf_ActeurReaManagement::getInstance()->idToPeople($params["id"]);
        $viewparams['personne'] = $data_Personne;
        if(isset($_POST['nom']) && isset($_POST['prenom']))
        {

            $new_Personne = $this->createActeur($_POST['nom'],$_POST['prenom'],$_POST['annee'],$_POST['nationalite'],$_POST['sexe']);
            //$data_Personne = $this->createActeur($_POST['nom'],$_POST['prenom'],$_POST['annee'],$_POST['nationalite'],$_POST['sexe']);

            $params = $this->updatePersonne($data_Personne, $new_Personne);
            if(isset($params["update"])) $viewparams["update"] = $params["update"];
            if(isset($params["erreur"])) $viewparams["erreur"] = $params["erreur"];


            //traitement des affiches existantes
            $nbUpdatePortrait = 1;
            $data_Portrait = array();
            while(isset($_POST['updatePortrait' . $nbUpdatePortrait])) {
                if($_POST['updatePortrait' . $nbUpdatePortrait] != 0) {

                    array_push($data_Portrait, $_POST['updatePortrait' . $nbUpdatePortrait]);

                }
                ++$nbUpdatePortrait;
            }

            $params = $this->deletePortrait($data_Portrait);
            if(isset($params["delete"])) $viewparams["update"] .= '<br />' . $params["delete"];
            if(isset($params["erreur"])) $viewparams["erreur"] .= '<br />' . $params["erreur"];


            // ajout des affiches
            $param = $this->registerPortrait($data_Personne, $data_Portrait);
            $viewparams["update"] .= $param["ajouter"];
            $viewparams["erreur"] .= $param["erreur"];

            //rafraichissement du film
            $data_Personne = Nf_ActeurReaManagement::getInstance()->idToPeople($data_Personne->getId());
            $viewparams['personne'] = $data_Personne;

        }

        //recupération des affiches du film
        $data_Portrait2 = Nf_ActeurReaManagement::getInstance()->getPortraits($data_people);
        $viewparams['portrait'] = $data_Portrait2;


        $view = new Update_ActeurRea_View($viewparams);
        $view->display();
    }


    /**
     * @param $old_Personne
     * @param $new_Personne
     * @return mixed
     */
    private function updatePersonne($old_Personne, $new_Personne) {

        if(Nf_ActeurReaManagement::getInstance()->updatePersonne($old_Personne, $new_Personne)) {
            $viewparams["update"] = "Update de l'acteur / réalisateur réussie";
        } else { $viewparams["erreur"] =  'Erreur lors de l\'update de l\'acteur / réalisateur' ;}

        return $viewparams;
    }


    private function deletePortrait($data_Portrait) {
        foreach ($data_Portrait as $value) {
            $img = new Data_Img($value);
            if(Nf_ActeurReaManagement::getInstance()->removePortraitAUnePersonne($img->getId())) {
                $viewparams["delete"] .= '<br />Portrait de la personne supprimé';
            } else {
                $viewparams["erreur"] .= '<br />Le portrait n\'a pas pu être supprimé';
            }
        }
        return $viewparams;
    } // deleteAffiche()

}