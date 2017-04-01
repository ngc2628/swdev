
var textcalc={

asciispace: Uint16Array([32]), asciihash: Uint16Array([35]), asciiplus: Uint16Array([43]),
asciiminus: Uint16Array([45]), asciidec: Uint16Array([46]), asciizero: Uint16Array([48]),
asciione: Uint16Array([49]), asciitwo: Uint16Array([50]), asciithree: Uint16Array([51]),
asciifour: Uint16Array([52]), asciifive: Uint16Array([53]), asciisix: Uint16Array([54]),
asciiseven: Uint16Array([55]), asciieight: Uint16Array([56]), asciinine: Uint16Array([57]),
asciiA: Uint16Array([65]), asciiB: Uint16Array([66]), asciiC: Uint16Array([67]),
asciiD: Uint16Array([68]), asciiE: Uint16Array([69]), asciiF: Uint16Array([70]),
asciiG: Uint16Array([71]), asciiH: Uint16Array([72]), asciiI: Uint16Array([73]),
asciiJ: Uint16Array([74]), asciiK: Uint16Array([75]), asciiL: Uint16Array([76]),
asciiM: Uint16Array([77]), asciiN: Uint16Array([78]), asciiO: Uint16Array([79]),
asciiP: Uint16Array([80]), asciiQ: Uint16Array([81]), asciiR: Uint16Array([82]),
asciiS: Uint16Array([83]), asciiT: Uint16Array([84]), asciiU: Uint16Array([85]),
asciiV: Uint16Array([86]), asciiW: Uint16Array([87]), asciiX: Uint16Array([88]),
asciiY: Uint16Array([89]), asciiZ: Uint16Array([90]), ascii_: Uint16Array([95]),
asciie: Uint16Array([101]), asciif: Uint16Array([102]), asciii: Uint16Array([105]),
asciix: Uint16Array([120]), asciidel: Uint16Array([127]),

maxbase: 37,

basecharcode: Uint16Array([48,49,50,51,52,53,54,55,56,57,65,66,67,68,
69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,95]),

/** ********************** helpers *************************/

zeroarr: function(arr,begend) {

  var ii=0,beg=begend[0],end=begend[1];
  if (beg<0)
    beg=0;
  if (end<0 || end>=arr.length)
    end=arr.length;
  for (ii=beg;ii<end;ii++)
    arr[ii]='0';

},

trimint10: function(str) {

  var istr=str.trim().toUpperCase();

  var ll=istr.length;
  if (ll==0)
    return '0';
  var ii=0,ee=ll,ss=0;
  var cc=new Uint16Array([0]);

  for (ii=0;ii<ll;ii++) {
    cc[0]=istr.charCodeAt(ii);
    if (cc[0]==this.asciiminus[0] || cc[0]<=this.asciiplus[0]) {
      ss=(cc[0]==this.asciiminus[0] ? ii : ii+1);
      break;
    }
  }

  if (ss>=ee)
    return '0';

  for (ii=ss;ii<ll;ii++) {
    cc[0]=istr.charCodeAt(ii);
    if (cc[0]>this.asciizero[0] && cc[0]<=this.asciinine[0])
      break;
    ss++;
  }
  for (ii=ll-1;ii>=ss;ii--) {
    cc[0]=istr.charCodeAt(ii);
    if (cc[0]>=this.asciizero[0] && cc[0]<=this.asciinine[0])
      break;
    ee--;
  }

  if (ee<=ss)
    return '0';
  else if (ss==0 && ee==ll)
    return istr;

  return istr.slice(ss,ee);

},

txtswap: function(c1c2) {

  var tmp=c1c2[0];
  c1c2[0]=c1c2[1];
  c1c2[1]=tmp;

},

txtsimplei102str: function(number) {

  var ans=[];
  var inum=new Int32Array([number,0]);
  var sgn=new Int16Array([inum[0]<0 ? 1 : 0]);
  if (sgn[0]==1)
    inum[0]=-inum[0];
  do {
    inum[1]=inum[0]%10;
    ans.push(String.fromCharCode(this.basecharcode[inum[1]]));
    inum[0]=inum[0]/10;
  } while (inum[0]>0);
  if (sgn[0]==1)
    ans.push('-');

  return ans.reverse().join('');

},

txtstr2simplei10: function(str) {

  var strarr=str.trim().toUpperCase().split('');
  var inum=new Uint32Array([0,1]);
  var sgn=new Int16Array([1]);
  var cc=new Uint16Array([strarr[0].charCodeAt(0)]);

  if (cc[0]==this.asciiminus[0])
    sgn[0]=-1;

  var ii=0,jj=0,idx=-1,ll=strarr.length;

  for (ii=ll-1;ii>(sgn[0]==1 ? -1 : 0);ii--)  {
    cc[0]=strarr[ii].charCodeAt(0);
    for (jj=0;jj<10;jj++) {
      if (cc[0]==this.basecharcode[jj]) {
        idx=jj;
        break;
      }
    }
    inum[0]+=idx*inum[1];
    inum[1]*=10;
  }

  return (sgn[0]*inum[0]);

},

txt2float: function(str) {

  if (str.length==0)
    return "NaN";

  var strarr=str.trim().split('');
  var ii=0,dpos=-1,epos=-1,mpos=-1,pos=-1,ll=strarr.length,llm=0,lle=0,sgn=0,esgn=0;
  var mant=[];
  var exp=[];
  var cc=new Uint16Array([strarr[0].charCodeAt(0)]);

  // tolerance - leading crap ignored
  while (cc[0]!=this.asciie[0] && cc[0]!=this.asciiE[0] && cc[0]!=this.asciidec[0] &&
         cc[0]!=this.asciiminus[0] && cc[0]!=this.asciiplus[0] &&
         (cc[0]<this.asciizero[0] || cc[0]>this.asciinine[0])) {
    strarr.shift();
    if (--ll==0)
      return "NaN";
    cc[0]=strarr[0].charCodeAt(0);
  }

  // tolerance - trailing crap ignored
  cc[0]=strarr[ll-1].charCodeAt(0);
  while (cc[0]<this.asciizero[0] || cc[0]>this.asciinine[0]) {
    strarr.pop();
    if (--ll==0)
      return "NaN";
    cc[0]=strarr[ll-1].charCodeAt(0);
  }

  // strip front sign
  cc[0]=strarr[0].charCodeAt(0);
  if (cc[0]==this.asciiminus[0] || cc[0]==this.asciiplus[0]) {
    if (cc[0]==this.asciiminus[0])
      sgn=1;
    strarr.shift();
    if (--ll==0)
      return "NaN";
  }

  // find exp position
  for (ii=0;ii<ll;ii++) {
    cc[0]=strarr[ii].charCodeAt(0);
    if (cc[0]==this.asciie[0] || cc[0]==this.asciiE[0]) {
      epos=ii;
      break;
    }
  }
  if (epos==ll-1) {
    strarr.pop();
    if (--ll==0)
      return "NaN";
    epos=-1;
  }

  mpos=(epos>=0 ? epos : ll);
  // find dec position
  for (ii=0;ii<mpos;ii++) {
    cc[0]=strarr[ii].charCodeAt(0);
    if (cc[0]==this.asciidec[0]) {
      dpos=ii;
      break;
    }
  }

  // stop conversion at mantissa non-digit
  pos=(dpos>=0 ? dpos : mpos);
  for (ii=0;ii<pos;ii++) {
    cc[0]=strarr[ii].charCodeAt(0);
    if (cc[0]<this.asciizero[0] || cc[0]>this.asciinine[0]) {
      strarr.splice(ii,ll-ii);
      ll=ii;
      if (ll==0)
        return "NaN";
      dpos=epos=-1;
      mpos=ll;
      break;
    }
  }
  pos=(dpos>=0 ? dpos+1 : 0);
  for (ii=pos;ii<mpos;ii++) {
    cc[0]=strarr[ii].charCodeAt(0);
    if (cc[0]<this.asciizero[0] || cc[0]>this.asciinine[0]) {
      strarr.splice(ii,ll-ii);
      ll=ii;
      if (ll==0)
        return "NaN";
      epos=-1;
      mpos=ll;
      break;
    }
  }

  // alloc mant and exp
  if (epos<0)
    mant=strarr;
  else {
    exp=strarr.slice(epos+1,ll);
    if (epos>0)
      mant=strarr.slice(0,epos);
  }

  llm=mant.length;
  lle=exp.length;

  // strip exp-sign
  if (lle>0) {
    cc[0]=exp[0].charCodeAt(0);
    if (cc[0]==this.asciiminus[0] || cc[0]==this.asciiplus[0]) {
      if (cc[0]==this.asciiminus[0])
        esgn=1;
      exp.shift();
      lle--;
    }
  }

  // discard all from exp non-digit
  for (ii=0;ii<lle;ii++) {
    cc[0]=exp[ii].charCodeAt(0);
    if (cc[0]<this.asciizero[0] || cc[0]>this.asciinine[0]) {
      exp.splice(ii,lle-ii);
      lle=ii;
      break;
    }
  }

  // strip exp leading zeros
  if (lle>0) {
    cc[0]=exp[0].charCodeAt(0);
    while (cc[0]==this.asciizero[0]) {
      exp.shift();
      if (--lle==0)
        break;
      cc[0]=exp[0].charCodeAt(0);
    }
  }

  // dec as only mantissa char may not result to 0 for exp ??
  if (llm==1) {
    cc[0]=mant[0].charCodeAt(0);
    if (cc[0]==this.asciidec[0]) {
      if (lle==0)
        return "NaN";
      mant.splice(0,1,'1');
      dpos=-1;
    }
  }

  // strip mantissa leading zeros
  if (llm>0) {
    cc[0]=mant[0].charCodeAt(0);
    while (cc[0]==this.asciizero[0]) {
      mant.shift();
      if (--llm==0)
        return "0";
      cc[0]=mant[0].charCodeAt(0);
    }
  }

  // strip mantissa trailing zeros
  if (llm>0 && dpos>=0) {
    cc[0]=mant[llm-1].charCodeAt(0);
    while (cc[0]==this.asciizero[0]) {
      mant.pop();
      llm--;
      cc[0]=mant[llm-1].charCodeAt(0);
    }
    // dec at last mantissa char is useless
    cc[0]=mant[llm-1].charCodeAt(0);
    if (cc[0]==this.asciidec[0]) {
      mant.pop();
      llm--;
    }
  }

  // only exp or nothing is left
  if (llm==0) {
    if (lle==0)
      return "0";
    mant.push('1');
    llm++;
  }

  if (lle>0) {
    mant.push('e');
    llm++;
    if (esgn==1) {
      mant.push('-');
      llm++;
    }
    for (ii=0;ii<lle;ii++) {
      mant.push(exp[ii]);
      llm++;
    }
  }

  // prepend - in case
  if (sgn==1)
    llm=mant.unshift('-');

  return mant.join('');

},

txtabs: function(str,qqszsgnfe) {

  var strnum=this.txt2float(str);
  if (strnum=="NaN")
    return strnum;

  var strarr=strnum.split('');
  var ll=strarr.length;
  var mant=[];
  var exp=[];
  var cc=new Uint16Array([strarr[0].charCodeAt(0)]);
  var esz=new Uint32Array([1]);
  var ii=0,dpos=-1,epos=-1,mpos=-1,pos=-1,llm=0,lle=0,esgn=0;

  // strip front sign
  if (cc[0]==this.asciiminus[0]) {
    qqszsgnfe[2]=1;
    strarr.shift();
    ll--;
  }

  // find exp position
  for (ii=0;ii<ll;ii++) {
    cc[0]=strarr[ii].charCodeAt(0);
    if (cc[0]==this.asciie[0]) {
      epos=ii;
      break;
    }
  }

  // alloc mant and exp
  if (epos<0)
    mant=strarr;
  else {
    qqszsgnfe[3]=1;
    exp=strarr.slice(epos+1,ll);
    if (epos>0)
      mant=strarr.slice(0,epos);
  }

  llm=mant.length;
  lle=exp.length;

  // find dec position
  for (ii=0;ii<llm;ii++) {
    cc[0]=mant[ii].charCodeAt(0);
    if (cc[0]==this.asciidec[0]) {
      dpos=ii;
      break;
    }
  }

  if (lle>0) {
    cc[0]=exp[0].charCodeAt(0);
    if (cc[0]==this.asciiminus[0]) {
      esgn=1;
      exp.shift();
      lle--;
    }
    for (ii=lle-1;ii>-1;ii--) {
      cc[0]=exp[ii].charCodeAt(0);
      qqszsgnfe[0]+=esz[0]*(cc[0]-this.asciizero[0]);
      esz[0]*=10;
    }
    if (esgn==1)
      qqszsgnfe[0]=-qqszsgnfe[0];
  }
  qqszsgnfe[1]=qqszsgnfe[0];

  if (dpos<0)
    qqszsgnfe[1]=(qqszsgnfe[0]+llm-1);
  else {
    if (dpos>1)
      qqszsgnfe[1]+=(dpos-1);
    else {
      cc[0]=(dpos==1 ? mant[0].charCodeAt(0) : this.asciizero[0]);
      if (cc[0]==this.asciizero[0]) {
        qqszsgnfe[1]-=1;
        for (ii=dpos+1;ii<llm;ii++) {
          cc[0]=mant[ii].charCodeAt(0);
          if(cc[0]!=this.asciizero[0])
            break;
          qqszsgnfe[1]-=1;
        }
      }
    }
    qqszsgnfe[0]-=(llm-dpos-1);
    mant.splice(dpos,1);
    llm--;
  }

  cc[0]=mant[0].charCodeAt(0);
  while (cc[0]==this.asciizero[0]) {
    mant.shift();
    if (--llm==0)
      return '0';
    cc[0]=mant[0].charCodeAt(0);
  }

  if (qqszsgnfe[0]>0) {
    for (ii=0;ii<qqszsgnfe[0];ii++) {
      mant.push('0');
      llm++;
    }
    qqszsgnfe[0]=0;
  }

  return mant.join('');

},

txtfixqq: function(calctype,qq12,strnum12) {

  var dfqq=new Int32Array([qq12[0]-qq12[1]]);

  if (calctype=='*' || dfqq==0)
    return;

  var ii=0,mod=0;

  if (dfqq[0]<0) {
    dfqq[0]=-dfqq[0];
    mod=1;
  }

  var strarr=strnum12[mod].split('');
  for (ii=0;ii<dfqq[0];ii++)
    strarr.push('0');

  strnum12[mod]=this.trimint10(strarr.join(''));

},

txtunfixqq: function(calctype,strnum,sz12,qq12) {

  var qq=new Int32Array([0]);
  var ii=0,ll=strnum.length;

  if (calctype=='+' || calctype=='-')
    qq[0]=Math.abs(Math.min(qq12[0],qq12[1]))
  else if (calctype=='*')
    qq[0]=Math.abs(qq12[0]+qq12[1]);
  else if (calctype=='/')
    qq[0]=(ll-(sz12[0]-sz12[1])-1);

  if (qq[0]<=0)
    return strnum;

  var strarr=strnum.split('');
  ll=strarr.length;

  if (ll>qq[0])
    strarr.splice(ll-qq[0],0,'.');
  else {
    while (ll<qq[0]) {
      strarr.unshift('0');
      qq[0]-=1;
    }
    strarr.unshift('.');
  }

  return strarr.join('');

},

txtcmp: function(strnum1,strnum2) {

  var l1=strnum1.length,l2=strnum2.length;

  if (l2<l1)
    return 1;
  else if (l1<l2)
    return -1;
  else if (l1==0 && l2==0)
    return 0;

  var strarr1=strnum1.split('');
  var strarr2=strnum2.split('');
  var c1=new Uint16Array([0]);
  var c2=new Uint16Array([0]);
  var ii=0;

  for (ii=0;ii<l1;ii++) {
    c1[0]=strarr1[ii].charCodeAt(0);
    c2[0]=strarr2[ii].charCodeAt(0);
    if (c1[0]<c2[0])
      return -1;
    if (c2[0]<c1[0])
      return 1;
  }

  return 0;

},

txtadd: function(strnum1,strnum2) {

  var strarr1=strnum1.split('');
  var strarr2=strnum2.split('');
  var ii=0,jj=0,kk=0,l1=strarr1.length,l2=strarr2.length;
  var c1=new Uint16Array([0]);
  var c2=new Uint16Array([0]);
  var c3=new Uint16Array([0]);
  var strarr3=new Array(l1+1);
  var calc=new Uint16Array([0]);
  var carry=new Uint16Array([0]);

  for (ii=l1-1,jj=l2-1;ii>=0;ii--,jj--) {
    c1[0]=strarr1[ii].charCodeAt(0)-this.asciizero[0];
    c2[0]=(jj<0 ? 0 : strarr2[jj].charCodeAt(0)-this.asciizero[0]);
    calc[0]=c1[0]+c2[0];
    if (calc[0]>=10) {
      carry[0]=1;
      calc[0]-=10;
      kk=ii;
      while (carry[0]>0 && kk>0) {
        kk--;
        c3[0]=strarr1[kk].charCodeAt(0)-this.asciizero[0]+carry[0];
        if (c3[0]>=10)
          c3[0]-=10;
        else
          carry[0]=0;
        strarr1[kk]=String.fromCharCode(c3[0]+this.asciizero[0]);
      }
    }
    strarr3[ii+1]=String.fromCharCode(calc[0]+this.asciizero[0]);
  }
  if (carry[0]>0)
    strarr3[0]='1';
  else strarr3.shift();

  return this.trimint10(strarr3.join(''));

},

txtsubs: function(strnum1,strnum2) {

  var strarr1=strnum1.split('');
  var strarr2=strnum2.split('');
  var ii=0,jj=0,kk=0,l1=strarr1.length,l2=strarr2.length;
  var c1=new Uint16Array([0]);
  var c2=new Uint16Array([0]);
  var c3=new Uint16Array([0]);
  var strarr3=new Array(l1);
  var calc=new Int16Array([0]);
  var carry=new Uint16Array([0]);

  for (ii=l1-1,jj=l2-1;ii>=0;ii--,jj--) {
    c1[0]=strarr1[ii].charCodeAt(0)-this.asciizero[0];
    c2[0]=(jj<0 ? 0 : strarr2[jj].charCodeAt(0)-this.asciizero[0]);
    calc[0]=c1[0]-c2[0];
    if (calc[0]<0) {
      carry[0]=1;
      calc[0]+=10;
      kk=ii;
      while (carry[0]>0 && kk>0) {
        kk--;
        c3[0]=strarr1[kk].charCodeAt(0)-this.asciizero[0]-carry[0];
        if (c3[0]<0)
          c3[0]+=10;
        else
          carry[0]=0;
        strarr1[kk]=String.fromCharCode(c3[0]+this.asciizero[0]);
      }
    }
    strarr3[ii]=String.fromCharCode(calc[0]+this.asciizero[0]);
  }
  c3[0]=strarr3[0].charCodeAt(0);
  while (c3[0]==0) {
    strarr3.shift();
    c3[0]=strarr3[0].charCodeAt(0);
  }

  return this.trimint10(strarr3.join(''));

},

txtmult: function(strnum1,strnum2) {

  var strarr1=strnum1.split('');
  var strarr2=strnum2.split('');
  var ii=0,jj=0,kk=0,idx=0,l1=strarr1.length,l2=strarr2.length;
  var c1=new Uint16Array([0]);
  var c2=new Uint16Array([0]);
  var c3=new Uint16Array([0]);
  var strarr3=new Array(l1+l2);
  var calc=new Int16Array([0]);
  var carry=new Uint16Array([0]);

  for (ii=0;ii<(l1+l2);ii++)
    strarr3[ii]='0';

  for (ii=l1-1;ii>-1;ii--) {
    c1[0]=strarr1[ii].charCodeAt(0)-this.asciizero[0];
    for (jj=l2-1;jj>-1;jj--) {
      idx=ii+jj+1;
      c2[0]=strarr2[jj].charCodeAt(0)-this.asciizero[0];
      c3[0]=strarr3[idx].charCodeAt(0)-this.asciizero[0];
      calc[0]=c1[0]*c2[0]+c3[0];
      if (calc[0]>9) {
        carry[0]=calc[0]/10;
        calc[0]-=(carry[0]*10);
        kk=idx;
        while (carry[0]>0) {
          kk--;
          c3[0]=strarr3[kk].charCodeAt(0)-this.asciizero[0];
          c3[0]+=carry[0];
          if (c3[0]>9) {
            carry[0]=c3[0]/10;
            c3[0]-=(carry[0]*10);
          }
          else
            carry[0]=0;
          strarr3[kk]=String.fromCharCode(c3[0]+this.asciizero[0]);
        }
      }
      strarr3[idx]=String.fromCharCode(calc[0]+this.asciizero[0]);
    }
  }

  return this.trimint10(strarr3.join(''));

},

txtdiv: function(strnum1,strnum2,maxlen,corrsz) {

  var ii=0,jj=0,kk=0,idx=0,cmp=0,chklow=0,l1=strnum1.length,l2=strnum2.length;
  var imaxlen=new Uint32Array([maxlen]);
  var zcmp=new Array();
  var begend=new Int16Array([-1,-1]);

  zcmp.length=l1;
  this.zeroarr(zcmp,begend);
  cmp=this.txtcmp(strnum1,zcmp.join(''));
  if (cmp==0)
    return '0';
  zcmp.length=l2;
  this.zeroarr(zcmp,begend);
  cmp=this.txtcmp(strnum2,zcmp.join(''));
  if (cmp==0 || imaxlen[0]==0)
    return 'NaN';

  var strarr3=new Array(imaxlen[0]);
  var strslice,xxx,inter='0';
  var mult=new Array(['0']);

  this.zeroarr(strarr3,begend);
  strslice=strnum1.slice(0,l2<l1 ? l2 : l1);

  while (kk<imaxlen[0]) {
    if (chklow==0 && strslice.length==l2) {
      cmp=this.txtcmp(strslice,strnum2);
      if (cmp<0)
        corrsz[0]=1;
      chklow=1;
    }
    for (ii=1;ii<10;ii++) {
      jj=ii;
      mult[0]=String.fromCharCode(ii+this.asciizero[0]);
      xxx=[].join('');
      xxx=this.txtmult(strnum2,mult.join(''));
      cmp=this.txtcmp(strslice,xxx);
      if (cmp<0) {
        jj--;
        break;
      }
      inter=[].join('');
      inter=xxx.slice();
      if (cmp==0)
        break;
    }
    strarr3[kk]=String.fromCharCode(jj+this.asciizero[0]);
    xxx=[].join('');
    xxx=this.txtsubs(strslice,inter);
    zcmp.length=xxx.length;
    this.zeroarr(zcmp,begend);
    cmp=this.txtcmp(xxx,zcmp.join(''));
    jj=xxx.length;
    if (cmp==0) {
      if (l2+kk<l1)
        jj=0;
      else
        break;
    }
    if (jj==0)
      xxx=[].join('');

    xxx+=(l2+kk<l1 ? strnum1.charAt(l2+kk) : '0');
    strslice=[].join('');
    strslice=xxx.slice();
    inter='0';
    kk++;
  }

  if (kk==imaxlen[0])
    kk--;

  return this.trimint10(strarr3.join('').substr(0,kk+1));

},

txtround: function(strnum,pos) {

  var qqszsgnfe=new Int32Array([0,0,0,0]);
  var snum=this.txtabs(strnum,qqszsgnfe);

  if (snum==='NaN' || snum==='0')
    return snum;

  var ii=0,ll=0,llrd=snum.length,sz=qqszsgnfe[1];
  var ipos=new Int32Array([pos]);
  var rdpos=new Int32Array([ipos[0]+sz]);

  if (rdpos[0]>(llrd-2))
    return strnum;
  if (rdpos[0]<-1)
    return '0';

  var rdstrarr=snum.split('');
  llrd=rdstrarr.length;
  ii=rdpos[0]+1;
  var cc=new Uint16Array([rdstrarr[ii].charCodeAt(0)-this.asciizero[0]+5]);

  while (cc[0]>9 && ii>0) {
    ii--;
    cc[0]=rdstrarr[ii].charCodeAt(0)-this.asciizero[0]+1;
    if (cc[0]<10)
      rdstrarr[ii]=String.fromCharCode(cc[0]+this.asciizero[0]);
    else
      rdstrarr[ii]='0';
  }
  if (cc[0]==10)
    sz++;
  rdstrarr.splice(rdpos[0]+1,llrd-rdpos[0]-1);
  llrd=rdpos[0]+1;

  var ans=[];

  if (sz<0) {
    ans.push('0');
    ans.push('.');
    ll+=2;
    for (ii=-1;ii>sz;ii--) {
      ans.push('0');
      ll++;
    }
    if (cc[0]==10)
      ans.push('1');
    else
      ans=ans.concat(rdstrarr);
  }
  else {
    if (cc[0]==10) {
      ans.push('1');
      sz--;
    }
    for (ii=0;ii<=sz;ii++) {
      if (ii>=llrd)
        ans.push('0');
      else
        ans.push(rdstrarr[ii]);
    }
    ans.push('.');
    for (ii=sz+1;ii<llrd;ii++)
      ans.push(rdstrarr[ii]);
  }
  ll=ans.length;

  if (ans[ll-1]=='.')
    ans.pop();
  else if (ll>1) {
    cc[0]=ans[1].charCodeAt(0);
    if (cc[0]==this.asciidec[0]) {
      cc[0]=ans[0].charCodeAt(0);
      if (cc[0]==this.asciizero[0])
        ans.shift();
    }
  }

  if (qqszsgnfe[2]>0)
    ans.unshift('-');

  return ans.join('');

},

txtsimple2exp: function(str) {

  var strnum=this.txt2float(str);
  if (strnum=="NaN")
    return "NaN";

  var strarr=str.split('');
  var ii=0,dpos=-1,epos=-1,sc=0,scpos=-1,sgn=0,ll=strarr.length;
  var cc=new Uint16Array([0]);

  for (ii=0;ii<ll;ii++) {
    cc[0]=strarr[ii].charCodeAt(0);
    if (cc[0]==this.asciiminus[0])
      sgn=1;
    else if (cc[0]==this.asciidec[0])
      dpos=ii;
    else if (cc[0]==this.asciie[0] || cc[0]==this.asciiE[0]) {
      epos=ii;
      break;
    }
  }

  if (epos>=0)
    return strnum;

  if (sgn==1) {
    strarr.shift();
    ll--;
    dpos--;
  }

  if (dpos<0)
    sc=ll-1;
  else if (dpos>1)
    sc=dpos-1;
  else if (dpos==0) {
    for (ii=dpos+1;ii<ll;ii++) {
      sc--;
      cc[0]=strarr[ii].charCodeAt(0);
      if (cc[0]!=this.asciizero[0]) {
        scpos=ii;
        break;
      }
    }
  }

  if (sc==0)
    return strnum;

  if (sc>0) {
    if (dpos>=0) {
      strarr.splice(dpos,1);
      ll--;
    }
  }
  else {
    strarr.splice(0,scpos);
    ll-=scpos;
  }

  if (ll>1) {
    strarr.splice(1,0,'.');
    ll++;
  }

  for (ii=ll-1;ii>-1;ii--) {
    cc[0]=strarr[ii].charCodeAt(0);
    if (cc[0]!=this.asciizero[0]) {
      if (ii<ll-1)
        strarr.splice(ii+1,ll-(ii+1));
      break;
    }
  }

  if (sgn==1)
    strarr.unshift('-');
  strarr.push('E');

  return (strarr.join('')+this.txtsimplei102str(sc));

},

txtexp2simple: function(str) {

  var strnum=this.txt2float(str);
  if (strnum=="NaN")
    return "NaN";

  var strarr=str.split('');
  var ii=0,dpos=-1,epos=-1,sc=0,sgn=0,ll=strarr.length,lle=0,llm=0;
  var mant=[];
  var exp=[];
  var ans=[];
  var cc=new Uint16Array([0]);
  var expnum=new Int32Array([0]);

  for (ii=0;ii<ll;ii++) {
    cc[0]=strarr[ii].charCodeAt(0);
    if (cc[0]==this.asciiminus[0])
      sgn=1;
    else if (cc[0]==this.asciie[0] || cc[0]==this.asciiE[0]) {
      epos=ii;
      break;
    }
  }

  if (epos<0)
    return str;

  if (epos==(ll-1))
    return str.substr(0,ll-1);

  if (sgn==1) {
    strarr.shift();
    ll--;
    epos--;
  }

  // alloc mant and exp
  if (epos>0)
    mant=strarr.slice(0,epos);
  llm=mant.length;
  exp=strarr.slice(epos+1,ll);
  expnum[0]=this.txtstr2simplei10(exp.join(''));

  for (ii=0;ii<llm;ii++) {
    cc[0]=mant[ii].charCodeAt(0);
    if (cc[0]==this.asciidec[0]) {
      dpos=ii;
      break;
    }
  }

  if (llm==1 && dpos==0)
    return '0';
  if (llm==0) {
    mant.push('1');
    llm++;
  }

  if (dpos<0)
    sc+=llm;
  else {
    if (dpos>0)
      sc+=dpos;
    else {
      sc--;
      cc[0]=mant[1].charCodeAt(0);
      while (cc[0]==this.asciizero[0]) {
        mant.splice(1,1);
        llm--;
        sc--;
        cc[0]=mant[1].charCodeAt(0);
      }
    }
    mant.splice(dpos,1);
    llm--;
  }
  sc+=expnum[0];

  if (sc<0) {
    sc=-sc;
    ans.length=sc;
    for (ii=0;ii<sc;ii++)
      ans[ii]='0';
    ans=ans.concat(mant);
    ans.splice(1,0,'.');
  }
  else {
    ans=mant.slice(0,llm);
    if (llm<sc) {
      ans.length=(llm+sc);
      for (ii=llm;ii<sc;ii++)
        ans[ii]='0';
    }
    else if (sc<llm)
      ans.splice(sc,0,'.');
  }

  if (sgn==1)
    ans.unshift('-');

  return ans.join('');

},

txtmod: function(strnum1,strnum2,maxlen) {

  var ll1=strnum1.length,ll2=strnum2.length,sgn1=0,sgn2=0;
  var cc=new Uint16Array([strnum1[0].charCodeAt(0)]);
  if (cc[0]==this.asciiminus[0])
    sgn1=1;
  cc[0]=strnum2[0].charCodeAt(0);
  if (cc[0]==this.asciiminus[0])
    sgn2=1;

  var ans=new Array(strnum1.substr(sgn1,ll1-sgn1),strnum2.substr(sgn2,ll2-sgn2));

  var strcalc=this.txtcalc('/',ans[0],ans[1],maxlen);
  var ii=0,dpos=-1,ll=strcalc.length;
  ll1=ans[0].length;
  ll2=ans[1].length;

  cc[0]=strcalc[0].charCodeAt(0);
  if (cc[0]==this.asciizero[0] || cc[0]==this.asciidec[0]) {
    ans[1]=ans[0].substr(0,ll1);
    ans[0]=(sgn1!=sgn2 ? '-0' : '0');
    if (sgn1>0)
      ans[1].unshift('-');
    return ans;
  }

  for (ii=0;ii<ll;ii++) {
    cc[0]=strcalc[ii].charCodeAt(0);
    if (cc[0]==this.asciidec[0]) {
      dpos=ii;
      break;
    }
  }

  if (dpos<0) {
    ans[0]=strcalc.substr(0,ll);
    ans[1]='0';
    if (sgn1!=sgn2)
      ans[0].unshift('-');
    return ans;
  }

  var istrcalc=strcalc.substr(0,dpos);
  strcalc=this.txtcalc('*',ans[1],istrcalc,0);
  ans[1]=this.txtcalc('-',ans[0],strcalc,0);
  ans[0]=istrcalc.substr(0);
  if (sgn1!=sgn2)
    ans[0].unshift('-');

  return ans;

},

txtchknumfmt: function(strnum,pref) {

  var strarr=strnum.trim().toUpperCase().split('');
  var ii=0,jj=0,chk=0,ll=strarr.length;
  var ans=new Int8Array([-1,-1]);
  if (ll==0)
    return ans[0];
  if (pref)
    ans[1]=pref;
  var cc=new Uint16Array([0]);

  if (ans[1]<2 || ans[1]>=this.maxbase)
    ans[1]=-1;

  if (cc[0]==this.asciiminus[0]) {
    strarr.shift();
    if (--ll==0)
      return ans[0];
  }

  cc[0]=strarr[0].charCodeAt(0);
  if (cc[0]==this.asciizero[0]) {
    strarr.shift();
    if (--ll==0) {
      ans[0]=0;
      return ans[0];
    }
    cc[0]=strarr[0].charCodeAt(0);
    if (cc[0]==this.asciiX[0]) {
      if (ans[1]==-1)
        ans[1]=16;
      if (ans[1]==16) {
        strarr.shift();
        ll--;
      }
    }
    else if (ans[1]==-1)
      ans[1]=8;
  }
  else if (cc[0]==this.asciihash[0]) {
    if (ans[1]==-1)
      ans[1]=16;
    if (ans[1]==16) {
      strarr.shift();
      ll--;
    }
  }

  if (ll==0) {
    ans[0]=-1;
    return ans[0];
  }

  ans[0]=0;
  for (ii=0;ii<ll;ii++) {
    cc[0]=strarr[ii].charCodeAt(0);
    chk=1;
    for (jj=0;jj<this.maxbase;jj++) {
      if (cc[0]==this.basecharcode[jj]) {
        if (jj>ans[0])
          ans[0]=jj;
        chk=0;
        break;
      }
    }
    if (chk==1) {
      ans[0]=-1;
      break;
    }
  }
  if (ans[0]==0)
    return ans[0];

  ans[0]++;

  if (ans[0]==-1 || (ans[0]==15 && ans[1]==-1)) {
    if (ans[0]==-1 && ans[1]!=-1 && ans[1]!=10)
      return ans[0];
    var qqszsgnfe=new Int32Array([0,0,0,0]);
    var snum=this.txtabs(strnum,qqszsgnfe);
    if (snum=='NaN') {
      if (ans[0]!=15)
        return ans[0];
    }
    else
      ans[0]=(qqszsgnfe[0]==0 ? 10 : 100);
  }

  if (ans[1]!=-1) {
    if (ans[0]<=ans[1])
      return ans[1];
    ans[0]=-1;
    return ans[0];
  }

  if (ans[0]>=16 || ans[0]==2 || ans[0]==8)
    return ans[0];

  if (ans[0]>10)
    ans[0]=16;
  else if (ans[0]!=100)
    ans[0]=10;

  return ans[0];

},

txtconvfmt: function(strnum,fmt12,maxlen) {

  var ll=strnum.length;
  if (ll==0)
    return strnum;

  var ans=[],strarr=[],modans=[];
  var strfmt,strshacc,strnum10,strtmp;
  var ii=0,jj=0,idx=-1,sgn=0,cmp=0;
  var begend=new Int16Array([-1,-1]);
  var cc=new Uint16Array([0]);
  if (fmt12[1]<2 || fmt12[1]>=this.maxbase)
    fmt12[1]=10;

  fmt12[0]=this.txtchknumfmt(strnum,fmt12[0]);
  if (fmt12[0]==-1 || fmt12[0]==100)
    return strnum;

  if (fmt12[0]==fmt12[1])
    return strnum;

  if (fmt12[0]==10)
    strarr=this.txtexp2simple(strnum).trim().toUpperCase().split('');
  else
    strarr=strnum.trim().toUpperCase().split('');
  ll=strarr.length;

  cc[0]=strarr[0].charCodeAt(0);
  if (cc[0]==this.asciiminus[0]) {
    strarr.shift();
    if (--ll==0)
      return strnum;
    sgn=1;
    cc[0]=strarr[0].charCodeAt(0);
  }

  if (fmt12[0]!=10) {
    if (cc[0]==this.asciizero[0]) {
      strarr.shift();
      if (--ll==0)
        return strnum;
      cc[0]=strarr[0].charCodeAt(0);
      if (cc[0]==this.asciiX[0]) {
        if (fmt12[0]==16) {
          strarr.shift();
          if (--ll==0)
            return strnum;
        }
      }
    }
    else if (cc[0]==this.asciihash[0]) {
      if (fmt12[0]==16) {
        strarr.shift();
        if (--ll==0)
          return strnum;
      }
    }
    strfmt=this.txtsimplei102str(fmt12[0]);
    strnum10='0';
    strshacc='1';
    for (ii=ll-1;ii>-1;ii--)  {
      cc[0]=strarr[ii].charCodeAt(0);
      for (jj=0;jj<fmt12[0];jj++) {
        if (cc[0]==this.basecharcode[jj]) {
          idx=jj;
          break;
        }
      }
      strtmp=this.txtcalc('*',this.txtsimplei102str(idx),strshacc,0);
      strnum10=this.txtcalc('+',strnum10,strtmp,0);
      if (ii==0)
        break;
      strshacc=this.txtcalc('*',strshacc,strfmt,0);
    }
  }
  else
    strnum10=strarr.join('');

  strfmt=this.txtsimplei102str(fmt12[1]);
  ans.length=strnum10.length;
  idx=0;

  do {
    modans=this.txtmod(strnum10,strfmt,maxlen);
    ans[idx++]=String.fromCharCode(this.basecharcode[this.txtstr2simplei10(modans[1])]);
    strnum10=modans[0].slice();
    strarr.length=strnum10.length;
    this.zeroarr(strarr,begend);
    cmp=this.txtcmp(strnum10,strarr.join(''));
  } while (cmp===1);
  if (sgn==1)
    ans[idx++]='-';
  ans.length=idx;

  return ans.reverse().join('');


},

txtcalc: function(calctype,strnum1,strnum2,maxlen) {

  var ans="NaN";

  if (calctype!='+' && calctype!='-' && calctype!='*' && calctype!='/')
    return ans;

  var ii=0,cmp=0;
  var addsubstype='';

  var qqszsgnfe1=new Int32Array([0,0,0,0]);
  var snum1=this.txtabs(strnum1,qqszsgnfe1);
  var qqszsgnfe2=new Int32Array([0,0,0,0]);
  var snum2=this.txtabs(strnum2,qqszsgnfe2);
  var qq=new Int32Array([qqszsgnfe1[0],qqszsgnfe2[0]]);
  var sz=new Int32Array([qqszsgnfe1[1],qqszsgnfe2[1]]);
  var corrdiv=new Int32Array([0]);
  var snum=[snum1,snum2];
  this.txtfixqq(calctype,qq,snum);
  cmp=this.txtcmp(snum[0],snum[1]);

//print (new Error().lineNumber+" n1,n2 ["+snum[0]+","+snum[1]+"] qq1,sz1,n2,qq2,sz2 ["+
//qq[0]+","+sz[0]+"] ["+qq[1]+","+sz[1]+"]");

  if (calctype=='*') {
    ans=this.txtmult(snum[0],snum[1]);
  }
  else if (calctype=='/') {
    ans=this.txtdiv(snum[0],snum[1],maxlen,corrdiv);
    sz[0]-=corrdiv[0];
  }
  else {
    addsubstype='+';
    if ((calctype=='+' && qqszsgnfe1[2]!=qqszsgnfe2[2]) ||
        (calctype=='-' && qqszsgnfe1[2]==qqszsgnfe2[2]))
      addsubstype='-';
    if (cmp<0)
      this.txtswap(snum);
    ans=(addsubstype=='+' ? this.txtadd(snum[0],snum[1]) : this.txtsubs(snum[0],snum[1]));
  }

  if ((ans+'')==='NaN')
    return ans;

//print (new Error().lineNumber+" ans ["+ans+"] qq1,sz1,qq2,sz2 ["+
//qq[0]+","+sz[0]+"] ["+qq[1]+","+sz[1]+"]");

  ans=this.txtunfixqq(calctype,ans,sz,qq);

  if ((addsubstype.length==0 && qqszsgnfe1[2]!=qqszsgnfe2[2]) ||
      (addsubstype.length>0 && ((calctype=='+' && ((qqszsgnfe1[2]>0 && qqszsgnfe2[2]>0) ||
      (qqszsgnfe1[2]>0 && qqszsgnfe2[2]==0 && cmp>0) ||
      (qqszsgnfe2[2]>0 && qqszsgnfe1[2]==0 && cmp<0))) ||
      (calctype=='-' && ((qqszsgnfe1[2]>0 && qqszsgnfe2[2]==0) ||
      (qqszsgnfe1[2]==0 && qqszsgnfe2[2]==0 && cmp<0) ||
      (qqszsgnfe1[2]>0 && qqszsgnfe2[2]>0 && cmp<0)))))) {
    ans=('-'+ans);
  }

  return ans;

}

}

