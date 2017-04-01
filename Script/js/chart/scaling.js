
chart.scale={};

chart.scale.datetimeintv=[
  1,datetime.asecondmillis,5*datetime.asecondmillis,10*datetime.asecondmillis,
  15*datetime.asecondmillis,30*datetime.asecondmillis,datetime.aminutemillis,
  2*datetime.aminutemillis,3*datetime.aminutemillis,5*datetime.aminutemillis,
  10*datetime.aminutemillis,15*datetime.aminutemillis,30*datetime.aminutemillis,
  datetime.anhourmillis,2*datetime.anhourmillis,6*datetime.anhourmillis,
  8*datetime.anhourmillis,12*datetime.anhourmillis,datetime.adaymillis,
  2*datetime.adaymillis,7*datetime.adaymillis,14*datetime.adaymillis,
  30*datetime.adaymillis,60*datetime.adaymillis,90*datetime.adaymillis,
  180*datetime.adaymillis,365*datetime.adaymillis,730*datetime.adaymillis
];

chart.scale.datetimeintvidx=function(intv) {
  intv=intobj.chknumber(intv,0);
  var ii=0;
  for (ii=0;ii<chart.scale.datetimeintv.length;ii++) {
    if (chart.scale.datetimeintv[ii]>intv)
      return (ii==0 ? 0 : ii-1);
  }
  return chart.scale.datetimeintv.length-1;
};

chart.scale.datetimefmtfromintv=function(intv) {
  intv=intobj.chknumber(intv,0);
  var ans={
    'year':{'prefix':'','postfix':'-'},
    'month':{'prefix':'','postfix':''}
  };
  var ii=0;
  for (ii=0;ii<chart.scale.datetimeintv.length;ii++) {
    if (chart.scale.datetimeintv[ii]>intv) {
      if (ii<=22) {
        ans.month.postfix='-';
        ans.day={'prefix':'','postfix':''};
      }
      if (ii<=18) {
        ans.day.postfix='T';
        ans.hour={'prefix':'','postfix':'h'};
      }
      if (ii<=13) {
        ans.hour.postfix=':';
        ans.minute={'prefix':'','postfix':''};
      }
      if (ii<=6) {
        ans.minute.postfix=':';
        ans.second={'prefix':'','postfix':''};
      }
      if (ii<=2) {
        ans.second.postfix='.';
        ans.millisecond={'prefix':'','postfix':''};
      }
      break;
    }
  }
  return ans;
};

chart.scale.Tic=function(dval,sstr,isz,idr) {
  this.val=intobj.chknumber(dval,NaN);
  this.str=(typeof sstr=='string' ? sstr : '');
  this.sz=parseInt(intobj.chknumber(isz,0),10);
  this.drawable=parseInt(intobj.chknumber(idr,1),10);
};

chart.scale.Tic.prototype.copy=function() {
  var ans=new this.constructor(this.val,this.str,this.sz,this.drawable);
  return ans;
};

chart.scale.Tic.prototype.toJson=function() {
  var ans=aux.qdecl('val');
  ans+=aux.qword(this.val.toString());
  ans+=aux.qnextprop();
  ans+=aux.qdecl('str');
  ans+=aux.qword(this.str);
  ans+=aux.qnextprop();
  ans+=aux.qdecl('sz');
  ans+=aux.qword(this.sz>0 ? 'y' : 'n');
  return ans;
};

chart.scale.chkrangetype=function(rangetype) {
  var ans='auto';
  if (typeof rangetype=='string' && rangetype.trim().toLowerCase()=='static')
    ans='static';
  return ans;
};

chart.scale.Range=function(typename,typenum) {
  if (typeof typename!='string')
    typename='range';
  aux.TypeId.call(this,typename,typenum);
  this.bounds={'min':{'val':0,'type':'auto'},'max':{'val':0,'type':'auto'}};
};

chart.scale.Range.prototype=Object.create(aux.TypeId.prototype);

chart.scale.Range.prototype.constructor=chart.scale.Range;

