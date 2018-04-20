
var mkcalc={};

mkcalc.fmt_toggle='toggle';
mkcalc.fmt_deg='deg'; mkcalc.fmt_rad='rad';
mkcalc.fmt_defsci=7;
mkcalc.fmt_direct='direct'; mkcalc.fmt_invert='invert';
mkcalc.fmt_addaction=2048; mkcalc.fmt_multaction=4096;
mkcalc.fmt_action=6144; mkcalc.fmt_unaction=8192;

mkcalc.fn_noop='noop';
// buttons
mkcalc.fn_inv='inv'; mkcalc.fn_memrcl='memrcl'; mkcalc.fn_memput='memput';
mkcalc.fn_clear='clear'; mkcalc.fn_clearall='clearall'; mkcalc.fn_euler='euler';
mkcalc.fn_reci='reci'; mkcalc.fn_sqr='sqr'; mkcalc.fn_pow='pow';
mkcalc.fn_fac='fac'; mkcalc.fn_sqrt='sqrt'; mkcalc.fn_pi='pi';
mkcalc.fn_sin='sin'; mkcalc.fn_cos='cos'; mkcalc.fn_tan='tan';
mkcalc.fn_ln='ln'; mkcalc.fn_log='log'; mkcalc.fn_exp='exp';
mkcalc.fn_7='7'; mkcalc.fn_8='8'; mkcalc.fn_9='9';
mkcalc.fn_A='A'; mkcalc.fn_B='B'; mkcalc.fn_plus='plus';
mkcalc.fn_4='4'; mkcalc.fn_5='5'; mkcalc.fn_6='6';
mkcalc.fn_C='C'; mkcalc.fn_D='D'; mkcalc.fn_minus='minus';
mkcalc.fn_1='1'; mkcalc.fn_2='2'; mkcalc.fn_3='3';
mkcalc.fn_E='E'; mkcalc.fn_F='F'; mkcalc.fn_mult='mult';
mkcalc.fn_0='0'; mkcalc.fn_decsep='decsep'; mkcalc.fn_chsgn='chsgn';
mkcalc.fn_parenl='parenl'; mkcalc.fn_parenr='parenr'; mkcalc.fn_div='div';
mkcalc.fn_equals='equals';
// fn-invert
mkcalc.fn_invpow='invpow'; mkcalc.fn_asin='asin'; mkcalc.fn_acos='acos';  mkcalc.fn_atan='atan';
// trig
mkcalc.fn_deg='deg'; mkcalc.fn_rad='rad';

/* ********** */
mkcalc.keycode2fnnumber=function(keycode) {
  var ans='';
  keycode=mkuni.chknumber(keycode);
  if (isNaN(keycode))
    return ans;
  if (keycode==mkuni.asciizero)
    ans=mkcalc.fn_0;
  else if (keycode==mkuni.asciione)
    ans=mkcalc.fn_1;
  else if (keycode==mkuni.asciitwo)
    ans=mkcalc.fn_2;
  else if (keycode==mkuni.asciithree)
    ans=mkcalc.fn_3;
  else if (keycode==mkuni.asciifour)
    ans=mkcalc.fn_4;
  else if (keycode==mkuni.asciifive)
    ans=mkcalc.fn_5;
  else if (keycode==mkuni.asciisix)
    ans=mkcalc.fn_6;
  else if (keycode==mkuni.asciiseven)
    ans=mkcalc.fn_7;
  else if (keycode==mkuni.asciieight)
    ans=mkcalc.fn_8;
  else if (keycode==mkuni.asciinine)
    ans=mkcalc.fn_9;
  else if (keycode==mkuni.asciiA || keycode==mkuni.asciia)
    ans=mkcalc.fn_A;
  else if (keycode==mkuni.asciiB || keycode==mkuni.asciib)
    ans=mkcalc.fn_B;
  else if (keycode==mkuni.asciiC || keycode==mkuni.asciic)
    ans=mkcalc.fn_C;
  else if (keycode==mkuni.asciiD || keycode==mkuni.asciid)
    ans=mkcalc.fn_D;
  else if (keycode==mkuni.asciiE || keycode==mkuni.asciie)
    ans=mkcalc.fn_E;
  else if (keycode==mkuni.asciiF || keycode==mkuni.asciif)
    ans=mkcalc.fn_F;
  return ans;
};

/* ********** */
mkcalc.isfnnumber=function(fnnum) {
  fnnum=mkuni.chkstr(fnnum);
  if (fnnum.length==0)
    return mkuni.bad;
  if (fnnum==mkcalc.fn_decsep || fnnum==mkcalc.fn_0 || fnnum==mkcalc.fn_1 ||
      fnnum==mkcalc.fn_2 || fnnum==mkcalc.fn_3 || fnnum==mkcalc.fn_4 ||
      fnnum==mkcalc.fn_5 || fnnum==mkcalc.fn_6 || fnnum==mkcalc.fn_7 ||
      fnnum==mkcalc.fn_8 || fnnum==mkcalc.fn_9 || fnnum==mkcalc.fn_A ||
      fnnum==mkcalc.fn_B || fnnum==mkcalc.fn_C || fnnum==mkcalc.fn_D ||
      fnnum==mkcalc.fn_E || fnnum==mkcalc.fn_F)
    return mkuni.good;
  return mkuni.bad;
};

/* ********** */
mkcalc.isfnoperator=function(info) {
  info=mkuni.chkstr(info);
  if (info.length==0)
    return mkuni.bad;
  if (info==mkcalc.fn_cos || info==mkcalc.fn_div || info==mkcalc.fn_fac ||
      info==mkcalc.fn_ln || info==mkcalc.fn_log || info==mkcalc.fn_minus ||
      info==mkcalc.fn_mult || info==mkcalc.fn_plus || info==mkcalc.fn_pow ||
      info==mkcalc.fn_reci || info==mkcalc.fn_sin || info==mkcalc.fn_sqr ||
      info==mkcalc.fn_sqrt || info==mkcalc.fn_tan)
    return mkuni.good;
  return mkuni.bad;
};

/* ********** */
mkcalc.isfnextranumber=function(info) {
  info=mkuni.chkstr(info);
  if (info.length==0)
    return mkuni.bad;
  if (info==mkcalc.fn_exp || info==mkcalc.fn_euler || info==mkcalc.fn_pi)
    return mkuni.good;
};

