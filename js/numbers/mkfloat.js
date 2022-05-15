
var mkfloat={};

mkfloat.maxieee=15;
mkfloat.euler='2.718281828459045235360287471352662497757247093699959574966967627724076630';
mkfloat.pi='3.141592653589793238462643383279502884197169399375105820974944592307816406';
mkfloat.rad='0.017453292519943295769236907684886127134428718885417254560971914401710091';
mkfloat.ln10='2.302585092994046';

mkfloat.basecharcode=[48,49,50,51,52,53,54,55,56,57];

/* ********** */
mkfloat.fabs=function(num) {
  if ((typeof num!='number') || isNaN(num))
    return num;
  if (!isFinite(num))
    return Infinity;
  return (num<0 ? -num : num);
};

/* ********** */
mkfloat.ipow2=function(idx) {
  var chk=mkuni.chknumber(idx,0)
  idx=parseInt(chk.toString());
  var ii=0,sz=mkint.basepow2.length,aidx=(idx<0 ? -idx : idx);
  for (ii=sz;ii<=aidx;ii++)
    mkint.basepow2.push(Math.pow(2,ii));
  var ans=mkint.basepow2[aidx];
  if (idx<0)
    ans=1./ans;
  return ans;
};

/* ********** */
mkfloat.ipow10=function(idx) {
  var chk=mkuni.chknumber(idx,0)
  idx=parseInt(chk.toString());
  var ii=0,sz=mkint.basepow10.length,aidx=(idx<0 ? -idx : idx);
  for (ii=sz;ii<=aidx;ii++)
    mkint.basepow10.push(Math.pow(10,ii));
  var ans=mkint.basepow10[aidx];
  if (idx<0)
    ans=1./ans;
  return ans;
};

/* ********** */
mkfloat.exp2simple=function(num) {
  if ((typeof num!='number') && (typeof num!='string'))
    return mkuni.str4code(mkuni.asciizero);
  var str=((typeof num=='string') ? num : num.toString());
  str=str.trim().toUpperCase();
  var ii=0,ll=str.length,expsgn=1,idxsgn=0,idxdec=-1,idxexp=-1,idxexpsgn=-1;
  if (ll==0)
    return mkuni.str4code(mkuni.asciizero);
  var ans=[];
  var intg=[];
  var frac=[];
  var exp=[];
  if (str.charCodeAt(0)==mkuni.asciiplus || str.charCodeAt(0)==mkuni.asciiminus) {
    if (str.charCodeAt(0)==mkuni.asciiminus)
      ans.push(mkuni.str4code(mkuni.asciiminus));
    idxsgn=1;
  }
  for (ii=idxsgn;ii<ll;ii++) {
    if (str.charCodeAt(ii)==mkuni.asciidot) {
      if (idxdec>=0 || idxexp>=0)
        break;
      idxdec=ii;
      continue;
    }
    if (str.charCodeAt(ii)==mkuni.asciiE) {
      if (idxexp>=0)
        break;
      idxexp=ii;
      idxdec=-1;
      continue;
    }
    if (str.charCodeAt(ii)==mkuni.asciiplus || str.charCodeAt(ii)==mkuni.asciiminus) {
      if (idxexp<0 || idxexpsgn>=0)
        break;
      idxexpsgn=ii;
      if (str.charCodeAt(ii)==mkuni.asciiminus)
        expsgn=-1;
      continue;
    }
    if (str.charCodeAt(ii)<mkuni.asciizero || str.charCodeAt(ii)>mkuni.asciinine)
      break;
    if (idxdec>=0)
      frac.push(str.charCodeAt(ii));
    else if (idxexp>=0)
      exp.push(str.charCodeAt(ii));
    else
      intg.push(str.charCodeAt(ii));
  }
  while (exp.length>1 && exp[0]==mkuni.asciizero)
    exp.shift();
  while (intg.length>1 && intg[0]==mkuni.asciizero)
    intg.shift();
  var expnumstr=mkuni.str4code(mkuni.asciizero);
  if (exp.length>0)
    expnumstr=String.fromCharCode.apply(String,exp);
  var expnum=parseInt(expnumstr,10);
  if (expsgn==1) {
    while (frac.length>0 && expnum>0) {
      intg.push(frac.shift());
      expnum-=1;
    }
    while (expnum>0) {
      intg.push(mkuni.asciizero);
      expnum-=1;
    }
  }
  else {
    while (intg.length>0 && expnum>0) {
      frac.unshift(intg.pop());
      expnum-=1;
    }
    while (expnum>0) {
      frac.unshift(mkuni.asciizero);
      expnum-=1;
    }
  }
  if (intg.length==0)
    ans.push(mkuni.str4code(mkuni.asciizero));
  for (ii=0;ii<intg.length;ii++)
    ans.push(mkuni.str4code(intg[ii],mkfloat.basecharcode));
  while (frac.length>0 && frac[frac.length-1]==mkuni.asciizero)
    frac.pop();
  if (frac.length>0)
    ans.push(mkuni.str4code(mkuni.asciidot));
  for (ii=0;ii<frac.length;ii++)
    ans.push(mkuni.str4code(frac[ii],mkfloat.basecharcode));
  return ans.join('');
};

