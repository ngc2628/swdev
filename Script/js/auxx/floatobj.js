
var floatobj={};

floatobj.asciihash=35;
floatobj.asciiplus=43;
floatobj.asciiminus=45;
floatobj.asciidec=46;
floatobj.asciizero=48;
floatobj.asciione=49;
floatobj.asciifive=53;
floatobj.asciinine=57;
floatobj.asciiE=69;

floatobj.rad=0.0174532925199433; // 0.0174532925199432957694;

floatobj.exp2simple=function(str) {
  if (typeof str!='string')
    str=str.toString();
  str=str.trim().toUpperCase();
  var ii=0,ll=str.length,expsgn=1,idxsgn=0,idxdec=-1,idxexp=-1,idxexpsgn=-1;
  if (ll==0)
    return '0';
  var ans=[];
  var intg=[];
  var frac=[];
  var exp=[];
  if (str.charCodeAt(0)==floatobj.asciiplus || str.charCodeAt(0)==floatobj.asciiminus) {
    if (str.charCodeAt(0)==floatobj.asciiminus)
      ans.push(String.fromCharCode(floatobj.asciiminus));
    idxsgn=1;
  }
  for (ii=idxsgn;ii<ll;ii++) {
    if (str.charCodeAt(ii)==floatobj.asciidec) {
      if (idxdec>=0 || idxexp>=0)
        break;
      idxdec=ii;
      continue;
    }
    if (str.charCodeAt(ii)==floatobj.asciiE) {
      if (idxexp>=0)
        break;
      idxexp=ii;
      idxdec=-1;
      continue;
    }
    if (str.charCodeAt(ii)==floatobj.asciiplus || str.charCodeAt(ii)==floatobj.asciiminus) {
      if (idxexp<0 || idxexpsgn>=0)
        break;
      idxexpsgn=ii;
      if (str.charCodeAt(ii)==floatobj.asciiminus)
        expsgn=-1;
      continue;
    }
    if (str.charCodeAt(ii)<floatobj.asciizero || str.charCodeAt(ii)>floatobj.asciinine)
      break;
    if (idxdec>=0)
      frac.push(str.charCodeAt(ii));
    else if (idxexp>=0)
      exp.push(str.charCodeAt(ii));
    else
      intg.push(str.charCodeAt(ii));
  }
  while (exp.length>1 && exp[0]==floatobj.asciizero)
    exp.shift();
  while (intg.length>1 && intg[0]==floatobj.asciizero)
    intg.shift();
  var expnumstr='0';
  if (exp.length>0)
    expnumstr=String.fromCharCode.apply(String,exp);
  var expnum=parseInt(expnumstr,10);
  if (expsgn==1) {
    while (frac.length>0 && expnum>0) {
      intg.push(frac.shift());
      expnum-=1;
    }
    while (expnum>0) {
      intg.push(floatobj.asciizero);
      expnum-=1;
    }
  }
  else {
    while (intg.length>0 && expnum>0) {
      frac.unshift(intg.pop());
      expnum-=1;
    }
    while (expnum>0) {
      frac.unshift(floatobj.asciizero);
      expnum-=1;
    }
  }
  if (intg.length==0)
    ans.push(String.fromCharCode(floatobj.asciizero));
  for (ii=0;ii<intg.length;ii++)
    ans.push(String.fromCharCode(intg[ii]));
  while (frac.length>0 && frac[frac.length-1]==floatobj.asciizero)
    frac.pop();
  if (frac.length>0)
    ans.push(String.fromCharCode(floatobj.asciidec));
  for (ii=0;ii<frac.length;ii++)
    ans.push(String.fromCharCode(frac[ii]));
  return ans.join('');
};

