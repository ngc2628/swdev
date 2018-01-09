
var mkmat={};

/* ########## */
mkmat.Vertex=function(xx,yy,zz,ww) {
  this.xyzw=[
    mkuni.chknumber(xx,NaN),mkuni.chknumber(yy,NaN),
    mkuni.chknumber(zz,NaN),mkuni.chknumber(ww,NaN)
  ];
};

/* ********** */
mkmat.Vertex.prototype.copy=function() {
  var ans=new this.constructor(this.xyzw[0],this.xyzw[1],this.xyzw[2],this.xyzw[3]);
  return ans;
};

/* ********** */
mkmat.Vertex.prototype.val=function(idx) {
  var ans=mkint.chknumberrange(idx,mkuni.typeX,mkuni.typeW);
  if (ans==mkuni.good)
    return this.xyzw[mkfloat.cut2int(idx)];
  return NaN;
};

/* ********** */
mkmat.Vertex.prototype.setVal=function(idx,val) {
  var ans=mkint.chknumberrange(idx,mkuni.typeX,mkuni.typeW);
  if (ans==mkuni.good)
    this.xyzw[mkfloat.cut2int(idx)]=mkuni.chknumber(val,NaN);
};

/* ********** */
mkmat.Vertex.prototype.reset=function() {
  this.xyzw=[NaN,NaN,NaN,NaN];
};

/* ********** */
mkmat.Vertex.prototype.toJson=function(indent,dim,addname) {
  var ans='';
  if (typeof dim!='number')
    dim=4;
  if ((typeof addname=='number') && addname>0)
    ans+=aux.qdecl('xyzw');
  var varr='';
  if (dim>0) {
    varr+=(isNaN(this.xyzw[0]) ? aux.qany('null') : aux.qword(this.xyzw[0]));
    if (dim>1) {
      varr+=aux.qnextprop();
      varr+=(isNaN(this.xyzw[1]) ? aux.qany('null') : aux.qword(this.xyzw[1]));
      if (dim>2) {
        varr+=aux.qnextprop();
        varr+=(isNaN(this.xyzw[2]) ? aux.qany('null') : aux.qword(this.xyzw[2]));
        if (dim>3) {
          varr+=aux.qnextprop();
          varr+=(isNaN(this.xyzw[3]) ? aux.qany('null') : aux.qword(this.xyzw[3]));
        }
      }
    }
  }
  ans+=aux.qarr(varr);
  if (typeof indent=='string')
    ans=(indent+ans);
  return ans;
};
/* ########## */

/* ********** */
mkmat.vectorsubs=function(vertex1,vertex2) {
  var ans=new mkmat.Vertex();
  if ( ! (vertex1 instanceof mkmat.Vertex) || ! (vertex2 instanceof mkmat.Vertex))
    return ans;
  var pp1=[
    mkuni.chknumber(vertex1.xyzw[0],NaN),
    mkuni.chknumber(vertex1.xyzw[1],NaN),
    mkuni.chknumber(vertex1.xyzw[2],NaN)
  ];
  var pp2=[
    mkuni.chknumber(vertex2.xyzw[0],NaN),
    mkuni.chknumber(vertex2.xyzw[1],NaN),
    mkuni.chknumber(vertex2.xyzw[2],NaN)
  ];
  ans=new mkmat.Vertex(
    isNaN(pp1[0]) || isNaN(pp2[0]) ? NaN : pp1.xyzw[0]-pp2.xyzw[0],
    isNaN(pp1[1]) || isNaN(pp2[1]) ? NaN : pp1.xyzw[1]-pp2.xyzw[1],
    isNaN(pp1[2]) || isNaN(pp2[2]) ? NaN : pp1.xyzw[2]-pp2.xyzw[2]
  );
  return ans;
};

/* ********** */
mkmat.vectorlen=function(vertex) {
  if ( ! (vertex instanceof mkmat.Vertex))
    return 0;
  var pp=[
    mkuni.chknumber(vertex.xyzw[0],0),
    mkuni.chknumber(vertex.xyzw[1],0),
    mkuni.chknumber(vertex.xyzw[2],0)
  ];
  var ans=pp[0]*pp[0]+pp[1]*pp[1]+pp[2]*pp[2];
  return Math.sqrt(ans);
};

