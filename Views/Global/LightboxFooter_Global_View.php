<?php

	class LightboxFooter_Global_View {
		
		public function getFooter() {
			ob_start();
?>
				</section>
			</div>
		
	</body>
</html>
<?php			
			$footer = ob_get_contents();
			ob_end_clean();
			
			return $footer;
		}
		
	}
	
	
?>