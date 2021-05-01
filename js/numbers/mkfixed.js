
var mkfixed={};

mkfixed.maxnumsz=10000;
mkfixed.minnumsz=-10000;
mkfixed.maxmaxlen=4096;
mkfixed.maxlen=512;
mkfixed.defrd=128;

/* ***** internal ***** */
mkfixed.cmpstr10=function(cmp1,cmp2) {
  var s1=mkuni.chkstr(cmp1).toUpperCase(),s2=mkuni.chkstr(cmp2).toUpperCase();
  if (s1.length!=s2.length)
    return (s1.length<s2.length ? -1 : 1);
  if (s1.length==0 && s2.length==0)
    return 0;
  if (s1==mkuni.strnan || s2==mkuni.strnan)
    return (s1==mkuni.strnan ? (s2==mkuni.strnan ? 0 : 1) : -1);
  var ii=0,cmp=0;
  for (ii=0;ii<s1.length;ii++) {
    cmp=(s1.charCodeAt(ii)-s2.charCodeAt(ii));
    if (cmp!=0)
      return (cmp<0 ? -1 : 1)
  }
  return 0;
};

/* ***** internal ***** */
mkfixed.cutstr10=function(str) {
  var ans=mkuni.chkstr(str);
  if (ans.length==0)
    return ans;
  var idx=ans.indexOf(mkuni.str4code(mkuni.asciidot));
  if (idx>=0)
    ans=ans.substring(0,idx);
  if (ans.length==0)
    ans=mkuni.str4code(mkuni.asciizero);
  return ans;
};

/* ***** internal ***** */
mkfixed.chkcalctype=function(calctype) {
  var ans=0;
  if ((typeof calctype=='string') && calctype.length>0) {
    ans=calctype.charCodeAt(0);
    if (ans!=mkuni.asciiplus && ans!=mkuni.asciiminus &&
        ans!=mkuni.asciimult && ans!=mkuni.asciidiv)
      ans=0;
  }
  return ans;
};

/* ##### FixedPoint ##### */
mkfixed.FixedPoint=function(num) {
  this.compound=mkuni.chkstr(num);
  this.base=0;
  this.sgn=0;
  this.intg='';
  this.frac='';
  this.mag=0;
  this.mant='';
  this.shr=0;
  if (this.compound.length==0)
    this.compound=mkuni.strnan;
  else
    this.compound=this.compound.toUpperCase();
};

/* ***** FixedPoint ***** */
mkfixed.FixedPoint.prototype.copy=function() {
  var ans=new this.constructor();
  ans.compound=this.compound;
  ans.base=this.base;
  ans.sgn=this.sgn;
  ans.intg=this.intg;
  ans.frac=this.frac;
  ans.mag=this.mag;
  ans.mant=this.mant;
  ans.shr=this.shr;
  return ans;
};

/* ***** FixedPoint ***** */
mkfixed.FixedPoint.prototype.toJson=function(indent) {
  var nextindent='  ',xans='',ans='';
  if (typeof indent=='string')
    nextindent+=indent;
  ans+=aux.qdecl('compound',nextindent);
  ans+=aux.qword(this.compound);
  ans+=aux.qnextprop(1,indent);
  ans+=aux.qdecl('base',nextindent);
  ans+=aux.qword(mkint.int2str(this.base));
  ans+=aux.qnextprop(1,indent);
  ans+=aux.qdecl('sgn',nextindent);
  ans+=aux.qword(mkuni.str4code(this.sgn));
  ans+=aux.qnextprop(1,indent);
  ans+=aux.qdecl('intg',nextindent);
  ans+=aux.qword(this.intg);
  ans+=aux.qnextprop(1,indent);
  ans+=aux.qdecl('frac',nextindent);
  ans+=aux.qword(this.frac);
  ans+=aux.qnextprop(1,indent);
  ans+=aux.qdecl('mag',nextindent);
  ans+=aux.qword(mkint.int2str(this.mag));
  ans+=aux.qnextprop(1,indent);
  ans+=aux.qdecl('mant',nextindent);
  ans+=aux.qword(this.mant);
  ans+=aux.qnextprop(1,indent);
  ans+=aux.qdecl('shr',nextindent);
  ans+=aux.qword(this.shr);
  xans+=aux.qattr(ans,1,indent);
  return xans;
};