floatobj.simple2exp=function(str) {
  if (typeof str!='string')
    str=str.toString();
  str=str.trim().toUpperCase();
  var ii=0,ll=str.length,idxdec=-1,idxsgn=0,expnum=0;
  if (ll==0)
    return '0';
  var ans=[];
  var intg=[];
  var frac=[];
  if (str.charCodeAt(0)==floatobj.asciiplus || str.charCodeAt(0)==floatobj.asciiminus) {
    if (str.charCodeAt(0)==floatobj.asciiminus)
      ans.push(floatobj.asciiminus);
    idxsgn=1;
  }
  for (ii=idxsgn;ii<ll;ii++) {
    if (str.charCodeAt(ii)==floatobj.asciidec) {
      if (idxdec>=0)
        break;
      idxdec=ii;
      continue;
    }
    if (str.charCodeAt(ii)<floatobj.asciizero || str.charCodeAt(ii)>floatobj.asciinine)
      break;
    if (idxdec>=0)
      frac.push(str.charCodeAt(ii));
    else
      intg.push(str.charCodeAt(ii));
  }
  while (intg.length>1 && intg[0]==floatobj.asciizero)
    intg.shift();
  while (frac.length>0 && frac[frac.length-1]==floatobj.asciizero)
    frac.pop();
  if (intg.length==0 || intg[0]==floatobj.asciizero) { // <1
    expnum-=1;
    while (frac.length>0 && frac[0]==floatobj.asciizero) {
      frac.shift();
      expnum-=1;
    }
    intg.shift();
  }
  else // >1
    expnum=intg.length-1;
  if (intg.length>0)
    intg.splice(1,0,floatobj.asciidec);
  else if (frac.length>0)
    frac.splice(1,0,floatobj.asciidec);
  else 
    return '0';
  ans=ans.concat(intg,frac);
  if (expnum!=0)
    ans.push(floatobj.asciiE);
  var strans=String.fromCharCode.apply(String,ans); 
  if (expnum!=0)
    strans+=expnum.toString();
  return strans;
};

floatobj.roundpos=function(num,pos,epspos) {
  num=intobj.chknumber(num,NaN);
  pos=intobj.chknumber(pos,NaN);
  if (isNaN(num) || isNaN(pos))
    return num;
  var strnum=num.toString();
  strnum=floatobj.exp2simple(strnum);
  var stri=strnum.split('');
  var sgn=1;
  if (stri[0].charCodeAt(0)==floatobj.asciiminus) {
    stri.shift();
    sgn=-1;
  }
  var ii=0,ll=stri.length,phony1=0,idx=stri.indexOf(String.fromCharCode(floatobj.asciidec));
  if (idx<0)
    idx=ll;
  if ((idx+pos)>=ll)
    return num;
  if (idx==1 && stri[0].charCodeAt(0)==floatobj.asciizero) {
    stri[0]=String.fromCharCode(floatobj.asciione);
    phony1=1;
  }
  if (idx<ll) {
    stri.splice(idx,1);
    ll-=1;
  }
  var chk=[];
  for (ii=0;ii<(idx+pos);ii++)
    chk.push(floatobj.asciizero);
  if (chk.length<stri.length)
    chk.push(floatobj.asciifive);
  epspos=intobj.chknumber(epspos,4);
  for (ii=0;chk.length<stri.length;ii++)
    chk.push(ii==epspos ? floatobj.asciione : floatobj.asciizero);
  var strinum=stri.join(''),strchk=String.fromCharCode.apply(String,chk); 
  var strrd=intobj.addbits(intobj.str2bits(strinum,10),intobj.str2bits(strchk,10)); 
  chk=intobj.bits2str(strrd,10).split('');
  if (chk.length>stri.length) // add overflow
    idx++;
  if ((idx+pos)<=0)
    return 0;
  chk.splice(idx,0,String.fromCharCode(floatobj.asciidec));
  if (phony1==1) {
    if (chk[0].charCodeAt(0)==floatobj.asciione)
      chk[0]=String.fromCharCode(floatobj.asciizero);
    else
      chk[0]=String.fromCharCode(floatobj.asciione);
  }
  if (pos>=0)
    chk=chk.slice(0,idx+pos+1);
  else {
    chk=chk.slice(0,idx+pos);
    for (ii=pos;ii<0;ii++)
      chk.push(String.fromCharCode(floatobj.asciizero));
  }
  if (sgn<0)
    chk.unshift(String.fromCharCode(floatobj.asciiminus));
  var ans=chk.join('');
  return parseFloat(ans); 
};

