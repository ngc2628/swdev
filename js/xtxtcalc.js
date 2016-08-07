
var textcalc={

// char code values for base calculation and comparison
asciibackspace: 8, asciispace: 32, asciihash: 35, asciimult: 42, asciiplus: 43, asciiminus: 45,
asciidec: 46, asciidivide: 47, asciizero: 48, asciione: 49, asciitwo: 50, asciithree: 51,
asciifour: 52, asciifive: 53, asciisix: 54, asciiseven: 55, asciieight: 56, asciinine: 57,
asciiA: 65, asciiB: 66, asciiC: 67, asciiD: 68, asciiE: 69, asciiF: 70, asciiG: 71, asciiH: 72,
asciiI: 73, asciiJ: 74, asciiK: 75, asciiL: 76, asciiM: 77, asciiN: 78, asciiO: 79, asciiP: 80,
asciiQ: 81, asciiR: 82, asciiS: 83, asciiT: 84, asciiU: 85, asciiV: 86, asciiW: 87, asciiX: 88,
asciiY: 89, asciiZ: 90, ascii_: 95, asciie: 101, asciif: 102, asciii: 105, asciix: 120,
asciidel: 127,

maxbase: 37,

basecharcode: [48,49,50,51,52,53,54,55,56,57,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,
  80,81,82,83,84,85,86,87,88,89,90,95],

// integer compare functions
// lower-than, lower-equal, equal, non-equal, greater-than, greater-equal
icmp: function(num1,num2) {

  if (typeof num1=='undefined') {
    if (typeof num2=='undefined')
      return 'eq';
    return 'lt';
  }
  if (typeof num2=='undefined')
    return 'gt';

  var inum1=parseInt(num1,10),inum2=parseInt(num2,10);

  if (isNaN(inum1)) {
    if (isNaN(inum2))
      return 'eq';
    return 'gt';
  }
  if (isNaN(inum2))
    return 'lt';

  if (inum1<inum2)
    return 'lt';
  if (inum2<inum1)
    return 'gt';
  return 'eq';

},

ilt: function(num1,num2) {

  if (this.icmp(num1,num2)=='lt')
    return true;
  return false;

},

ile: function(num1,num2) {

  if (this.icmp(num1,num2)=='lt' || this.icmp(num1,num2)=='eq')
    return true;
  return false;

},

ieq: function(num1,num2) {

  if (this.icmp(num1,num2)=='eq')
    return true;
  return false;

},

ine: function(num1,num2) {

  if (this.icmp(num1,num2)=='eq')
    return false;
  return true;

},

igt: function(num1,num2) {

  if (this.icmp(num2,num1)=='lt')
    return true;
  return false;

},

ige: function(num1,num2) {

  if (this.icmp(num2,num1)=='lt' || this.icmp(num1,num2)=='eq')
    return true;
  return false;

},

// integer text compare
txtcmp: function(num1,num2) {

  var l1=num1.length,l2=num2.length;

  if (l2<l1)
    return 1;
  else if (l1<l2)
    return -1;
  else if (l1==0 && l2==0)
    return 0;

  var strarr1=num1.split('');
  var strarr2=num2.split('');
  var ii=0;
  var cmp='eq';

  for (ii=0;ii<l1;ii++) {
    cmp=this.icmp(strarr1[ii].charCodeAt(0),strarr2[ii].charCodeAt(0));
    if (cmp=='lt')
      return -1;
    if (cmp=='gt')
      return 1;
  }

  return 0;

},

// array zero fill
zeroarr: function(arr,beg,end) {

  var ii=0;
  if (this.ilt(beg,0))
    beg=0;
  if (this.ilt(end,0) || this.ige(end,10))
    end=arr.length;
  for (ii=parseInt(beg,10);ii<parseInt(end,10);ii++)
    arr[ii]='0';

},

// modify string to integer number format
trimint10: function(str) {

  var istr=str.trim().toUpperCase();

  var ll=istr.length;
  if (ll==0)
    return '0';
  var ii=0,ee=ll,ss=0;
  var cc=0;

  // remove leading crap
  for (ii=0;ii<ll;ii++) {
    cc=istr.charCodeAt(ii);
    if (this.ieq(cc,this.asciiminus) || this.ieq(cc,this.asciiplus)) {
      ss=(this.ieq(cc,this.asciiminus) ? ii : ii+1);
      break;
    }
  }

  if (ss>=ee)
    return '0';

  // remove leading zeros
  for (ii=ss;ii<ll;ii++) {
    cc=istr.charCodeAt(ii);
    if (this.igt(cc,this.asciizero) && this.ile(cc,this.asciinine))
      break;
    ss++;
  }
  // remove trailing non-digits
  for (ii=ll-1;ii>=ss;ii--) {
    cc=istr.charCodeAt(ii);
    if (this.ige(cc,this.asciizero) && this.ile(cc,this.asciinine))
      break;
    ee--;
  }

  if (ee<=ss)
    return '0';
  else if (ss==0 && ee==ll)
    return istr;

  return istr.slice(ss,ee);

},

// swap within array
txtswap: function(c1c2) {

  var tmp=c1c2[0];
  c1c2[0]=c1c2[1];
  c1c2[1]=tmp;

},

// integer32 to string
txtsimplei102str: function(number) {

  var ans=[];
  var sgn=(this.ilt(number,0) ? 1 : 0);
  if (sgn==1)
    number=-number;
  var inum=parseInt(number,10),res=0;
  do {
    res=parseInt(inum%10,10);
    ans.push(String.fromCharCode(parseInt(this.basecharcode[res],10)));
    inum=parseInt(inum/10,10);
  } while (inum>0);
  if (sgn==1)
    ans.push('-');

  return ans.reverse().join('');

},

// string to integer32
txtstr2simplei10: function(str) {

  var strarr=str.trim().toUpperCase().split('');
  var inum=0,res=1,sgn=1,cc=strarr[0].charCodeAt(0);

  if (this.ieq(cc,this.asciiminus))
    sgn=-1;

  var ii=0,jj=0,idx=-1,ll=strarr.length;

  for (ii=ll-1;ii>(sgn==1 ? -1 : 0);ii--)  {
    cc=strarr[ii].charCodeAt(0);
    for (jj=0;jj<10;jj++) {
      if (this.ieq(cc,this.basecharcode[jj])) {
        idx=jj;
        break;
      }
    }
    inum=parseInt(parseInt(inum,10)+parseInt(idx,10)*parseInt(res,10),10);
    res=parseInt(parseInt(res,10)*10,10);
  }

  return (parseInt(parseInt(sgn)*parseInt(inum),10));

},

// modify string to floating point number format
txt2float: function(str) {

  if (!str || str.length==0)
    return "NaN";

  var strarr=str.trim().split('');
  var ii=0,dpos=-1,epos=-1,mpos=-1,pos=-1,ll=strarr.length,llm=0,lle=0,sgn=0,esgn=0;
  var mant=[];
  var exp=[];
  var cc=strarr[0].charCodeAt(0);

  // tolerance - leading crap ignored
  while (this.ine(cc,this.asciie) && this.ine(cc,this.asciiE) && this.ine(cc,this.asciidec) &&
         this.ine(cc,this.asciiminus) && this.ine(cc,this.asciiplus) &&
         (this.ilt(cc,this.asciizero) || this.igt(cc,this.asciinine))) {
    strarr.shift();
    if (--ll==0)
      return "NaN";
    cc=strarr[0].charCodeAt(0);
  }

  // tolerance - trailing crap ignored
  cc=strarr[ll-1].charCodeAt(0);
  while (this.ilt(cc,this.asciizero) || this.igt(cc,this.asciinine)) {
    strarr.pop();
    if (--ll==0)
      return "NaN";
    cc=strarr[ll-1].charCodeAt(0);
  }

  // strip front sign
  cc=strarr[0].charCodeAt(0);
  if (this.ieq(cc,this.asciiminus) || this.ieq(cc,this.asciiplus)) {
    if (this.ieq(cc,this.asciiminus))
      sgn=1;
    strarr.shift();
    if (--ll==0)
      return "NaN";
  }

  // find exp position
  for (ii=0;ii<ll;ii++) {
    cc=strarr[ii].charCodeAt(0);
    if (this.ieq(cc,this.asciie) || this.ieq(cc,this.asciiE)) {
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
    cc=strarr[ii].charCodeAt(0);
    if (this.ieq(cc,this.asciidec)) {
      dpos=ii;
      break;
    }
  }

  // stop conversion at mantissa non-digit
  pos=(dpos>=0 ? dpos : mpos);
  for (ii=0;ii<pos;ii++) {
    cc=strarr[ii].charCodeAt(0);
    if (this.ilt(cc,this.asciizero) || this.igt(cc,this.asciinine)) {
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
    cc=strarr[ii].charCodeAt(0);
    if (this.ilt(cc,this.asciizero) || this.igt(cc,this.asciinine)) {
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
    cc=exp[0].charCodeAt(0);
    if (this.ieq(cc,this.asciiminus) || this.ieq(cc,this.asciiplus)) {
      if (this.ieq(cc,this.asciiminus))
        esgn=1;
      exp.shift();
      lle--;
    }
  }

  // discard all from exp non-digit
  for (ii=0;ii<lle;ii++) {
    cc=exp[ii].charCodeAt(0);
    if (this.ilt(cc,this.asciizero) || this.igt(cc,this.asciinine)) {
      exp.splice(ii,lle-ii);
      lle=ii;
      break;
    }
  }

  // strip exp leading zeros
  if (lle>0) {
    cc=exp[0].charCodeAt(0);
    while (this.ieq(cc,this.asciizero)) {
      exp.shift();
      if (--lle==0)
        break;
      cc=exp[0].charCodeAt(0);
    }
  }

  // dec as only mantissa char may not result to 0 for exp ??
  if (llm==1) {
    cc=mant[0].charCodeAt(0);
    if (this.ieq(cc,this.asciidec)) {
      if (lle==0)
        return "NaN";
      mant.splice(0,1,'1');
      dpos=-1;
    }
  }

  // strip mantissa leading zeros
  if (llm>0) {
    cc=mant[0].charCodeAt(0);
    while (this.ieq(cc,this.asciizero)) {
      mant.shift();
      if (--llm==0)
        return "0";
      cc=mant[0].charCodeAt(0);
    }
  }

  // strip mantissa trailing zeros
  if (llm>0 && dpos>=0) {
    cc=mant[llm-1].charCodeAt(0);
    while (this.ieq(cc,this.asciizero)) {
      mant.pop();
      llm--;
      cc=mant[llm-1].charCodeAt(0);
    }
    // dec at last mantissa char is useless
    cc=mant[llm-1].charCodeAt(0);
    if (this.ieq(cc,this.asciidec)) {
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

  // append exp if applicable
  if (lle>0) {
    mant.push('E');
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

// modify floating point format string to big integer format string
// digits after radix position
// number size (exp)
// sign
// given format (simple floating, exp)
txtabs: function(str,qqszsgnfe) {

  var strnum=this.txt2float(str);
  if (strnum=="NaN")
    return strnum;

  var strarr=strnum.split('');
  var ll=strarr.length;
  var mant=[];
  var exp=[];
  var cc=strarr[0].charCodeAt(0);
  var ii=0,esz=1,dpos=-1,epos=-1,mpos=-1,pos=-1,llm=0,lle=0,esgn=0;

  // strip front sign
  if (this.ieq(cc,this.asciiminus)) {
    qqszsgnfe[2]=parseInt(1,10);
    strarr.shift();
    ll--;
  }

  // find exp position
  for (ii=0;ii<ll;ii++) {
    cc=strarr[ii].charCodeAt(0);
    if (this.ieq(cc,this.asciie) || this.ieq(cc,this.asciiE)) {
      epos=ii;
      break;
    }
  }

  // alloc mant and exp
  if (epos<0)
    mant=strarr;
  else {
    qqszsgnfe[3]=parseInt(1,10);
    exp=strarr.slice(epos+1,ll);
    if (epos>0)
      mant=strarr.slice(0,epos);
  }

  llm=mant.length;
  lle=exp.length;

  // find dec position
  for (ii=0;ii<llm;ii++) {
    cc=mant[ii].charCodeAt(0);
    if (this.ieq(cc,this.asciidec)) {
      dpos=ii;
      break;
    }
  }

  // if exp format adjust qq to exp number
  if (lle>0) {
    cc=exp[0].charCodeAt(0);
    if (this.ieq(cc,this.asciiminus)) {
      esgn=1;
      exp.shift();
      lle--;
    }
    for (ii=lle-1;ii>-1;ii--) {
      cc=exp[ii].charCodeAt(0);
      qqszsgnfe[0]=parseInt(parseInt(qqszsgnfe[0],10)+
        parseInt(esz,10)*parseInt(parseInt(cc,10)-parseInt(this.asciizero,10),10),10);
      esz=parseInt(parseInt(esz,10)*parseInt(10,10),10);
    }
    if (esgn==1)
      qqszsgnfe[0]=parseInt(parseInt(qqszsgnfe[0],10)*parseInt(-1,10),10);
  }
  // first guess number size = exp
  qqszsgnfe[1]=qqszsgnfe[0];

  // add full mantissa length to number size
  if (dpos<0)
    qqszsgnfe[1]=parseInt(parseInt(qqszsgnfe[0],10)+llm-1,10);
  else {
    // add number of mantissa digits before radix to number size
    if (dpos>1)
      qqszsgnfe[1]=parseInt(parseInt(qqszsgnfe[1],10)+parseInt(dpos-1,10),10);
    // tiny number has zeros before and after radix
    else {
      cc=(dpos==1 ? mant[0].charCodeAt(0) : this.asciizero);
      if (this.ieq(cc,this.asciizero)) {
        qqszsgnfe[1]=parseInt(parseInt(qqszsgnfe[1],10)-1,10);
        for (ii=dpos+1;ii<llm;ii++) {
          cc=mant[ii].charCodeAt(0);
          if(this.ine(cc,this.asciizero))
            break;
          qqszsgnfe[1]=parseInt(parseInt(qqszsgnfe[1],10)-1,10);
        }
      }
    }
    // there are fraction digits so adjust qq
    qqszsgnfe[0]=parseInt(parseInt(qqszsgnfe[0],10)-(llm-dpos-1),10);
    mant.splice(dpos,1);
    llm--;
  }

  // unnessassary remove leading zeros
  cc=mant[0].charCodeAt(0);
  while (this.ieq(cc,this.asciizero)) {
    mant.shift();
    if (--llm==0)
      return '0';
    cc=mant[0].charCodeAt(0);
  }

  // if qq>0 add trailing zeros instead
  if (this.igt(qqszsgnfe[0],0)) {
    for (ii=0;ii<parseInt(qqszsgnfe[0],10);ii++) {
      mant.push('0');
      llm++;
    }
    qqszsgnfe[0]=0;
  }

  return mant.join('');

},

// before calc function call make first number the larger one
txtfixqq: function(calctype,qq12,strnum12) {

  var dfqq=parseInt(parseInt(qq12[0],10)-parseInt(qq12[1],10),10);

  if (calctype=='*' || dfqq==0)
    return;

  var ii=0,mod=0;

  if (dfqq<0) {
    dfqq=parseInt(parseInt(dfqq,10)*parseInt(-1,10),10);
    mod=1;
  }

  // swap numbers and add zeros in case
  var strarr=strnum12[mod].split('');
  for (ii=0;ii<dfqq;ii++)
    strarr.push('0');

  strnum12[mod]=this.trimint10(strarr.join(''));

},

// after calc function call undo fixqq
txtunfixqq: function(calctype,strnum,sz12,qq12) {

  var ii=0,ll=strnum.length,qq=0;

  if (calctype=='+' || calctype=='-') {
    qq=parseInt(qq12[0],10);
    if (this.ilt(qq12[1],qq12[0]))
      qq=parseInt(qq12[1],10);
    if (this.ilt(qq,0))
      qq=parseInt(parseInt(qq,10)*parseInt(-1,10),10);
  }
  else if (calctype=='*') {
    qq=parseInt(parseInt(qq12[0],10)+parseInt(qq12[1],10),10);
    if (this.ilt(qq,0))
      qq=parseInt(parseInt(qq,10)*parseInt(-1,10),10);
  }
  else if (calctype=='/')
    qq=parseInt(ll-(parseInt(sz12[0],10)-parseInt(sz12[1],10))-1,10);

  if (this.ile(qq,0))
    return strnum;

  var strarr=strnum.split('');
  ll=strarr.length;

  if (this.igt(ll,qq))
    strarr.splice(ll-qq,0,'.');
  else {
    while (this.ilt(ll,qq)) {
      strarr.unshift('0');
      qq=parseInt(qq,10)-1;
    }
    strarr.unshift('.');
  }

  return strarr.join('');

},

// calc add numbers
txtadd: function(strnum1,strnum2) {

  var strarr1=strnum1.split('');
  var strarr2=strnum2.split('');
  var ii=0,jj=0,kk=0,l1=strarr1.length,l2=strarr2.length;
  var c1=0,c2=0,c3=0,calc=0,carry=0;
  var strarr3=new Array(l1+1);

  // as doing on paper - walk from back to front adding digits and use carry in case
  for (ii=l1-1,jj=l2-1;ii>=0;ii--,jj--) {
    c1=parseInt(parseInt(strarr1[ii].charCodeAt(0),10)-parseInt(this.asciizero,10),10);
    c2=(jj<0 ? 0 : parseInt(parseInt(strarr2[jj].charCodeAt(0),10)-parseInt(this.asciizero,10)));
    calc=parseInt(parseInt(c1,10)+parseInt(c2,10),10);
    if (calc>=10) {
      carry=1;
      calc=parseInt(parseInt(calc,10)-10,10);
      kk=ii;
      while (carry>0 && kk>0) {
        kk--;
        c3=parseInt(parseInt(strarr1[kk].charCodeAt(0),10)-parseInt(this.asciizero,10)+parseInt(carry,10),10);
        if (c3>=10)
          c3=parseInt(parseInt(c3,10)-10,10);
        else
          carry=0;
        strarr1[kk]=String.fromCharCode(parseInt(parseInt(c3,10)+parseInt(this.asciizero,10),10));
      }
    }
    strarr3[ii+1]=String.fromCharCode(parseInt(parseInt(calc,10)+parseInt(this.asciizero,10),10));
  }
  if (carry>0)
    strarr3[0]='1';
  else strarr3.shift();

  return this.trimint10(strarr3.join(''));

},

// calc substract numbers
txtsubs: function(strnum1,strnum2) {

  var strarr1=strnum1.split('');
  var strarr2=strnum2.split('');
  var ii=0,jj=0,kk=0,l1=strarr1.length,l2=strarr2.length;
  var c1=0,c2=0,c3=0,calc=0,carry=0;
  var strarr3=new Array(l1);

  // as doing on paper - walk from back to front substracting digits and use carry in case
  for (ii=l1-1,jj=l2-1;ii>=0;ii--,jj--) {
    c1=parseInt(parseInt(strarr1[ii].charCodeAt(0),10)-parseInt(this.asciizero,10),10);
    c2=(jj<0 ? 0 : parseInt(parseInt(strarr2[jj].charCodeAt(0),10)-parseInt(this.asciizero,10),10));
    calc=parseInt(parseInt(c1,10)-parseInt(c2,10),10);
    if (calc<0) {
      carry=1;
      calc=parseInt(parseInt(calc,10)+10,10);
      kk=ii;
      while (carry>0 && kk>0) {
        kk--;
        c3=parseInt(parseInt(strarr1[kk].charCodeAt(0),10)-parseInt(this.asciizero,10)-parseInt(carry,10),10);
        if (c3<0)
          c3=parseInt(parseInt(c3,10)+10,10);
        else
          carry=0;
        strarr1[kk]=String.fromCharCode(parseInt(parseInt(c3,10)+parseInt(this.asciizero,10),10));
      }
    }
    strarr3[ii]=String.fromCharCode(parseInt(parseInt(calc,10)+parseInt(this.asciizero,10),10));
  }
  c3=strarr3[0].charCodeAt(0);
  while (this.ieq(c3,0)) {
    strarr3.shift();
    c3=strarr3[0].charCodeAt(0);
  }

  return this.trimint10(strarr3.join(''));

},

// calc multiply numbers
txtmult: function(strnum1,strnum2) {

  var strarr1=strnum1.split('');
  var strarr2=strnum2.split('');
  var ii=0,jj=0,kk=0,idx=0,l1=strarr1.length,l2=strarr2.length;
  var c1=0,c2=0,c3=0,calc=0,carry=0;
  var strarr3=new Array(l1+l2);

  for (ii=0;ii<(l1+l2);ii++)
    strarr3[ii]='0';

  // as doing on paper - walk from back to front multiplying digits and use carry in case
  for (ii=l1-1;ii>-1;ii--) {
    c1=parseInt(parseInt(strarr1[ii].charCodeAt(0),10)-parseInt(this.asciizero,10),10);
    for (jj=l2-1;jj>-1;jj--) {
      idx=ii+jj+1;
      c2=parseInt(parseInt(strarr2[jj].charCodeAt(0),10)-parseInt(this.asciizero,10),10);
      c3=parseInt(parseInt(strarr3[idx].charCodeAt(0),10)-parseInt(this.asciizero,10),10);
      calc=parseInt(parseInt(c1,10)*parseInt(c2,10)+parseInt(c3,10),10);
      if (calc>9) {
        carry=parseInt(parseInt(calc,10)/parseInt(10,10),10);
        calc=parseInt(parseInt(calc,10)-parseInt(carry,10)*parseInt(10,10),10);
        kk=idx;
        while (carry>0) {
          kk--;
          c3=parseInt(parseInt(strarr3[kk].charCodeAt(0),10)-parseInt(this.asciizero,10),10);
          c3=parseInt(parseInt(c3,10)+parseInt(carry,10),10);
          if (c3>9) {
            carry=parseInt(parseInt(c3,10)/parseInt(10,10),10);
            c3=parseInt(parseInt(c3,10)-parseInt(carry,10)*parseInt(10,10),10);
          }
          else
            carry=0;
          strarr3[kk]=String.fromCharCode(parseInt(parseInt(c3,10)+parseInt(this.asciizero,10),10));
        }
      }
      strarr3[idx]=String.fromCharCode(parseInt(parseInt(calc,10)+parseInt(this.asciizero,10),10));
    }
  }

  return this.trimint10(strarr3.join(''));

},

// calc dividing numbers
txtdiv: function(strnum1,strnum2,maxlen,corrsz) {

  var ii=0,jj=0,kk=0,idx=0,cmp=0,chklow=0,l1=strnum1.length,l2=strnum2.length;
  var zcmp=new Array();
  var beg=-1,end=-1;

  zcmp.length=l1;
  this.zeroarr(zcmp,beg,end);
  cmp=this.txtcmp(strnum1,zcmp.join(''));
  if (cmp==0)
    return '0';
  zcmp.length=l2;
  this.zeroarr(zcmp,beg,end);
  cmp=this.txtcmp(strnum2,zcmp.join(''));
  if (cmp==0 || maxlen===0)
    return 'NaN';

  var strarr3=new Array(maxlen);
  var strslice,xxx,inter='0';
  var mult=new Array('0');

  this.zeroarr(strarr3,beg,end);
  // cannot do any division due to ieee floating point calculation so build
  // num1-slice of length num2 and try multiplications
  strslice=strnum1.slice(0,l2<l1 ? l2 : l1);

  while (kk<maxlen) {
    if (chklow==0 && strslice.length==l2) {
      cmp=this.txtcmp(strslice,strnum2);
      if (cmp<0)
        corrsz[0]=1;
      chklow=1;
    }
    // try 1,2,...10 * num2 break if result < slice
    for (ii=1;ii<10;ii++) {
      jj=ii;
      mult[0]=String.fromCharCode(parseInt(parseInt(ii,10)+parseInt(this.asciizero,10),10));
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
    // pile up result
    strarr3[kk]=String.fromCharCode(parseInt(parseInt(jj,10)+parseInt(this.asciizero,10),10));
    xxx=[].join('');
    // substract intermediate result from slice
    xxx=this.txtsubs(strslice,inter);
    zcmp.length=xxx.length;
    this.zeroarr(zcmp,beg,end);
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
    // append next digit to slice from num1 or zero if exhausted and repeat
    xxx+=(l2+kk<l1 ? strnum1.charAt(l2+kk) : '0');
    strslice=[].join('');
    strslice=xxx.slice();
    inter='0';
    kk++;
  }

  if (kk==maxlen)
    kk--;

  return this.trimint10(strarr3.join('').substr(0,kk+1));

},

// round number to pos
txtround: function(strnum,pos) {

  var qqszsgnfe=new Array(0,0,0,0);
  var snum=this.txtabs(strnum,qqszsgnfe);

  if ((snum+'')=='NaN' || (snum+'')=='0')
    return snum;

  var ii=0,ll=0,llrd=snum.length,sz=parseInt(qqszsgnfe[1],10),rdpos=parseInt(pos,10)+sz;

  // no rounding if pos exceeds mantissa length
  if (rdpos>(llrd-2))
    return strnum;
  // rounded to zero
  if (rdpos<-1)
    return '0';

  var rdstrarr=snum.split('');
  llrd=rdstrarr.length;
  ii=rdpos+1;
  // do the simple digit round at pos
  var cc=parseInt(parseInt(rdstrarr[ii].charCodeAt(0),10)-parseInt(this.asciizero,10)+parseInt(5,10),10);

  // carry rounding
  while (cc>9 && ii>0) {
    ii--;
    cc=parseInt(parseInt(rdstrarr[ii].charCodeAt(0),10)-parseInt(this.asciizero,10)+parseInt(1,10),10);
    if (cc<10)
      rdstrarr[ii]=String.fromCharCode(cc+parseInt(this.asciizero,10));
    else
      rdstrarr[ii]='0';
  }
  if (cc==10)
    sz++;
  rdstrarr.splice(rdpos+1,llrd-rdpos-1);
  llrd=rdpos+1;

  var ans=[];

  if (sz<0) {
    ans.push('0');
    ans.push('.');
    ll+=2;
    for (ii=-1;ii>sz;ii--) {
      ans.push('0');
      ll++;
    }
    if (cc==10)
      ans.push('1');
    else
      ans=ans.concat(rdstrarr);
  }
  else {
    if (cc==10) {
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
    cc=ans[1].charCodeAt(0);
    if (this.ieq(cc,this.asciidec)) {
      cc=ans[0].charCodeAt(0);
      if (this.ieq(cc,this.asciizero))
        ans.shift();
    }
  }

  if (this.igt(qqszsgnfe[2],0))
    ans.unshift('-');

  return ans.join('');

},

txtsimple2exp: function(str) {

  var strnum=this.txt2float(str);
  if (strnum=="NaN")
    return "NaN";

  var strarr=str.split('');
  var ii=0,cc=0,dpos=-1,epos=-1,sc=0,scpos=-1,sgn=0,ll=strarr.length;

  for (ii=0;ii<ll;ii++) {
    cc=strarr[ii].charCodeAt(0);
    if (this.ieq(cc,this.asciiminus))
      sgn=1;
    else if (this.ieq(cc,this.asciidec))
      dpos=ii;
    else if (this.ieq(cc,this.asciie) || this.ieq(cc,this.asciiE)) {
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
      cc=strarr[ii].charCodeAt(0);
      if (this.ine(cc,this.asciizero)) {
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
    cc=strarr[ii].charCodeAt(0);
    if (this.ine(cc,this.asciizero)) {
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
  var ii=0,cc=0,expnum=0,dpos=-1,epos=-1,sc=0,sgn=0,ll=strarr.length,lle=0,llm=0;
  var mant=[];
  var exp=[];
  var ans=[];

  for (ii=0;ii<ll;ii++) {
    cc=strarr[ii].charCodeAt(0);
    if (this.ieq(cc,this.asciiminus))
      sgn=1;
    else if (this.ieq(cc,this.asciie) || this.ieq(cc,this.asciiE)) {
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
  expnum=parseInt(this.txtstr2simplei10(exp.join('')),10);
  sc=expnum;

  for (ii=0;ii<llm;ii++) {
    cc=mant[ii].charCodeAt(0);
    if (this.ieq(cc,this.asciidec)) {
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
    if (dpos>0) {
      sc+=dpos;
      if (sc<0)
        sc--
      mant.splice(dpos,1);
      llm--;
    }
    else {
      sc--;
      mant.shift();
      llm--;
      cc=mant[0].charCodeAt(0);
      while (this.ieq(cc,this.asciizero)) {
        sc--;
        mant.shift();
        if (--llm==0)
          break;
        cc=mant[0].charCodeAt(0);
      }
    }
  }

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
  var cc=strnum1[0].charCodeAt(0);
  if (this.ieq(cc,this.asciiminus))
    sgn1=1;
  cc=strnum2[0].charCodeAt(0);
  if (this.ieq(cc,this.asciiminus))
    sgn2=1;

  var ans=new Array(strnum1.substr(sgn1,ll1-sgn1),strnum2.substr(sgn2,ll2-sgn2));

  var strcalc=this.txtcalc('/',ans[0],ans[1],maxlen);

  var ii=0,dpos=-1,ll=strcalc.length;
  ll1=ans[0].length;
  ll2=ans[1].length;

  cc=strcalc[0].charCodeAt(0);
  if (this.ieq(cc,this.asciizero) || this.ieq(cc,this.asciidec)) {
    ans[1]=ans[0].substr(0,ll1);
    ans[0]=(sgn1!=sgn2 ? '-0' : '0');
    if (sgn1>0)
      ans[1].unshift('-');
    return ans;
  }

  for (ii=0;ii<ll;ii++) {
    cc=strcalc[ii].charCodeAt(0);
    if (this.ieq(cc,this.asciidec)) {
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
  var ans=new Array(-1,-1);
  if (ll==0)
    return ans[0];
  if (pref)
    ans[1]=parseInt(pref,10);
  var cc=strarr[0].charCodeAt(0);

  if (this.ilt(ans[1],2) || this.ige(ans[1],this.maxbase))
    ans[1]=-1;

  if (this.ieq(cc,this.asciiminus)) {
    strarr.shift();
    if (--ll==0)
      return ans[0];
  }

  cc=strarr[0].charCodeAt(0);
  if (this.ieq(cc,this.asciizero)) {
    strarr.shift();
    if (--ll==0) {
      ans[0]=0;
      return ans[0];
    }
    cc=strarr[0].charCodeAt(0);
    if (this.ieq(cc,this.asciiX)) {
      if (this.ieq(ans[1]-1))
        ans[1]=16;
      if (this.ieq(ans[1],16)) {
        strarr.shift();
        ll--;
      }
    }
    else if (this.ieq(ans[1],-1))
      ans[1]=8;
  }
  else if (this.ieq(cc,this.asciihash)) {
    if (this.ieq(ans[1],-1))
      ans[1]=16;
    if (this.ieq(ans[1],16)) {
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
    cc=strarr[ii].charCodeAt(0);
    chk=1;
    for (jj=0;jj<this.maxbase;jj++) {
      if (this.ieq(cc,this.basecharcode[jj])) {
        if (this.igt(jj,ans[0]))
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

  if (this.ieq(ans[0],0))
    return ans[0];

  if (this.ine(ans[0],-1))
    ans[0]=parseInt(ans[0],10)+1;

  var chk10=-1;
  var qqszsgnfe=new Array(0,0,0,0);
  var snum=this.txtabs(strnum,qqszsgnfe);
  if (snum!='NaN')
    chk10=(this.ieq(qqszsgnfe[0],0) ? 10 : 100);

  if (this.ieq(ans[1],10)) {
    if (this.ile(ans[0],10) || this.ieq(ans[0],15))
      ans[0]=chk10;
    return ans[0];
  }

  if (this.ine(ans[1],-1)) {
    if (this.ine(ans[0],-1) && this.ile(ans[0],ans[1]))
      return ans[1];
    ans[0]=-1;
    return ans[0];
  }

  if (this.ieq(ans[0],-1) || this.ieq(ans[0],10)) {
    ans[0]=chk10;
    return ans[0];
  }

  if (this.ige(ans[0],16) || this.ieq(ans[0],2) || this.ieq(ans[0],8))
    return ans[0];

  if (this.igt(ans[0],10))
    ans[0]=16;

  return ans[0];

},

txtconvfmt: function(strnum,fmt12,maxlen) {

  var ll=strnum.length;
  if (ll==0)
    return strnum;

  var ans=[],strarr=[],modans=[];
  var strfmt,strshacc,strnum10,strtmp;
  var cc=0,ii=0,jj=0,idx=-1,sgn=0,cmp=0,beg=-1,end=-1;
  if (this.ilt(fmt12[1],2) || this.ige(fmt12[1],this.maxbase))
    fmt12[1]=10;

  fmt12[0]=this.txtchknumfmt(strnum,fmt12[0]);

  if (this.ieq(fmt12[0],-1) || this.ieq(fmt12[0],100))
    return strnum;

  if (this.ieq(fmt12[0],fmt12[1]))
    return strnum;

  if (this.ieq(fmt12[0],10))
    strarr=this.txtexp2simple(strnum).trim().toUpperCase().split('');
  else
    strarr=strnum.trim().toUpperCase().split('');
  ll=strarr.length;

  cc=strarr[0].charCodeAt(0);
  if (this.ieq(cc,this.asciiminus)) {
    strarr.shift();
    if (--ll==0)
      return strnum;
    sgn=1;
    cc=strarr[0].charCodeAt(0);
  }

  if (this.ine(fmt12[0],10)) {
    if (this.ieq(cc,this.asciizero)) {
      strarr.shift();
      if (--ll==0)
        return strnum;
      cc=strarr[0].charCodeAt(0);
      if (this.ieq(cc,this.asciiX)) {
        if (this.ieq(fmt12[0],16)) {
          strarr.shift();
          if (--ll==0)
            return strnum;
        }
      }
    }
    else if (this.ieq(cc,this.asciihash)) {
      if (this.ieq(fmt12[0],16)) {
        strarr.shift();
        if (--ll==0)
          return strnum;
      }
    }
    strfmt=this.txtsimplei102str(fmt12[0]);
    strnum10='0';
    strshacc='1';
    for (ii=ll-1;ii>-1;ii--)  {
      cc=strarr[ii].charCodeAt(0);
      for (jj=0;jj<fmt12[0];jj++) {
        if (this.ieq(cc,this.basecharcode[jj])) {
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
    ans[idx++]=
      String.fromCharCode(parseInt(this.basecharcode[parseInt(this.txtstr2simplei10(modans[1]),10)],10));
    strnum10=modans[0].slice();
    strarr.length=strnum10.length;
    this.zeroarr(strarr,beg,end);
    cmp=this.txtcmp(strnum10,strarr.join(''));
  } while (cmp==1);
  if (sgn==1)
    ans[idx++]='-';
  ans.length=idx;

  return ans.reverse().join('');


},

txtcalc: function(calctype,strnum1,strnum2,maxlen) {

  if (calctype!='+' && calctype!='-' && calctype!='*' && calctype!='/')
    return "NaN";

  var ii=0,cmp=0;
  var addsubstype='';
  var ans="NaN";

  var qqszsgnfe1=new Array(0,0,0,0);
  var snum1=this.txtabs(strnum1,qqszsgnfe1);

  var qqszsgnfe2=new Array(0,0,0,0);
  var snum2=this.txtabs(strnum2,qqszsgnfe2);
  var qq=new Array(qqszsgnfe1[0],qqszsgnfe2[0]);
  var sz=new Array(qqszsgnfe1[1],qqszsgnfe2[1]);
  var corrdiv=new Array('0');
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
    sz[0]=parseInt(parseInt(sz[0],10)-parseInt(corrdiv[0],10),10);
  }
  else {
    addsubstype='+';
    if ((calctype=='+' && this.ine(qqszsgnfe1[2],qqszsgnfe2[2])) ||
        (calctype=='-' && this.ieq(qqszsgnfe1[2],qqszsgnfe2[2])))
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

  if ((addsubstype.length==0 && this.ine(qqszsgnfe1[2],qqszsgnfe2[2])) ||
      (addsubstype.length>0 && ((calctype=='+' &&
      ((this.igt(qqszsgnfe1[2],0) && this.igt(qqszsgnfe2[2],0)) ||
      (this.igt(qqszsgnfe1[2],0) && this.ieq(qqszsgnfe2[2],0) && cmp>0) ||
       (this.igt(qqszsgnfe2[2],0) && this.ieq(qqszsgnfe1[2],0) && cmp<0))) ||
       (calctype=='-' &&
       ((this.igt(qqszsgnfe1[2],0) && this.ieq(qqszsgnfe2[2],0)) ||
       (this.ieq(qqszsgnfe1[2],0) && this.ieq(qqszsgnfe2[2],0) && cmp<0) ||
       (this.igt(qqszsgnfe1[2],0) && this.igt(qqszsgnfe2[2],0) && cmp<0)))))) {
    ans=('-'+ans);
  }

  return ans;

}

}

var combobase1=10,combobase2=10;

function checkBase1(evt) {

  var kevt=(evt || window.event);
  var key=(kevt.keyCode || kevt.which);

  if (textcalc.ieq(key,textcalc.asciibackspace) ||
      textcalc.ieq(key,textcalc.asciidel))
    return;

  key=String.fromCharCode(key);
  var strnum1=document.getElementById("number1").value;
  var strnum1chk=(strnum1+key);
  var combo1=combobase1;
  var ans=textcalc.txtchknumfmt(strnum1chk,combo1);
  if (textcalc.ieq(ans,100))
    ans=10;

  if (textcalc.ine(combo1,ans) && textcalc.ine(ans,0)) {
    kevt.returnValue=false;
    if(kevt.preventDefault)
      kevt.preventDefault();
  }

}

function checkBase2(evt) {

  var kevt=(evt || window.event);
  var key=(kevt.keyCode || kevt.which);

  if (textcalc.ieq(key,textcalc.asciibackspace) ||
      textcalc.ieq(key,textcalc.asciidel))
    return;

  key=String.fromCharCode(key);
  var strnum2=document.getElementById("number2").value;
  var strnum2chk=(strnum2+key);
  var combo2=combobase2;
  var ans=textcalc.txtchknumfmt(strnum2chk,combo2);
  if (textcalc.ieq(ans,100))
    ans=10;

  if (textcalc.ine(combo2,ans) && textcalc.ine(ans,0)) {
    kevt.returnValue=false;
    if(kevt.preventDefault)
      kevt.preventDefault();
  }

}

function convBase1(form) {

  var idx=form.base1.options.selectedIndex;
  var strbase1=form.base1.options[idx].text;
  var strnum1=form.number1.value;

  if (typeof strnum1=='undefined') {
    combobase1=strbase1;
    return;
  }

  var fmt12=new Array(combobase1,strbase1);
  if (textcalc.ieq(fmt12[0],fmt12[1]))
    return;
  var strmaxlen=form.maxlen.value;
  var strnumconv=textcalc.txtconvfmt(strnum1,fmt12,strmaxlen);

  if (textcalc.igt(fmt12[0],1) && textcalc.ilt(fmt12[0],37)) {
    form.elements["number1"].value=strnumconv;
    combobase1=fmt12[1];
  }
  else
    form.base1.options.selectedIndex=combobase1-2;

}

function convBase2(form) {

  var idx=form.base2.options.selectedIndex;
  var strbase2=form.base2.options[idx].text;
  var strnum2=form.number2.value;
  if (typeof strnum2=='undefined') {
    combobase2=strbase2;
    return;
  }

  var fmt12=new Array(combobase2,strbase2);
  if (textcalc.ieq(fmt12[0],fmt12[1]))
    return;
  var strmaxlen=form.maxlen.value;
  var strnumconv=textcalc.txtconvfmt(strnum2,fmt12,strmaxlen);
  if (textcalc.igt(fmt12[0],1) && textcalc.ilt(fmt12[0],37)) {
    form.elements["number2"].value=strnumconv;
    combobase2=fmt12[1];
  }
  else
    form.base2.options.selectedIndex=combobase2-2;

}

function calc3(form) {

  var strmaxlen=form.maxlen.value;
  if (textcalc.ilt(strmaxlen,4)) {
    form.elements["maxlen"].value='4';
    strmaxlen='4';
  }

  var strnum1=form.number1.value;
  var idx=form.base1.options.selectedIndex;
  var strbase1=form.base1.options[idx].text;
  var fmt12=new Array(strbase1,10);
  if (textcalc.ine(fmt12[0],fmt12[1]))
    strnum1=textcalc.txtconvfmt(strnum1,fmt12,strmaxlen);

  var strnum2=form.number2.value;
  idx=form.base2.options.selectedIndex;
  var strbase2=form.base2.options[idx].text;
  fmt12[0]=strbase2;
  if (textcalc.ine(fmt12[0],fmt12[1]))
    strnum2=textcalc.txtconvfmt(strnum2,fmt12,strmaxlen);

  idx=form.operator.options.selectedIndex;
  var strop=form.operator.options[idx].text;
  var strres=textcalc.txtcalc(strop,strnum1,strnum2,strmaxlen);

  form.elements["result"].value=strres;

}

function checkMaxlen(evt) {

  var kevt=(evt || window.event);
  var key=(kevt.keyCode || kevt.which);
  key=String.fromCharCode(key);

  var strmaxlen=document.getElementById("maxlen").value;
  var strmaxlenchk=(strmaxlen+key);

  if (textcalc.ilt(strmaxlenchk,1) || textcalc.igt(strmaxlenchk,999)) {
    kevt.returnValue=false;
    if(kevt.preventDefault)
      kevt.preventDefault();
  }

}

function roundResult(form) {

  var strrd=form.roundresult.value;
  var strres=form.result.value;
  if (typeof strrd!='undefined' && typeof strres!='undefined') {
    var ans=textcalc.txtround(strres,strrd);
    form.elements["result"].value=ans;
  }

}

function f2e(form) {

  var strres=form.result.value;
  if (typeof strres!='undefined') {
    var ans=textcalc.txtsimple2exp(strres);
    form.elements["result"].value=ans;
  }

}

function e2f(form) {

  var strres=form.result.value;
  if (typeof strres!='undefined') {
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