/* ***** FixedPoint ***** */
mkfixed.FixedPoint.prototype.funmix=function() {
  this.base=0;
  this.sgn=0;
  this.intg='';
  this.frac='';
  this.mag=0;
  this.mant='';
  this.shr=0;
  if (this.compound.length==0 || this.compound==mkuni.strnan)
    return this.base;
  var str=this.compound;
  var re_f=/^([+-]?)([0-9]*)(\.?)([0-9]*)(E[+-]?[0-9]{1,4})?$/;
  var matcharr=str.match(re_f);
  if ((typeof matcharr=='undefined') || !(matcharr instanceof Array) || matcharr.length==0)
    return this.base;
  var strintg='',strfrac='',strexp='';
  if (matcharr.length>1 && (typeof matcharr[1]!='undefined') && matcharr[1].length>0) {
    if (mkint.sgn4charcode(matcharr[1].charCodeAt(0))<0)
      this.sgn=mkuni.asciiminus;
  }
  if (matcharr.length>2 && (typeof matcharr[2]!='undefined') && matcharr[2].length>0)
    strintg=matcharr[2];
  if (matcharr.length>4 && (typeof matcharr[4]!='undefined') && matcharr[4].length>0)
    strfrac=matcharr[4];
  if (matcharr.length>5 && (typeof matcharr[5]!='undefined') && matcharr[5].length>0) {
    var stre=matcharr[5];
    var re_e=/^E([+-]?)([0-9]{1,4})$/;
    var ematcharr=stre.match(re_e);
    if ((typeof ematcharr!='undefined') && (ematcharr instanceof Array) && ematcharr.length>0) {
      if (ematcharr.length>2 && (typeof ematcharr[2]!='undefined') && ematcharr[2].length>0) {
        strexp=mkint.stripleadingzeros(ematcharr[2]);
        if (strexp.length>0 && (typeof ematcharr[1]!='undefined') &&
            ematcharr[1].length>0 && mkint.sgn4charcode(ematcharr[1].charCodeAt(0))<0)
          strexp=(mkuni.str4code(mkuni.asciiminus)+strexp);
      }
    }
  }
  if (strintg.length==0 && strfrac.length==0 && strexp.length==0)
    return this.base;
  strintg=mkint.stripleadingzeros(strintg);
  if (strintg.length==0 && strfrac.length==0)
    strintg=mkuni.str4code(strexp.length==0 ? mkuni.asciizero : mkuni.asciione);
  var intgarr=[];
  if (strintg.length>0)
    intgarr=strintg.split('');
  var fracarr=[];
  if (strfrac.length>0)
    fracarr=strfrac.split('');
  var iexp=0;
  if (strexp.length>0)
    iexp=mkint.int4str(strexp);
  while (intgarr.length>0 && iexp<mkfixed.maxnumsz) {
    fracarr.unshift(intgarr.pop());
    iexp++;
  }
  if (fracarr.length>0) {
    strfrac=mkint.striptrailingzeros(fracarr.join(''));
    if (strfrac.length>0)
      fracarr=strfrac.split('');
    else
      fracarr=[];
  }
  while (fracarr.length>0 && mkfixed.minnumsz<iexp) {
    cc=fracarr[0].charCodeAt(0);
    if (cc!=mkuni.asciizero)
      break;
    fracarr.shift();
    iexp--;
  }
  if (iexp<=mkfixed.minnumsz || mkfixed.maxnumsz<=iexp)
    return this.base;
  if (fracarr.length==0) {
    this.base=1;
    this.intg=mkuni.str4code(mkuni.asciizero);
  }
  else {
    this.base=10;
    this.intg=mkuni.str4code(fracarr.shift().charCodeAt(0));
    this.mag=iexp-1;
  }
  if (fracarr.length>0)
    this.frac=fracarr.join('');
  this.shr=this.mag-this.frac.length;
  this.mant=this.intg+this.frac;
  while (this.shr>0) {
    this.mant+=mkuni.str4code(mkuni.asciizero);
    this.shr--;
  }
  return this.base;
};