/* ##### Entry ##### */
mkcalc.Entry=function(sstr,ioption,sinvstr) {
  this.str=mkuni.chkstr(sstr);
  this.option=Math.floor(mkuni.chknumber(ioption,0));
  this.invstr=mkuni.chkstr(sinvstr);
};

/* ***** Entry ***** */
mkcalc.Entry.prototype.invert=function() {
  if (this.invstr.length>0) {
    var tmp=this.str;
    this.str=this.invstr;
    this.invstr=tmp;
  }
};

/* ***** Entry ***** */
mkcalc.Entry.prototype.toJson=function(indent) {
  var nextindent='  ',xans='',ans='';
  if (typeof indent=='string')
    nextindent+=indent;
  ans+=aux.qdecl('str',nextindent);
  ans+=aux.qword(this.str);
  ans+=aux.qnextprop(1,indent);
  ans+=aux.qdecl('option',nextindent);
  ans+=aux.qword(mkint.int2str(this.option));
  ans+=aux.qnextprop(1,indent);
  ans+=aux.qdecl('invstr',nextindent);
  ans+=aux.qword(this.invstr);
  xans+=aux.qattr(ans,1,indent);
  return xans;
};

/* ********** */
mkcalc.chkentry=function(entry,len) {
  var ans=mkuni.bad;
  if ((typeof entry=='undefined') || !(entry instanceof mkcalc.Entry))
    return ans;
  len=mkuni.chknumber(len);
  if (isNaN(len) || len<entry.str.length)
    ans=mkuni.good;
  return ans;
};

mkcalc.actionL=[
  new mkcalc.Entry(mkcalc.fn_plus,mkcalc.fmt_addaction,mkcalc.fn_minus),
  new mkcalc.Entry(mkcalc.fn_minus,mkcalc.fmt_addaction,mkcalc.fn_plus),
  new mkcalc.Entry(mkcalc.fn_mult,mkcalc.fmt_multaction,mkcalc.fn_div),
  new mkcalc.Entry(mkcalc.fn_div,mkcalc.fmt_multaction,mkcalc.fn_mult),
  new mkcalc.Entry(mkcalc.fn_pow,mkcalc.fmt_multaction,mkcalc.fn_invpow),
  new mkcalc.Entry(mkcalc.fn_invpow,mkcalc.fmt_unaction,mkcalc.fn_pow),
  new mkcalc.Entry(mkcalc.fn_sqrt,mkcalc.fmt_unaction,mkcalc.fn_sqr),
  new mkcalc.Entry(mkcalc.fn_sqr,mkcalc.fmt_unaction,mkcalc.fn_sqrt),
  new mkcalc.Entry(mkcalc.fn_sin,mkcalc.fmt_unaction,mkcalc.fn_asin),
  new mkcalc.Entry(mkcalc.fn_asin,mkcalc.fmt_unaction,mkcalc.fn_sin),
  new mkcalc.Entry(mkcalc.fn_cos,mkcalc.fmt_unaction,mkcalc.fn_acos),
  new mkcalc.Entry(mkcalc.fn_acos,mkcalc.fmt_unaction,mkcalc.fn_cos),
  new mkcalc.Entry(mkcalc.fn_tan,mkcalc.fmt_unaction,mkcalc.fn_atan),
  new mkcalc.Entry(mkcalc.fn_atan,mkcalc.fmt_unaction,mkcalc.fn_tan),
  new mkcalc.Entry(mkcalc.fn_ln,mkcalc.fmt_unaction,mkcalc.fn_powe),
  new mkcalc.Entry(mkcalc.fn_powe,mkcalc.fmt_unaction,mkcalc.fn_ln),
  new mkcalc.Entry(mkcalc.fn_log,mkcalc.fmt_unaction,mkcalc.fn_pow10),
  new mkcalc.Entry(mkcalc.fn_pow10,mkcalc.fmt_unaction,mkcalc.fn_log),
  new mkcalc.Entry(mkcalc.fn_noop,mkcalc.fmt_unaction,mkcalc.fn_reci),
  new mkcalc.Entry(mkcalc.fn_reci,mkcalc.fmt_unaction,mkcalc.fn_noop),
  new mkcalc.Entry(mkcalc.fn_fac,mkcalc.fmt_unaction),
  new mkcalc.Entry(mkcalc.fn_chsgn,mkcalc.fmt_unaction)
];

/* ********** */
mkcalc.cmpentry=function(entry1,entry2) {
  var ans=entry1.str.localeCompare(entry2.str);
  return (ans<0 ? -1 : (ans>0 ? 1 : 0));
};

/* ********** */
mkcalc.isaction=function(entry) {
  var ans=0;
  if (mkcalc.chkentry(entry,0)==mkuni.good) {
    var idx=mkuni.linsearch(entry,mkcalc.actionL,mkcalc.cmpentry);
    if (idx>=0)
      ans=mkcalc.actionL[idx].option;
  }
  return ans;
};
/* ########## */

/* ********** */
mkcalc.calcieee754=function(entry,num1,num2) {
  var ans=NaN;
  var action=mkcalc.isaction(entry);
  if (action==0)
    return ans;
  if (isNaN(mkuni.chknumber(num1)))
    return ans;
  if (action!=mkcalc.fmt_unaction) {
    if (isNaN(mkuni.chknumber(num2)))
      return ans;
  }
  if (entry.str==mkcalc.fn_plus)
    ans=num1+num2;
  else if (entry.str==mkcalc.fn_minus)
    ans=num1-num2;
  else if (entry.str==mkcalc.fn_mult)
    ans=num1*num2;
  else if (entry.str==mkcalc.fn_div)
    ans=num1/num2;
  else if (entry.str==mkcalc.fn_pow)
    ans=Math.pow(num1,num2);
  else if (entry.str==mkcalc.fn_invpow)
    ans=1./Math.pow(num1,num2);
  else if (entry.str==mkcalc.fn_sqrt)
    ans=Math.sqrt(num1);
  else if (entry.str==mkcalc.fn_sqr)
    ans=num1*num1;
  else if (entry.str==mkcalc.fn_sin)
    ans=Math.sin(num1);
  else if (entry.str==mkcalc.fn_asin)
    ans=Math.asin(num1);
  else if (entry.str==mkcalc.fn_cos)
    ans=Math.cos(num1);
  else if (entry.str==mkcalc.fn_acos)
    ans=Math.acos(num1);
  else if (entry.str==mkcalc.fn_tan)
    ans=Math.tan(num1);
  else if (entry.str==mkcalc.fn_atan)
    ans=Math.atan(num1);
  else if (entry.str==mkcalc.fn_ln)
    ans=Math.log(num1);
  else if (entry.str==mkcalc.fn_powe)
    ans=Math.exp(num1);
  else if (entry.str==mkcalc.fn_log)
    ans=Math.log10(num1);
  else if (entry.str==mkcalc.fn_pow10)
    ans=Math.pow(10,num1);
  else if (entry.str==mkcalc.fn_fac) {
    var inum=Math.floor(num1);
    ans=inum;
    while (inum>1) {
      inum--;
      ans*=inum;
    }
  }
  else if (entry.str==mkcalc.fn_noop)
    ans=num1;
  else if (entry.str==mkcalc.fn_reci)
    ans=1./num1;
  else if (entry.str==mkcalc.fn_chsgn)
    ans=-num1;
  return ans;
};

