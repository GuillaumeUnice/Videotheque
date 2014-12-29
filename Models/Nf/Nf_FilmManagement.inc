<?php

//include constant("PATHTOROOT")."includes/connexion.inc";

/**
 * 
 * @author renevier-gonin
 * @package Nf_
 * 
 * @abstract la classe Nf_FilmManagement permet de lire, d'ajouter, de modifier ou de supprimer
 * des films du monde du cinéma. Elle hérite de Nf_FilmManagement pour pouvoir "gérer" les rôles
 * et les réalisateurs. Nf_FilmManagement peut donc se substituer à Nf_FilmManagement :
 * <code>
 * $filmMgt = Nf_FilmManagement::getInstance();
 * $films = $filmMgt->getFilms(); // pour obtenir la liste de tous les films
 * </code>
 * 
 */
class Nf_FilmManagement extends Nf_ActeurReaManagement {

    /**
     * @ignore
     * @var Nf_FilmManagement
     */
    protected static $instance;

    /**
     * @ignore
     */
    protected function __construct() {
        $this->connexion = constant('CONNEXION');
    }

    /**
     * pour obtenir l'unique instance de Nf_FilmManagement
     * @return Nf_FilmManagement
     */
    public static function getInstance() {
        if (!Nf_FilmManagement::$instance)
            Nf_FilmManagement::$instance = new Nf_FilmManagement();

        return Nf_FilmManagement::$instance;
    }

    /**
     * @ignore
     */
    protected function fromCompleteRequestToFilm($localResults) {
        $f = new Data_Film();
        $f->setTitre($localResults["Titre"]);
        $f->setResume($localResults["Resume"]);
        $f->setAnnee($localResults["Sortie"]);
        $f->setLangue($localResults["Langue"]);
        $f->setStyle($localResults["Style"]);
        $f->setId($localResults["id"]);

        return $f;
    }

    /**
     * @ignore
     */
    protected function getRealisateurFromFilm($id) {
        $r = null;

        $query = "SELECT * FROM realise where film=\"" . $id . "\"";
        // Statement readStmt = con.createStatement();
        $result = mysql_query($query, $this->connexion);
        if (mysql_num_rows($result)) {
            $rep = mysql_fetch_array($result);
            $rea = $rep["realisateur"];

            $r = $this->idToRealisateur($rea); // par heritage
        }

        return $r;
    }

    /**
     * @ignore
     */
    protected function getPersonnagesFromFilm($id) {
        $perso = array();

        $query = "SELECT * FROM jouedans JOIN acteursrealisateurs ON acteursrealisateurs.id = jouedans.acteur where jouedans.film=\"$id\"";
        // Statement readStmt = con.createStatement();
        $localResults = mysql_query($query, $this->connexion);

        if ($localResults) {
            while ($rep = mysql_fetch_array($localResults)) {
                $a = $this->fromCompleteRequestToActeur($rep);
                $r = new Data_Role($rep["role"], $a);
                $r->setId($rep["rid"]);
                $perso[] = $r;
            }
        }



        return $perso;
    }

    /**
     * pour retrouver un rôle à partir d'un id
     * @param int $id : l'id du rôle
     * @return Data_Role le rôle s'il existe ou null
     */
    public function idToRole($id) {
        $result = null;
        $query = "SELECT * FROM jouedans where rid=\"$id\"";
        $localResults = mysql_query($query, $this->connexion);

        //		 on prend le resultat
        if (mysql_num_rows($localResults)) {
            $rep = mysql_fetch_array($localResults);
            $a = $this->idToActeur($rep["acteur"]);
            $result = new Data_Role($rep["role"], $a);
            $result->setId($rep["rid"]);
        }

        return $result;
    }

