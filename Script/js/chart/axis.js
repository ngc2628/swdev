
chart.axis={};

chart.axis.typeLeft=1;
chart.axis.typeRight=2;
chart.axis.typeTop=4;
chart.axis.typeBottom=8;
chart.axis.typeShowLine=1;
chart.axis.typeShowNotch=2;
chart.axis.typeShowText=4;

chart.axis.Axis=function(typename,typenum) {
  if (typeof typename!='string')
    typename='axis';
  chart.scale.Scale.call(this,typename,typenum);
  this.pos=0;
  this.visible=(chart.axis.typeShowLine|chart.axis.typeShowNotch|chart.axis.typeShowText);
  this.linestyle={'color':0,'style':1,'width':1};
  this.scaling=new chart.scale.Scaling();
  this.graphs=[];
};

chart.axis.Axis.prototype=Object.create(chart.scale.Scale.prototype);

chart.axis.Axis.prototype.constructor=chart.axis.Axis;

chart.axis.Axis.prototype.findGraph=function(graph) {
  if (graph instanceof aux.TypeId) {
    var idx=aux.bsearch(graph,this.graphs,aux.cmptypeid);
    if (idx>=0)
      return this.graphs[idx];
  }
  return void(0);
};

chart.axis.Axis.prototype.assignGraph=function(graph) {
  var ans=aux.ansko;
  if (graph instanceof aux.TypeId) {
    var idx=aux.bsearch(graph,this.graphs,aux.cmptypeid);
    if (idx<0) {
      this.graphs.push(graph);
      this.graphs.sort(aux.cmptypeid);
      if (graph instanceof chart.graph.Graph)
        graph.setScale(this);
      ans=aux.ansok;
    }
  }
  return ans;
};

chart.axis.Axis.prototype.unassignGraph=function(graph) {
  var ans=aux.ansko;
  if (graph instanceof aux.TypeId) {
    var idx=aux.bsearch(graph,this.graphs,aux.cmptypeid);
    if (idx>=0) {
      this.graphs.splice(idx,1);
      if (graph instanceof chart.graph.Graph)
        graph.unsetScale(this);
      ans=aux.ansok;
    }
  }
  return ans;
};

chart.axis.Axis.prototype.setScaling=function(sc) {
  if (sc instanceof chart.scale.Scaling)
    this.scaling=sc.copy();
};

chart.axis.Axis.prototype.getScaling=function() {
  var ans=this.scaling.copy();
  return ans;
};

chart.axis.Axis.prototype.calcRange=function(dir) {
  var currange=this.scaling.currRange();
  if (currange.bounds.min.type=='static' && currange.bounds.max.type=='static')
    return aux.ansnop;
  var accmin=NaN,accmax=NaN;
  var graph,graphdata;
  var bounds={'min':NaN,'max':NaN};
  var ii=0;
  for (ii=0;ii<this.graphs.length;ii++) {
    graph=this.graphs[ii];
    if (graph instanceof chart.graph.Graph) {
      graphdata=graph.getData();
      if (graphdata instanceof chart.dataset.Data) {
        bounds=graphdata.getBounds(dir);
        if (!isNaN(bounds.min)) {
          if (isNaN(accmin) || bounds.min<accmin)
            accmin=bounds.min;
        }
        if (!isNaN(bounds.max)) {
          if (isNaN(accmax) || accmax<bounds.max)
            accmax=bounds.max;
        }
      }
    }
  }
  if (currange.bounds.min.type=='static')
    accmin=currange.bounds.min.val;
  if (currange.bounds.max.type=='static')
    accmax=currange.bounds.max.val;
  this.scaling.setRange(accmin,accmax,currange.bounds.min.type,currange.bounds.max.type);
  return aux.ansok;
};

chart.axis.Axis.prototype.setPos=function(pos) {
  return 0;
};

chart.axis.Axis.prototype.sc2sz=function(vertex) {
  return NaN;
};

chart.axis.Axis.prototype.sz2sc=function(vertex) {
  return NaN;
};

chart.axis.Axis.prototype.findRect=function(ctx,rect,space) {
  return rect.copy();
};