/* ##### Calculator ##### */
mkcalc.Calculator=function(cbout,cbinfo,cbbut,cbtrig,cbrd,cbsci,cbscale) {
  this.numfmt=10;
  this.scifmt=mkcalc.fmt_defsci;
  this.opfmt=mkcalc.fmt_direct;
  this.trigfmt=mkcalc.fmt_deg;
  this.maxdig=mkfixed.maxlen;
  this.rddig=mkfixed.defrd;
  this.input='';
  this.output='';
  this.info='';
  this.showinfo='';
  this.store=[];
  this.ops=[];
  this.paren=[];
  this.outcb=cbout;
  this.infocb=cbinfo;
  this.butcb=cbbut;
  this.trigcb=cbtrig;
  this.rdcb=cbrd;
  this.scicb=cbsci;
  this.scalecb=cbscale;
  if (typeof this.butcb=='function')
    this.butcb();
  if (typeof this.trigcb=='function')
    this.trigcb(this.trigfmt);
  if (typeof this.rdcb=='function')
    this.rdcb(this.rddig);
  if (typeof this.scicb=='function')
    this.scicb(this.scifmt);
  if (typeof this.scalecb=='function')
    this.scalecb(this.maxdig);
};

/* ***** Calculator ***** */
mkcalc.Calculator.prototype.opsdbg=function() {
  var ans='';
  var ii=0;
  for (ii=0;ii<this.ops.length;ii++)
    ans+=this.ops[ii].toJson();
  return ans;
};

/* ***** Calculator ***** */
mkcalc.Calculator.prototype.setinput=function(inp) {
  this.input=mkuni.chkstr(inp);
  return this.input;
};

/* ***** Calculator ***** */
mkcalc.Calculator.prototype.setinfo=function(inf) {
  this.info=mkuni.chkstr(inf);
  return this.info;
};

/* ***** Calculator ***** */
mkcalc.Calculator.prototype.updateoutput=function(msg) {
  msg=mkuni.chkstr(msg);
  if (msg.length==0)
    msg=this.input;
  if (msg.length==0)
    msg=mkuni.str4code(mkuni.asciizero);
  this.output=msg;
  if (typeof this.outcb=='function')
    this.outcb(this.output);
  else
    console.log(new Error().lineNumber+' ['+msg+']');
  return mkuni.good;
};

/* ***** Calculator ***** */
mkcalc.Calculator.prototype.updateinfo=function(msg) {
  msg=mkuni.chkstr(msg);
  if (msg.length==0)
    msg=this.info;
  if (typeof this.infocb=='function')
    this.infocb(msg);
  else
    console.log(new Error().lineNumber+' ['+msg+']');
  return msg;
};

/* ***** Calculator ***** */
mkcalc.Calculator.prototype.errcomp=function(msg) {
  this.ops=[];
  this.paren=[];
  this.trigfmt=mkcalc.fmt_deg;
  //this.rddig=mkfixed.defrd;
  this.scifmt=mkcalc.fmt_defsci;
  this.opfmt=mkcalc.fmt_direct;
  this.input='';
  this.output='';
  this.info='';
  msg=mkuni.chkstr(msg);
  if (msg.length==0)
    msg='Err';
  this.updateoutput(msg);
  this.updateinfo();
  if (typeof this.butcb=='function')
    this.butcb();
  if (typeof this.trigcb=='function')
    this.trigcb(this.trigfmt);
  if (typeof this.rdcb=='function')
    this.rdcb(this.rddig);
  if (typeof this.scicb=='function')
    this.scicb(this.scifmt);
  return mkuni.good;
};

/* ***** Calculator ***** */
mkcalc.Calculator.prototype.clean=function() {
  this.ops=[];
  this.paren=[];
  this.trigfmt=mkcalc.fmt_deg;
  //this.rddig=mkfixed.defrd;
  this.scifmt=mkcalc.fmt_defsci;
  this.opfmt=mkcalc.fmt_direct;
  this.input='';
  this.output='';
  this.info='';
  this.updateoutput();
  this.updateinfo();
  if (typeof this.butcb=='function')
    this.butcb(this.numfmt);
  if (typeof this.trigcb=='function')
    this.trigcb(this.trigfmt);
  if (typeof this.rdcb=='function')
    this.rdcb(this.rddig);
  if (typeof this.scicb=='function')
    this.scicb(this.scifmt);
  return mkuni.good;
};

/* ***** Calculator ***** */
mkcalc.Calculator.prototype.clear=function() {
  this.setinput();
  this.updateoutput();
  return mkuni.good;
};

/* ***** Calculator ***** */
mkcalc.Calculator.prototype.chkinfo=function(msg) {
  msg=mkuni.chkstr(msg);
  var infostr=(msg.length==0 ? '' : msg);
  infostr+=' ';
  if ((this.chkopfmt(0)&this.fmt_invert)>0)
    infostr+='Inv ';
  if (this.store.length>0)
    infostr+='Mem ';
  var ii=0,cnt=this.paren.length;
  if (cnt>0)
    infostr+='(';
  for (ii=1;ii<this.paren.length;ii++)
    infostr+=',(';
  var ans=this.setinfo(infostr);
  return ans;
};

/* ***** Calculator ***** */
mkcalc.Calculator.prototype.chktrigfmt=function(fmt) {
  fmt=mkuni.chkstr(fmt);
  if (fmt.length==0)
    return this.trigfmt;
  if (fmt==this.trigfmt)
    return this.trigfmt;
  var ff=this.trigfmt;
  if (fmt==mkcalc.fmt_deg || fmt==mkcalc.fmt_rad)
    this.trigfmt=fmt;
  if (typeof this.trigcb=='function')
    this.trigcb(this.trigfmt);
  return ff;
};

