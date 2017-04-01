
var aux={};

aux.ansok='ok';
aux.ansko='ko';
aux.ansnop='nop';

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

aux.qnextprop=function(lf,indent) {
  var ans=',';
  if (lf===1)
    ans+='\n';
  if (typeof indent=='string')
    ans+=indent;
  return ans;
};

var intobj={};

intobj.minbase=2;
intobj.maxbase=37;
intobj.maxintbits=32;
intobj.maxfraction=15;

intobj.typeX=0;
intobj.typeY=1;
intobj.typeZ=2;
intobj.typeW=3;

intobj.basecharcode=[48,49,50,51,52,53,54,55,56,57,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,
  80,81,82,83,84,85,86,87,88,89,90,95,35];

intobj.basepow2=[1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072,
  262144,524288,1048576,2097152,4194304,8388608,16777216,33554432,67108864,134217728,
  268435456,536870912,1073741824,2147483648,4294967296];

intobj.basepow10=[1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000];

intobj.chknumber=function(num,defnum) {
  if (typeof num!='number')
    return defnum;
  if (!isNaN(defnum) && (isNaN(num) || !isFinite(num)))
    return defnum;
  return num;
};

intobj.chknumberrange=function(num,minnum,maxnum) {
  var ans=aux.ansko;
  num=intobj.chknumber(num,NaN);
  if (isNaN(num) || (isNaN(minnum) && isNaN(maxnum)))
    return ans;
  if ((isNaN(minnum) && num<=maxnum) || 
      (isNaN(maxnum) && minnum<=num) ||
      (minnum<=num && num<=maxnum))
    ans=aux.ansok;
  return ans;
};

intobj.chkbase=function(base) {
  var chk=intobj.chknumber(base,10);
  base=parseInt(chk.toString());
  var ans=intobj.chknumberrange(base,intobj.minbase,intobj.maxbase);
  if (ans==aux.ansko)
    base=10;
  return base;
};

intobj.fabs=function(num) {
  if (typeof num!='number' || isNaN(num))
    return num;
  if (!isFinite(num))
    return Infinity;
  return (num<0 ? -num : num);
};

intobj.cutfloat=function(str) {
  if (typeof str!='string')
    str=str.toString();
  var idx=str.indexOf('.');
  if (idx<0)
    idx=str.length;
  str=str.substring(0,idx);
  if (str.length==0)
    str='0';
  var ans=parseFloat(str);
  return ans;
};

intobj.ipow2=function(idx) {
  var chk=intobj.chknumber(idx,0)
  idx=parseInt(chk.toString());
  var ii=0,sz=intobj.basepow2.length,aidx=(idx<0 ? -idx : idx);
  for (ii=sz;ii<=aidx;ii++)
    intobj.basepow2.push(Math.pow(2,ii));    
  var ans=intobj.basepow2[aidx];
  if (idx<0)
    ans=1./ans;
  return ans;
};

intobj.ipow10=function(idx) {
  var chk=intobj.chknumber(idx,0)
  idx=parseInt(chk.toString());
  var ii=0,sz=intobj.basepow10.length,aidx=(idx<0 ? -idx : idx);
  for (ii=sz;ii<=aidx;ii++)
    intobj.basepow10.push(Math.pow(10,ii));    
  var ans=intobj.basepow10[aidx];
  if (idx<0)
    ans=1./ans;
  return ans;
};

intobj.trimbits=function(str) {
  var ii=0,ll=str.length;
  for (ii=0;ii<ll;ii++) {
    if (str.charCodeAt(ii)==intobj.basecharcode[1])
      break;
  }
  return (ii==0 ? str : (ii==ll ? '0' : str.substring(ii)));
};

intobj.padbits=function(str,cnt) {
  if (cnt<=str.length)
    return str;
  var ans=new Array(cnt);
  var ii=0,jj=0;
  for (ii=0;ii<cnt-str.length;ii++)
    ans[jj++]=String.fromCharCode(intobj.basecharcode[0]);
  for (ii=0;ii<str.length;ii++)
    ans[jj++]=str.charAt(ii);
  return ans.join('');
};

intobj.bits2num=function(str) {
  str=intobj.trimbits(str);
  var ii=0,idx=0,ll=str.length,num=0,basenum=1.;
  for (ii=ll-1;ii>-1;ii--) {
    idx=ll-ii-1;
    if (str.charCodeAt(ii)==intobj.basecharcode[1]) {
      if (idx<=intobj.maxintbits)
        num+=intobj.basepow2[idx];
      else
        num+=basenum;
    }
    basenum*=2.;
  }
  return num;
};

intobj.scale=function(num,base) {
  num=intobj.chknumber(num,NaN);
  if (isNaN(num))
    return 308; // maxscale
  if (num==0)
    return 0;
  num=intobj.fabs(num);
  var base2e=(intobj.chkbase(base)==2 ? Math.LN2 : Math.LN10);
  var ans=Math.log(num)/base2e,corr=intobj.ipow10(-intobj.maxfraction);
  if (ans<0 && intobj.cutfloat(ans-corr)<intobj.cutfloat(ans))
    return intobj.cutfloat(ans-corr);
  return Math.floor(ans+corr);
};

