// Call service url to update article status.
function UpdateArticleEditStatus(articleId, serviceUrl, linkId, statusId) {

	var link = $('#' + linkId);
	var oldHtml = link.html();
	link.html('<img src="/Images/animated_loading_blue.gif" align="absmiddle" />');

	$.ajax(
	{
		type:		'POST',
		cache:		false,
		url:		serviceUrl,
		data:		'{articleId: ' + articleId + '}',
		contentType: 'application/json; charset=utf-8',
		dataType:	'json',
		tryCount:	0,
		retryLimit: 3,
		timeout:	5000,

		success: function (data) {
			link.remove();
			$('#' + statusId).html(data.d); 
		},
		error: function (jqXHR, textStatus, errorThrown) {
			link.html(oldHtml);
			$('#' + statusId).html('Error'); 
		}
	});

	return false;
}
/*
Text marker code 
Ashok Hariharan
http://www.unganisha.org/home/pages/dhtmlmarker/

Butchered by Chris Maunder to allow multiple word match
and preservation of search word case. http://www.codeproject.com
*/

// Wrap a word in a phrase with two strings.
// Input string contains 'phrase', is plain text and has no HTML tags.
// Output will be the string with 'phrase' wrapped by prefix and suffix strings.
function WrapMe(str, phrase, prefix, suffix) {
	var nPos;
	var result = "";
	var start = 0;
	var Searchable = str.toLowerCase();
	var Length = phrase.length;

	phrase = phrase.toLowerCase();
	while ((nPos = Searchable.indexOf(phrase, start)) >= 0) {
		result += str.substr(start, nPos - start);
		result += prefix + str.substr(nPos, Length) + suffix;
		start = nPos + Length;
	}
	result += str.substr(start);

	return result;
}

// Adds markup to text by sequentially searching for blocks of non-marked up text 
// that contains the search phrase
function markText(txtKeyword, inputHtml) {
	var prefix = '<span class="search-highlight">';
	var suffix = '</span>';
	var re = new RegExp("(\<[^>][^<]*\>)([^<]*)", "g"); // create non-greedy regex match

	/*
	note : in 99% of cases <[^>]*\>)([^<]*) should also work , I did have the rare case where some 
	invisible characters caused this to crap out ....
	*/

	var varMatches; 											 // matches array 
	var outHtml = new String(''); 								 // init html string
	while ((varMatches = re.exec(inputHtml.toString())) != null) // exec sequentially to apply span tags
	{
		outHtml += varMatches[1]; 								 // html tag part
		outHtml += WrapMe(varMatches[2], txtKeyword, prefix, suffix);
	}
	return outHtml;
}

function unmarkText(txtInputId, commandElmId) {
	elemObj = document.getElementById(txtInputId);
	if (elemObj) {
		var re = new RegExp('<span class="?search-highlight"?>([^<]*)?</span>', "gi"); // "i" because IE screws around.
		elemObj.parentNode.innerHTML = elemObj.parentNode.innerHTML.replace(re, "$1");
	}
	if (commandElmId) {
		elemObj = document.getElementById(commandElmId);
		if (elemObj) elemObj.className += " hidden";
	}
}

// Override markText to call it on an element of a given Id 
function markTextInElement(txtKeyword, txtInputId) {

	elemObj = document.getElementById(txtInputId);
	if (elemObj)
		elemObj.parentNode.innerHTML = markText(txtKeyword, elemObj.parentNode.innerHTML);
}
function social() {
	this.txtVersion = "1.2";
	this.addtoInterval = null;
	this.popupWin = '';
	this.addtoMethod=1;
	this.AddTitle=null;
	this.AddURL=null;
	this.Horizontal=false;
	this.Showname=false;
	this.ObjectTypeId=0;
	this.ObjectId=0;
	this.PlaceholderId='';
	
	this.sns=[
	 {name:'Twitter',		image: 'AddTo_Twitter.gif',		url: 'http://twitter.com/home?',			urlVar: 'status', titleVar: '',		noteVar: '', urlPre:'Currently reading ', returnVar: '', otherVars: '' },
	 {name:'Digg',			image: 'AddTo_Digg.png',		url: 'http://digg.com/submit?',				urlVar: 'url',	titleVar: '',		noteVar: '', urlPre:'',	returnVar: '',	otherVars: '&phase=2' },
	 {name:'Facebook',		image: 'AddTo_Facebook.png',	url: 'http://www.facebook.com/sharer.php?', urlVar: 'u',	titleVar: 't',		noteVar: '', urlPre:'',	returnVar: '', otherVars: '' },
	 {name:'Del.icio.us',	image: 'AddTo_Delicious.png',	url: 'http://del.icio.us/post?',			urlVar: 'url',	titleVar: 'title',	noteVar: '', urlPre:'',	returnVar: '',	otherVars: '' },
	 {name:'Reddit',		image: 'AddTo_Reddit.png',		url: 'http://reddit.com/submit?',			urlVar: 'url',	titleVar: 'title',	noteVar: '', urlPre:'',	returnVar: '',	otherVars: '' },
	 {name:'Stumbleupon',	image: 'AddTo_stumbleupon.png',	url: 'http://www.stumbleupon.com/submit?',	urlVar: 'url',	titleVar: 'title',	noteVar: '', urlPre:'',	 returnVar: '',	otherVars: '' },
	 {name:'Newsvine',		image: 'AddTo_Newsvine.png',	url: 'http://www.newsvine.com/_wine/save?',	urlVar: 'u',	titleVar: '',		noteVar: '', urlPre:'',	returnVar: '',	otherVars: '&popoff=0' },
	 {name:'Technorati',	image: 'AddTo_Technorati.png',	url: 'http://technorati.com/faves?',		urlVar: 'add',	titleVar: '',		noteVar: '', urlPre:'',	returnVar: '',	otherVars: '' },
	 {name:'Mr. Wong',		image: 'AddTo_MrWong.png',		url: 'http://www.mister-wong.com/index.php?',urlVar: 'bm_url',	titleVar: 'bm_description',	noteVar: '', urlPre:'',	returnVar: '', otherVars: '&action=addurl' },
	 {name:'Yahoo!',		image: 'AddTo_Yahoo.png',		url: 'http://myweb2.search.yahoo.com/myresults/bookmarklet?', urlVar: 'u', titleVar: 't',	noteVar: '', urlPre:'',	returnVar: '', otherVars: '&d=&ei=UTF-8' },
	 {name:'Google',		image: 'AddTo_Google.png',		url: 'http://www.google.com/bookmarks/mark?', urlVar: 'bkmk', titleVar: 'title',noteVar: '', urlPre:'',	returnVar: '',	otherVars: '&op=edit' },
	 {name:'Windows Live',	image: 'AddTo_Live.png',		url: 'https://favorites.live.com/quickadd.aspx?', urlVar: 'url', titleVar: 'title',	noteVar: '', urlPre:'',	returnVar: '',	otherVars: '&marklet=1&mkt=en-us&top=1' },
	 {name:'Send as Email',	image: 'envelope.gif',			url: '/script/common/TellFriend.aspx?obtid={0}&obid={1}&',urlVar: '', titleVar: '',	noteVar: '', prefix:'',	returnVar: '', otherVars: '' }

//	 {name:'Blink',			image: 'AddTo_Blink.png',		url: 'http://www.blinklist.com/index.php?', urlVar: 'url',	titleVar: 'title',	noteVar: 'description', urlPre:'',	returnVar: '', otherVars: '&Action=Blink/addblink.php' },
//	 {name:'Furl',			image: 'AddTo_Furl.png',		url: 'http://www.furl.net/storeIt.jsp?',	urlVar: 'u',	titleVar: 't',		noteVar: '', urlPre:'',	returnVar: '',	otherVars: '' },
//	 {name:'Simpy',			image: 'AddTo_Simpy.png',		url: 'http://simpy.com/simpy/LinkAdd.do?',	urlVar: 'href',	titleVar: 'title',	noteVar: 'note', urlPre:'',	 returnVar: '_doneURI', otherVars: '&v=6&src=bookmarklet' },
	];

	 this.DrawLink = function(varName, index, cellClass)
	 {
	 	var prompt = (index == (this.sns.length - 1)) ? '' : 'Add this page to ';
	 	var text = "<a class=\"" + cellClass + "\" title=\"" + prompt + this.sns[index].name +
	 				"\" onclick=\"return " + varName + ".addto(" + index.toString() + ");\" " +
					"href=\"#\"><img align=\"absmiddle\" src=\"/images/" + this.sns[index].image +
					"\" width=\"16px\" height=\"16px\" border=\"0\" /> ";
	 	if (this.Showname) text += this.sns[index].name;
	 	return text + "</a> ";
	 };

	this.setupLinks = function (varName, href, title, cols, width, headClass, cellClass) 
	{
		var i,text = "";
		this.AddTitle = title;
		this.AddURL = href;
		if (cols > this.sns.length) {
			if (this.Horizontal)text += "<span class=\"" + headClass + "\">Add this article to: </span> ";
			for (i = 0; i < this.sns.length; i++) {
				if (!this.Horizontal) text+="<div>";
				text+=this.DrawLink(varName, i, cellClass)
				text+= (this.Horizontal?" ":"</div>");
			}
		} else {
			text+="<table width=\"" + width.toString() + "\" border=\"0\" cellspacing=\"0\" " +
				  "cellpadding=\"0\"><tr><tr><td colspan=\"" + cols.toString() + "\" class=\"" + 
				  headClass + "\" height=\"20\">Add this article to:</td></tr>\n";
			for (i = 0; i < this.sns.length; i++) {
				text+="<td valign=\"middle\">";
				text+=this.DrawLink(varName, i, cellClass);
				text+="</td>";
				if ( (((i+1)%cols) == 0) && (i != (this.sns.length-1))) text+="</tr><tr>";
			}
			text+="</td></tr></table>";
		}

		if($("#ATD")[0])$("#ATD")[0].innerHTML = text; 		
	};

	this.addtoWin = function(addtoFullURL)
	{
		if (this.popupWin && !this.popupWin.closed && this.popupWin.location){
			this.popupWin.location.href = addtoFullURL;
			//this.addtoInterval = setInterval("this.closeAddTo();",1000);
		}
		else{
			this.popupWin = window.open(addtoFullURL,'addtoPopUp','width=770px,height=500px,menubar=1,toolbar=1,status=1,location=1,resizable=1,scrollbars=1,left=0,top=100');
			//this.addtoInterval = setInterval("this.closeAddTo();",1000);
			if (!this.popupWin.opener) this.popupWin.opener = self;
		}
		if (window.focus) {this.popupWin.focus()}
		return false;
	};
	
	// closes the popupWin
	this.closeAddTo = function() {
		if (!this.popupWin.closed && this.popupWin.location){
			if (this.popupWin.location.href == this.AddURL)	//if it's the same url as what was bookmarked, close the win
			this.popupWin.close();
		}
		else {	//if it's closed - clear the timer
			clearInterval(this.addtoInterval)
			return true
		}
	};

	this.addto = function (index) {
		if (!this.AddURL) this.AddURL = document.location.href;
		if (!this.AddTitle) this.AddTitle = escape(document.title);

		var url = this.sns[index].url.replace("{0}", this.ObjectTypeId.toString());
		url = url.replace("{1}", this.ObjectId.toString());
		var addtoFullURL = url;
			addtoFullURL += this.sns[index].urlVar + "=" + escape(this.sns[index].urlPre) + this.AddURL;
		if (this.sns[index].titleVar != "")
			addtoFullURL += "&" + this.sns[index].titleVar + "=" + this.AddTitle;
		addtoFullURL += this.sns[index].otherVars;
		if (this.sns[index].noteVar != "")
			addtoFullURL += "&" + this.sns[index].noteVar + "=" + this.AddTitle;
		if (this.sns[index].returnVar != "")
			addtoFullURL += "&" + this.sns[index].returnVar + "=" + this.AddURL;

		switch (this.addtoMethod) {
			case 0: // 0=direct link
				self.location = addtoFullURL
				break
			case 1: // 1=popup
				this.addtoWin(addtoFullURL);
				break
			default:
		}
		return false;
	};
	
	this.setupMenu = function() {
		var over = false;
		$(document).ready(function(){
			var e = $("#SBI"), m = $("#ATD");
			if (!e.length||!m.length) return;
			var pos = $(e).position();
			var eWidth = $(e).outerWidth();
			var mWidth = $(m).outerWidth();
			var eHeight = $(e).outerHeight();
			var left = (pos.left + (eWidth - mWidth)/2) + "px";
			var top = eHeight+pos.top + "px";
			$(m).css({ left: left, top: top });

			$(e).hover(
				function() { $(m).stop(true, true).fadeIn('fast'); }, 
				function() { setTimeout( function() { if (!over) $("#ATD").fadeOut('fast');}, 200); }
			);
			$(m).hover( function() { over=true;}, function() { over=false; $(m).fadeOut('fast');} );
		});
	}
}
// Copyright (c) 2008, The Code Project. All rights reserved.
/// <reference path="jquery-1.3.2-vsdoc2.js" />

// Initialize watermark on input element.
function InitWatermark(inputId, label) {
	var inputObj = $('#' + inputId);
	if (!inputObj) return;

	inputObj.blur(function() { UpdateWatermark(this, label); });
	inputObj.focus(function() { UpdateWatermark(this, label); });

	var text = $.trim(inputObj.val());
	if (text == '' || text == label) {
		inputObj.val(label);
		inputObj.addClass('subdue');
	}
	else if (text != '' && text != label)
		inputObj.removeClass('subdue');
}