/* ***** Calculator ***** */
mkcalc.Calculator.prototype.chkopfmt=function(fmt) {
  fmt=mkuni.chkstr(fmt);
  if (fmt.length==0)
    return this.opfmt;
  if (fmt==this.opfmt)
    return this.opfmt;
  var ff=this.opfmt;
  if (fmt==mkcalc.fmt_toggle)
    fmt=(ff==mkcalc.fmt_direct ? mkcalc.fmt_invert : mkcalc.fmt_direct);
  if (fmt==mkcalc.fmt_direct || fmt==mkcalc.fmt_invert)
    this.opfmt=fmt;
  return ff;
};

/* ***** Calculator ***** */
mkcalc.Calculator.prototype.setop=function(entry,idx) {
  var newentry=new mkcalc.Entry();
  if (mkcalc.chkentry(entry)==mkuni.bad) {
    entry=mkuni.chkstr(entry);
    if (entry.length==0)
      return mkuni.bad;
    newentry=new mkcalc.Entry(entry,this.numfmt);
  }
  else
    newentry=new mkcalc.Entry(entry.str,entry.option,entry.invstr);
  idx=mkuni.chknumber(idx,this.ops.length);
  if (idx>=0 && idx<this.ops.length)
    this.ops[idx]=newentry;
  else
    this.ops.push(newentry);
  return mkuni.good;
};

/* ***** Calculator ***** */
mkcalc.Calculator.prototype.getop=function(idx,entry) {
  idx=mkuni.chknumber(idx,this.ops.length);
  if (idx<0 || idx>=this.ops.length)
    return mkuni.bad;
  if (typeof entry=='undefined' || !(entry instanceof mkcalc.Entry))
    return mkuni.bad;
  var ee=this.ops[idx];
  entry.str=ee.str;
  entry.option=ee.option;
  entry.invstr=ee.invstr;
  return mkuni.good;
};

/* ***** Calculator ***** */
mkcalc.Calculator.prototype.chknumfmt=function(fmt) {
  fmt=mkint.chkbase(fmt,this.numfmt);
  if (fmt==this.numfmt)
    return this.numfmt;
  var strinput=this.input;
  var idx=this.ops.length-1;
  var fp=new mkfixed.FixedPoint();
  var entry=new mkcalc.Entry();
  if (strinput.length>0) {
    fp=new mkfixed.FixedPoint(strinput);
    if (fp.unmix(this.numfmt)!=this.numfmt) {
      this.errcomp('err : format mismatch');
      return this.numfmt;
    }
    if (fp.convert(fmt,this.maxdig)!=fmt) {
      this.errcomp('err : cannot convert to integer');
      return this.numfmt;
    }
    strinput=fp.toStringI();
    this.setinput(strinput);
  }
  else if (this.getop(idx,entry)==mkuni.good) {
    var entryaction=mkcalc.isaction(entry);
    if ((entryaction&mkcalc.fmt_action)==0 && (entryaction&mkcalc.fmt_unaction)==0) {
      fp=new mkfixed.FixedPoint(entry.str);
      if (fp.unmix(this.numfmt)!=this.numfmt) {
        this.errcomp('err : format mismatch');
        return this.numfmt;
      }
      if (fp.convert(fmt,this.maxdig)!=fmt) {
        this.errcomp('err : cannot convert to integer');
        return this.numfmt;
      }
      strinput=fp.toStringI();
      entry.str=strinput;
      entry.option=fmt;
      this.setop(entry,idx);
    }
  }
  this.numfmt=fmt;
  this.updateoutput(strinput);
  if (typeof this.butcb=='function')
    this.butcb(this.numfmt);
  return this.numfmt;
};

/* ***** Calculator ***** */
mkcalc.Calculator.prototype.chkrdpos=function(digs) {
  var dd=this.rddig;
  digs=mkuni.chknumber(digs);
  if (isNaN(digs))
    return dd;
  if (digs>=this.maxdig)
    digs=this.maxdig-1;
  if (digs==dd)
    return dd;
  this.rddig=digs;
  if (this.numfmt==10 && this.output.length>0) {
    var fp=new mkfixed.FixedPoint(this.output);
    var bb=fp.unmix();
    if (bb==0)
      this.errcomp('err');
    else if (bb==1)
      this.updateoutput(mkuni.str4code(mkuni.asciizero));
    else
      this.updateoutput(fp.roundpos(this.rddig).toStringF(this.scifmt,this.maxdig));
  }
  if (typeof this.rdcb=='function')
    this.rdcb(this.rddig);
  return this.rddig;
};

/* ***** Calculator ***** */
mkcalc.Calculator.prototype.chkscifmt=function(isci) {
  isci=mkuni.chknumber(isci);
  if (isNaN(isci) || isci<0 || this.scifmt==isci)
    return this.scifmt;
  this.scifmt=isci;
  if (this.numfmt==10 && this.output.length>0) {
    var fp=new mkfixed.FixedPoint(this.output);
    var bb=fp.unmix();
    if (bb==0)
      this.errcomp('err');
    else if (bb==1)
      this.updateoutput(mkuni.str4code(mkuni.asciizero));
    else
      this.updateoutput(fp.toStringF(this.scifmt,this.maxdig));
  }
  return this.scifmt;
};

/* ***** Calculator ***** */
mkcalc.Calculator.prototype.chkscale=function(digs) {
  var dd=this.maxdig;
  if (mkint.chknumberrange(digs,1,mkfixed.maxmaxlen)==mkuni.bad)
    digs=dd;
  if (digs==dd)
    return dd;
  if (digs<=this.rddig)
    this.chkrdpos(digs-1);
  this.maxdig=digs;
  if (typeof this.scalecb=='function')
    this.scalecb(this.maxdig);
  return this.maxdig;
};

