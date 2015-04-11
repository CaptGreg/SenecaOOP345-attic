function NavBarMenu(varName, menuSelector) {

	// Private variables ---------------------------------------------------------------------------

	var self          = this;
	var openTimeout   = 100;
	var closeTimeout  = 500;
	var openTimer     = 0;
	var closeTimer    = 0;
	var openMenuItem  = 0;
	var hoverMenuItem = 0;
	var myName        = varName;			 // Name of "this"'s variable
	var selector      = menuSelector;

	// Private methods -----------------------------------------------------------------------------

	var CancelTimer = function () {
		if (openTimer) {
			window.clearTimeout(openTimer);
			openTimer = null;
		}
	}

	var OpenMenu = function () {
		CancelTimer();
		CloseMenu();

		// hoverMenuItem is the item the cursor is currently over. it's the candidate for having its
		// submenu opened.
		if (hoverMenuItem) 
			openMenuItem = $(hoverMenuItem).addClass('open');
	}

	var CloseMenu = function () {

		// openMenuItem is the item whose submenu is currently open. We only close this menu if the 
		// cursor isn't over this, or any of this item's children.
		if (openMenuItem) {

			// This or a child of this is under the cursor? If so, don't close.
			if (openMenuItem.attr('over') || openMenuItem.find('[over=true]').length > 0)
				return;

			openMenuItem.removeClass('open');

			// This item may be a submenu of a menu and so if this item is being closed, we also
			// need to check, and close, all parent menus of this item that aren't currently under
			// the cursor.
			openMenuItem.parents('[over!=true].open').removeClass('open');
		}

		// We've closed this item, and possible parent items. We should now traverse up the 
		// hierarchy of menu items to see if there's a parent item that is open under the cursor. If
		// so, remember this as the currently opened item so we can close it when another item is
		// hovered over.
		if (openMenuItem) {
			var openParents = openMenuItem.parents('[over=true]');
			openMenuItem = (openParents.length > 0)? $(openParents[0]) : null;
		}
	}

	// Delay the open so that a cursor transiting over the menu doesn't accidentally open it.
	var ScheduleOpen = function () {
		hoverMenuItem = this;	// Candidate for having it's menu opened
		openTimer     = window.setTimeout(OpenMenu, openMenuItem? 0 : openTimeout);
	}

	// Delay the close so that a cursor accidentally leaving the menu doesn't prematurely close it.
	var ScheduleClose = function () {
		hoverMenuItem = 0;
		closeTimer    = window.setTimeout(CloseMenu, closeTimeout);
	}

	// Public methods ------------------------------------------------------------------------------

	if ($.support.opacity) // Means everyone except IE8 and below.
		$(document).ready(function () { eval(myName + '.InitMenu();'); }).click(self.CloseMenu);

	this.InitMenu = function () {
	    if (!navigator.msMaxTouchPoints) {
		    $(selector + '.openable').removeClass('openable').hover(ScheduleOpen, ScheduleClose);
		    $(selector).hover(function (e) { $(this).attr('over', 'true'); },
					          function (e) { $(this).removeAttr('over'); });
        }
	}

	// Specific to the Article dropdown. Included here just to keep it out of harm's way.
	this.ShowMap = function (prnt, elmId) {
		var map = $('#' + elmId);

		if (!prnt.populated && !prnt.populating) {
			prnt.populating = true;
			map.css({ 'height': '200px', 'width': '100px' });
			map.load("/script/content/Ajax/SiteMap.aspx", function () { prnt.populated = true; });
			window.setTimeout(this.InitMenu, 200); // process new additions to menu so they are openable.
			prnt.populated = true;
			prnt.populating = false;
		}

		if (prnt.populated) {
			this.InitMenu(); //  Just in case it didn't work the first time.

			map.css('margin', '0');
			map.css('width', 'auto');
			map.css('height', 'auto');
		}
	}

}

var navBarMenu = new NavBarMenu('navBarMenu', '.navmenu li');
var signinMenu = new NavBarMenu('signinMenu', '.member-signin');