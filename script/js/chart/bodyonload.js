
// 1208925819614629174706176 = 1024^8
// 1099511627776.*1099511627776.;
// 9223372036854775808
// 18446744073709551616
// 1716119 6311K-23

var dbg={};

/* ********** */
dbg.fdbg1=function() {
  var ans=('\ndbg1\n');
  var dnow=new Date();
  ans+=dnow.toString();
  ans+='\n';
  var datetime1=new datetime.Datetime(dnow.valueOf().toString());
  var datetime2=new datetime.Datetime(datetime1.toString(1));
  ans+=datetime1.toString(1);
  ans+='\n';
  ans+=datetime2.toString(1);
  ans+='\n';
  ans+=datetime2.toUtcmillicnt();
  ans+='\n';
  ans+=dnow.valueOf();
  ans+='\n';
  var fields={
    'month' : {'prefix' : '' , 'postfix' : '/'},
    'day' : {'prefix' : '' , 'postfix' : ' '},
    'hour' : {'prefix' : '' , 'postfix' : ':'},
    'minute' : {'prefix' : '' , 'postfix' : ''}
  };
  ans+=datetime1.toStructTmString(fields);

  //var filereq=new XMLHttpRequest();
  //filereq.open("GET","/js/trend11.txt",false);
  //filereq.send();
  //var strvertices=filereq.responseText;
  //alert(strvertices);
  //var jvertices=JSON.parse('{"trend11vertices" : ["{[now-01h,0.0057,1,1]}"]}');
  //var jvertices=JSON.parse('{\n"trend11vertices" :\n[\n"{[1,0.0058,1,1]}"\n]\n}');
  //var jvertices=JSON.parse(strvertices);
  return ans;
};

/* ********** */
dbg.fdbg2=function() {
  var ans=('\ndbg2\n');
  var vv1=new mkmat.Vertex(4,2,4);
  ans+=mkfloat.vectorlen(vv1).toString();
  ans+='\n';
  ans+=mkfloat.vectornorm(vv1).toString();
  ans+=('\n');
  return ans;
};

/* ********** */
dbg.fdbg3=function() {
  var ans=('\ndbg3\n');
  var dbl1=174.010248876739e3,dbl2=-100;
  ans+=(mkfloat.roundpos(dbl2,-2).toString());
  ans+=('\n');
  return ans;
};

/* ********** */
dbg.fdbg4=function() {
  var ans=('\ndbg4\n');
  var conv=mkfloat.ieee2bits(0.1);
  ans+=conv;
  ans+=(' ### '+mkfloat.bits2ieee(conv));
  ans+=('\n');
  return ans;
};

/* ********** */
dbg.fdbg=function(txtout) {
  var outstr='';
  outstr+=dbg.fdbg1();
  outstr+=dbg.fdbg2();
  outstr+=dbg.fdbg3();
  outstr+=dbg.fdbg4();
  txtout.value=outstr;

  //setTimeout(fdbg,1000);
};

var chartdbg={};