/* ********** */
mkfloat.simple2exp=function(num) {
  if ((typeof num!='number') && (typeof num!='string'))
    return mkuni.str4code(mkuni.asciizero);
  var str=((typeof num=='string') ? num : num.toString());
  str=str.trim().toUpperCase();
  var ii=0,ll=str.length,idxdec=-1,idxsgn=0,expnum=0;
  if (ll==0)
    return mkuni.str4code(mkuni.asciizero);
  var ans=[];
  var intg=[];
  var frac=[];
  if (str.charCodeAt(0)==mkuni.asciiplus || str.charCodeAt(0)==mkuni.asciiminus) {
    if (str.charCodeAt(0)==mkuni.asciiminus)
      ans.push(mkuni.asciiminus);
    idxsgn=1;
  }
  for (ii=idxsgn;ii<ll;ii++) {
    if (str.charCodeAt(ii)==mkuni.asciidot) {
      if (idxdec>=0)
        break;
      idxdec=ii;
      continue;
    }
    if (str.charCodeAt(ii)<mkuni.asciizero || str.charCodeAt(ii)>mkuni.asciinine)
      break;
    if (idxdec>=0)
      frac.push(str.charCodeAt(ii));
    else
      intg.push(str.charCodeAt(ii));
  }
  while (intg.length>1 && intg[0]==mkuni.asciizero)
    intg.shift();
  while (frac.length>0 && frac[frac.length-1]==mkuni.asciizero)
    frac.pop();
  if (intg.length==0 || intg[0]==mkuni.asciizero) { // <1
    expnum-=1;
    while (frac.length>0 && frac[0]==mkuni.asciizero) {
      frac.shift();
      expnum-=1;
    }
    intg.shift();
  }
  else // >1
    expnum=intg.length-1;
  if (intg.length>0)
    intg.splice(1,0,mkuni.asciidot);
  else if (frac.length>0)
    frac.splice(1,0,mkuni.asciidot);
  else
    return mkuni.str4code(mkuni.asciizero);
  ans=ans.concat(intg,frac);
  if (expnum!=0)
    ans.push(mkuni.asciiE);
  var strans=String.fromCharCode.apply(String,ans);
  if (expnum!=0)
    strans+=expnum.toString();
  return strans;
};

/* ********** */
mkfloat.cut2int=function(num) {
  if ((typeof num!='number') && (typeof num!='string'))
    return mkuni.str4code(mkuni.asciizero);
  var str=((typeof num=='string') ? num : num.toString());
  var idx=str.indexOf(mkuni.str4code(mkuni.asciidot));
  if (idx<0)
    idx=str.length;
  str=str.substring(0,idx);
  if (str.length==0)
    str=mkuni.str4code(mkuni.asciizero);
  var ans=parseInt(str,10);
  return ans;
};

/* ********** */
mkfloat.base10scale=function(num,base) {
  num=mkuni.chknumber(num,NaN);
  if (isNaN(num))
    return 308; // maxscale
  if (num==0)
    return 0;
  num=mkfloat.fabs(num);
  var base2e=(mkint.chkbase(base)==2 ? Math.LN2 : Math.LN10);
  var ans=Math.log(num)/base2e,corr=mkfloat.ipow10(-mkfloat.maxieee);
  if (ans<0 && mkfloat.cut2int(ans-corr)<mkfloat.cut2int(ans))
    return mkfloat.cut2int(ans-corr);
  return Math.floor(ans+corr);
};

