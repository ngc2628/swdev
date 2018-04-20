
/* ########## */
chart.scale.Scale=function(typename,typenum) {
  if (typeof typename!='string')
    typename='scale';
  aux.TypeId.call(this,typename,typenum);
  this.rect=new chart.shapes.WinRect();
  this.font={'major':chart.cpfnt(chart.chartfont),'minor':chart.cpfnt(chart.chartfont)};
  this.font.minor.pxsz=parseInt(this.font.major.pxsz)-3;
};

/* ********** */
chart.scale.Scale.prototype=Object.create(aux.TypeId.prototype);

/* ********** */
chart.scale.Scale.prototype.constructor=chart.scale.Scale;

/* ********** */
chart.scale.Scale.prototype.setRect=function(rect) {
  if (rect instanceof chart.shapes.WinRect)
    this.rect=rect;
};

/* ********** */
chart.scale.Scale.prototype.getRect=function() {
  return this.rect.copy();
};

/* ********** */
chart.scale.Scale.prototype.setFont=function(type,fnt) {
  if ((typeof type!='string') || (type!='major' && type!='minor'))
    type='minor';
  this.font[type]=chart.cpfnt(fnt);
};

/* ********** */
chart.scale.Scale.prototype.getFont=function(type) {
  if ((typeof type!='string') || (type!='major' && type!='minor'))
    type='minor';
  var ans=chart.cpfnt(this.font[type]);
  return ans;
};

/* ********** */
chart.scale.Scale.prototype.sc2sz=function(vertex) {
  return NaN;
};

/* ********** */
chart.scale.Scale.prototype.sz2sc=function(vertex) {
  return NaN;
};

/* ********** */
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
/* ########## */

chart.graph={};

/* ########## */
chart.graph.Graph=function(typename,typenum) {
  if (typeof typename!='string')
    typename='graph';
  aux.TypeId.call(this,typename,typenum);
  this.linestyle={'color':0,'style':'1','width':1};
  this.markstyle={'color':0,'style':'none','width':1};
  this.rect=new chart.shapes.WinRect();
  this.font=chart.cpfnt(chart.chartfont);
  this.axes={};
  this.data=new chart.dataset.Data();
};

/* ********** */
chart.graph.Graph.prototype=Object.create(aux.TypeId.prototype);

/* ********** */
chart.graph.Graph.prototype.constructor=chart.graph.Graph;

/* ********** */
chart.graph.Graph.prototype.setLinestyle=function(cc,ss,ww) {
  cc=mkuni.chknumber(cc,this.linestyle.color);
  if (mkint.chknumberrange(cc,0,16777215)==mkuni.bad)
    cc=this.linestyle.color;
  this.linestyle.color=cc;
  this.linestyle.style=chart.stipple(ss,this.linestyle.style);
  ww=mkuni.chknumber(ww,this.linestyle.width);
  if (mkint.chknumberrange(ww,0,16)==mkuni.bad)
    ww=this.linestyle.width;
  this.linestyle.width=ww;
};

/* ********** */
chart.graph.Graph.prototype.getLinestyle=function() {
  var ans={
    'color':this.linestyle.color,'style':this.linestyle.style,'width':this.linestyle.width
  };
  return ans;
};

/* ********** */
chart.graph.Graph.prototype.setMarkstyle=function(cc,ss,ww) {
  cc=mkuni.chknumber(cc,this.markstyle.color);
  if (mkint.chknumberrange(cc,0,16777215)==mkuni.bad)
    cc=this.markstyle.color;
  this.markstyle.color=cc;
  if (typeof ss!='string')
    ss='none';
  ss=ss.toLowerCase();
  if (ss!='square' && ss!='diamond' && ss!='triangle' && ss!='circle')
    ss='none';
  this.markstyle.style=ss;
  ww=mkuni.chknumber(ww,this.markstyle.width);
  if (mkint.chknumberrange(ww,0,100)==mkuni.bad)
    ww=this.markstyle.width;
  this.markstyle.width=ww;
};

