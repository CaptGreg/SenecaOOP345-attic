// This script causes a box with profile information to show when the user hovers over a
// member's link.
// Dependencies on jQuery and the Data Service /App_Code/MemberSummaryController

function setPopupHeight(frame) {
    var $popup = $("#MemberProfilePopupDiv");
    var divHeight = $popup.height();
    var frameHeight = frame.contentWindow.document.body.scrollHeight;

    frame.height = frameHeight;

    $popup.css({ 'top': $popup.offset().top + (divHeight - frameHeight) });
}

(function ($) {
    // REVIEWED: [Chris]  yeah, I know "Don't hardcode ..." but we need a way of
    // passing in arguments to the scripts, and storing these arguments in the database.
    // Probably, we will pass in an options object, but we will need a way of documenting the
    // members of the object, much the same ways its done for jQuery API.
    // REVIEWED: [Matthew] Agree, and I was thinking simple placeholders would do it
    // eg: var memberProfileViewUrl = "%ProfilePageUrl%";

    var useFlair             = true;
    var useFriendlyUrl       = false;
    var memberFlairiFrameUrl = "/members/Flair/";
    var memberProfileViewUrl = "/script/Membership/View.aspx";
    var turnoffUrl           = "/script/Membership/Ajax/SetPreference.aspx";

    $.fn.MemberProfilePopup = function (options) {
        var settings = {
            memberSummariesUrl:    "/API/Membership/Summary/",
            missingMemberImageUrl: "/script/Membership/Images/member_unknown.gif"
           };

        if (useFriendlyUrl)
            settings.memberSummariesUrl = "/API/Member/Summary/";

        // If options exist, lets merge them
        // with our default settings
        if (options)
            $.extend(settings, options);

        $("<div id=\"MemberProfilePopupDiv\" class='raised box'></div>")
            .css({
                'display': 'none',
                'position': 'absolute'
            })
            .appendTo("body");

        var $popup = $("#MemberProfilePopupDiv");

        var hoverConfig = {
            sensitivity: 7,
            interval   : 250,
            timeout    : 250,         // number = milliseconds delay before onMouseOut    
            over       : function (e) // show the Member Profile
            {
                $target         = $(this);
                var url         = $target.attr("href");
                var memberIdStr = GetQueryStringParameter("mid", url);

                if (useFlair) {
                    $popup.empty();
                    
                    $("<iFrame src='" + memberFlairiFrameUrl + memberIdStr + "?Display=Embedded' " +
                      "frameborder='0' width='300' scrolling='no' height='150' " +
                      "onload='setPopupHeight(this)'></iFrame>").appendTo("#MemberProfilePopupDiv");

                    var divHeight = $popup.height();
                    $popup.css({
                        'left': Math.min($target.offset().left + 20, window.innerWidth - $popup.width() - 25),
                        'top': $target.offset().top - (divHeight + 16)
                    })
                    .show();
                }
                else {
                    var requestUrl = settings.memberSummariesUrl; // +memberIdStr;
                    requestUrl = requestUrl + memberIdStr;
                    var sendData = "{ 'memberId' : " + memberIdStr + " }";

                    // if membersummary url is in friendlyUrl form, then use following for request url
                    // var requestUrl = settings.memberSummariesUrl + memberIdStr;
                    $.ajax(
                    {
                        url        : requestUrl,
                        data       : sendData, // if friendlyUrl is used, no data needs to be sent, but it will not hurt
                        type       : 'POST',
                        dataType   : 'json',
                        contentType: 'application/json; charset=utf-8',
                        success    : function (data) {
                            // if friendly url format is used, then 
                            // var profile = data;

                            var profile = data;

                            if (!profile.ImageUrl || profile.ImageUrl.length == 0)
                                profile.ImageUrl = settings.missingMemberImageUrl;

                            $popup.empty();
                            $("<table class='member-popup'><tr valign='top'>" +
                                "<td><img /></td>" +
                                "<td><div class='name'>" + profile.Name + "</div>" +
                                "<div class='title'>" + profile.Title +
                                ((profile.Title && profile.Company) ? " at " : "") +
                                profile.Company + "</div>" +
                                "<div class='location'>" + profile.Country + "</div>" +
                                "<div class='rep'>" + formatNumber(profile.RepPoints) + " points</div>" +
                                "</td></tr></table>").appendTo("#MemberProfilePopupDiv");

                            $("#MemberProfilePopupDiv img").error(function () {
                                $this = $(this);
                                $this.unbind("error");
                                $this.attr("src", settings.missingMemberImageUrl);

                                var divHeight = $popup.height();
                                $popup.css({
                                    'left': $target.offset().left + 20,
                                    'top': $target.offset().top - (divHeight + 16)
                                })
                            }).attr("src", profile.ImageUrl);


                            var divHeight = $popup.height();
                            $popup.css({
                                'left': $target.offset().left + 20,
                                'top': $target.offset().top - (divHeight + 16)
                            })
                            .show();
                        }
                    });
                }
            },
            out: function () // hide the Member Profile
            {
                $popup.hide();
            }
        }

        if (this.hoverIntent)
            this.hoverIntent(hoverConfig);
        else
            this.hover(hoverConfig.over, hoverConfig.out);

        function formatNumber(number) {
            var comma = ',',
            string = Math.max(0, number).toFixed(0),
            length = string.length,
            end = /^\d{4,}$/.test(string) ? length % 3 : 0;
            return (end ? string.slice(0, end) + comma : '') + string.slice(end).replace(/(\d{3})(?=\d)/g, '$1' + comma);
        }

        function GetQueryStringParameter(key, url) {
            if (key.charAt(key.length - 1) != '=')
                key = key + '=';

            var startPos = url.toLowerCase().indexOf(key);
            if ((startPos < 0) || (startPos + key.length == url.length))
                return "";
            var endPos = url.indexOf("&", startPos);
            if (endPos < 0)
                endPos = url.length;

            var queryString = url.substring(startPos + key.length, endPos);
            // fix the space characters
            queryString = queryString.replace(/%20/gi, " ");
            queryString = queryString.replace(/\+/gi, " ");
            // remove the quotes (if you're really creative, you could search for the
            // terms within the quotes as phrases, and everything else as single terms)
            queryString = queryString.replace(/%22/gi, "");
            queryString = queryString.replace(/\"/gi, "");

            return queryString;
        }


        return this;
    };

    $(function () {
        $(".qa-info a[href*='" + memberProfileViewUrl + "']").MemberProfilePopup();
        $(".qa-list a[href*='" + memberProfileViewUrl + "']").MemberProfilePopup();
        $(".author a[href*='" + memberProfileViewUrl + "']").MemberProfilePopup();
    });
})(jQuery);