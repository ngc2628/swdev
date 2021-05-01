
var datetime={};

datetime.structtmtype=['millisecond','second','minute','hour','day','month','year','timezone'];
datetime.asecondmillis='1000';
datetime.aminutemillis='60000';
datetime.anhourmillis='3600000';
datetime.adaymillis='86400000';
datetime.year2038millis='1653247000'; // 2038-01-19T03:14:07
datetime.bstrdecacc={
  '1980' : '00100100101110111001110000111000000000000' ,
  '1990' : '01001001011110011100101110011110000000000' ,
  '2000' : '01101110001101010110011111010110000000000' ,
  '2010' : '10010010111100111001011100111100000000000' ,
  '2020' : '10110111101011110011001101110100000000000' ,
  '2030' : '11011100011011010110001011011010000000000'
};

/* ********** */
datetime.utcmillicnt=function(date) {
  if (typeof date!='date')
    date=new Date();
  return date.valueOf();
};

/* ********** */
datetime.isLeapYear=function(yy) {
  var ans=0;
  if (yy>0 && (yy%4)==0 && ((yy%100)!=0 || (yy%400)==0))
    ans=1;
  return ans;
};

/* ********** */
datetime.yearcnt=function(yy) {
  if (yy==2038)
    return mkint.str2bits(datetime.year2038millis,10);
  var aday=mkint.str2bits(datetime.adaymillis,10);
  if (datetime.isLeapYear(yy)==1)
    return mkint.multbits(aday,mkint.str2bits('366'),10);
  else
    return mkint.multbits(aday,mkint.str2bits('365'),10);
};

/* ********** */
datetime.monthdays=function(mm,yy) {
  var ans=0;
  var dcnt=[28,29,30,31];
  if (mm==2)
    ans=dcnt[datetime.isLeapYear(yy)];
  else if (mm==1 || mm==3 || mm==5 || mm==7 || mm==8 || mm==10 || mm==12)
    ans=dcnt[3];
  else if (mm==4 || mm==6 || mm==9 || mm==11)
    ans=dcnt[2];
  return ans;
};

/* ********** */
datetime.monthcnt=function(mm,yy) {
  var aday=mkint.str2bits(datetime.adaymillis,10);
  var ans=mkint.multbits(aday,mkint.str2bits(datetime.monthdays(mm,yy).toString(),10));
  return ans;
};

/* ********** */
datetime.chkstructtmtype=function(type) {
  var ans='';
  if (typeof type=='string') {
    type=type.trim().toLowerCase();
    if (type=='year' || type=='month' || type=='day' || type=='hour' || type=='minute' ||
        type=='second' || type=='millisecond' || type=='timezone')
      ans=type;
  }
  return ans;
};

/* ********** */
datetime.chkStructTm=function(structtm) {
  if (mkint.chknumberrange(structtm.millisecond,0,999)==mkuni.bad)
    structtm.millisecond=0;
  if (mkint.chknumberrange(structtm.second,0,59)==mkuni.bad)
    structtm.second=0;
  if (mkint.chknumberrange(structtm.minute,0,59)==mkuni.bad)
    structtm.minute=0;
  if (mkint.chknumberrange(structtm.hour,0,23)==mkuni.bad)
    structtm.hour=0;
  if (mkint.chknumberrange(structtm.month,1,12)==mkuni.bad)
    structtm.month=1;
  if (mkint.chknumberrange(structtm.year,1970,2038)==mkuni.bad) {
    var dt=new Date();
    structtm.year=dt.getFullYear();
  }
  if (mkint.chknumberrange(structtm.day,1,datetime.monthdays(structtm.month,structtm.year))==mkuni.bad)
    structtm.day=1;
  return structtm;
};