/* ********** */
mkfloat.roundpos=function(num,pos,epspos) {
  num=mkuni.chknumber(num,NaN);
  pos=mkuni.chknumber(pos,NaN);
  if (isNaN(num) || isNaN(pos))
    return num;
  var strnum=num.toString();
  strnum=mkfloat.exp2simple(strnum);
  var stri=strnum.split('');
  var sgn=1;
  if (stri[0].charCodeAt(0)==mkuni.asciiminus) {
    stri.shift();
    sgn=-1;
  }
  var ii=0,ll=stri.length,phony1=0,idx=stri.indexOf(mkuni.str4code(mkuni.asciidot));
  if (idx<0)
    idx=ll;
  if ((idx+pos)>=ll)
    return num;
  if (idx==1 && stri[0].charCodeAt(0)==mkuni.asciizero) {
    stri[0]=mkuni.str4code(mkuni.asciione);
    phony1=1;
  }
  if (idx<ll) {
    stri.splice(idx,1);
    ll-=1;
  }
  var chk=[];
  for (ii=0;ii<(idx+pos);ii++)
    chk.push(mkuni.asciizero);
  if (chk.length<stri.length)
    chk.push(mkuni.asciifive);
  epspos=mkuni.chknumber(epspos,4);
  for (ii=0;chk.length<stri.length;ii++)
    chk.push(ii==epspos ? mkuni.asciione : mkuni.asciizero);
  var strinum=stri.join(''),strchk=String.fromCharCode.apply(String,chk);
  var strrd=mkint.addbits(mkint.str2bits(strinum,10),mkint.str2bits(strchk,10));
  chk=mkint.bits2str(strrd,10).split('');
  if (chk.length>stri.length) // add overflow
    idx++;
  if ((idx+pos)<=0)
    return 0;
  chk.splice(idx,0,mkuni.str4code(mkuni.asciidot));
  if (phony1==1) {
    if (chk[0].charCodeAt(0)==mkuni.asciione)
      chk[0]=mkuni.str4code(mkuni.asciizero);
    else
      chk[0]=mkuni.str4code(mkuni.asciione);
  }
  if (pos>=0)
    chk=chk.slice(0,idx+pos+1);
  else {
    chk=chk.slice(0,idx+pos);
    for (ii=pos;ii<0;ii++)
      chk.push(mkuni.str4code(mkuni.asciizero));
  }
  if (sgn<0)
    chk.unshift(mkuni.str4code(mkuni.asciiminus));
  var ans=chk.join('');
  return parseFloat(ans);
};

/* ********** */
mkfloat.rounddown=function(num,pos) {
  var ans=mkfloat.roundpos(num,pos);
  if (isNaN(ans))
    return NaN;
  if (ans>num) {
    num-=mkfloat.ipow10(-pos);
    ans=mkfloat.roundpos(num,pos);
  }
  return ans;
};

/* ********** */
mkfloat.roundup=function(num,pos) {
  var ans=mkfloat.roundpos(num,pos);
  if (isNaN(ans))
    return NaN;
  if (ans<num) {
    num+=mkfloat.ipow10(-pos);
    ans=mkfloat.roundpos(num,pos);
  }
  return ans;
};

/* ********** */
mkfloat.diff=function(dnum1,dnum2,blur) {
  dnum1=mkuni.chknumber(dnum1,NaN),dnum2=mkuni.chknumber(dnum2,NaN);
  if (isNaN(dnum1)) {
    if (isNaN(dnum2))
      return 0;
    return Infinity;
  }
  if (isNaN(dnum2))
    return Infinity;
  if (!isFinite(dnum1)) {
    if (dnum1==-Infinity) {
      if (!isFinite(dnum2) && dnum2==-Infinity)
        return 0;
      return -Infinity;
    }
    else {
      if (!isFinite(dnum2) && dnum2==Infinity)
        return 0;
      return Infinity;
    }
  }
  if (!isFinite(dnum2))
    return (dnum2==-Infinity ? Infinity : -Infinity);
  var ans=dnum1-dnum2;
  if (isNaN(ans) || !isFinite(ans))
    return (dnum1<dnum2 ? -Infinity : (dnum2<dnum1 ? Infinity : 0));
  blur=mkuni.chknumber(blur,NaN);
  if (!isNaN(blur)) {
   if (mkfloat.fabs(ans)<mkfloat.fabs(blur))
     return 0;
  }
  return dnum1-(dnum1-ans);
};

/* ********** */
mkfloat.cmp=function(dnum1,dnum2,blur) {
  var df=mkfloat.diff(dnum1,dnum2,blur);
  if (df==Infinity || df>0)
    return 1;
  if (df==-Infinity || df<0)
    return -1;
  return 0;
};