chart.axis.Axis.prototype.setStyle=function(cc,ss,ww) {
  cc=intobj.chknumber(cc,this.linestyle.color);
  if (intobj.chknumberrange(cc,0,16777215)==aux.ansko)
    cc=this.linestyle.color;
  ss=intobj.chknumber(ss,this.linestyle.style);
  if (intobj.chknumberrange(ss,0,8)==aux.ansko)
    ss=this.linestyle.style;
  ww=intobj.chknumber(ww,this.linestyle.width);
  if (intobj.chknumberrange(ww,0,16)==aux.ansko)
    ww=this.linestyle.width;
  this.linestyle.color=cc;
  this.linestyle.style=ss;
  this.linestyle.width=ww;
};

chart.axis.Axis.prototype.getStyle=function() {
  var ans={
    'color':this.linestyle.color,'style':this.linestyle.style,'width':this.linestyle.width
  };
  return ans;
};

chart.axis.Axis.prototype.mendScaling=function(ctx) {
  return aux.ansko;
};

chart.axis.Axis.prototype.drawCanvas2d=function(ctx) {
  return aux.ansko;
};

chart.axis.Axis.prototype.drawSvg=function(ctx) {
  return aux.ansko;
};

chart.axis.Axis.prototype.draw=function(ctx) {
  var ans=aux.ansko;
  if (ctx instanceof CanvasRenderingContext2D) {
    ans=this.drawCanvas2d(ctx);
  }
  else if (ctx instanceof SVGElement) {
    ans=this.drawSvg(ctx);
  }
  return ans;
};

chart.axis.Axis.prototype.toJson=function(indent) {
  var nextindent='  ',ans=chart.scale.Scale.prototype.toJson.call(this,indent);
  if (typeof indent=='string')
    nextindent+=indent;
  ans+=aux.qnextprop(1);
  ans+=aux.qdecl('pos',indent);
  ans+=aux.qword(this.pos.toString());
  ans+=aux.qnextprop(1);
  ans+=aux.qdecl('visible',indent);
  ans+=aux.qword(this.visible.toString());
  ans+=aux.qnextprop(1);
  ans+=aux.qdecl('linestyle',indent);
  var tmp=aux.qdecl('color');
  tmp+=aux.qword(this.linestyle.color.toString());
  tmp+=aux.qnextprop();
  tmp+=aux.qdecl('style');
  tmp+=aux.qword(this.linestyle.style.toString());
  tmp+=aux.qnextprop();
  tmp+=aux.qdecl('width');
  tmp+=aux.qword(this.linestyle.width.toString());
  ans+=aux.qattr(tmp);
  ans+=aux.qnextprop(1);
  ans+=aux.qdecl('scaling',indent);  
  ans+=aux.qattr(this.scaling.toJson(nextindent),1,indent);
  ans+=aux.qnextprop(1);
  ans+=aux.qdecl('graphids',indent);  
  tmp='';
  var ii=0;
  for (ii=0;ii<this.graphs.length;ii++) {
    tmp+=aux.qword(aux.TypeId.prototype.toString.call(this.graphs[ii]));
    if (ii<this.graphs.length-1)
      tmp+=aux.qnextprop();
  }
  ans+=aux.qarr(tmp,0,indent);
  return ans;
};

chart.axis.Xaxis=function(typenum) {
  chart.axis.Axis.call(this,'xaxis',typenum);
  this.pos=chart.axis.typeBottom;
};

chart.axis.Xaxis.prototype=Object.create(chart.axis.Axis.prototype);

chart.axis.Xaxis.prototype.constructor=chart.axis.Xaxis;

chart.axis.Xaxis.prototype.assignGraph=function(graph) {
  var ans=chart.axis.Axis.prototype.assignGraph.call(this,graph);
  return ans;
};

chart.axis.Xaxis.prototype.calcRange=function(dir) {
  chart.axis.Axis.prototype.calcRange.call(this,intobj.typeX);
};

chart.axis.Xaxis.prototype.setPos=function(pos) {
  pos=intobj.chknumber(pos,chart.axis.typeBottom);
  if (pos!=chart.axis.typeBottom && pos!=chart.axis.typeTop)
    pos=chart.axis.typeBottom;
  this.pos=pos;
  return this.pos;
};

chart.axis.Xaxis.prototype.sc2sz=function(vertex) {
  var ans=NaN,minmax=this.scaling.effRange();
  var dfrange=minmax.max-minmax.min,dfsz=this.rect.right()-this.rect.left();
  if (dfrange>0)
    ans=this.rect.left()+(vertex.val(0)-minmax.min)*dfsz/dfrange;
  return ans;
};