chart.scale.Range.prototype.copy=function() {
  var ans=new this.constructor(this.typename());
  ans.bounds.min.val=this.bounds.min.val;
  ans.bounds.min.type=this.bounds.min.type;
  ans.bounds.max.val=this.bounds.max.val;
  ans.bounds.max.type=this.bounds.max.type;
  return ans;
};

chart.scale.Range.prototype.adjust=function() {
  var ans={'min':this.bounds.min.val,'max':this.bounds.max.val};
  return ans;
};

chart.scale.Range.prototype.toJson=function(indent) {
  var nextindent='  ',ans=aux.TypeId.prototype.toJson.call(this,indent);
  if (typeof indent=='string')
    nextindent+=indent;
  ans+=aux.qnextprop(1);
  ans+=aux.qdecl('bounds',indent);
  var tmp1=aux.qdecl('min',nextindent);
  var tmp2=aux.qdecl('val');
  tmp2+=aux.qword(this.bounds.min.val);
  tmp2+=aux.qnextprop();
  tmp2+=aux.qdecl('type');
  tmp2+=aux.qword(this.bounds.min.type);
  tmp1+=aux.qattr(tmp2);
  tmp1+=aux.qnextprop(1);
  tmp1+=aux.qdecl('max',nextindent);
  tmp2=aux.qdecl('val');
  tmp2+=aux.qword(this.bounds.max.val);
  tmp2+=aux.qnextprop();
  tmp2+=aux.qdecl('type');
  tmp2+=aux.qword(this.bounds.max.type);
  tmp1+=aux.qattr(tmp2);
  ans+=aux.qattr(tmp1,1,indent);
  return ans;
};

chart.scale.FloatRange=function(dmin,dmax,tmin,tmax,typenum) {
  chart.scale.Range.call(this,'floatrange',typenum);
  var dminmax=[intobj.chknumber(dmin,0),intobj.chknumber(dmax,0)];
  if (dminmax[1]<dminmax[0])
    aux.swap(dminmax);
  this.bounds.min.val=dminmax[0];
  this.bounds.max.val=dminmax[1];
  this.bounds.min.type=chart.scale.chkrangetype(tmin);
  this.bounds.max.type=chart.scale.chkrangetype(tmax);
};

chart.scale.FloatRange.prototype=Object.create(chart.scale.Range.prototype);

chart.scale.FloatRange.prototype.constructor=chart.scale.FloatRange;

chart.scale.FloatRange.prototype.adjust=function() {
  var ans={'min':this.bounds.min.val,'max':this.bounds.max.val};
  var sgnmin=(ans.min<0 ? -1 : 1),sgnmax=(ans.max<0 ? -1 : 1);
  var scmin=intobj.scale(ans.min),scmax=intobj.scale(ans.max),rdpos=-scmax;
  var valdf=ans.max-ans.min,scdf=intobj.scale(valdf);
  if (valdf<intobj.ipow10(scdf-3)) {
    valdf=0;
    scdf=0;
  }
  if (intobj.fabs(scmax-scmin)>1 || (valdf==0 && sgnmin!=sgnmax)) {
    ans.max=floatobj.roundup(ans.max,rdpos);
    ans.min=floatobj.rounddown(ans.min,rdpos);
    if (sgnmin==sgnmax)
      ans.min=0;
    else {
      if (intobj.fabs(ans.min)<=ans.max)
        ans.min=-ans.max;
      else {
        ans.max=-ans.min;
      }
    }
  }
  else if (valdf==0 && sgnmin==sgnmax) {
    ans.min-=intobj.ipow10(-rdpos);
    ans.max+=intobj.ipow10(-rdpos);
  }
  else {
    rdpos=-scdf;
    ans.min=floatobj.rounddown(ans.min,rdpos);
    ans.max=floatobj.roundup(ans.max,rdpos);
  }
  ans.min=floatobj.roundpos(ans.min,rdpos);
  ans.max=floatobj.roundpos(ans.max,rdpos);
  return ans;
};

