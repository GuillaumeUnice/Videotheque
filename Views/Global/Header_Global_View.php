<?php

class Header_Global_View
{

    private $css;
    private $js;

    public function Header_Global_View($css, $js)
    {
        $this->css = $css;
        $this->js = $js;
    }

    public function getHeader()
    {
        ob_start();
        ?>
        <!doctype html>
        <html lang="fr">
        <head>
            <title>POO-IHM 2013-2014</title>
            <meta charset="utf-8">

            <?php
            // Traitement des feuilles de styles à intégrer
            foreach ($this->css as $cssFileName) {
                ?>
                <link rel="stylesheet" type="text/css" href="./public/css/<?php echo $cssFileName; ?>">
            <?php
            }
            ?>

            <?php
            // Traitement des scripts js à intégrer
            foreach ($this->js as $jsFileName) {
                ?>
                <script type="text/javascript" src="./public/js/<?php echo $jsFileName; ?>"></script>
            <?php
            }
            ?>
            <!-- inclusion du script de génération de lightbox -->
            <script type="text/javascript" src="./public/js/fancybox.js"></script>
        </head>
    <body>

        <header id="header_top">
            <nav id="menu">
                <a href="./index.php">Accueil</a>
                <a href="./index.php?controller=Films&action=listAll">Films</a>
                <a href="./index.php?controller=Films&action=listAutoload">Films(Autoload)</a>
                <a href="./index.php?controller=ActeurRea&action=listAll">Acteurs/Realisateurs</a>
                <?php
                if (isset($_SESSION['id'])) {
                    echo '<a id="iframe" href="./index.php?controller=Membres&action=InfoUser&id=' . $_SESSION['id'] . '">Profil</a>';
                }
                ?>
                <?php
                if (isset($_SESSION['id'])) {
                    ?>
                    <a href="./index.php?controller=Membres&action=disconnect">Déconnexion</a>
                <?php
                } else {
                    ?>
                    <a id="iframe" class="right"
                       href="./index.php?controller=Membres&action=connect">Se connecter</a>
                <?php } ?>
            </nav>
            <div id="rechercher"><input id="search" type="search" name="search" placeholder="Rechercher"></div>
        </header>

        <div id="wrapper">
            <section id="main">
<?php			
			$head = ob_get_contents();
			ob_end_clean();
			
			return $head;
		}

}
?>