chart.axis.Xaxis.prototype.sz2sc=function(vertex) {
  var ans=NaN,minmax=this.scaling.effRange();
  var dfrange=minmax.max-minmax.min,dfsz=this.rect.right()-this.rect.left();
  if (dfsz>0 && (vertex instanceof floatobj.Vertex))
    ans=minmax.min+(vertex.val(0)-this.rect.left())*dfrange/dfsz;
  return ans;
};

chart.axis.Xaxis.prototype.findRect=function(ctx,rect,space) {
  var majfntstr=chart.fnt2str(this.font.major),minfntstr=chart.fnt2str(this.font.minor);
  var ans=rect.copy(),pos=0;
  this.rect=rect.copy();
  if (this.pos==chart.axis.typeBottom) {
    pos=rect.bottom()-parseInt(this.font.major.pxsz,10)-2*parseInt(this.font.minor.pxsz,10)-space;
    this.rect.setTop(pos);
    ans.setBottom(pos);
  }
  else {
    pos=rect.bottom()+parseInt(this.font.major.pxsz,10)+2*parseInt(this.font.minor.pxsz,10)+space;
    this.rect.setBottom(pos);
    ans.setTop(pos);
  }
  return ans;
};

chart.axis.Xaxis.prototype.drawCanvas2d=function(ctx) {
  ctx.textBaseline='top';  
  ctx.textAlign='center';  
  ctx.strokeStyle=('#'+intobj.bits2str(intobj.str2bits(this.linestyle.color.toString(),10),16));
  ctx.fillStyle=ctx.strokeStyle;
  var base=this.rect.top(),loff=0,tboff=0;
  var fntoff=[
    intobj.cutfloat(parseFloat(this.font.minor.pxsz)),
    intobj.cutfloat(parseFloat(this.font.major.pxsz))
  ];
  if (this.pos==chart.axis.typeTop) {
    base=this.rect.bottom();
    fntoff[0]=-fntoff[0];
    fntoff[1]=-fntoff[1];
    ctx.textBaseline='bottom';
  }
  var ii=0,fnt2off=intobj.cutfloat(parseFloat(fntoff[0]/2));
  var fntstr=[chart.fnt2str(this.font.minor),chart.fnt2str(this.font.major)];
  ctx.beginPath();
  ctx.moveTo(this.rect.left(),base);
  ctx.lineTo(this.rect.right(),base);
  ctx.stroke();
  ctx.beginPath();
  var tic=new chart.scale.Tic();
  var vertex=new floatobj.Vertex();
  for (ii=0;ii<this.scaling.tics.length;ii++) {
    tboff=base;
    tic=this.scaling.tics[ii];
    vertex.setVal(0,tic.val);
    loff=this.sc2sz(vertex);
    ctx.font=fntstr[tic.sz];
    ctx.moveTo(loff,tboff);
    tboff+=(tic.sz==1 ? fntoff[0] : fnt2off);
    ctx.lineTo(loff,tboff);
    tboff+=fnt2off;
    ctx.fillText(tic.str,loff,tboff);  
  }
  ctx.stroke();
  return aux.ansok;
};

