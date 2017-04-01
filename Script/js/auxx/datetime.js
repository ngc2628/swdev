
var datetime={};

datetime.structtmtype=['millisecond','second','minute','hour','day','month','year','timezone'];
datetime.asecondmillis=1000;
datetime.aminutemillis=60000; 
datetime.anhourmillis=3600000; 
datetime.adaymillis=86400000;
datetime.year2038millis=1653247000; // 2038-01-19T03:14:07
datetime.decacc={
  '1980': 315532800000,'1990': 631152000000,'2000': 946684800000,  
  '2010':1262304000000,'2020':1577836800000,'2030':1893456000000   
};

datetime.hfields={
  'year':{'prefix':'','postfix':'/'},
  'month':{'prefix':'','postfix':'/'},
  'day':{'prefix':'','postfix':' '},
  'hour':{'prefix':'','postfix':'h'}
};

datetime.dfields={
  'year':{'prefix':'','postfix':'/'},
  'month':{'prefix':'','postfix':'/'},
  'day':{'prefix':'','postfix':''}
};

datetime.mfields={
  'year':{'prefix':'','postfix':'/'},
  'month':{'prefix':'','postfix':''},
};

datetime.utcmillicnt=function(date) {
  if (typeof date!='date')
    date=new Date();
  return date.valueOf();
};

datetime.isLeapYear=function(yy) {
  var ans=0;
  if (yy>0 && (yy%4)==0 && ((yy%100)!=0 || (yy%400)==0))
    ans=1;
  return ans;
};

datetime.yearcnt=function(yy) {
  if (yy==2038)
    return datetime.year2038millis;
  var ans=365*datetime.adaymillis;
  if (datetime.isLeapYear(yy)==1)
    ans+=datetime.adaymillis;
  return ans;
};

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

datetime.monthcnt=function(mm,yy) {
  var ans=datetime.adaymillis*datetime.monthdays(mm,yy);
  return ans;
};

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

datetime.chkStructTm=function(structtm) {
  if (intobj.chknumberrange(structtm.millisecond,0,999)==aux.ansko)
    structtm.millisecond=0;
  if (intobj.chknumberrange(structtm.second,0,59)==aux.ansko)
    structtm.second=0;
  if (intobj.chknumberrange(structtm.minute,0,59)==aux.ansko)
    structtm.minute=0;
  if (intobj.chknumberrange(structtm.hour,0,23)==aux.ansko)
    structtm.hour=0;
  if (intobj.chknumberrange(structtm.month,1,12)==aux.ansko)
    structtm.month=1;
  if (intobj.chknumberrange(structtm.year,1970,2038)==aux.ansko) {
    var dt=new Date();
    structtm.year=dt.getFullYear();
  }
  if (intobj.chknumberrange(structtm.day,1,datetime.monthdays(structtm.month,structtm.year))==aux.ansko)
    structtm.day=1;
  return structtm;
};

datetime.add2StructTm=function(structtm,type,val) {
  structtm=datetime.chkStructTm(structtm);
  type=datetime.chkstructtmtype(type);
  val=intobj.cutfloat(intobj.chknumber(val,0));
  if (type.length==0 || val==0)
    return structtm; 
  var mdays=datetime.monthdays(structtm.month,structtm.year);
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
    structtm.day-=mdays;
    structtm.month+=1;
    if (structtm.month>12) {
      structtm.month-=12;
      structtm.year+=1;
    }
    mdays=datetime.monthdays(structtm.month,structtm.year);
  }
  while (structtm.month>12) {
    structtm.month-=12;
    structtm.year+=1;
  }
  return structtm;
};