// Update the watermark as text changes within the input element.
function UpdateWatermark(inputObj, label) {
	if (!inputObj) return;

	var text = $.trim($(inputObj).val());
	if (text == label && $(inputObj).hasClass('subdue')) {
		$(inputObj).val('');
		$(inputObj).removeClass('subdue');
	}
	else if (text == '' && !$(inputObj).hasClass('subdue')) {
		$(inputObj).val(label);
		$(inputObj).addClass('subdue');
	}
	else
		$(inputObj).removeClass('subdue');
}

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
// Makes an element stick to the top of a page on scroll:
//
//   $(elm).MakeSticky("sticky_div", "stop_div", "top");    // only "top" supported so far.

(function ($) {

    $.sticky = function (stickyElement, $stopElement, stickyPos) {

        // To avoid scope issues, use 'self' instead of 'this' to reference this class from internal 
        // events and functions.
        var self = this;

        var $stickyElement = $(stickyElement);
        var padding        = 10;
        var isStuck        = false;
        var originalLayout;

        function GetOriginalLayout() {
            if ($stickyElement) {

                var position = $stickyElement.offset();

                var original = new Object();
                original.isStickyRequired = stickyPos === "top";
                original.top      = position.top;
                original.left     = position.left;
                original.position = $stickyElement.css('position');

                // if one of the coords is negative return null to identify that we can't sticky
                if (original.top === -1)
                    return null;

                return original;
            }
        };

        function ResetLayout() {
            if (originalLayout) {
                $stickyElement.css('position', originalLayout.position);
                $stickyElement.css('top',      originalLayout.top);
                $stickyElement.css('left',     originalLayout.left);
                $stickyElement.removeClass("stuck");
                isStuck = false;
            }
        }

        self.ReGetOriginalLayout = function () {
            if (originalLayout) {

                // Position the element back normally
                ResetLayout();

                // recreate the settings to account for possible absolute position changes
                originalLayout = GetOriginalLayout();

                // apply new settings
                self.RepositionStickyElement();
            }
        };

        self.RepositionStickyElement = function () {

            var stopBounds;
            if ($stopElement && $stopElement.length)
                stopBounds = getBoundingRect($stopElement); 

            var scrollTop    = $(window).scrollTop();
            var stickyBounds = getBoundingRect($stickyElement); 

            var stickTop;
            if (stopBounds && stickyBounds.bottom > stopBounds.top) {
                stickTop = stickyBounds.top - (stickyBounds.bottom - stopBounds.top) - scrollTop;
            } else if (scrollTop + padding >= originalLayout.top)
                stickTop = padding;
            else if (!isStuck)
                originalLayout = GetOriginalLayout();

            // PROBLEM: fixed elements are not being identified as fixed
            // if (originalLayout.position == 'fixed' && stickyBounds.bottom < stopBounds.top)
            //    stickTop = 0; // Leave already fixed elements where they were if above bottom stop.

            // If the element had previously hit a stop element then it would have been positioned
            // such that it wasn't overlapping the stop element. If we're now thinking we can set it
            // back to the top of the frame then we need to double-check
            if (stickTop && stopBounds && (scrollTop + stickTop + stickyBounds.height) > stopBounds.top)
                stickTop = stickyBounds.top - (stickyBounds.bottom - stopBounds.top) - scrollTop;

            if (stickTop) {

                // self.ReGetOriginalLayout(true);

                $stickyElement.css('position', 'fixed');
                $stickyElement.css('top',      stickTop);
                $stickyElement.css('left',     originalLayout.left -$(window).scrollLeft());
                $stickyElement.addClass("stuck");

                isStuck = true;

                // Make sure we're not pushing the sticky element above the original location.
                /* COMMENTED: (CM) slows things down and crashes IE. Awesome.
                stickyBounds = getBoundingRect($stickyElement);
                if (stickyBounds.top <= originalLayout.top)
                    ResetLayout(); 
                */
            }
            else
                ResetLayout();
        };

        function getBoundingRect($element) {
            var bounds = $element.offset();

            bounds.width  = $element.outerWidth(true); // true = include margins in the value
            bounds.height = $element.outerHeight(true);

            bounds.right  = bounds.left + bounds.width;
            bounds.bottom = bounds.top  + bounds.height;

            // Not needed.
            // bounds.marginVertical   = (bounds.height - $element.outerHeight(false)) / 2;
            // bounds.marginHorizontal = (bounds.width  - $element.outerWidth(false))  / 2;

            return bounds;
        }

        self.init = function () {

            stickyPos = stickyPos || "top";

            if ($stickyElement) {
                originalLayout = GetOriginalLayout();

                $(window).bind('resize', self.ReGetOriginalLayout);
                $(window).bind('scroll', self.RepositionStickyElement);

                // Looks crappy. The content to be fixed slides underneath the newly fixed content
                /*
                if ('ontouchmove' in window)
                    $(window).bind('touchmove', self.RepositionStickyElement);
                */
            }
        }

        self.init();
    };

    // Returns enhanced elements that will fix to the top of the page when the
    // page is scrolled.
    $.fn.sticky = function ($stopElement, stickyPos) {
        return this.each(function () {
            (new $.sticky(this, $stopElement, stickyPos));
        });
    };

})(jQuery);
function NotificationAlert(parentId, elementId, ajaxPageUrl) {

	// Public variables ----------------------------------------------------------------------------

	// this.updateMessageUrl = ''; 		 // The URL to call when updating the message

	// Private variables ---------------------------------------------------------------------------

	var self         = this;
	var _parent      = $('#' + parentId);
	var _element     = $('#' + elementId);
	var _ajaxPageUrl = ajaxPageUrl;

	// Sets up a flyout menu with the flag options. Recreated each time - may want to create once
	// and store if dynamic creation not needed.
	var setupAlertBox = function () {
		if (!_parent.populated && !_parent.populating) {
			_parent.populating = true;
			_element.load(_ajaxPageUrl, {cache:false}, function () { _parent.populated = true; });
			_parent.populated = true;
			_parent.populating = false;
		}
	}

	// Call service url to update article status.
	this.MarkAsRead = function (notificationId, serviceUrl, linkId, containerId) {

		var link = $('#' + linkId);
		var oldHtml = link.html();
		link.html('<img src="/Images/animated_loading_blue.gif" align="absmiddle" style="border:0" />');

		$.ajax(
		{
			type:		'POST',
			cache:		false,
			url:		serviceUrl,
			data:		'{notificationId: ' + notificationId + '}',
			contentType: 'application/json; charset=utf-8',
			dataType:	'json',
			tryCount:	0,
			retryLimit: 3,
			timeout:	5000,

			success: function () {
				link.remove();
				$('#' +containerId).children().first().removeClass('bold');	
			},
			error: function () {
				link.html(oldHtml);		
			}
		});

		return false;
	}

	// Setup everything.
	this.initialise = function () {
		$(_parent).bind('mouseover', setupAlertBox);
	}
}

/*
var notificationAlert;
$(document).ready(function () {
	notificationAlert = new NotificationAlert(parent, 'elementId', 'http://mydomain.com/ajax');
	notificationAlert.initialise();
});
*/
// Copyright (c) 2008, The Code Project. All rights reserved.
/// <reference path="../../JS/jquery-1.3.2-vsdoc2.js" />

function getInternetExplorerVersion() {
    var rv = -1; // Return value assumes failure.
    if (navigator.appName == 'Microsoft Internet Explorer') {
        var ua = navigator.userAgent;
        var re = new RegExp("MSIE ([0-9]{1,}[\.0-9]{0,})");
        if (re.exec(ua) != null)
            rv = parseFloat(RegExp.$1);
    }
    return rv;
}

function rateItem(objId, objTypeId, objSubtypeId, forceComment, allowAjaxOnLowVote, lowVoteThreshold, displayMode) {
	if (objId > 0 && objTypeId > 0) {
		var value = $("div[name=RateItem_" + objId + "]").find("input[type=radio]:checked").val();

		var comment = $("[name=RateItem_" + objId + "] .RateComment")[0];
		if (comment) comment = comment.value.replace(/^\s+|\s+$/g, ''); else comment = '';

		if (value <= 0)
			ShowErrorMessage("You must select a value to vote", objId);
		else if (value <= lowVoteThreshold && comment == '' && forceComment && allowAjaxOnLowVote)
			ShowErrorMessage("You must provide a comment", objId);
		else if (allowAjaxOnLowVote || value > lowVoteThreshold) {
			PrepElements(objId);
			$.get("/Script/Ratings/Ajax/RateItem.aspx?obid=" + objId + "&obtid=" + objTypeId +
					"&obstid=" + objSubtypeId + "&rvv=" + value + "&rvc=" + escape(comment) +
					(displayMode ? "&rdm=" + displayMode : ""),
					function (data) { ratingAjaxCallback(data, objId); });
		}
	}
	return false;
}

function ratingAjaxCallback(data, objId) {
	if (data.length > 0) {
		var voteRes = $("div[name=RateItem_" + objId + "] .rating-result span")[0];
		if (voteRes) {
			voteRes.innerHTML = data;
			voteRes.style.display = "";
		}
		voteRes = $("[name=CurRat_" + objId + "]")[0];
		if (voteRes) voteRes.style.display = "none";
	}
	var loader = $("div[name=RateItem_" + objId + "] .loaderImg")[0];

	// We may have an ajax tooltip here. Remove it because we'll get a histogram with the return
	// data
	$("div[name=RateItem_" + objId + "] .ajaxHist").remove();
	if (loader && loader.style) loader.style.display = "none";

	$(".rating-comment").remove(); //off("mouseenter mouseleave");
}

function PrepElements(objId) {
	var loader = $("div[name=RateItem_" + objId + "] .loaderImg")[0];
	if (loader && loader.style.display == "none")
		loader.style.display = "";

	loader = $("div[name=RateItem_" + objId + "] .voteTbl")[0];
	if (loader) loader.style.display = "none";

	loader = $("div[name=RateItem_" + objId + "] .rating-result span")[0];
	if (loader) loader.style.display = "none";

	loader = $("div[name=RateItem_" + objId + "] .rating-comment")[0];
	if (loader) loader.style.display = "none";
}

function ShowErrorMessage(msg, objId) {
	var loader = $("div[name=RateItem_" + objId + "] .loaderImg")[0];
	if (loader) loader.style.display = "none";
}

function starRating(selector, ratingCallback, clientID) {
	var initialise = function (selector, ratingCallback) {

	    var ieVersion = getInternetExplorerVersion();
	    var isIE8orBelow = ieVersion > 0 && ieVersion <= 8;

		// loop over every element matching the selector
		$(selector).each(function () {

			// Hide submit button
			$(this).find('input:submit').hide();

			// On hover, hide content currently showing and show rating (only if we have content)
			if ($(".content", this).length > 0) {
				// Initially hide rating control container
				$(this).find(".voting").hide();

				// Toggle between rating control and content during mouseover
				$(this).hover(function () {
					$(".content", this).hide();
					$(".voting", this).show();
				}, function () {
					$(".content", this).show();
					$(".voting", this).hide();
				});
			}

			// Create rating control from radio list. Loop over buttons.
			var $list = $('<div class="rating-star-block"></div>');
			$(this)
				.find('input:radio')
				.each(function (i) {
					// var rating = $(this).parent().text(); - if wrapping in labels
					var rating = "vote " + this.value;
					var $item = $('<a href="#"></a>')
						.attr('rating', this.value)
						.text(rating)
						.addClass("star outline");

					if (!isIE8orBelow)
					    $item.attr('title', rating)

					addHandlers($item, clientID);
					$list.append($item);

					if ($(this).is(':checked'))
						$item.prevAll().andSelf().removeClass('outline').addClass('filled');
				});

			// Hide the original radio buttons
			//$(this).find(".voting").append($list).find('label').hide(); - if wrapping in labels
			$(this).find(".voting").append($list).find('input:radio').hide();
		});
	}

    /*
	var showCommentBox = function (objId, show) {
		var commentBox = $('#' + objId + '_RCD');
		if (show) commentBox.show(); else commentBox.hide();
	}
    */

	var addHandlers = function (item, clientID) {
		// Show comment box on mouse over
		// $(item).mouseenter(function (e) {
		//	showCommentBox(clientID, true);
		//});

		// Actual rating
		$(item).click(function (e) {
			// Handle Star click
			var $star = $(this);
			var $linksParent = $(this).parent();
			var rating = $star.attr('rating');

			// Set the radio button value
			$linksParent
				.parent()
				.find('input:radio[value=' + rating + ']')
				.attr('checked', true);

			// prevent default link click
			e.preventDefault();

			// Execute the callback or find parent form and submit it instead.
			if (ratingCallback) ratingCallback(rating);
			else $(this).closest('form').submit();

		}).hover(function () {
			// Handle star mouse over
			$(this).prevAll().andSelf().removeClass("outline").addClass('filled');
			$(this).nextAll().addClass('outline');
		}, function () {
			// Handle star mouse out
			$(this).siblings().andSelf().removeClass('filled').addClass('outline')
		});
	}

	initialise(selector, ratingCallback);
}
function ReportMe(objId, objTypeId, reportTypeId, url, directLink, commentsClientId) {
	var canReport = true;
	if (directLink)
		canReport = confirm("Are you sure your want to report this item")
	if (canReport) {
		var idPrefix = "rpt_";
		var reportLink = GetReportLink(idPrefix, objId.toString(), objTypeId.toString(), reportTypeId.toString());
		var reportStatusMsg = GetReportStatusMsgElm(idPrefix, objId.toString(), objTypeId.toString(), reportTypeId.toString(), directLink);

		var comments = $('#' + commentsClientId).val();
		if (ShowReportingMessage(reportLink, reportStatusMsg))
			$(reportStatusMsg).load(url + '&comments=' + escape(comments));
	}
	return false;
}

