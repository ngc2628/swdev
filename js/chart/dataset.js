
chart.dataset={};

/* ########## */
chart.dataset.Data=function(typename,typenum) {
  if (typeof typename!='string')
    typename='dataset';
  aux.TypeId.call(this,typename,typenum);
  this.vertices=[];
  this.bounds=[new mkmat.Vertex(),new mkmat.Vertex()];
  this.sorted=-1;
  this.datatype=[{'type':'none'},{'type':'none'},{'type':'none'},{'type':'none'}];
  this.descr='';
};

/* ********** */
chart.dataset.Data.prototype=Object.create(aux.TypeId.prototype);

/* ********** */
chart.dataset.Data.prototype.constructor=chart.dataset.Data;

/* ********** */
chart.dataset.Data.prototype.findBounds=function() {
  this.bounds=[new mkmat.Vertex(),new mkmat.Vertex()];
  var dval=NaN;
  var ii=0,jj=0;
  for (ii=0;ii<this.vertices.length;ii++) {
    for (jj=0;jj<4;jj++) {
      dval=this.vertices[ii].val(jj);
      if (!isNaN(dval)) {
        if (isNaN(this.bounds[0].val(jj)) || dval<this.bounds[0].val(jj))
          this.bounds[0].setVal(jj,dval);
        if (isNaN(this.bounds[1].val(jj)) || this.bounds[1].val(jj)<dval)
          this.bounds[1].setVal(jj,dval);
      }
    }
  }
};

/* ********** */
chart.dataset.Data.prototype.getBounds=function(typeidx) {
  var ans={'min':NaN,'max':NaN};
  if (mkint.chknumberrange(typeidx,mkuni.typeX,mkuni.typeW)==mkuni.good) {
    ans.min=this.bounds[0].val(typeidx);
    ans.max=this.bounds[1].val(typeidx);
  }
  return ans;
};

/* ********** */
chart.dataset.Data.prototype.fromJsonObj=function(dataobj) {
  if (typeof dataobj=='undefined')
    return mkuni.bad;
  var cntvertices=0;
  if (dataobj.hasOwnProperty('len')) {
    if (parseInt(dataobj.len,10)>0)
      cntvertices=parseInt(dataobj.len,10);
  }
  if (cntvertices==0)
    return mkuni.bad;
  this.vertices=[];
  this.bounds=[new mkmat.Vertex(),new mkmat.Vertex()];
  this.sorted=-1;
  this.datatype=[{'type':'none'},{'type':'none'},{'type':'none'},{'type':'none'}];
  this.descr=(dataobj.hasOwnProperty('descr') ? dataobj.descr : '');
  var rtime=new datetime.Datetime();
  if (dataobj.hasOwnProperty('x')) {
    if (dataobj.x.hasOwnProperty('type')) {
      this.datatype[0].type=dataobj.x.type;
      if (this.datatype[0].type=='time') {
        if (dataobj.x.hasOwnProperty('scale'))
          this.datatype[0].scale=dataobj.x.scale;
        if (dataobj.x.hasOwnProperty('reftime')) {
          this.datatype[0].reftime=dataobj.x.reftime;
          if (this.datatype[0].reftime!='now')
            rtime.fromIso8601(this.datatype[0].reftime);
        }
      }
    }
  }
  if (dataobj.hasOwnProperty('y')) {
    if (dataobj.y.hasOwnProperty('type'))
      this.datatype[1].type=dataobj.y.type;
  }
  if (dataobj.hasOwnProperty('z')) {
    if (dataobj.z.hasOwnProperty('type'))
      this.datatype[2].type=dataobj.z.type;
  }
  if (dataobj.hasOwnProperty('w')) {
    if (dataobj.w.hasOwnProperty('type'))
      this.datatype[3].type=dataobj.w.type;
  }
  if (this.datatype[0].type=='time') {
    if (this.datatype[0].scale=='hour')
      rtime.fromStructTmValues(-1,-1,-1,-1,0,0,0,0);
    else if (this.datatype[0].scale=='day')
      rtime.fromStructTmValues(-1,-1,1,0,0,0,0,0);
    else if (this.datatype[0].scale=='month')
      rtime.fromStructTmValues(-1,-1,1,0,0,0,0,0);
  }
  var rtimecnt=rtime.toUtcmillicnt();
  var tmrt=rtime.toStructTm();
  var strvertex=[];
  var vv=new mkmat.Vertex();
  var ii=0,jj=0,sgn=1,toff=0,cnt=0;
  for (ii=0;ii<cntvertices;ii++) {
    strvertex=dataobj.vertices[ii];
    vv=new mkmat.Vertex(parseFloat(strvertex[0],10),parseFloat(strvertex[1],10));
    if (isNaN(vv.val(0)))
      continue;
    sgn=(vv.val(0)<0 ? -1 : 1);
    if (this.datatype[0].type=='time') {
      toff=mkfloat.fabs(vv.val(0));
      cnt=toff;
      if (this.datatype[0].scale=='month') {
        cnt=0;
        tmrt=rtime.toStructTm();
        for (jj=0;jj<toff;jj++) {
          if (sgn<0)
            tmrt=datetime.subs2StructTm(tmrt,'month',1);
          else
            tmrt=datetime.add2StructTm(tmrt,'month',1);
          cnt+=datetime.monthcnt(tmrt.month,tmrt.year);
        }
      }
      else if (this.datatype[0].scale=='day')
        cnt*=datetime.adaymillis;
      else if (this.datatype[0].scale=='hour')
        cnt*=datetime.anhourmillis;
      if (sgn<0)
        cnt=rtimecnt-cnt;
      else
        cnt=rtimecnt+cnt;
      vv.setVal(0,cnt);
    }
    this.vertices.push(vv);
  }
  return mkuni.good;
};

