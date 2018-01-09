
var chart={};

chart.lchartcanvas=void(0);
chart.lchartsvg=void(0);

chart.svgns='http://www.w3.org/2000/svg';

chart.win=void(0);

/* ********** */
chart.svgDmpElementAttributes=function(esvg) {
  var ans='';
  if (!esvg)
    return ans;
  ans+=esvg.toString();
  if (!(esvg instanceof SVGElement))
    return ans;
  ans+='\n';
  ans+=esvg.tagName;
  ans+='\n';
  var attrL=esvg.attributes;
  var ii=0;
  for (ii=attrL.length-1;ii>-1;ii--) {
    ans+='  [';
    ans+=attrL[ii].name;
    ans+='] [';
    ans+=attrL[ii].value;
    ans+=']\n';
  }
  return ans;
};

/* ********** */
chart.findElementById=function(eid) {
  if (chart.win &&
      (typeof eid=='string') && eid.length>0)
    return chart.win.document.getElementById(eid);
  return void(0);
};

/* ********** */
chart.svgCreateElement=function(etype) {
  if (chart.win &&
      (typeof etype=='string') && etype.length>0) {
    var elem=chart.win.document.createElementNS(chart.svgns,etype);
    return elem;
  }
  return void(0);
};

chart.svgAddElement=function(esvg,eappend) {
  if (esvg && esvg instanceof SVGElement && eappend && eappend instanceof SVGElement) {
    return esvg.appendChild(eappend);
  }
  return void(0);
};

/* ********** */
chart.svgRemoveElement=function(esvg,eremove) {
  if (esvg && esvg instanceof SVGElement && eremove && eremove instanceof SVGElement)
    return esvg.removeChild(eremove);
  return void(0);
};

/* ********** */
chart.svgPath2=function(type,vertex) {
  var ans='';
  if ((typeof type!='string') ||
      (type.toUpperCase()!='M' && type.toUpperCase()!='L') ||
      (! vertex instanceof mkmat.Vertex))
    return ans;
  ans+=' ';
  ans+=type;
  ans+=' ';
  ans+=mkfloat.sprintf(vertex.val(0),0,'d');
  ans+=' ';
  ans+=mkfloat.sprintf(vertex.val(1),0,'d');
  ans+=' ';
  return ans;
};

chart.chartfont={'style':'','variant':'','weight':'','pxsz':'14','family':'Helvetica'};

chart.chartfontstr='14px Helvetica';

/* ********** */
chart.cpfnt=function(fnt) {
  var ans={
    'style':fnt.style,'variant':fnt.variant,'weight':fnt.weight,
    'pxsz':fnt.pxsz,'family':fnt.family
  };
  return ans;
};

/* ********** */
chart.fnt2str=function(fnt) {
  var ans=chart.chartfontstr;
  if ((typeof fnt!='undefined') && fnt.hasOwnProperty('pxsz'))
    ans=fnt.style+' '+fnt.variant+' '+fnt.weight+' '+fnt.pxsz+'px '+fnt.family;
  return ans;
};

/* ********** */
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

/* ********** */
chart.stipple=function(style,defstyle) {
  var ans=defstyle;
  if (typeof style!='string') {
    if (typeof style=='number') {
      if (parseInt(style)<=0)
        ans='0';
      else
        ans='1';
    }
    return ans;
  }
  var splitL=style.split(',');
  var ii=0,ll=splitL.length,npix=0;
  var stippleL=[];
  for (ii=0;ii<ll;ii++) {
    npix=parseInt(splitL[ii]);
    if (npix>0)
      stippleL.push(npix);
  }
  ll=stippleL.length;
  if (ll==0)
    ans='0';
  else if (ll==1)
    ans='1';
  else {
    ans='';
    for (ii=0;ii<ll;ii++) {
      if (ii>0)
        ans+=',';
      ans+=stippleL[ii].toString();
    }
  }
  return ans;
};

/* ********** */
chart.mouseclick=function(evt) {
  if (chart.lchartsvg &&
      evt.target && evt.target.getAttribute('pos') && evt.target.parentNode) {
    var grid=new aux.TypeId();
    grid.fromString(evt.target.parentNode.id);
    var gr=chart.lchartsvg.findGraph(grid);
    if (gr) {
      var pos=evt.target.getAttribute('pos');
      var jpos;
      try { jpos=JSON.parse(pos); }
      catch(exc) { }
      if (jpos instanceof Array) {
        var vin=new mkmat.Vertex(
          mkuni.chknumber(parseFloat(jpos[0]),NaN),
          mkuni.chknumber(parseFloat(jpos[1]),NaN)
        );
        var vout=new mkmat.Vertex();
        gr.sz2sc(vin,vout);
        var strx=mkfloat.sprintf(vout.val(0),3,'f'),
            stry=mkfloat.sprintf(vout.val(1),3,'f');
        if (gr.getData().datatype[0].type=='time') {
          var tt=new datetime.Datetime();
          tt.fromUtcmillicnt(vout.val(0));
          strx=tt.toIso8601();
        }
        alert(grid.toJson(0,0)+'\n'+strx+'\n'+stry);
      }
    }
  }
};

/* ########## */
chart.Chart=function(typename,typenum) {
  if (typeof typename!='string')
    typename='chart';
  aux.TypeId.call(this,typename,typenum);
};

/* ********** */
chart.Chart.prototype=Object.create(aux.TypeId.prototype);

/* ********** */
chart.Chart.prototype.constructor=chart.Chart;

/* ********** */
chart.Chart.prototype.toJson=function(indent) {
  var ans=aux.TypeId.prototype.toJson.call(this,indent);
  return ans;
};
/* ########## */

