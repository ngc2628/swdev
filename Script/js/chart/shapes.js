
chart.shapes={};

chart.shapes.Rect=function(left,top,right,bottom) {
  this.ltrb=[0,0,0,0];
  this.setRect(left,top,right,bottom);
};

chart.shapes.Rect.prototype.setRect=function(left,top,right,bottom) {
  this.ltrb[0]=intobj.chknumber(left,0);
  this.ltrb[1]=intobj.chknumber(top,0);
  this.ltrb[2]=intobj.chknumber(right,0);
  this.ltrb[3]=intobj.chknumber(bottom,0);
  if (this.ltrb[2]<this.ltrb[0])
    aux.swapidx(this.ltrb,2,0);
  if (this.ltrb[3]<this.ltrb[1])
    aux.swapidx(this.ltrb,3,1);
};

chart.shapes.Rect.prototype.copy=function() {
  var ans=new this.constructor(this.ltrb[0],this.ltrb[1],this.ltrb[2],this.ltrb[3]);
  return ans;
};

chart.shapes.Rect.prototype.left=function() {
  return this.ltrb[0];
};

chart.shapes.Rect.prototype.setLeft=function(left) {
  this.ltrb[0]=intobj.chknumber(left,0);
  if (this.ltrb[2]<this.ltrb[0])
    aux.swapidx(this.ltrb,2,0);
};

chart.shapes.Rect.prototype.top=function() {
  return this.ltrb[1];
};

chart.shapes.Rect.prototype.setTop=function(top) {
  this.ltrb[1]=intobj.chknumber(top,0);
  if (this.ltrb[3]<this.ltrb[1])
    aux.swapidx(this.ltrb,3,1);
};

chart.shapes.Rect.prototype.right=function() {
  return this.ltrb[2];
};

chart.shapes.Rect.prototype.setRight=function(right) {
  this.ltrb[2]=intobj.chknumber(right,0);
  if (this.ltrb[2]<this.ltrb[0])
    aux.swapidx(this.ltrb,2,0);
};

chart.shapes.Rect.prototype.bottom=function() {
  return this.ltrb[3];
};

chart.shapes.Rect.prototype.setBottom=function(bottom) {
  this.ltrb[3]=intobj.chknumber(bottom,0);
  if (this.ltrb[3]<this.ltrb[1])
    aux.swapidx(this.ltrb,3,1);
};

chart.shapes.Rect.prototype.width=function() {
  return (this.ltrb[2]-this.ltrb[0]);
};

chart.shapes.Rect.prototype.setWidth=function(width) {
  width=intobj.chknumber(width,0);
  if (width<0)
    width=0;
  this.ltrb[2]=this.ltrb[0]+width;
};

chart.shapes.Rect.prototype.height=function() {
  return (this.ltrb[3]-this.ltrb[1]);
};

chart.shapes.Rect.prototype.setHeight=function(height) {
  height=intobj.chknumber(height,0);
  if (height<0)
    height=0;
  this.ltrb[3]=this.ltrb[1]+height;
};

chart.shapes.Rect.prototype.deflate=function(percent) {
  percent=intobj.chknumber(percent,100);
  if (intobj.chknumberrange(percent,0,100)==aux.ansko)
    percent=100;
  var ww=this.ltrb[2]-this.ltrb[0],hh=this.ltrb[3]-this.ltrb[1];
  this.ltrb[0]+=((100-percent)*ww/200);
  this.ltrb[1]+=((100-percent)*hh/200);
  this.ltrb[2]-=((100-percent)*ww/200);
  this.ltrb[3]-=((100-percent)*hh/200);
};

chart.shapes.Rect.prototype.toJson=function(indent) {
  var ans=aux.qdecl('ltrb');
  var varr=(isNaN(this.ltrb[0]) ? 'null' : aux.qword(this.ltrb[0]));
  varr+=aux.qnextprop();
  varr+=(isNaN(this.ltrb[1]) ? 'null' : aux.qword(this.ltrb[1]));
  varr+=aux.qnextprop();
  varr+=(isNaN(this.ltrb[2]) ? 'null' : aux.qword(this.ltrb[2]));
  varr+=aux.qnextprop();
  varr+=(isNaN(this.ltrb[3]) ? 'null' : aux.qword(this.ltrb[3]));
  ans+=aux.qarr(varr);
  if (typeof indent=='string')
    ans=(indent+ans);
  return ans;
};