chart.axis.Xaxis.prototype.drawSvg=function(ctx) {
  var gxax=chart.findElementById(this.toString());
  if (!gxax) {
    gxax=chart.svgCreateElement('g');
    if (!gxax)
      return aux.ansko;
    gxax.setAttribute('id',this.toString());
    chart.svgAddElement(ctx,gxax);
  }
  while (gxax.firstChild)
    gxax.removeChild(gxax.firstChild);
  var base=this.rect.top(),loff=0,tboff=0;
  var fntoff=[
    intobj.cutfloat(parseFloat(this.font.minor.pxsz)),
    intobj.cutfloat(parseFloat(this.font.major.pxsz))
  ];
  if (this.pos==chart.axis.typeTop) {
    base=this.rect.bottom();
    fntoff[0]=-fntoff[0];
    fntoff[1]=-fntoff[1];
  }
  var strokestyle=('#'+intobj.bits2str(intobj.str2bits(this.linestyle.color.toString(),10),16));
  var svgtictxt=void(0);
  var vertex=new floatobj.Vertex(this.rect.left(),base);
  var strpath=chart.svgPath2('M',vertex);
  vertex.setVal(0,this.rect.right());
  strpath+=chart.svgPath2('L',vertex);
  var tic=new chart.scale.Tic();
  var ii=0,fnt2off=intobj.cutfloat(parseFloat(fntoff[0]/2));
  var fntstr=[chart.fnt2str(this.font.minor),chart.fnt2str(this.font.major)];
  for (ii=0;ii<this.scaling.tics.length;ii++) {
    tboff=base;
    tic=this.scaling.tics[ii];
    vertex.setVal(0,tic.val);
    loff=this.sc2sz(vertex);
    vertex.setVal(0,loff);
    vertex.setVal(1,tboff);
    strpath+=chart.svgPath2('M',vertex);
    tboff+=(tic.sz==1 ? fntoff[0] : fnt2off);
    vertex.setVal(1,tboff);
    strpath+=chart.svgPath2('L',vertex);
    tboff+=fntoff[tic.sz];
    tboff+=fnt2off;
    vertex.setVal(1,tboff);
    svgtictxt=chart.svgCreateElement('text');
    svgtictxt.setAttribute('x',floatobj.sprintf(vertex.val(0),0,'d'));
    svgtictxt.setAttribute('y',floatobj.sprintf(vertex.val(1),0,'d'));
    svgtictxt.setAttribute('text-anchor','middle');
    txtstyle=('fill:'+strokestyle+';font:');
    txtstyle+=fntstr[tic.sz];
    svgtictxt.setAttribute('style',txtstyle);
    svgtictxt.textContent=tic.str;
    chart.svgAddElement(gxax,svgtictxt);
  }
  var svgbase=chart.svgCreateElement('path');
  svgbase.setAttribute('d',strpath);
  svgbase.setAttribute('stroke',strokestyle);
  svgbase.setAttribute('stroke-width',this.linestyle.width.toString());
  svgbase.setAttribute('fill','none');
  chart.svgAddElement(gxax,svgbase);

//alert(document.body.innerHTML);

  return aux.ansok;
};

chart.axis.Xaxis.prototype.mendScaling=function(ctx) {
  var majfntstr=chart.fnt2str(this.font.major),minfntstr=chart.fnt2str(this.font.minor);
  var minfntoff=parseInt(this.font.minor.pxsz,10),majfntoff=parseInt(this.font.major.pxsz,10);
  var ii=0,cnt=0,lastcnt=0,fast=0,ww=this.rect.width();
  var currange=this.scaling.currRange();
  var adjustedrange=currange.adjust();
  var ntics=this.scaling.calcInterval(50,adjustedrange);
  var chkstr=this.scaling.interval.toString();
  if (this.scaling.typename()=='timescale') {
    var fmt=chart.scale.datetimefmtfromintv(this.scaling.interval);
    var dt=new datetime.Datetime();
    chkstr=dt.toStructTmString(fmt);
  }
  var svgtxt=void(0);
  if (ctx instanceof SVGElement) {
    svgtxt=chart.svgCreateElement('text');
    chart.svgAddElement(ctx,svgtxt);
  }
  var wtxt=chart.wtext(chkstr,minfntstr,svgtxt ? svgtxt : ctx);
  var maxtics=intobj.cutfloat(ww/wtxt),lastmaxtics=maxtics;
  var tic=new chart.scale.Tic();
  while (ww>=this.rect.width() && maxtics>2) {
    cnt=this.scaling.calcTics(maxtics);
    if (cnt!=lastcnt) {
      ww=0;
      for (ii=0;ii<cnt;ii++) {
        tic=this.scaling.tics[ii];
        wtxt=chart.wtext(tic.str,(tic.sz==1 ? majfntstr : minfntstr),svgtxt ? svgtxt : ctx);
        ww+=wtxt;
      }
      lastcnt=cnt;
    }
    if (fast==1) {
      if (ww<this.rect.width()) {
        fast=0;
        ww=this.rect.width();
        maxtics=lastmaxtics-1;
      }
      else {
        lastmaxtics=maxtics;
        maxtics=intobj.cutfloat(maxtics/2);
      }
    }
    else
      maxtics--;
  }
  chart.svgRemoveElement(ctx,svgtxt);
};

chart.axis.Yaxis=function(typenum) {
  chart.axis.Axis.call(this,'yaxis',typenum);
  this.pos=chart.axis.typeLeft;
};