    /**
     * // public car requise dans UserDvdManagement
     * // public car requise dans CommNoteManagment
     * permet de transformer un id en film, si l'id existe bien...
     * @param int $id
     * @return Data_Film ... ou null si l'id n'existe pas
     */
    public function idToFilm($id) {
        $query = "SELECT * FROM film where id=\"$id\"";
        $localResults = mysql_query($query, $this->connexion);

        $f = null;
        if (mysql_num_rows($localResults)) {
            //		 on prend le resultat
            $rep = mysql_fetch_array($localResults);

            $f = $this->fromCompleteRequestToFilm($rep);

            //		recuperation du realisateur
            $rea = $this->getRealisateurFromFilm($id);
            if ($rea)
                $f->setRealisateur($rea);

            //	recuperation des acteurs
            $perso = $this->getPersonnagesFromFilm($id);
            foreach ($perso as $r)
                $f->addPersonnage($r);
        }

        return $f;
    }

    // la condition est que le resultat de la requete contienne tous les champs de film
    /**
     * @ignore
     */
    protected function getFilmsFromRequest($query) {
        $result = array();

        $localResults = mysql_query($query, $this->connexion);

        if ($localResults)
            while ($rep = mysql_fetch_array($localResults)) {
                $f = $this->fromCompleteRequestToFilm($rep);

                $id = $rep["id"];

                $rea = $this->getRealisateurFromFilm($id);
                if ($rea)
                    $f->setRealisateur($rea);

                $perso = $this->getPersonnagesFromFilm($id);
                foreach ($perso as $r)
                    $f->addPersonnage($r);

                $result[] = $f;
            }

        return $result;
    }

    /**
     * // public car requise dans UserDvdManagement
     * // public car requise dans CommNoteManagment
     * permet d'obtenir un id à partir d'un Film. Notamment quand c'est un film 
     * créer par l'utilisateur... (pour vérifier s'il n'existe pas encore ou plus)
     * @param Data_Film $film
     * @return int
     */
    public function filmToId(Data_Film $film) {
        $query = "SELECT * FROM film where Titre=\"" . $film->getTitre() . "\"";
        $query .= " AND Sortie = \"" . $film->getAnnee() . "\"";
        $query .= " AND Style = \"" . $film->getStyle() . "\"";

        $filmId = -1;


        $localResults = mysql_query($query, $this->connexion);

        //	on prend le resultat
        if ($localResults) {
            if ($rep = mysql_fetch_array($localResults))
                $filmId = $rep["id"];
        }


        return $filmId;
    }

    /**
     * // public car requise dans UserDvdManagement
     * permet de savoir si toutes les informations liées au film sont entrées
     * @param Data_Film $f
     * @return boolean
     */
    public function estFilmIncomplet(Data_Film $f) {
        return ($f == null) || ($f->getLangue() == null) || ($f->getLangue() == "") || ($f->getTitre() == null) || ($f->getTitre() == "") || ($f->getResume() == null) || ($f->getResume() == "") || ($f->getStyle() == null) || ($f->getStyle() == "") || ($f->getAnnee() <= 0);
    }

    /*     * *************************************************************************
     * **************** Partie pour lire les données ************
     * *************************************************************************** */

    /**
     * pour obtenir la liste de tous les films (en remottant le temps, les plus récents d'abords)
     * @return array retourne tous les films 
     */
    public function getFilms() {
        $query = "SELECT * FROM film ORDER BY Sortie DESC";
        return $this->getFilmsFromRequest($query);
    }

    /**
     * pour obtenir la liste des nb plus récents films
     * @param int $nb précise le nombre maximum de réponses qu'on veut
     * @return array retourne tous les films les plus recents
     */
    public function getFilmsRecents($nb) {
        $query = "SELECT * FROM film ORDER BY Sortie DESC limit " . $nb;
        return $this->getFilmsFromRequest($query);
    }

    /**
     * pour obtenir une liste de nb film en commençant à start+1 (car l’OFFSET commence toujours à 0)
     * @param type $start
     * @param type $nb
     * @return type
     */
    public function getFilmsAtoB($start, $nb) {
        $query = "SELECT * FROM film LIMIT " . $nb . " OFFSET " . $start;
        return $this->getFilmsFromRequest($query);
    }