floatobj.rounddown=function(num,pos) {
  var ans=floatobj.roundpos(num,pos);
  if (isNaN(ans))
    return NaN;
  if (ans>num) {
    num-=intobj.ipow10(-pos);
    ans=floatobj.roundpos(num,pos);
  }
  return ans;
};

floatobj.roundup=function(num,pos) {
  var ans=floatobj.roundpos(num,pos);
  if (isNaN(ans))
    return NaN;
  if (ans<num) {
    num+=intobj.ipow10(-pos);
    ans=floatobj.roundpos(num,pos);
  }
  return ans;
};

floatobj.sprintf=function(val,fdigit,fmt) {
  var ans='NaN';
  if (typeof val!='number' || isNaN(val))
    return ans;
  if (fdigit==0 && (typeof fmt=='string') && fmt=='d') {
    ans=val.toString();
    var idx=ans.indexOf('.');
    if (idx>=0)
      ans=ans.substring(0,idx);
    return ans;
  }
  var sc=intobj.scale(val,10);
  fdigit=intobj.chknumber(fdigit,sc);
  if (fdigit>sc+14)
    fdigit=sc+14;
  var rdval=floatobj.roundpos(val,fdigit);
  ans=rdval.toString();
  if (typeof fmt!='string' || (fmt!='f' && fmt!='e')) {
    fmt='f';
    absrdval=intobj.fabs(rdval);
    if (absrdval>intobj.ipow10(6) || absrdval<intobj.ipow10(-6))
      fmt='e';
  }
  if (fmt=='e')
    ans=floatobj.simple2exp(ans);
  return ans;
};

floatobj.ieee2bits=function(dnum) {
  var ans=[];
  var str='';
  var bias=1023,fbits=52,ebits=11,ee=0,ii=0,idx=0;
  var ff=.0,c1=2.225073858507201e-308, // 2^1-bias
      c2=4.494232837155790e+307, // 2^bias-1
      c3=4.503599627370496e+15; // 2^fbits
  dnum=intobj.chknumber(dnum,NaN);
  if (!isNaN(dnum) && (dnum===-Infinity || dnum===-.0 || dnum<.0))
    idx=1;
  ans.push(String.fromCharCode(intobj.basecharcode[idx]));
  dnum=intobj.fabs(dnum);
  if (isNaN(dnum) || dnum==Infinity || dnum==0) {
    var cnt=(isNaN(dnum) ? 13 : (dnum==Infinity ? 12 : 0));
    for (ii=1;ii<64;ii++) {
      idx=(ii<cnt ? 1 : 0);
      ans.push(String.fromCharCode(intobj.basecharcode[idx]));
    }
    return ans.join('');
  }
  if (dnum>=c1) {
    ee=intobj.scale(dnum,2);
    ff=dnum*intobj.ipow2(-ee);
    if (ff<1.) {
      ff*=2.;
      ee-=1;
    }
    ff=c3*(ff-1.);
    ee+=bias;
  }
  else
    ff=((dnum*c2)*c3);
  str=intobj.str2bits(ee.toString(),10);
  for (ii=0;ii<ebits-str.length;ii++)
    ans.push(String.fromCharCode(intobj.basecharcode[0]));
  for (ii=0;ii<str.length;ii++)
    ans.push(str.charAt(ii));
  str=intobj.str2bits(ff.toString(),10);
  for (ii=0;ii<fbits-str.length;ii++)
    ans.push(String.fromCharCode(intobj.basecharcode[0]));
  for (ii=0;ii<str.length;ii++)
    ans.push(str.charAt(ii));
  return ans.join('');
};