chart.scale.TimeRange=function(dmin,dmax,tmin,tmax,typenum) {
  chart.scale.Range.call(this,'timerange',typenum);
  var dminmax=[intobj.chknumber(dmin,0),intobj.chknumber(dmax,0)];
  if (dminmax[0]<0)
    dminmax[0]=0;
  if (dminmax[1]<0)
    dminmax[1]=0;
  if (dminmax[1]<dminmax[0])
    aux.swap(dminmax);
  this.bounds.min.val=dminmax[0];
  this.bounds.max.val=dminmax[1];
  this.bounds.min.type=chart.scale.chkrangetype(tmin);
  this.bounds.max.type=chart.scale.chkrangetype(tmax);
};

chart.scale.TimeRange.prototype=Object.create(chart.scale.Range.prototype);

chart.scale.TimeRange.prototype.constructor=chart.scale.TimeRange;

chart.scale.TimeRange.prototype.adjust=function() {
  var ans={'min':this.bounds.min.val,'max':this.bounds.max.val};
  var dtmin=new datetime.Datetime(ans.min),dtmax=new datetime.Datetime(ans.max);
  var tmmin=dtmin.toStructTm(),tmmax=dtmax.toStructTm();
  var idx=chart.scale.datetimeintvidx(ans.max-ans.min);
  if (idx>0) {
    tmmin.millisecond=0;
    tmmax.millisecond=0;
    if (idx<=5)
      tmmax=datetime.add2StructTm(tmmax,'second',1);
  }
  if (idx>5) {
    tmmin.second=0;
    tmmax.second=0;
    if (idx<=13)
      tmmax=datetime.add2StructTm(tmmax,'minute',1);
  }
  if (idx>13) {
    tmmin.minute=0;
    tmmax.minute=0;
    if (idx<=19)
      tmmax=datetime.add2StructTm(tmmax,'hour',1);
  }
  if (idx>18) {
    tmmin.hour=0;
    tmmax.hour=0;
    if (idx<=21)
      tmmax=datetime.add2StructTm(tmmax,'day',1);
  }
  if (idx>20) {
    tmmin.day=1;
    tmmax.day=1;
    if (idx<=24)
      tmmax=datetime.add2StructTm(tmmax,'month',1);
  }
  if (idx>23) {
    tmmin.month=(tmmin.month<4 ? 1 : (tmmin.month<7 ? 4 : (tmmin.month<10 ? 7 : 10)));
    tmmax.month+=+3;
    if (tmmax.month>12) {
      tmmax.month=1;
      tmmax=datetime.add2StructTm(tmmax,'year',1);
    }
  } 
  if (idx>24) {
    tmmin.month=1;
    tmmax.month=1;
    tmmax=datetime.add2StructTm(tmmax,'year',1);
  } 
  dtmin.fromStructTm(tmmin);
  dtmax.fromStructTm(tmmax);
  ans.min=dtmin.toUtcmillicnt();
  ans.max=dtmax.toUtcmillicnt();
  return ans;
};

chart.scale.TimeRange.prototype.toJson=function(indent) {
  var nextindent='  ',tmp1='',tmp2='',ans=aux.TypeId.prototype.toJson.call(this,indent);
  if (typeof indent=='string')
    nextindent+=indent;
  ans+=aux.qnextprop(1);
  ans+=aux.qdecl('bounds',indent);
  tmp1+=aux.qdecl('min',nextindent);
  tmp2=aux.qdecl('val');
  var dt=new datetime.Datetime(this.bounds.min.val);
  tmp2+=aux.qword(dt.toIso8601());
  tmp2+=aux.qnextprop();
  tmp2+=aux.qdecl('type');
  tmp2+=aux.qword(this.bounds.min.type);
  tmp1+=aux.qattr(tmp2,0);
  tmp1+=aux.qnextprop(1);
  tmp1+=aux.qdecl('max',nextindent);
  tmp2=aux.qdecl('val');
  dt=new datetime.Datetime(this.bounds.max.val);
  tmp2+=aux.qword(dt.toIso8601());
  tmp2+=aux.qnextprop();
  tmp2+=aux.qdecl('type');
  tmp2+=aux.qword(this.bounds.max.type);
  tmp1+=aux.qattr(tmp2,0);
  ans+=aux.qattr(tmp1,1,indent);
  return ans;
};