    /**
     * pour obtenir la liste des films dont le titre comprend la chaîne passée en paramètre
     * @param string $titre précise le titre (ou un morceau) d'un ou des films recherchés.
     * @param boolean $exact pour savoir si la recherche est exacte ($exact = true) ou approximative
     * @return array retourne tous les films dont le titre est spécifié en paramètre.
     */
    public function getFilmsParTitre($titre, $exact = false) {
        // LIKE \"%"+nom+"%\"
        if ($exact)
            $query = "SELECT * FROM film where Titre=\"$titre\"";
        else
            $query = "SELECT * FROM film where Titre LIKE \"%$titre%\"";
        return $this->getFilmsFromRequest($query);
    }

    /**
     * pour obtenir la liste des films sortis une ou plusieurs années particulières
     * il faut au moins une année
     * @param int $annee (plusieurs) les annees dont on veut connaître tous les films.
     * @return array retourne tous les films de l'année spécifiée en paramètre.
     */
    public function getFilmsParAnnee($annee) {
        $nb_params = func_num_args();
        if ($nb_params > 0) {
            $query = "SELECT * FROM film where ";
            $annees = func_get_args();
            $query .= " Sortie = '{$annees[0]} ' ";
            for ($i = 1; $i < $nb_params; $i++)
                $query .= " OR  Sortie = '{$annees[$i]}' ";
            return $this->getFilmsFromRequest($query);
        } else
            return array();
    }

    /**
     * pour obtenir la liste des films tournés dans la langue spécifiée
     * @param string $langue précise la langue originale du film, une valeur du type ENUM LANGUE.
     * @return array  retourne tous les films dans la langue en paramètre.
     */
    public function getFilmsParLangue($langue) {
        if (Data_LANGUE::isInEnum($langue)) {
            $query = "SELECT * FROM film where Langue=\"$langue\"";
            return $this->getFilmsFromRequest($query);
        } else
            return array();
    }

    /**
     * pour obtenir la liste des films d'un réalisateur, en précisant éventuellement des années
     * les années sont optionnelles et passées en paramètres (plusieurs possibles)
     * @param Data_Realisateur $realisateur précise un réalisateur dont on recherche tous les films.
     * @param int $annee sont des années, c'est optionel. il peut y en avoir plusieurs
     * @return array retourne tous les films réalisé par le réalisateur spécifié en paramètre.
     */
    public function getFilmsParRea(Data_Realisateur $realisateur, $annee = 0) {
        // DISTINCT film.id, film.Titre, film.Sortie, film.Style, film.Resume, film.Langue
        // attention : on espère ici que l'ordre sera respecter...
        // , acteursrealisateurs.*
        $query = "SELECT film.* FROM acteursrealisateurs JOIN film JOIN realise ON acteursrealisateurs.id = realise.realisateur AND film.id = realise.film";
        $query .= " WHERE  acteursrealisateurs.Nom=\"" . $realisateur->getNom() . "\"";
        $query .= " AND acteursrealisateurs.Prenom = \"" . $realisateur->getPrenom() . "\"";
        $query .= " AND acteursrealisateurs.Naissance = \"" . $realisateur->getNaissance() . "\"";
        $nb_params = func_num_args();
        if (($nb_params > 1) && ($annee > 0)) {
            $annees = func_get_args();
            $query .= " AND (film.Sortie = \"" . $annees[1] . "\"";
            for ($a = 2; $a < $nb_params; $a++)
                $query .= " OR film.Sortie = \"" . $annees[$a] . "\"";
            $query .= ")";
        }

        return $this->getFilmsFromRequest($query);
    }