function GetReportLink(idPrefix, objectId, objectTypeId, reportTypeId) {
	return $("a[name=" + idPrefix + objectId + "_" + objectTypeId + "_" + reportTypeId + "]");
}
function GetReportStatusMsgElm(idPrefix, objectId, objectTypeId, reportTypeId, directLink) {
	var elm = document.getElementsByName(idPrefix + objectId + "_" + objectTypeId + "_" + reportTypeId)[0];
	return directLink? elm.parentNode : elm.parentNode.parentNode.parentNode;
}

function ShowReportingMessage(reportLink, reportStatusMsg) {
	if (!reportLink || !reportStatusMsg) return false;

	$(reportLink).css("display", "none");
	$(reportStatusMsg).html('<img src="/images/animated_loading_blue.gif" width="16px" height="16px" border="0">');
	$(reportStatusMsg).css("display", "");

	return true;
}

function CheckForUndo(elm, memberID) {
	if ($(elm).prop('checkForUndo')) return;

	var undo = $(elm).find(".Undo");

	// TODO:  We need to do two things: (issue #62 Add ability to remove reports)
	// 1. Create an ajax page that will check if the current member (in the javascript var "reportingMemberId")
	//    has reported this object, and if so, show a link that says "remove your report"
	// 2. Add the service to remove reports

	$(elm).prop('checkForUndo', true);
}
function bookmarkMe(objId, objTypeId, url, imageSize, bookmarkLink, statusMsg) {
	var idPrefix     = "bm_";
	var bmkLink      = bookmarkLink || GetBmkLink(idPrefix, objId.toString(), objTypeId.toString());
	var bmkStatusMsg = statusMsg    || GetBmkStatusMsg(idPrefix, objId.toString(), objTypeId.toString());

	if (LoadWaitMessage(bmkLink, bmkStatusMsg, imageSize))
		$(bmkStatusMsg).load(url);
	return false;
}

function watchMe(objId, objTypeId, url, imageSize, bookmarkLink, statusMsg) {
	var idPrefix     = "bmw_";
	var bmkLink      = bookmarkLink || GetBmkLink(idPrefix, objId.toString(), objTypeId.toString());
	var bmkStatusMsg = statusMsg    || GetBmkStatusMsg(idPrefix, objId.toString(), objTypeId.toString());

	if (LoadWaitMessage(bmkLink, bmkStatusMsg, imageSize))
		$(bmkStatusMsg).load(url);
	return false;
}

function GetBmkLink(idPrefix, objectId, objectTypeId) {
	return $("a[name=" + idPrefix + objectId + "_" + objectTypeId + "]");
}
function GetBmkStatusMsg(idPrefix, objectId, objectTypeId) {
	return document.getElementsByName(idPrefix + objectId + "_" + objectTypeId)[0];
}

function LoadWaitMessage(bmkLink, bmkStatusMsg, imageSize) {
	if (!bmkLink || !bmkStatusMsg) return false;

	$(bmkLink).css("display", "none");
	if (imageSize == "large")
	    $(bmkStatusMsg).html('<img src="/images/animated_loading" width="32px" height="32px" border="0" class="align-top">');
	else if (imageSize == "medium")
	    $(bmkStatusMsg).html('<img src="/images/loading.gif" width="24px" height="24px" border="0" class="align-top">');
	else
		$(bmkStatusMsg).html('<img src="/images/animated_loading_blue.gif" width="16px" height="16px" border="0">');

	$(bmkStatusMsg).css("display", "");

	return true;
}
// A jQuery plugin to get an Advertisement html from Lake Quincy and put it into an element
// (c) 2010 The Code Project
// Author : Matthew Dennis
// Date : July 21, 2010

(function ($)
{
    function getRandom()
    {
        var result, i, j;
        result = '';
        for (j = 0; j < 32; j++)
        {
            if (j == 8 || j == 12 || j == 16 || j == 20)
                result = result + '-';
            i = Math.floor(Math.random() * 16).toString(16).toUpperCase();
            result = result + i;
        }
        return result
    }

    jQuery.fn.AdSigniaAd = function (url)
    {
        var pageId = getRandom();

        this.each(function (index)
        {
            var self = this;
            var requestData = {};
            requestData.channel = $(this).attr('lqm_channel');
            requestData.publisher = $(this).attr('lqm_publisher');
            requestData.zone = $(this).attr('lqm_zone');
            requestData.format = $(this).attr('lqm_format');

            var country = $(this).attr('lqm_country');
            if (country != undefined)
                requestData.country = country;

            var tags = $(this).attr('lqm_tags');
            if (tags != undefined)
                requestData.tags = tags;

            requestData.ypos = $(this).offset().top;
            requestData.pageid = pageId;
            requestData.ct = index + 1;

            requestData.renderhtml = true;

            requestData.target = "_blank";
            var srcStr = url + "?&" + $.param(requestData);

            $(this).attr('src', srcStr);
        });
    };
})(jQuery);
/*******

	***	Anchor Slider by Cedric Dugas   ***
	*** Http://www.position-absolute.com ***
	
	Never have an anchor jumping your content, slide it.

	Don't forget to put an id to your anchor !
	You can use and modify this script for any project you want, but please leave this comment as credit.
	
*****/

function scrollToAnchor(hash, settings) {

    settings = jQuery.extend({
        speed: 1100
    }, settings);

    var elmId;
    var anchorIndex = hash.indexOf("#");
    if (anchorIndex > 0)
    	elmId = hash.substring(anchorIndex);
    else
    	elmId = hash;

	var destination = $(elmId);

	if (destination.length > 0) {
		var top = destination.offset().top;
		$("html:not(:animated),body:not(:animated)").animate({ scrollTop: top }, settings.speed, function () {
			window.location.hash = hash
		});
	}

	return false;
}

