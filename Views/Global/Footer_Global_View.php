<?php

	class Footer_Global_View {
		
		public function getFooter() {
			ob_start();
?>
				</section>
			</div>
			<footer>
			&copy Guillaume Borg  - Videotheque 2013-2014
			</footer>
	</body>
</html>
<?php			
			$footer = ob_get_contents();
			ob_end_clean();
			
			return $footer;
		}
		
	}
	
	
?>