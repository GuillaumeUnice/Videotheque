<?php

class Print_Films_View extends Ajax_Global_View {

    public function Print_Films_View($viewparams) {
        $this->films = $viewparams["films"];
    }

    public function mainContent() {
        foreach ($this->films as $film) {
            ?>
            <article class="film" id="<?php echo $film->getId(); ?>">
                <header>
                    <?php echo utf8_encode($film->getTitre()); ?>
                </header>

                <aside>
                    <?php
                    if ($film->affiches) {
                        foreach ($film->affiches as $affiche) {
                            echo '<img src="' . $affiche->getSrc() . '" />';
                        }
                    } else {
                        echo '<img src="" title="Pas d\'affiche" alt="Pas d\'affiche" />';
                    }
                    ?>
                </aside>

                <section>
                    <p>Ann&eacute;e : <?php echo $film->getAnnee(); ?></p>

                    <p>R&eacute;alisateur :
                        <?php if ($film->getRealisateur()) { ?>
                            <a href="./index.php?controller=ActeurRea&action=show&id=<?php echo $film->getRealisateur()->getId(); ?>"><?php echo utf8_encode($film->getRealisateur()); ?></a>
                        <?php } else { ?>
                            R&eacute;alisateur inconnu.
                        <?php } ?>
                    </p>

                    <p>Style : <?php echo utf8_encode($film->getStyle()); ?></p>

                    <p>Langue : <?php echo utf8_encode($film->getLangue()); ?></p>

                    <h2>Description</h2>

                    <p><?php echo utf8_encode($film->getResume()); ?></p>

                    <h2>Acteurs</h2>

                    <p>
                        <?php
                        if (!count($film->getPersonnages())) {
                            ?>

                            Ce film n'a aucun personnage.<br/><br/>

                            <?php
                        } else {
                            ?>
                        <ul>

                            <?php
                            foreach ($film->getPersonnages() as $role) {
                                ?>

                                <li>
                                    <a href="./index.php?controller=ActeurRea&action=show&id=<?php echo $role->acteur->getId(); ?>"><?php echo utf8_encode($role->acteur); ?></a>
                                    : <?php echo utf8_encode($role->nom); ?>
                                </li>

                                <?php
                            }
                            ?>

                        </ul>

                        <?php
                    }
                    ?>
                    </p>

                    <p class="note" id="note<?php echo $film->getId(); ?>">
                        <?php
                        if (isset($film->note)) {
                            echo 'Note moyenne du film : ' . round($film->note, 2, PHP_ROUND_HALF_UP) . ' ';
                        } else {
                            echo 'Ce film ne possède encore aucune note! ';
                        }

                        if (isset($film->ANote)) {
                            echo '<br />Vous avez déjà noté ce film : ' . $film->ANote->getNote();
                            ?>
                            <a id="iframe" href="./index.php?controller=Films&action=deleteNote&id=<?php echo $film->ANote->getId(); ?>&idFilm=<?php echo $film->getId(); ?>">Supprimer</a>

                            <?php
                        } else {
                            ?>
                            <a id="iframe" href="./index.php?controller=Films&action=addNote&id=<?php echo $film->getId(); ?>">Noter</a>
                            <?php
                        }
                        ?>
                    </p>
                </section>
                <section class="note">
                    <h2>Commentaires</h2>
                    <?php
                    if (isset($film->com)) {
                        foreach ($film->com as $com) {
                            echo '<p>L\'utilisateur "' .
                            utf8_encode($com->getAuteur()->getNom()) . ' ' .
                            utf8_encode($com->getAuteur()->getPrenom()) . '" a écrit : "' . utf8_encode($com->getCommentaire()) .
                            '", le ' . utf8_encode($com->getDate()) . '</p>';
                        }
                    } else {
                        if ($res = Nf_CommNoteManagement::getInstance()->getCommentairesParFilm($film)) {
                            $film->com = $res;
                            foreach ($film->com as $com) {
                                echo '<p>L\'utilisateur "' .
                                utf8_encode($com->getAuteur()->getNom()) . ' ' .
                                utf8_encode($com->getAuteur()->getPrenom()) . '" a écrit : "' . utf8_encode($com->getCommentaire()) .
                                '", le ' . utf8_encode($com->getDate()) . '</p>';
                            }
                        } else {
                            echo '<p>Ce film n\'a pas encore de commentaire.</p>';
                        }
                    }
                    if (isset($_SESSION['id'])) {
                        ?>
                    <p><a id="iframe" href="./index.php?controller=Films&action=addCom&id=<?php echo $film->getId(); ?>">Ajouter un commentaire</a></p>
                        <?php
                    }
                    ?>
                </section>
                <footer>
                    <?php
                    if (isset($_SESSION['id'])) {
                        echo '<a id="iframe" href="./index.php?controller=Films&action=edit&id=' . $film->getId() . '">Editer</a> ';
                        echo ' <a  id="iframe" href="./index.php?controller=Films&action=delete&id=' . $film->getId() . '">Supprimer</a>';
                    }
                    ?>
                </footer>
            </article>
            <?php
        }
    }

}
?>