/* ***** Calculator ***** */
mkcalc.Calculator.prototype.validatehist=function() {
  var ecurr=new mkcalc.Entry();
  var enext=new mkcalc.Entry();
  if (this.getop(0,ecurr)==mkuni.bad)
    return mkuni.bad;
  var numL=[];
  var opL=[];
  var ii=0,opscnt=this.ops.length;
  if (opscnt==1) {
    if ((mkcalc.isaction(ecurr)&mkcalc.fmt_action)>0)
      opL.push(new mkcalc.Entry(ecurr.str,ecurr.option,ecurr.invstr));
    else
      numL.push(new mkcalc.Entry(ecurr.str,ecurr.option,ecurr.invstr));
  }
  for (ii=0;ii<(opscnt-1);ii++) {
    this.getop(ii,ecurr);
    this.getop(ii+1,enext);
    if (ecurr.str.length>0) {
      if ((mkcalc.isaction(ecurr)&mkcalc.fmt_action)==0) {
        if ((mkcalc.isaction(enext)&mkcalc.fmt_action)==0) {
          numL=[];
          opL=[];
        }
        else
          numL.push(new mkcalc.Entry(ecurr.str,ecurr.option,ecurr.invstr));
      }
      else {
        if (ii==0)
          numL.push(new mkcalc.Entry(mkuni.str4code(mkuni.asciizero),this.numfmt));
        if ((mkcalc.isaction(enext)&mkcalc.fmt_action)==0)
          opL.push(new mkcalc.Entry(ecurr.str,ecurr.option,ecurr.invstr));
      }
    }
    if (ii==(opscnt-2) && enext.str.length>0) {
      if ((mkcalc.isaction(enext)&mkcalc.fmt_action)==0)
        numL.push(new mkcalc.Entry(enext.str,enext.option,enext.invstr));
      else
        opL.push(new mkcalc.Entry(enext.str,enext.option,enext.invstr));
    }
  }
  this.ops=[];
  if (numL.length==0 && opL.length==0)
    return mkuni.bad;
  if (numL.length==0 || opL.length==0) {
    ecurr=(numL.length==0 ? opL[0] : numL[0]);
    if (ecurr.str.length>0) {
      if (numL.length==0)
        this.setop(mkuni.str4code(mkuni.asciizero));
      this.setop(ecurr);
    }
    return mkuni.bad;
  }
  while (numL.length<3)
    numL.push(new mkcalc.Entry());
  while (opL.length<3)
    opL.push(new mkcalc.Entry());
  if (numL[0].str.length==0)
    return mkuni.bad;
  if (opL[0].str.length==0) {
    this.setop(numL[0]);
    return mkuni.bad;
  }
  else if (numL[1].str.length==0) {
    this.setop(numL[0]);
    this.setop(opL[0]);
    return mkuni.bad;
  }
  if (opL[1].str.length==0) {
    this.setop(numL[0]);
    this.setop(opL[0]);
    if (numL[1].str.length>0)
      this.setop(numL[1]);
    return mkuni.bad;
  }
  else if (numL[1].str.length==0) {
    this.setop(numL[0]);
    this.setop(opL[0]);
    return mkuni.bad;
  }
  this.setop(numL[0]);
  this.setop(opL[0]);
  this.setop(numL[1]);
  this.setop(opL[1]);
  if (numL[2].str.length>0)
    this.setop(numL[2]);
  if ((mkcalc.isaction(opL[0])&mkcalc.fmt_addaction)>0 &&
      (mkcalc.isaction(opL[1])&mkcalc.fmt_multaction)>0 && opL[2].str.length>0)
    return mkuni.bad;
  if (opL[2].str.length>0)
    this.setop(opL[2]);
  return mkuni.good;
};

/* ***** Calculator ***** */
mkcalc.Calculator.prototype.calcunary=function(unop) {
  var ans=mkuni.bad;
  if ((mkcalc.isaction(unop)&mkcalc.fmt_unaction)==0)
    return ans;
  var einp=new mkcalc.Entry(this.input,this.numfmt);
  if (einp.str.length==0) {
    this.validatehist();
    var opscnt=this.ops.length;
    while (opscnt>0) {
      this.getop(--opscnt,einp);
      if (einp.str.length>0 && (mkcalc.isaction(einp)&mkcalc.fmt_action)==0)
        break;
    }
    if (einp.str.length==0 || (mkcalc.isaction(einp)&mkcalc.fmt_action)>0)
      return ans;
    einp.option=mkint.chkbase(einp.option);
  }
  var fp=new mkfixed.FixedPoint(einp.str);
  if (fp.unmix(einp.option)==0)
    return ans;
  fp.convert(10,this.maxdig);
  var msg='';
  if (fp.sgn==mkuni.asciiminus) {
    if (unop.str==mkcalc.fn_ln || unop.str==mkcalc.fn_log || unop.str==mkcalc.fn_sqrt)
      msg='err : cannot compute transcendent number';
    else if (unop.str==mkcalc.fn_fac)
      msg='err : input must be positive number';
  }
  if (msg.length==0 && fp.base==1) {
    if (unop.str==mkcalc.fn_ln || unop.str==mkcalc.fn_log)
      msg='err : cannot compute transcendent number';
    else if (unop.str==mkcalc.fn_reci || unop.str==mkcalc.fn_fac)
      msg='err : input must not be \"0\"';
  }
  if (msg.length==0 && fp.isInt()==0) {
    if (unop.str==mkcalc.fn_fac)
      msg='err : input must be integer';
  }
  if (msg.length>0) {
    this.errcomp(msg);
    return ans;
  }
  var domult=mkuni.str4code(mkuni.asciimult);
  var dodiv=mkuni.str4code(mkuni.asciidiv);
  var dosubs=mkuni.str4code(mkuni.asciiminus);
  var trignum=(this.trigfmt==mkcalc.fmt_rad ? mkuni.str4code(mkuni.asciione) : mkfloat.rad);
  var strffp=fp.toString();
  if (unop.str==mkcalc.fn_sin || unop.str==mkcalc.fn_cos || unop.str==mkcalc.fn_tan)
    strffp=mkfixed.calculate(domult,strffp,trignum,this.maxdig);
  var ffp=parseFloat(strffp);
  var ieeecalc=mkcalc.calcieee754(unop,ffp);
  var strcalc='';
  if (unop.str==mkcalc.fn_noop)
    strcalc=strffp;
  else if (unop.str==mkcalc.fn_sqr) {
    strcalc=mkfixed.calculate(domult,strffp,strffp,this.maxdig);
  }
  else if (unop.str==mkcalc.fn_asin || unop.str==mkcalc.fn_acos || unop.str==mkcalc.fn_atan) {
    strcalc=mkfixed.calculate(dodiv,ieeecalc.toString(),trignum,this.maxdig);
    var rdfp=new mkfixed.FixedPoint(strcalc);
    strcalc=rdfp.roundpos(mkfloat.maxieee).toString();
  }
  else if ((unop.str==mkcalc.fn_powe || unop.str==mkcalc.fn_pow10) && fp.isInt()>1) {
    var fpsgn=fp.toAbs();
    var strprov=fp.toStringI();
    if (mkfixed.cmpstr10(strprov,mkuni.str4code(mkuni.asciizero))==0)
      strcalc=mkuni.str4code(mkuni.asciione);
    else {
      var baseop=(unop.str==mkcalc.fn_powe ? mkfloat.euler : mkuni.strten);
      strcalc=baseop;
      while (mkfixed.cmpstr10(strprov,mkuni.str4code(mkuni.asciizero))==1) {
        strcalc=mkfixed.calculate(domult,strcalc,baseop,this.maxdig);
        strprov=mkfixed.calculate(dosubs,strprov,mkuni.str4code(mkuni.asciione),this.maxdig);
      }
    }
    if (fpsgn==mkuni.asciiminus)
      strcalc=mkfixed.calculate(dodiv,mkuni.str4code(mkuni.asciione),strcalc,this.maxdig);
  }
  else if (unop.str==mkcalc.fn_fac) {
    var strprov=fp.toStringI();
    strcalc=strprov;
    while (mkfixed.cmpstr10(strprov,mkuni.str4code(mkuni.asciione))==1) {
      strprov=mkfixed.calculate(dosubs,strprov,mkuni.str4code(mkuni.asciione),this.maxdig);
      strcalc=mkfixed.calculate(domult,strcalc,strprov,this.maxdig);
    }
  }
  else if (unop.str==mkcalc.fn_reci) {
    strcalc=mkfixed.calculate(dodiv,mkuni.str4code(mkuni.asciione),fp.toString(),this.maxdig);
  }
  else if (unop.str==mkcalc.fn_chsgn) {
    fp.sgn=(fp.sgn==0 ? mkuni.asciiminus : 0);
    strcalc=fp.toString();
  }
  if (strcalc.length==0) {
    if (this.rddig<=mkfloat.maxieee)
      ieeecalc=ieeecalc.toFixed(this.rddig);
    else if (this.maxdig<=mkfloat.maxieee)
      ieeecalc=ieeecalc.toFixed(this.maxdig);
    strcalc=ieeecalc.toString();
    var info=this.chkinfo('ieee754');
    this.updateinfo(info);
  }
  fp=new mkfixed.FixedPoint(strcalc);
  if (fp.unmix()==0)
    return ans;
  ans=mkuni.good;
  fp=fp.roundpos(this.rddig);
  strcalc=fp.toStringF(this.scifmt,this.maxdig);
  if (this.numfmt!=10) {
    if (fp.isInt()==0) {
      this.numfmt=10;
      if (typeof this.butcb=='function')
        this.butcb();
    }
    else {
      if (fp.convert(this.numfmt)==0) {
        this.errcomp('err : format mismatch');
        return ans;
      }
      strcalc=fp.toStringI();
    }
  }
  this.setinput();
  this.setop(new mkcalc.Entry(strcalc));
  this.updateoutput(strcalc);
  return ans;
};