    /**
     * pour obtenir la liste des films d'un acteur, en précisant éventuellement des années
     * les années sont optionnelles et passées en paramètres (plusieurs possibles)
     * @param Data_Acteur $acteur précise un acteur dont on recherche tous les films.
     * @param int $annee sont des années, c'est optionnel. 
     * @return array retourne tous les films dans lesquels a joué l'acteur spécifié en paramètre.
     */
    public function getFilmsParActeur(Data_Acteur $acteur, $annee = 0) {
        // DISTINCT film.id, film.Titre, film.Sortie, film.Style, film.Resume, film.Langue
        // attention : on espère ici que l'ordre sera respecter...
        $query = "SELECT DISTINCT film.* FROM acteursrealisateurs JOIN film JOIN jouedans ON acteursrealisateurs.id = jouedans.acteur AND film.id = jouedans.film";
        $query .= " WHERE  acteursrealisateurs.Nom=\"" . $acteur->getNom() . "\"";
        $query .= " AND acteursrealisateurs.Prenom = \"" . $acteur->getPrenom() . "\"";
        $query .= " AND acteursrealisateurs.Naissance = \"" . $acteur->getNaissance() . "\"";
        $nb_params = func_num_args();
        if (($nb_params > 1) && ($annee > 0)) {
            $annees = func_get_args();
            $query .= " AND (film.Sortie = \"" . $annees[1] . "\"";
            for ($a = 2; $a < $nb_params; $a++)
                $query .= " OR film.Sortie = \"" . $annees[$a] . "\"";
            $query .= ")";
        }

        return $this->getFilmsFromRequest($query);
    }

    /**
     * pour obtenir la liste des films d'un ou de plusieurs acteurs
     * les acteurs sont passés en paramètres par exemple : getFilmsAvecLUnDesActeurs($a1, $a2, $a3);
     * il faut au moins un acteur !
     * @param Data_Acteur $acteurs est un des acteurs dont on recherche tous les films.
     * @return array retourne tous les films dans lesquels a joué au moins l'un des acteurs spécifiés en paramètre.
     */
    public function getFilmsAvecLUnDesActeurs(Data_Acteur $acteurs = null) {
        $nb_params = func_num_args();

        if (($nb_params >= 1) && ($acteurs)) {
            $query = "SELECT DISTINCT film.* FROM acteursrealisateurs JOIN film JOIN jouedans ON acteursrealisateurs.id = jouedans.acteur AND film.id = jouedans.film WHERE";

            $query .= " (acteursrealisateurs.Nom=\"" . $acteurs->getNom() . "\"";
            $query .= " AND acteursrealisateurs.Prenom = \"" . $acteurs->getPrenom() . "\"";
            $query .= " AND acteursrealisateurs.Naissance = \"" . $acteurs->getNaissance() . "\") ";

            $act = func_get_args();

            for ($a = 1; $a < $nb_params; $a++) {

                $query .= " OR ";

                $query .= " (acteursrealisateurs.Nom=\"" . $act[$a]->getNom() . "\"";
                $query .= " AND acteursrealisateurs.Prenom = \"" . $act[$a]->getPrenom() . "\"";
                $query .= " AND acteursrealisateurs.Naissance = \"" . $act[$a]->getNaissance() . "\") ";
            }

            return $this->getFilmsFromRequest($query);
        } else
            return array();
    }

    /**
     * pour obtenir la liste des films avec tous les acteurs précisés en paramètre
     * les acteurs sont passés en paramètres par exemple : getFilmsAvecTousLesActeurs($a1, $a2, $a3);
     * il faut au moins un acteur !
     * @param Data_Acteur acteurs est un des acteurs dont on veut trouver les films communs
     * @return array retourne tous les films dans lesquels a joué tous les acteurs spécifiés en paramètre.
     */
    public function getFilmsAvecTousLesActeurs(Data_Acteur $acteurs = null) {
        $nb_params = func_num_args();
        $act = func_get_args();

        if (($nb_params >= 1) && ($acteurs)) {
            $ou = array();
            foreach ($act as $actor)
                $ou = array_merge($ou, $this->getFilmsParActeur($actor));
            $result = array();

            foreach ($ou as $f) {
                $tous = true;
                foreach ($act as $a) {
                    $tous = $tous && $f->joueDedans($a);
                    if (!$tous)
                        break;
                }
                if ($tous) {
                    if (!in_array($f, $result))
                        $result[] = $f;
                }
            }

            return $result;
        } else
            return array();
    }