var combobase1=10,combobase2=10;

function checkBase1(evt) {

  var kevt=(evt || window.event);
  var key=(kevt.keyCode || kevt.which);
  key=String.fromCharCode(key);

  var strnum1=document.getElementById("number1").value;
  var strnum1chk=(strnum1+key);
  var ans=Int32Array([combobase1,-1]);
  ans[1]=textcalc.txtchknumfmt(strnum1chk,combobase1);
  if (ans[0]!=ans[1]) {
    kevt.returnValue=false;
    if(kevt.preventDefault)
      kevt.preventDefault();
  }

}

function checkBase2(evt) {

  var kevt=(evt || window.event);
  var key=(kevt.keyCode || kevt.which);
  key=String.fromCharCode(key);

  var strnum2=document.getElementById("number2").value;
  var strnum2chk=(strnum2+key);
  var ans=Int32Array([combobase2,-1]);
  ans[1]=textcalc.txtchknumfmt(strnum2chk,combobase2);
  if (ans[0]!=ans[1]) {
    kevt.returnValue=false;
    if(kevt.preventDefault)
      kevt.preventDefault();
  }

}

function convBase1(form) {

  var idx=form.base1.options.selectedIndex;
  var strbase1=form.base1.options[idx].text;
  var strnum1=form.number1.value;
  if (!strnum1) {
    combobase1=strbase1;
    return;
  }

  var fmt12=new Int8Array([combobase1,strbase1]);
  if (fmt12[0]==fmt12[1])
    return;
  var strmaxlen=form.maxlen.value;
  var strnumconv=textcalc.txtconvfmt(strnum1,fmt12,strmaxlen);
  if (fmt12[0]>1 && fmt12[0]<37) {
    form.elements["number1"].value=strnumconv;
    combobase1=fmt12[1];
  }

}