chart.scale.Scaling=function(typename,typenum) {
  if (typeof typename!='string')
    typename='scale';
  aux.TypeId.call(this,typename,typenum);
  this.range=[];
  this.tics=[];
  this.interval=NaN;
};

chart.scale.Scaling.prototype=Object.create(aux.TypeId.prototype);

chart.scale.Scaling.prototype.constructor=chart.scale.Scaling;

chart.scale.Scaling.prototype.copy=function() {
  var ans=new this.constructor(this.typename());
  ans.range.shift();
  var ii=0;
  for (ii=0;ii<this.range.length;ii++)
    ans.range.push((this.range[ii]).copy());
  for (ii=0;ii<this.tics.length;ii++)
    ans.tics.push((this.tics[ii]).copy());
  ans.interval=this.interval;
  return ans;
};

chart.scale.Scaling.prototype.setRange=function(dmin,dmax,tmin,tmax) {

};

chart.scale.Scaling.prototype.stackRange=function(dmin,dmax,tmin,tmax) {

};

chart.scale.Scaling.prototype.unstackRange=function(cnt) {
  var ll=this.range.length;
  if (intobj.chknumberrange(cnt,1,ll-1)==aux.ansko)
    cnt=ll-1;
  while (cnt>0) {
    this.range.pop();
    cnt-=1;
  }
};

chart.scale.Scaling.prototype.currRange=function(isconst) { 
  var idx=this.range.length-1;
  var ans=(idx<0 ? new chart.scale.Range() : (this.range[idx]).copy());
  return ans;
};

chart.scale.Scaling.prototype.effRange=function() {
  var ans={'min':NaN,'max':NaN};
  var idx=this.range.length-1,cnttics=this.tics.length;
  if (cnttics>=2) {
    ans.min=this.tics[0].val;
    ans.max=this.tics[cnttics-1].val;
  }
  else if (idx>=0) {
    ans.min=this.range[idx].bounds.min.val;
    ans.max=this.range[idx].bounds.max.val;
  }
  return ans;
};

chart.scale.Scaling.prototype.toJson=function(indent) {
  var nextindent='  ',ans=aux.TypeId.prototype.toJson.call(this,indent);
  if (typeof indent=='string')
    nextindent+=indent;
  ans+=aux.qnextprop(1);
  ans+=aux.qdecl('range',indent);
  var currange=this.currRange();
  ans+=aux.qattr(currange.toJson(nextindent),1,indent);
  ans+=aux.qnextprop(1);
  ans+=aux.qdecl('tics',indent);
  var tmp='';
  var ii=0;
  for (ii=0;ii<this.tics.length;ii++) {
    if (ii==0)
      tmp+=aux.qany('\n');
    else if (ii>0)
      tmp+=aux.qnextprop(1);
    tmp+=aux.qattr(this.tics[ii].toJson(),0,nextindent);
  }
  ans+=aux.qarr(tmp,1,indent);
  ans+=aux.qnextprop(1);
  ans+=aux.qdecl('interval',indent);
  ans+=aux.qword(this.interval.toString());
  return ans;
};

chart.scale.Scaling.prototype.growIntervalSize=function(sz) {
  return 2*sz;
};

chart.scale.Scaling.prototype.calcInterval=function(maxcnttics,adjustedrange) {
  return 0;
};

chart.scale.Scaling.prototype.setMajTics=function() {
  return aux.ansok;
};

chart.scale.Scaling.prototype.calcTics=function(maxcnttics) {
  return 0;
}

chart.scale.LinearScaling=function() {
  chart.scale.Scaling.call(this,'linearscale');
  var sc=new chart.scale.FloatRange(0,0)
  this.range.push(sc);
};

