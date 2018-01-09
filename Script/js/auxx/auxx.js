var aux={};

/* ********** */
aux.qany=function(word1,word2,word3) {
  var ans='';
  if (typeof word1=='string')
    ans+=word1;
  if (typeof word2=='string')
    ans+=word2;
  if (typeof word3=='string')
    ans+=word3;
  return ans;
};

/* ********** */
aux.qword=function(word,indent) {
  var ans='';
  if (typeof indent=='string')
    ans+=indent;
  ans+='\"';
  if (typeof word=='string')
    ans+=word;
  else if (typeof word!='undefined')
    ans+=word.toString();
  ans+='\"';
  if (typeof suffix=='string')
    ans+=suffix;
  return ans;
};

/* ********** */
aux.qdecl=function(word,indent) {
  var ans='';
  if (typeof indent=='string')
    ans+=indent;
  ans+='\"';
  if (typeof word=='string')
    ans+=word;
  else if (typeof word!='undefined')
    ans+=word.toString();
  ans+='\":';
  return ans;
};

/* ********** */
aux.qarr=function(word,lf,indent) {
  var ans='';
  if (lf===1)
    ans+='\n';
  if (typeof indent=='string')
    ans+=indent;
  ans+='[';
  if (typeof word=='string')
    ans+=word;
  else if (typeof word!='undefined')
    ans+=word.toString();
  if (lf===1) {
    ans+='\n';
    if (typeof indent=='string')
      ans+=indent;
  }
  ans+=']';
  return ans;
};

/* ********** */
aux.qattr=function(word,lf,indent) {
  var ans='';
  if (lf===1)
    ans+='\n';
  if (typeof indent=='string')
    ans+=indent;
  ans+='{';
  if (lf===1)
    ans+='\n';
  if (typeof word=='string')
    ans+=word;
  else if (typeof word!='undefined')
    ans+=word.toString();
  if (lf===1) {
    ans+='\n';
    if (typeof indent=='string')
      ans+=indent;
  }
  ans+='}';
  return ans;
};

/* ********** */
aux.qnextprop=function(lf,indent,del) {
  var ans=(typeof del=='string' ? del : ',');
  if (lf===1)
    ans+='\n';
  if (typeof indent=='string')
    ans+=indent;
  return ans;
};

/* ********** */
aux.swap=function(itm12) {
  var tmp=itm12[0];
  itm12[0]=itm12[1];
  itm12[1]=tmp;
};

/* ********** */
aux.swapidx=function(itmarr,idx1,idx2) {
  var tmp=itmarr[idx1];
  itmarr[idx1]=itmarr[idx2];
  itmarr[idx2]=tmp;
};

/* ********** */
aux.reqFileRead=function(strfilename) {
  var filereq=new XMLHttpRequest();
  filereq.open("GET",strfilename,false);
  filereq.send();
  var strcontent=filereq.responseText;
  return strcontent;
};

/* ********** */
aux.reqFileSave=function(strcontent,strfilename,strcginame) {
  var filereq=new XMLHttpRequest();
  filereq.open('POST',strcginame,false);
  strsend='//';
  strsend+=strfilename;
  strsend+='\n';
  strsend+=strcontent;
  filereq.send(strsend);
  var ans=filereq.responseText;
  return ans;
};

/* ########## */
aux.Inx=function(str,bb) {
  this.fromString(str,bb);
};

/* ********** */
aux.Inx.prototype.toJson=function(indent) {
  var ans=aux.qdecl('num');
  ans+=aux.qword(this.num);
  if (typeof indent=='string')
    ans=(indent+ans);
  return ans;
};

/* ********** */
aux.Inx.prototype.toString=function(bb) {
  bb=mkint.chkbase(bb);
  if (bb==this.base)
    return this.num;
  var ans=(this.base==2 ? this.num : mkint.str2bits(this.num,this.base));
  if (bb!=2)
    ans=mkint.bits2str(ans,bb);
  return ans;
};

/* ********** */
aux.Inx.prototype.fromString=function(str,bb) {
  this.base=mkint.chkbase(bb);
  if (typeof str!='string')
    str=mkuni.str4code(mkuni.asciizero);
  str=str.trim().toUpperCase();
  if (str.length==0)
    this.num=mkuni.str4code(mkuni.asciizero);
  else {
    var ii=0,idx=-1;
    for (ii=0;ii<str.length;ii++) {
      idx=mkint.basecharcode.indexOf(str.charCodeAt(ii));
      if (idx<0 || idx>=this.base) {
        str=(ii==0 ? mkuni.str4code(mkuni.asciizero) : str.substring(0,ii));
        break;
      }
    }
    this.num=str;
  }
};
/* ########## */

/* ********** */
aux.cmpinx=function(inx1,inx2) {
  var ans=mkint.cmpbits(inx1.toString(2),inx2.toString(2));
  return ans;
};

/* ########## */
aux.TypeId=function(typename,typenum) {
  var tname=(((typeof typename!='string') || typename.length==0) ? 'unnamed' : typename);
  var msnow=Date.now();
  var tnum=new aux.Inx(msnow.toString(),10);
  if (typenum instanceof aux.Inx)
    tnum=typenum;
  else if (typeof typenum=='string')
    tnum.num=typenum;
  else if (typeof typenum=='number')
    tnum.num=typenum.toString();
  this.idstr=tname+mkuni.str4code(mkuni.asciiminus)+tnum.toString(mkint.maxbase);
  while (Date.now()==msnow) ;
};

/* ********** */
aux.TypeId.prototype.typename=function() {
  return this.idstr.substring(0,this.idstr.indexOf('-'));
};

/* ********** */
aux.TypeId.prototype.typenum=function() {
  var inx=new aux.Inx();
  inx.fromString(this.idstr.substring(this.idstr.indexOf('-')+1),mkint.maxbase);
  return inx;
};

/* ********** */
aux.TypeId.prototype.toJson=function(indent,addname) {
  var ans='';
  if ((typeof addname=='number') && addname>0)
    ans+=aux.qdecl('idstr');
  ans+=aux.qword(this.idstr);
  if (typeof indent=='string')
    ans=(indent+ans);
  return ans;
};

/* ********** */
aux.TypeId.prototype.toString=function() {
  return this.idstr;
};

/* ********** */
aux.TypeId.prototype.fromString=function(str) {
  var tname='unnamed';
  var date=new Date();
  var tnum=new aux.Inx(date.valueOf().toString(),10);
  if ((typeof str=='string') && str.length>0) {
    var idx=str.indexOf(mkuni.str4code(mkuni.asciiminus));
    if (idx<0)
      tname=str;
    else {
      tnum=new aux.Inx(str.substring(idx+1),mkint.maxbase);
      if (idx>0)
        tname=str.substring(0,idx);
    }
  }
  this.idstr=tname+mkuni.str4code(mkuni.asciiminus)+tnum.toString(mkint.maxbase);
};
/* ########## */

/* ********** */
aux.cmptypeid=function(typeid1,typeid2) {
  var ans=typeid1.typename().localeCompare(typeid2.typename());
  if (ans==0)
    ans=aux.cmpinx(typeid1.typenum(),typeid2.typenum());
  return ans;
};