/* ********** */
datetime.add2StructTm=function(structtm,type,val) {
  structtm=datetime.chkStructTm(structtm);
  type=datetime.chkstructtmtype(type);
  val=mkfloat.cut2int(mkuni.chknumber(val,0));
  if (type.length==0 || val==0)
    return structtm;
  structtm[type]+=val;
  while (structtm.millisecond>999) {
    structtm.millisecond-=1000;
    structtm.second+=1;
  }
  while (structtm.second>59) {
    structtm.second-=60;
    structtm.minute+=1;
  }
  while (structtm.minute>59) {
    structtm.minute-=60;
    structtm.hour+=1;
  }
  while (structtm.hour>23) {
    structtm.hour-=24;
    structtm.day+=1;
  }
  while (structtm.day>mdays) {
    var mdays=datetime.monthdays(structtm.month,structtm.year);
    structtm.day-=mdays;
    structtm.month+=1;
  }
  while (structtm.month>12) {
    structtm.month-=12;
    structtm.year+=1;
  }
  return structtm;
};

/* ********** */
datetime.subs2StructTm=function(structtm,type,val) {
  structtm=datetime.chkStructTm(structtm);
  type=datetime.chkstructtmtype(type);
  val=mkfloat.cut2int(mkuni.chknumber(val,0));
  if (type.length==0 || val==0)
    return structtm;
  structtm[type]-=val;
  while (structtm.millisecond<0) {
    structtm.millisecond+=1000;
    structtm.second-=1;
  }
  while (structtm.second<0) {
    structtm.second+=60;
    structtm.minute-=1;
  }
  while (structtm.minute<0) {
    structtm.minute+=60;
    structtm.hour-=1;
  }
  while (structtm.hour<0) {
    structtm.hour+=24;
    structtm.day-=1;
  }
  while (structtm.day<1) {
    var mdays=(structtm.month==1 ? 31 : datetime.monthdays(structtm.month,structtm.year));
    structtm.day+=mdays;
    structtm.month-=1;
  }
  while (structtm.month<1) {
    structtm.month+=12;
    structtm.year-=1;
  }
  return structtm;
};

/* ########## */
datetime.Datetime=function(utccntoriso8601) {
  this.structtm={
    'year':0,'month':0,'day':0,'hour':0,'minute':0,'second':0,'millisecond':0,'timezone':0
  };
  var ans=mkuni.bad;
  if (typeof utccntoriso8601=='string')
    ans=this.fromIso8601(utccntoriso8601);
  if (ans==mkuni.bad)
    this.fromUtcmillicnt(utccntoriso8601);
};

/* ********** */
datetime.Datetime.prototype.toStructTm=function() {
  var ans={
    'year':this.structtm.year,
    'month':this.structtm.month,
    'day':this.structtm.day,
    'hour':this.structtm.hour,
    'minute':this.structtm.minute,
    'second':this.structtm.second,
    'millisecond':this.structtm.millisecond,
    'timezone':this.structtm.timezone
  };
  return ans;
};