chart.scale.LinearScaling.prototype=Object.create(chart.scale.Scaling.prototype);

chart.scale.LinearScaling.prototype.constructor=chart.scale.LinearScaling;

chart.scale.LinearScaling.prototype.setRange=function(dmin,dmax,tmin,tmax) {
  var idx=this.range.length-1;
  this.range[idx<0 ? 0 : idx]=new chart.scale.FloatRange(dmin,dmax,tmin,tmax);
};

chart.scale.LinearScaling.prototype.stackRange=function(dmin,dmax,tmin,tmax) {
  this.range.push(new chart.scale.FloatRange(dmin,dmax,tmin,tmax));
};

chart.scale.LinearScaling.prototype.growIntervalSize=function(sz) {
  var ans=0,lggrow=Math.log(2.)/Math.LN10;
  var lower=floatobj.roundpos(Math.log(sz)/Math.LN10,intobj.maxfraction);
  var higher=floatobj.roundpos(lower+lggrow,intobj.maxfraction);
  var chsgn=((higher>0 && lower<0) || (higher<0 && lower>0) ? 1 : 0);
  if (parseInt(higher.toString())>parseInt(lower.toString()) || chsgn==1) {
    ans=intobj.ipow10(Math.floor(higher));
    if (ans>sz)
      return ans;
  }
  if (floatobj.roundpos(higher,0)>floatobj.roundpos(lower,0) || chsgn==1) {
		ans=intobj.ipow10(floatobj.roundpos(higher,0))/4;
    if (ans>sz)
      return ans;
	}
  lower=floatobj.roundpos(lower+lggrow,intobj.maxfraction);
  higher=floatobj.roundpos(higher+lggrow,intobj.maxfraction);
  chsgn=((higher>0 && lower<0) || (higher<0 && lower>0) ? 1 : 0);
  if (parseInt(higher.toString())>parseInt(lower.toString()) || chsgn==1) {
    ans=intobj.ipow10(Math.floor(higher))/2;
    if (ans>sz)
      return ans;
  }
  ans=chart.scale.Scaling.prototype.growIntervalSize.call(this,sz);
  return ans;
};

chart.scale.LinearScaling.prototype.calcInterval=function(maxcnttics,adjustedrange) {
  maxcnttics=intobj.chknumber(maxcnttics,11);
  var cdiff=adjustedrange.max-adjustedrange.min;
  var intv=intobj.ipow10(intobj.scale(cdiff)-2);
  var ntics=floatobj.roundup(cdiff/intv,0);
  while (ntics<maxcnttics) {
    intv/=10;
    ntics=floatobj.roundup(cdiff/intv,0)+1;
  }
  while (maxcnttics>2 && ntics>maxcnttics) {
    intv=this.growIntervalSize(intv);
    ntics=floatobj.roundup(cdiff/intv,0)+1;
  }
  var prec=-intobj.scale(intv);
  if (floatobj.roundpos(intv,prec)>intv)
    prec+=1;
  this.interval=floatobj.roundpos(intv,prec);
  return ntics;
};