chart.axis.Yaxis.prototype=Object.create(chart.axis.Axis.prototype);

chart.axis.Yaxis.prototype.constructor=chart.axis.Yaxis;

chart.axis.Yaxis.prototype.assignGraph=function(graph) {
  var ans=chart.axis.Axis.prototype.assignGraph.call(this,graph);
  return ans;
};

chart.axis.Yaxis.prototype.calcRange=function(dir) {
  chart.axis.Axis.prototype.calcRange.call(this,intobj.typeY);
};

chart.axis.Yaxis.prototype.setPos=function(pos) {
  pos=intobj.chknumber(pos,chart.axis.typeLeft);
  if (pos!=chart.axis.typeLeft && pos!=chart.axis.typeRight)
    pos=chart.axis.typeLeft;
  this.pos=pos;
  return this.pos;
};

chart.axis.Yaxis.prototype.sc2sz=function(vertex) {
  var ans=NaN,minmax=this.scaling.effRange();
  var dfrange=minmax.max-minmax.min,dfsz=this.rect.bottom()-this.rect.top();
  if (dfrange>0 && (vertex instanceof floatobj.Vertex))
    ans=this.rect.bottom()-(vertex.val(1)-minmax.min)*dfsz/dfrange;
  return ans;
};

chart.axis.Yaxis.prototype.sz2sc=function(vertex) {
  var ans=NaN,minmax=this.scaling.effRange();
  var dfrange=minmax.max-minmax.min,dfsz=this.rect.bottom()-this.rect.top();
  if (dfsz>0 && (vertex instanceof floatobj.Vertex))
    ans=minmax.min+(this.rect.bottom()-vertex.val(1))*dfrange/dfsz;
  return ans;
};

chart.axis.Yaxis.prototype.findRect=function(ctx,rect,space) {
  var majfntstr=chart.fnt2str(this.font.major),minfntstr=chart.fnt2str(this.font.minor);
  var ans=rect.copy();
  this.rect=rect.copy();
  var maxtics=intobj.cutfloat(rect.height()/parseFloat(this.font.major.pxsz));
  this.scaling.calcTics(maxtics);
  var svgtxt=void(0);
  if (ctx instanceof SVGElement) {
    svgtxt=chart.svgCreateElement('text');
    chart.svgAddElement(ctx,svgtxt);
  }
  var ii=0,ww=0,wwmax=0;
  for (ii=0;ii<this.scaling.tics.length;ii++) {
    tic=this.scaling.tics[ii];
    ww=chart.wtext(tic.str,(tic.sz==1 ? majfntstr : minfntstr),svgtxt ? svgtxt : ctx);
    if (ww>wwmax)
      wwmax=ww;
  }
  chart.svgRemoveElement(ctx,svgtxt);
  if (this.pos==chart.axis.typeLeft) {
    ww=rect.left()+wwmax+2*parseInt(this.font.minor.pxsz,10)+space;
    this.rect.setRight(ww);
    ans.setLeft(ww);
  }
  else {
    ww=rect.right()-wwmax-2*parseInt(this.font.minor.pxsz,10)-space;
    this.rect.setLeft(ww);
    ans.setRight(ww);
  }
  return ans;
};

chart.axis.Yaxis.prototype.drawCanvas2d=function(ctx) {
  ctx.textBaseline='middle';  
  ctx.textAlign='end';  
  ctx.strokeStyle=('#'+intobj.bits2str(intobj.str2bits(this.linestyle.color.toString(),10),16));
  ctx.fillStyle=ctx.strokeStyle;
  var fntstr=[chart.fnt2str(this.font.minor),chart.fnt2str(this.font.major)];
  var base=this.rect.right(),loff=0,toff=0;
  var fntoff=[
    intobj.cutfloat(parseFloat(this.font.minor.pxsz)),
    intobj.cutfloat(parseFloat(this.font.major.pxsz))
  ];
  if (this.pos==chart.axis.typeRight) {
    base=this.rect.left();
    fntoff[0]=-fntoff[0];
    fntoff[1]=-fntoff[1];
    ctx.textAlign='start'; 
  }
  var tic=new chart.scale.Tic();
  var ii=0,fnt2off=intobj.cutfloat(parseFloat(fntoff[0]/2));
  ctx.beginPath();
  ctx.moveTo(base,this.rect.top());
  ctx.lineTo(base,this.rect.bottom());
  ctx.stroke();
  ctx.beginPath();
  var vertex=new floatobj.Vertex();
  for (ii=0;ii<this.scaling.tics.length;ii++) {
    loff=base;
    tic=this.scaling.tics[ii];
    vertex.setVal(1,tic.val);
    toff=this.sc2sz(vertex);
    ctx.font=fntstr[tic.sz];
    ctx.moveTo(loff,toff);
    loff-=(tic.sz==1 ? fntoff[0] : fnt2off);
    ctx.lineTo(loff,toff);
    loff-=fnt2off;
    ctx.fillText(tic.str,loff,toff);  
  }
  ctx.stroke();
  return aux.ansok;
};