floatobj.bits2ieee=function(str) {
  if (typeof str!='string')
    return NaN;
  var bias=1023,fbits=52,ebits=11,
      ii=0,ll=str.length,ss=(str.charCodeAt(0)==intobj.basecharcode[0] ? 1 : -1);
  for (ii=1;ii<ebits+1;ii++) {
    if (str.charCodeAt(ii)==intobj.basecharcode[0])
      break;
  }
  if (ii>ebits) {
    for (ii=ebits+1;ii<ll;ii++) {
      if (str.charCodeAt(ii)==intobj.basecharcode[1])
        return (ss==1 ? NaN : -NaN);
    }
    return (ss==1 ? Infinity : -Infinity); 
  }
  var c1=2.225073858507201e-308, // 2^1-bias
      c2=2.220446049250313E-16; // 2^-fbits
  var ee=intobj.bits2num(str.substring(1,ebits+1));
  var ff=intobj.bits2num(str.substring(ebits+1));
  var dnum=.0;
  if (ee>0)
    dnum=intobj.ipow2(ee-bias)*(1.+ff*c2); 
  else if (ff!==0) 
    dnum=c1*(ff*c2);
  return ss*dnum;
};

floatobj.diff=function(dnum1,dnum2,blur) {
  dnum1=intobj.chknumber(dnum1,NaN),dnum2=intobj.chknumber(dnum2,NaN);
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
  blur=intobj.chknumber(blur,NaN); 
  if (!isNaN(blur)) {
   if (intobj.fabs(ans)<intobj.fabs(blur))
     return 0; 
  }
  return dnum1-(dnum1-ans);  
};

floatobj.cmp=function(dnum1,dnum2,blur) {
  var df=floatobj.diff(dnum1,dnum2,blur);
  if (df==Infinity || df>0)
    return 1;
  if (df==-Infinity || df<0)
    return -1;
  return 0;
};

floatobj.Vertex=function(xx,yy,zz,ww) {
  this.xyzw=[
    intobj.chknumber(xx,NaN),intobj.chknumber(yy,NaN),
    intobj.chknumber(zz,NaN),intobj.chknumber(ww,NaN)
  ];
};

floatobj.Vertex.prototype.val=function(idx) {
  var ans=intobj.chknumberrange(idx,intobj.typeX,intobj.typeW);
  if (ans==aux.ansok)
    return this.xyzw[intobj.cutfloat(idx)];
  return NaN;
};

floatobj.Vertex.prototype.setVal=function(idx,val) {
  var ans=intobj.chknumberrange(idx,intobj.typeX,intobj.typeW);
  if (ans==aux.ansok)
    this.xyzw[intobj.cutfloat(idx)]=intobj.chknumber(val,NaN);
};

floatobj.Vertex.prototype.toJson=function(indent) {
  var ans=aux.qdecl('xyzw');
  var varr=(isNaN(this.xyzw[0]) ? aux.qany('null') : aux.qword(this.xyzw[0]));
  varr+=aux.qnextprop();
  varr+=(isNaN(this.xyzw[1]) ? aux.qany('null') : aux.qword(this.xyzw[1]));
  varr+=aux.qnextprop();
  varr+=(isNaN(this.xyzw[2]) ? aux.qany('null') : aux.qword(this.xyzw[2]));
  varr+=aux.qnextprop();
  varr+=(isNaN(this.xyzw[3]) ? aux.qany('null') : aux.qword(this.xyzw[3]));
  ans+=aux.qarr(varr);
  if (typeof indent=='string')
    ans=(indent+ans);
  return ans;
};

floatobj.vectorsubs=function(vertex1,vertex2) {
  var ans=new floatobj.Vertex();
  if ( ! (vertex1 instanceof floatobj.Vertex) || ! (vertex2 instanceof floatobj.Vertex))
    return ans;
  var pp1=[
    intobj.chknumber(vertex1.xyzw[0],NaN),
    intobj.chknumber(vertex1.xyzw[1],NaN),
    intobj.chknumber(vertex1.xyzw[2],NaN)
  ];
  var pp2=[
    intobj.chknumber(vertex2.xyzw[0],NaN),
    intobj.chknumber(vertex2.xyzw[1],NaN),
    intobj.chknumber(vertex2.xyzw[2],NaN)
  ];
  ans=new floatobj.Vertex(
    isNaN(pp1[0]) || isNaN(pp2[0]) ? NaN : pp1.xyzw[0]-pp2.xyzw[0],
    isNaN(pp1[1]) || isNaN(pp2[1]) ? NaN : pp1.xyzw[1]-pp2.xyzw[1],
    isNaN(pp1[2]) || isNaN(pp2[2]) ? NaN : pp1.xyzw[2]-pp2.xyzw[2]
  );
  return ans;
};

