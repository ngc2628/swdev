var mkuni={};

// any answer
mkuni.good=1; mkuni.bad=0;

// coor index
mkuni.typeX=0; mkuni.typeY=1; mkuni.typeZ=2; mkuni.typeW=3;

// nan
mkuni.strten='10';
mkuni.strnan='NAN';
// alert(new Error().lineNumber+' ');

// ascii char code names
mkuni.asciibackspace=8; mkuni.asciilf=10; mkuni.asciicr=13; mkuni.asciispace=32; 
mkuni.asciihash=35; mkuni.asciileft=37; mkuni.asciiup=38; mkuni.asciiright=39; 
mkuni.asciidown=40; mkuni.asciimult=42; mkuni.asciiplus=43; mkuni.asciiminus=45; 
mkuni.asciidot=46; mkuni.asciidiv=47; mkuni.asciizero=48; mkuni.asciione=49; 
mkuni.asciitwo=50; mkuni.asciithree=51; mkuni.asciifour=52; mkuni.asciifive=53; 
mkuni.asciisix=54; mkuni.asciiseven=55; mkuni.asciieight=56; mkuni.asciinine=57; 
mkuni.asciicolon=58; mkuni.asciiequals=61; mkuni.asciiA=65; mkuni.asciiB=66; 
mkuni.asciiC=67; mkuni.asciiD=68; mkuni.asciiE=69; mkuni.asciiF=70; mkuni.asciiG=71; 
mkuni.asciiH=72; mkuni.asciiI=73; mkuni.asciiJ=74; mkuni.asciiK=75; mkuni.asciiL=76; 
mkuni.asciiM=77; mkuni.asciiN=78; mkuni.asciiO=79; mkuni.asciiP=80; mkuni.asciiQ=81; 
mkuni.asciiR=82; mkuni.asciiS=83; mkuni.asciiT=84; mkuni.asciiU=85; mkuni.asciiV=86; 
mkuni.asciiW=87; mkuni.asciiX=88; mkuni.asciiY=89; mkuni.asciiZ=90; mkuni.ascii_=95; 
mkuni.asciia=97; mkuni.asciib=98; mkuni.asciic=99; mkuni.asciid=100; mkuni.asciie=101; 
mkuni.asciif=102; mkuni.asciig=103; mkuni.asciii=105; mkuni.asciix=120; mkuni.asciidel=127;

/* ********** */
mkuni.chkstr=function(obj) {
  var ans='';
  if (typeof obj!='undefined')
    ans=((typeof obj=='string') ? obj : obj.toString()).trim();
  return ans;
}

/* ********** */
mkuni.chknumber=function(num,defnum) {
  if (typeof defnum!='number')
    defnum=NaN;
  if (typeof num!='number')
    return defnum;
  if (!isNaN(defnum) && (isNaN(num) || !isFinite(num)))
    return defnum;
  return num;
};

/* ********** */
mkuni.linsearch=function(itm,arr,fcmp) {
  if ((typeof itm=='undefined') || (typeof fcmp!='function') ||
      (typeof arr=='undefined') || !(arr instanceof Array) || arr.length==0)
    return -1;
  var ii=0;
  for (ii=0;ii<arr.length;ii++) {
    if (fcmp(arr[ii],itm)==0)
      return ii;
  }
  return -1;
};

/* ********** */
mkuni.bsearch=function(itm,arr,fcmp) {
  if ((typeof itm=='undefined') || (typeof fcmp!='function') ||
      (typeof arr=='undefined') || !(arr instanceof Array) || arr.length==0)
    return -1;
  var lb=-1,mb=0,ub=arr.length,cmp=0;
  while ((ub-lb)>1) {
    mb=parseInt((ub+lb)/2,10);
    cmp=fcmp(arr[mb],itm);
    if (cmp==0)
      return mb;
    if (cmp<0)
      lb=mb;
    else
      ub=mb;
  }
  return -1;
};

/* ********** */
mkuni.str4code=function(charcode,charcodeL) {
  var num=mkuni.chknumber(charcode,NaN);
  var ans='';
  if (isNaN(num) || num<=0)
    return ans;
  if ((typeof charcodeL!='undefined') && (charcodeL instanceof Array) && charcodeL.length>0) {
    var idx=mkuni.bsearch(num,charcodeL,
      function(cmp1,cmp2) { return (cmp1<cmp2 ? -1 : (cmp2<cmp1 ? 1 : 0)); }
    );
    if (idx<0)
      return ans;
    num=charcodeL[idx];
  }
  ans=String.fromCharCode(num);
  return ans;
};