function convBase2(form) {

  var idx=form.base2.options.selectedIndex;
  var strbase2=form.base2.options[idx].text;
  var strnum2=form.number2.value;
  if (!strnum2) {
    combobase2=strbase2;
    return;
  }

  var fmt12=new Int8Array([combobase2,strbase2]);
  if (fmt12[0]==fmt12[1])
    return;
  var strmaxlen=form.maxlen.value;
  var strnumconv=textcalc.txtconvfmt(strnum2,fmt12,strmaxlen);
  if (fmt12[0]>1 && fmt12[0]<37) {
    form.elements["number2"].value=strnumconv;
    combobase2=fmt12[1];
  }

}

function calc3(form) {

  var strmaxlen=form.maxlen.value;
  var maxlen=new Int32Array([strmaxlen]);
  if (maxlen[0]<3)
    form.elements["maxlen"].value='3';

  var strnum1=form.number1.value;
  var idx=form.base1.options.selectedIndex;
  var strbase1=form.base1.options[idx].text;
  var fmt12=new Int8Array([strbase1,10]);
  if (fmt12[0]!=fmt12[1])
    strnum1=textcalc.txtconvfmt(strnum1,fmt12,maxlen.join(''));

  var strnum2=form.number2.value;
  idx=form.base2.options.selectedIndex;
  var strbase2=form.base2.options[idx].text;
  fmt12[0]=strbase2;
  if (fmt12[0]!=fmt12[1])
    strnum2=textcalc.txtconvfmt(strnum2,fmt12,maxlen.join(''));

  idx=form.operator.options.selectedIndex;
  var strop=form.operator.options[idx].text;

  var strres=textcalc.txtcalc(strop,strnum1,strnum2,maxlen.join(''));

  form.elements["result"].value=strres;

}