    /**
     * pour obtenir la liste des films d'un genre, en précisant une éventuelle année ou plusieurs
     * les années sont optionnelles et passées en paramètres (plusieurs possibles)
     * @param string $style précise le style du film, une valeur du type ENUM STYLE.
     * @param int $annee sont des années, c'est optionel. il peut y en avoir plusieurs
     * @return array retourne tous les films du style spécifié en paramètre.
     */
    public function getFilmsParStyle($style, $annee = 0) {
        if (Data_STYLE::isInEnum($style)) {
            $query = "SELECT * FROM film where Style=\"$style\"";
            $nb_params = func_num_args();
            if (($nb_params > 1) && ($annee > 0)) {
                $annees = func_get_args();
                $query .= " AND (Sortie = \"" . $annees[1] . "\"";
                for ($a = 2; $a < $nb_params; $a++)
                    $query .= " OR film.Sortie = \"" . $annees[$a] . "\"";
                $query .= ")";
            }
            return $this->getFilmsFromRequest($query);
        } else
            return array();
    }

    /**
     * retourne une liste d'image (sous forme de tableau de Data_Img a utiliser avec getSrc() )à partir d'un film
     * s'il n'y a pas d'image ou un probleme avec la base de donnees, retourne une liste vide (ou partielle)
     * @param Data_Film $f le film
     * @return array toutes les images (Data_Img) liees a ce film
     */
    public function getAffiches(Data_Film $f) {
        $result = array();


        $fid = $this->filmToId($f);

        $query = "SELECT afficheId from affiche WHERE film = \"$fid\"";

        $localResults = mysql_query($query, $this->connexion);

        //	on prend le resultat
        if ($localResults)
            while ($rep = mysql_fetch_array($localResults)) {
                $id = $rep["afficheId"];
                $result[] = new Data_Img($id, true);
            }


        return $result;
    }

    /*     * *************************************************************************
     * **************** Partie pour ajouter les données ************
     * *************************************************************************** */

    /**
     * pour entrer un nouveau Film sans realisateur sans acteurs. 
     * a faire après : ajout des acteurs / rôle / personnage
     * a faire après : ajout du realisateur
     * @param Data_Film $f le film à ajouter
     * @return boolean vrai si l'action a réussie, faux sinon (déjà présent dans la base)
     */
    public function addFilm(Data_Film $f) {
        if ($this->estFilmIncomplet($f)) {
            return false;
        }

        // test si le film existe : 
        if ($this->filmToId($f) > 0) {
            return false;
        }

        $txt = str_replace("\"", "'", $f->getResume());

        // ajout du film
        $query = 'INSERT INTO film (id, Titre, Style, Resume, Sortie, Langue) VALUES ("", "' . $f->getTitre() . '", "' . $f->getStyle() .
                '", "' . $f->getResume() . '", ' . $f->getAnnee() . ' , "' . $f->getLangue() . '")';

        return (mysql_query($query, $this->connexion));
    }

