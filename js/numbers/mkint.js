
var mkint={};

mkint.minbase=2;
mkint.maxbase=36;
mkint.maxintbits=32;
mkint.maxint=2147483648;
mkint.maxuint=4294967296;

mkint.basecharcode=[
  48,49,50,51,52,53,54,55,56,57,65,66,67,
  68,69,70,71,72,73,74,75,76,77,78,79,80,
  81,82,83,84,85,86,87,88,89,90,95
];

// 2**0 ... 2**32
mkint.basepow2=[
  1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,
  32768,65536,131072,262144,524288,1048576,2097152,4194304,
  8388608,16777216,33554432,67108864,134217728,268435456,
  536870912,1073741824,2147483648,4294967296
];

mkint.basepow10=[1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000];

/* ********** */
mkint.sgn4charcode=function(numcode) {
  var ans=0;
  if (typeof numcode!='number')
    return ans;
  if (numcode==mkuni.asciiplus)
    ans=1;
  else if (numcode==mkuni.asciiminus)
    ans=-1;
  return ans;
};

/* ********** */
mkint.base4charcode=function(numcode) {
  var ans=-1;
  if (typeof numcode!='number')
    return ans;
  ans=mkint.basecharcode.indexOf(numcode);
  if (ans>=0)
    ans+=1;
  return ans;
};

/* ********** */
mkint.stripleadingzeros=function(str) {
  str=mkuni.chkstr(str);
  if (str.length==0)
    return str;
  var cc=0,ii=0,idx=-1;
  for (ii=0;ii<str.length;ii++) {
    cc=str.charCodeAt(ii);
    if (cc!=mkuni.asciizero) {
      idx=ii;
      break;
    }
  }
  if (idx<0)
    str='';
  else if (idx>0)
    str=str.substring(idx);
  return str;
};

/* ********** */
mkint.striptrailingzeros=function(str) {
  str=mkuni.chkstr(str);
  if (str.length==0)
    return str;
  var cc=0,ii=0,idx=-1;
  for (ii=str.length-1;ii>-1;ii--) {
    cc=str.charCodeAt(ii);
    if (cc!=mkuni.asciizero) {
      idx=ii;
      break;
    }
  }
  idx++;
  if (idx==0)
    str='';
  else if (idx<str.length)
    str=str.substring(0,idx);
  return str;
};

/* ********** */
mkint.chknumberrange=function(num,minnum,maxnum) {
  num=mkuni.chknumber(num,NaN);
  if (isNaN(num))
    return mkuni.bad;
  minnum=mkuni.chknumber(minnum,NaN);
  maxnum=mkuni.chknumber(maxnum,NaN);
  if (isNaN(minnum) && isNaN(maxnum))
    return mkuni.bad;
  if (isNaN(minnum))
    return (num<=maxnum ? mkuni.good : mkuni.bad);
  if (isNaN(maxnum))
    return (minnum<=num ? mkuni.good : mkuni.bad);
  return ((minnum<=num && num<=maxnum) ? mkuni.good : mkuni.bad);
};

/* ********** */
mkint.chkbase=function(base,defbase) {
  defbase=mkuni.chknumber(defbase,10);
  var ans=parseInt(mkuni.chknumber(base,defbase),10);
  if (mkint.chknumberrange(ans,mkint.minbase,mkint.maxbase)==mkuni.bad)
    ans=defbase;
  return ans;
};

/* ********** */
mkint.int2str=function(num) {
  num=mkuni.chknumber(num);
  if (isNaN(num))
    return mkuni.strnan;
  var sgn=0;
  if (num<0) {
    num=-num;
    if (mkint.maxint<num)
      return mkuni.strnan;
    sgn=mkuni.asciiminus;
  }
  else if (mkint.maxuint<num)
    return mkuni.strnan;
  num=Math.floor(num);
  var ans=[];
  var idx=0;
  do {
    idx=Math.floor(num%10);
    ans.push(mkuni.str4code(this.basecharcode[idx]));
    num=Math.floor(num/10);
  } while (num>0);
  ans.push(mkuni.str4code(sgn));
  return ans.reverse().join('');
};

/* ********** */
mkint.int4str=function(str) {
  str=mkuni.chkstr(str);
  var cc=0,ii=0,ll=str.length,ans=0;
  if (ll==0 || ll>10)
    return NaN;
  var strarr=str.trim().toUpperCase().split('');
  for (ii=ll-1,jj=0;ii>-1;ii--,jj++) {
    cc=strarr[ii].charCodeAt(0);
    idx=mkint.basecharcode.indexOf(cc);
    if (idx<0 || idx>9) {
      if (ii==0 && cc==mkuni.asciiminus)
        ans*=(-1);
      else
        return NaN;
    }
    else
      ans+=(idx*mkint.basepow10[jj]);
  }
  return ans;
};

