/**
 * Mobile navigation
 *
 * @package    Modern
 * @copyright  WebMan Design, Oliver Juhas
 *
 * @since    1.4.5
 * @version  2.3.0
 */





( function( $ ) {

	'use strict';





	/**
	 * Helper variables
	 */

		var
			$siteNavigation   = $( document.getElementById( 'site-navigation' ) ),
			$siteMenuPrimary  = $( document.getElementById( 'menu-primary' ) ),
			$menuToggleButton = $( '#menu-toggle, #menu-toggle-bar' ),
			$breakpoints      = ( 'undefined' !== typeof $modernBreakpoints ) ? ( $modernBreakpoints ) : ( { 'l' : 880 } );



	/**
	 * Mobile menu actions
	 */
	function mobileMenuActions() {

		// Processing

			if ( ! $siteNavigation.hasClass( 'is-active' ) ) {

				$siteMenuPrimary
					.attr( 'aria-expanded', 'false' );

				$menuToggleButton
					.attr( 'aria-expanded', 'false' );

			}

			$siteNavigation
				.on( 'keydown', function( e ) {

					// Processing

						if ( e.which === 27 ) {

							// ESC key

								e.preventDefault();

								$siteNavigation
									.removeClass( 'is-active' );

								$siteMenuPrimary
									.attr( 'aria-expanded', 'false' );

								$menuToggleButton
									.attr( 'aria-expanded', 'false' );

								$menuToggleButton
									.focus();

						}

				} );

	} // /mobileMenuActions



	/**
	 * Default mobile menu setup
	 */

		if ( parseInt( $breakpoints['l'] ) >= window.innerWidth ) {

			$siteNavigation
				.removeClass( 'is-active' );

			mobileMenuActions();

		}



	/**
	 * Clicking the menu toggle button
	 */

		$menuToggleButton
			.on( 'click', function( e ) {

				// Processing

					e.preventDefault();

					$siteNavigation
						.toggleClass( 'is-active' );

					if ( $siteNavigation.hasClass( 'is-active' ) ) {

						$siteMenuPrimary
							.attr( 'aria-expanded', 'true' );

						$menuToggleButton
							.attr( 'aria-expanded', 'true' );

					} else {

						$siteMenuPrimary
							.attr( 'aria-expanded', 'false' );

						$menuToggleButton
							.attr( 'aria-expanded', 'false' );

					}

			} );



	/**
	 * Refocus to menu toggle button once the end of the menu is reached
	 */

		$siteNavigation
			.on( 'focus.aria', '.menu-toggle-skip-link', function( e ) {

				// Processing

					$menuToggleButton
						.focus();

			} );



	/**
	 * Disable mobile navigation on wider screens
	 */

		$( window )
			.on( 'resize orientationchange', function( e ) {

				// Processing

					if ( parseInt( $breakpoints['l'] ) < window.innerWidth ) {

						// On desktops

						$siteNavigation
							.removeClass( 'is-active' );

						$siteMenuPrimary
							.attr( 'aria-expanded', 'true' );

						$menuToggleButton
							.attr( 'aria-expanded', 'true' );

					} else {

						// On mobiles

						mobileMenuActions();

					}

			} );





} )( jQuery );
