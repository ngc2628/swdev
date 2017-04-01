
chart.scale.Scale=function(typename,typenum) {
  if (typeof typename!='string')
    typename='scale';
  aux.TypeId.call(this,typename,typenum);
  this.rect=new chart.shapes.Rect();
  this.font={'major':chart.cpfnt(chart.chartfont),'minor':chart.cpfnt(chart.chartfont)};
  this.font.minor.pxsz=parseInt(this.font.major.pxsz)-3;
};

chart.scale.Scale.prototype=Object.create(aux.TypeId.prototype);

chart.scale.Scale.prototype.constructor=chart.scale.Scale;

chart.scale.Scale.prototype.setRect=function(rect) {
  if (rect instanceof chart.shapes.Rect)
    this.rect=rect;
};

chart.scale.Scale.prototype.getRect=function() {
  return this.rect.copy();
};

chart.scale.Scale.prototype.setFont=function(type,fnt) {
  if (typeof type!='string' || (type!='major' && type!='minor')) 
    type='minor';
  this.font[type]=chart.cpfnt(fnt);
};

chart.scale.Scale.prototype.getFont=function(type) {
  if (typeof type!='string' || (type!='major' && type!='minor')) 
    type='minor';
  var ans=chart.cpfnt(this.font[type]);
  return ans;
};

chart.scale.Scale.prototype.sc2sz=function(vertex) {
  return NaN;
};

chart.scale.Scale.prototype.sz2sc=function(vertex) {
  return NaN;
};

chart.scale.Scale.prototype.toJson=function(indent) {
  var nextindent='  ',ans=aux.TypeId.prototype.toJson.call(this,indent);
  if (typeof indent=='string')
    nextindent+=indent;
  ans+=aux.qnextprop(1);
  ans+=aux.qdecl('rect',indent);
  ans+=aux.qattr(this.rect.toJson(nextindent),1,indent);
  ans+=aux.qnextprop(1);
  ans+=aux.qdecl('font',indent);
  var tmp=nextindent;
  tmp+=aux.qdecl('minor');
  tmp+=aux.qword(chart.fnt2str(this.font['minor']));
  tmp+=aux.qnextprop(1);
  tmp+=nextindent;
  tmp+=aux.qdecl('major');
  tmp+=aux.qword(chart.fnt2str(this.font['major']));
  ans+=aux.qattr(tmp,1,indent);
  return ans;
};

chart.graph={};

chart.graph.Graph=function(typename,typenum) {
  if (typeof typename!='string')
    typename='graph';
  aux.TypeId.call(this,typename,typenum);
  this.linestyle={'color':0,'style':1,'width':1};
  this.markstyle={'color':0,'style':1,'width':1};
  this.rect=new chart.shapes.Rect();
  this.font=chart.cpfnt(chart.chartfont);
  this.axes={};
  this.data=new chart.dataset.Data();
};

chart.graph.Graph.prototype=Object.create(aux.TypeId.prototype);

chart.graph.Graph.prototype.constructor=chart.graph.Graph;