/* ***** FixedPoint ***** */
mkfixed.FixedPoint.prototype.unmix=function(baseto) {
  baseto=mkint.chkbase(baseto,10);
  if (baseto==10)
    return this.funmix();
  this.base=0;
  this.sgn=0;
  this.intg='';
  this.frac='';
  this.mag=0;
  this.mant='';
  this.shr=0;
  if (this.compound.length==0 || this.compound==mkuni.strnan)
    return this.base;
  var str=this.compound;
  var re_hex=/^(0x)$/;
  var matcharr=str.match(re_hex);
  if ((typeof matcharr!='undefined') && (matcharr instanceof Array) && matcharr.length>1) {
    str=str.substring(2);
    baseto=16;
  }
  var re_i=/^([+-]?)([A-Z0-9_]+)$/;
  matcharr=str.match(re_i);
  if ((typeof matcharr=='undefined') || !(matcharr instanceof Array) || matcharr.length==0)
    return this.base;
  if (matcharr.length>1 && (typeof matcharr[1]!='undefined') && matcharr[1].length>0) {
    if (mkint.sgn4charcode(matcharr[1].charCodeAt(0))<0)
      this.sgn=mkuni.asciiminus;
  }
  var strintg='';
  if (matcharr.length>2 && (typeof matcharr[2]!='undefined') && matcharr[2].length>0)
    strintg=matcharr[2];
  if (strintg.length==0)
    return this.base;
  strintg=mkint.stripleadingzeros(strintg);
  if (strintg.length==0) {
    this.base=1;
    this.intg=mkuni.str4code(mkuni.asciizero);
    return this.base;
  }
  var cc=0,ii=0,calcedbase=0;
  for (ii=0;ii<strintg.length;ii++) {
    cc=mkint.base4charcode(strintg.charCodeAt(ii));
    if (cc>calcedbase)
      calcedbase=cc;
  }
  if (calcedbase>0 && calcedbase<=baseto) {
    this.base=baseto;
    this.intg=strintg;
  }
  return this.base;
};

/* ***** FixedPoint ***** */
mkfixed.FixedPoint.prototype.toString=function() {
  if (this.base==0)
    this.unmix();
  if (this.base==0)
    return mkuni.strnan;
  if (this.base==1)
    return mkuni.str4code(mkuni.asciizero);
  var ans=mkuni.str4code(this.sgn);
  if (this.intg.length>0)
    ans+=this.intg;
  if (this.frac.length>0) {
    ans+=mkuni.str4code(mkuni.asciidot);
    ans+=this.frac;
  }
  if (this.mag!=0) {
    ans+=mkuni.str4code(mkuni.asciiE);
    ans+=mkint.int2str(this.mag);
  }
  return ans;
};

/* ***** FixedPoint ***** */
mkfixed.FixedPoint.prototype.toStringI=function() {
  var ans=mkuni.strnan;
  if (this.base==0)
    this.unmix();
  if (this.base==0)
    return ans;
  if (this.base==1) {
    ans=mkuni.str4code(mkuni.asciizero);
    return ans;
  }
  var iexp=this.mag;
  var intgarr=this.intg.split('');
  if (iexp<0) {
    while (intgarr.length>0 && iexp++<0)
      intgarr.pop();
  }
  else if (iexp>0) {
    var fracarr=this.frac.split('');
    while (fracarr.length>0 && iexp-->0)
      intgarr.push(fracarr.shift());
    while (iexp-->0)
      intgarr.push(mkuni.str4code(mkuni.asciizero));
  }
  if (intgarr.length>0)
    ans=(mkuni.str4code(this.sgn)+intgarr.join(''));
  else
    ans=mkuni.str4code(mkuni.asciizero);
  return ans;
};

/* ***** FixedPoint ***** */
mkfixed.FixedPoint.prototype.toZero=function() {
  this.compound=mkuni.str4code(mkuni.asciizero);
  this.base=1;
  this.sgn=0;
  this.intg=mkuni.str4code(mkuni.asciizero);
  this.frac='';
  this.mag=0;
  this.mant=mkuni.str4code(mkuni.asciizero);
  this.shr=0;
};

/* ***** FixedPoint ***** */
mkfixed.FixedPoint.prototype.toAbs=function() {
  var ans=this.sgn;
  this.sgn=0;
  return ans;
};

/* ***** FixedPoint ***** */
mkfixed.FixedPoint.prototype.isInt=function() {
  if (this.base!=10)
    return this.base;
  return (this.mag<this.frac.length ? 0 : 10);
};