/* ********** */
datetime.Datetime.prototype.fromUtcmillicnt=function(utccnt) {
  this.structtm={
    'year':0,'month':0,'day':0,'hour':0,'minute':0,'second':0,'millisecond':0,'timezone':0
  };
  var nn=datetime.utcmillicnt();
  var strcnt=nn.toString();
  if (typeof utccnt=='number') {
    utccnt=mkfloat.cut2int(mkuni.chknumber(utccnt,nn));
    if (utccnt<=0)
      utccnt=nn;
    strcnt=utccnt.toString();
  }
  else if (typeof utccnt=='string')
    strcnt=utccnt;
  strcnt=mkint.str2bits(strcnt,10);
  var preyy=0,yy=0,mm=0,dd=0,hh=0,mmm=0,ss=0;
  if (mkint.cmpbits(strcnt,datetime.bstrdecacc['1980'])<0)
    preyy=1970;
  if (preyy==0 && mkint.cmpbits(strcnt,datetime.bstrdecacc['1990'])<0)
    preyy=1980;
  if (preyy==0 && mkint.cmpbits(strcnt,datetime.bstrdecacc['2000'])<0)
    preyy=1990;
  if (preyy==0 && mkint.cmpbits(strcnt,datetime.bstrdecacc['2010'])<0)
    preyy=2000;
  if (preyy==0 && mkint.cmpbits(strcnt,datetime.bstrdecacc['2020'])<0)
    preyy=2010;
  if (preyy==0 && mkint.cmpbits(strcnt,datetime.bstrdecacc['2030'])<0)
    preyy=2020;
  if (preyy==0)
    preyy=2030;
  strcnt=mkint.subsbits(strcnt,datetime.bstrdecacc[preyy.toString()]);
  for (yy=preyy;yy<2039;yy++) {
    if (mkint.cmpbits(strcnt,datetime.yearcnt(yy))<0)
      break;
    strcnt=mkint.subsbits(strcnt,datetime.yearcnt(yy));
  }
  /*for (yy=1970;yy<2038;yy++) {
    if (mkint.cmpbits(strcnt,datetime.yearcnt(yy))<0)
      break;
    strcnt=mkint.subsbits(strcnt,datetime.yearcnt(yy));
  }*/
  this.structtm.year=yy;
  for (mm=1;mm<13;mm++) {
    if (mkint.cmpbits(strcnt,datetime.monthcnt(mm,yy))<0)
      break;
    strcnt=mkint.subsbits(strcnt,datetime.monthcnt(mm,yy));
  }
  this.structtm.month=mm;
  var mmdcnt=datetime.monthdays(mm,yy);
  var cmpcnt=mkint.str2bits(datetime.adaymillis,10);
  for (dd=1;dd<=mmdcnt;dd++) {
    if (mkint.cmpbits(strcnt,cmpcnt)<0)
      break;
    strcnt=mkint.subsbits(strcnt,cmpcnt);
  }
  this.structtm.day=dd;
  cmpcnt=mkint.str2bits(datetime.anhourmillis,10);
  for (hh=0;hh<24;hh++) {
    if (mkint.cmpbits(strcnt,cmpcnt)<0)
      break;
    strcnt=mkint.subsbits(strcnt,cmpcnt);
  }
  this.structtm.hour=hh;
  cmpcnt=mkint.str2bits(datetime.aminutemillis,10);
  for (mmm=0;mmm<60;mmm++) {
    if (mkint.cmpbits(strcnt,cmpcnt)<0)
      break;
    strcnt=mkint.subsbits(strcnt,cmpcnt);
  }
  this.structtm.minute=mmm;
  cmpcnt=mkint.str2bits(datetime.asecondmillis,10);
  for (ss=0;ss<60;ss++) {
    if (mkint.cmpbits(strcnt,cmpcnt)<0)
      break;
    strcnt=mkint.subsbits(strcnt,cmpcnt);
  }
  this.structtm.second=ss;
  strcnt=mkint.bits2str(strcnt);
  this.structtm.millisecond=parseInt(strcnt,10);
  this.structtm.timezone=0;
};

/* ********** */
datetime.Datetime.prototype.fromStructTmValues=function(yy,mm,dd,hh,mmm,ss,msec,tz) {
  this.fromUtcmillicnt();
  if (mkint.chknumberrange(yy,1970,2038)==mkuni.good)
    this.structtm.year=yy;
  if (mkint.chknumberrange(mm,1,12)==mkuni.good &&
      (this.structtm.year!=2038 || (this.structtm.year==2038 && mm==1)))
    this.structtm.month=mm;
  var mdays=datetime.monthdays(this.structtm.month,this.structtm.year);
  if (mkint.chknumberrange(dd,1,mdays)==mkuni.good &&
      (this.structtm.year!=2038 || (this.structtm.year==2038 && dd<=19)))
    this.structtm.day=dd;
  if (mkint.chknumberrange(hh,0,23)==mkuni.good &&
      (this.structtm.year!=2038 || (this.structtm.year==2038 && hh<=3)))
    this.structtm.hour=hh;
  if (mkint.chknumberrange(mmm,0,59)==mkuni.good &&
      (this.structtm.year!=2038 || (this.structtm.year==2038 && mmm<=14)))
    this.structtm.minute=mmm;
  if (mkint.chknumberrange(ss,0,59)==mkuni.good &&
      (this.structtm.year!=2038 || (this.structtm.year==2038 && ss<=8)))
    this.structtm.second=ss;
  if (mkint.chknumberrange(msec,0,999)==mkuni.good &&
      (this.structtm.year!=2038 || (this.structtm.year==2038 && msec<=499)))
    this.structtm.millisecond=msec;
  if (mkint.chknumberrange(tz,-720,720)==mkuni.good)
    this.structtm.timezone=tz;
};