/* ********** */
mkfloat.sprintf=function(val,fdigit,fmt) {
  var ans=mkuni.strnan;
  if ((typeof val!='number') || isNaN(val))
    return ans;
  if (typeof fmt!='string')
    fmt=mkuni.str4code(mkuni.asciig);
  var ff=fmt.toLowerCase().charCodeAt(0);
  if (fdigit===0 && ff==mkuni.asciid) {
    ans=Math.round(val).toString();
    return ans;
  }
  var sc=mkfloat.base10scale(val,10);
  fdigit=mkuni.chknumber(fdigit,sc);
  if (fdigit>=sc+mkfloat.maxieee)
    fdigit=sc+mkfloat.maxieee;
  var rdval=mkfloat.roundpos(val,fdigit);
  ans=rdval.toString();
  if (ff!=mkuni.asciie && ff!=mkuni.asciif) {
    ff=mkuni.asciif;
    var absrdval=mkfloat.fabs(rdval);
    if (absrdval>mkfloat.ipow10(6) || absrdval<mkfloat.ipow10(-6))
      ff=mkuni.asciie;
  }
  if (ff==mkuni.asciie)
    ans=mkfloat.simple2exp(ans);
  return ans;
};

/* ********** */
mkfloat.ieee2bits=function(dnum) {
  var ans=[];
  var str='';
  var bias=1023,fbits=52,ebits=11,ee=0,ii=0,idx=0;
  var ff=.0,c1=2.225073858507201e-308, // 2^1-bias
      c2=4.494232837155790e+307, // 2^bias-1
      c3=4.503599627370496e+15; // 2^fbits
  dnum=mkuni.chknumber(dnum,NaN);
  if (!isNaN(dnum) && (dnum===-Infinity || dnum===-.0 || dnum<.0))
    idx=1;
  ans.push(mkuni.str4code(mkint.basecharcode[idx],mkint.basecharcode));
  dnum=mkfloat.fabs(dnum);
  if (isNaN(dnum) || dnum==Infinity || dnum==0) {
    var cnt=(isNaN(dnum) ? 13 : (dnum==Infinity ? 12 : 0));
    for (ii=1;ii<64;ii++) {
      idx=(ii<cnt ? 1 : 0);
      ans.push(mkuni.str4code(mkint.basecharcode[idx],mkint.basecharcode));
    }
    return ans.join('');
  }
  if (dnum>=c1) {
    ee=mkfloat.base10scale(dnum,2);
    ff=dnum*mkfloat.ipow2(-ee);
    if (ff<1.) {
      ff*=2.;
      ee-=1;
    }
    ff=c3*(ff-1.);
    ee+=bias;
  }
  else
    ff=((dnum*c2)*c3);
  str=mkint.str2bits(ee.toString(),10);
  for (ii=0;ii<ebits-str.length;ii++)
    ans.push(mkuni.str4code(mkuni.asciizero));
  for (ii=0;ii<str.length;ii++)
    ans.push(str.charAt(ii));
  str=mkint.str2bits(ff.toString(),10);
  for (ii=0;ii<fbits-str.length;ii++)
    ans.push(mkuni.str4code(mkuni.asciizero));
  for (ii=0;ii<str.length;ii++)
    ans.push(str.charAt(ii));
  return ans.join('');
};

/* ********** */
mkfloat.bits2ieee=function(str) {
  if (typeof str!='string')
    return NaN;
  var bias=1023,fbits=52,ebits=11,
      ii=0,ll=str.length,ss=(str.charCodeAt(0)==mkuni.asciizero ? 1 : -1);
  for (ii=1;ii<ebits+1;ii++) {
    if (str.charCodeAt(ii)==mkuni.asciizero)
      break;
  }
  if (ii>ebits) {
    for (ii=ebits+1;ii<ll;ii++) {
      if (str.charCodeAt(ii)==mkuni.asciione)
        return (ss==1 ? NaN : -NaN);
    }
    return (ss==1 ? Infinity : -Infinity);
  }
  var c1=2.225073858507201e-308, // 2^1-bias
      c2=2.220446049250313E-16; // 2^-fbits
  var ee=mkint.bits2num(str.substring(1,ebits+1));
  var ff=mkint.bits2num(str.substring(ebits+1));
  var dnum=.0;
  if (ee>0)
    dnum=mkfloat.ipow2(ee-bias)*(1.+ff*c2);
  else if (ff!==0)
    dnum=c1*(ff*c2);
  return ss*dnum;
};