mkmat.vectordot=function(vertex1,vertex2) {
  if ( ! (vertex1 instanceof mkmat.Vertex) ||
       ! (vertex2 instanceof mkmat.Vertex))
    return NaN;
  var pp=[
    [mkuni.chknumber(vertex1.xyzw[0],0),
     mkuni.chknumber(vertex1.xyzw[1],0),
     mkuni.chknumber(vertex1.xyzw[2],0)
    ],
    [mkuni.chknumber(vertex2.xyzw[0],0),
     mkuni.chknumber(vertex2.xyzw[1],0),
     mkuni.chknumber(vertex2.xyzw[2],0)
    ]
  ];
  var ans=pp[0][0]*pp[1][0]+pp[0][1]*pp[1][1]+pp[0][2]*pp[1][2];
  return ans;
};

/* ********** */
mkmat.vectornorm=function(vertex) {
  var ans=new mkmat.Vertex();
  if ( ! (vertex instanceof mkmat.Vertex))
    return ans;
  var nn=mkmat.vectorlen(vertex);
  if (nn<=0)
    return ans;
  if (!isNaN(vertex.xyzw[0]))
    ans.xyzw[0]=vertex.xyzw[0]/nn;
  if (!isNaN(vertex.xyzw[1]))
    ans.xyzw[1]=vertex.xyzw[1]/nn;
  if (!isNaN(vertex.xyzw[2]))
    ans.xyzw[2]=vertex.xyzw[2]/nn;
  return ans;
};

/* ********** */
mkmat.vectorcross=function(vertex1,vertex2) {
  var ans=new mkmat.Vertex();
  if ( ! (vertex1 instanceof mkmat.Vertex) || ! (vertex2 instanceof mkmat.Vertex))
    return ans;
  var pp1=[
    mkuni.chknumber(vertex1.xyzw[0],NaN),
    mkuni.chknumber(vertex1.xyzw[1],NaN),
    mkuni.chknumber(vertex1.xyzw[2],NaN)
  ];
  var pp2=[
    mkuni.chknumber(vertex2.xyzw[0],NaN),
    mkuni.chknumber(vertex2.xyzw[1],NaN),
    mkuni.chknumber(vertex2.xyzw[2],NaN)
  ];
  if (!isNaN(pp1[1]) && !isNaN(pp2[1]) && !isNaN(pp1[2]) && !isNaN(pp2[2]))
    ans.xyzw[0]=pp1[1]*pp2[2]-pp1[2]*pp2[1];
  if (!isNaN(pp1[0]) && !isNaN(pp2[0]) && !isNaN(pp1[2]) && !isNaN(pp2[2]))
    ans.xyzw[1]=pp1[2]*pp2[0]-pp1[0]*pp2[2];
  if (!isNaN(pp1[0]) && !isNaN(pp2[0]) && !isNaN(pp1[1]) && !isNaN(pp2[1]))
    ans.xyzw[2]=pp1[0]*pp2[1]-pp1[1]*pp2[0];
  return ans;
};

/* ********** */
mkmat.angrad=function(vertex1,vertex2) {
  if ( ! (vertex1 instanceof mkmat.Vertex) ||
       ! (vertex2 instanceof mkmat.Vertex))
    return 0;
  var den=mkmat.vectorlen(vertex1)*mkmat.vectorlen(vertex2);
  if (den==0)
    return 0;
  var ans=mkmat.vectordot(vertex1,vertex2)/den;
  ans=Math.acos(ans);
  return ans;
  //return atan2(vectordot(vertex1,vertex2)/den,
  //sqrt(1.-(vectordot(vertex1,vertex2)/den)*(vectordot(vertex1,vertex2)/den)));
};

/* ********** */
mkmat.angdeg=function(vertex1,vertex2) {
  if ( ! (vertex1 instanceof mkmat.Vertex) ||
       ! (vertex2 instanceof mkmat.Vertex))
    return 0;
  var den=mkmat.vectorlen(vertex1)*mkmat.vectorlen(vertex2);
  if (den==0)
    return 0;
  var ans=vectordot(vertex1,vertex2)/den;
  ans=Math.acos(ans)/parseFloat(mkfloat.rad);
  return ans;
  //return atan2(sqrt(1.-ans*ans),ans)/mkfloat.rad;
};

/* ********** */
mkmat.lineRot=function(vertex11,vertex12,vertex21,vertex22,rotax) {
  if (rotax instanceof mkmat.Vertex)
    rotax.xyzw=[NaN,NaN,NaN,NaN];
  if ( ! (vertex11 instanceof mkmat.Vertex) ||
       ! (vertex12 instanceof mkmat.Vertex) ||
       ! (vertex21 instanceof mkmat.Vertex) ||
       ! (vertex22 instanceof mkmat.Vertex))
    return 0;
  var v1=mkmat.vectorsubs(vertex12,vertex11),v2=mkmat.vectorsubs(vertex22,vertex21);
  v1=mkmat.vectornorm(v1);
  v2=mkmat.vectornorm(v2);
  if (rotax instanceof mkmat.Vertex)
    rotax=mkmat.vectorcross(v1,v2);
  var ans=mkmat.angrad(v1,v2);
  return ans;
};

