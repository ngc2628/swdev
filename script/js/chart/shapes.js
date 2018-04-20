
chart.shapes={};

/* ########## */
chart.shapes.Shape2=function(name,cntvertices) {
  if (typeof name!='string')
    name='shape2';
  this.typename=name;
  this.radius=[0,0];
  this.mtransform=new mkmat.TransformMatrix(2);
  this.vertices=[];
  cntvertices=mkfloat.cut2int(mkuni.chknumber(cntvertices,0));
  var ii=0;
  for (ii=0;ii<cntvertices;ii++)
    this.vertices.push(new mkmat.Vertex());
};

/* ********** */
chart.shapes.Shape2.prototype.move=function(vertex) {
  if (vertex instanceof mkmat.Vertex)
    this.mtransform.translate(vertex);
};

/* ********** */
chart.shapes.Shape2.prototype.toJson=function(indent) {
  var nextindent='  ',ans=aux.qdecl('typename',indent);
  if (typeof indent=='string')
    nextindent+=indent;
  ans+=aux.qword(this.typename);
  ans+=aux.qnextprop(1);
  ans+=aux.qdecl('radius',indent);
  var tmp=aux.qword(mkfloat.sprintf(this.radius[0],0,'d'));
  tmp+=aux.qnextprop();
  tmp+=aux.qword(mkfloat.sprintf(this.radius[1],0,'d'));
  ans+=aux.qarr(tmp,0);
  ans+=aux.qnextprop(1);
  if (this.mtransform.isidentity()==0) {
    ans+=aux.qdecl('transform',indent);
    ans+=this.mtransform.toJson(indent);
    ans+=aux.qnextprop(1);
  }
  ans+=aux.qdecl('vertices',indent);
  tmp='';
  var ii=0;
  for (ii=0;ii<this.vertices.length;ii++) {
    if (ii>0)
      tmp+=aux.qnextprop(0);
    tmp+=this.vertices[ii].toJson(void(0),2);
  }
  ans+=aux.qarr(tmp,0);
  return ans;
};

/* ********** */
chart.shapes.Shape2.prototype.toSvgPath=function(center) {
  var ans='';
  var ii=0,jj=0;
  var vertex=new mkmat.Vertex();
  if (center instanceof mkmat.Vertex)
    this.mtransform.getTranslate(center);
  if (this.typename=='diamond')
    this.mtransform.rotatez(45);
  for (ii=0;ii<=this.vertices.length;ii++) {
    jj=(ii==this.vertices.length ? 0 : ii);
    this.mtransform.transform(this.vertices[jj],vertex);
  //alert(jj+' '+this.typename+' '+this.vertices[jj].toJson()+' '+vertex.toJson());
    ans+=(ii==0 ? 'M ' : ' L ');
    ans+=mkfloat.sprintf(vertex.val(0),0,'d');
    ans+=' ';
    ans+=mkfloat.sprintf(vertex.val(1),0,'d');
  }
  return ans;
};
/* ########## */

/* ########## */
chart.shapes.Rect=function(name,ax,ay) {
  chart.shapes.Shape2.call(this,name,4);
  this.setRect(ax,ay);
};

/* ********** */
chart.shapes.Rect.prototype=Object.create(chart.shapes.Shape2.prototype);

/* ********** */
chart.shapes.Rect.prototype.constructor=chart.shapes.Rect;

/* ********** */
chart.shapes.Rect.prototype.setRect=function(ax,ay) {
  ax=mkuni.chknumber(ax,0);
  ax=(ax<0 ? 0 : ax);
  ay=mkuni.chknumber(ay,0);
  ay=(ay<0 ? 0 : ay);
  this.radius[0]=ax/2;
  this.radius[1]=ay/2;
  this.vertices[0].setVal(0,-this.radius[0]);
  this.vertices[0].setVal(1,-this.radius[1]);
  this.vertices[1].setVal(0,this.radius[0]);
  this.vertices[1].setVal(1,-this.radius[1]);
  this.vertices[2].setVal(0,this.radius[0]);
  this.vertices[2].setVal(1,this.radius[1]);
  this.vertices[3].setVal(0,-this.radius[0]);
  this.vertices[3].setVal(1,this.radius[1]);
};
/* ########## */

/* ########## */
chart.shapes.Triangle=function(aa) {
  chart.shapes.Shape2.call(this,'triangle',3);
  this.setTriangle(aa);
};

/* ********** */
chart.shapes.Triangle.prototype=Object.create(chart.shapes.Shape2.prototype);