/* ***** Calculator ***** */
mkcalc.Calculator.prototype.compute=function(num1,num2,action) {
  var ans=new mkcalc.Entry();
  if (mkcalc.chkentry(num1,0)==mkuni.bad ||
      mkcalc.chkentry(num2,0)==mkuni.bad ||
      mkcalc.isaction(action)==0)
    return ans;
  var fp1=new mkfixed.FixedPoint(num1.str);
  var fp2=new mkfixed.FixedPoint(num2.str);
  if (fp1.unmix(num1.option)==0 || fp2.unmix(num2.option)==0)
    return ans;
  fp1.convert(10,this.maxdig);
  fp2.convert(10,this.maxdig);
  var doadd=mkuni.str4code(mkuni.asciiplus);
  var dosubs=mkuni.str4code(mkuni.asciiminus);
  var domult=mkuni.str4code(mkuni.asciimult);
  var dodiv=mkuni.str4code(mkuni.asciidiv);
  var strffp1=fp1.toString(),strffp2=fp2.toString();
  var ffp1=parseFloat(strffp1),ffp2=parseFloat(strffp2);
  var ieeecalc=mkcalc.calcieee754(action,ffp1,ffp2);
  var strcalc='';
  if (action.str==mkcalc.fn_plus)
    strcalc=mkfixed.calculate(doadd,strffp1,strffp2,this.maxdig);
  else if (action.str==mkcalc.fn_minus)
    strcalc=mkfixed.calculate(dosubs,strffp1,strffp2,this.maxdig);
  else if (action.str==mkcalc.fn_mult)
    strcalc=mkfixed.calculate(domult,strffp1,strffp2,this.maxdig);
  else if (action.str==mkcalc.fn_div)
    strcalc=mkfixed.calculate(dodiv,strffp1,strffp2,this.maxdig);
  else if (fp1.isInt()>1 && fp2.isInt()>1) {
    if (action.str==mkcalc.fn_pow) {
      var fpsgn2=fp2.toAbs();
      var strfp1=fp1.toStringI(),strfp2=fp2.toStringI();
      strcalc=strfp1;
      while (mkfixed.cmpstr10(strfp2,mkuni.str4code(mkuni.asciione))==1) {
        strcalc=mkfixed.calculate(domult,strcalc,strfp1,this.maxdig);
        strfp2=mkfixed.calculate(dosubs,strfp2,mkuni.str4code(mkuni.asciione),this.maxdig);
      }
      if (fpsgn2==mkuni.asciiminus)
        strcalc=mkfixed.calculate(dodiv,mkuni.str4code(mkuni.asciione),strcalc,this.maxdig);
    }
  }
  if (strcalc.length==0) {
    if (this.rddig<=mkfloat.maxieee)
      ieeecalc=ieeecalc.toFixed(this.rddig);
    else if (this.maxdig<=mkfloat.maxieee)
      ieeecalc=ieeecalc.toFixed(this.maxdig);
    strcalc=ieeecalc.toString();
    var info=this.chkinfo('ieee754');
    this.updateinfo(info);
  }
  var fp=new mkfixed.FixedPoint(strcalc);
  if (fp.unmix()==0)
    return ans;
  fp=fp.roundpos(this.rddig);
  ans.str=fp.toStringF(this.scifmt,this.maxdig);
  ans.option=10;
  if (this.numfmt!=10) {
    if (fp.isInt()==0) {
      this.numfmt=10;
      if (typeof this.butcb=='function')
        this.butcb();
    }
    else {
      if (fp.convert(this.numfmt)==0) {
        this.errcomp('err : format mismatch');
        return ans;
      }
      ans.str=fp.toStringI();
      ans.option=this.numfmt;
    }
  }
  return ans;
};