/* ********** */
datetime.Datetime.prototype.fromStructTm=function(structtm) {
  structtm=datetime.chkStructTm(structtm);
  this.fromStructTmValues(
    structtm.year,structtm.month,structtm.day,structtm.hour,structtm.minute,
    structtm.second,structtm.millisecond,structtm.timezone
  );
};

/* ********** */
datetime.Datetime.prototype.fromIso8601=function(striso8601) {
  if (typeof striso8601!='string')
    return mkuni.bad;
  var strregexp='[1-2][0-9][0-9][0-9]-[0-1][0-9]-[0-3][0-9]'; // yy-mm-dd
  strregexp+='T[0-2][0-9]:[0-5][0-9]:[0-5][0-9]'; // hh:mm:ss
  strregexp+='(\.[0-9][0-9][0-9])?'; // msec
  strregexp+='[+-][0-1][0-9]:[03]0'; // tz
  var regexp=new RegExp(strregexp);
  var ans=striso8601.search(regexp);
  if (ans<0)
    return mkuni.bad;
  var idxs=0,idxe=4;
  var yy=parseInt(striso8601.substring(idxs,idxe));
  idxs+=5;
  idxe+=3;
  var mm=parseInt(striso8601.substring(idxs,idxe));
  idxs+=3;
  idxe+=3;
  var dd=parseInt(striso8601.substring(idxs,idxe));
  idxs+=3;
  idxe+=3;
  var hh=parseInt(striso8601.substring(idxs,idxe));
  idxs+=3;
  idxe+=3;
  var mmm=parseInt(striso8601.substring(idxs,idxe));
  idxs+=3;
  idxe+=3;
  var ss=parseInt(striso8601.substring(idxs,idxe));
  idxs+=3;
  idxe+=4;
  var msec=0;
  regexp=new RegExp('[0-9][0-9][0-9]');
  ans=striso8601.substring(idxs,idxe).search(regexp);
  if (ans>=0) {
    msec=parseInt(striso8601.substring(idxs,idxe));
    idxs+=4;
    idxe+=3;
  }
  else {
    idxs-=3;
    idxe-=4;
    idxs+=2;
    idxe+=3;
  }
  var tzhoff=parseInt(striso8601.substring(idxs,idxe));
  idxs+=3;
  var tzoff=parseInt(striso8601.substring(idxs));
  tzoff=((60*tzhoff)+(tzhoff<0 ? -tzoff : tzoff));
  this.fromStructTmValues(yy,mm,dd,hh,mmm,ss,msec,tzoff);
  return mkuni.good;
};

/* ********** */
datetime.Datetime.prototype.tm2string=function(key) {
  var ans='';
  var tmval=this.structtm[key];
  if (key=='timezone') {
    ans+=(tmval<0 ? mkuni.str4code(mkuni.asciiminus) : mkuni.str4code(mkuni.asciiplus));
    var tzoff=mkfloat.cut2int(tmval/60);
    if (tzoff<10)
      ans+=mkuni.str4code(mkuni.asciizero);
    ans+=tzoff.toString();
    ans+=mkuni.str4code(mkuni.asciicolon);
    tzoff=mkfloat.cut2int(tmval-tzoff*60);
    if (tzoff<10)
      ans+=mkuni.str4code(mkuni.asciizero);
    ans+=tzoff.toString();
  }
  else {
    if (tmval<10)
      ans+=mkuni.str4code(mkuni.asciizero);
    if (key=='millisecond' && tmval<100)
      ans+=mkuni.str4code(mkuni.asciizero);
    ans+=tmval.toString();
  }
  return ans;
};