floatobj.vectorlen=function(vertex) {
  if ( ! (vertex instanceof floatobj.Vertex))
    return 0;
  var pp=[
    intobj.chknumber(vertex.xyzw[0],0),
    intobj.chknumber(vertex.xyzw[1],0),
    intobj.chknumber(vertex.xyzw[2],0)
  ];
  var ans=pp[0]*pp[0]+pp[1]*pp[1]+pp[2]*pp[2];
  return Math.sqrt(ans);
}; 

floatobj.vectordot=function(vertex1,vertex2) {
  if ( ! (vertex1 instanceof floatobj.Vertex) || 
       ! (vertex2 instanceof floatobj.Vertex))
    return NaN;
  var pp=[
    [intobj.chknumber(vertex1.xyzw[0],0),
     intobj.chknumber(vertex1.xyzw[1],0),
     intobj.chknumber(vertex1.xyzw[2],0)
    ],
    [intobj.chknumber(vertex2.xyzw[0],0),
     intobj.chknumber(vertex2.xyzw[1],0),
     intobj.chknumber(vertex2.xyzw[2],0)
    ]
  ]; 
  var ans=pp[0][0]*pp[1][0]+pp[0][1]*pp[1][1]+pp[0][2]*pp[1][2]; 
  return ans;
};

floatobj.vectornorm=function(vertex) {
  var ans=new floatobj.Vertex();
  if ( ! (vertex instanceof floatobj.Vertex))
    return ans;
  var nn=floatobj.vectorlen(vertex);
  if (nn<=0)
    return ans;
  if (!isNaN(vertex.xyzw[0]))
    ans.xyzw[0]=vertex.xyzw[0]/nn;
  if (!isNaN(vertex.xyzw[1]))
    ans.xyzw[1]=vertex.xyzw[1]/nn;
  if (!isNaN(vertex.xyzw[2]))
    ans.xyzw[2]=vertex.xyzw[2]/nn;
  return ans;
};

floatobj.vectorcross=function(vertex1,vertex2) {
  var ans=new floatobj.Vertex();
  if ( ! (vertex1 instanceof floatobj.Vertex) || ! (vertex2 instanceof floatobj.Vertex))
    return ans;
  var pp1=[
    intobj.chknumber(vertex1.xyzw[0],NaN),
    intobj.chknumber(vertex1.xyzw[1],NaN),
    intobj.chknumber(vertex1.xyzw[2],NaN)
  ];
  var pp2=[
    intobj.chknumber(vertex2.xyzw[0],NaN),
    intobj.chknumber(vertex2.xyzw[1],NaN),
    intobj.chknumber(vertex2.xyzw[2],NaN)
  ];
  if (!isNaN(pp1[1]) && !isNaN(pp2[1]) && !isNaN(pp1[2]) && !isNaN(pp2[2]))
    ans.xyzw[0]=pp1[1]*pp2[2]-pp1[2]*pp2[1];
  if (!isNaN(pp1[0]) && !isNaN(pp2[0]) && !isNaN(pp1[2]) && !isNaN(pp2[2]))
    ans.xyzw[1]=pp1[2]*pp2[0]-pp1[0]*pp2[2];
  if (!isNaN(pp1[0]) && !isNaN(pp2[0]) && !isNaN(pp1[1]) && !isNaN(pp2[1]))
    ans.xyzw[2]=pp1[0]*pp2[1]-pp1[1]*pp2[0];
  return ans;  
};