/* ********** */
chart.graph.Graph.prototype.getMarkstyle=function() {
  var ans={
    'color':this.markstyle.color,'style':this.markstyle.style,'width':this.markstyle.width
  };
  return ans;
};

/* ********** */
chart.graph.Graph.prototype.getRect=function() {
  return this.rect.copy();
};

/* ********** */
chart.graph.Graph.prototype.setData=function(graphdata) {
  var ans=mkuni.bad;
  if (graphdata instanceof chart.dataset.Data) {
    this.data=graphdata;
    ans=mkuni.good;
  }
  return ans;
};

/* ********** */
chart.graph.Graph.prototype.getData=function() {
  if (this.data instanceof chart.dataset.Data)
    return this.data;
  return void(0);
};

/* ********** */
chart.graph.Graph.prototype.setScale=function(scale) {
  var ans=mkuni.bad;
  if (scale instanceof chart.scale.Scale) {
    this.axes[scale.typename()]=scale;
    ans=mkuni.good;
  }
  return ans;
};

/* ********** */
chart.graph.Graph.prototype.unsetScale=function(scale) {
  var ans=mkuni.bad;
  if (scale instanceof chart.scale.Scale) {
    var axkey=scale.typename();
    if (axkey in this.axes) {
      delete this.axes.axkey;
      ans=mkuni.good;
    }
  }
  return ans;
};

/* ********** */
chart.graph.Graph.prototype.getScale=function(strtypename) {
  if (strtypename in this.axes)
    return this.axes[strtypename];
  return void(0);
};

/* ********** */
chart.graph.Graph.prototype.sc2sz=function(vin,vout,xax,yax) {
  if ((vin instanceof mkmat.Vertex) && (vout instanceof mkmat.Vertex)) {
    if (! (xax instanceof chart.scale.Scale))
      xax=this.axes['xaxis'];
    if (! (yax instanceof chart.scale.Scale))
      yax=this.axes['yaxis'];
    if ((xax instanceof chart.scale.Scale) && (yax instanceof chart.scale.Scale)) {
      vout.setVal(0,xax.sc2sz(vin));
      vout.setVal(1,yax.sc2sz(vin));
      return mkuni.good;
    }
  }
  if (vout instanceof mkmat.Vertex)
    vout.reset();
  return mkuni.bad;
};

/* ********** */
chart.graph.Graph.prototype.sz2sc=function(vin,vout,xax,yax) {
  if ((vin instanceof mkmat.Vertex) && (vout instanceof mkmat.Vertex)) {
    if (! (xax instanceof chart.scale.Scale))
      xax=this.axes['xaxis'];
    if (! (yax instanceof chart.scale.Scale))
      yax=this.axes['yaxis'];
    if ((xax instanceof chart.scale.Scale) && (yax instanceof chart.scale.Scale)) {
      vout.setVal(0,xax.sz2sc(vin));
      vout.setVal(1,yax.sz2sc(vin));
      return mkuni.good;
    }
  }
  if (vout instanceof mkmat.Vertex)
    vout.reset();
  return mkuni.bad;
};

/* ********** */
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
  tmp+=aux.qword(this.linestyle.style);
  tmp+=aux.qnextprop();
  tmp+=aux.qdecl('width');
  tmp+=aux.qword(this.linestyle.width.toString());
  ans+=aux.qattr(tmp);
  ans+=aux.qnextprop(1);
  ans+=aux.qdecl('markstyle',indent);
  tmp=aux.qdecl('color');
  tmp+=aux.qword(this.markstyle.color.toString());
  tmp+=aux.qnextprop();
  tmp+=aux.qdecl('style');
  tmp+=aux.qword(this.markstyle.style);
  tmp+=aux.qnextprop();
  tmp+=aux.qdecl('width');
  tmp+=aux.qword(this.markstyle.width.toString());
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

/* ********** */
chart.graph.Graph.prototype.drawCanvas2d=function(ctx) {
  var ii=0,idx=-1,xplot=0,yplot=0;
  var vertex=new mkmat.Vertex();
  ctx.strokeStyle=('#'+mkint.bits2str(mkint.str2bits(this.linestyle.color.toString(),10),16));
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
  return mkuni.good;
};

