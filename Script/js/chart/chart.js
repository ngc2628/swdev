
var chart={};

chart.svgns='http://www.w3.org/2000/svg';

chart.win=void(0);

chart.findElementById=function(eid) {
  if (chart.win && chart.win.constructor.name=='Window' && 
      typeof eid=='string' && eid.length>0)
    return chart.win.document.getElementById(eid);
  return void(0);
};

chart.svgCreateElement=function(etype) {
  if (chart.win && chart.win.constructor.name=='Window' && 
      typeof etype=='string' && etype.length>0)
    return chart.win.document.createElementNS(chart.svgns,etype);
  return void(0);
};

chart.svgAddElement=function(esvg,eappend) {
  if (esvg && esvg instanceof SVGElement && eappend && eappend instanceof SVGElement)
    return esvg.appendChild(eappend);
  return void(0);
};

chart.svgRemoveElement=function(esvg,eremove) {
  if (esvg && esvg instanceof SVGElement && eremove && eremove instanceof SVGElement)
    return esvg.removeChild(eremove);
  return void(0);
};

chart.svgPath2=function(type,vertex) {
  var ans='';
  if (typeof type!='string' || 
      (type.toUpperCase()=='M' && type.toUpperCase()=='L') ||
      (! vertex instanceof floatobj.Vertex)) 
    return ans;
  ans+=' ';
  ans+=type;
  ans+=' ';
  ans+=floatobj.sprintf(vertex.val(0),0,'d');
  ans+=' ';
  ans+=floatobj.sprintf(vertex.val(1),0,'d');
  ans+=' ';
  return ans;
};

chart.chartfont={'style':'','variant':'','weight':'','pxsz':'14','family':'Helvetica'};

chart.chartfontstr='14px Helvetica';

chart.cpfnt=function(fnt) {
  var ans={
    'style':fnt.style,'variant':fnt.variant,'weight':fnt.weight,
    'pxsz':fnt.pxsz,'family':fnt.family
  };
  return ans;
};

chart.fnt2str=function(fnt) {
  var ans=chart.chartfontstr;
  if (typeof fnt!='undefined' && fnt.hasOwnProperty('pxsz')) 
    ans=fnt.style+' '+fnt.variant+' '+fnt.weight+' '+fnt.pxsz+'px '+fnt.family;
  return ans;
};

chart.wtext=function(text,fntstr,ctx) {
  var ans=1;
  if (!ctx)
    return ans;
  if (ctx instanceof CanvasRenderingContext2D) {
    var ctxfnt=ctx.font;
    ctx.font=fntstr;
    ans=ctx.measureText(text).width;
    ctx.font=ctxfnt;
  }
  else if ((ctx instanceof SVGElement) && (ctx.nodeName=='text') && ctx.parentNode) { 
    ctx.setAttribute('style','font:'+fntstr+';');
    ctx.textContent=text;
    ans=ctx.getBBox().width;
  }
  return ans;
};

chart.Chart=function(typename,typenum) {
  if (typeof typename!='string')
    typename='chart';
  aux.TypeId.call(this,typename,typenum);
};

chart.Chart.prototype=Object.create(aux.TypeId.prototype);

chart.Chart.prototype.constructor=chart.Chart;

chart.Chart.prototype.toJson=function(indent) {
  var ans=aux.TypeId.prototype.toJson.call(this,indent);
  return ans;
};