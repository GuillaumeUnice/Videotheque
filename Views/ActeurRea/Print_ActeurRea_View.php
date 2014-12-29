<?php
class Print_ActeurRea_View extends Ajax_Global_View
{

    var $people;
    public function Print_ActeurRea_View($viewparams)
    {
        $this->people = $viewparams["acteurRea"];
    }

    public function mainContent()
    {
        foreach ($this->people as $personne) {
            $mort = $personne->getMort();
            if ($mort == -1) {
                $mort = "AUJOURD'HUI";
            }
            ?>
            <article class="acteurRea" id="<?php echo $personne->getId(); ?>">
                <header>
                    <?php echo $personne->getNom() . ' ' . $personne->getPrenom() . '<br />' . utf8_encode($personne->getNaissance()) . ' - ' . $mort; ?>
                </header>
                <aside>
                        <?php
                        if ($personne->portrait) {
                            echo '<img src="' . $personne->portrait[0]->getSrc() . '"/>';
                        } else {
                            echo '<div class="noimage">PAS DE PHOTO</div>';
                        }
                        ?>
                    </aside>
                <section>
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
                <section>
                    <h2>Filmographie</h2>
                    <p><a href="./index.php?controller=acteurRea&action=show&id=<?php echo $personne->getId(); ?>">+ PLUS DE DETAILS</a></p>
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
        <?php
        }
    }
}
?>