/* ********** */
chart.shapes.Triangle.prototype.constructor=chart.shapes.Triangle;

/* ********** */
chart.shapes.Triangle.prototype.setTriangle=function(aa) {
  aa=mkuni.chknumber(aa,0);
  aa=(aa<0 ? 0 : aa);
  this.radius[0]=0.577350269189626*aa; // rout, sqrt(3)/3=1.732050807568877/3
  this.radius[1]=this.radius[0]/2; // rin
  this.vertices[0].setVal(0,0);
  this.vertices[0].setVal(1,-this.radius[0]);
  this.vertices[1].setVal(0,aa/2);
  this.vertices[1].setVal(1,this.radius[1]);
  this.vertices[2].setVal(0,-aa/2);
  this.vertices[2].setVal(1,this.radius[1]);
};
/* ########## */

chart.shapes.cntsvgcirclearc=4;

/* ########## */
chart.shapes.Circle=function(radius,cntvertices) {
  chart.shapes.Shape2.call(this,'circle',0);
  this.setCircle(radius,cntvertices);
};

/* ********** */
chart.shapes.Circle.prototype=Object.create(chart.shapes.Shape2.prototype);

/* ********** */
chart.shapes.Circle.prototype.constructor=chart.shapes.Circle;

/* ********** */
chart.shapes.Circle.prototype.setCircle=function(rr,cntvertices) {
  rr=mkuni.chknumber(rr,0);
  rr=(rr<0 ? 0 : rr);
  this.radius[0]=this.radius[1]=rr;
  var cntsvgarcvertices=4;
  cntvertices=mkint.cut2int(mkuni.chknumber(cntvertices,chart.shapes.cntsvgcirclearc));
  cntvertices=
    (cntvertices<chart.shapes.cntsvgcirclearc ? chart.shapes.cntsvgcirclearc : cntvertices);
  if (cntvertices!=this.vertices.length)
    this.vertices=[];
  if (cntvertices==chart.shapes.cntsvgcirclearc) {
    this.vertices[0]=new mkmat.Vertex(0,-this.radius[0]);
    this.vertices[1]=new mkmat.Vertex(this.radius[0],0);
    this.vertices[2]=new mkmat.Vertex(0,this.radius[0]);
    this.vertices[3]=new mkmat.Vertex(-this.radius[0],0);
  }
  else {
    var ii=0,sc=2*Math.PI/(cnt-1);
    for (ii=0;ii<cntvertices;ii++)
      this.vertices[ii]=new mkmat.Vertex(rr*Math.cos(sc*ii),rr*Math.sin(sc*ii));
  }
};

/* ********** */
chart.shapes.Circle.prototype.toSvgPath=function() {
  var ii=0,jj=0,cntvertices=this.vertices.length;
  if (cntvertices!=chart.shapes.cntsvgcirclearc)
    return chart.shapes.Shape2.prototype.toSvgPath.call(this);
  var ans='',sradius=mkfloat.sprintf(this.radius[0],0,'d'),srotlargesweep=' 0 0 1 ';
  var vertex=new mkmat.Vertex();
  for (ii=0;ii<=cntvertices;ii++) {
    if (ii==0)
      ans+='M ';
    jj=(ii==cntvertices ? 0 : ii);
    this.mtransform.transform(this.vertices[jj],vertex);
    ans+=mkfloat.sprintf(vertex.val(0),0,'d');
    ans+=' ';
    ans+=mkfloat.sprintf(vertex.val(1),0,'d');
    if (ii==cntvertices)
      break;
    ans+=' A ';
    ans+=sradius;
    ans+=' ';
    ans+=sradius;
    ans+=srotlargesweep;
  }
  return ans;
};
/* ########## */

/* ########## */
chart.shapes.WinRect=function(left,top,right,bottom) {
  chart.shapes.Shape2.call(this,'rect',4);
  this.setRect(left,top,right,bottom);
};

/* ********** */
chart.shapes.WinRect.prototype=Object.create(chart.shapes.Shape2.prototype);

/* ********** */
chart.shapes.WinRect.prototype.constructor=chart.shapes.WinRect;

/* ********** */
chart.shapes.WinRect.prototype.align=function() {
  var dfx=this.vertices[2].val(0)-this.vertices[0].val(0),
      dfy=this.vertices[2].val(1)-this.vertices[0].val(1);
  if (dfx<0) {
    aux.swapidx(this.vertices,0,1);
    aux.swapidx(this.vertices,2,3);
    dfx=-dfx;
  }
  if (dfy<0) {
    aux.swapidx(this.vertices,0,3);
    aux.swapidx(this.vertices,1,2);
    dfy=-dfy;
  }
  this.radius[0]=dfx/2;
  this.radius[1]=dfy/2;
};