/* ********** */
mkmat.mindistpointplane=function(vertex1,vertex2,vertex3,vertex4,vertexinter) {
  if (vertexinter instanceof mkmat.Vertex)
    vertexinter.xyzw=[NaN,NaN,NaN,NaN];
  if ( ! (vertex1 instanceof mkmat.Vertex) ||
       ! (vertex2 instanceof mkmat.Vertex) ||
       ! (vertex3 instanceof mkmat.Vertex) ||
       ! (vertex4 instanceof mkmat.Vertex))
    return NaN;
  var v21=mkmat.vectorsubs(vertex2,vertex1),v31=mkmat.vectorsubs(vertex3,vertex1);
  var v41=mkmat.vectorsubs(vertex4,vertex1),vn=mkmat.vectorcross(v21,v31);
  var ans=NaN,lvn=mkmat.vectorlen(vn);
  if (lvn!=0) {
    ans=mkmat.vectordot(vn,v41)/lvn;
    if (vertexinter instanceof mkmat.Vertex) {
      vertexinter.xyzw[0]=vn[0]*(dist/lvn);
      vertexinter.xyzw[1]=vn[1]*(dist/lvn);
      vertexinter.xyzw[2]=vn[2]*(dist/lvn);
      vertexinter=mkmat.vectorsubs(vertex4,vertexinter);
    }
  }
  return ans;
};

/* ********** */
mkmat.mindistpointline=function(vertex1,vertex2,vertex3,vertexinter) {
  if (vertexinter instanceof mkmat.Vertex)
    vertexinter.xyzw=[NaN,NaN,NaN,NaN];
  if ( ! (vertex1 instanceof mkmat.Vertex) ||
       ! (vertex2 instanceof mkmat.Vertex) ||
       ! (vertex3 instanceof mkmat.Vertex))
    return NaN;
  var v21=mkmat.vectorsubs(vertex2,vertex1),v31=mkmat.vectorsubs(vertex3,vertex1);
  var v21xv31=mkmat.vectorcross(v21,v31),v1x=mkmat.vectorsubs(vertex1,v21xv31);
  var ans=mkmat.mindistpointplane(vertex1,vertex2,v1x,vertex3,vertexinter);
  return ans;
};

/* ********** */
mkmat.rotate2=function(vertex,deg) {
  var ans=new mkmat.Vertex();
  if (vertex instanceof mkmat.Vertex) {
    var ss=Math.sin(deg*parseFloat(mkfloat.rad));
    var cc=Math.cos(deg*parseFloat(mkfloat.rad));
    ans.setVal(0,vertex.val(0)*cc-vertex.val(1)*ss);
    ans.setVal(1,vertex.val(0)*ss+vertex.val(1)*cc);
  }
  return ans;
};

/* ********** */
mkmat.translate2=function(vertex,vertextrans) {
  var ans=new mkmat.Vertex();
  if ((vertex instanceof mkmat.Vertex) && (vertextrans instanceof mkmat.Vertex)) {
    ans.setVal(0,vertex.val(0)+vertextrans.val(0));
    ans.setVal(1,vertex.val(1)+vertextrans.val(1));
  }
  return ans;
};

/* ########## */
mkmat.Matrix=function(dimr,dimc) {
  dimr=mkuni.chknumber(dimr,0);
  dimc=mkuni.chknumber(dimc,0);
  this.matrix=[];
  var row=[];
  var ii=0,jj=0;
  for (ii=0;ii<dimr;ii++) {
    for (jj=0;jj<dimc;jj++)
      row.push(ii==jj ? 1 : 0);
    this.matrix.push(row);
    row=[];
  }
};

/* ********** */
mkmat.Matrix.prototype.copy=function() {
  var ii=0,jj=0,dimr=this.matrix.length,dimc=(dimr==0 ? 0 : this.matrix[0].length);
  var ans=new this.constructor(dimr,dimc);
  for (ii=0;ii<dimr;ii++) {
    for (jj=0;jj<dimc;jj++)
      ans.matrix[ii][jj]=this.matrix[ii][jj];
  }
  return ans;
};