/* ********** */
chart.dataset.Data.prototype.fromJson=function(str) {
  if ((typeof str!='string') || str.length==0)
    return mkuni.bad;
  var dataobj;
  try {
    dataobj=JSON.parse(str);
  }
  catch(exc) {
    alert(str);
    return mkuni.bad;
  }
  if (typeof dataobj=='undefined') {
    alert(str);
    return mkuni.bad;
  }
  return this.fromJsonObj(dataobj);
};

/* ********** */
chart.dataset.Data.prototype.toJson=function(indent) {
  var nextindent='  ',ans=aux.qdecl('descr',indent);
  if (typeof indent=='string')
    nextindent+=indent;
  ans+=aux.qword(this.descr);
  ans+=aux.qnextprop(1);
  ans+=aux.qdecl('len',indent);
  ans+=aux.qword(this.vertices.length);
  ans+=aux.qnextprop(1);
  ans+=aux.qdecl('datatype',indent);
  var tmp1='',tmp2='';
  var ii=0,jj=0;
  for (ii=0;ii<4;ii++) {
    if (ii==0)
      tmp2+=aux.qany('\n');
    else if (ii>0)
      tmp2+=aux.qnextprop(1);
    tmp1=aux.qdecl('type');
    tmp1+=aux.qword(this.datatype[ii].type);
    if (this.datatype[ii].hasOwnProperty('scale')) {
      tmp1+=aux.qnextprop();
      tmp1+=aux.qdecl('scale');
      tmp1+=aux.qword(this.datatype[ii].scale);
    }
    if (this.datatype[ii].hasOwnProperty('reftime')) {
      tmp1+=aux.qnextprop();
      tmp1+=aux.qdecl('reftime');
      tmp1+=aux.qword(this.datatype[ii].reftime);
    }
    tmp2+=aux.qattr(tmp1,0,nextindent);
  }
  ans+=aux.qarr(tmp2,1,indent);
  ans+=aux.qnextprop(1);
  tmp1='';
  tmp2='';
  ans+=aux.qdecl('vertices',indent);
  var tt=new datetime.Datetime();
  for (ii=0;ii<this.vertices.length;ii++) {
    if (ii==0)
      tmp1+=aux.qany('\n',nextindent);
    if (ii>0)
      tmp1+=aux.qnextprop(1,nextindent);
    tmp2='';
    for (jj=0;jj<4;jj++) {
      if (this.datatype[jj].type=='time') {
        tt.fromUtcmillicnt(this.vertices[ii].val(jj));
        tmp2+=aux.qword(tt.toIso8601());
      }
      else if (this.datatype[jj].type=='float') {
        if (!isNaN(this.vertices[ii].val(jj)))
          tmp2+=aux.qword(mkfloat.sprintf(this.vertices[ii].val(jj),6));
        else
          tmp2+='null';
      }
      else
        tmp2+='null';
      if (jj<3)
        tmp2+=aux.qnextprop();
    }
    tmp1+=aux.qarr(tmp2);
  }
  ans+=aux.qarr(tmp1,1,indent);
  ans+=aux.qnextprop(1);
  ans+=aux.qdecl('min',indent);
  ans+=aux.qattr(this.bounds[0].toJson());
  ans+=aux.qnextprop(1);
  ans+=aux.qdecl('max',indent);
  ans+=aux.qattr(this.bounds[1].toJson());
  ans+=aux.qnextprop(1);
  ans+=aux.qdecl('sorted',indent);
  ans+=aux.qword(this.sorted.toString());
  return ans;
};
/* ########## */