/* ********** */
mkint.trimbits=function(str) {
  var ans=mkuni.str4code(mkuni.asciizero);
  str=mkuni.chkstr(str);
  if (str.length==0)
    return ans;
  var ii=0,ll=str.length;
  for (ii=0;ii<ll;ii++) {
    if (str.charCodeAt(ii)==mkuni.asciione)
      break;
  }
  if (ii==0)
    ans=str;
  else if (ii<ll)
    ans=str.substring(ii);
  return ans;
};

/* ********** */
mkint.padbits=function(str,cnt) {
  cnt=mkuni.chknumber(cnt,0);
  str=mkuni.chkstr(str);
  if (cnt<=str.length)
    return str;
  var ans=new Array(cnt);
  var ii=0,jj=0;
  for (ii=0;ii<cnt-str.length;ii++)
    ans[jj++]=mkuni.str4code(mkuni.asciizero);
  for (ii=0;ii<str.length;ii++)
    ans[jj++]=str.charAt(ii);
  return ans.join('');
};

/* ********** */
mkint.bits2num=function(str) {
  str=mkuni.chkstr(str);
  if (str.length==0)
    return 0;
  str=mkint.trimbits(str);
  var ii=0,idx=0,ll=str.length,num=0,basenum=1.;
  for (ii=ll-1;ii>-1;ii--) {
    idx=ll-ii-1;
    if (str.charCodeAt(ii)==mkuni.asciione) {
      if (idx<=mkint.maxintbits)
        num+=mkint.basepow2[idx];
      else
        num+=basenum;
    }
    basenum*=2.;
  }
  return num;
};

/* ********** */
mkint.cmpbits=function(str1,str2) {
  str1=mkuni.chkstr(str1);
  str2=mkuni.chkstr(str2);
  if (str1.length==0)
    return ((str2.length==0) ? 0 : -1);
  if (str2.length==0)
    return 1;
  var str1arr=str1.split(''),str2arr=str2.split('');
  while (str1arr.length<str2arr.length)
    str1arr.unshift(mkuni.str4code(mkuni.asciizero));
  while (str2arr.length<str1arr.length)
    str2arr.unshift(mkuni.str4code(mkuni.asciizero));
  var ii=0;
  for (ii=0;ii<str2arr.length;ii++) {
    if (str1arr[ii].charCodeAt(0)==mkuni.asciizero &&
        str2arr[ii].charCodeAt(0)==mkuni.asciione)
      return -1;
    if (str1arr[ii].charCodeAt(0)==mkuni.asciione &&
        str2arr[ii].charCodeAt(0)==mkuni.asciizero)
      return 1;
  }
  return 0;
};

/* ********** */
mkint.addbits=function(str1,str2) {
  str1=mkuni.chkstr(str1);
  str2=mkuni.chkstr(str2);
  if (str1.length==0)
    return str2;
  if (str2.length==0)
    return str1;
  var str1arr=str1.split(''),str2arr=str2.split('');
  while (str1arr.length<str2arr.length)
    str1arr.unshift(mkuni.str4code(mkuni.asciizero));
  while (str2arr.length<str1arr.length)
    str2arr.unshift(mkuni.str4code(mkuni.asciizero));
  var ans=[];
  var ii=0,b1=0,b2=0,ba=0,carry=0;
  for (ii=str2arr.length-1;ii>-1;ii--) {
    b1=(str1arr[ii].charCodeAt(0)==mkuni.asciizero ? 0 : 1);
    b2=(str2arr[ii].charCodeAt(0)==mkuni.asciizero ? 0 : 1);
    ba=b1+b2+carry;
    if (ba==3) {
      ans.push(mkuni.str4code(mkuni.asciione));
    }
    else if (ba==2) {
      ans.push(mkuni.str4code(mkuni.asciizero));
      carry=1;
    }
    else {
      if (ba==0)
        ans.push(mkuni.str4code(mkuni.asciizero));
      else
        ans.push(mkuni.str4code(mkuni.asciione));
      carry=0;
    }
  }
  if (carry==1)
    ans.push(mkuni.str4code(mkuni.asciione));
  var strans=ans.reverse().join('');
  return mkint.trimbits(strans);
};

/* ********** */
mkint.subsbits=function(str1,str2) {
  str1=mkuni.chkstr(str1);
  str2=mkuni.chkstr(str2);
  if (str1.length==0)
    return str2;
  if (str2.length==0)
    return str1;
  var str1arr=str1.split(''),str2arr=str2.split('');
  while (str1arr.length<str2arr.length)
    str1arr.unshift(mkuni.str4code(mkuni.asciizero));
  while (str2arr.length<str1arr.length)
    str2arr.unshift(mkuni.str4code(mkuni.asciizero));
  var ans=[];
  var ii=0,b1=0,b2=0,ba=0,carry=0;
  for (ii=str2arr.length-1;ii>-1;ii--) {
    b1=(str1arr[ii].charCodeAt(0)==mkuni.asciizero ? 0 : 1);
    b2=(str2arr[ii].charCodeAt(0)==mkuni.asciizero ? 0 : 1);
    ba=b1-b2-carry;
    if (ba==-2) {
      ans.push(mkuni.str4code(mkuni.asciizero));
    }
    else if (ba==-1) {
      ans.push(mkuni.str4code(mkuni.asciione));
      carry=1;
    }
    else {
      if (ba==0)
        ans.push(mkuni.str4code(mkuni.asciizero));
      else
        ans.push(mkuni.str4code(mkuni.asciione));
      carry=0;
    }
  }
  var strans=ans.reverse().join('');
  return mkint.trimbits(strans);
};

