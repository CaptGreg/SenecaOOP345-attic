
if (typeof wm_indiv_stats == 'undefined') {
	wm_indiv_stats = new Object();  // object to add the individual parameters
}

(function(){

	/**
	 * Creates the wiredminds literal object and returns it.
	 *
	 * @return	object
	 */
	function getWiredMinds() {
		var wiredminds = {

			getlist : "",

			www_server : "www.count.carrierzone.com",
			app_server : "www.count.carrierzone.com",
			cnt_server : "count.carrierzone.com",
			app_server_proto : "",
			cnt_server_proto : "",
			numberOfCalls : 0,
			wm_track_url : "",
			wm_track_alt : "",
			wm_cvid : 0,
			placeOverlay : false,
			placeHeatmap : false,
			placeOverlayMenu : false,
			overlayKeyLength : 32,
			wm_page_name_value : null,
			ovk : null,
			overlayScriptUrl : '',
			validateUrl      : '',
			still_alive_interval : 5000,  // msecs!
			still_alive_time_max : 30*60*1000,  // msecs!
			still_alive_time_remaining : 0,
			still_alive_request : false,
			use_image_container : true,
			wm_use_ic : 0,
			cntParamContainer: {},

			getOverlayScriptUrl : function (){
				var proto = wiredminds.getProtoApp();
				return proto+'//' + wiredminds.app_server + '/app/frontend/Ajax/overlay/OverlayManager.php?objMethod=loadScript';
			},

			getValidateUrl : function (){
				var proto = wiredminds.getProtoApp();
				return proto+'//' + wiredminds.app_server + '/app/frontend/Ajax/overlay/OverlayManager.php?objMethod=validateExternalKey';
			},

			createRandomKey : function () {
				var keyLenght = 16;
				var now = new Date();
				var outputStr = '';
				var inputNum = now.getTime();
				inputNum = inputNum.toString();
				var newInputNum = 0;
				var conStr = '';
				var partition = 3;

				while (outputStr.length < keyLenght) {
					// Add random value
					if (inputNum.length==0) {
						do {
							newInputNum = Math.round(Math.random()*123);
						} while (newInputNum<48 || newInputNum>57 && newInputNum<65 || newInputNum>80 && newInputNum<97 || newInputNum>122);
						inputNum = newInputNum.toString();
					}
					// Separate numbers out of string
					partition = 3;
					do {
						conStr = parseInt(inputNum.substr(0, partition));
						partition--;
					} while ((conStr<48 || conStr>57 && conStr<65 || conStr>80 && conStr<97 || conStr>122) && partition>0);
					inputNum = inputNum.substr(partition+1);
					// Convert and add new character
					if (conStr<48 || conStr>57 && conStr<65 || conStr>80 && conStr<97 || conStr>122) {
						outputStr += conStr.toString();
					} else {
						outputStr += String.fromCharCode(conStr);
					}
				}
				if (outputStr.length > keyLenght) {
					outputStr = outputStr.substr(0, keyLenght);
				}
				return outputStr;
			},

			get_GET_value:function ( get_param_name ) {
				if (!window) {return '';}
				if (!window.location) {return '';}
				if (!window.location.search) {return '';}
				var search=window.location.search.substr(1);
				var assignments=search.split('&');
				for( var i=0; i<assignments.length; i++ ) {
					var assignment_splitted=assignments[i].split('=');
					var param_name=assignment_splitted[0];
					var param_value=assignment_splitted[1];
					if (param_name == get_param_name) {
						return decodeURIComponent(param_value);
					}
				}
				return '';
			},

			get_HASH_value:function (hashName){

				var hashValue = null;
				if (self.document.location){
					hashValue = unescape(self.document.location.hash.substring(1));
				}
				if ((hashValue != null) && (hashValue != '')){
					var parts = hashValue.split('=');
					if (parts.length == 2){
						if ((typeof(hashName) != 'undefined') && (parts[0] == hashName)){
							hashValue = parts[1];
						}
					}
				}
				return hashValue;
			},

			add_to_list : function( key, value) {
				wiredminds.getlist += '&' + key + '=' + encodeURIComponent(value);
			},

			//Get protocol, returns protocol string
			getProto : function () {
				var proto = 'http:';
				if ( typeof (document.location) != 'undefined') {
					if ( typeof (document.location.protocol) != 'undefined') {
						if ( document.location.protocol != 'file:') {
							proto = document.location.protocol;
						}
					}
				}
				return proto;
			},

			// get protocol for app server
			getProtoApp : function () {
				if (wiredminds.app_server_proto != ""){
					return wiredminds.app_server_proto + ':';
				}else{
					return this.getProto();
				}
			},

			// get protocol for couning server
			getProtoCnt : function () {
				if (wiredminds.cnt_server_proto != ""){
					return wiredminds.cnt_server_proto + ':';
				}else{
					return this.getProto();
				}
			},

			// set protocol for app server
			setProtoApp : function (p){
				if (wiredminds.checkProto(p) == true){
					wiredminds.app_server_proto = p;
				}
			},

			// get protocol for couning server
			setProtoCnt : function (p){
				if (wiredminds.checkProto(p) == true){
					wiredminds.cnt_server_proto = p;
				}
			},

			// check protocol for legal values
			checkProto : function (p){
				var legalProtocols = new Array('http', 'https');
				for (var i in legalProtocols){
					if (p == legalProtocols[i]){
						return true;
					}
				}
				return false;
			},

			count_URL : function (getlist) {
				var proto = wiredminds.getProtoCnt();
				var WMCT_URL=proto+'//'+this.cnt_server+'/track/ctin.php?'+getlist;

				//Check if it is the first time the pixel is called (numberOfCalls==0)
				//if it is the first then write pixel, this is required to show the webcounter on the website
				if ( (wiredminds.numberOfCalls > 0) || wiredminds.use_image_container ) {
					var CT_container=new Image();
					CT_container.src=WMCT_URL;
				} else {
					//NOTE: Important to first insert the node before assigning innerHTML, because IE(6) would count twice otherwise!
					var ndDiv=document.createElement('div');
					var bodyEl = document.getElementsByTagName('body')[0];
					ndDiv = bodyEl.insertBefore(ndDiv, bodyEl.firstChild);
					ndDiv.innerHTML='<a target="_blank" href="' + wiredminds.wm_track_url + '"><img src="' + WMCT_URL + '" alt="' + wiredminds.wm_track_alt + '" border="0" /></a>';
				}
				wiredminds.numberOfCalls++;
			},

			countpix : function (pixcode) {
				wiredminds.setTrackParam('wm_pixelcode', pixcode);
				wiredminds.count();
			},

			count_flash : function (flashArgs) {
				if ( typeof(flashArgs) != 'string') {
					return;
				}
				if ( flashArgs == '') {
					return;
				}

				if ( flashArgs.substr(0,1) == '&') {
					flashArgs='t2=1'+flashArgs;
				}

				wiredminds.count_URL(flashArgs);
			},

			getPageName : function (ext){

				// auto-detect page name, if necessary
				if (wiredminds.getTrackParam('wm_page_name') === null) {
					//Decode to make sure it does not get double encoded
					var wm_page_name_value = decodeURIComponent(document.location.pathname);
					// replace '/' with '|'
					wm_page_name_value = wm_page_name_value.replace(/^\/*/, '');
					wm_page_name_value = wm_page_name_value.replace(/\/*$/, '');
					wm_page_name_value = wm_page_name_value.replace(/\//g, '|');
				} else {
					var wm_page_name_value = wiredminds.getTrackParam('wm_page_name');
				}
				if (typeof(ext) == 'string') {
					wm_page_name_value+='|'+ext;
				}

				return wm_page_name_value;
			},

			internalCount : function (ext) {
				//If wm_use_ic is greater than 0, it was set and ic should be stored
				if ((wiredminds.getInternParam("wm_use_ic") > 0)) {
					//So first check if the IC snippet was included
					try {
						if ((typeof ic_options !== 'undefined')) {
							if ((typeof ic !== 'undefined') && (typeof ic.pagetrackId !== 'undefined')) {
								//IC snippet is loaded and got a visitor id, so store it to defined indiv param
								var icParamVal = ic.clientInfo.company.name+'#user:'+ic.gUsers[ic.activeUserId].login;
								wiredminds.setIndivParam('wm_visit_'+wiredminds.getInternParam("wm_use_ic"), icParamVal);
							} else {
								//Wait a while for IC snippet
								if (isNaN(ic_options.wired_minds_count)) {
									ic_options.wired_minds_count = 0;
								}
								ic_options.wired_minds_count++;
								// We give IC 1000ms to load
								if (ic_options.wired_minds_count <= 5) {
									setTimeout(function() { wiredminds.internalCount(ext); }, 200);
									return;
								}
							}
						}
					} catch (e) {
						//Ignore and go on
					}
				}
				
				var myDate = new Date();
				wiredminds.getlist="t="+myDate.getTime(); // Clear/Initialize the get list.
				// NOTE: Clearing the getlist is a must!
				//       Otherwise subsequent calls to count
				//       (e.g. by using onClick="") would
				//       append their values to the old get list.

				// First the required parameters
				if ( wiredminds.getTrackParam('wm_custnum') !== null) {
					wiredminds.add_to_list('custnum',wiredminds.getTrackParam('wm_custnum'));
				} else {
					return; // no customer number given, do nothing
				}


				// auto-detect sname, if necessary
				if ( wiredminds.getTrackParam('wm_sname') === null ) {
					wiredminds.setTrackParam('wm_sname', document.location.hostname);
				}
				wiredminds.add_to_list('sname',wiredminds.getTrackParam('wm_sname'));

				wiredminds.add_to_list('pagename',this.getPageName(ext));

				// auto-detect group name, if necessary
				// only auto-detect if no group AND page name was explicitly set,
				// if a page name was set, the group name will be derrived from it in the ctin.php
				if ((wiredminds.getTrackParam('wm_group_name') === null) && (wiredminds.getTrackParam('wm_page_name') === null)) {
					//Decode to make sure it does not get double encoded
					wiredminds.setTrackParam('wm_group_name', decodeURIComponent(document.location.pathname.substring(0,document.location.pathname.lastIndexOf('/'))));
				}
				if (wiredminds.getTrackParam('wm_group_name') !== null) {
					wiredminds.add_to_list('group', wiredminds.getTrackParam('wm_group_name'));
				}

				if ( this.still_alive_request == true) {
					wiredminds.add_to_list('still_alive',1);
				}

				// Then the optional parameters
				// Note: The optional parameters first get collected and added
				//       afterwards to the getlist to better control their order.
				if (wiredminds.getTrackParam('wm_version') === null) {
					wiredminds.setTrackParam('wm_version', '$Rev: 7840 $');
				}
				if (wiredminds.getTrackParam('wm_resolution') === null) {
					wiredminds.setTrackParam('wm_resolution', screen.width + "x" + screen.height);
				}
				if (wiredminds.getTrackParam('wm_color_depth') === null) {
					var wm_color_depth = navigator.appName != 'Netscape' ? screen.colorDepth : screen.pixelDepth;
					wiredminds.setTrackParam('wm_color_depth', wm_color_depth);
				}

				if (wiredminds.getTrackParam('wm_plugin_list') === null) {
					var wm_plugin_list = "";
					for( var i=0; i<navigator.plugins.length; i++ ) {
						wm_plugin_list += navigator.plugins[i].name + ';';
					}
					wiredminds.setTrackParam('wm_plugin_list', wm_plugin_list);
				}

				if (wiredminds.getTrackParam('wm_jv') === null) {
					var wm_jv = 0;
					if (navigator.javaEnabled()) {
						wm_jv=1;
					}
					wiredminds.setTrackParam('wm_jv', wm_jv);
				}

				if (wiredminds.getTrackParam('wm_js') === null) {
					wiredminds.setTrackParam('wm_js', 1);
				}

				// auto-detect referrer, if necessary
				if ( wiredminds.getTrackParam('wm_ref') === null ) {
					if ( wiredminds.getTrackParam('wm_referrer') === null ) {
						try {
							if( typeof( top.document ) == "object" ) {
								var wm_referrer_value = top.document.referrer;
							}
						} catch (e) {
							var wm_referrer_value = document.referrer;
						}
					} else {
						var wm_referrer_value = wiredminds.getTrackParam('wm_referrer');
					}
				} else {
					var wm_referrer_value = wiredminds.getTrackParam('wm_ref');
				}

				if (wiredminds.getTrackParam('wm_campaign') !== null) {
					var wm_camp_value = wiredminds.getTrackParam('wm_campaign');
				} else if (wiredminds.getTrackParam('wm_campaign_key') !== null) {
					var wm_camp_value = wiredminds.get_GET_value(wiredminds.getTrackParam('wm_campaign_key'));
				}

				//Call Center
				if (wiredminds.getTrackParam('wm_useccapplet') !== null) {
					if (wiredminds.getTrackParam('wm_useccapplet') == 1) {
						var wm_cvid = wiredminds.createRandomKey();
						wiredminds.setTrackParam('wm_cvid', wm_cvid);
						wiredminds.setInternParam('wm_cvid', wm_cvid);
					}
				}
				//END Call Center

				if (wiredminds.getTrackParam('wm_version') !== null)     {wiredminds.add_to_list('version',wiredminds.getTrackParam('wm_version'));}
				if (wiredminds.getTrackParam('wm_js') !== null)          {wiredminds.add_to_list('js',wiredminds.getTrackParam('wm_js'));}
				if (wiredminds.getTrackParam('wm_jv') !== null)          {wiredminds.add_to_list('jv',wiredminds.getTrackParam('wm_jv'));}
				if (wiredminds.getTrackParam('wm_resolution') !== null)  {wiredminds.add_to_list('resolution',wiredminds.getTrackParam('wm_resolution'));}
				if (wiredminds.getTrackParam('wm_color_depth') !== null) {wiredminds.add_to_list('color_depth',wiredminds.getTrackParam('wm_color_depth'));}
				if (wiredminds.getInternParam('numberOfCalls') == 0) {
					//Only request webcounter if it is the first call (performance issues)
					if (wiredminds.getTrackParam('wm_wc') !== null)      {wiredminds.add_to_list('wc',wiredminds.getTrackParam('wm_wc'));}
				}
				if ( typeof (wm_camp_value) != 'undefined')              {wiredminds.add_to_list('campaign',wm_camp_value);}
				if (wiredminds.getTrackParam('wm_milestone') !== null)   {wiredminds.add_to_list('milestone',wiredminds.getTrackParam('wm_milestone'));}
				if (wiredminds.getTrackParam('wm_sales') !== null)       {wiredminds.add_to_list('sales',wiredminds.getTrackParam('wm_sales'));}
				if (wiredminds.getTrackParam('wm_order_id') !== null)    {wiredminds.add_to_list('order_id',wiredminds.getTrackParam('wm_order_id'));}
				if (wiredminds.getTrackParam('wm_basket') !== null)      {wiredminds.add_to_list('basket',wiredminds.getTrackParam('wm_basket'));}
				if (wiredminds.getTrackParam('wm_connspeed') !== null)   {wiredminds.add_to_list('speed',wiredminds.getTrackParam('wm_connspeed'));}
				if (wiredminds.getTrackParam('wm_txid') !== null)        {wiredminds.add_to_list('txid',wiredminds.getTrackParam('wm_txid'));}
				if (wiredminds.getTrackParam('wm_pixelcode') !== null)   {wiredminds.add_to_list('pixel',wiredminds.getTrackParam('wm_pixelcode'));}
				if ( typeof (wm_referrer_value) != 'undefined')          {wiredminds.add_to_list('referrer',wm_referrer_value);}
				if (wiredminds.getTrackParam('wm_ivw') !== null)         {wiredminds.add_to_list('ivw',wiredminds.getTrackParam('wm_ivw'));}
				if (wiredminds.getTrackParam('wm_ab_data') !== null)     {wiredminds.add_to_list('ab_data',wiredminds.getTrackParam('wm_ab_data'));}
				if (wiredminds.getTrackParam('wm_ud1') !== null)         {wiredminds.add_to_list('ud1',wiredminds.getTrackParam('wm_ud1'));}
				if (wiredminds.getTrackParam('wm_ud2') !== null)         {wiredminds.add_to_list('ud2',wiredminds.getTrackParam('wm_ud2'));}
				if (wiredminds.getTrackParam('wm_cd1') !== null)         {wiredminds.add_to_list('cd1',wiredminds.getTrackParam('wm_cd1'));}
				if (wiredminds.getTrackParam('wm_cvid') !== null)        {wiredminds.add_to_list('cvid',wiredminds.getTrackParam('wm_cvid'));}
				if (wiredminds.getTrackParam('wm_score') !== null)       {wiredminds.add_to_list('score',wiredminds.getTrackParam('wm_score'));}
				if (wiredminds.getTrackParam('wm_no_pi') !== null)       {wiredminds.add_to_list('wm_no_pi',wiredminds.getTrackParam('wm_no_pi'));}
				if (wiredminds.getTrackParam('wm_flver') !== null)       {wiredminds.add_to_list('wm_flver',wiredminds.getTrackParam('wm_flver'));}
					// ecommerce tracking
				if (wiredminds.getTrackParam('wm_ec_action') !== null)           {wiredminds.add_to_list('wm_ec_action',wiredminds.getTrackParam('wm_ec_action'));}
				if (wiredminds.getTrackParam('wm_ec_basket_id') !== null)        {wiredminds.add_to_list('wm_ec_basket_id',wiredminds.getTrackParam('wm_ec_basket_id'));}
				if (wiredminds.getTrackParam('wm_ec_product_id') !== null)       {wiredminds.add_to_list('wm_ec_product_id',wiredminds.getTrackParam('wm_ec_product_id'));}
				if (wiredminds.getTrackParam('wm_ec_product_name') !== null)     {wiredminds.add_to_list('wm_ec_product_name',wiredminds.getTrackParam('wm_ec_product_name'));}
				if (wiredminds.getTrackParam('wm_ec_category_name') !== null)    {wiredminds.add_to_list('wm_ec_category_name',wiredminds.getTrackParam('wm_ec_category_name'));}
				if (wiredminds.getTrackParam('wm_ec_order_id') !== null)         {wiredminds.add_to_list('wm_ec_order_id',wiredminds.getTrackParam('wm_ec_order_id'));}
				if (wiredminds.getTrackParam('wm_ec_customer_id') !== null)      {wiredminds.add_to_list('wm_ec_customer_id',wiredminds.getTrackParam('wm_ec_customer_id'));}
				if (wiredminds.getTrackParam('wm_ec_currency') !== null)         {wiredminds.add_to_list('wm_ec_currency',wiredminds.getTrackParam('wm_ec_currency'));}
				if (wiredminds.getTrackParam('wm_ec_unit_price') !== null)       {wiredminds.add_to_list('wm_ec_unit_price',wiredminds.getTrackParam('wm_ec_unit_price'));}
				if (wiredminds.getTrackParam('wm_ec_unit_price_in_currency') !== null)  {wiredminds.add_to_list('wm_ec_unit_price_in_currency',wiredminds.getTrackParam('wm_ec_unit_price_in_currency'));}
				if (wiredminds.getTrackParam('wm_ec_total_price') !== null)      {wiredminds.add_to_list('wm_ec_total_price',wiredminds.getTrackParam('wm_ec_total_price'));}
				if (wiredminds.getTrackParam('wm_ec_total_price_in_currency') !== null) {wiredminds.add_to_list('wm_ec_total_price_in_currency',wiredminds.getTrackParam('wm_ec_total_price_in_currency'));}
				if (wiredminds.getTrackParam('wm_ec_quantity') !== null)         {wiredminds.add_to_list('wm_ec_quantity',wiredminds.getTrackParam('wm_ec_quantity'));}
				if (wiredminds.getTrackParam('wm_ec_basket_special') !== null)   {wiredminds.add_to_list('wm_ec_basket_special',wiredminds.getTrackParam('wm_ec_basket_special'));}
				if (wiredminds.getTrackParam('wm_ec_product_special') !== null)  {wiredminds.add_to_list('wm_ec_product_special',wiredminds.getTrackParam('wm_ec_product_special'));}
				if (wiredminds.getTrackParam('wm_ec_payment_method') !== null)   {wiredminds.add_to_list('wm_ec_payment_method',wiredminds.getTrackParam('wm_ec_payment_method'));}

				// check for individual parameters
				var wm_indiv_stats = wiredminds.getTrackParam('wm_indiv_stats');
				for (var key in wm_indiv_stats) {
					if (wm_indiv_stats[key]){
						wiredminds.add_to_list(key, wm_indiv_stats[key]);
					}
				}

				// Page Url - put it at the (near) end, because it may overflow the GET list.
				if (wiredminds.getTrackParam('wm_page_url') !== null) {
					wiredminds.add_to_list('page_url', encodeURIComponent(wiredminds.getTrackParam('wm_page_url')));
				} else {
					wiredminds.add_to_list('page_url', encodeURIComponent(document.location));
				}

				// The plugin list always goes last, because it can become very long and is not that required.
				if (wiredminds.getTrackParam('wm_plugin_list') !== null) {wiredminds.add_to_list('plugins',wiredminds.getTrackParam('wm_plugin_list'));}

				// end individual parameters
				wiredminds.count_URL(wiredminds.getlist);
			},

			ReadCookie : function (name) {
				var nameEQ = name + "=";
				var ca = document.cookie.split(';');
				for(var i=0;i < ca.length;i++) {
					var c = ca[i];
					while (c.charAt(0)==' ') {
						c = c.substring(1,c.length);
					}
					if (c.indexOf(nameEQ) == 0) {
						return c.substring(nameEQ.length,c.length);
					}
				}
				return null;
			},

			SetCookie : function (name,value,nHours, path) {
				var today = new Date();
				var expire = new Date();
				var pathStr = "/";
				if (nHours==null || nHours==0) {
					nHours=1;
				}
				if ((typeof(path) != 'undefined') && (path != null)) {
					pathStr = path;
				}
				expire.setTime(today.getTime() + 3600000 * nHours);
				document.cookie = name+"="+escape(value) + ";expires="+expire.toGMTString() + ";path=" + pathStr;

			},

			eraseCookie : function(name) {
				this.SetCookie(name,"",-1);
			},

			loadExternalScript : function (id, url){
				var oScript  = document.createElement('script');
				oScript.id = id;
				oScript.type = "text/javascript";
				oScript.src  = url;
				document.getElementsByTagName("head")[0].appendChild(oScript);
			},

			setOverlayCookies : function(){
				wiredminds.eraseCookie('WMOverlayMenu');
				wiredminds.eraseCookie('WMOverlayKey');
				wiredminds.SetCookie('WMOverlayMenu', '1', 24);
				wiredminds.SetCookie('WMOverlayKey', wiredminds.ovk, 24);

				if (this.checkOverlayCookies()){
					this.startOverlay();
				}
			},

			checkOverlayCookies : function(){
				wiredminds.placeOverlay = (wiredminds.ReadCookie('WMOverlay') == '1') ? true : false;
				wiredminds.placeHeatmap = (wiredminds.ReadCookie('WMHeatmap') == '1') ? true : false;
				wiredminds.placeOverlayMenu = (wiredminds.ReadCookie('WMOverlayMenu') == '1') ? true : false;

				if (wiredminds.placeOverlayMenu || wiredminds.placeOverlay || wiredminds.placeHeatmap) {
					return true;
				}
				return false;
			},

			// load the extra javascripts for the overlay/heatmap
			startOverlay : function(){
				if (this.checkOverlayCookies()){
					this.overlayScriptUrl = wiredminds.getOverlayScriptUrl();
					this.overlayScriptUrl += '&key=' +  wiredminds.ovk;
					setTimeout("wiredminds.loadExternalScript('wmOverlay','" + this.overlayScriptUrl + "')", 1000);
				}
			},

			doCountWiredminds : function( ext ){
				if (wiredminds.getTrackParam('wm_track_url') !== null) {
					wiredminds.setInternParam('wm_track_url', wiredminds.getTrackParam('wm_track_url'));
				} else {
					var proto = wiredminds.getProto();
					wiredminds.setInternParam('wm_track_url', proto+'//'+this.www_server);
				}

				if (wiredminds.getTrackParam('wm_track_alt') !== null) {
					wiredminds.setInternParam('wm_track_alt', wiredminds.getTrackParam('wm_track_alt'));
				} else {
					wiredminds.setInternParam('wm_track_alt', '');
				}
				wiredminds.internalCount( ext );
			},

			// for Wiredminds
			count : function ( ext ) {

				var getKey = wiredminds.get_HASH_value('WMOverlayKey');

				if (getKey.length == wiredminds.overlayKeyLength){
					wiredminds.ovk = getKey;
				}else{
					wiredminds.ovk = wiredminds.ReadCookie('WMOverlayKey');
				}
				if ((wiredminds.ovk) && (wiredminds.ovk.length == wiredminds.overlayKeyLength)){
					// - if the key was passed and it has a length of 'overlayKeyLength' chars, it might be valid.
					// - check the key in the backend
					// - if the key is valid. it will automatically call the functions to load the extra javascript,
					//   and show the overlay menu.
					wiredminds.validateUrl = wiredminds.getValidateUrl();
					wiredminds.validateUrl += '&key=' + wiredminds.ovk;
					this.loadExternalScript('wmOverlayKeyValidation', wiredminds.validateUrl);
				} else {
					wiredminds.eraseCookie('WMOverlayKey');
					this.doCountWiredminds( ext );
				}
			},

			// for backwards compatibility
			counter : function ( ext ) {
				wiredminds.count(ext);
			},

			getVisitorLink : function () {
				var destinationDomain = '//'+this.app_server;
				var wm_custnum = wiredminds.getTrackParam('wm_custnum');
				var relPath = '/app/ct/index.php?mode=last_visitors_detail&websitesel='+wm_custnum+'&cvid='+wiredminds.wm_cvid;
				var proto = wiredminds.getProto();
				var url = proto + destinationDomain + relPath;
				return url;
			},

			setVisitorLink : function (flashObjectId) {
				var visitor_link = wiredminds.getVisitorLink();
				eval("document."+flashObjectId+".SetVariable('wm_visitor_link', visitor_link)");
			},
			
			setICForIndivVisitStat : function (indivStatId) {
				wiredminds.setInternParam('wm_use_ic', indivStatId);
			},

			sendStillAlive : function() {
				if(this.placeOverlayMenu == false){
					this.still_alive_request=true;
					wiredminds.internalCount();
					this.still_alive_request=false; // Immediatly remove it, so onClicks are counted correctly, for example!

					this.still_alive_time_remaining -= this.still_alive_interval;
					if (this.still_alive_time_remaining > 0) {
						setTimeout("wiredminds.sendStillAlive()", this.still_alive_interval);
					}
				}
			},

			activateStillAlive : function() {
				if(this.placeOverlayMenu == false){
					this.still_alive_time_remaining=this.still_alive_time_max;
					setTimeout("wiredminds.sendStillAlive()", this.still_alive_interval);
				}
			},

			/**
			 * Event Tracking
			 * @param string eventStr
			 */
			trackEvent: function(eventStr){
				if(this.placeOverlayMenu == false){
					if ((typeof(eventStr) == 'undefined') && (eventStr != '')){
						return; // no event string given, do nothing
					}

					var myDate = new Date();
					wiredminds.getlist = "t="+myDate.getTime(); // Clear/Initialize the get list.
					// NOTE: Clearing the getlist is a must!
					//       Otherwise subsequent calls to count
					//       (e.g. by using onClick="") would
					//       append their values to the old get list.
					// First the required parameters
					if (wiredminds.getTrackParam('wm_custnum') !== null) {
						wiredminds.add_to_list('custnum',wiredminds.getTrackParam('wm_custnum'));
					} else {
						return; // no customer number given, do nothing
					}

					wiredminds.add_to_list('wm_event', eventStr);
					wiredminds.count_URL(wiredminds.getlist);
				}
			},
			/**
			 * Sets param into the cntParamContainer dataset used for tracking.
			 *
			 * @param	string paramName	The paramName to set
			 * @param	mixed  paramValue	The paramValue to set
			 * @return	void
			 */
			setTrackParam: function(paramName, paramValue) {
				wiredminds['cntParamContainer'][paramName] = paramValue;
			},
			/**
			 * Gets param from the cntParamContainer dataset used for tracking.
			 *
			 * @param	string paramName	The paramName to get
			 * @return	mixed
			 */
			getTrackParam: function(paramName) {
				//First check if the param is set internally
				if (typeof wiredminds['cntParamContainer'][paramName] != 'undefined') {
					return wiredminds['cntParamContainer'][paramName];
				}
				//To be backwards compatible, also check for global variable
				if (typeof window[paramName] != 'undefined') {
					return window[paramName];
				}

				return null;
			},
			/**
			 * Sets internal params directly in the wiredminds object.
			 *
			 * @param	string paramName	The paramName to set
			 * @param	mixed  paramValue	The paramValue to set
			 * @return	void
			 */
			setInternParam: function(paramName, paramValue) {
				wiredminds[paramName] = paramValue;
			},
			/**
			 * Gets internal params directly from the wiredminds object.
			 *
			 * @param	string paramName	The paramName to get
			 * @return	mixed
			 */
			getInternParam: function(paramName) {
				if (typeof wiredminds[paramName] != 'undefined') {
					return wiredminds[paramName];
				}
				return null;
			},
			/**
			 * Sets the given param for the indiv stats.
			 *
			 * @param	string paramName	The paramName to set
			 * @param	mixed  paramValue	The paramValue to set
			 * @return	void
			 */
			setIndivParam: function(paramName, paramValue) {
				//Create object if it does not yet exist
				if (typeof wiredminds['cntParamContainer']['wm_indiv_stats'] != 'object') {
					wiredminds['cntParamContainer']['wm_indiv_stats'] = new Object(); // object to add the individual parameters
				}
				wiredminds['cntParamContainer']['wm_indiv_stats'][paramName] = paramValue;
			},
			/**
			 * Gets the given param from the indiv stats.
			 *
			 * @param	string paramName	The paramName to get
			 * @return	mixed
			 */
			getIndivParam: function(paramName) {
				//First check if the param is set internally
				if ((typeof wiredminds['cntParamContainer']['wm_indiv_stats'] == 'object')
					&& (typeof wiredminds['cntParamContainer']['wm_indiv_stats'][paramName] != 'undefined')) {
					return wiredminds['cntParamContainer']['wm_indiv_stats'][paramName];
				}
				//To be backwards compatible, also check for global variable
				if ((typeof window['wm_indiv_stats'] != 'undefined') && (typeof window['wm_indiv_stats'][paramName] != 'undefined')) {
					return window['wm_indiv_stats'][paramName];
				}

				return null;
			},
			/**
			 * Mimics Array.push function, but instead of adding the given arguments to an array,
			 * it checks if a function for value at pos 0 exists and if so, it calls it and
			 * applies the other elements as arguments to it.
			 * NOTE: Expects an array to be given, other data types will be ignored.
			 * Example: push(['functionName', param1, param2])
			 *
			 * @return integer
			 */
			push: function() {
				var called = 0;
				var args = arguments;
				if (args.length > 0) {
					for (var i=0; i<args.length; i++) {
						try {
							//We check against given value, we expect it to be an array
							if (typeof args[i].push=="function" && args[i].constructor==Array) {
								if (typeof wiredminds[args[i][0]] === 'function') {
									var result = wiredminds[args[i][0]].apply(wiredminds, args[i].slice(1));
									called++;
								}
							}
						} catch (e) {
							//Ignore
						}
					}
				}
				return called;
			},
			/****  START HEATMAP PART  ****/
			lastClickTime: 0,
			cnt_path: '/track/countxy.php',
			/**
			 * Get the click.
			 *
			 */
			getClick: function(e) {

				var click_x = 0;
				var click_y = 0;
				var max_allowed_x = 2500;

				//Check if a cusnum is set
				if (wiredminds.getTrackParam('wm_custnum') === null) {
					return; // no customer number given, do nothing
				}

				// Check if last click was at least 1 second ago
				var time = new Date();
				if ((time.getTime() - wiredminds.lastClickTime) < 1000){
					return;
				}
				wiredminds.lastClickTime = time.getTime();

				/**
				 * Check if the heatmap is started
				 * If the overlay is still set, then the overlay is active, so there is no need to count the click.
				 * No need to validate the key, the counting pixel already did, and invalid keys are not stored.
				 */
				var overlayKey = wiredminds.ReadCookie('WMOverlayKey');
				if ((overlayKey) && (overlayKey.length == wiredminds.overlayKeyLength)){
					return;
				}

				// Grab coordinate x,y of clicks
				var browser = wiredminds.getBrowser();
				if (browser.search(/IE/) != -1) {
					click_x = event.clientX;
					click_y = event.clientY;
				} else {
					click_x = e.clientX;
					click_y = e.clientY;
				}

				var scrollbarWidth = wiredminds.getScrollbarWidth();
				var d = document.documentElement != undefined && document.documentElement.clientHeight != 0 ? document.documentElement : document.body;
				var scrollx  = window.pageXOffset == undefined ? d.scrollLeft : window.pageXOffset;
				var scrolly  = window.pageYOffset == undefined ? d.scrollTop : window.pageYOffset;
				var client_w = window.innerWidth == undefined ? d.clientWidth : window.innerWidth;
				var client_h = window.innerHeight == undefined ? d.clientHeight : window.innerHeight;
				var body_w   = d.offsetWidth;
				var body_h   = d.offsetHeight;

				// Check if clicks was on the scrollbars
				if ((body_h > client_h) && (click_x > (client_w - scrollbarWidth))) {
					return;
				}

				// Add the width/height of scroll to coordinate
				click_x = click_x + scrollx;
				click_y = click_y + scrolly;

				/**
				 * If wm_content_width provided content is centered, we need to calculate the mouse position relative to content width.
				 * Else content is left aligned, no need to modify mouse position.
				 */
				var click_x_orig = click_x;
				var wm_content_width = wiredminds.getTrackParam('wm_content_width');
				if (wm_content_width !== null){
					wm_content_width = parseInt(wm_content_width);
					if (!isNaN(wm_content_width) && (wm_content_width <= body_w)){
						click_x = (click_x - (body_w - wm_content_width)/2 );
					}
				}
				
				var page_name = wiredminds.getPageName();
				var params = new Array(
					'custnum='	+ wiredminds.getTrackParam('wm_custnum'),
					'pagename='	+ encodeURIComponent(page_name),
					'cx='		+ Math.round(click_x),
					'cy='		+ Math.round(click_y),
					'body_height=' + Math.round(body_h),
					'browser='	+ browser
					);

				if (wm_content_width !== null){
					if (!isNaN(wm_content_width) && (wm_content_width > 0)){
						max_allowed_x = ((body_w - wm_content_width)/2) + wm_content_width;
						params.push('content=' + parseInt(wm_content_width));
					}
				}
				// send request
				if ((click_x > 0) && (click_x_orig < max_allowed_x)){
					var proto = wiredminds.getProto();
					var count_heat_URL = proto + '//' + wiredminds.cnt_server + wiredminds.cnt_path;
					var heatmap_container = new Image();
					heatmap_container.src=count_heat_URL + '?' + params.join('&');
				}
			},
			/**
			 * Get the width of scrollbar
			 *
			 */
			getScrollbarWidth: function() {

				var scr = null;
				var inn = null;
				var wNoScroll = 0;
				var wScroll = 0;

				// Outer scrolling div
				scr = document.createElement('div');
				scr.style.position = 'absolute';
				scr.style.top = '-1000px';
				scr.style.left = '-1000px';
				scr.style.width = '100px';
				scr.style.height = '50px';
				// Start with no scrollbar
				scr.style.overflow = 'hidden';

				// Inner content div
				inn = document.createElement('div');
				inn.style.width = '100%';
				inn.style.height = '200px';

				// Put the inner div in the scrolling div
				scr.appendChild(inn);
				// Append the scrolling div to the doc
				document.body.appendChild(scr);

				// Width of the inner div sans scrollbar
				wNoScroll = inn.offsetWidth;

				// Add the scrollbar
				var browser = wiredminds.getBrowser();
				if (browser.search(/IE/) != -1) {
					scr.style.overflow = 'scroll';
				} else {
					scr.style.overflow = 'auto';
				}
				// Width of the inner div width scrollbar
				wScroll = inn.offsetWidth;

				// Remove the scrolling div from the doc
				document.body.removeChild(document.body.lastChild);

				// Pixel width of the scroller
				return (wNoScroll - wScroll);
			},
			/**
			 * Register heatmaps getClick event to given event type.
			 */
			registerHeatmapEvent: function(eventType) {
				var eventHandlerName = 'getClick';
				if (typeof wiredminds[eventHandlerName] === 'function') {
					if (document.addEventListener) {
						document.addEventListener(eventType, wiredminds[eventHandlerName], false);
					} else if (document.attachEvent){
						var ieEventType = 'on'+eventType;
						document.attachEvent(ieEventType, wiredminds[eventHandlerName]);
					}
				}
			},

			/**
			 * Detect the browsers the clickers used to view page.
			 * TODO: Moment take care the function only 4 Browsers: IE6/IE7/IE8/IE9/FF
			 */
			getBrowser: function() {
				var version = navigator.appVersion;
				if (version.search(/MSIE 6/i) != -1) return "IE6";
				if (version.search(/MSIE 7/i) != -1) return "IE7";
				if (version.search(/MSIE 8/i) != -1) return "IE8";
				if (version.search(/MSIE 9/i) != -1) return "IE9";
				if (version.search(/MSIE/i)   == -1) return "Others";
			}
			/****  END HEATMAP PART  ****/
		};
		return wiredminds;
	}

	/**
	 * Initializes the wiredminds object if an wiredminds array exists
	 */
	function initWMObj() {
		//If it is not yet our object, but an array. Then save the array, create our object and pass the array to our push function.
		if (typeof wiredminds != 'undefined' && typeof wiredminds.push=="function" && wiredminds.constructor==Array) {
			var wiredmindsObj = getWiredMinds();
			wiredmindsObj.push.apply(wiredmindsObj, wiredminds);
			wiredminds = wiredmindsObj;
		} else {
			if (typeof wiredminds == 'undefined') {
				wiredminds = getWiredMinds();
			}
		}
	}
	//Init the object
	initWMObj();

})();

function click_track(ext) {
	wiredminds.push(['count', ext]);
}

/**
 * Catch click for Heatmap
 *
 */
var getClick = function(e){
	wiredminds.push(['getClick', e]);
}