/* ********** */
chart.graph.Graph.prototype.drawSvg=function(ctx) {
  var ggr=chart.findElementById(this.toString());
  if (!ggr) {
    ggr=chart.svgCreateElement('g');
    if (!ggr)
      return mkuni.bad;
    ggr.setAttribute('id',this.toString());
    //ggr.setAttribute('pointer-events','all');
    //ggr.addEventListener('click',chart.mouseclick);
    chart.svgAddElement(ctx,ggr);
  }
  while (ggr.firstChild)
    ggr.removeChild(ggr.firstChild);
  var ii=0,idx=-1;
  var vertex=new mkmat.Vertex(),vxy=new mkmat.Vertex();
  var strpath='';
  var xax=this.axes['xaxis'],yax=this.axes['yaxis'];
  var markerL=[];
  var marker=void(0);
  for (ii=0;ii<this.data.vertices.length;ii++) {
    vertex=this.data.vertices[ii];
    this.sc2sz(vertex,vxy,xax,yax);
    if (isNaN(vxy.val(0)) || isNaN(vxy.val(1))) {
      idx=-1;
      continue;
    }
    if (idx<0)
      strpath+=chart.svgPath2('M',vxy);
    else {
      if (this.markstyle.style!='none' && this.markstyle.width>0) {
        var rr=this.markstyle.width/2;
        if (this.markstyle.style=='square'|| this.markstyle.style=='diamond')
          marker=new chart.shapes.Rect(this.markstyle.style,rr,rr);
        else if (this.markstyle.style=='triangle')
          marker=new chart.shapes.Triangle(rr);
        else if (this.markstyle.style=='circle')
          marker=new chart.shapes.Circle(rr);
        marker.move(vxy);
        markerL.push(marker);
      }
      strpath+=chart.svgPath2('L',vxy);
    }
    idx=ii;
  }
  var svgplot=chart.svgCreateElement('path');
  svgplot.setAttribute('d',strpath);
  var strokecolor=('#'+mkint.bits2str(mkint.str2bits(this.linestyle.color.toString(),10),16));
  svgplot.setAttribute('stroke',strokecolor);
  if (this.linestyle.style=='0')
    svgplot.setAttribute('stroke-width','0');
  else {
    svgplot.setAttribute('stroke-width',this.linestyle.width.toString());
    if (this.linestyle.style!='1') {
      svgplot.setAttribute('stroke-dasharray',this.linestyle.style);
    }
  }
  svgplot.setAttribute('fill','none');
  //svgplot.setAttribute('pointer-events','all');
  svgplot.addEventListener('click',chart.mouseclick);
  chart.svgAddElement(ggr,svgplot);
//alert(chart.svgDmpElementAttributes(svgplot));
  var svgmarker=void(0);
  for (ii=0;ii<markerL.length;ii++) {
    svgmarker=chart.svgCreateElement('path');
    strpath=markerL[ii].toSvgPath(vertex);
    svgmarker.setAttribute('d',strpath);
    svgmarker.setAttribute('pos',vertex.toJson(0,2,0));
    strokecolor=('#'+mkint.bits2str(mkint.str2bits(this.markstyle.color.toString(),10),16));
    svgmarker.setAttribute('stroke',strokecolor);
    svgmarker.setAttribute('stroke-width','1');
    svgmarker.setAttribute('fill','none');
    svgmarker.setAttribute('pointer-events','all');
    svgmarker.addEventListener('mouseup',chart.mouseclick);
    chart.svgAddElement(ggr,svgmarker);
  }
  return mkuni.good;
};

/* ********** */
chart.graph.Graph.prototype.draw=function(ctx) {
  var ans=mkuni.bad;
  if (ctx instanceof CanvasRenderingContext2D) {
    ans=this.drawCanvas2d(ctx);
  }
  else if (ctx instanceof SVGElement) {
    ans=this.drawSvg(ctx);
  }
  return ans;
};
/* ########## */