/* ***** FixedPoint ***** */
mkfixed.FixedPoint.prototype.roundpos=function(pos) {
  var ans=this.copy();
  if (ans.base==0)
    ans.unmix();
  if (ans.base!=10)
    return ans;
  pos=mkuni.chknumber(pos,0);
  var rdabspos=ans.shr;
  if (rdabspos<0)
    rdabspos=-rdabspos;
  rdabspos-=pos;
  if (rdabspos<=0)
    return ans;
  rdabspos=ans.mant.length-rdabspos;
  if (rdabspos<=0) {
    ans.toZero();
    return ans;
  }
  var rdarr=ans.mant.split('');
  var dig=0,cc=rdarr[rdabspos].charCodeAt(0)-mkuni.asciizero+5;
  rdarr=rdarr.slice(0,rdabspos);
  while (cc>9 && rdabspos>0) {
    cc=rdarr[--rdabspos].charCodeAt(0)-mkuni.asciizero+1;
    dig=(cc<10 ? cc : 0);
    rdarr[rdabspos]=mkuni.str4code(dig+mkuni.asciizero);
  }
  if (cc>9)
    rdarr.unshift(mkuni.str4code(mkuni.asciione));
  var revpos=pos;
  while (revpos++<0)
    rdarr.push(mkuni.str4code(mkuni.asciizero));
  var dpos=rdarr.length-pos;
  while (dpos<0) {
    rdarr.unshift(mkuni.str4code(mkuni.asciizero));
    dpos++;
  }
  ans.compound=mkuni.str4code(ans.sgn);
  ans.compound+=rdarr.slice(0,dpos).join('');
  if (dpos<rdarr.length) {
    ans.compound+=mkuni.str4code(mkuni.asciidot);
    ans.compound+=rdarr.slice(dpos).join('');
  }
  ans.unmix();
  return ans;
};

/* ***** internal ***** */
mkfixed.unfixcalc=function(calctype,strres,fp1,fp2,corr) {
  var itype=mkfixed.chkcalctype(calctype);
  if (itype==0 || (typeof strres!='string') || strres.length==0 ||
       (typeof fp1=='undefined') || !(fp1 instanceof mkfixed.FixedPoint) ||
       (typeof fp2=='undefined') || !(fp2 instanceof mkfixed.FixedPoint))
    return strres;
  if (typeof corr!='number')
    corr=0;
  var fpres=new mkfixed.FixedPoint(strres);
  fpres.unmix();
  var idx=0,ll=strres.length;
  if (itype==mkuni.asciiplus || itype==mkuni.asciiminus)
    idx=(fp1.shr<fp2.shr ? fp1.shr : fp2.shr);
  else if (itype==mkuni.asciimult)
    idx=fp1.shr+fp2.shr;
  else if (itype==mkuni.asciidiv) {
    idx=ll-(fp1.mag-corr-fp2.mag)-1;
    if (idx<0)
      return strres;
  }
  if (idx==0)
    return strres;
  if (idx<0)
    idx=-idx;
  var strarr=strres.split('');
  if (ll>idx)
    strarr.splice(ll-idx,0,mkuni.str4code(mkuni.asciidot));
  else {
    while (ll<idx) {
      strarr.unshift(mkuni.str4code(mkuni.asciizero));
      idx--;
    }
    strarr.unshift(mkuni.str4code(mkuni.asciidot));
  }
  var ans=strarr.join('');
  return ans;
};

/* ***** internal ***** */
mkfixed.add=function(strnum1,strnum2) {
  var ans=mkuni.strnan;
  strnum1=mkuni.chkstr(strnum1);
  strnum2=mkuni.chkstr(strnum2);
  var strarr1=strnum1.split('');
  var strarr2=strnum2.split('');
  var ii=0,jj=0,kk=0,l1=strarr1.length,l2=strarr2.length;
  if (l1==0 || l2==0)
    return ans;
  var c1=0,c2=0,c3=0,calc=0,carry=0;
  var strarr3=new Array(l1+1);
  // as doing on paper - walk from back to front adding digits and use carry in case
  for (ii=l1-1,jj=l2-1;ii>=0;ii--,jj--) {
    c1=strarr1[ii].charCodeAt(0)-mkuni.asciizero;
    c2=(jj<0 ? 0 : strarr2[jj].charCodeAt(0)-mkuni.asciizero);
    calc=c1+c2;
    if (calc>=10) {
      carry=1;
      calc-=10;
      kk=ii;
      while (carry>0 && kk>0) {
        kk--;
        c3=strarr1[kk].charCodeAt(0)-mkuni.asciizero+carry;
        if (c3>=10)
          c3-=10;
        else
          carry=0;
        strarr1[kk]=mkuni.str4code(c3+mkuni.asciizero);
      }
    }
    strarr3[ii+1]=mkuni.str4code(calc+mkuni.asciizero);
  }
  if (carry>0)
    strarr3[0]=mkuni.str4code(mkuni.asciione);
  else
    strarr3.shift();
  ans=mkint.stripleadingzeros(strarr3.join(''));
  if (ans.length==0)
    ans=mkuni.str4code(mkuni.asciizero);
  return ans;
};