chart.graph.Graph.prototype.setLinestyle=function(cc,ss,ww) {
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

chart.graph.Graph.prototype.getLinestyle=function() {
  var ans={
    'color':this.linestyle.color,'style':this.linestyle.style,'width':this.linestyle.width
  };
  return ans;
};

chart.graph.Graph.prototype.getRect=function() {
  return this.rect.copy();
};

chart.graph.Graph.prototype.setData=function(graphdata) {
  var ans=aux.ansko;
  if (graphdata instanceof chart.dataset.Data) {
    this.data=graphdata;
    ans=aux.ansok;
  }
  return ans;
};

chart.graph.Graph.prototype.getData=function() {
  if (this.data instanceof chart.dataset.Data)
    return this.data;
  return void(0);
};

chart.graph.Graph.prototype.setScale=function(scale) {
  var ans=aux.ansko;
  if (scale instanceof chart.scale.Scale) {
    this.axes[scale.typename()]=scale;
    ans=aux.ansok;
  }
  return ans;
};

chart.graph.Graph.prototype.unsetScale=function(scale) {
  var ans=aux.ansko;
  if (scale instanceof chart.scale.Scale) {
    var axkey=scale.typename();
    if (axkey in this.axes) {
      delete this.axes.axkey;
      ans=aux.ansok;
    }
  }
  return ans;
};

chart.graph.Graph.prototype.getScale=function(strtypename) {
  if (strtypename in this.axes)
    return this.axes[strtypename];
  return void(0);
};

chart.graph.Graph.prototype.toJson=function(indent) {
  var nextindent='  ',ans=aux.TypeId.prototype.toJson.call(this,indent);
  if (typeof indent=='string')
    nextindent+=indent;
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
  ans+=aux.qdecl('markstyle',indent);
  tmp=aux.qdecl('color');
  tmp+=aux.qword(this.linestyle.color.toString());
  tmp+=aux.qnextprop();
  tmp+=aux.qdecl('style');
  tmp+=aux.qword(this.linestyle.style.toString());
  tmp+=aux.qnextprop();
  tmp+=aux.qdecl('width');
  tmp+=aux.qword(this.linestyle.width.toString());
  ans+=aux.qattr(tmp);
  ans+=aux.qnextprop(1);
  ans+=aux.qdecl('rect',indent);
  ans+=aux.qattr(this.rect.toJson(nextindent),1,indent);
  ans+=aux.qnextprop(1);
  ans+=aux.qdecl('font',indent);
  ans+=aux.qword(chart.fnt2str(this.font));
  ans+=aux.qnextprop(1);
  ans+=aux.qdecl('axes',indent);
  tmp='';
  var axkeys=Object.keys(this.axes);
  var ii=0;
  for (ii=0;ii<axkeys.length;ii++) {
    if (ii>0)
      tmp+=aux.qnextprop(1);
    tmp+=aux.qdecl(axkeys[ii],nextindent);
    tmp+=aux.qword(aux.TypeId.prototype.toString.call(this.axes[axkeys[ii]]));
  }
  ans+=aux.qattr(tmp,1,indent);
  ans+=aux.qnextprop(1);
  ans+=aux.qdecl('data',indent);
  ans+=aux.qattr(this.data.toJson(nextindent),1,indent);
  return ans;
};

chart.graph.Graph.prototype.drawCanvas2d=function(ctx) {
  var ii=0,idx=-1,xplot=0,yplot=0;
  var vertex=new floatobj.Vertex();
  ctx.strokeStyle=('#'+intobj.bits2str(intobj.str2bits(this.linestyle.color.toString(),10),16));
  ctx.beginPath();
  for (ii=0;ii<this.data.vertices.length;ii++) {
    vertex=this.data.vertices[ii];
    xplot=this.axes['xaxis'].sc2sz(vertex);
    yplot=this.axes['yaxis'].sc2sz(vertex);
    if (isNaN(xplot) || isNaN(yplot)) {
      idx=-1;
      continue;
    }
    if (idx<0)
      ctx.moveTo(xplot,yplot);
    else
      ctx.lineTo(xplot,yplot);
    idx=ii;
  }
  ctx.stroke();
  return aux.ansok;
};

chart.graph.Graph.prototype.drawSvg=function(ctx) {
  var ggr=chart.findElementById(this.toString());
  if (!ggr) {
    ggr=chart.svgCreateElement('g');
    if (!ggr)
      return aux.ansko;
    ggr.setAttribute('id',this.toString());
    chart.svgAddElement(ctx,ggr);
  }
  while (ggr.firstChild)
    ggr.removeChild(ggr.firstChild);
  var ii=0,idx=-1;
  var vertex=new floatobj.Vertex(),vxy=new floatobj.Vertex();
  var strpath='';
  var xax=this.axes['xaxis'],yax=this.axes['yaxis'];

var dt=new datetime.Datetime();
var cnt=dt.toUtcmillicnt();

  for (ii=0;ii<this.data.vertices.length;ii++) {
    vertex=this.data.vertices[ii];
    vxy.setVal(0,xax.sc2sz(vertex));
    vxy.setVal(1,yax.sc2sz(vertex));
    if (isNaN(vxy.val(0)) || isNaN(vxy.val(1))) {
      idx=-1;
      continue;
    }
    if (idx<0)
      strpath+=chart.svgPath2('M',vxy);
    else
      strpath+=chart.svgPath2('L',vxy);
    idx=ii;
  }

dt=new datetime.Datetime();
var cnt2=dt.toUtcmillicnt();
//alert(cnt2-cnt);

  var svgplot=chart.svgCreateElement('path');
  svgplot.setAttribute('d',strpath);
  var strokestyle=('#'+intobj.bits2str(intobj.str2bits(this.linestyle.color.toString(),10),16));
  svgplot.setAttribute('stroke',strokestyle);
  svgplot.setAttribute('stroke-width',this.linestyle.width.toString());
  svgplot.setAttribute('fill','none');
  chart.svgAddElement(ggr,svgplot);
  return aux.ansok;
};

chart.graph.Graph.prototype.draw=function(ctx) {
  var ans=aux.ansko;
  if (ctx instanceof CanvasRenderingContext2D) {
    ans=this.drawCanvas2d(ctx);
  }
  else if (ctx instanceof SVGElement) {
    ans=this.drawSvg(ctx);
  }
  return ans;
};