/* ********** */
chartdbg.linechartcreate=function(dbgchart) {
  //alert(dbgchart);
  var ii=0;
  var gridL=[11,2,39,22,15,20];
  var colorL=[3381589,12285781,3111111,12345678];
  var markerL=['square','diamond','triangle','circle'];
  var str='',strfile='/js/chart/trend1';
  var strid='';
  var typeid=new aux.TypeId();
  var xax=void(0),yax=void(0),graph=void(0),graphdata=void(0),scaling=void(0);
  for (ii=0;ii<3;ii++) {
    typeid.fromString(dbgchart.addGraph(new chart.graph.Graph('gr',gridL[ii])));
    graph=dbgchart.findGraph(typeid);
    // aaaaa
    graph.setData(new chart.dataset.DataSinCos());
    // aaaaa
    graphdata=graph.getData();
    // aaaaa
    graphdata.datatype[0].type='time';
    if (ii==0) {
      typeid.fromString(dbgchart.addAxis(new chart.axis.Xaxis()));
      xax=dbgchart.findAxis(typeid);
      typeid.fromString(dbgchart.addAxis(new chart.axis.Yaxis()));
      yax=dbgchart.findAxis(typeid);
      graphdata.datatype[0].scale='day';
      graphdata.genData('sin',36,50);
    }
    else if (ii==1) {
      /*typeid.fromString(dbgchart.addAx4is(new chart.axis.Xaxis()));
      xax=dbgchart.findAxis(typeid);
      typeid.fromString(dbgchart.addAxis(new chart.axis.Yaxis()));
      yax=dbgchart.findAxis(typeid);*/
      graphdata.datatype[0].scale='day';
      graphdata.genData('cos',54,11);
    }
    else if (ii==2) {
      /*typeid.fromString(dbgchart.addAxis(new chart.axis.Xaxis()));
      xax=dbgchart.findAxis(typeid);
      typeid.fromString(dbgchart.addAxis(new chart.axis.Yaxis()));
      yax=dbgchart.findAxis(typeid);*/
      graphdata.datatype[0].scale='day';
      graphdata.genData('sin',36,2,50);
    }
    // aaaaa
    // bbbbb
    //str=aux.reqFileRead(strfile+(ii+1).toString()+'.txt');
    //graphdata.fromJson(str);
    // bbbbb
    graphdata.findBounds();
    graph.setLinestyle(colorL[ii],'1',1);
    graph.setMarkstyle(colorL[ii+1],markerL[ii],16);

    xax.setStyle(colorL[ii]);
    // xax.setPos(chart.axis.typeTop);
    xax.setScaling(new chart.scale.TimeScaling());
    //xax.setScaling(new chart.scale.LinearScaling());
    xax.assignGraph(graph);

    yax.setStyle(colorL[ii]);
    // yax.setPos(chart.axis.typeRight);
    yax.setScaling(new chart.scale.LinearScaling());
    // scaling=yax.getScaling();
    // scaling.setRange(-11.1,137.9,'static','static');
    // scaling.setRange(-0.33,NaN,'static','auto');
    // yax.setScaling(scaling);
    yax.assignGraph(graph);
  }
};

/* ********** */
chartdbg.drawcanvas=function(canvas) {
  if (chart.lchartcanvas && canvas && canvas.getContext) {
    chart.lchartcanvas.setCtx(canvas.getContext('2d'),canvas.scrollWidth,canvas.scrollHeight);
    chart.lchartcanvas.redraw();
  }
};

/* ********** */
chartdbg.drawsvg=function(esvg) {
  if (chart.lchartsvg && esvg) {
    var ww=0,hh=0;
    if (esvg.getBoundingClientRect()) {
      ww=esvg.getBoundingClientRect().width;
      hh=esvg.getBoundingClientRect().height;
    }
    if (ww==0) {
      ww=esvg.offsetWidth;
      hh=esvg.offsetHeight;
    }
    chart.lchartsvg.setCtx(esvg,ww,hh);
    chart.lchartsvg.redraw();
  }
};