function checkMaxlen(form) {

  var strmaxlen=form.maxlen.value;
  var maxlen=new Int32Array([strmaxlen]);
  if (maxlen[0]<1)
    maxlen[0]=3;
  else if (maxlen[0]>999)
    maxlen[0]=999;

  form.elements["maxlen"].value=maxlen.join('');

}

function roundResult(form) {

  var strrd=form.roundresult.value;
  var strres=form.result.value;
  if (strrd && strres) {
    var rdpos=new Int32Array([strrd]);
    var ans=textcalc.txtround(strres,rdpos[0]);
    form.elements["result"].value=ans;
  }

}

function f2e(form) {

  var strres=form.result.value;
  if (strres) {
    var ans=textcalc.txtsimple2exp(strres);
    form.elements["result"].value=ans;
  }

}

function e2f(form) {

  var strres=form.result.value;
  if (strres) {
    var ans=textcalc.txtexp2simple(strres);
    form.elements["result"].value=ans;
  }

}

function resetInput(form) {

  form.elements["number1"].value='';
  form.base1.options.selectedIndex=8;
  form.operator.options.selectedIndex=0;
  form.elements["number2"].value='';
  form.base2.options.selectedIndex=8;
  form.elements["result"].value='';
  form.elements["maxlen"].value='100'
  form.elements["roundresult"].value='';

}