/* ********** */
chart.shapes.WinRect.prototype.setRect=function(left,top,right,bottom) {
  left=mkuni.chknumber(left,0);
  top=mkuni.chknumber(top,0);
  right=mkuni.chknumber(right,0);
  bottom=mkuni.chknumber(bottom,0);
  this.vertices[0].setVal(0,left);
  this.vertices[0].setVal(1,top);
  this.vertices[1].setVal(0,right);
  this.vertices[1].setVal(1,top);
  this.vertices[2].setVal(0,right);
  this.vertices[2].setVal(1,bottom);
  this.vertices[3].setVal(0,left);
  this.vertices[3].setVal(1,bottom);
  this.align();
};

/* ********** */
chart.shapes.WinRect.prototype.copy=function() {
  var ans=new this.constructor(
    this.vertices[0].val(0),this.vertices[0].val(1),
    this.vertices[2].val(0),this.vertices[2].val(1)
  );
  return ans;
};

/* ********** */
chart.shapes.WinRect.prototype.left=function() {
  return this.vertices[0].val(0);
};

/* ********** */
chart.shapes.WinRect.prototype.setLeft=function(left) {
  left=mkuni.chknumber(left,0);
  this.vertices[0].setVal(0,left);
  this.vertices[3].setVal(0,left);
  this.align();
};

/* ********** */
chart.shapes.WinRect.prototype.top=function() {
  return this.vertices[0].val(1);
};

/* ********** */
chart.shapes.WinRect.prototype.setTop=function(top) {
  top=mkuni.chknumber(top,0);
  this.vertices[0].setVal(1,top);
  this.vertices[1].setVal(1,top);
  this.align();
};

/* ********** */
chart.shapes.WinRect.prototype.right=function() {
  return this.vertices[2].val(0);
};

/* ********** */
chart.shapes.WinRect.prototype.setRight=function(right) {
  right=mkuni.chknumber(right,0);
  this.vertices[1].setVal(0,right);
  this.vertices[2].setVal(0,right);
  this.align();
};

/* ********** */
chart.shapes.WinRect.prototype.bottom=function() {
  return this.vertices[2].val(1);
};

/* ********** */
chart.shapes.WinRect.prototype.setBottom=function(bottom) {
  bottom=mkuni.chknumber(bottom,0);
  this.vertices[2].setVal(1,bottom);
  this.vertices[3].setVal(1,bottom);
  this.align();
};

/* ********** */
chart.shapes.WinRect.prototype.width=function() {2
  return (this.vertices[2].val(0)-this.vertices[0].val(0));
};

/* ********** */
chart.shapes.WinRect.prototype.setWidth=function(width) {
  width=mkuni.chknumber(width,0);
  if (width<0)
    width=0;
  var right=this.vertices[0].val(0)+width;
  this.vertices[1].setVal(0,right);
  this.vertices[2].setVal(0,right);
  this.align();
};

/* ********** */
chart.shapes.WinRect.prototype.height=function() {
  return (this.vertices[2].val(1)-this.vertices[0].val(1));
};

/* ********** */
chart.shapes.WinRect.prototype.setHeight=function(height) {
  height=mkuni.chknumber(height,0);
  if (height<0)
    height=0;
  var bottom=this.vertices[0].val(1)+height;
  this.vertices[2].setVal(1,bottom);
  this.vertices[3].setVal(0,bottom);
  this.align();
};

/* ********** */
chart.shapes.WinRect.prototype.deflate=function(percent) {
  percent=mkuni.chknumber(percent,100);
  if (mkint.chknumberrange(percent,0,100)==mkuni.bad)
    percent=100;
  var left=this.vertices[0].val(0),top=this.vertices[0].val(1),
      right=this.vertices[2].val(0),bottom=this.vertices[2].val(1),
      modw=(100-percent)*(right-left)/200,modh=(100-percent)*(bottom-top)/200;
  this.vertices[0].setVal(0,left+modw);
  this.vertices[0].setVal(1,top+modh);
  this.vertices[1].setVal(0,right-modw);
  this.vertices[1].setVal(1,top+modh);
  this.vertices[2].setVal(0,right-modw);
  this.vertices[2].setVal(1,bottom-modh);
  this.vertices[3].setVal(0,left+modw);
  this.vertices[3].setVal(1,bottom-modh);
  this.align();
};
/* ########## */

