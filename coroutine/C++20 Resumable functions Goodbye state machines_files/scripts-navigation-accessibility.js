/**
 * Accessible navigation
 *
 * @link  http://a11yproject.com/
 * @link  https://codeable.io/community/wordpress-accessibility-creating-accessible-dropdown-menus/
 *
 * @package    Modern
 * @copyright  WebMan Design, Oliver Juhas
 *
 * @since    2.0.0
 * @version  2.4.0
 */





( function( $ ) {

	'use strict';





	/**
	 * Helper variables
	 */

		var
			$siteNavigation = $( document.getElementById( 'site-navigation' ) );



	/**
	 * Adding ARIA attributes
	 */

		$siteNavigation
			.find( '.menu-item-has-children' )
				.attr( 'aria-haspopup', 'true' );



	/**
	 * Setting `.focus` class for menu parent
	 */

		$siteNavigation
			.on( 'focus.aria mouseenter.aria', '.menu-item-has-children', function( e ) {

				// Processing

					$( e.currentTarget )
						.addClass( 'focus' );

			} );

		$siteNavigation
			.on( 'blur.aria mouseleave.aria', '.menu-item-has-children', function( e ) {

				// Processing

					$( e.currentTarget )
						.removeClass( 'focus' );

			} );



	/**
	 * Touch-enabled
	 */

		$siteNavigation
			.on( 'touchstart', '.menu-item-has-children > a', function( e ) {

				// Helper variables

					var
						el = $( this ).parent( 'li' );


				// Processing

					/**
					 * First touch does not trigger the link, only opens the submenu.
					 * Second touch does trigger the link.
					 */
					if ( ! el.hasClass( 'focus' ) ) {
						e.preventDefault();

						el
							.toggleClass( 'focus' )
							.siblings( '.focus' )
								.removeClass( 'focus' );
					}

			} );



	/**
	 * Menu navigation with arrow keys
	 */

		$siteNavigation
			.on( 'keydown', 'a', function( e ) {

				// Helper variables

					var
						$this = $( this );


				// Processing

					if ( e.which === 37 ) {

						// Left key

							e.preventDefault();

							$this
								.parent()
								.prev()
									.children( 'a' )
										.focus();

					} else if ( e.which === 39 ) {

						// Right key

							e.preventDefault();

							$this
								.parent()
								.next()
									.children( 'a' )
										.focus();

					} else if ( e.which === 40 ) {

						// Down key

							e.preventDefault();

							if ( $this.next().length ) {

								$this
									.next()
										.find( 'li:first-child a' )
										.first()
											.focus();

							} else {

								$this
									.parent()
									.next()
										.children( 'a' )
											.focus();

							}

					} else if ( e.which === 38 ) {

						// Up key

							e.preventDefault();

							if ( $this.parent().prev().length ) {

								$this
									.parent()
									.prev()
										.children( 'a' )
											.focus();

							} else {

								$this
									.parents( 'ul' )
									.first()
									.prev( 'a' )
										.focus();

							}

					}

			} );





} )( jQuery );