datetime.subs2StructTm=function(structtm,type,val) {
  structtm=datetime.chkStructTm(structtm);
  type=datetime.chkstructtmtype(type);
  val=intobj.cutfloat(intobj.chknumber(val,0));
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

datetime.Datetime=function(utccntoriso8601) {
  this.structtm={
    'year':0,'month':0,'day':0,'hour':0,'minute':0,'second':0,'millisecond':0,'timezone':0
  };
  var ans=aux.ansko;
  if (typeof utccntoriso8601=='string')
    ans=this.fromIso8601(utccntoriso8601);
  if (ans==aux.ansko)
    this.fromUtcmillicnt(utccntoriso8601);
};

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

datetime.Datetime.prototype.fromUtcmillicnt=function(utccnt) {
  this.structtm={
    'year':0,'month':0,'day':0,'hour':0,'minute':0,'second':0,'millisecond':0,'timezone':0
  };
  var nn=datetime.utcmillicnt();
  if (typeof utccnt=='number') {
    utccnt=intobj.cutfloat(intobj.chknumber(utccnt,nn)); 
    if (utccnt<=0)
      utccnt=nn;
  }   
  else if (typeof utccnt=='string')
    utccnt=parseFloat(utccnt);
  else
    utccnt=nn;
  var preyy=0,yy=0,mm=0,dd=0,hh=0,mmm=0,ss=0;
  if (utccnt<datetime.decacc['1980'])
    preyy=1970;
  if (preyy==0 && utccnt<datetime.decacc['1990'])
    preyy=1980;
  if (preyy==0 && utccnt<datetime.decacc['2000'])
    preyy=1990;
  if (preyy==0 && utccnt<datetime.decacc['2010'])
    preyy=2000;
  if (preyy==0 && utccnt<datetime.decacc['2020'])
    preyy=2010;
  if (preyy==0 && utccnt<datetime.decacc['2030'])
    preyy=2020;
  if (preyy==0)
    preyy=2030;
  utccnt-=datetime.decacc[preyy.toString()];
  for (yy=preyy;yy<2039;yy++) {
    if (utccnt<datetime.yearcnt(yy))
      break;
    utccnt-=datetime.yearcnt(yy);
  }
  this.structtm.year=yy;
  for (mm=1;mm<13;mm++) {
    if (utccnt<datetime.monthcnt(mm,yy))
      break;
    utccnt-=datetime.monthcnt(mm,yy);
  }
  this.structtm.month=mm;
  var mmdcnt=datetime.monthdays(mm,yy);
  for (dd=1;dd<=mmdcnt;dd++) {
    if (utccnt<datetime.adaymillis)
      break;
    utccnt-=datetime.adaymillis;
  }
  this.structtm.day=dd;
  for (hh=0;hh<24;hh++) {
    if (utccnt<datetime.anhourmillis)
      break;
    utccnt-=datetime.anhourmillis;
  }
  this.structtm.hour=hh;
  for (mmm=0;mmm<60;mmm++) {
    if (utccnt<datetime.aminutemillis)
      break;
    utccnt-=datetime.aminutemillis;
  }
  this.structtm.minute=mmm;
  for (ss=0;ss<60;ss++) {
    if (utccnt<datetime.asecondmillis)
      break;
    utccnt-=datetime.asecondmillis;
  }
  this.structtm.second=ss;
  this.structtm.millisecond=utccnt;
  this.structtm.timezone=0;
};

datetime.Datetime.prototype.fromStructTmValues=function(yy,mm,dd,hh,mmm,ss,msec,tz) {
  this.fromUtcmillicnt();
  if (intobj.chknumberrange(yy,1970,2038)==aux.ansok)
    this.structtm.year=yy;
  if (intobj.chknumberrange(mm,1,12)==aux.ansok && 
      (this.structtm.year!=2038 || (this.structtm.year==2038 && mm==1)))
    this.structtm.month=mm;  
  var mdays=datetime.monthdays(this.structtm.month,this.structtm.year);
  if (intobj.chknumberrange(dd,1,mdays)==aux.ansok &&
      (this.structtm.year!=2038 || (this.structtm.year==2038 && dd<=19)))
    this.structtm.day=dd;
  if (intobj.chknumberrange(hh,0,23)==aux.ansok &&
      (this.structtm.year!=2038 || (this.structtm.year==2038 && hh<=3)))
    this.structtm.hour=hh;
  if (intobj.chknumberrange(mmm,0,59)==aux.ansok &&
      (this.structtm.year!=2038 || (this.structtm.year==2038 && mmm<=14)))
    this.structtm.minute=mmm;
  if (intobj.chknumberrange(ss,0,59)==aux.ansok &&
      (this.structtm.year!=2038 || (this.structtm.year==2038 && ss<=8)))
    this.structtm.second=ss;
  if (intobj.chknumberrange(msec,0,999)==aux.ansok &&
      (this.structtm.year!=2038 || (this.structtm.year==2038 && msec<=499)))
    this.structtm.millisecond=msec;
  if (intobj.chknumberrange(tz,-720,720)==aux.ansok)
    this.structtm.timezone=tz;
};

datetime.Datetime.prototype.fromStructTm=function(structtm) {
  structtm=datetime.chkStructTm(structtm);
  this.fromStructTmValues(
    structtm.year,structtm.month,structtm.day,structtm.hour,structtm.minute,
    structtm.second,structtm.millisecond,structtm.timezone
  );
};

datetime.Datetime.prototype.fromIso8601=function(striso8601) {
  if (typeof striso8601!='string')
    return aux.ansko;
  var strregexp='[1-2][0-9][0-9][0-9]-[0-1][0-9]-[0-3][0-9]'; // yy-mm-dd
  strregexp+='T[0-2][0-9]:[0-5][0-9]:[0-5][0-9]'; // hh:mm:ss
  strregexp+='(\.[0-9][0-9][0-9])?'; // msec
  strregexp+='[+-][0-1][0-9]:[03]0'; // tz
  var regexp=new RegExp(strregexp);
  var ans=striso8601.search(regexp);
  if (ans<0)
    return aux.ansko;
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
  return aux.ansok;
};

datetime.Datetime.prototype.tm2string=function(key) {
  var ans='';
  var tmval=this.structtm[key];
  if (key=='timezone') {
    ans+=(tmval<0 ? '-' : '+');
    var tzoff=intobj.cutfloat(tmval/60);
    if (tzoff<10)
      ans+='0';
    ans+=tzoff.toString();
    ans+=':';
    tzoff=intobj.cutfloat(tmval-tzoff*60);
    if (tzoff<10)
      ans+='0';
    ans+=tzoff.toString();
  }
  else {
    if (tmval<10)
      ans+='0';
    if (key=='millisecond' && tmval<100)
      ans+='0';
    ans+=tmval.toString();
  }
  return ans;
};

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
  showmillis=intobj.chknumber(showmillis,0);
  if (showmillis!=0) {
    ans+='.';
    ans+=this.tm2string('millisecond');
  }
  ans+=this.tm2string('timezone');  
  return ans;
};

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

