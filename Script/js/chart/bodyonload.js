
// 1208925819614629174706176 = 1024^8
// 1099511627776.*1099511627776.;
// 9223372036854775808
// 18446744073709551616
// 1716119 6311K-23

var dbg={};

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

dbg.fdbg2=function() {
  var ans=('\ndbg2\n');
  var vv1=new floatobj.Vertex(4,2,4);
  ans+=floatobj.vectorlen(vv1).toString();
  ans+='\n';
  ans+=floatobj.vectornorm(vv1).toString();
  ans+=('\n');
  return ans;
};

dbg.fdbg3=function() {
  var ans=('\ndbg3\n');
  var dbl1=174.010248876739e3,dbl2=-100;
  ans+=(floatobj.roundpos(dbl2,-2).toString());
  ans+=('\n');
  return ans;
};

dbg.fdbg4=function() {
  var ans=('\ndbg4\n');
  var conv=floatobj.ieee2bits(0.1);
  ans+=conv;
  ans+=(' ### '+floatobj.bits2ieee(conv));
  ans+=('\n');
  return ans;
};

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

chartdbg.lchartcanvas=new chart.LineChart();
chartdbg.lchartsvg=new chart.LineChart();

chartdbg.linechartcreate=function(dbgchart) {
  //alert(dbgchart);
  var ii=0;
  var gridL=[11,2,39,22,15,20];
  var colorL=[3381589,12285781,3111111];
  var str='',strfile='/js/chart/trend1';
  var strid='';
  var typeid=new aux.TypeId();
  var xax=void(0),yax=void(0),graph=void(0),graphdata=void(0),scaling=void(0);
  for (ii=0;ii<3;ii++) {
    if (ii==0) {
      typeid.fromString(dbgchart.addAxis(new chart.axis.Xaxis()));
      xax=dbgchart.findAxis(typeid);
    
      typeid.fromString(dbgchart.addAxis(new chart.axis.Yaxis()));
      yax=dbgchart.findAxis(typeid);
    }
    typeid.fromString(dbgchart.addGraph(new chart.graph.Graph('gr',gridL[ii])));
    graph=dbgchart.findGraph(typeid);
    xax.setStyle(colorL[ii]);
    xax.assignGraph(graph);
    // xax.setPos(chart.axis.typeTop);
    yax.setStyle(colorL[ii]);
    yax.assignGraph(graph);
    // yax.setPos(chart.axis.typeRight);
    graph.setLinestyle(colorL[ii]);
    // aaaaa
    graph.setData(new chart.dataset.DataSinCos());
    // aaaaa
    graphdata=graph.getData();
    // aaaaa
    graphdata.datatype[0].type='time';
    graphdata.datatype[0].scale='day';
    if (ii==0)
      graphdata.genData('sin',36,50);
    else if (ii==1)
      graphdata.genData('cos',54,11);
    else if (ii==2)
      graphdata.genData('sin',36,2,50);
    // aaaaa
    // bbbbb
    //str=aux.reqFileRead(strfile+(ii+1).toString()+'.txt');
    //graphdata.fromJson(str);
    // bbbbb
    graphdata.findBounds();
    xax.setScaling(new chart.scale.TimeScaling());
    //xax.setScaling(new chart.scale.LinearScaling());
    yax.setScaling(new chart.scale.LinearScaling());
    // scaling=yax.getScaling();
    // scaling.setRange(-11.1,137.9,'static','static');
    // scaling.setRange(-0.33,NaN,'static','auto');
    // yax.setScaling(scaling);
  }

};

chartdbg.drawcanvas=function(canvas) {
  if (chartdbg.lchartcanvas && canvas && canvas.getContext) {
    chartdbg.lchartcanvas.setCtx(canvas.getContext('2d'),canvas.scrollWidth,canvas.scrollHeight);
    chartdbg.lchartcanvas.redraw();
  }
};

chartdbg.drawsvg=function(esvg) {
  if (chartdbg.lchartsvg && esvg) {
    chartdbg.lchartsvg.setCtx(esvg,esvg.getBoundingClientRect().width,esvg.getBoundingClientRect().height);
    chartdbg.lchartsvg.redraw();
  }
};

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
  var canvas11=chart.findElementById('canvas11');
  var svg11=chart.findElementById('svg11');
  
  /*var txt11=chart.svgCreateElement('text');
  txt11.setAttribute('id','txt11');
  svg11.appendChild(txt11);
  //alert(chart.wtext('Hello Martin',chart.fnt2str(chart.chartfont),txt11));
  txt11.setAttribute('x','50');
  txt11.setAttribute('y','50');
  var st='fill:#ff2288;';
  txt11.setAttribute('style',st);
  txt11.textContent='Hello Martin';
  st='fill:#ff2288;font:'+chart.fnt2str(chart.chartfont)+';';
  txt11.setAttribute('style',st);
  var poly11=chart.svgCreateElement('polyline');
  poly11.setAttribute('points','50,50 100,100 200,50');
  poly11.setAttribute('stroke','#11ee33');
  poly11.setAttribute('stroke-width','4');
  poly11.setAttribute('fill','#ccee33');
  svg11.appendChild(poly11);*/

//getPropertyValue('font-family'));

  chartdbg.linechartcreate(chartdbg.lchartcanvas);
  chartdbg.drawcanvas(canvas11);

  chartdbg.linechartcreate(chartdbg.lchartsvg);
  chartdbg.drawsvg(svg11);

  var ans=txtout[2].value;
  ans+=chartdbg.lchartsvg.toJson();
  txtout[2].value=ans;

};

function click11() {
  var snap=chartdbg.lchartcanvas.toJson();
  var ans=aux.reqFileSave(snap,'linechart11.txt','/cgi/dmpsent.pl');
  alert(ans);
};

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

function out11_keypress(evt) {
  var kevt=(evt || window.event);
  var key=(kevt.key || kevt.which);
  var regexp=new RegExp('[cvx]');
  var ans=key.match(regexp);

  if (!kevt.getModifierState('Control') || !ans) {
    kevt.preventDefault();
  }
};

function out12_keypress(evt) {
  var kevt=(evt || window.event);
  var key=(kevt.key || kevt.which);
  var regexp=new RegExp('[cvx]');
  var ans=key.match(regexp);

  if (!kevt.getModifierState('Control') || !ans) {
    kevt.preventDefault();
  }
};

function out13_keypress(evt) {
  var kevt=(evt || window.event);
  var key=(kevt.key || kevt.which);
  var regexp=new RegExp('[cvx]');
  var ans=key.match(regexp);

  if (!kevt.getModifierState('Control') || !ans) {
    kevt.preventDefault();
  }
};

function canvas11_mousedown(evt) {
  var canvas11=chart.findElementById('canvas11');
  var rr=new chart.shapes.Rect();
  rr.setWidth(canvas11.scrollWidth);
  rr.setHeight(canvas11.scrollHeight);
  //alert(rr.toString());
};