    /**
     * ajoute la liste de role (acteur+nom personnage) au film
     * si le film n'est pas dans la base, il est ajouté
     * si les acteurs ne le sont pas, ils sont ajoutés
     * vérification si le role existe deja
     * conseillé : ajouter les acteurs avant
     * s'utilise comme : addPersonnagesAuFilm($f, $r1, $r2, $r3)
     * @param Data_Film $f est le film dans lequel évolue les personnages
     * @param Data_Role $role1 est la liste des personnages (couple acteur / nom de personnage), il peut y en avoir plusieurs 
     * @return boolean vrai si l'ajout c'est bien passe (meme si les roles existaient deja et qu'ils ne sont pas rajoutes ou si l'acteur n'existe pas) faux sinon
     */
    public function addPersonnagesAuFilm(Data_Film $f, Data_Role $role1 = null) {
        $nb_params = func_num_args();
        if (($nb_params <= 2) && (!$role1)) {
            return false;
        }

        $fid = $this->filmToId($f);
        //		 test si le film existe : 
        if ($fid <= 0) {
            if (!$this->addFilm($f))
                return false;
            $fid = $this->filmToId($f);
        }

        $result = true;

        $roles = func_get_args();
        for ($i = 1; $i < $nb_params; $i++) {
            $r = $roles[$i];
            if (($r == null) || ($r->nom == null) || ($r->nom == "")) {
                continue;
            }
            $a = $r->acteur;

            $aid = $this->peopleToId($a);
            //		 test si l'acteur existe : 
            if ($aid <= 0) {
                if (!$this->addActeur($a)) {
                    continue;
                }
                $aid = $this->peopleToId($a);
            }


            $exist = false;
            $txt = str_replace("\"", "'", $r->nom);
            // test si le role existe deja 
            $query = "SELECT * FROM jouedans WHERE jouedans.film = \"$fid\" AND jouedans.acteur = \"$aid\"";

            $localResults = mysql_query($query, $this->connexion);

            if ($localResults)
                while ($rep = mysql_fetch_array($localResults)) {
                    $nom = $rep["role"];
                    $acteur = $rep["acteur"];

                    $exist = ($acteur == $aid) && ($nom == $txt);
                    // le role existe deja
                    if ($exist)
                        break;
                }


            if ($exist) {
                continue;
            }


            //	 ajout du role
            $insert = "INSERT INTO jouedans VALUES (rid, \"$fid\", \"$aid\", \"$txt\")";

            $result = $result && mysql_query($insert, $this->connexion);
            if (!$result)
                break;
        }
        return $result;
    }

    /**
     * ajoute une image (affiche) du film. 
     * Le type mime (image/png, image/gif, image/jpeg) est déterminé automatiquement
     * en fonction du nom de l'image (.png, .gif, .jpg ou .jpeg)
     * @param Data_Film $f le film
     * @param string image : le nom de fichier ou l'url
     * @return boolean vrai si cela a fonctionné
     */
    public function addAfficheAuFilm(Data_Film $f, $image) {
        $retour = false;

        $fid = $this->filmToId($f);

        $ext3 = substr($file, -4);
        $ext4 = substr($file, -5);
        $type = "image/png";
        if (($ext3 == ".jpg") || ($ext4 == ".jpeg"))
            $type = "image/jpg";
        else if ($ext3 == ".gif")
            $type = "image/jpg";

        $content = addslashes(file_get_contents($image));

        if ($content) {
            $query = "insert into affiche (afficheId, film, image, type) values ('','$fid','$content', '$type')";

            $retour = mysql_query($query, $this->connexion);
        }



        return $retour;
    }

    /**
     * Pour assigner un réalisateur à un film
     * LIMITATION : une seul realisateur par film
     * si le film n'est pas dans la base, il est ajouté
     * si le réalisateur n'est pas dans la base, il est ajouté
     * @param Data_Film $f le film réalisé par le réalisateur
     * @param Data_Realisateur $r le réalisateur du film f
     * @return boolean vrai si l'action a réussie, faux sinon (déjà présent dans la base)
     */
    public function addRealisateurAuFilm(Data_Film $f, Data_Realisateur $r) {
        $fid = $this->filmToId($f);
        //		 test si le film existe : 
        if ($fid <= 0) {
            if (!$this->addFilm($f))
                return false;
            $fid = $this->filmToId($f);
        }

        if ($this->estPeopleIncomplet($r)) {
            return false;
        }

        $rid = $this->peopleToId($r);
        //		 test si l'acteur existe : 
        if ($rid <= 0) {
            $this->addRealisateur($r);
            $rid = $this->peopleToId($r);
        }

        $exist = false;
        // verification si la relation existe deja
        $query = "SELECT * FROM realise WHERE realise.film = \"$fid\" AND realise.realisateur = \"$rid\"";
        $localResults = mysql_query($query, $this->connexion);
        $exist = (mysql_num_rows($localResults) >= 1 );

        if ($exist) {
            return false;
        }

        $insert = "INSERT INTO realise VALUES (id, \"$fid\", \"$rid\")";

        if (mysql_query($insert, $this->connexion))
            return true; // pour ne pas renvoyer une resource
        else
            return false;
    }