/* ########## */
chart.dataset.DataSinCos=function(typenum) {
  chart.dataset.Data.call(this,'sinus',typenum);
  this.descr='sinus testdata';
  this.datatype[0].type='float';
  this.datatype[1].type='float';
};

/* ********** */
chart.dataset.DataSinCos.prototype=Object.create(chart.dataset.Data.prototype);

/* ********** */
chart.dataset.DataSinCos.prototype.constructor=chart.dataset.DataSinCos;

/* ********** */
chart.dataset.DataSinCos.prototype.genData=function(sincos,cntvertices,amp,offset) {
  if ((typeof sincos!='string') || (sincos!='sin' && sincos!='cos'))
    sincos='sin';
  if (mkint.chknumberrange(cntvertices,5,1800)==mkuni.bad)
    cntvertices=60;
  amp=mkuni.chknumber(amp,100);
  offset=mkuni.chknumber(offset,0);
  var rtime=new datetime.Datetime();
  if (this.datatype[0].type=='time') {
    if (this.datatype[0].hasOwnProperty('reftime')) {
      if (this.datatype[0].reftime!='now')
        rtime.fromIso8601(this.datatype[0].reftime);
    }
    if (this.datatype[0].hasOwnProperty('scale')) {
      if (this.datatype[0].scale=='day')
        rtime.fromStructTmValues(-1,-1,1,0,0,0,0,0);
      else if (this.datatype[0].scale=='month')
        rtime.fromStructTmValues(-1,-1,1,0,0,0,0,0);
      else {
        if (this.datatype[0].scale!='hour')
          this.datatype[0].scale='hour';
        rtime.fromStructTmValues(-1,-1,-1,-1,0,0,0,0);
      }
    }
    else {
      this.datatype[0].scale='hour';
      rtime.fromStructTmValues(-1,-1,-1,-1,0,0,0,0);
    }
  }
  var tmrt=rtime.toStructTm();
  var xx=offset,dfx=cntvertices/(2*Math.PI);
  var vv=new mkmat.Vertex();
  var ii=0;
  for (ii=0;ii<cntvertices;ii++) {
    vv=new mkmat.Vertex();
    if (this.datatype[0].type=='time') {
      tmrt=rtime.toStructTm();
      if (this.datatype[0].scale=='month')
        tmrt=datetime.add2StructTm(tmrt,'month',1);
      else if (this.datatype[0].scale=='day')
        tmrt=datetime.add2StructTm(tmrt,'day',1);
      else if (this.datatype[0].scale=='hour')
        tmrt=datetime.add2StructTm(tmrt,'hour',1);
      rtime.fromStructTm(tmrt);
      xx=rtime.toUtcmillicnt();
    }
    else {
      xx+=dfx;
    }
    vv.setVal(0,xx);
    if (sincos=='sin')
      vv.setVal(1,amp*Math.sin(xx));
    else
      vv.setVal(1,amp*Math.cos(xx));
    this.vertices.push(vv);
  }
};
/* ########## */





