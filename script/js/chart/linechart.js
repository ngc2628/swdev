
/* ########## */
chart.LineChart=function(typename,typenum) {
  if (typeof typename!='string')
    typename='linechart';
  chart.Chart.call(this,typename,typenum);
  this.rendercontext=void(0);
  this.elementrect=new chart.shapes.WinRect();
  this.rect=new chart.shapes.WinRect();
  this.font=chart.cpfnt(chart.chartfont);
  this.graphs=[];
  this.axes=[];
};

/* ********** */
chart.LineChart.prototype=Object.create(chart.Chart.prototype);

/* ********** */
chart.LineChart.prototype.constructor=chart.LineChart;

/* ********** */
chart.LineChart.prototype.setCtx=function(ctx,ww,hh) {
  var ans=mkuni.bad;
  if (ctx) {
    this.rendercontext=ctx;
    ans=mkuni.good;
  }
  this.elementrect.setWidth(mkuni.chknumber(ww,0));
  this.elementrect.setHeight(mkuni.chknumber(hh,0));
  this.rect=this.elementrect.copy();
  this.rect.deflate(96);
  return ans;
};

/* ********** */
chart.LineChart.prototype.getCtx=function() {
  return this.rendercontext;
};

/* ********** */
chart.LineChart.prototype.getRect=function() {
  return this.rect.copy();
};

/* ********** */
chart.LineChart.prototype.findAxis=function(axis) {
  if (axis instanceof aux.TypeId) {
    var idx=mkuni.bsearch(axis,this.axes,aux.cmptypeid);
    if (idx>=0)
      return this.axes[idx];
  }
  return void(0);
};

/* ********** */
chart.LineChart.prototype.addAxis=function(axis) {
  var ans=mkuni.bad;
  if (axis instanceof aux.TypeId) {
    var idx=mkuni.bsearch(axis,this.axes,aux.cmptypeid);
    if (idx<0) {
      this.axes.push(axis);
      this.axes.sort(aux.cmptypeid);
      ans=axis.toString();
    }
  }
  return ans;
};

/* ********** */
chart.LineChart.prototype.findGraph=function(graph) {
  if (graph instanceof aux.TypeId) {
    var idx=mkuni.bsearch(graph,this.graphs,aux.cmptypeid);
    if (idx>=0)
      return this.graphs[idx];
  }
  return void(0);
};

/* ********** */
chart.LineChart.prototype.addGraph=function(graph) {
  var ans=mkuni.bad;
  if (graph instanceof aux.TypeId) {
    var idx=mkuni.bsearch(graph,this.graphs,aux.cmptypeid);
    if (idx<0) {
      this.graphs.push(graph);
      this.graphs.sort(aux.cmptypeid);
      ans=graph.toString();
    }
  }
  return ans;
};

/* ********** */
chart.LineChart.prototype.setupAxes=function(ctx) {
  var ans=mkuni.bad;
  var majfnt=chart.cpfnt(this.font),minfnt=chart.cpfnt(this.font);
  minfnt.pxsz-=3;
  var rr=this.rect.copy();
  var ax=void(0);
  var ii=0;
  for (ii=0;ii<this.axes.length;ii++) {
    ax=this.axes[ii];
    if (ax.typename()=='yaxis')
      continue;
    ax.calcRange();
    ax.setFont('major',majfnt);
    ax.setFont('minor',minfnt);
    rr=ax.findRect(ctx,rr,5);
  }
  for (ii=0;ii<this.axes.length;ii++) {
    ax=this.axes[ii];
    if (ax.typename()=='xaxis')
      continue;
    ax.calcRange();
    ax.setFont('major',majfnt);
    ax.setFont('minor',minfnt);
    rr=ax.findRect(ctx,rr,5);
  }
  var graphrect=rr.copy();
  for (ii=0;ii<this.axes.length;ii++) {
    ax=this.axes[ii];
    if (ax.typename()=='yaxis')
      continue;
    rr=ax.getRect();
    rr.setLeft(graphrect.left());
    rr.setRight(graphrect.right());
    ax.setRect(rr);
  }
  for (ii=0;ii<this.axes.length;ii++) {
    ax=this.axes[ii];
    ax.mendScaling(ctx);
  }
  return ans;
};

/* ********** */
chart.LineChart.prototype.redraw=function() {
  var ans=mkuni.bad;
  var ctx=this.getCtx();
  this.setupAxes(ctx);
  var ax=void(0);
  var ii=0;
  for (ii=0;ii<this.axes.length;ii++) {
    ax=this.axes[ii];
    ax.draw(ctx);
  }
  var graph=void(0);
  for (ii=0;ii<this.graphs.length;ii++) {
    graph=this.graphs[ii];
    graph.draw(ctx);
  }
  return ans;
};

/* ********** */
chart.LineChart.prototype.toJson=function(indent) {
  var nextindent='  ',nextnextindent='    ',nextnextnextindent='      ';
  if (typeof indent=='string') {
    nextindent+=indent;
    nextnextindent+=indent;
    nextnextnextindent+=indent;
  }
  var xans='';
  var ans=chart.Chart.prototype.toJson.call(this,nextindent);
  ans+=aux.qnextprop(1);
  ans+=aux.qdecl('elementrect',nextindent);
  ans+=aux.qattr(this.elementrect.toJson(nextnextindent),1,nextindent);
  ans+=aux.qnextprop(1);
  ans+=aux.qdecl('rect',nextindent);
  ans+=aux.qattr(this.rect.toJson(nextnextindent),1,nextindent);
  ans+=aux.qnextprop(1);
  ans+=aux.qdecl('font',nextindent);
  ans+=aux.qword(chart.fnt2str(this.font));
  ans+=aux.qnextprop(1);
  ans+=aux.qdecl('axes',nextindent);
  var tmp='';
  var ii=0;
  for (ii=0;ii<this.axes.length;ii++) {
    if (ii>0)
      tmp+=aux.qnextprop();
    tmp+=aux.qattr(this.axes[ii].toJson(nextnextnextindent),1,nextnextindent);
  }
  ans+=aux.qarr(tmp,1,nextindent);
  ans+=aux.qnextprop(1);
  ans+=aux.qdecl('graphs',nextindent);
  tmp='';
  for (ii=0;ii<this.graphs.length;ii++) {
    if (ii>0)
      tmp+=aux.qnextprop();
    tmp+=aux.qattr(this.graphs[ii].toJson(nextnextnextindent),1,nextnextindent);
  }
  ans+=aux.qarr(tmp,1,nextindent);
  xans+=aux.qattr(ans,1,indent);
  xans+=aux.qany('\n\n');
  return xans;
};
/* ########## */