/* ********** */
function bodyonload() {
  chart.win=window;
  var fnt={'style':'','variant':'','weight':'','pxsz':'12','family':'monospace'};
  var txtout=[
    chart.findElementById("out11"),
    chart.findElementById("out12"),
    chart.findElementById("out13")
  ];
  txtout[0].style.font=chart.fnt2str(fnt);
  txtout[1].style.font=chart.fnt2str(fnt);
  txtout[2].style.font=chart.fnt2str(fnt);

  var svg11=chart.findElementById('svg11');
  var canvas11=chart.findElementById('canvas11');

  /*var txt11=chart.svgCreateElement('text');
  txt11.setAttribute('id','txt11');
  svg11.appendChild(txt11);
  txt11.setAttribute('x','50');
  txt11.setAttribute('y','50');
  var st='fill:#ff2288;';
  txt11.setAttribute('style',st);
  txt11.textContent='Hello Martin';
  st='fill:#ff2288;font:'+chart.fnt2str(chart.chartfont)+';';
  txt11.setAttribute('style',st);
  var poly13=chart.svgCreateElement('path');
  var cc13=new chart.shapes.Circle(void(0),new mkmat.Vertex(200,500),20);
  poly13.setAttribute('d',cc13.toSvgPath());
  poly13.setAttribute('stroke','#226644');
  poly13.setAttribute('stroke-width','1');
  poly13.setAttribute('fill','none');
  svg11.appendChild(poly13);
  var poly14=chart.svgCreateElement('path');
  var cc14=new chart.shapes.Square(void(0),new mkmat.Vertex(200,500),40);
  poly14.setAttribute('d',cc14.toSvgPath());
  poly14.setAttribute('stroke','#226644');
  poly14.setAttribute('stroke-width','1');
  poly14.setAttribute('fill','none');
  svg11.appendChild(poly14);
  var poly15=chart.svgCreateElement('path');
  var cc15=new chart.shapes.Triangle(void(0),new mkmat.Vertex(200,500),40);
  alert(cc15.toSvgPath());
  poly15.setAttribute('d',cc15.toSvgPath());
  poly15.setAttribute('stroke','#226644');
  poly15.setAttribute('stroke-width','1');
  poly15.setAttribute('fill','none');
  svg11.appendChild(poly15);
  var poly16=chart.svgCreateElement('path');
  var cc16=new chart.shapes.Diamond(void(0),new floatobcircle id="mycirc" cx="60" cy="60" r="22" onmousedown="mouseDown(evt)"j.Vertex(200,500),40);
  poly16.setAttribute('d',cc16.toSvgPath());
  poly16.setAttribute('stroke','#226644');
  poly16.setAttribute('stroke-width','1');
  poly16.setAttribute('fill','none');
  svg11.appendChild(poly16);*/

  /*var trv=new mkmat.Vertex(100,50);
  var tm=new mkmat.TransformMatrix(2);
  tm.translate(trv);
  tm.rotatez(30);
  alert(' m '+tm.toJson());
  var vertex=new mkmat.Vertex(2,1);
  vertex=tm.transform(vertex);
  alert(' v '+vertex.toJson());
  vertex.setVal(0,2);;ans+=attrL
  vertex.setVal(1,1);
  vertex=mkfloat.rotate2(vertex,30);
  alert(' w '+vertex.toJson());*/

  /*var tstrect=chart.svgCreateElement('rect');
  tstrect.setAttribute('x',200);
  tstrect.setAttribute('y',100);
  tstrect.setAttribute('width',300);
  tstrect.setAttribute('height',200);
  tstrect.setAttribute('stroke','#226644');
  tstrect.setAttribute('stroke-width','1');
  tstrect.setAttribute('fill','none');
  tstrect.setAttribute('pointer-events','all');
  svg11.appendChild(tstrect);
  tstrect.addEventListener('click',fmousedown,false);*/

  chart.lchartsvg=new chart.LineChart();
  chartdbg.linechartcreate(chart.lchartsvg);
  chartdbg.drawsvg(svg11);

  //chart.lchartcanvas=new chart.LineChart();
  //chartdbg.linechartcreate(chart.lchartcanvas);
  //chart.drawcanvas(canvas11);


};

/* ********** */
function click11() {
  chart.findElementById("out13").value=chart.lchartsvg.toJson();
};

/* ********** */
function click12() {
  var out12=chart.findElementById("out12");
  var strlchart=aux.reqFileRead('/download/linechart11.txt');
  var readlchart;
  try {
    readlchart=JSON.parse(strlchart);
  }
  catch(exc) {
    alert(exc.message);
  }
  if (readlchart.hasOwnProperty('graphs')) {
    var ii=0,jj=0;
    for (ii=0;ii<readlchart.graphs.length;ii++) {
      if (readlchart.graphs[ii].hasOwnProperty('data')) {

      }
    }
  }
};

/* ********** */
function out11_keypress(evt) {
  var kevt=(evt || window.event);
  var key=(kevt.key || kevt.which);
  var regexp=new RegExp('[cvx]');
  var ans=key.match(regexp);

  if (!kevt.getModifierState('Control') || !ans) {
    kevt.preventDefault();
  }
};

/* ********** */
function out12_keypress(evt) {
  var kevt=(evt || window.event);
  var key=(kevt.key || kevt.which);
  var regexp=new RegExp('[cvx]');
  var ans=key.match(regexp);

  if (!kevt.getModifierState('Control') || !ans) {
    kevt.preventDefault();
  }
};

/* ********** */
function out13_keypress(evt) {
  var kevt=(evt || window.event);
  var key=(kevt.key || kevt.which);
  var regexp=new RegExp('[cvx]');
  var ans=key.match(regexp);

  if (!kevt.getModifierState('Control') || !ans) {
    kevt.preventDefault();
  }
};

/* ********** */
function canvas11_mousedown(evt) {
  var canvas11=chart.findElementById('canvas11');
  var rr=new chart.shapes.WinRect();
  rr.setWidth(canvas11.scrollWidth);
  rr.setHeight(canvas11.scrollHeight);
  //alert(rr.toJson());
};