/* ********** */
mkmat.Matrix.prototype.loadidentity=function() {
  var ii=0,jj=0,dimr=this.matrix.length,dimc=(dimr==0 ? 0 : this.matrix[0].length);
  for (ii=0;ii<dimr;ii++) {
    for (jj=0;jj<dimc;jj++)
      this.matrix[ii][jj]=(ii==jj ? 1 : 0);
  }
};

/* ********** */
mkmat.Matrix.prototype.isidentity=function() {
  var ans=1;
  var ii=0,jj=0,dimr=this.matrix.length,dimc=(dimr==0 ? 0 : this.matrix[0].length);
  for (ii=0;ii<dimr;ii++) {
    for (jj=0;jj<dimc;jj++) {
      if (isNaN(this.matrix[ii][jj]) ||
          (ii==jj && this.matrix[ii][jj]!=1) ||
          (ii!=jj && this.matrix[ii][jj]!=0)) {
        ans=0;
        break;
      }
    }
  }
  return ans;
};

/* ********** */
mkmat.Matrix.prototype.rows=function() {
  return this.matrix.length;
};

/* ********** */
mkmat.Matrix.prototype.cols=function() {
  var ans=this.matrix.length;
  if (ans>0)
    ans=this.matrix[0].length;
  return ans;
};

/* ********** */
mkmat.Matrix.prototype.val=function(row,col) {
  var ans=NaN;
  var dimr=this.matrix.length,dimc=(dimr==0 ? 0 : this.matrix[0].length);
  if (dimr==0 || dimc==0)
    return ans;
  row=mkuni.chknumber(row,-1);
  col=mkuni.chknumber(col,-1);
  if (mkint.chknumberrange(row,0,dimr-1)==mkuni.bad ||
      mkint.chknumberrange(col,0,dimc-1)==mkuni.bad)
    return ans;
  ans=this.matrix[row][col];
  return ans;
};

/* ********** */
mkmat.Matrix.prototype.setVal=function(row,col,vv) {
  var ans=mkuni.bad;
  var dimr=this.matrix.length,dimc=(dimr==0 ? 0 : this.matrix[0].length);
  if (dimr==0 || dimc==0) {
    row=mkuni.chknumber(row,-1);
    col=mkuni.chknumber(col,-1);
    if (mkint.chknumberrange(row,0,dimr-1)==mkuni.good ||
        mkint.chknumberrange(col,0,dimc-1)==mkuni.good) {
      this.matrix[row][col]=mkuni.chknumber(vv,NaN);
      ans=mkuni.good;
    }
  }
  return ans;
};

/* ********** */
mkmat.Matrix.prototype.mult=function(multmat) {
  var ans=mkuni.bad;
  if (! (multmat instanceof mkmat.Matrix))
    return ans;
  var dimr=this.matrix.length,dimc=(dimr==0 ? 0 : this.matrix[0].length);
  var mmdimr=multmat.rows(),mmdimc=multmat.cols();
  if (dimr==0 || dimc==0 || mmdimr==0 || mmdimc==0 || dimc!=mmdimr)
    return ans;
  var ii=0,jj=0,kk=0,vv=0;
  var mat=[];
  var row=[];
  for (ii=0;ii<dimr;ii++) {
    for (jj=0;jj<dimc;jj++)
      row.push(this.matrix[ii][jj]);
    mat.push(row);
    row=[];
  }
  this.matrix=[];
  for (ii=0;ii<dimr;ii++) {
    for (jj=0;jj<mmdimc;jj++) {
      for (kk=0;kk<dimc;kk++)
        vv+=mat[ii][kk]*multmat.matrix[kk][jj];
      row.push(vv);
      vv=0;
    }
    this.matrix.push(row);
    row=[];
  }
  ans=mkuni.good;
  return ans;
};

/* ********** */
mkmat.Matrix.prototype.toJson=function(indent) {
  var nextindent='  ';
  if (typeof indent=='string')
    nextindent+=indent;
  var ans='';
  var varr='';
  var dimr=this.matrix.length,dimc=(dimr==0 ? 0 : this.matrix[0].length);
  var ii=0,jj=0;
  for (ii=0;ii<dimr;ii++) {
    ans+=(ii==0 ? aux.qany('\n') : aux.qnextprop(1));
    for (jj=0;jj<dimc;jj++) {
      if (jj>0)
        varr+=aux.qnextprop(0,' ');
      varr+=this.matrix[ii][jj].toString();
    }
    ans+=aux.qarr(varr,0,nextindent);
    varr='';
  }
  ans=aux.qarr(ans,1,indent);
  return ans;
};
/* ########## */

/* ########## */
mkmat.TransformMatrix=function(dim) {
  dim=mkuni.chknumber(dim,0);
  dim=(dim==2 ? 3 : 4);
  mkmat.Matrix.call(this,dim,dim);
};

