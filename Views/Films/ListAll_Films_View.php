<?php

class ListAll_Films_View extends Main_Global_View
{
    var $films;
    public function ListAll_Films_View($viewparams)
    {
        $filmscss = array("films.css", "list.css");
        $this->setCSS($filmscss);
		$filmsjs = array("film.js");
		$this->setJS($filmsjs);
        $this->setFilms($viewparams["films"]);
        $this->maincontent = new Print_Films_View($viewparams);
        $this->menu = new Menu_Films_View($viewparams);
    }

    public function setFilms($films) {
        $this->films = $films;
    }

    public function mainContent()
    {
        ob_start();
    ?>
            <div id="nav_left">
				<?php $this->menu->mainContent(); ?>
            </div>
            <div id="liste">
                <?php $this->maincontent->mainContent(); ?>
            </div>
        <?php
        $content = ob_get_contents();
        ob_end_clean();

        return $content;
    }
}

?>