/* ********** */
mkint.multbits=function(str1,str2) {
  str1=mkuni.chkstr(str1);
  str2=mkuni.chkstr(str2);
  if (str1.length==0 || str2.length==0)
    return mkuni.str4code(mkuni.asciizero);
  var str1arr=str1.split(''),str2arr=str2.split('');
  var ans=[mkuni.str4code(mkuni.asciizero)];
  var mstr1arr=str1arr.slice();
  var mans='';
  var ii=0;
  for (ii=str2arr.length-1;ii>-1;ii--) {
    if (str2arr[ii].charCodeAt(0)==mkuni.asciione) {
      mans=mkint.addbits(mstr1arr.join(''),ans.join(''));
      ans=mans.split('');
    }
    mstr1arr.push(mkuni.str4code(mkuni.asciizero));
  }
  return ans.join('');
};

/* ********** */
mkint.divbits=function(str1,str2,rem) {
  str1=mkuni.chkstr(str1);
  str2=mkuni.chkstr(str2);
  if (str2.length==0)
    return mkuni.strnan;
  if (str1.length==0)
    return mkuni.str4code(mkuni.asciizero);
  var ans=[];
  var str1arr=str1.split('');
  var dstr1arr=[];
  var dstr1='';
  var ii=0,cmp=0;
  for (ii=0;ii<str1arr.length;ii++) {
    dstr1arr.push(str1arr[ii]);
    dstr1=dstr1arr.join('');
    cmp=mkint.cmpbits(dstr1,str2);
    if (cmp>=0) {
      ans.push(mkuni.str4code(mkuni.asciione));
      dstr1=mkint.subsbits(dstr1,str2);
      dstr1arr=dstr1.split('');
    }
    else {
      if (ans.length>0)
        ans.push(mkuni.str4code(mkuni.asciizero));
    }
  }
  if ((typeof rem!='undefined') && rem.hasOwnProperty('rem'))
    rem.rem=dstr1arr.join('');
  if (ans.length==0)
    return mkuni.str4code(mkuni.asciizero);
  return ans.join('');
};

/* ********** */
mkint.shiftright=function(str,base,bitarr) {
  str=mkuni.chkstr(str);
  if (str.length==0)
    return str;
  base=mkint.chkbase(base);
  var ans=[];
  var strarr=str.split('');
  var ii=0,idx=-1,diga=0,digb=0,digc=0,cc=0;
  for (ii=0;ii<strarr.length;ii++) {
    cc=strarr[ii].charCodeAt(0);
    idx=mkint.basecharcode.indexOf(cc);
    digb=base*digb+idx;
    digc=digb/2;
    diga=parseInt(digc.toString());
    if (ii>0 || diga>0)
      ans.push(mkuni.str4code(mkint.basecharcode[diga],mkint.basecharcode));
    digc=digb%2;
    digb=parseInt(digc.toString());
  }
  if ((typeof bitarr!='undefined') && (bitarr instanceof Array))
    bitarr.push(digb);
  return ans.join('');
};

/* ********** */
mkint.str2bits=function(str,base) {
  str=mkuni.chkstr(str);
  if (str.length==0)
    return mkuni.str4code(mkuni.asciizero);
  base=mkint.chkbase(base);
  var ans=[];
  while (str.length>0)
    str=mkint.shiftright(str,base,ans);
  return ans.reverse().join('');
};

/* ********** */
mkint.bits2str=function(str,base) {
  str=mkuni.chkstr(str);
  if (str.length==0)
    return mkuni.str4code(mkuni.asciizero);
  base=mkint.chkbase(base);
  var i8=0,bbase=mkint.str2bits(base.toString(),10);
  var numarr=[];
  var rem={'rem':mkuni.str4code(mkuni.asciizero)};
  while (str.charCodeAt(0)!=mkuni.asciizero) {
    str=mkint.divbits(str,bbase,rem);
    i8=mkint.bits2num(rem.rem);
    if (i8>mkint.maxbase)
      i8=0;
    numarr.push(mkuni.str4code(mkint.basecharcode[i8],mkint.basecharcode));
  }
  if (numarr.length==0)
    return mkuni.str4code(mkuni.asciizero);
  return numarr.reverse().join('');
};

