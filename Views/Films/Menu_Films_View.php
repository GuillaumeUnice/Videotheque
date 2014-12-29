<?php

/**
 * Auteur: Jean-Christophe Isoard
 */
class Menu_Films_View extends Ajax_Global_View
{
    public function Menu_Films_View($viewparams)
    {
        $this->items = $viewparams["items"];
    }

    /**
     * Permet d'obtenir le contenu principal de la page.
     *
     * @return string
     */
    public function mainContent()
    {
        $genres = array();
        if ($this->items["genres"]) {
            $genres = $this->items["genres"];
        }
        ?>
        <?php if (isset($_SESSION['id'])) { ?>
        <h3>Ajouter</h3>
        <ul>
            <li><a class="bottom" id="iframe" href="./index.php?controller=Films&action=addFilm">Un Film</a></li>
            <li><a class="bottom" id="iframe" href="./index.php?controller=ActeurRea&action=addActeurRea">Un Réalisateur/Acteur</a></li>
        </ul>
        <?php } ?>
        <h2>Filtres</h2>
        <h3 class="title">Genre :</h3>
        <ul id="menu_genres">
            <?php foreach ($genres as $genre) { ?>
                <li value="<?php echo $genre["name"]; ?>"><?php echo ucfirst(strtolower($genre["name"])); ?></li>
            <?php } ?>
        </ul>
        <h3 class="title">Année :</h3>
        <ul id="menu_annee">
            <?php for($i=1969; $i<=date("Y"); $i+=10) {?>
                <li value="<?php echo $i-9; ?>"><?php echo ($i-9).'-'.$i; ?></li>
            <?php } ?>
                <li value="<?php echo $i-9; ?>"><?php echo ($i-9).'-FUTUR'; ?></li>
        </ul>
    <?php
    }
}

?>