/* ********** */
mkmat.TransformMatrix.prototype=Object.create(mkmat.Matrix.prototype);

/* ********** */
mkmat.TransformMatrix.prototype.constructor=mkmat.TransformMatrix;

/* ********** */
mkmat.TransformMatrix.prototype.copy=function() {
  var ans=new this.constructor(this.matrix.length-1);
  return ans;
};

/* ********** */
mkmat.TransformMatrix.prototype.translate=function(vertex) {
  if (! (vertex instanceof mkmat.Vertex))
    return;
  var multmat=this.copy();
  multmat.loadidentity();
  var dim=multmat.matrix.length;
  multmat.matrix[0][dim-1]=mkuni.chknumber(vertex.val(0),0);
  multmat.matrix[1][dim-1]=mkuni.chknumber(vertex.val(1),0);
  if (dim==4)
    multmat.matrix[2][dim-1]=mkuni.chknumber(vertex.val(2),0);
  this.mult(multmat);
};

/* ********** */
mkmat.TransformMatrix.prototype.getTranslate=function(vertex) {
  if (vertex instanceof mkmat.Vertex) {
    var dim=this.matrix.length;
    vertex.setVal(0,this.matrix[0][dim-1]);
    vertex.setVal(1,this.matrix[1][dim-1]);
    vertex.setVal(2,dim==4 ? this.matrix[2][dim-1] : 1);
    vertex.setVal(3,1);
  }
};

/* ********** */
mkmat.TransformMatrix.prototype.scale=function(xx,yy,zz) {
  var multmat=this.copy();
  multmat.loadidentity();
  multmat.matrix[0][0]=mkuni.chknumber(xx,0);
  multmat.matrix[1][1]=mkuni.chknumber(yy,0);
  multmat.matrix[2][2]=mkuni.chknumber(zz,0);
  this.mult(multmat);
};

/* ********** */
mkmat.TransformMatrix.prototype.rotatex=function(ang) {
  var multmat=this.copy();
  multmat.loadidentity();
  ang=mkuni.chknumber(ang,0);
  var ss=Math.sin(ang*parseFloat(mkfloat.rad));
  var cc=Math.cos(ang*parseFloat(mkfloat.rad));
  multmat.matrix[1][1]=cc;
  multmat.matrix[1][2]=-ss;
  multmat.matrix[2][1]=ss;
  multmat.matrix[2][2]=cc;
  this.mult(multmat);
};

/* ********** */
mkmat.TransformMatrix.prototype.rotatey=function(ang) {
  var multmat=this.copy();
  multmat.loadidentity();
  ang=mkuni.chknumber(ang,0);
  var ss=Math.sin(ang*parseFloat(mkfloat.rad));
  var cc=Math.cos(ang*parseFloat(mkfloat.rad));
  multmat.matrix[0][0]=cc;
  multmat.matrix[0][2]=ss;
  multmat.matrix[2][0]=-ss;
  multmat.matrix[2][2]=cc;
  this.mult(multmat);
};

/* ********** */
mkmat.TransformMatrix.prototype.rotatez=function(ang) {
  var multmat=this.copy();
  multmat.loadidentity();
  ang=mkuni.chknumber(ang,0);
  var ss=Math.sin(ang*parseFloat(mkfloat.rad));
  var cc=Math.cos(ang*parseFloat(mkfloat.rad));
  multmat.matrix[0][0]=cc;
  multmat.matrix[0][1]=-ss;
  multmat.matrix[1][0]=ss;
  multmat.matrix[1][1]=cc;
  this.mult(multmat);
};

/* ********** */
mkmat.TransformMatrix.prototype.transform=function(vin,vout) {
  var ans=mkuni.bad;
  if ((vin instanceof mkmat.Vertex) && (vout instanceof mkmat.Vertex)) {
    ans=mkuni.good;
    var dim=this.matrix.length,ii=0,jj=0,vv=0;
    var vvin=[
      isNaN(vin.val(0)) ? 0 : vin.val(0),isNaN(vin.val(1)) ? 0 : vin.val(1),
      isNaN(vin.val(2)) ? (dim==3 ? 1 : 0) : vin.val(2),isNaN(vin.val(3)) ? 1 : vin.val(3)
    ];
    for (ii=0;ii<dim;ii++) {
      for (jj=0;jj<dim;jj++) {
        vv+=vvin[jj]*this.matrix[ii][jj];
      }
      vout.setVal(ii,vv);
      vv=0;
    }
  }
  return ans;
};
/* ########## */