/* ***** internal ***** */
mkfixed.substract=function(strnum1,strnum2) {
  var ans=mkuni.strnan;
  strnum1=mkuni.chkstr(strnum1);
  strnum2=mkuni.chkstr(strnum2);
  var strarr1=strnum1.split('');
  var strarr2=strnum2.split('');
  var ii=0,jj=0,kk=0,l1=strarr1.length,l2=strarr2.length;
  if (l1==0 || l2==0)
    return ans;
  var c1=0,c2=0,c3=0,calc=0,carry=0;
  var strarr3=new Array(l1);
  // as doing on paper - walk from back to front substracting digits and use carry in case
  for (ii=l1-1,jj=l2-1;ii>=0;ii--,jj--) {
    c1=strarr1[ii].charCodeAt(0)-mkuni.asciizero;
    c2=(jj<0 ? 0 : strarr2[jj].charCodeAt(0)-mkuni.asciizero);
    calc=c1-c2;
    if (calc<0) {
      carry=1;
      calc+=10;
      kk=ii;
      while (carry>0 && kk>0) {
        kk--;
        c3=strarr1[kk].charCodeAt(0)-mkuni.asciizero-carry;
        if (c3<0)
          c3+=10;
        else
          carry=0;
        strarr1[kk]=mkuni.str4code(c3+mkuni.asciizero);
      }
    }
    strarr3[ii]=mkuni.str4code(calc+mkuni.asciizero);
  }
  c3=strarr3[0].charCodeAt(0);
  while (c3==0) {
    strarr3.shift();
    c3=strarr3[0].charCodeAt(0);
  }
  ans=mkint.stripleadingzeros(strarr3.join(''));
  if (ans.length==0)
    ans=mkuni.str4code(mkuni.asciizero);
  return ans;
};

/* ***** internal ***** */
mkfixed.multiply=function(strnum1,strnum2) {
  var ans=mkuni.strnan;
  strnum1=mkuni.chkstr(strnum1);
  strnum2=mkuni.chkstr(strnum2);
  var strarr1=strnum1.split('');
  var strarr2=strnum2.split('');
  var ii=0,jj=0,kk=0,idx=0,l1=strarr1.length,l2=strarr2.length;
  if (l1==0 || l2==0)
    return ans;
  var c1=0,c2=0,c3=0,calc=0,carry=0;
  var strarr3=new Array(l1+l2);
  for (ii=0;ii<(l1+l2);ii++)
    strarr3[ii]='0';
  // as doing on paper - walk from back to front multiplying digits and use carry in case
  for (ii=l1-1;ii>-1;ii--) {
    c1=strarr1[ii].charCodeAt(0)-mkuni.asciizero;
    for (jj=l2-1;jj>-1;jj--) {
      idx=ii+jj+1;
      c2=strarr2[jj].charCodeAt(0)-mkuni.asciizero;
      c3=strarr3[idx].charCodeAt(0)-mkuni.asciizero;
      calc=c1*c2+c3;
      if (calc>9) {
        carry=parseInt(calc/10,10);
        calc-=(10*carry);
        kk=idx;
        while (carry>0) {
          kk--;
          c3=strarr3[kk].charCodeAt(0)-mkuni.asciizero;
          c3=c3+carry;
          if (c3>9) {
            carry=parseInt(c3/10,10);
            c3-=(10*carry);
          }
          else
            carry=0;
          strarr3[kk]=mkuni.str4code(c3+mkuni.asciizero);
        }
      }
      strarr3[idx]=mkuni.str4code(calc+mkuni.asciizero);
    }
  }
  ans=mkint.stripleadingzeros(strarr3.join(''));
  if (ans.length==0)
    ans=mkuni.str4code(mkuni.asciizero);
  return ans;
};