chart.axis.Yaxis.prototype.drawSvg=function(ctx) {
  var gyax=chart.findElementById(this.toString());
  if (!gyax) {
    gyax=chart.svgCreateElement('g');
    if (!gyax)
      return aux.ansko;
    gyax.setAttribute('id',this.toString());
    chart.svgAddElement(ctx,gyax);
  }
  while (gyax.firstChild)
    gyax.removeChild(gyax.firstChild);
  var fntoff=[
    intobj.cutfloat(parseFloat(this.font.minor.pxsz)),
    intobj.cutfloat(parseFloat(this.font.major.pxsz))
  ];
  var base=this.rect.right(),loff=0,toff=0;
  var tcoff=[intobj.cutfloat(fntoff[0]/2),intobj.cutfloat(fntoff[1]/2)];
  if (this.pos==chart.axis.typeRight) {
    base=this.rect.left();
    fntoff[0]=-fntoff[0];
    fntoff[1]=-fntoff[1];
  }
  var strokestyle=('#'+intobj.bits2str(intobj.str2bits(this.linestyle.color.toString(),10),16));
  var svgtictxt=void(0);
  var vertex=new floatobj.Vertex(base,this.rect.top());
  var strpath=chart.svgPath2('M',vertex);
  vertex.setVal(1,this.rect.bottom());
  strpath+=chart.svgPath2('L',vertex);
  var tic=new chart.scale.Tic();
  var ii=0,fnt2off=intobj.cutfloat(parseFloat(fntoff[0]/2));
  var fntstr=[chart.fnt2str(this.font.minor),chart.fnt2str(this.font.major)];
  for (ii=0;ii<this.scaling.tics.length;ii++) {
    loff=base;
    vertex.setVal(0,loff);
    tic=this.scaling.tics[ii];
    vertex.setVal(1,tic.val);
    toff=this.sc2sz(vertex);
    vertex.setVal(1,toff);
    strpath+=chart.svgPath2('M',vertex);
    loff-=(tic.sz==1 ? fntoff[0] : fnt2off);
    vertex.setVal(0,loff);
    strpath+=chart.svgPath2('L',vertex);
    loff-=fnt2off;
    vertex.setVal(0,loff);
    toff+=(tcoff[tic.sz]-1);
    vertex.setVal(1,toff);
    svgtictxt=chart.svgCreateElement('text');
    svgtictxt.setAttribute('x',floatobj.sprintf(vertex.val(0),0,'d'));
    svgtictxt.setAttribute('y',floatobj.sprintf(vertex.val(1),0,'d'));
    svgtictxt.setAttribute('text-anchor','end');
    txtstyle=('fill:'+strokestyle+';font:');
    txtstyle+=fntstr[tic.sz];
    svgtictxt.setAttribute('style',txtstyle);
    svgtictxt.textContent=tic.str;
    chart.svgAddElement(gyax,svgtictxt);
  }
  var svgbase=chart.svgCreateElement('path');
  svgbase.setAttribute('d',strpath);
  svgbase.setAttribute('stroke',strokestyle);
  svgbase.setAttribute('stroke-width',this.linestyle.width.toString());
  svgbase.setAttribute('fill','none');
  chart.svgAddElement(gyax,svgbase);

//alert(document.body.innerHTML);

  return aux.ansok;
};

chart.axis.Yaxis.prototype.mendScaling=function(ctx) {
  var maxtics=intobj.cutfloat(this.rect.height()/parseFloat(this.font.major.pxsz));
  this.scaling.calcTics(maxtics); 
};