chart.scale.LinearScaling.prototype.setMajTics=function() {
  var ii=0,ntics=this.tics.length;
  for (ii=0;ii<ntics;ii++)
    this.tics[ii].sz=0;
  var cmp=0,idx=-1,scintv=intobj.scale(this.interval),rdpos=-scintv-1;
  var rddown=0,rdup=0,rdblur=intobj.ipow10(scintv),val=NaN;
  var iintv=floatobj.roundpos(10*this.interval/rdblur,0);
  var tic=chart.scale.Tic();
  for (ii=0;ii<ntics;ii++) {
    tic=this.tics[ii];
    val=intobj.fabs(tic.val);
    cmp=floatobj.cmp(val,0,intobj.ipow10(scintv-1));
    if (cmp==0) {
      tic.sz=1;
      continue;
    }
    rdup=floatobj.roundup(val,rdpos);
    rddown=floatobj.rounddown(val,rdpos);
    cmp=floatobj.cmp(rdup,rddown,rdblur);
    if (cmp==0) {
      idx=ii;
      break;
    }
    if (iintv==10) {
      rdup=floatobj.roundup(2*val,rdpos);
      rddown=floatobj.rounddown(2*val,rdpos);
      cmp=floatobj.cmp(rdup,rddown,rdblur);
      if (cmp==0) {
        idx=ii;
        break;
      }
    }
  }
  if (idx>=0) {
    var rangeoption=this.currRange();
    tic=this.tics[idx];
    tic.sz=1;
    if (iintv==50 && (idx-2>0 || (idx-2==0 && rangeoption.bounds.min.type=='auto'))) {
      tic=this.tics[idx-2];
      tic.sz=1;
    }
    while (idx<ntics) {
      if (iintv==10 || iintv==20)
        idx+=5;
      else if (iintv==25)
        idx+=4;
      else if (iintv==50)
        idx+=2;
      else
        break;
      if (idx<ntics-1 || (idx==ntics-1 && rangeoption.bounds.max.type=='auto')) {
        tic=this.tics[idx];
        tic.sz=1;
      }
    }
  }
  return aux.ansok;
};

chart.scale.LinearScaling.prototype.calcTics=function(maxcnttics) {
  maxcnttics=intobj.chknumber(maxcnttics,11);
  this.tics=[];
  var currange=this.currRange();
  var adjustedrange=currange.adjust();
  var ii=0,idx=-1,ntics=this.calcInterval(maxcnttics,adjustedrange);
  var pos=adjustedrange.min,scintv=intobj.scale(this.interval);
  for (ii=0;ii<ntics;ii++) {
    if (pos>currange.bounds.min.val) {
      idx=ii-1;
      break;
    }
    pos=floatobj.roundpos(pos+this.interval,1-scintv);
  }
  if (idx<0)
    idx=0;
  pos=floatobj.roundpos(adjustedrange.min+idx*this.interval,1-scintv);
  for (ii=idx;ii<ntics;ii++) {
    this.tics.push(new chart.scale.Tic(pos,pos.toString()));
    if (pos>currange.bounds.max.val)
      break;
    pos=floatobj.roundpos(pos+this.interval,1-scintv);
  }
  ntics=this.tics.length;
  this.setMajTics();
  return ntics;
};

chart.scale.TimeScaling=function() {
  chart.scale.Scaling.call(this,'timescale');
  var sc=new chart.scale.TimeRange(0,0)
  this.range.push(sc);
};

chart.scale.TimeScaling.prototype=Object.create(chart.scale.Scaling.prototype);

chart.scale.TimeScaling.prototype.constructor=chart.scale.TimeScaling;

chart.scale.TimeScaling.prototype.setRange=function(dmin,dmax,tmin,tmax) {
  var idx=this.range.length-1;
  this.range[idx<0 ? 0 : idx]=new chart.scale.TimeRange(dmin,dmax,tmin,tmax);
};

chart.scale.TimeScaling.prototype.stackRange=function(dmin,dmax,tmin,tmax) {
  this.range.push(new chart.scale.TimeRange(dmin,dmax,tmin,tmax));
};

chart.scale.TimeScaling.prototype.growIntervalSize=function(sz) {
  var ii=0,ans=sz;
  for (ii=0;ii<chart.scale.datetimeintv.length;ii++) {
    if (chart.scale.datetimeintv[ii]>sz) {
      ans=chart.scale.datetimeintv[ii];
      break;
    }
  }
  while (ans<=sz)
    ans=chart.scale.Scaling.prototype.growIntervalSize.call(this,sz);
  return ans;
};

chart.scale.TimeScaling.prototype.calcInterval=function(maxcnttics,adjustedrange) {
  maxcnttics=intobj.chknumber(maxcnttics,13);
  var dfadj=adjustedrange.max-adjustedrange.min;
  var ii=0,intv=1,ntics=Math.ceil(dfadj/intv)+1;
  while (maxcnttics>2 && ntics>maxcnttics) {
    intv=this.growIntervalSize(intv);
    ntics=Math.ceil(dfadj/intv)+1;
  }  
  this.interval=floatobj.roundpos(intv,0);
  return ntics;
};