intobj.cmpbits=function(str1,str2) {
  var str1arr=str1.split(''),str2arr=str2.split('');
  while (str1arr.length<str2arr.length)
    str1arr.unshift('0');
  while (str2arr.length<str1arr.length)
    str2arr.unshift('0');
  var ii=0;
  for (ii=0;ii<str2arr.length;ii++) {
    if (str1arr[ii].charCodeAt(0)==intobj.basecharcode[0] &&
        str2arr[ii].charCodeAt(0)==intobj.basecharcode[1])
      return -1;
    if (str1arr[ii].charCodeAt(0)==intobj.basecharcode[1] &&
        str2arr[ii].charCodeAt(0)==intobj.basecharcode[0])
      return 1;
  }
  return 0;
};

intobj.addbits=function(str1,str2) {
  var str1arr=str1.split(''),str2arr=str2.split('');
  while (str1arr.length<str2arr.length)
    str1arr.unshift('0');
  while (str2arr.length<str1arr.length)
    str2arr.unshift('0');
  var ans=[];
  var ii=0,b1=0,b2=0,ba=0,carry=0;
  for (ii=str2arr.length-1;ii>-1;ii--) {
    b1=(str1arr[ii].charCodeAt(0)==intobj.basecharcode[0] ? 0 : 1);
    b2=(str2arr[ii].charCodeAt(0)==intobj.basecharcode[0] ? 0 : 1);
    ba=b1+b2+carry;
    if (ba==3) {
      ans.push('1');
    }
    else if (ba==2) {
      ans.push('0');
      carry=1;
    }
    else {
      if (ba==0)
        ans.push('0');
      else
        ans.push('1');
      carry=0;
    }
  }
  if (carry==1)
    ans.push('1');
  var strans=ans.reverse().join('');
  return intobj.trimbits(strans);
};

intobj.subsbits=function(str1,str2) {
  var str1arr=str1.split(''),str2arr=str2.split('');
  while (str1arr.length<str2arr.length)
    str1arr.unshift('0');
  while (str2arr.length<str1arr.length)
    str2arr.unshift('0');
  var ans=[];
  var ii=0,b1=0,b2=0,ba=0,carry=0;
  for (ii=str2arr.length-1;ii>-1;ii--) {
    b1=(str1arr[ii].charCodeAt(0)==intobj.basecharcode[0] ? 0 : 1);
    b2=(str2arr[ii].charCodeAt(0)==intobj.basecharcode[0] ? 0 : 1);
    ba=b1-b2-carry;
    if (ba==-2) {
      ans.push('0');
    }
    else if (ba==-1) {
      ans.push('1');
      carry=1;
    }
    else {
      ans.push(ba==0 ? '0' : '1');
      carry=0;
    }
  }
  var strans=ans.reverse().join('');
  return intobj.trimbits(strans);
};

intobj.multbits=function(str1,str2) {
  var str1arr=str1.split(''),str2arr=str2.split('');
  var ans=['0'];
  var mstr1arr=str1arr.slice();
  var mans='';
  var ii=0;
  for (ii=str2arr.length-1;ii>-1;ii--) {
    if (str2arr[ii].charCodeAt(0)==intobj.basecharcode[1]) {
      mans=intobj.addbits(mstr1arr.join(''),ans.join(''));
      ans=mans.split('');
    }
    mstr1arr.push('0');
  }
  return ans.join('');
};

intobj.divbits=function(str1,str2,rem) {
  var ans=[];
  var str1arr=str1.split('');
  var dstr1arr=[];
  var dstr1='';
  var ii=0,cmp=0;
  for (ii=0;ii<str1arr.length;ii++) {
    dstr1arr.push(str1arr[ii]);
    dstr1=dstr1arr.join('');
    cmp=intobj.cmpbits(dstr1,str2);
    if (cmp>=0) {
      ans.push('1');
      dstr1=intobj.subsbits(dstr1,str2);
      dstr1arr=dstr1.split('');
    }
    else {
      if (ans.length>0)
        ans.push('0');
    }
  }
  if (typeof rem!='undefined')
    rem[0]=dstr1arr.join('');
  if (ans.length==0)
    return '0';
  return ans.join('');
};

intobj.shiftright=function(str,base,bitarr) {
  var ans=[];
  var strarr=str.split('');
  var ii=0,idx=-1,diga=0,digb=0,digc=0,cc=0;
  for (ii=0;ii<strarr.length;ii++) {
    cc=strarr[ii].charCodeAt(0);
    idx=intobj.basecharcode.indexOf(cc);
    digb=base*digb+idx;
    digc=digb/2;
    diga=parseInt(digc.toString());
    if (ii>0 || diga>0)
      ans.push(String.fromCharCode(intobj.basecharcode[diga]));
    digc=digb%2;
    digb=parseInt(digc.toString());
  }
  bitarr.push(digb);
  return ans.join('');
};

intobj.str2bits=function(str,base) {
  if (str.length==0)
    return '0';
  base=intobj.chkbase(base);
  var ans=[];
  while (str.length>0)
    str=intobj.shiftright(str,base,ans);
  return ans.reverse().join('');
};

intobj.bits2str=function(str,base) {
  if (str.length==0)
    return '0';
  base=intobj.chkbase(base);
  var i8=0,bbase=intobj.str2bits(base.toString(),10);
  var numarr=[];
  var rem=['0'];
  while (str.charCodeAt(0)!=intobj.basecharcode[0]) {
    str=intobj.divbits(str,bbase,rem);
    i8=intobj.bits2num(rem[0]);
    if (i8>intobj.maxbase)
      i8=0;
    numarr.push(String.fromCharCode(intobj.basecharcode[i8]));
  }
  if (numarr.length==0)
    return '0';
  return numarr.reverse().join('');
};