    /**
     * pour ajouter un film à la base, à partir d'un objet de type Data_Film "complet" (réalisateur, roles compris)
     * par appel des méthodes addFilm, addRealisateurAuFilm et addPersonnagesAuFilm
     * @param Data_Film $f le film à ajouter, avec son réalisateur et ses rôles
     * @return vrai si l'action a réussie, faux sinon (déjà présent dans la base)
     */
    public function addFilmComplet(Data_Film $f) {
        $result = true;
        $result = $result && $this->addFilm($f);
        if ($result)
            $result = $result && $this->addRealisateurAuFilm($f, $f->getRealisateur());
        if ($result) {
            $roles = $f->getPersonnages();
            foreach ($roles as $r)
                $result = $result && $this->addPersonnagesAuFilm($f, $r);
        }

        return $result;
    }

    /*     * *************************************************************************
     * **************** Partie pour supprimer les données ************
     * ******************************************************************************** */

    /**
     * pour retirer un personnage d'un film
     * @param Data_Role $r le role (couple acteur / nom de personnage ) à enlever
     * @param Data_Film $f le film
     * @return boolean vrai si la suppression a été acceptée (mais pas forcément faite si elle ne correspondait à rien)
     */
    public function removePersonnageDuFilm(Data_Role $r, Data_Film $f) {
        $aid = $this->peopleToId($r->acteur);
        if ($aid <= 0) {
            return false;
        }

        $fid = $this->filmToId($f);
        if ($fid < 0) {
            return false;
        }

        $delete = "DELETE FROM jouedans WHERE film=\"$fid\" AND acteur=\"$aid\" AND role=\"{$r->nom}\"";
        if (mysql_query($delete, $this->connexion))
            return true; // pour ne pas renvoyer une resource
        else
            return false;
    }

    /**
     * pour retirer un acteur d'un film (en cas de plusieurs apparition, tout est enlevé)
     * @param Data_Acteur $a à enlever
     * @param Data_Film $f le film
     * @return boolean vrai si la suppression a été acceptée (mais pas forcément faite si elle ne correspondait à rien)
     */
    public function removeActeurDuFilm(Data_Acteur $a, Data_Film $f) {
        $aid = $this->peopleToId($a);
        if ($aid <= 0) {
            return false;
        }

        $fid = $this->filmToId($f);
        if (fid < 0) {
            return false;
        }

        $delete = "DELETE FROM jouedans WHERE film=\"$fid\" AND acteur=\"$aid\"";
        if (mysql_query($delete, $this->connexion))
            return true; // pour ne pas renvoyer une resource
        else
            return false;
    }

    /**
     * pour retirer le réalisateur du film
     * @param Data_Realisateur $r le réalisateur à enlever
     * @param Data_Film $f le film
     * @return boolean vrai si la suppression a été acceptée (mais pas forcément faite si elle ne correspondait à rien)
     */
    public function removeRealisateurDuFilm(Data_Realisateur $r, Data_Film $f) {
        $rid = $this->peopleToId($r);
        if ($rid <= 0) {
            return false;
        }

        $fid = $this->filmToId($f);
        if ($fid < 0) {
            return false;
        }

        $delete = "DELETE FROM realise WHERE film=\"$fid\" AND realisateur=\"$rid\"";
        if (mysql_query($delete, $this->connexion))
            return true; // pour ne pas renvoyer une resource
        else
            return false;
    }

