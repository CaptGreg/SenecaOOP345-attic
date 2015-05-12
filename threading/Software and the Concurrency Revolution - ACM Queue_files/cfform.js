/*ADOBE SYSTEMS INCORPORATED
Copyright 2007 Adobe Systems Incorporated
All Rights Reserved.

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the
terms of the Adobe license agreement accompanying it.  If you have received this file from a
source other than Adobe, then your use, modification, or distribution of it requires the prior
written permission of Adobe.*/
var _CF_error_messages=new Array();
var _CF_error_fields=new Object();
var _CF_FirstErrorField=null;
var _CF_submit_status=new Array();
_CF_signalLoad=function(){
_CF_loaded=1;
};
_CF_onError=function(_1,_2,_3,_4){
if(_CF_error_fields[_2]==null){
if(_CF_FirstErrorField==null){
_CF_FirstErrorField=_2;
}
_CF_error_exists=true;
_CF_error_fields[_2]=_4;
_CF_error_messages[_CF_error_messages.length]=_4;
}
};
_CF_onErrorAlert=function(_5){
var _6="";
for(var i=0;i<_5.length;i++){
_6+=_5[i]+"\n";
}
alert(_6);
return false;
};
updateHiddenValue=function(_8,_9,_a){
if(_9==null||_9==""){
_9=0;
}
if(document.forms[_9]==null||document.forms[_9][_a]==null){
return;
}
document.forms[_9][_a].value=_8;
};
_CF_hasValue=function(_b,_c,_d){
if(_c=="TEXT"||_c=="FILE"||_c=="PASSWORD"||_c=="CFTEXTAREA"||_c=="TEXTAREA"||_c=="CFTEXTINPUT"||_c=="DATEFIELD"){
if(_b.value.length==0){
return false;
}else{
if(_d){
str=_b.value.replace(/^\s+/,"").replace(/\s+$/,"");
if(str.length==0){
return false;
}
}
}
return true;
}else{
if(_c=="SELECT"){
for(i=0;i<_b.length;i++){
if(_b.options[i].selected&&_b.options[i].value.length>0){
return true;
}
}
return false;
}else{
if(_c=="SINGLE_VALUE_RADIO"||_c=="SINGLE_VALUE_CHECKBOX"){
if(_b.checked){
return true;
}else{
return false;
}
}else{
if(_c=="RADIO"||_c=="CHECKBOX"){
if(_b.length==undefined&&_b.checked){
return true;
}else{
for(i=0;i<_b.length;i++){
if(_b[i].checked){
return true;
}
}
}
return false;
}else{
if(_c=="CFTREE"){
if(_b["value"].length>0){
return true;
}else{
return false;
}
}else{
if(_c=="RICHTEXT"){
var _e=FCKeditorAPI.GetInstance(_b.id);
var _f=_e.GetXHTML();
if(_f.length==0){
return false;
}else{
if(_d){
str=_f.replace(/^\s+/,"").replace(/\s+$/,"");
if(str.length==0){
return false;
}
}
return true;
}
}else{
return true;
}
}
}
}
}
}
};
_CF_checkdate=function(_10,_11){
_10=_10.replace(/^\s+/,"").replace(/\s+$/,"");
_10=_10=_10.replace(/{d \'/,"").replace(/'}/,"");
if(_11){
if(_10.length==0){
return false;
}
}else{
if(_10.length==0){
return true;
}
}
if(_10.length==0){
return true;
}
isplit=_10.indexOf("/");
splitchr="/";
if(isplit==-1){
isplit=_10.indexOf(".");
splitchr=".";
}
if(isplit==-1){
isplit=_10.indexOf("-");
splitchr="-";
}
if(isplit==-1||isplit==_10.length){
return false;
}
var _12=_10.substring(0,isplit);
if(_12.length==4){
sYear=_10.substring(0,isplit);
isplit=_10.indexOf(splitchr,isplit+1);
if(isplit==-1||(isplit+1)==_10.length){
return false;
}
sMonth=_10.substring((sYear.length+1),isplit);
sDay=_10.substring(isplit+1);
}else{
sMonth=_10.substring(0,isplit);
isplit=_10.indexOf(splitchr,isplit+1);
if(isplit==-1||(isplit+1)==_10.length){
return false;
}
sDay=_10.substring((sMonth.length+1),isplit);
sYear=_10.substring(isplit+1);
}
if((sDay.length==0)||(sMonth.length==0)||(sYear.length==0)){
return false;
}
if(!_CF_checkinteger(sMonth)){
return false;
}else{
if(!_CF_checkrange(sMonth,1,12)){
return false;
}else{
if(!_CF_checkinteger(sYear)){
return false;
}else{
if(sYear.length!=1&&sYear.length!=2&&sYear.length!=4){
return false;
}else{
if(!_CF_checkrange(sYear,0,9999)){
return false;
}else{
if(!_CF_checkinteger(sDay)){
return false;
}else{
if(!_CF_checkday(sYear,sMonth,sDay)){
return false;
}else{
return true;
}
}
}
}
}
}
}
};
_CF_checkeurodate=function(_13,_14){
_13=_13.replace(/^\s+/,"").replace(/\s+$/,"");
_13=_13=_13.replace(/{d \'/,"").replace(/'}/,"");
if(_14){
if(_13.length==0){
return false;
}
}else{
if(_13.length==0){
return true;
}
}
isplit=_13.indexOf("/");
splitchr="/";
if(isplit==-1){
isplit=_13.indexOf(".");
splitchr=".";
}
if(isplit==-1){
isplit=_13.indexOf("-");
splitchr="-";
}
if(isplit==-1||isplit==_13.length){
return false;
}
var _15=_13.substring(0,isplit);
if(_15.length==4){
sYear=_13.substring(0,isplit);
isplit=_13.indexOf(splitchr,isplit+1);
if(isplit==-1||(isplit+1)==_13.length){
return false;
}
sMonth=_13.substring((sYear.length+1),isplit);
sDay=_13.substring(isplit+1);
}else{
sDay=_13.substring(0,isplit);
isplit=_13.indexOf(splitchr,isplit+1);
if(isplit==-1||(isplit+1)==_13.length){
return false;
}
sMonth=_13.substring((sDay.length+1),isplit);
sYear=_13.substring(isplit+1);
}
if(!_CF_checkinteger(sMonth)){
return false;
}else{
if(!_CF_checkrange(sMonth,1,12)){
return false;
}else{
if(!_CF_checkinteger(sYear)){
return false;
}else{
if(!_CF_checkrange(sYear,0,null)){
return false;
}else{
if(!_CF_checkinteger(sDay)){
return false;
}else{
if(!_CF_checkday(sYear,sMonth,sDay)){
return false;
}else{
return true;
}
}
}
}
}
}
};
_CF_checkday=function(_16,_17,_18){
maxDay=31;
if(_17==4||_17==6||_17==9||_17==11){
maxDay=30;
}else{
if(_17==2){
if(_16%4>0){
maxDay=28;
}else{
if(_16%100==0&&_16%400>0){
maxDay=28;
}else{
maxDay=29;
}
}
}
}
return _CF_checkrange(_18,1,maxDay);
};
_CF_checkinteger=function(_19,_1a){
_19=_19.replace(/^\s+/,"").replace(/\s+$/,"");
_19=_19.replace(/[$£¥€,~+]?/g,"");
if(_1a){
if(_19.length==0){
return false;
}
}else{
if(_19.length==0){
return true;
}
}
var _1b=".";
var _1c=_19.indexOf(_1b);
if(_1c==-1){
return _CF_checknumber(_19);
}else{
return false;
}
};
_CF_numberrange=function(_1d,_1e,_1f,_20){
if(_20){
if(_1d.length==0){
return false;
}
}else{
if(_1d.length==0){
return true;
}
}
if(_1e!=null){
if(_1d<_1e){
return false;
}
}
if(_1f!=null){
if(_1d>_1f){
return false;
}
}
return true;
};
_CF_checknumber=function(_21,_22){
var _23=" .+-0123456789";
var _24=" .0123456789";
var _25;
var _26=false;
var _27=false;
var _28=false;
_21=_21.replace(/^\s+/,"").replace(/\s+$/,"");
_21=_21.replace(/[$£¥€,~+]?/g,"");
if(_22){
if(_21.length==0){
return false;
}
}else{
if(_21.length==0){
return true;
}
}
_25=_23.indexOf(_21.charAt(0));
if(_25==1){
_26=true;
}else{
if(_25<1){
return false;
}
}
for(var i=1;i<_21.length;i++){
_25=_24.indexOf(_21.charAt(i));
if(_25<0){
return false;
}else{
if(_25==1){
if(_26){
return false;
}else{
_26=true;
}
}else{
if(_25==0){
if(_26||_28){
_27=true;
}
}else{
if(_27){
return false;
}else{
_28=true;
}
}
}
}
}
return true;
};
_CF_checkrange=function(_2a,_2b,_2c,_2d){
_2a=_2a.replace(/^\s+/,"").replace(/\s+$/,"");
if(_2d){
if(_2a.length==0){
return false;
}
}else{
if(_2a.length==0){
return true;
}
}
if(!_CF_checknumber(_2a)){
return false;
}else{
return (_CF_numberrange((eval(_2a)),_2b,_2c));
}
return true;
};
_CF_checktime=function(_2e,_2f){
_2e=_2e.replace(/^\s+/,"").replace(/\s+$/,"");
_2e=_2e.replace(/\s+:\s+/,":");
_2e=_2e=_2e.replace(/{t \'/,"").replace(/'}/,"");
if(_2f){
if(_2e.length==0){
return false;
}
}else{
if(_2e.length==0){
return true;
}
}
var _30=_CF_checkregex(_2e,/^((([0-1]?\d)|(2[0-3])):[0-5]?\d)?(:[0-5]?\d)? ?([AP]M|[AP]m|[ap]m|[ap]M)?$/,_2f);
return _30;
};
_CF_checkphone=function(_31,_32){
_31=_31.replace(/^\s+/,"").replace(/\s+$/,"");
if(_32){
if(_31.length==0){
return false;
}
}else{
if(_31.length==0){
return true;
}
}
if(_31.length==0){
return true;
}
return _CF_checkregex(_31,/^(((1))?[ ,\-,\.]?([\\(]?([1-9][0-9]{2})[\\)]?))?[ ,\-,\.]?([^0-1]){1}([0-9]){2}[ ,\-,\.]?([0-9]){4}(( )((x){0,1}([0-9]){1,5}){0,1})?$/,_32);
};
_CF_checkzip=function(_33,_34){
_33=_33.replace(/^\s+/,"").replace(/\s+$/,"");
if(_34){
if(_33.length==0){
return false;
}
}else{
if(_33.length==0){
return true;
}
}
return _CF_checkregex(_33,/^([0-9]){5,5}$|(([0-9]){5,5}(-| ){1}([0-9]){4,4}$)/,_34);
};
_CF_checkcreditcard=function(_35,_36){
_35=_35.replace(/^\s+/,"").replace(/\s+$/,"");
if(_36){
if(_35.length==0){
return false;
}
}else{
if(_35.length==0){
return true;
}
}
if(_35.length==0){
return true;
}
var _37=" -";
var _38="";
var _39;
for(var i=0;i<_35.length;i++){
_39=_37.indexOf(_35.charAt(i));
if(_39<0){
_38+=_35.substring(i,(i+1));
}
}
if(_38.length<13||_38.length>19){
return false;
}
if(_38.charAt(0)=="+"){
return false;
}
if(!_CF_checkinteger(_38)){
return false;
}
var _3b=_38.length%2==1?false:true;
var _3c=0;
var _3d;
for(var i=0;i<_38.length;i++){
_3d=eval(_38.charAt(i));
if(_3b){
_3d*=2;
_3c+=(_3d%10);
if((_3d/10)>=1){
_3c++;
}
_3b=false;
}else{
_3c+=_3d;
_3b=true;
}
}
return (_3c%10)==0?true:false;
};
_CF_checkssn=function(_3e,_3f){
_3e=_3e.replace(/^\s+/,"").replace(/\s+$/,"");
if(_3f){
if(_3e.length==0){
return false;
}
}else{
if(_3e.length==0){
return true;
}
}
return _CF_checkregex(_3e,/^[0-9]{3}(-| )[0-9]{2}(-| )[0-9]{4}$/,_3f);
};
_CF_checkEmail=function(_40,_41){
_40=_40.replace(/^\s+/,"").replace(/\s+$/,"");
if(_41){
if(_40.length==0){
return false;
}
}else{
if(_40.length==0){
return true;
}
}
return _CF_checkregex(_40,/^[a-zA-Z_0-9-'\+~]+(\.[a-zA-Z_0-9-'\+~]+)*@([a-zA-Z_0-9-]+\.)+[a-zA-Z]{2,7}$/,_41);
};
_CF_checkURL=function(_42,_43){
_42=_42.replace(/^\s+/,"").replace(/\s+$/,"");
if(_43){
if(_42.length==0){
return false;
}
}else{
if(_42.length==0){
return true;
}
}
return _CF_checkregex(_42.toLowerCase(),/^((http|https|ftp|file)\:\/\/([a-zA-Z0-0]*:[a-zA-Z0-0]*(@))?[a-zA-Z0-9-\.]+(\.[a-zA-Z]{2,3})?(:[a-zA-Z0-9]*)?\/?([a-zA-Z0-9-\._\?\,\'\/\+&amp;%\$#\=~])*)|((mailto)\:[a-zA-Z0-9-]+(\.[a-zA-Z0-9-]+)*@([a-zA-Z0-9-]+\.)+[a-zA-Z0-9]{2,7})|((news)\:[a-zA-Z0-9\.]*)$/,_43);
};
_CF_checkUUID=function(_44,_45){
_44=_44.replace(/^\s+/,"").replace(/\s+$/,"");
if(_45){
if(_44.length==0){
return false;
}
}else{
if(_44.length==0){
return true;
}
}
return _CF_checkregex(_44,/[A-Fa-f0-9]{8,8}-[A-Fa-f0-9]{4,4}-[A-Fa-f0-9]{4,4}-[A-Fa-f0-9]{16,16}/,_45);
};
_CF_checkGUID=function(_46,_47){
_46=_46.replace(/^\s+/,"").replace(/\s+$/,"");
if(_47){
if(_46.length==0){
return false;
}
}else{
if(_46.length==0){
return true;
}
}
return _CF_checkregex(_46,/[A-Fa-f0-9]{8,8}-[A-Fa-f0-9]{4,4}-[A-Fa-f0-9]{4,4}-[A-Fa-f0-9]{4,4}-[A-Fa-f0-9]{12,12}/,_47);
};
_CF_checkBoolean=function(_48,_49){
_48=_48.replace(/^\s+/,"").replace(/\s+$/,"");
if(_49){
if(_48.length==0){
return false;
}
}else{
if(_48.length==0){
return true;
}
}
if(_48.toUpperCase()=="TRUE"||_48.toUpperCase()=="YES"||(_CF_checknumber(_48)&&_48!="0")){
return true;
}else{
if(_48.toUpperCase()=="FALSE"||_48.toUpperCase()=="NO"||_48=="0"){
return true;
}else{
return false;
}
}
};
_CF_setFormParam=function(_4a,_4b,_4c){
var _4d="document['"+_4a+"']['"+_4b+"']";
var obj=eval(_4d);
if(obj==undefined){
return false;
}else{
obj.value=_4c;
return true;
}
};
_CF_checkregex=function(_4f,_50,_51){
if(_51){
if(_4f.length==0){
return false;
}
}else{
if(_4f.length==0){
return true;
}
}
return _50.test(_4f);
};