floatobj.angrad=function(vertex1,vertex2) {
  if ( ! (vertex1 instanceof floatobj.Vertex) || 
       ! (vertex2 instanceof floatobj.Vertex))
    return 0;
  var den=floatobj.vectorlen(vertex1)*floatobj.vectorlen(vertex2);
  if (den==0)
    return 0;
  var ans=floatobj.vectordot(vertex1,vertex2)/den;
  ans=Math.acos(ans);
  return ans;
  //return atan2(vectordot(vertex1,vertex2)/den,
  //sqrt(1.-(vectordot(vertex1,vertex2)/den)*(vectordot(vertex1,vertex2)/den)));
};

floatobj.angdeg=function(vertex1,vertex2) {
  if ( ! (vertex1 instanceof floatobj.Vertex) || 
       ! (vertex2 instanceof floatobj.Vertex))
    return 0;
  var den=floatobj.vectorlen(vertex1)*floatobj.vectorlen(vertex2);
  if (den==0)
    return 0;
  var ans=vectordot(vertex1,vertex2)/den;
  ans=Math.acos(ans)/floatobj.rad;
  return ans;
  //return atan2(sqrt(1.-ans*ans),ans)/floatobj.rad;
};

floatobj.lineRot=function(vertex11,vertex12,vertex21,vertex22,rotax) {
  if (rotax instanceof floatobj.Vertex)
    rotax.xyzw=[NaN,NaN,NaN,NaN];
  if ( ! (vertex11 instanceof floatobj.Vertex) || 
       ! (vertex12 instanceof floatobj.Vertex) ||
       ! (vertex21 instanceof floatobj.Vertex) || 
       ! (vertex22 instanceof floatobj.Vertex))
    return 0;
  var v1=floatobj.vectorsubs(vertex12,vertex11),v2=floatobj.vectorsubs(vertex22,vertex21);
  v1=floatobj.vectornorm(v1);
  v2=floatobj.vectornorm(v2);
  if (rotax instanceof floatobj.Vertex)
    rotax=floatobj.vectorcross(v1,v2);
  var ans=floatobj.angrad(v1,v2);
  return ans;
};

floatobj.mindistpointplane=function(vertex1,vertex2,vertex3,vertex4,vertexinter) {
  if (vertexinter instanceof floatobj.Vertex)
    vertexinter.xyzw=[NaN,NaN,NaN,NaN];
  if ( ! (vertex1 instanceof floatobj.Vertex) || 
       ! (vertex2 instanceof floatobj.Vertex) ||
       ! (vertex3 instanceof floatobj.Vertex) || 
       ! (vertex4 instanceof floatobj.Vertex))
    return NaN;
  var v21=floatobj.vectorsubs(vertex2,vertex1),v31=floatobj.vectorsubs(vertex3,vertex1);
  var v41=floatobj.vectorsubs(vertex4,vertex1),vn=floatobj.vectorcross(v21,v31);
  var ans=NaN,lvn=floatobj.vectorlen(vn);
  if (lvn!=0) {
    ans=floatobj.vectordot(vn,v41)/lvn;
    if (vertexinter instanceof floatobj.Vertex) {
      vertexinter.xyzw[0]=vn[0]*(dist/lvn);
      vertexinter.xyzw[1]=vn[1]*(dist/lvn);
      vertexinter.xyzw[2]=vn[2]*(dist/lvn);
      vertexinter=floatobj.vectorsubs(vertex4,vertexinter);
    }
  }
  return ans;
};

floatobj.mindistpointline=function(vertex1,vertex2,vertex3,vertexinter) {
  if (vertexinter instanceof floatobj.Vertex)
    vertexinter.xyzw=[NaN,NaN,NaN,NaN];
  if ( ! (vertex1 instanceof floatobj.Vertex) || 
       ! (vertex2 instanceof floatobj.Vertex) ||
       ! (vertex3 instanceof floatobj.Vertex))
    return NaN;
  var v21=floatobj.vectorsubs(vertex2,vertex1),v31=floatobj.vectorsubs(vertex3,vertex1);
  var v21xv31=floatobj.vectorcross(v21,v31),v1x=floatobj.vectorsubs(vertex1,v21xv31);
  var ans=floatobj.mindistpointplane(vertex1,vertex2,v1x,vertex3,vertexinter);
  return ans;
};


