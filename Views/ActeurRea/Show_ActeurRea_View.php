<?php
/**
 * Permet d'afficher un réalisateur ou un acteur en détail
 */

class Show_ActeurRea_View extends Main_Global_View {
    var $personne;
    var $roles;
    var $productions;
    var $films_roles;

    public function Show_ActeurRea_View($viewparams)
    {
        $this->setCSS(array("show.css","films.css"));

        $this->personne = $viewparams["personne"];
        $this->roles = $viewparams["roles"];
        $this->films_roles = $viewparams["films_roles"];
        $this->productions = $viewparams["productions"];
    }
    /**
     * Permet d'obtenir le contenu principal de la page.
     *
     * @return string
     */
    protected function mainContent()
    {
        ob_start();

        $personne = $this->personne;

        $mort = $personne->getMort();
        if ($mort == -1) {
            $mort = "AUJOURD'HUI";
        }
        ?>
        <article class="show" id="<?php echo $personne->getId(); ?>">
            <header>
                <?php echo $personne->getNom() . ' ' . $personne->getPrenom() . '<br />' . utf8_encode($personne->getNaissance()) . ' - ' . $mort; ?>
            </header>
            <aside>
                <?php
                if ($personne->portrait) {
                    foreach($personne->portrait as $photo) {
                        echo '<img src="' . $photo->getSrc() . '"/>';
                    }
                } else {
                    echo '<div class="noimage">PAS DE PHOTO</div>';
                }
                ?>
            </aside>
            <section>
                <h2>Filmographie</h2>
                <h3>Rôles</h3>
                <p>
                    <?php
                    $roles = $this->roles;
                    $films_roles = $this->films_roles;
                    if(!$roles) { ?>
                        Cette personne n'a joué dans aucun film.<br/><br/>
                    <?php } else {
                        foreach($roles as $role) {
                            echo '<u>'.$role->getTitreFilm().'</u> en tant que <b>'.$role->getRole()->nom.'</b><br />';
                        } ?>
                    <?php } ?>
                </p>
                <h3>Productions</h3>
                <p>
                    <?php
                    $productions = $this->productions;
                    if(!$productions) { ?>
                        Cette personne n'a réalisé aucun film.<br/><br/>
                    <?php } else {
                        echo 'Voir la liste ci-après';
                    } ?>
                </p>
            </section>
            <section class="right">
                <p>Nationalité :
                    <?php echo $personne->getNationalite(); ?>
                </p>

                <p>Sexe :
                    <?php
                    if ($personne->isSexeFeminin()) {
                        echo 'Femme';
                    } else {
                        echo 'Homme';
                    }
                    ?>
                </p>
                </section>
                <footer>
                    <?php
                    if (isset($_SESSION['id'])) {
                        echo '<a id="iframe" href="./index.php?controller=ActeurRea&action=edit&id='. $personne->getId() .'">Editer</a> ';
                        echo '<a id="iframe" href="./index.php?controller=ActeurRea&action=delete&id=' . $personne->getId() . '">Supprimer</a>';
                    }
                    ?>
                </footer>
        </article>
        <articles id="liste">
            <?php if($productions) {
                $params['films'] = $productions;
                (new Print_Films_View($params))->display();
            } elseif($films_roles) {
                $params['films'] = $films_roles;
                (new Print_Films_View($params))->display();
            } ?>
        </articles>
<?php
        $content = ob_get_contents();
        ob_end_clean();

        return $content;
    }
}
?>