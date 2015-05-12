/*ADOBE SYSTEMS INCORPORATED
Copyright 2007 Adobe Systems Incorporated
All Rights Reserved.

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the
terms of the Adobe license agreement accompanying it.  If you have received this file from a
source other than Adobe, then your use, modification, or distribution of it requires the prior
written permission of Adobe.*/
var KT_focusedEl=null;
KT_validateSingle=function(_52,_53){
var _54=_52.charCodeAt(0);
switch(_53){
case "9":
if(_54<58&&_54>47){
return true;
}
break;
case "A":
if((_54<91&&_54>64)||(_54<123&&_54>96)){
return true;
}
break;
case "X":
if((_54<91&&_54>64)||(_54<123&&_54>96)||(_54<58&&_54>47)){
return true;
}
break;
case "?":
return true;
break;
default:
return true;
break;
}
};
KT_maskDefaultValue=function(_55){
switch(_55){
case "9":
return "0";
break;
case "A":
return "a";
break;
case "X":
return "0";
break;
case "?":
return "0";
break;
default:
return "0";
break;
}
};
KT_isSpecialChar=function(_56){
if(_56=="9"||_56=="A"||_56=="X"||_56=="?"){
return true;
}else{
return false;
}
};
mask_onValueChanged=function(){
if((typeof window.getSelection=="undefined"&&typeof document.selection=="undefined")){
return;
}
if(KT_focusedEl==null||KT_focusedEl.mask==null||KT_focusedEl.mask==""){
return;
}
var _57=KT_focusedEl.mask;
var val=KT_focusedEl.value;
var i=0;
var _5a=false;
if(val==KT_focusedEl.oldText){
return;
}
if(val.length>_57.length){
val=val.substr(0,_57.length);
_5a=true;
}
for(;i<_57.length;i++){
if(val.charCodeAt(i).toString()!="NaN"){
if(KT_isSpecialChar(_57.charAt(i))){
if(KT_validateSingle(val.charAt(i),_57.charAt(i))){
continue;
}else{
val=KT_focusedEl.oldText;
i=_57.length;
break;
}
}else{
if(val.charAt(i)!=_57.charAt(i)){
if(i==val.length-1){
var _5b=val.substr(val.length-1,val.length);
val=val.substr(0,val.length-1)+_57.charAt(i)+_5b;
_5a=true;
continue;
}else{
val=KT_focusedEl.oldText;
i=_57.length;
}
break;
}
}
}else{
if(val.length<KT_focusedEl.oldText.length){
break;
}
for(;i<_57.length;i++){
if(!KT_isSpecialChar(_57.charAt(i))){
val+=_57.charAt(i);
_5a=true;
}else{
break;
}
}
break;
}
}
if(val.length>_57.length){
val=val.substr(0,_57.length);
_5a=true;
}
if(KT_focusedEl.value!=val){
KT_focusedEl.value=val;
}
KT_focusedEl.oldText=val;
if(_5a){
}
};
mask_parseFirstTime=function(_5c,_5d){
var _5e="";
var _5f="";
cond=1;
imask=0;
ival=0;
cnt=0;
while(cond==1){
cond=1;
if(!KT_isSpecialChar(_5d.charAt(imask))){
if(_5c.charCodeAt(ival).toString()!="NaN"){
if(_5d.charAt(imask)==_5c.charAt(ival)){
imask++;
ival++;
}else{
_5c=_5c.substr(0,ival)+_5d.charAt(imask)+_5c.substr(ival,_5c.length);
imask=0;
ival=0;
cond=1;
}
}else{
_5c+=KT_maskDefaultValue(_5d.charAt(imask));
}
}else{
imask++;
ival++;
}
if(imask>=_5d.length||ival>=_5c.length){
cond=0;
}
}
for(i=0;i<_5d.length;i++){
if(KT_isSpecialChar(_5d.charAt(i))){
_5e+=_5d.charAt(i);
if(_5c.charCodeAt(i).toString()!="NaN"){
_5f+=_5c.charAt(i);
}else{
_5f+=KT_maskDefaultValue(_5d.charAt(i));
}
}
}
oldvalue=_5c;
_5c=_5f;
var _60="";
for(i=0;i<_5e.length;i++){
if(!KT_validateSingle(_5c.charAt(i),_5e.charAt(i))){
_60+=KT_maskDefaultValue(_5e.charAt(i));
}else{
_60+=_5c.charAt(i);
}
}
var _61="";
var j=0;
for(i=0;i<_5d.length;i++){
if(KT_isSpecialChar(_5d.charAt(i))){
_61+=_60.charAt(j++);
}else{
_61+=_5d.charAt(i);
}
}
return _61;
};
mask_onSetFocus=function(obj,_64){
if((typeof window.getSelection=="undefined"&&typeof document.selection=="undefined")){
return;
}
if(typeof obj.mask=="undefined"){
ret="";
if(obj.value!=""){
ret=mask_parseFirstTime(obj.value,_64);
}
obj.value=ret;
obj.mask=_64;
}
KT_focusedEl=obj;
if(typeof KT_focusedEl.oldText=="undefined"){
KT_focusedEl.oldText=obj.value;
mask_onValueChanged();
}
};
mask_onKillFocus=function(){
if((typeof window.getSelection=="undefined"&&typeof document.selection=="undefined")){
return;
}
mask_onValueChanged();
KT_focusedEl=null;
};