/* ***** internal ***** */
mkfixed.divide=function(strnum1,strnum2,corr,lmax) {
  var ans=mkuni.strnan;
  strnum1=mkuni.chkstr(strnum1);
  strnum2=mkuni.chkstr(strnum2);
  lmax=mkuni.chknumber(lmax,mkfixed.maxlen);
  var ii=0,jj=0,kk=0,idx=0,cmp=0,chklow=0,l1=strnum1.length,l2=strnum2.length;
  if (l1==0 || l2==0 || lmax<1)
    return ans;
  var zcmp=mkint.stripleadingzeros(strnum1);
  if (zcmp.length==0)
    return mkuni.str4code(mkuni.asciizero);
  zcmp=mkint.stripleadingzeros(strnum2);
  if (zcmp.length==0)
    return ans;
  var strarr3=new Array(lmax);
  var strslice,xxx,inter=mkuni.str4code(mkuni.asciizero);
  var mult=new Array(mkuni.str4code(mkuni.asciizero));
  // cannot do any division due to ieee floating point calculation so build
  // num1-slice of length num2 and try multiplications
  strslice=strnum1.slice(0,l2<l1 ? l2 : l1);
  while (kk<lmax) {
    if (chklow==0 && strslice.length==l2) {
      cmp=mkfixed.cmpstr10(strslice,strnum2);
      if (cmp<0 && (typeof corr!='undefined') && corr.hasOwnProperty('corr'))
        corr.corr=1;
      chklow=1;
    }
    // try 1,2,...10 * num2 break if result < slice
    for (ii=1;ii<10;ii++) {
      jj=ii;
      mult[0]=mkuni.str4code(ii+mkuni.asciizero);
      xxx=[].join('');
      xxx=mkfixed.multiply(strnum2,mult.join(''));
      cmp=mkfixed.cmpstr10(strslice,xxx);
      if (cmp<0) {
        jj--;
        break;
      }
      inter=[].join('');
      inter=xxx.slice();
      if (cmp==0)
        break;
    }
    // pile up resultmkfixed.chknum('7612.876345');
    strarr3[kk]=mkuni.str4code(jj+mkuni.asciizero);
    xxx=[].join('');
    // substract intermediate result from slice
    xxx=mkfixed.substract(strslice,inter);
    jj=xxx.length;
    zcmp=mkint.stripleadingzeros(xxx);
    if (zcmp.length==0) {
      if (l2+kk<l1)
        jj=0;
      else
        break;
    }
    if (jj==0)
      xxx=[].join('');
    // append next digit to slice from num1 or zero if exhausted and repeat
    xxx+=(l2+kk<l1 ? strnum1.charAt(l2+kk) : mkuni.str4code(mkuni.asciizero));
    strslice=[].join('');
    strslice=xxx.slice();
    inter=mkuni.str4code(mkuni.asciizero);
    kk++;
  }
  if (kk==lmax)
    kk--;
  ans=mkint.stripleadingzeros(strarr3.join(''));
  if (ans.length==0)
    ans=mkuni.str4code(mkuni.asciizero);
  return ans;
};