chart.scale.TimeScaling.prototype.setMajTics=function() {
  var ii=0,idx=chart.scale.datetimeintvidx(this.interval),ntics=this.tics.length;
  for (ii=0;ii<ntics;ii++)
    this.tics[ii].sz=0;
  var dt=new datetime.Datetime();
  var tm=dt.toStructTm();
  var tic=chart.scale.Tic();
  for (ii=0;ii<ntics;ii++) {
    tic=this.tics[ii];
    dt.fromUtcmillicnt(tic.val);
    tm=dt.toStructTm();
    if (idx<3) {
      if (tm.millisecond==0 && (tm.second==0 || (tm.second%10)==0))
        tic.sz=1;
    }
    else if (idx<6) {
      if (tm.second==0)
        tic.sz=1;
    }
    else if (idx<11) {
      if (tm.minute==0 || tm.minute==30)
        tic.sz=1;
    }
    else if (idx<13) {
      if (tm.minute==0)
        tic.sz=1;
    }
    else if (idx<16) {
      if (tm.hour==0 || tm.hour==12)
        tic.sz=1;
    }
    else if (idx<18) {
      if (tm.hour==0)
        tic.sz=1;
    }
    else if (idx<22) {
      if (tm.day==1 || (idx<21 && tm.day==15))
        tic.sz=1;
    }
    else if (idx<23) {
      if (tm.month==1 || tm.month==4 || tm.month==7 || tm.month==10)
        tic.sz=1;
    }
    else if (idx<26) {
      if (tm.month==1)
        tic.sz=1;
    }
    else {
      if ((tm.year%5)==0)
        tic.sz=1;
    }
  }
  return aux.ansok;
};

chart.scale.TimeScaling.prototype.calcTics=function(maxcnttics) {
  maxcnttics=intobj.chknumber(maxcnttics,11);
  this.tics=[];
  var currange=this.currRange();
  var adjustedrange=currange.adjust();
  var ntics=this.calcInterval(maxcnttics,adjustedrange);
  var pos=adjustedrange.min;
  var strpos='';
  var dt=new datetime.Datetime();
  var tm=dt.toStructTm();
  var fuzzydays=[2,7,14,30,60,90,180,365,730];
  var ii=0,idx=chart.scale.datetimeintvidx(this.interval),chkultimo=0;
  var fmt=chart.scale.datetimefmtfromintv(this.interval);
  idx-=19;
  for (ii=0;ii<ntics;ii++) {
    dt.fromUtcmillicnt(pos);
    this.tics.push(new chart.scale.Tic(pos,dt.toStructTmString(fmt)));
    if (pos>currange.bounds.max.val)
      break;
    if (idx<0)
      pos=floatobj.roundpos(pos+this.interval,0);
    else {
      tm=dt.toStructTm();
      if (idx<3) {
        chkultimo=datetime.monthdays(tm.month,tm.year)-tm.day;
        if ((idx==0 && chkultimo<fuzzydays[idx]) ||
            (idx>0 && (
             (tm.month!=2 && chkultimo<2*fuzzydays[idx]) ||
             (tm.month==2 && chkultimo<=fuzzydays[idx])
            )
           )
          ) {
          tm.day=1;
          tm=datetime.add2StructTm(tm,'month',1);
          dt.fromStructTm(tm);
          pos=dt.toUtcmillicnt();
        }
        else
          pos=floatobj.roundpos(pos+this.interval,0);
      }
      else {
        tm.day=1;
        if (idx<7)
          tm=datetime.add2StructTm(tm,'month',fuzzydays[idx]/30);
        else {
          tm.month=1;
          tm=datetime.add2StructTm(tm,'year',fuzzydays[idx]/365);
        }
        dt.fromStructTm(tm);
        pos=dt.toUtcmillicnt();
      }
    }
  }
  ntics=this.tics.length;
  this.setMajTics();
  return ntics;
}