datetime.Datetime.prototype.toUtcmillicnt=function() {
  var cnt=0,ans=0;
  for (cnt=1970;cnt<this.structtm.year;cnt++)
    ans+=datetime.yearcnt(cnt);
  for (cnt=1;cnt<this.structtm.month;cnt++)
    ans+=datetime.monthcnt(cnt,this.structtm.year);
  for (cnt=1;cnt<this.structtm.day;cnt++)
    ans+=datetime.adaymillis;
  for (cnt=0;cnt<this.structtm.hour;cnt++)
    ans+=datetime.anhourmillis;
  for (cnt=0;cnt<this.structtm.minute;cnt++)
    ans+=datetime.aminutemillis;
  for (cnt=0;cnt<this.structtm.second;cnt++)
    ans+=datetime.asecondmillis;
  ans+=this.structtm.millisecond;
  ans-=this.structtm.timezone;
  return ans;
};

datetime.Datetime.prototype.toString=function(showmillis) {
  var ans=this.toIso8601(showmillis);
  return ans;
};

datetime.Datetime.prototype.toJson=function(indent) {
  var ans=aux.qdecl('iso8601');
  ans+=aux.qword(this.toIso8601(1));
  if (typeof indent=='string')
    ans=(indent+ans);
  return ans;
};

datetime.chkpt=0;

datetime.chkrun=function(start) {
  start=intobj.chknumber(start,0);
  var ans=0;
  if (start==0)
    datetime.chkpt=datetime.utcmillicnt();
  else
    ans=datetime.utcmillicnt()-datetime.chkpt;
  return ans/1000;
};