    /**
     * supprime une occurence de l'image dans la table "affiche"
     * (en general, la derniere occurence). <br /> 
     * idee initiale : requete sql : DELETE FROM `affiche` WHERE CONVERT(`image` USING utf8) = CAST("+blob+" AS BINARY) LIMIT 1;
     * ou blob est le tableau d'octet convertit en chaine de caractere (0xAB00055...) <br />
     * ceci ne marchant pas, la requete est du style : WHERE 'afficheID' = 'id';
     * @param Data_Img $img : l'image sous forme Data_Img (i.e. un id...)
     * @return boolean vrai si la suppression a été effectuée
     */
    public function removeAffiche(Data_Img $img) {
        if ($img->isAffiche()) {
            $delete = "DELETE FROM `affiche` WHERE `afficheId` = '{$img->getId()}' LIMIT 1;";
            if (mysql_query($delete, $this->connexion))
                return true; // pour ne pas renvoyer une resource
            else
                return false;
        } else
            return false;
    }

    /**
     * efface toutes les images d'un film
     * @param f le film
     * @return vrai si des donnees ont ete effacees
     */
    public function removeAffiches(Data_Film $f) {
        $fid = $this->filmToId($f);
        if ($fid) {
            $delete = "DELETE FROM `affiche` WHERE film = \"$fid\"";
            if (mysql_query($delete, $this->connexion))
                return true; // pour ne pas renvoyer une resource
            else
                return false;
        } else
            return false;
    }

    public function removeFilm(Data_Film $film) {

        $fid = $this->filmToId($film);
        if ($fid) {
            // on récupère le film de la bd
            $f = $this->idToFilm($fid);
            // on efface les roles
            foreach ($f->getPersonnages() as $r)
                $this->removePersonnageDuFilm($r, $f);

            // on efface le realisateur
            if ($f->getRealisateur())
                $this->removeRealisateurDuFilm($f->getRealisateur(), $f);
            // securite, s'il y a plusieurs realisateur
            $delete = "DELETE FROM realise WHERE film=\"$fid$\"";
            mysql_query($delete, $this->connexion);

            // on efface les commentaires
            Nf_CommNoteManagement::getInstance()->removeCommentairesParFilm($f);

            // on efface les affiches
            $this->removeAffiches($f);

            // on efface les notes
            Nf_CommNoteManagement::getInstance()->removeNotesParFilm($f);

            // on efface les emprunts...
            $dvds = Nf_UserDvdManagement::getInstance()->getDvdsParFilm($f);

            //... puis les "dvd"
            foreach ($dvds as $d) {
                Nf_UserDvdManagement::getInstance()->returnEmpruntsParDvd($d);
                for ($i = 0; $i < $d->getNbExemplaires(); $i++)
                    Nf_UserDvdManagement::getInstance()->removeDvd($d);
            }


            $delete = "DELETE FROM film WHERE id=\"$fid\"";
            if (mysql_query($delete, $this->connexion))
                return true; // pour ne pas renvoyer une resource
        } else
            return false;
    }

    /*     * *************************************************************************
     * **************** Partie pour modifier  les données ************
     * ******************************************************************************** */

    /**
     * pour mettre à jour un film (tout sauf la liste des personnages ET le réalisateur)
     * pour le reste : à faire séparément
     * @param Data_Film $oldf l'ancienne version du film
     * @param Data_Film $f la nouvelle valeur 
     * @return boolean vrai si l'update a été effectuée
     */
    public function updateFilm(Data_Film $oldf, Data_Film $f) {
        if ($this->estFilmIncomplet($f) || $this->estFilmIncomplet($oldf)) {
            return false;
        }
        $fid = $this->filmToId($oldf);

        if ($fid < 0) {
            return false;
        }

        $txt = str_replace("\"", "'", $f->getResume());

        $update = "UPDATE film SET Titre=\"{$f->getTitre()}\", Style=\"{$f->getStyle()}\", Resume=\"$txt\", Sortie=\"{$f->getAnnee()}\", Langue=\"{$f->getLangue()}\" WHERE id=\"$fid\"";
        if (mysql_query($update, $this->connexion))
            return true; // pour ne pas renvoyer une resource
        else
            return false;
    }

}

?>