function anchorAnimate(settings) {

	$('a.anchorLink').each(function () {
		var caller = this;

		$(caller).click(function (event) {
			event.preventDefault();
			var locationHref = window.location.href
			var elementClick = $(caller).attr("href")

			var destination;
			var anchorIndex = elementClick.indexOf("#");
			if (anchorIndex > 0)
				destination = elementClick.substring(anchorIndex);
			else
				destination = elementClick;

			return scrollToAnchor(destination, settings);
		});
	});
}
(function(n){n.fn.hoverIntent=function(t,i){var r={sensitivity:7,interval:100,timeout:0};r=n.extend(r,i?{over:t,out:i}:t);var u,f,e,o,s=function(n){u=n.pageX;f=n.pageY},h=function(t,i){if(i.hoverIntent_t=clearTimeout(i.hoverIntent_t),Math.abs(e-u)+Math.abs(o-f)<r.sensitivity)return n(i).unbind("mousemove",s),i.hoverIntent_s=1,r.over.apply(i,[t]);e=u;o=f;i.hoverIntent_t=setTimeout(function(){h(t,i)},r.interval)},l=function(n,t){return t.hoverIntent_t=clearTimeout(t.hoverIntent_t),t.hoverIntent_s=0,r.out.apply(t,[n])},c=function(t){var u=jQuery.extend({},t),i=this;i.hoverIntent_t&&(i.hoverIntent_t=clearTimeout(i.hoverIntent_t));t.type=="mouseenter"?(e=u.pageX,o=u.pageY,n(i).bind("mousemove",s),i.hoverIntent_s!=1&&(i.hoverIntent_t=setTimeout(function(){h(u,i)},r.interval))):(n(i).unbind("mousemove",s),i.hoverIntent_s==1&&(i.hoverIntent_t=setTimeout(function(){l(u,i)},r.timeout)))};return this.bind("mouseenter",c).bind("mouseleave",c)}})(jQuery);
/*
 * flowplayer.js 3.2.8. The Flowplayer API
 *
 * Copyright 2009-2011 Flowplayer Oy
 *
 * This file is part of Flowplayer.
 *
 * Flowplayer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Flowplayer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Flowplayer.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Date: 2011-12-30 12:34:08 -0500 (Fri, 30 Dec 2011)
 * Revision: 761
 */ (function () {
	function g(o) {
		console.log("$f.fireEvent", [].slice.call(o))
	}
	function k(q) {
		if (!q || typeof q != "object") {
			return q
		}
		var o = new q.constructor();
		for (var p in q) {
			if (q.hasOwnProperty(p)) {
				o[p] = k(q[p])
			}
		}
		return o
	}
	function m(t, q) {
		if (!t) {
			return
		}
		var o, p = 0,
			r = t.length;
		if (r === undefined) {
			for (o in t) {
				if (q.call(t[o], o, t[o]) === false) {
					break
				}
			}
		} else {
			for (var s = t[0]; p < r && q.call(s, p, s) !== false; s = t[++p]) {}
		}
		return t
	}
	function c(o) {
		return document.getElementById(o)
	}
	function i(q, p, o) {
		if (typeof p != "object") {
			return q
		}
		if (q && p) {
			m(p, function (r, s) {
				if (!o || typeof s != "function") {
					q[r] = s
				}
			})
		}
		return q
	}
	function n(s) {
		var q = s.indexOf(".");
		if (q != -1) {
			var p = s.slice(0, q) || "*";
			var o = s.slice(q + 1, s.length);
			var r = [];
			m(document.getElementsByTagName(p), function () {
				if (this.className && this.className.indexOf(o) != -1) {
					r.push(this)
				}
			});
			return r
		}
	}
	function f(o) {
		o = o || window.event;
		if (o.preventDefault) {
			o.stopPropagation();
			o.preventDefault()
		} else {
			o.returnValue = false;
			o.cancelBubble = true
		}
		return false
	}
	function j(q, o, p) {
		q[o] = q[o] || [];
		q[o].push(p)
	}
	function e() {
		return "_" + ("" + Math.random()).slice(2, 10)
	}
	var h = function (t, r, s) {
			var q = this,
				p = {},
				u = {};
			q.index = r;
			if (typeof t == "string") {
				t = {
					url: t
				}
			}
			i(this, t, true);
			m(("Begin*,Start,Pause*,Resume*,Seek*,Stop*,Finish*,LastSecond,Update,BufferFull,BufferEmpty,BufferStop").split(","), function () {
				var v = "on" + this;
				if (v.indexOf("*") != -1) {
					v = v.slice(0, v.length - 1);
					var w = "onBefore" + v.slice(2);
					q[w] = function (x) {
						j(u, w, x);
						return q
					}
				}
				q[v] = function (x) {
					j(u, v, x);
					return q
				};
				if (r == -1) {
					if (q[w]) {
						s[w] = q[w]
					}
					if (q[v]) {
						s[v] = q[v]
					}
				}
			});
			i(this, {
				onCuepoint: function (x, w) {
					if (arguments.length == 1) {
						p.embedded = [null, x];
						return q
					}
					if (typeof x == "number") {
						x = [x]
					}
					var v = e();
					p[v] = [x, w];
					if (s.isLoaded()) {
						s._api().fp_addCuepoints(x, r, v)
					}
					return q
				},
				update: function (w) {
					i(q, w);
					if (s.isLoaded()) {
						s._api().fp_updateClip(w, r)
					}
					var v = s.getConfig();
					var x = (r == -1) ? v.clip : v.playlist[r];
					i(x, w, true)
				},
				_fireEvent: function (v, y, w, A) {
					if (v == "onLoad") {
						m(p, function (B, C) {
							if (C[0]) {
								s._api().fp_addCuepoints(C[0], r, B)
							}
						});
						return false
					}
					A = A || q;
					if (v == "onCuepoint") {
						var z = p[y];
						if (z) {
							return z[1].call(s, A, w)
						}
					}
					if (y && "onBeforeBegin,onMetaData,onStart,onUpdate,onResume".indexOf(v) != -1) {
						i(A, y);
						if (y.metaData) {
							if (!A.duration) {
								A.duration = y.metaData.duration
							} else {
								A.fullDuration = y.metaData.duration
							}
						}
					}
					var x = true;
					m(u[v], function () {
						x = this.call(s, A, y, w)
					});
					return x
				}
			});
			if (t.onCuepoint) {
				var o = t.onCuepoint;
				q.onCuepoint.apply(q, typeof o == "function" ? [o] : o);
				delete t.onCuepoint
			}
			m(t, function (v, w) {
				if (typeof w == "function") {
					j(u, v, w);
					delete t[v]
				}
			});
			if (r == -1) {
				s.onCuepoint = this.onCuepoint
			}
		};
	var l = function (p, r, q, t) {
			var o = this,
				s = {},
				u = false;
			if (t) {
				i(s, t)
			}
			m(r, function (v, w) {
				if (typeof w == "function") {
					s[v] = w;
					delete r[v]
				}
			});
			i(this, {
				animate: function (y, z, x) {
					if (!y) {
						return o
					}
					if (typeof z == "function") {
						x = z;
						z = 500
					}
					if (typeof y == "string") {
						var w = y;
						y = {};
						y[w] = z;
						z = 500
					}
					if (x) {
						var v = e();
						s[v] = x
					}
					if (z === undefined) {
						z = 500
					}
					r = q._api().fp_animate(p, y, z, v);
					return o
				},
				css: function (w, x) {
					if (x !== undefined) {
						var v = {};
						v[w] = x;
						w = v
					}
					r = q._api().fp_css(p, w);
					i(o, r);
					return o
				},
				show: function () {
					this.display = "block";
					q._api().fp_showPlugin(p);
					return o
				},
				hide: function () {
					this.display = "none";
					q._api().fp_hidePlugin(p);
					return o
				},
				toggle: function () {
					this.display = q._api().fp_togglePlugin(p);
					return o
				},
				fadeTo: function (y, x, w) {
					if (typeof x == "function") {
						w = x;
						x = 500
					}
					if (w) {
						var v = e();
						s[v] = w
					}
					this.display = q._api().fp_fadeTo(p, y, x, v);
					this.opacity = y;
					return o
				},
				fadeIn: function (w, v) {
					return o.fadeTo(1, w, v)
				},
				fadeOut: function (w, v) {
					return o.fadeTo(0, w, v)
				},
				getName: function () {
					return p
				},
				getPlayer: function () {
					return q
				},
				_fireEvent: function (w, v, x) {
					if (w == "onUpdate") {
						var z = q._api().fp_getPlugin(p);
						if (!z) {
							return
						}
						i(o, z);
						delete o.methods;
						if (!u) {
							m(z.methods, function () {
								var B = "" + this;
								o[B] = function () {
									var C = [].slice.call(arguments);
									var D = q._api().fp_invoke(p, B, C);
									return D === "undefined" || D === undefined ? o : D
								}
							});
							u = true
						}
					}
					var A = s[w];
					if (A) {
						var y = A.apply(o, v);
						if (w.slice(0, 1) == "_") {
							delete s[w]
						}
						return y
					}
					return o
				}
			})
		};

	function b(q, G, t) {
		var w = this,
			v = null,
			D = false,
			u, s, F = [],
			y = {},
			x = {},
			E, r, p, C, o, A;
		i(w, {
			id: function () {
				return E
			},
			isLoaded: function () {
				return (v !== null && v.fp_play !== undefined && !D)
			},
			getParent: function () {
				return q
			},
			hide: function (H) {
				if (H) {
					q.style.height = "0px"
				}
				if (w.isLoaded()) {
					v.style.height = "0px"
				}
				return w
			},
			show: function () {
				q.style.height = A + "px";
				if (w.isLoaded()) {
					v.style.height = o + "px"
				}
				return w
			},
			isHidden: function () {
				return w.isLoaded() && parseInt(v.style.height, 10) === 0
			},
			load: function (J) {
				if (!w.isLoaded() && w._fireEvent("onBeforeLoad") !== false) {
					var H = function () {
							if (u && !flashembed.isSupported(G.version)) {
								q.innerHTML = ""
							}
							if (J) {
								J.cached = true;
								j(x, "onLoad", J)
							}
							flashembed(q, G, {
								config: t
							})
						};
					var I = 0;
					m(a, function () {
						this.unload(function (K) {
							if (++I == a.length) {
								H()
							}
						})
					})
				}
				return w
			},
			unload: function (J) {
				if (u.replace(/\s/g, "") !== "") {
					if (w._fireEvent("onBeforeUnload") === false) {
						if (J) {
							J(false)
						}
						return w
					}
					D = true;
					try {
						if (v) {
							v.fp_close();
							w._fireEvent("onUnload")
						}
					} catch (H) {}
					var I = function () {
							v = null;
							q.innerHTML = u;
							D = false;
							if (J) {
								J(true)
							}
						};
					if (/WebKit/i.test(navigator.userAgent) && !/Chrome/i.test(navigator.userAgent)) {
						setTimeout(I, 0)
					} else {
						I()
					}
				} else {
					if (J) {
						J(false)
					}
				}
				return w
			},
			getClip: function (H) {
				if (H === undefined) {
					H = C
				}
				return F[H]
			},
			getCommonClip: function () {
				return s
			},
			getPlaylist: function () {
				return F
			},
			getPlugin: function (H) {
				var J = y[H];
				if (!J && w.isLoaded()) {
					var I = w._api().fp_getPlugin(H);
					if (I) {
						J = new l(H, I, w);
						y[H] = J
					}
				}
				return J
			},
			getScreen: function () {
				return w.getPlugin("screen")
			},
			getControls: function () {
				return w.getPlugin("controls")._fireEvent("onUpdate")
			},
			getLogo: function () {
				try {
					return w.getPlugin("logo")._fireEvent("onUpdate")
				} catch (H) {}
			},
			getPlay: function () {
				return w.getPlugin("play")._fireEvent("onUpdate")
			},
			getConfig: function (H) {
				return H ? k(t) : t
			},
			getFlashParams: function () {
				return G
			},
			loadPlugin: function (K, J, M, L) {
				if (typeof M == "function") {
					L = M;
					M = {}
				}
				var I = L ? e() : "_";
				w._api().fp_loadPlugin(K, J, M, I);
				var H = {};
				H[I] = L;
				var N = new l(K, null, w, H);
				y[K] = N;
				return N
			},
			getState: function () {
				return w.isLoaded() ? v.fp_getState() : -1
			},
			play: function (I, H) {
				var J = function () {
						if (I !== undefined) {
							w._api().fp_play(I, H)
						} else {
							w._api().fp_play()
						}
					};
				if (w.isLoaded()) {
					J()
				} else {
					if (D) {
						setTimeout(function () {
							w.play(I, H)
						}, 50)
					} else {
						w.load(function () {
							J()
						})
					}
				}
				return w
			},
			getVersion: function () {
				var I = "flowplayer.js 3.2.8";
				if (w.isLoaded()) {
					var H = v.fp_getVersion();
					H.push(I);
					return H
				}
				return I
			},
			_api: function () {
				if (!w.isLoaded()) {
					throw "Flowplayer " + w.id() + " not loaded when calling an API method"
				}
				return v
			},
			setClip: function (H) {
				m(H, function (I, J) {
					if (typeof J == "function") {
						j(x, I, J);
						delete H[I]
					} else {
						if (I == "onCuepoint") {
							$f(q).getCommonClip().onCuepoint(H[I][0], H[I][1])
						}
					}
				});
				w.setPlaylist([H]);
				return w
			},
			getIndex: function () {
				return p
			},
			bufferAnimate: function (H) {
				v.fp_bufferAnimate(H === undefined || H);
				return w
			},
			_swfHeight: function () {
				return v.clientHeight
			}
		});
		m(("Click*,Load*,Unload*,Keypress*,Volume*,Mute*,Unmute*,PlaylistReplace,ClipAdd,Fullscreen*,FullscreenExit,Error,MouseOver,MouseOut").split(","), function () {
			var H = "on" + this;
			if (H.indexOf("*") != -1) {
				H = H.slice(0, H.length - 1);
				var I = "onBefore" + H.slice(2);
				w[I] = function (J) {
					j(x, I, J);
					return w
				}
			}
			w[H] = function (J) {
				j(x, H, J);
				return w
			}
		});
		m(("pause,resume,mute,unmute,stop,toggle,seek,getStatus,getVolume,setVolume,getTime,isPaused,isPlaying,startBuffering,stopBuffering,isFullscreen,toggleFullscreen,reset,close,setPlaylist,addClip,playFeed,setKeyboardShortcutsEnabled,isKeyboardShortcutsEnabled").split(","), function () {
			var H = this;
			w[H] = function (J, I) {
				if (!w.isLoaded()) {
					return w
				}
				var K = null;
				if (J !== undefined && I !== undefined) {
					K = v["fp_" + H](J, I)
				} else {
					K = (J === undefined) ? v["fp_" + H]() : v["fp_" + H](J)
				}
				return K === "undefined" || K === undefined ? w : K
			}
		});
		w._fireEvent = function (Q) {
			if (typeof Q == "string") {
				Q = [Q]
			}
			var R = Q[0],
				O = Q[1],
				M = Q[2],
				L = Q[3],
				K = 0;
			if (t.debug) {
				g(Q)
			}
			if (!w.isLoaded() && R == "onLoad" && O == "player") {
				v = v || c(r);
				o = w._swfHeight();
				m(F, function () {
					this._fireEvent("onLoad")
				});
				m(y, function (S, T) {
					T._fireEvent("onUpdate")
				});
				s._fireEvent("onLoad")
			}
			if (R == "onLoad" && O != "player") {
				return
			}
			if (R == "onError") {
				if (typeof O == "string" || (typeof O == "number" && typeof M == "number")) {
					O = M;
					M = L
				}
			}
			if (R == "onContextMenu") {
				m(t.contextMenu[O], function (S, T) {
					T.call(w)
				});
				return
			}
			if (R == "onPluginEvent" || R == "onBeforePluginEvent") {
				var H = O.name || O;
				var I = y[H];
				if (I) {
					I._fireEvent("onUpdate", O);
					return I._fireEvent(M, Q.slice(3))
				}
				return
			}
			if (R == "onPlaylistReplace") {
				F = [];
				var N = 0;
				m(O, function () {
					F.push(new h(this, N++, w))
				})
			}
			if (R == "onClipAdd") {
				if (O.isInStream) {
					return
				}
				O = new h(O, M, w);
				F.splice(M, 0, O);
				for (K = M + 1; K < F.length; K++) {
					F[K].index++
				}
			}
			var P = true;
			if (typeof O == "number" && O < F.length) {
				C = O;
				var J = F[O];
				if (J) {
					P = J._fireEvent(R, M, L)
				}
				if (!J || P !== false) {
					P = s._fireEvent(R, M, L, J)
				}
			}
			m(x[R], function () {
				P = this.call(w, O, M);
				if (this.cached) {
					x[R].splice(K, 1)
				}
				if (P === false) {
					return false
				}
				K++
			});
			return P
		};

		function B() {
			if ($f(q)) {
				$f(q).getParent().innerHTML = "";
				p = $f(q).getIndex();
				a[p] = w
			} else {
				a.push(w);
				p = a.length - 1
			}
			A = parseInt(q.style.height, 10) || q.clientHeight;
			E = q.id || "fp" + e();
			r = G.id || E + "_api";
			G.id = r;
			u = q.innerHTML;
			if (typeof t == "string") {
				t = {
					clip: {
						url: t
					}
				}
			}
			t.playerId = E;
			t.clip = t.clip || {};
			if (q.getAttribute("href", 2) && !t.clip.url) {
				t.clip.url = q.getAttribute("href", 2)
			}
			s = new h(t.clip, -1, w);
			t.playlist = t.playlist || [t.clip];
			var I = 0;
			m(t.playlist, function () {
				var L = this;
				if (typeof L == "object" && L.length) {
					L = {
						url: "" + L
					}
				}
				m(t.clip, function (M, N) {
					if (N !== undefined && L[M] === undefined && typeof N != "function") {
						L[M] = N
					}
				});
				t.playlist[I] = L;
				L = new h(L, I, w);
				F.push(L);
				I++
			});
			m(t, function (L, M) {
				if (typeof M == "function") {
					if (s[L]) {
						s[L](M)
					} else {
						j(x, L, M)
					}
					delete t[L]
				}
			});
			m(t.plugins, function (L, M) {
				if (M) {
					y[L] = new l(L, M, w)
				}
			});
			if (!t.plugins || t.plugins.controls === undefined) {
				y.controls = new l("controls", null, w)
			}
			y.canvas = new l("canvas", null, w);
			u = q.innerHTML;

			function K(L) {
				if (/iPad|iPhone|iPod/i.test(navigator.userAgent) && !/.flv$/i.test(F[0].url) && !J()) {
					return true
				}
				if (!w.isLoaded() && w._fireEvent("onBeforeClick") !== false) {
					w.load()
				}
				return f(L)
			}
			function J() {
				return w.hasiPadSupport && w.hasiPadSupport()
			}
			function H() {
				if (u.replace(/\s/g, "") !== "") {
					if (q.addEventListener) {
						q.addEventListener("click", K, false)
					} else {
						if (q.attachEvent) {
							q.attachEvent("onclick", K)
						}
					}
				} else {
					if (q.addEventListener && !J()) {
						q.addEventListener("click", f, false)
					}
					w.load()
				}
			}
			setTimeout(H, 0)
		}
		if (typeof q == "string") {
			var z = c(q);
			if (!z) {
				throw "Flowplayer cannot access element: " + q
			}
			q = z;
			B()
		} else {
			B()
		}
	}
	var a = [];

	function d(o) {
		this.length = o.length;
		this.each = function (q) {
			m(o, q)
		};
		this.size = function () {
			return o.length
		};
		var p = this;
		for (name in b.prototype) {
			p[name] = function () {
				var q = arguments;
				p.each(function () {
					this[name].apply(this, q)
				})
			}
		}
	}
	window.flowplayer = window.$f = function () {
		var p = null;
		var o = arguments[0];
		if (!arguments.length) {
			m(a, function () {
				if (this.isLoaded()) {
					p = this;
					return false
				}
			});
			return p || a[0]
		}
		if (arguments.length == 1) {
			if (typeof o == "number") {
				return a[o]
			} else {
				if (o == "*") {
					return new d(a)
				}
				m(a, function () {
					if (this.id() == o.id || this.id() == o || this.getParent() == o) {
						p = this;
						return false
					}
				});
				return p
			}
		}
		if (arguments.length > 1) {
			var t = arguments[1],
				q = (arguments.length == 3) ? arguments[2] : {};
			if (typeof t == "string") {
				t = {
					src: t
				}
			}
			t = i({
				bgcolor: "#000000",
				version: [9, 0],
				expressInstall: "http://static.flowplayer.org/swf/expressinstall.swf",
				cachebusting: false
			}, t);
			if (typeof o == "string") {
				if (o.indexOf(".") != -1) {
					var s = [];
					m(n(o), function () {
						s.push(new b(this, k(t), k(q)))
					});
					return new d(s)
				} else {
					var r = c(o);
					return new b(r !== null ? r : k(o), k(t), k(q))
				}
			} else {
				if (o) {
					return new b(o, k(t), k(q))
				}
			}
		}
		return null
	};
	i(window.$f, {
		fireEvent: function () {
			var o = [].slice.call(arguments);
			var q = $f(o[0]);
			return q ? q._fireEvent(o.slice(1)) : null
		},
		addPlugin: function (o, p) {
			b.prototype[o] = p;
			return $f
		},
		each: m,
		extend: i
	});
	if (typeof jQuery == "function") {
		jQuery.fn.flowplayer = function (q, p) {
			if (!arguments.length || typeof arguments[0] == "number") {
				var o = [];
				this.each(function () {
					var r = $f(this);
					if (r) {
						o.push(r)
					}
				});
				return arguments.length ? o[arguments[0]] : new d(o)
			}
			return this.each(function () {
				$f(this, k(q), p ? k(p) : {})
			})
		}
	}
})();
(function () {
	var h = document.all,
		j = "http://www.adobe.com/go/getflashplayer",
		c = typeof jQuery == "function",
		e = /(\d+)[^\d]+(\d+)[^\d]*(\d*)/,
		b = {
			width: "100%",
			height: "100%",
			id: "_" + ("" + Math.random()).slice(9),
			allowfullscreen: true,
			allowscriptaccess: "always",
			quality: "high",
			version: [3, 0],
			onFail: null,
			expressInstall: null,
			w3c: false,
			cachebusting: false
		};
	if (window.attachEvent) {
		window.attachEvent("onbeforeunload", function () {
			__flash_unloadHandler = function () {};
			__flash_savedUnloadHandler = function () {}
		})
	}
	function i(m, l) {
		if (l) {
			for (var f in l) {
				if (l.hasOwnProperty(f)) {
					m[f] = l[f]
				}
			}
		}
		return m
	}
	function a(f, n) {
		var m = [];
		for (var l in f) {
			if (f.hasOwnProperty(l)) {
				m[l] = n(f[l])
			}
		}
		return m
	}
	window.flashembed = function (f, m, l) {
		if (typeof f == "string") {
			f = document.getElementById(f.replace("#", ""))
		}
		if (!f) {
			return
		}
		if (typeof m == "string") {
			m = {
				src: m
			}
		}
		return new d(f, i(i({}, b), m), l)
	};
	var g = i(window.flashembed, {
		conf: b,
		getVersion: function () {
			var m, f;
			try {
				f = navigator.plugins["Shockwave Flash"].description.slice(16)
			} catch (o) {
				try {
					m = new ActiveXObject("ShockwaveFlash.ShockwaveFlash.7");
					f = m && m.GetVariable("$version")
				} catch (n) {
					try {
						m = new ActiveXObject("ShockwaveFlash.ShockwaveFlash.6");
						f = m && m.GetVariable("$version")
					} catch (l) {}
				}
			}
			f = e.exec(f);
			return f ? [f[1], f[3]] : [0, 0]
		},
		asString: function (l) {
			if (l === null || l === undefined) {
				return null
			}
			var f = typeof l;
			if (f == "object" && l.push) {
				f = "array"
			}
			switch (f) {
			case "string":
				l = l.replace(new RegExp('(["\\\\])', "g"), "\\$1");
				l = l.replace(/^\s?(\d+\.?\d*)%/, "$1pct");
				l = l.replace(/(%)/g, "%25").replace(/'/g, "\\u0027").replace(/"/g, "\\u0022").replace(/&/g, "%26");
				return '"' + l + '"';
			case "array":
				return "[" + a(l, function (o) {
					return g.asString(o)
				}).join(",") + "]";
			case "function":
				return '"function()"';
			case "object":
				var m = [];
				for (var n in l) {
					if (l.hasOwnProperty(n)) {
						m.push('"' + n + '":' + g.asString(l[n]))
					}
				}
				return "{" + m.join(",") + "}"
			}
			return String(l).replace(/\s/g, " ").replace(/\'/g, '"')
		},
		getHTML: function (o, l) {
			o = i({}, o);
			var n = '<object width="' + o.width + '" height="' + o.height + '" id="' + o.id + '" name="' + o.id + '"';
			if (o.cachebusting) {
				o.src += ((o.src.indexOf("?") != -1 ? "&" : "?") + Math.random())
			}
			if (o.w3c || !h) {
				n += ' data="' + o.src + '" type="application/x-shockwave-flash"'
			} else {
				n += ' classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000"'
			}
			n += ">";
			if (o.w3c || h) {
				n += '<param name="movie" value="' + o.src + '" />'
			}
			o.width = o.height = o.id = o.w3c = o.src = null;
			o.onFail = o.version = o.expressInstall = null;
			for (var m in o) {
				if (o[m]) {
					n += '<param name="' + m + '" value="' + o[m] + '" />'
				}
			}
			var p = "";
			if (l) {
				for (var f in l) {
					if (l[f]) {
						var q = l[f];
						p += f + "=" + (/function|object/.test(typeof q) ? g.asString(q) : q) + "&"
					}
				}
				p = p.slice(0, -1);
				n += '<param name="flashvars" value=\'' + p + "' />"
			}
			n += "</object>";
			return n
		},
		isSupported: function (f) {
			return k[0] > f[0] || k[0] == f[0] && k[1] >= f[1]
		}
	});
	var k = g.getVersion();

	function d(f, n, m) {
		if (g.isSupported(n.version)) {
			f.innerHTML = g.getHTML(n, m)
		} else {
			if (n.expressInstall && g.isSupported([6, 65])) {
				f.innerHTML = g.getHTML(i(n, {
					src: n.expressInstall
				}), {
					MMredirectURL: encodeURIComponent(location.href),
					MMplayerType: "PlugIn",
					MMdoctitle: document.title
				})
			} else {
				if (!f.innerHTML.replace(/\s/g, "")) {
					f.innerHTML = "<h2>Flash version " + n.version + " or greater is required</h2><h3>" + (k[0] > 0 ? "Your version is " + k : "You have no flash plugin installed") + "</h3>" + (f.tagName == "A" ? "<p>Click here to download latest version</p>" : "<p>Download latest version from <a href='" + j + "'>here</a></p>");
					if (f.tagName == "A") {
						f.onclick = function () {
							location.href = j
						}
					}
				}
				if (n.onFail) {
					var l = n.onFail.call(this);
					if (typeof l == "string") {
						f.innerHTML = l
					}
				}
			}
		}
		if (h) {
			window[n.id] = document.getElementById(n.id)
		}
		i(this, {
			getRoot: function () {
				return f
			},
			getOptions: function () {
				return n
			},
			getConf: function () {
				return m
			},
			getApi: function () {
				return f.firstChild
			}
		})
	}
	if (c) {
		jQuery.tools = jQuery.tools || {
			version: "3.2.8"
		};
		jQuery.tools.flashembed = {
			conf: b
		};
		jQuery.fn.flashembed = function (l, f) {
			return this.each(function () {
				$(this).data("flashembed", flashembed(this, l, f))
			})
		}
	}
})();
/*
	This code wraps PRE blocks in shrink / copy code commands
*/

var processCodeBlocks = new function () {

    // Private variables --------------------------------------------------------------------------

    var _minHeightToShrink = 400;
    var _shrinkCodeOnLoad  = false;

    // Private methods ----------------------------------------------------------------------------

    var prepareCopyText = function (input) {
        if (!input) return null;

        if (!$.support.opacity)
            return input;

        // Strip HTML
        var code = $("<div/>").html(input).text().trim();

        // Encode HTML tags
        code = code.replace(/\</gi, "&lt;");

        // Collapse multiple lines
        code = code.replace(/\r\n\r\n\r\n\r\n/, "\r\n\r\n");

        return code;
    }

    var convertCodeLanguage = function (code) {

        switch (code.toLowerCase()) {
            case "text":
                return "Plain Text";
                break;
            case "aspnet":
                return "ASP.NET";
                break;
            case "cs":
                return "C#";
                break;
            case "c++":
                return "C++";
                break;
            case "vb.net":
                return "VB.NET";
                break;
            case "mc++":
                return "C++ / CLI";
                break;
            case "css":
                return "CSS";
                break;
            case "delphi":
                return "Delphi";
                break;
            case "F#":
                return "F#";
                break;
            case "html":
                return "HTML";
                break;
            case "java":
                return "Java";
                break;
            case "jscript":
                return "Javascript";
                break;
            case "asm":
                return "MASM / ASM";
                break;
            case "msil":
                return "MSIL";
                break;
            case "midl":
                return "MIDL";
                break;
            case "php":
                return "PHP";
                break;
            case "sql":
                return "SQL";
                break;
            case "vbscript":
                return "VBScript";
                break;
            case "xml":
                return "XML";
                break;
        }

        return code;
    }

    var AddShrinkCommands = function ($preBlock, index) {

        // decoration="none" will turn off this shrink/collapse.
        if ($preBlock.attr("decoration") && $preBlock.attr("decoration").toLowerCase() === 'none')
            return;

        // Calculate only once
        var height = $preBlock.height();

        // Have we processed this block before?
        var currentId = $preBlock.attr("id");
        if (currentId && currentId.length > 3 && currentId.substring(0, 3) == "pre")
        {
            index = parseInt(currentId.substring(3));

            // If the shrink stuff was done wrong then strip it and do it again.
            if (index >= 0 && height > 0)
                $("#premain" + index.toString()).empty().remove();
            else
                return; // not enough info to setup the commands properly
        }
        else // if (typeof index == "number")
        {
            // ignore the provided index and use a random number. Reason: this script may have been
            // called against a set of PRE tags in an article, and then called again against forum
            // message, with a subsequent index reset. So: just make them all random and hope there's
            // no index collision
            index = Math.floor((Math.random() * 1000000) + 1);
            $preBlock.attr("id", "pre" + index.toString());
        }

        // Not enough info.
        if (typeof index != "number")
            return;

        $preBlock.css("margin-top", "0");

        var html = "<div class='pre-action-link' id='premain" + index.toString() + "' "
                 + " style='width:100%;display:block;'>";

        // html += " ID=premain" + index.toString() + " ";

        html += "<span id='prehide" + index.toString() + "' "
             + "   onclick='processCodeBlocks.togglePre(" + index.toString() + ");'>Hide</span> "
             + " &nbsp; ";

        // Invisible / display:none elements have height = 0
        if (height <= 0 || height > _minHeightToShrink) {

            var initialImage = '/images/arrow-up-16.png';
            var initialText  = 'Shrink';

            if (_shrinkCodeOnLoad && height > 0) {
                initialImage = '/images/arrow-down-16.png';
                initialText = 'Expand';
            }

            html += "<span id='preShrink" + index.toString() + "' "
                 + "   onclick='processCodeBlocks.shrinkExpand(" + index.toString() + ");'>"
                 +  initialText + "</span> "
                 + "<img id='preimg" + index.toString() + "' src='" + initialImage + "' "
                 + "   style='height:16px;width:16px' "
                 + "   onclick='processCodeBlocks.shrinkExpand(" + index.toString() + ");' /> "
                 + " &nbsp; ";

            if (_shrinkCodeOnLoad && height > 0) {
                $preBlock.css("overflow", "auto");
                $preBlock.height(_minHeightToShrink);
                $preBlock.attr('shrunk', 'true');
            }

            if (height > 0)
                !$preBlock.attr('processed', 'true');
        }

        html += "<span onclick='return processCodeBlocks.copyCode(" +index.toString() + ");'>"
             + "Copy Code</span>"

             + "</div>";

        $preBlock.before(html);

        if ($preBlock.height() > 0)
            $preBlock.collapsable = true;
    }

    // Public methods ------------------------------------------------------------------------------

    this.Initialise = function (parentSelector, shrinkHeight, shrinkCodeOnLoad) {

        _minHeightToShrink = shrinkHeight || _minHeightToShrink;
        _shrinkCodeOnLoad  = !!shrinkCodeOnLoad;

        $(parentSelector + " div.code-samples").each(function (sampleIndex) {
            var multiBlock = $(this);
            var preBlocks = multiBlock.children("pre");

            if (preBlocks.length > 1) {

                var divID = "divTab" + sampleIndex.toString();
                var csDivID = "codesampletab" + sampleIndex.toString();

                var tabSet = $("<div id='" + csDivID + "' class='clearfix'></div>").insertAfter(multiBlock);
                $("<ul id='codetab" + sampleIndex + "'></ul>").appendTo(tabSet);

                var divInfo = '';
                preBlocks.each(function () {

                    var innerThis = $(this);
                    var tabID = divID +innerThis.attr("lang").replace("#","sharp").replace("+","plus").replace(/[^a-zA-Z0-9]/, "");

                    $("<li><a href='#" + tabID + "'><span>" + convertCodeLanguage(innerThis.attr("lang")) +
				      "</span></a></li>").appendTo("#codetab" + sampleIndex);

                    divInfo += "<div id='" + tabID + "'><a name='" + tabID + "'></a><pre>" + innerThis.html() + "</pre></div>";
                });

                $(divInfo).insertAfter("#codetab" + sampleIndex);

                multiBlock.hide();
                tabSet.tabs();
            }
        });

        $(parentSelector + " pre").each(function (index) { AddShrinkCommands($(this), index); });
    }

    this.setupCollapsableCode = function (elm) {  
       $(elm).find("PRE").each(function (index) { AddShrinkCommands($(this)); });
    }

    // Toggle the visibility of the pre block
    this.togglePre = function (id) {
        var $preelm = $("#pre" + id);
        var $togelm = $("#prehide" + id);

        // $preelm.animate({ opacity: "toggle", display: "toggle" }, "fast");
        $preelm.toggle();

        if ($preelm.css("display") === 'none') {
            $togelm.text(" Show");
        } else {
            $togelm.text(" Hide");
        }
    }

    // Toggle the expand/shirnk status of the pre block
    this.shrinkExpand = function (id) {
        var $preelm = $("#pre" + id);
        var $imgelm = $("#preimg" + id);
        var $togelm = $("#preShrink" + id);

        // $preelm.animate({ opacity: "toggle", height: "toggle" }, "fast");

        if ($preelm.attr('shrunk') === 'true') {
            $imgelm.attr("src", "/images/arrow-up-16.png");
            $togelm.text(" Shrink");
            $preelm.attr('shrunk', 'false');
            // $preelm.animate({ height: "auto" }, "fast");
            $preelm.height('auto');
        } else {
            $imgelm.attr("src", "/images/arrow-down-16.png");
            $togelm.text(" Expand");
            $preelm.attr('shrunk', 'true');
            $preelm.animate({ height: Math.min(_minHeightToShrink,  $preelm.height()) }, "fast");
        }
    }

    this.copyCode = function (id) {

        var $pre = $("#pre" + id.toString());

        var text = $pre.html();
        if (!text) return false;

        text = prepareCopyText(text);
        if (!text) return false;

        // window.clipboardData.setData("Text", text);
        var copyWin = window.open("", "_blank", "width=500,height=300,scrollbars=yes");
        copyWin.document.writeln("<html><body><pre>" + text + "</pre></body></html>");

        return false;
    }
}();
var switchCallback     = null;
var minMessageScore    = 1;
var maxMessageScore    = 5;
var forumDir           = "/script/Forums/";
var staticServer       = "http://s.codeproject.com";
var allowReporting     = true;
var allowRating        = true;
var allowRatingDisplay = true;
var abuseScore         = -2;
var spamScore          = -1;
var getRatingUrl       = '/script/ratings/ajax/getratings.aspx';
var noiseThreshold     = 3;
var voteHiThreshold    = maxMessageScore - 0.5;
var getRatingRefKey    = 'obrs';

/* Acknowledgments: Josh Heyer / David Stone for initial versions. Bradml for stripdown and package */
var keyMapping = [];
keyMapping['13'] = "ENTER";
keyMapping['37'] = "LEFT";
keyMapping['38'] = "UP";
keyMapping['39'] = "RIGHT";
keyMapping['40'] = "DOWN";
keyMapping['16'] = "SHIFT";
keyMapping['17'] = "CTRL";
keyMapping['33'] = "PGUP";
keyMapping['34'] = "PGDOWN";

var commandMapping = {
	Combinations: {
		"CTRL+ENTER"	   : function() {Submit();},
		"CTRL+LEFT"		   : function() {MoveToPost(false, false);},
		"CTRL+RIGHT"	   : function() {MoveToPost(true, false);},
		"CTRL+UP"		   : function() {MoveToPost(false, true);},
		"CTRL+DOWN"		   : function() {MoveToPost(true, true);},
		"CTRL+PGUP"		   : function() {MoveToPage(true);},
		"CTRL+PGDOWN"	   : function() {MoveToPage(false);},
		"CTRL+SHIFT+LEFT"  : function() {MoveToPage(true);},
		"CTRL+SHIFT+RIGHT" : function() {MoveToPage(false);}
	}
};

document.onkeydown = function(e) {
	return shortcutEventHandler(e, commandMapping, keyMapping);
};

function getControlKeys(e) {
	var controls = "";
	if (e.ctrlKey)controls += "CTRL+";
	if (e.shiftKey)controls += "SHIFT+";
	if (e.altKey)controls += "ALT+";
	return controls;
}

// return: false == handled, true == keep looking
function shortcutEventHandler(KeyEvent, commandMapping, keyMapping) {
	if (!KeyEvent) KeyEvent = event;
	//Because IE knows better
	var eventKeyValue = (KeyEvent.keyCode) ? KeyEvent.keyCode: KeyEvent.charCode;
	var eventKeyName = "";
	if (keyMapping[eventKeyValue]) eventKeyName = keyMapping[eventKeyValue];
	var controlValue = getControlKeys(KeyEvent);
	eventKeyName = controlValue + eventKeyName;

	if (commandMapping.Combinations[eventKeyName]) 	{
		commandMapping.Combinations[eventKeyName]();
		
		// Stop propagation if we've found it
		if (KeyEvent.preventDefault) KeyEvent.preventDefault();
		else KeyEvent.returnValue = false;
		
		if (KeyEvent.stopPropagation) KeyEvent.stopPropagation();
		else KeyEvent.cancelBubble = true;

		return false;
	}

	return true;
}

function Submit() {
	var theForm = document.forms['aspnetForm'];
	if (!theForm) theForm = document.aspnetForm;
	if (theForm) theForm.submit();
}

function MoveToPage(pageUp) {
	var elm = document.getElementById(pageUp?'_mbpUrl':'_mbnUrl');
	if (elm) location.href=elm.value;
}

function MoveToPost(next, thread) {
	if (Selected=="" || isNaN(Selected)) return;
	var ForumTable = document.getElementById("ForumTable");
	if (!ForumTable) return;
	var elems = getElementsByClass("header",ForumTable,"tr");
	if (!elems) return;
	
	var moveTo = null;
	var selectedId = parseInt(Selected);
			
	var i,selectedIndex = -1;
	var selectedThreadIndex = 0;
	for (i=0; i<elems.length; ++i) 	{
		var id = elems[i].id.substring(1, elems[i].id.length-3);
		id = parseInt(id);
		
		if (IsThreadHead(elems[i]))
			selectedThreadIndex = i;
		
		if (id == selectedId) {
			selectedIndex = i;
			break;
		}
	}
	
	var nextId = null;
	for (i=selectedIndex + (next ? 1 : -1); i<elems.length && i >= 0; next ? ++i : --i)	{
	    if ((thread && IsThreadHead(elems[i])) || !thread) {

	        // Skip over sticky messages at the top since power users already know the stickies and
	        // new users won't know the keyboard shortcuts
            // HACK: DOM slection here is beyond terrible.
	        if (Selected < 0 && elems[i].childNodes[1].className.indexOf("sticky") >= 0)
	            continue;

			nextId = elems[i].id.substring(1, elems[i].id.length-3);
			nextId = parseInt(nextId);
			break;
		}
	}
	
	if (nextId && nextId != parseInt(Selected)) {
		SwitchMessage(null,nextId);
		// EnsureMessageVisible(Selected, true);
	}
}

function IsThreadHead(elem) {
	return elem.className.indexOf("root") >= 0;
}

function getElementsByClass(className, node, tag) {
	var found = new Array();
	if (node == null) node = document;
	if (tag == null) tag = '*';
	var elms = node.getElementsByTagName(tag);
	var length = elms.length;
	var pattern = new RegExp("(^|\\s)" + className + "(\\s|$)");
	for (var i = 0, j = 0; i < length; i++) {
		if (pattern.test(elms[i].className)) {
			found[j] = elms[i];
			j++;
		}
	}
	return found;
}

function getVoteFormHtml(MemberID, MsgID, votingType) {

	var script = "";

	if (allowRating) {
		
		if (votingType == 'GoodOrBad' || votingType == 'UpVoteOnly') {
			script += '<a href="#xx' + MsgID.toString() + 'xx" onclick="RateMsg(' + MemberID.toString()
			        + ',' + MsgID.toString() + ',5,0);"><img height="16px" width="16px" '
					+ 'src="/Images/upvote16.png" alt="upvote" title="upvote" border="0" '
					+ 'align="absmiddle" /></a>\n';

			if (votingType == 'GoodOrBad')
				script += '<br /><a href="#xx' + MsgID.toString() + 'xx" onclick="RateMsg(' + MemberID.toString() 
			            +  ',' + MsgID.toString() + ',1,0);"><img height="16px" width="16px" ' 
						+ 'src="/Images/downvote16.png" alt="downvote" title="downvote" border="0" '
						+ 'align="absmiddle" /></a>\n';
			else
				script += '<br /><img height="16px" width="16px" src="/Images/nodownvote16.png" '
				        + 'alt="downvoting not available" title="downvoting not available" '
						+ 'border="0" align="absmiddle" />\n';
		} 
		else if (votingType == 'RatingScale') {
			script += 'Rate:\n<a href="#xx' + MsgID.toString() + 'xx" onclick="RateMsg(' 
					+ MemberID.toString() + ',' + MsgID.toString() + ',1,0);"><img height="14px" '
					+ 'width="14px" src="' + staticServer + forumDir + 'Images/thumbs_down.gif" '
					+ 'alt="vote 1" border="0" align="absmiddle" /></a>\n';

			for (var i = 1; i <= 5; i++) {
				script += '<a class="toolbar nav-link" href="#xx' + MsgID.toString() + 'xx" '
					   +  'title="vote this message a ' + i.toString() 
					   +  '" onclick="RateMsg(' + MemberID.toString() + ',' + MsgID.toString() + ',' 
					   +  i.toString() + ',0);"><b>' + i.toString() + '</b></a>\n';
			}

			script += '<a href="#xx' + MsgID.toString() + 'xx" onclick="RateMsg(' + MemberID.toString() 
					+  ',' + MsgID.toString() + ',5,0);"><img height="14px" width="14px" src="' 
					+ staticServer + forumDir + 'Images/thumbs_up.gif" border="0" alt="vote 5" '
					+ 'align="absmiddle" /></a>\n';
		}
	}

	return script;
}

function getReportFormHtml(MemberID, MsgID) {

	var script = '';

	if (allowReporting) {

		script += '<span class="tooltip" style="padding:0 30px 0 25px">'
			   +	'<img height="16px" width="16px" src="' + staticServer
			   +		'/images/flag16.png" border="0" alt="report" style="vertical-align:text-top" />' // Report'
			   +    '<div class="tooltip-flyout" style="width:80px !important;line-height:17px">'
			   +        '<div class="list-item"><a href="#xx' + MsgID.toString() + 'xx" title="Report as Abuse" '
			   +			'onclick="return ReportMsg(' + MemberID.toString() + ',' + MsgID.toString() 
			   +			',' + abuseScore.toString() + ');">Abusive</a></div>'
			   +		'<div class="list-item"><a href="#xx' + MsgID.toString() + 'xx" title="Report as Spam" '
			   +			'onclick="return ReportMsg(' + MemberID.toString() + ',' + MsgID.toString() 
			   +			',' + spamScore.toString() + ');">Spam</a></div>'
			   +	'</div>'
			   +  '</span>';
	}

	return script;
}

function SetupVoteForm() {
	if (!allowRating) return;

	$("#ForumTable .voteform").each(function (index) {
		var elm = $(this);
		if (!elm.attr("done")) {
			var ownerId    = elm.attr("ownerid");
			var msgId      = elm.attr("msgid");
			var votingType = elm.attr("votingType");

			elm.html(getVoteFormHtml(ownerId, msgId, votingType));
			elm.attr("done", "true");
		}
	});
}

function SetupReportForm() {
	if (!allowReporting) return;

	$("#ForumTable span.reportform").each(function (index) {
		var elm = $(this);
		if (!elm.attr("done")) {
			var ownerId = elm.attr("ownerid");
			var msgId   = elm.attr("msgid");

			elm.html(getReportFormHtml(ownerId, msgId));
			elm.attr("done", "true");
		}
	});
}

function GetMessageVotes() {
	if (!allowRatingDisplay) return;

	var references = '';
	$("#ForumTable span.rating-label").each(function (index) {
		if (references) references += ',';
		// references += $(this).data('ref'); // Not working in Chrome
		references += $(this).attr('data-ref');
	});

	if (!references) return;

	$.ajax({
		type:	 'POST',
		// cache: true,
		url:	 getRatingUrl,
		data:	 { 'obrs' : references },
		timeout: 5000,
		success: function (msg) { SetMessageClassName(msg.data); },
		error:	 function (XMLHttpRequest, textStatus, errorThrown) { }
	});
}

function SetMessageClassName(data) {

	if (!data || !data.length) return;

	for (var i = 0; i < data.length; i++) {
		var ref       = data[i].reference;
		var rating    = data[i].rating;
		var score     = data[i].score;
		var votes     = data[i].votes;
		var upvotes   = data[i].upvotes;
		var className = null;

		var $elm = null, $content = null;
		if (score > 0) {
			$elm     = $('span[data-ref=' + ref + ']'); // span containing rating label

			$content = $elm.closest('.content')			// parent content block
			var votingType = $content.find('.voteform').attr('votingtype');
			var canVoteLow = votingType != 'None' && votingType != 'UpVoteOnly';

			if (score < noiseThreshold) {
				if (canVoteLow)
					className = "vote-lo";
			}
			else if (score >= voteHiThreshold)
				className = "vote-hi";

			if (!canVoteLow && votes)
				rating = (upvotes==1)? '1 upvote' : upvotes.toString() + ' upvotes';
		}

		if (className && $elm && $content) {
			$content.prev().find('.subject-line').addClass(className);
			$elm.prepend($('<span>' + rating + '</span>'));
		}
	}
}

function ReportMsg(userid, msgid, score) {
	if (confirm("Are you sure you want this message removed?"))
		return RateMsg(userid, msgid, score, 0);
	
	return false;
}

function RateMsg(memberid, msgid, score, questionId) {
	var req = window.XMLHttpRequest ? new XMLHttpRequest() : new ActiveXObject("MSXML2.XMLHTTP");
	if (!req) return;

	req.onreadystatechange = function () {
		if (req.readyState == 4) {
			if (req.status == 200) {
				var respText = req.responseText;
				var re = new RegExp('\<div>(.*)\</div\>', "g");
				var match = re.exec(respText);
				voteStatus.innerHTML = '<b>' + ((match && match.length > 1 && match[1]) ?
									   match[1] : 'An error occurred</b>');
			} else
				voteStatus.innerHTML = '<b style="color:red">Failed!' + req.statusText + '</b>';
		}
	}

	var voteStatus = document.getElementById("MVF" + msgid);
	if (!voteStatus) 
		return;

	voteStatus.innerHTML = '<b style="color:green">Voting...</b>';
	var strAction = forumDir + 'Vote.aspx?js=1&fmid=' + memberid.toString() + '&select=' +
					msgid.toString() + '&score=' + score.toString() + '&qid=' + questionId.toString();

	req.open("GET", strAction, true);
	req.send(null);

	return false;
}

// elm is the A tag wrapping the pin link. It's next to the A tag wrapping the subject line
function Pin(pinLink) {
	CloseMsg(Selected);

	Selected = ''

	// Get element that contains pin and subject line, then walk through looking for the A tag that's
	// a message-link (used for opening/closing messages).
	var subjectLine = pinLink.parentNode;	
	if (subjectLine) subjectLine = subjectLine.firstChild;
	while (subjectLine && subjectLine.className != 'message-link')
		subjectLine = subjectLine.nextSibling;

	var pinImage = pinLink ? pinLink.firstChild : null;
	if (subjectLine && !subjectLine.pinned) {
		OpenMsg(subjectLine.name, true);
		subjectLine.pinned = true;
		switchImage(pinImage, staticServer + forumDir + "images/pinned.png", 'Pinned. Click to unpin');
	} else {
		subjectLine.pinned = false;
		CloseMsg(subjectLine.name);
		switchImage(pinImage, staticServer + forumDir + "images/pin.png", 'Click to pin message');
	}

	return false;
}

function switchImage(image, newImgSrc, title) {
	image.src   = newImgSrc;
	image.title = title;
}

function CloseMsg(msgId) {
	if (!msgId || msgId == '' || msgId < 0) return;

	var lnk = document.getElementsByName(msgId);
	if (lnk && lnk.length > 0 && lnk[0].pinned) return;

	var hed = document.getElementById('F' + msgId.toString() + '_h0');
	if (hed)
		hed.className = hed.className.replace('selected', '');

	var bdy = document.getElementById('F' + msgId.toString() + '_h1');
	if (bdy)
		bdy.style.display = 'none';

	// if (oldTitle) document.title = oldTitle;
}

function OpenMsg(msgId, keep) {
	if (!msgId || msgId == '' || msgId < 0) return;

	var bdy = document.getElementById('F' + msgId.toString() + '_h1');
	if (bdy) {
		var lnk = document.getElementsByName(msgId);
		if (lnk && lnk.length > 0 && lnk[0].pinned)
			keep = true;

		/*
		if (lnk && lnk.length > 0) {
		oldTitle = document.title;
		document.title = lnk[0].innerHTML + ' - CodeProject';
		}
		*/

		if (keep && bdy.style.display != 'none') return;

		if (bdy.style.display == 'none')
			bdy.style.display = '';
		else
			bdy.style.display = 'none';
	}

	var hed = document.getElementById('F' + msgId.toString() + '_h0');
	if (hed)
		hed.className += ' selected';

	if (bdy && hed && bdy.style.display != 'none') {
		var webkitWeird = document.documentElement.scrollTop < document.body.scrollTop;
		if (!webkitWeird)
			document.body.scrollTop = getRealPos(hed, 'Top') - document.body.clientHeight / 10;

		EnsureMessageVisible(msgId, true);
	}

	if (processCodeBlocks)
	    processCodeBlocks.setupCollapsableCode(bdy);
}

function EnsureMessageVisible(msgID, bShowTop) {
	var webkitWeird = document.documentElement.scrollTop < document.body.scrollTop;
	var msgHeader   = document.getElementById('F' + msgID + '_h0');
	var msgBody     = document.getElementById('F' + msgID + '_h1');
	
	if (!msgBody || !msgHeader) return;

	var scrollContainer = document.documentElement;
	var top             = getRealPos(msgHeader, 'Top');
	var bottom          = getRealPos(msgBody, 'Top') + msgBody.offsetHeight;
	var scrollTop       = webkitWeird ? scrollTop = document.body.scrollTop : document.documentElement.scrollTop;

	if (scrollTop > top && !bShowTop)
		scrollTop = top - scrollContainer.clientHeight / 10;

	if (scrollTop + scrollContainer.clientHeight < bottom)
		scrollTop = bottom - scrollContainer.clientHeight;

	if (scrollTop > top && bShowTop)
		scrollTop = top - scrollContainer.clientHeight / 10;

	if (smoothScroll) {
		if (webkitWeird) $(document.body).stop().animate({ 'scrollTop': scrollTop }, 'fast');
		else $(scrollContainer).stop().animate({ 'scrollTop': scrollTop }, 'fast');
	} else {
		if (webkitWeird) document.body.scrollTop = scrollTop;
		else scrollContainer.scrollTop = scrollTop;
	}
}

function getRealPos(i, which) {
	iPos = 0;
	while (i != null) {
		iPos += i['offset' + which];
		i = i.offsetParent;
	}
	return iPos;
}

function SwitchMessage(e, msgId) {
	if (msgId) 
		msgId = msgId.toString();
	else {
		if (!e) e = window.event;
		var target = e.target ? e.target : e.srcElement;

		if (target && target.className == 'subject hover-container') {
			if (target.childNodes && target.childNodes.length > 2)
				target = target.childNodes[2];
		}
		else while (target && target.className != 'message-link')
			target = target.parentNode;

		if (!target || target.className != 'message-link') return;

		if (target.pinned) {
			/*Selected='';*/
			return false;
		}

		msgId = target.name;
	}

	CloseMsg(Selected)

	if (Selected == msgId) 
		Selected = '';
	else {
		OpenMsg(msgId, false); 
		Selected = msgId;
	}

	if (e) {
		if (e.preventDefault) e.preventDefault; 
		else e.returnValue = false;
	}

	if (switchCallback)
		switchCallback(null, msgId)

	return false;
}

function CheckAdminClick(Msg) {
	return window.confirm("Are you sure you want to " + Msg + "?");
}

function SelfCheck() {
	return window.confirm("You are about to reply to your own message. Is this really what you want?");
}

function FireNew(elm) {
	window.location = $(elm).find('a')[0].href;
}

$(document).ready(function () {
	SetupVoteForm();
	SetupReportForm();
	GetMessageVotes();

	processCodeBlocks.Initialise('#ForumTable', 400, true); // Expand/collapse code blocks
});

/*
	Description: Draws connectors between child and parent messages in the forums. HTML5 only.
	Date       : 1 Jul 2011
	Author     : Chris Maunder
	
*/
var forumNav = function () {

	var _iconSize             = { x: 16, y: 16 }
	var _padding              = { x: 4,  y: 2  }
	var _currentSource        = null
	var _currentTarget        = null
	var _interval             = 0
	var _connectorStage       = 1
	var _maxStages            = 5
	var _delay                = 50
	var _colour               = "200,200,200"
	var _startOpacity         = 0.2
	var _attemptedToAddCanvas = false
	var _overElement          = false
	var _parentOffPage        = false
	var _isChrome             = (navigator.userAgent.indexOf("Chrome/") >= 0)

	function getCanvas() {
		if (!_attemptedToAddCanvas)
			$(document.body).append('<canvas id="cv1" width="1px" height="1px" style="position:absolute;left:0;top:0;pointer-events:none"></canvas>' +
			                        '<canvas id="cv2" width="1px" height="1px" style="position:absolute;left:0;top:0;pointer-events:none"></canvas>');
		_attemptedToAddCanvas = true;

		if (!this.canvasVert) this.canvasVert = $("#cv1")[0];
		if (!this.canvasHorz) this.canvasHorz = $("#cv2")[0];

		return this.canvasVert && this.canvasHorz;
	}

	function getContext() {
		if (getCanvas()) {
			if (!this.contextVert) this.contextVert = this.canvasVert.getContext("2d"); // Vertical line
			if (!this.contextHorz) this.contextHorz = this.canvasHorz.getContext("2d"); // Horizontal line
		}
		return this.contextVert && this.contextHorz;
	}

	function locateForum() {
		if (!this.forum) this.forum = $('#ForumTable .fixed-layout')[0];
		return this.forum;
	}

	function clearDisplay() {

		if (!getContext()) return;

		contextVert.clearRect(0, 0, canvasVert.width, canvasVert.height);
		contextHorz.clearRect(0, 0, canvasHorz.width, canvasHorz.height);
	}

	function onMouseOver(event) {
		_overElement = true;
		showConnector(event, this.name);
	}

	function onMouseOut(event) {
		_overElement = false;
		if (_connectorStage >= _maxStages)
			drawConnectorInternal(null, null);
	}

	function showConnector(event, sourceId) {

		if (!getContext() || !locateForum()) return;
		clearDisplay();

		if (sourceId) {
			var newSource = $('a[name="' + sourceId + '"]')[0];
			if (_currentSource && _currentSource == newSource) return;
			_currentSource = newSource;
		} else {
			if (_currentSource && _currentSource == this) return;
			_currentSource = this;
		}
		_currentTarget = null;

		var parentID = _currentSource.attributes["parent"].value;
		if (!parentID || parentID == "0") return;

		_currentTarget = $('a[name="' + parentID + '"]')[0];
		_parentOffPage = !_currentTarget;

		// Get the first message in the thread
		if (_parentOffPage)
			_currentTarget = $('a[thread="' + _currentSource.attributes["thread"].value + '"]:first')[0];

		var source = $(_currentSource).offset();
		var target = _currentTarget
						? $(_currentTarget).offset()
						: { left: source.left - _padding.x - _iconSize.x,
							top:  $(forum).offset().top - _padding.y - _iconSize.y
						};
		if (!source || !target) return;

		if (_parentOffPage) {
			target.left = source.left - _iconSize.x - 2*_padding.x; // line up with source
			target.top  -= (_iconSize.y + _padding.y);              // line up with earliest msg in thread
		}

		var offset = $(self.canvasVert).offset();

		canvasVert.width      = Math.min(_iconSize.x, source.left - target.left);
		canvasVert.height     = source.top   - target.top  - _iconSize.y/2;
		canvasVert.style.top  = (target.top  + _iconSize.y + _padding.y) + "px";
		canvasVert.style.left = (target.left - _iconSize.x - _padding.x) + "px";

		canvasHorz.width      = Math.max(source.left  - target.left - _iconSize.x / 2 - _padding.x, 1);
		canvasHorz.height     = _iconSize.y;
		canvasHorz.style.top  = (source.top  + _padding.y) + "px";
		canvasHorz.style.left = (target.left - _iconSize.x - _padding.x + canvasVert.width/2) + "px";

		// Correction for Firefox
		if ($.browser.mozilla) {
			canvasVert.style.top = (target.top + _iconSize.y + 3*_padding.y) + "px";
			canvasHorz.style.top = (source.top + 3*_padding.y) + "px";
		}

		if (_isChrome) {
			_connectorStage = _maxStages;
			self._interval  = null;
			drawConnectorInternal();
		} else {
			_connectorStage = 1;
			clearInterval(self._interval);
			self._interval = setInterval("forumNav.drawConnector()", _delay);
		}
	}

	function drawConnectorInternal() {

		clearDisplay();

		if (!_currentSource) return;

		var opacity = _startOpacity + ((1.0 - _startOpacity) * _connectorStage / _maxStages);

		self.contextVert.lineWidth   = self.contextHorz.lineWidth = 1;
		self.contextVert.strokeStyle = self.contextHorz.strokeStyle = "rgba(" + _colour + "," + opacity + ")";

		self.contextVert.beginPath();
		self.contextVert.moveTo(canvasVert.width / 2, 0);
		self.contextVert.lineTo(canvasVert.width / 2, canvasVert.height);
		self.contextVert.stroke();

		self.contextHorz.beginPath();
		self.contextHorz.moveTo(0, canvasHorz.height / 2);
		self.contextHorz.lineTo(canvasHorz.width, canvasHorz.height / 2);
		self.contextHorz.stroke();

		if (_parentOffPage) {
			self.contextVert.beginPath();
			self.contextVert.moveTo(_iconSize.x     / 4, _iconSize.y / 4);
			self.contextVert.lineTo(_iconSize.x     / 2, 0);
			self.contextVert.lineTo(_iconSize.x * 3 / 4, _iconSize.y / 4);
			self.contextVert.stroke();
		}

		_connectorStage++;
		if (_connectorStage > _maxStages) {
			if (!_overElement)
				clearDisplay();
			if (self._interval)
				clearInterval(self._interval);

			_currentSource = null;
		}
	}

	// Public methods
	return {

		drawConnector: function () {
			drawConnectorInternal();
		},

		supportsCanvas: function () {
			getCanvas();
			return canvasVert && canvasVert.getContext;
		},

		initialise: function () {
			var self = this;

			$('.message-link').bind('mouseover', onMouseOver);
			$('.message-link').bind('mouseout',  onMouseOut);
			switchCallback = showConnector;
		},

		resize: function () {
			this.initialise();
		}
	};
} ();

$(document).ready(function () {
	if (forumNav.supportsCanvas()) {
		forumNav.initialise();

		$(window).bind('resize', function () {
			forumNav.resize();
		});
	}
});
/* Provides the ability to flag messages. 

Setup:

// Construct a new flagging object and set working values.
var forumMsgFlag;
$(document).ready(function () {
	forumMsgFlag = new ForumMsgFlag('forumMsgFlag');

	forumMsgFlag.updateMessageUrl = "http://localhost/script/Forums/Ajax/UpdateMessageFlag.aspx";
	forumMsgFlag.flagTypes = new Array();
	forumMsgFlag.flagTypes[0] = { name: 'Clear flag', cssClass:  '' };				// ALWAYS INCLUDE!!
	forumMsgFlag.flagTypes[1] = { name: 'Flag Name 1', cssClass: 'flag-class1' };
	forumMsgFlag.flagTypes[2] = { name: 'Flag Name 2', cssClass: 'flag-class2' };
	forumMsgFlag.flagTypes[3] = { name: 'Flag Name 3', cssClass: 'flag-class3' };
	forumMsgFlag.flagTypes[4] = { name: 'Flag Name 4', cssClass: 'flag-class4' };

	forumMsgFlag.initialise();
});

*/

function ForumMsgFlag(varName) {

	// Public variables ----------------------------------------------------------------------------

	this.updateMessageUrl = ''; 		 // The URL to call when updating the message
	this.flagTypes        = new Array(); // The flag types that can be applied

	// Private variables ---------------------------------------------------------------------------

	var self       = this;
	var flagMenuId = 'FrmMsgFlagMenu';   // ID of the flyout menu we'll create
	var myName     = varName;			 // Name of "this"'s variable

	// Private methods -----------------------------------------------------------------------------

	// Gets a full A tag for the a menu action link
	var getFlagLink = function(messageID, flagID) {
		return "<a href='javascript:void()' onclick='return " + myName + ".onClick(" +
		       messageID + "," + flagID + ");'>" + self.flagTypes[flagID].name + "</a>";
	}

	// Sets up a flyout menu with the flag options. Recreated each time - may want to create once
	// and store if dynamic creation not needed.
	var setupMenu = function(flagElm) {
		var elm = $(flagElm).parent().parent().parent().parent().find(".message-link"); 
		if (elm.length < 1) return;
		var id = elm[0].name;

		$("#" + flagMenuId + " div").remove();

		var html = '<div>';
		for (var i in self.flagTypes)
			html += '<div class="list-item">' + getFlagLink(id, i) + '</div>';
		html += '</div>';

		$("#" + flagMenuId).append(html);
	}

	// Displays the flyout nenu
	var showFlagMenu = function() {
		setupMenu(this);

		var over = false;

		var elm = $(this), menu = $("#" + flagMenuId);
		if (!elm || !menu) return;

		var pos     = $(elm).offset();
		var eWidth  = $(elm).outerWidth();
		var mWidth  = $(menu).outerWidth();
		var eHeight = $(elm).outerHeight();
		var left    = pos.left + "px";
		var top     = pos.top  + "px";

		$(menu).css({ left: left, top: top });
		$(menu).stop(true, true).show();
		$(elm).hover(function() {
					 setTimeout(function() { if (!over) $("#" + flagMenuId).fadeOut('fast'); }, 200);
				    });
		$(menu).hover(function() { over = true; }, 
		              function() { over = false; $(menu).fadeOut('fast'); });
	}

	// Public methods ------------------------------------------------------------------------------

	// Update the message based on the chosen flag/action
	this.onClick = function (messageID, flagID) {
		$.get(self.updateMessageUrl + '?msgflagId=' + flagID + '&msg=' + messageID, 
			  function(data) {
				var flagRow = $('#ForumTable').find('.message-link[name="' + messageID + '"]')
				              .parent().parent();
				for (var i in self.flagTypes)
					flagRow.removeClass(self.flagTypes[i].cssClass);

				if (self.flagTypes[flagID].cssClass)
					flagRow.addClass(self.flagTypes[flagID].cssClass);
			  });
		$("#" + flagMenuId).fadeOut('fast');
		return false;
	}

	// Setup everything.
	this.initialise = function() {
		$('body').append('<div id="' + flagMenuId + '" class="invisible tooltip-flyout" ' +
		                 'style="position:absolute"><div>Error: Unable to load flag types</div></div>');
		$('.flag').bind('click', showFlagMenu);
	}
}
function ValidateLength(input, name, limit, required) {
    if (!input && required)
        return name + " is a required field.<br/>"
    if (!(input.length > 5 && input.length < limit)) {
        return name + " must be between 5 and " + limit.toString() + " characters.<br/>";
    }
    return '';
}

function ValidateUrl(input, name, required) {
    var pattern = /((([A-Za-z]{3,9}:(?:\/\/)?)(?:[-;:&=\+\$,\w]+@)?[A-Za-z0-9.-]+|(?:www.|[-;:&=\+\$,\w]+@)[A-Za-z0-9.-]+)((?:\/[\+~%\/.\w-_]*)?\??(?:[-\+=&;%@.\w_]*)#?(?:[\w]*))?)/;
    if (!input && required)
        return name + " is a required field.<br/>"
    if (!input.match(pattern))
        return "Invalid Url format for " + name + ".<br/>";
    return '';
}

function ValidateDelimiter(input, delimiter, name, required) {
    var error = '';
    if (!input) {
        if (required)
            error = name + " is a required field.<br/>";
    }
    else {
        input = input.trim();
        if (input.charAt(0) == delimiter ||
        input.charAt(input.length - 1) == delimiter ||
        input.indexOf(delimiter + delimiter) > -1)
            error = "No leading, trailing or double " + delimiter + "'s are allowed in the " + name + " field.<br/>";
        else if (input.length > 100)
            error = name + " are limited to 100 characters.<br/>";
    }
    
    return error;
}

function ValidateDate(input, name, required) {
    var pattern = /^[0-9]{4}-[01][0-9]-[0-3][0-9]$/;
    if (!input && required)
        return name + " is a required field.<br/>"
    if (!input.match(pattern))
        return "Invalid date format for " + name + ".<br/>";
    return '';
}

function ValidateRequired(input, name) {
    if (!input || input == '')
        return name + " is a required field.<br/>";
    return '';
}

function ValidateNumber(input, name, required) {
    if (!input && required)
        return name + " is a required field.<br/>"
    if (isNaN(input))
        return name + " is not a number.<br/>";
    return '';
}
/*
 Ridiculously Responsive Social Sharing Buttons
 Team: @dbox, @seagoat
 Site: http://www.kurtnoble.com/labs/rrssb
 Twitter: @therealkni

        ___           ___
       /__/|         /__/\        ___
      |  |:|         \  \:\      /  /\
      |  |:|          \  \:\    /  /:/
    __|  |:|      _____\__\:\  /__/::\
   /__/\_|:|____ /__/::::::::\ \__\/\:\__
   \  \:\/:::::/ \  \:\~~\~~\/    \  \:\/\
    \  \::/~~~~   \  \:\  ~~~      \__\::/
     \  \:\        \  \:\          /__/:/
      \  \:\        \  \:\         \__\/
       \__\/         \__\/
*/


; (function (window, jQuery, undefined) {
    'use strict';


    /*
	 * Utility functions
	 */
    var setPercentBtns = function () {
        // loop through each instance of buttons
        jQuery('.rrssb-buttons').each(function (index) {
            var self = jQuery(this);
            var numOfButtons = jQuery('li', self).length;
            var initBtnWidth = 100 / numOfButtons;

            // set initial width of buttons
            jQuery('li', self).css('width', initBtnWidth + '%').attr('data-initwidth', initBtnWidth);
        });
    };

    var makeExtremityBtns = function () {
        // loop through each instance of buttons
        jQuery('.rrssb-buttons').each(function (index) {
            var self = jQuery(this);
            //get button width
            var containerWidth = parseFloat(jQuery(self).width());
            var buttonWidth = jQuery('li', self).not('.small').first().width();
            var smallBtnCount = jQuery('li.small', self).length;

            // enlarge buttons if they get wide enough
            if (buttonWidth > 170 && smallBtnCount < 1) {
                jQuery(self).addClass('large-format');
            } else {
                jQuery(self).removeClass('large-format');
            }

            if (containerWidth < 200) {
                jQuery(self).removeClass('small-format').addClass('tiny-format');
            } else {
                jQuery(self).removeClass('tiny-format');
            }
        });
    };

    var backUpFromSmall = function () {
        // loop through each instance of buttons
        jQuery('.rrssb-buttons').each(function (index) {
            var self = jQuery(this);
            var totalBtnSze = 0, totalTxtSze = 0, upCandidate, nextBackUp;
            var smallBtnCount = jQuery('li.small', self).length;

            if (smallBtnCount === jQuery('li', self).length) {
                var btnCalc = smallBtnCount * 42;
                var containerWidth = parseFloat(jQuery(self).width());
                upCandidate = jQuery('li.small', self).first();
                nextBackUp = parseFloat(jQuery(upCandidate).attr('data-size')) + 55;

                if ((btnCalc + nextBackUp) < containerWidth) {
                    jQuery(self).removeClass('small-format');
                    jQuery('li.small', self).first().removeClass('small');

                    sizeSmallBtns();
                }

            } else {
                jQuery('li', self).not('.small').each(function (index) {
                    var txtWidth = parseFloat(jQuery(this).attr('data-size')) + 55;
                    var btnWidth = parseFloat(jQuery(this).width());

                    totalBtnSze = totalBtnSze + btnWidth;
                    totalTxtSze = totalTxtSze + txtWidth;
                });

                var spaceLeft = totalBtnSze - totalTxtSze;
                upCandidate = jQuery('li.small', self).first();
                nextBackUp = parseFloat(jQuery(upCandidate).attr('data-size')) + 55;

                if (nextBackUp < spaceLeft) {
                    jQuery(upCandidate).removeClass('small');
                    sizeSmallBtns();
                }
            }
        });
    };

    var checkSize = function (init) {
        // loop through each instance of buttons
        jQuery('.rrssb-buttons').each(function (index) {
            //console.log('starting check for: '+(index+1));
            var self = jQuery(this);
            var elems = jQuery('li', self).nextAll(), count = elems.length;

            // get buttons in reverse order and loop through each
            jQuery(jQuery('li', self).get().reverse()).each(function (index, count) {

                if (jQuery(this).hasClass('small') === false) {
                    var txtWidth = parseFloat(jQuery(this).attr('data-size')) + 55;
                    var btnWidth = parseFloat(jQuery(this).width());

                    if (txtWidth > btnWidth) {
                        //console.log($(self).attr('class')+' '+$(this).attr('class')+' txtWidth: '+txtWidth+ ' & btnWidth: '+btnWidth);
                        var btn2small = jQuery('li', self).not('.small').last();
                        jQuery(btn2small).addClass('small');
                        //console.log($(btn2small).attr('class'));
                        sizeSmallBtns();
                    }
                }

                if (!--count) backUpFromSmall();
            });
        });

        // if first time running, put it through the magic layout
        if (init === true) {
            rrssbMagicLayout(sizeSmallBtns);
        }
    };

    var sizeSmallBtns = function () {
        // loop through each instance of buttons
        jQuery('.rrssb-buttons').each(function (index) {
            var self = jQuery(this);
            var regButtonCount,
					regPercent,
					pixelsOff,
					magicWidth,
					smallBtnFraction;

            // readjust buttons for small display
            var smallBtnCount = jQuery('li.small', self).length;

            // make sure there are small buttons
            if (smallBtnCount > 0 && smallBtnCount !== jQuery('li', self).length) {
                jQuery(self).removeClass('small-format');

                //make sure small buttons are square when not all small
                jQuery('li.small', self).css('width', '42px');
                pixelsOff = smallBtnCount * 42;
                regButtonCount = jQuery('li', self).not('.small').length;
                regPercent = 100 / regButtonCount;
                smallBtnFraction = pixelsOff / regButtonCount;

                if (navigator.userAgent.indexOf('Chrome') >= 0 || navigator.userAgent.indexOf('Safari') >= 0) {
                    magicWidth = '-webkit-calc(' + regPercent + '% - ' + smallBtnFraction + 'px)';
                } else if (navigator.userAgent.indexOf('Firefox') >= 0) {
                    magicWidth = '-moz-calc(' + regPercent + '% - ' + smallBtnFraction + 'px)';
                } else {
                    magicWidth = 'calc(' + regPercent + '% - ' + smallBtnFraction + 'px)';
                }
                jQuery('li', self).not('.small').css('width', magicWidth);

            } else if (smallBtnCount === jQuery('li', self).length) {
                // if all buttons are small, change back to percentage
                jQuery(self).addClass('small-format');
                setPercentBtns();
            } else {
                jQuery(self).removeClass('small-format');
                setPercentBtns();
            }
        }); //end loop

        makeExtremityBtns();
    };

    var rrssbInit = function () {
        jQuery('.rrssb-buttons').each(function (index) {
            jQuery(this).addClass('rrssb-' + (index + 1));
        });

        setPercentBtns();

        // grab initial text width of each button and add as data attr
        jQuery('.rrssb-buttons li .text').each(function (index) {
            var txtWdth = parseFloat(jQuery(this).width());
            jQuery(this).closest('li').attr('data-size', txtWdth);
        });

        checkSize(true);
    };

    var rrssbMagicLayout = function (callback) {
        //remove small buttons before each conversion try
        jQuery('.rrssb-buttons li.small').removeClass('small');

        checkSize();

        callback();
    };

    var popupCenter = function (url, title, w, h) {
        // Fixes dual-screen position                         Most browsers      Firefox
        var dualScreenLeft = window.screenLeft !== undefined ? window.screenLeft : screen.left;
        var dualScreenTop = window.screenTop !== undefined ? window.screenTop : screen.top;

        var width = window.innerWidth ? window.innerWidth : document.documentElement.clientWidth ? document.documentElement.clientWidth : screen.width;
        var height = window.innerHeight ? window.innerHeight : document.documentElement.clientHeight ? document.documentElement.clientHeight : screen.height;

        var left = ((width / 2) - (w / 2)) + dualScreenLeft;
        var top = ((height / 3) - (h / 3)) + dualScreenTop;

        var newWindow = window.open(url, title, 'scrollbars=yes, width=' + w + ', height=' + h + ', top=' + top + ', left=' + left);

        // Puts focus on the newWindow
        if (window.focus) {
            newWindow.focus();
        }
    };

    var waitForFinalEvent = (function () {
        var timers = {};
        return function (callback, ms, uniqueId) {
            if (!uniqueId) {
                uniqueId = "Don't call this twice without a uniqueId";
            }
            if (timers[uniqueId]) {
                clearTimeout(timers[uniqueId]);
            }
            timers[uniqueId] = setTimeout(callback, ms);
        };
    })();

    /*
	 * Event listners
	 */

    jQuery('.rrssb-buttons a.popup').live('click', function (e) {
        var _this = jQuery(this);
        popupCenter(_this.attr('href'), _this.find('.text').html(), 580, 470);
        e.preventDefault();
    });

    // resize function
    jQuery(window).resize(function () {

        rrssbMagicLayout(sizeSmallBtns);

        waitForFinalEvent(function () {
            rrssbMagicLayout(sizeSmallBtns);
        }, 200, "finished resizing");
    });

    // init load
    jQuery(document).ready(function () {
        rrssbInit();
    });


})(window, jQuery);