/* ********** */
mkcalc.Calculator.prototype.updateaction=function() {
  var ans=this.validatehist();
  if (ans==mkuni.bad)
    return ans;
  if (this.ops.length<4)
    return mkuni.bad;
  var numcnt=0,opscnt=0;
  var num1=new mkcalc.Entry();
  var num2=new mkcalc.Entry();
  var num3=new mkcalc.Entry();
  var op1=new mkcalc.Entry();
  var op2=new mkcalc.Entry();
  var op3=new mkcalc.Entry();
  if (this.getop(0,num1)==mkuni.good)
    numcnt++;
  if (this.getop(1,op1)==mkuni.good)
    opscnt++;
  if (this.getop(2,num2)==mkuni.good)
    numcnt++;
  if (this.getop(3,op2)==mkuni.good)
    opscnt++;
  if (this.getop(4,num3)==mkuni.good)
    numcnt++;
  if (this.getop(5,op3)==mkuni.good)
    opscnt++;
  this.ops=[];
  if (numcnt<3) {
    num1=this.compute(num1,num2,op1);
    if (num1.str.length==0) {
      this.errcomp();
      return mkuni.bad;
    }
    this.setop(num1);
    this.setop(op2);
    this.updateoutput(num1.str);
    return mkuni.bad;
  }
  if (opcnt<3)
    return mkuni.bad;
  num2=this.compute(num2,num3,op2);
  if (num1.str.length==0) {
    this.errcomp();
    return mkuni.bad;
  }
  if (op3.str==mkcalc.fn_mult ||
      op3.str==mkcalc.fn_pow ||
      op3.str==mkcalc.fn_invpow ||
      op3.str==mkcalc.fn_div) {
    this.setop(num1);
    this.setop(op1);
    this.setop(num2);
    this.setop(op3);
    this.updateoutput(num2.str);
    return mkuni.bad;
  }
  num1=this.compute(num1,num2,op1);
  if (num1.str.length==0) {
    this.errcomp();
    return mkuni.bad;
  }
  this.setop(num1);
  this.setop(op3);
  this.updateoutput(num1.str);
  return mkuni.good;
};

/* ********** */
mkcalc.Calculator.prototype.equals=function(calcparen) {
  calcparen=mkuni.chknumber(calcparen,0);
  this.chkopfmt(mkcalc.fmt_direct);
  if (calcparen==0 && this.paren.length>0) {
    this.errcomp("err : unmatched paren");
    return mkuni.bad;
  }
  if (this.input.length>0)
    this.setop(new mkcalc.Entry(this.input,this.numfmt));
  this.setinput();
  this.validatehist();
  if (this.ops.length==0)
    return mkuni.bad;
  var numcnt=0,opscnt=0;
  var num1=new mkcalc.Entry();
  var num2=new mkcalc.Entry();
  var num3=new mkcalc.Entry();
  var op1=new mkcalc.Entry();
  var op2=new mkcalc.Entry();
  var op3=new mkcalc.Entry();
  if (this.getop(0,num1)==mkuni.good)
    numcnt++;
  if (this.getop(1,op1)==mkuni.good)
    opscnt++;
  if (this.getop(2,num2)==mkuni.good)
    numcnt++;
  if (this.getop(3,op2)==mkuni.good)
    opscnt++;
  if (this.getop(4,num3)==mkuni.good)
    numcnt++;
  if (this.getop(5,op3)==mkuni.good)
    opscnt++;
  this.ops=[];
  if (opscnt==0) {
    this.setop(num1);
    return mkuni.bad;
  }
  if (numcnt<3 || opcnt<2) {
    num1=this.compute(num1,numcnt<2 ? num1 : num2,op1);
    if (num1.str.length==0) {
      this.errcomp();
      return mkuni.bad;
    }
    this.setop(num1);
    this.updateoutput(num1.str);
    return mkuni.good;
  }
  num2=this.compute(num2,num3,op2);
  if (num2.str.length==0) {
    this.errcomp();
    return mkuni.bad;
  }
  num1=this.compute(num1,num2,op1);
  if (num1.str.length==0) {
    this.errcomp();
    return mkuni.bad;
  }
  this.setop(num1);
  this.updateoutput(num1.str);
  return mkuni.good;
};

/* ********** */
mkcalc.Calculator.prototype.chgsgn=function() {
  if (this.chkopfmt(mkcalc.fmt_direct)==mkcalc.fmt_invert)
    return mkuni.bad;
  var fp=new mkfixed.FixedPoint(this.input);
  if (fp.unmix()==0) {
    this.calcunary(fn_chsgn);
    return mkuni.good;
  }
  fp.sgn=(fp.sgn==mkuni.asciiminus ? 0 : mkuni.asciiminus);
  this.setinput(fp.toStringF(this.scifmt,this.maxdig));
  this.updateoutput();
  return mkuni.good;
};

/* ********** */
mkcalc.Calculator.prototype.pushstore=function() {
  var str=this.input;
  var lastop=new mkcalc.Entry();
  if (str.length==0 && this.getop(this.ops.length-1,lastop)==mkuni.good) {
    var entryaction=mkcalc.isaction(lastop);
    if ((entryaction&mkcalc.fmt_action)==0 && (entryaction&mkcalc.fmt_unaction)==0)
      str=lastop.str;
  }
  if (str.length>0)
    this.store.push(str);
  else
    this.store.pop();
  var ans=this.chkinfo();
  this.updateinfo(ans);
  return mkuni.good;
};

/* ********** */
mkcalc.Calculator.prototype.popstore=function() {
  this.setinput();
  var str=this.store.pop();
  this.setop(str);
  this.updateoutput(str);
  var ans=this.chkinfo();
  this.updateinfo(ans);
  return mkuni.good;
};

/* ********** */
mkcalc.Calculator.prototype.fstore=function(option) {
  option=mkuni.chkstr(option);
  if (option.length==0)
    return mkuni.bad;
  var fmt=this.chkopfmt(mkcalc.fmt_direct);
  var ff=mkuni.bad;
  if ((option==mkcalc.fn_memput && fmt==mkcalc.fmt_direct) ||
      (option==mkcalc.fn_memrcl && fmt==mkcalc.fmt_invert))
    ff=this.pushstore();
  if ((option==mkcalc.fn_memrcl && fmt==mkcalc.fmt_direct) ||
      (option==mkcalc.fn_memput && fmt==mkcalc.fmt_invert))
    ff=this.popstore();
  if (typeof ff=='function')
    return ff();
  return ff;
};

