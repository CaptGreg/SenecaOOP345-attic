jQuery(document).ready(function() {
	// easy toggle for categories
	jQuery('#triggerCatID').click(function() {
		jQuery(this).toggleClass('focus');
		jQuery('#headerStrip').animate({ height: 'toggle', opacity: '100'}, 100);
		return false;
	});
	jQuery('#triggerCatID2').click(function() {
		jQuery(this).toggleClass('focus');
		jQuery('#footerStrip').animate({ height: 'toggle', opacity: '100'}, 100);
		return false;
	});
});