/* ********** */
mkfixed.calculate=function(calctype,num1,num2,lmax) {
  var ans=mkuni.strnan;
  var itype=mkfixed.chkcalctype(calctype);
  if (itype==0)
    return ans;
  lmax=mkuni.chknumber(lmax,mkfixed.maxlen);
  if (lmax<1)
    return ans;
  var fp1=new mkfixed.FixedPoint(num1);
  var fp2=new mkfixed.FixedPoint(num2);
  if (fp1.unmix()==0 || fp2.unmix()==0)
    return ans;
  var calc1=fp1.mant,calc2=fp2.mant;
  var ii=0,dfshr=fp1.shr-fp2.shr;
  for (ii=dfshr;(ii>0 && itype!=mkuni.asciimult);ii--)
    calc1+=mkuni.str4code(mkuni.asciizero);
  for (ii=dfshr;(ii<0 && itype!=mkuni.asciimult);ii++)
    calc2+=mkuni.str4code(mkuni.asciizero);
  var cmp=mkfixed.cmpstr10(calc1,calc2);
  var sgn1=(fp1.sgn==mkuni.asciiminus ? true : false);
  var sgn2=(fp2.sgn==mkuni.asciiminus ? true : false);
  var corr={'corr':0};
  if (itype==mkuni.asciimult) {
    ans=mkfixed.multiply(calc1,calc2);
  }
  else if (itype==mkuni.asciidiv) {
    ans=mkfixed.divide(calc1,calc2,corr,lmax);
  }
  else {
    if ((itype==mkuni.asciiplus && sgn1!=sgn2) || (itype==mkuni.asciiminus && sgn1==sgn2))
      ans=(cmp<0 ? mkfixed.substract(calc2,calc1) : mkfixed.substract(calc1,calc2));
    else
      ans=(cmp<0 ? mkfixed.add(calc2,calc1) : mkfixed.add(calc1,calc2));
  }
  if (ans==mkuni.strnan)
    return ans;
  ans=mkfixed.unfixcalc(calctype,ans,fp1,fp2,corr.corr);
  if (itype==mkuni.asciimult || itype==mkuni.asciidiv) {
    if (sgn1!=sgn2)
      ans=(mkuni.str4code(mkuni.asciiminus)+ans);
  }
  else if (itype==mkuni.asciiplus) {
    if ((sgn1 && sgn2) || (sgn1 && !sgn2 && cmp>0) || (sgn2 && !sgn1 && cmp<0))
      ans=(mkuni.str4code(mkuni.asciiminus)+ans);
  }
  else if (itype==mkuni.asciiminus) {
    if ((sgn1 && !sgn2) || (cmp<0 && sgn1==sgn2))
      ans=(mkuni.str4code(mkuni.asciiminus)+ans);
  }
  return ans;
};

/* ***** internal ***** */
mkfixed.calcmodulo=function(strnum1,strnum2,div,lmax) {
  var ans=mkuni.strnan;
  lmax=mkuni.chknumber(lmax,mkfixed.maxlen);
  if (lmax<1)
    return ans;
  strnum1=mkfixed.cutstr10(strnum1);
  strnum2=mkfixed.cutstr10(strnum2);
  if (strnum1.length==0 || strnum2.length==0)
    return ans;
  var domult=mkuni.str4code(mkuni.asciimult);
  var dodiv=mkuni.str4code(mkuni.asciidiv);
  var dosubs=mkuni.str4code(mkuni.asciiminus);
  var strcalc=mkfixed.calculate(dodiv,strnum1,strnum2,lmax);
  strcalc=mkfixed.cutstr10(strcalc);
  if ((typeof div!='undefined') && div.hasOwnProperty('div'))
    div.div=strcalc;
  if (strcalc==mkuni.str4code(mkuni.asciizero))
    ans=strnum1;
  else {
    strcalc=mkfixed.calculate(domult,strnum2,strcalc,lmax);
    ans=mkfixed.calculate(dosubs,strnum1,strcalc,lmax);
  }
  return ans;
};

/* ***** FixedPoint ***** */
mkfixed.FixedPoint.prototype.convert=function(baseto,lmax) {
  if (this.base==0)
    this.unmix();
  if (this.base<2)
    return this.base;
  var basefrom=this.base;
  baseto=mkint.chkbase(baseto,basefrom);
  if (basefrom==baseto || (basefrom==10 && this.isInt()==0))
    return this.base;
  var domult=mkuni.str4code(mkuni.asciimult);
  var doadd=mkuni.str4code(mkuni.asciiplus);
  var nsgn=this.toAbs();
  var cc=0,cmp=0,ii=0,jj=0,idx=-1;
  var strbasefrom=mkint.int2str(basefrom),strbaseto=mkint.int2str(baseto);
  var strnum=this.toStringI();
  var strnum10=mkuni.str4code(mkuni.asciizero);
  var strbaseacc=mkuni.str4code(mkuni.asciione);
  var strtmp='';
  if (basefrom==10)
    strnum10=strnum;
  else {
    for (ii=strnum.length-1;ii>-1;ii--)  {
      cc=strnum.charCodeAt(ii);
      for (jj=0;jj<basefrom;jj++) {
        if (cc==mkint.basecharcode[jj]) {
          idx=jj;
          break;
        }
      }
      strtmp=mkfixed.calculate(domult,mkint.int2str(idx),strbaseacc,lmax);
      strnum10=mkfixed.calculate(doadd,strnum10,strtmp,lmax);
      strbaseacc=mkfixed.calculate(domult,strbaseacc,strbasefrom,lmax);
    }
  }
  if (baseto==10) {
    this.compound=strnum10.toUpperCase();
    this.funmix();
    this.sgn=nsgn;
    return this.base;
  }
  var div={'div':mkuni.str4code(mkuni.asciizero)};
  var converted='';
  do {
    strtmp=mkfixed.calcmodulo(strnum10,strbaseto,div,lmax);
    if (strtmp==mkuni.strnan)
      return this.base;
    idx=mkint.int4str(strtmp);
    converted=mkuni.str4code(mkint.basecharcode[idx])+converted;
    strnum10=div.div;
    cmp=mkfixed.cmpstr10(strnum10,mkuni.str4code(mkuni.asciizero));
  } while (cmp==1);
  if (converted.length==0)
    converted=mkuni.str4code(mkuni.asciizero);
  this.compound=converted.toUpperCase();
  this.unmix(baseto);
  this.sgn=nsgn;
  return this.base;
};

