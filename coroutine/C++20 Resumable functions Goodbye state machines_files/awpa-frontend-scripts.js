(function (e) {
    "use strict";
    var n = window.AFTHRAMPES_JS || {};


        n.DataBackground = function () {
            var pageSection = e(".awpa-data-bg");
            pageSection.each(function (indx) {
                if (e(this).attr("data-background")) {
                    e(this).css("background-image", "url(" + e(this).data("background") + ")");
                }
            });

            e('.awpa-bg-image').each(function () {
                var src = e(this).children('img').attr('src');
                e(this).css('background-image', 'url(' + src + ')').children('img').hide();
            });
        },


        e(document).ready(function () {
            n.DataBackground();
        })
})(jQuery);