/* ********** */
mkcalc.Calculator.prototype.pushparen=function() {
  var fmt=this.chkopfmt(mkcalc.fmt_direct);
  if (fmt==mkcalc.fmt_invert)
    return this.popparen();
  this.validatehist();
  var entry=new mkcalc.Entry();
  if (this.getop(this.ops.length-1,entry)==mkuni.good) {
    var entryaction=mkcalc.isaction(entry);
    if ((entryaction&mkcalc.fmt_action)==0) {
      this.errcomp("err : prev must be operator");
      return mkuni.bad;
    }
  }
  this.paren.push(this.ops.slice());
  this.ops=[];
  this.setinput();
  var ans=this.chkinfo();
  this.updateinfo(ans);
  return mkuni.good;
};

/* ********** */
mkcalc.Calculator.prototype.popparen=function() {
  var fmt=this.chkopfmt(mkcalc.fmt_direct);
  if (fmt==mkcalc.fmt_invert)
    return this.pushparen();
  if (this.paren.length<1) {
    this.errcomp("unmatched paren");
    return mkuni.bad;
  }
  this.equals(1);
  var lastop=new mkcalc.Entry();
  if (this.getop(this.ops.length-1,lastop)==mkuni.bad) {
    this.errcomp(mkuni.str4code(mkuni.asciizero));
    return mkuni.bad;
  }
  this.ops=this.paren.pop().slice();
  this.ops.push(lastop);
  var ans=this.chkinfo();
  this.updateinfo(ans);
  return mkuni.good;
};

/* ********** */
mkcalc.Calculator.prototype.fparen=function(option) {
  option=mkuni.chkstr(option);
  if (option.length==0)
    return mkuni.bad;
  var fmt=this.chkopfmt(mkcalc.fmt_direct);
  var ff=mkuni.bad;
  if ((option==mkcalc.fn_parenl && fmt==mkcalc.fmt_direct) ||
      (option==mkcalc.fn_parenr && fmt==mkcalc.fmt_invert))
    ff=this.pushparen();
  if ((option==mkcalc.fn_parenr && fmt==mkcalc.fmt_direct) ||
      (option==mkcalc.fn_parenl && fmt==mkcalc.fmt_invert))
    ff=this.popparen();
  if (typeof ff=='function')
    return ff();
  return ff;
};

/* ********** */
mkcalc.Calculator.prototype.appendnumber=function(strnum) {
  strnum=mkuni.chkstr(strnum);
  if (strnum.length==0)
    return mkuni.good;
  if (strnum==mkcalc.fn_decsep)
    strnum=mkuni.str4code(mkuni.asciidot);
  this.chkopfmt(mkcalc.fmt_direct);
  this.setinput(this.input+strnum);
  this.updateoutput();
  return mkuni.good;
};

/* ********** */
mkcalc.Calculator.prototype.replaceoutput=function(strnum) {
  strnum=mkuni.chkstr(strnum);
  if (this.input.length>0)
    this.setinput(strnum);
  else {
    var entry=new mkcalc.Entry();
    var idx=this.ops.length-1;
    if (this.getop(idx,entry)==mkuni.good) {
      if (mkcalc.isaction(entry)==0) {
        entry.str=strnum;
        this.setop(entry,idx);
      }
    }
  }
  this.updateoutput(strnum);
  return mkuni.good;
};

/* ********** */
mkcalc.Calculator.prototype.appendoperator=function(strop) {
  var entry=new mkcalc.Entry(strop);
  var idx=mkuni.linsearch(entry,mkcalc.actionL,mkcalc.cmpentry);
  if (idx<0) {
    this.errcomp("err : unknown operator");
    return mkuni.bad;
  }
  var ee=mkcalc.actionL[idx];
  entry.str=ee.str;
  entry.option=ee.option;
  entry.invstr=ee.invstr;
  var fmt=this.chkopfmt(mkcalc.fmt_direct);
  if (fmt==mkcalc.fmt_invert)
    entry.invert();
  if (entry.str.length==0) {
    this.errcomp("err : unknown operator");
    return mkuni.bad;
  }
  if ((entry.option&mkcalc.fmt_unaction)==0) {
    if (this.input.length>0)
      this.setop(this.input);
    this.setop(entry);
    this.updateaction();
    this.setinput();
  }
  else {
    if (this.calcunary(entry)==mkuni.bad) {
      this.errcomp("err");
      return mkuni.bad;
    }
  }
  return mkuni.good;
};

/* ********** */
mkcalc.Calculator.prototype.setextranumber=function(numtype) {
  numtype=mkuni.chkstr(numtype);
  if (numtype.length==0)
    return mkuni.bad;
  if (numtype==mkcalc.fn_exp) {
    this.chkopfmt(mkcalc.fmt_direct);
    var strinput=this.input;
    if (strinput.length==0)
      this.setinput('1E');
    else {

    }
    this.updateoutput();
  }
  if (numtype==mkcalc.fn_euler) {
    this.setinput(mkfloat.euler);
    return this.updateoutput();
  }
  if (numtype==mkcalc.fn_pi) {
    this.setinput(mkfloat.pi);
    return this.updateoutput();
  }
  return mkuni.bad;
};

/* ********** */
mkcalc.Calculator.prototype.extrafunction=function(fid) {
  var ans=mkuni.good;
  fid=mkuni.chkstr(fid);
  if (fid.length==0)
    ans=mkuni.bad;
  else if (fid==mkcalc.fn_inv)
    this.chkopfmt(mkcalc.fmt_toggle);
  else if (fid==mkcalc.fn_memput || fid==mkcalc.fn_memrcl)
    this.fstore(fid);
  else if (fid==mkcalc.fn_clearall)
    this.clean();
  else if (fid==mkcalc.fn_clear)
    this.clear();
  else if (fid==mkcalc.fn_chsgn)
    this.chgsgn();
  else if (fid==mkcalc.fn_parenl || fid==mkcalc.fn_parenr)
    this.fparen(fid);
  else if (fid==mkcalc.fn_equals)
    this.equals();
  else
    ans=mkuni.bad;
  return ans;
};