/* ***** FixedPoint ***** */
mkfixed.FixedPoint.prototype.toStringF=function(needexp,lmax) {
  if (this.base>1 && this.base!=10) {
    var fp=this.copy();
    fp.convert(10,lmax);
    return fp.toStringF(needexp,lmax);
  }
  needexp=mkuni.chknumber(needexp,NaN);
  if (!isNaN(needexp) && needexp<mkfloat.fabs(this.mag))
    return this.toString();
  if (this.base==0)
    this.unmix();
  if (this.base==0)
    return mkuni.strnan;
  if (this.base==1)
    return mkuni.str4code(mkuni.asciizero);
  var iexp=this.mag;
  var intgarr=this.intg.split('');
  var fracarr=this.frac.split('');
  if (iexp<0) {
    while (intgarr.length>0 && iexp++<0)
      fracarr.unshift(intgarr.pop());
    while (iexp++<0)
      fracarr.unshift(mkuni.str4code(mkuni.asciizero));
  }
  else if (iexp>0) {
    while (fracarr.length>0 && iexp-->0)
      intgarr.push(fracarr.shift());
    while (iexp-->0)
      intgarr.push(mkuni.str4code(mkuni.asciizero));
  }
  var ans=mkuni.str4code(this.sgn);
  if (intgarr.length==0)
    ans+=mkuni.str4code(mkuni.asciizero);
  else
    ans+=intgarr.join('');
  if (fracarr.length>0) {
    ans+=mkuni.str4code(mkuni.asciidot);
    ans+=fracarr.join('');
  }
  return ans;
};

/* ********** */
mkfixed.chkfixedpoint=function(fp) {
  var ans=mkuni.bad;
  if ((typeof fp!='undefined') && (fp instanceof mkfixed.FixedPoint))
    ans=mkuni.good;
  return ans;
};

/* ********** */
/* ********** */
/* ********** */
mkfixed.docalc=function(calctype,num1,num2,lmax) {
  var ans=mkfixed.calculate(calctype,num1,num2,lmax);
  console.log(new Error().lineNumber+' '+num1+' '+num2+' '+ans);
};

mkfixed.round=function(num1,num2) {
  var fp=new mkfixed.FixedPoint(num1);
  var ff=fp.toStringF();
  console.log(new Error().lineNumber+' '+fp.toJson()+' '+ff);
  var rd=fp.roundpos(num2);
  console.log(new Error().lineNumber+' '+rd.toJson()+' '+rd.toStringF());
};

mkfixed.calcconvert=function(num1,num2,num3,lmax) {
  var fp=new mkfixed.FixedPoint(num1);
  fp.unmix(num2);
  fp.convert(num3);
  console.log(new Error().lineNumber+' '+fp.toJson()+' '+fp.toStringF(7));
};

mkfixed.chkfp=function(num,base) {
  var fp=new mkfixed.FixedPoint(num);
  fp.unmix(base);
  console.log(new Error().lineNumber+' '+
    fp.toJson()+'\n['+fp.toStringF(7)+'] ['+fp.toStringI()+'] ['+fp.isInt()+']');
  console.log(new Error().lineNumber+' '+fp.toJson());
};














































































