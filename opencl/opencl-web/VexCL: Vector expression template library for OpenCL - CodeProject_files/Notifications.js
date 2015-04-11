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