/* ********** */
datetime.Datetime.prototype.toIso8601=function(showmillis) {
  var ans=this.tm2string('year');
  ans+='-';
  ans+=this.tm2string('month');
  ans+='-';
  ans+=this.tm2string('day');
  ans+='T';
  ans+=this.tm2string('hour');
  ans+=':';
  ans+=this.tm2string('minute');
  ans+=':';
  ans+=this.tm2string('second');
  showmillis=mkuni.chknumber(showmillis,0);
  if (showmillis!=0) {
    ans+='.';
    ans+=this.tm2string('millisecond');
  }
  ans+=this.tm2string('timezone');
  return ans;
};

/* ********** */
datetime.Datetime.prototype.toStructTmString=function(fields) {
  var fieldL=['year','month','day','hour','minute','second','millisecond','timezone'];
  var ans='';
  var ii=0;
  for (ii=0;ii<fieldL.length;ii++) {
    if (fields.hasOwnProperty(fieldL[ii])) {
      if (fields[fieldL[ii]].hasOwnProperty('prefix'))
        ans+=fields[fieldL[ii]].prefix;
      ans+=this.tm2string(fieldL[ii]);
      if (fields[fieldL[ii]].hasOwnProperty('postfix'))
        ans+=fields[fieldL[ii]].postfix;
    }
  }
  return ans;
};

/* ********** */
datetime.Datetime.prototype.toUtcmillicnt=function() {
  var cnt=0,strcnt='0';
  for (cnt=1970;cnt<this.structtm.year;cnt++)
    strcnt=mkint.addbits(strcnt,datetime.yearcnt(cnt));
  for (cnt=1;cnt<this.structtm.month;cnt++)
    strcnt=mkint.addbits(strcnt,datetime.monthcnt(cnt,this.structtm.year));
  var cmpcnt=mkint.str2bits(datetime.adaymillis,10);
  for (cnt=1;cnt<this.structtm.day;cnt++)
    strcnt=mkint.addbits(strcnt,cmpcnt);
  cmpcnt=mkint.str2bits(datetime.anhourmillis,10);
  for (cnt=0;cnt<this.structtm.hour;cnt++)
    strcnt=mkint.addbits(strcnt,cmpcnt);
  cmpcnt=mkint.str2bits(datetime.aminutemillis,10);
  for (cnt=0;cnt<this.structtm.minute;cnt++)
    strcnt=mkint.addbits(strcnt,cmpcnt);
  cmpcnt=mkint.str2bits(datetime.asecondmillis,10);
  for (cnt=0;cnt<this.structtm.second;cnt++)
    strcnt=mkint.addbits(strcnt,cmpcnt);
  strcnt=mkint.addbits(strcnt,mkint.str2bits(this.structtm.millisecond.toString(),10));
  var tzoff=this.structtm.timezone,tzsgn=1;
  if (tzoff<0) {
    tzoff=-tzoff;
    tzsgn=-1;
  }
  cmpcnt=mkint.str2bits(datetime.aminutemillis,10);
  cmpcnt=mkint.multbits(mkint.str2bits(tzoff.toString(),10),cmpcnt);
  if (tzsgn<0)
    strcnt=mkint.addbits(strcnt,cmpcnt);
  else
    strcnt=mkint.subsbits(strcnt,cmpcnt);
  var ans=mkint.bits2str(strcnt,10);
  return ans;
};

/* ********** */
datetime.Datetime.prototype.toString=function(showmillis) {
  var ans=this.toIso8601(showmillis);
  return ans;
};

/* ********** */
datetime.Datetime.prototype.toJson=function(indent) {
  var ans=aux.qdecl('iso8601');
  ans+=aux.qword(this.toIso8601(1));
  if (typeof indent=='string')
    ans=(indent+ans);
  return ans;
};
/* ########## */

