
var mkcalc={};

mkcalc.fmt_deg='deg'; mkcalc.fmt_rad='rad';
mkcalc.fmt_direct='direct'; mkcalc.fmt_invert='invert'; mkcalc.fmt_toggle='toggle';
mkcalc.fmt_addaction=2048; mkcalc.fmt_multaction=4096;
mkcalc.fmt_action=6144; mkcalc.fmt_unaction=8192;

mkcalc.fn_inv='inv'; mkcalc.fn_reci='reci'; mkcalc.fn_memrcl='memrcl';
mkcalc.fn_memput='memput'; mkcalc.fn_clearall='clearall'; mkcalc.fn_clear='clear';
mkcalc.fn_ln='ln'; mkcalc.fn_log='log'; mkcalc.fn_sqrt='sqrt'; mkcalc.fn_sqr='sqr';
mkcalc.fn_pow='pow'; mkcalc.fn_invpow='invpow'; mkcalc.fn_pow10='pow10'; mkcalc.fn_powe='powe';
mkcalc.fn_exp='exp'; mkcalc.fn_sin='sin'; mkcalc.fn_asin='asin'; mkcalc.fn_cos='cos';
mkcalc.fn_acos='acos'; mkcalc.fn_tan='tan'; mkcalc.fn_atan='atan'; mkcalc.fn_fac='fac';
mkcalc.fn_euler='euler'; mkcalc.fn_pi='pi'; mkcalc.fn_7='7'; mkcalc.fn_8='8'; mkcalc.fn_9='9';
mkcalc.fn_div='div'; mkcalc.fn_A='A'; mkcalc.fn_B='B'; mkcalc.fn_4='4'; mkcalc.fn_5='5';
mkcalc.fn_6='6'; mkcalc.fn_mult='mult'; mkcalc.fn_C='C'; mkcalc.fn_D='D'; mkcalc.fn_1='1';
mkcalc.fn_2='2'; mkcalc.fn_3='3'; mkcalc.fn_plus='plus'; mkcalc.fn_E='E'; mkcalc.fn_F='F';
mkcalc.fn_0='0'; mkcalc.fn_chsgn='chsgn'; mkcalc.fn_decsep='decsep'; mkcalc.fn_minus='minus';
mkcalc.fn_parenl='parenl'; mkcalc.fn_parenr='parenr'; mkcalc.fn_equals='equals';
mkcalc.fn_dec='dec'; mkcalc.fn_bin='bin'; mkcalc.fn_oct='oct'; mkcalc.fn_hex='hex';
mkcalc.fn_deg='deg'; mkcalc.fn_rad='rad';

/* ********** */
mkcalc.isfnnumber=function(info) {
  info=mkuni.chkstr(info);
  if (info.length==0)
    return mkuni.bad;
  if (info==mkcalc.fn_decsep || info==mkcalc.fn_0 || info==mkcalc.fn_1 ||
      info==mkcalc.fn_2 || info==mkcalc.fn_3 || info==mkcalc.fn_4 ||
      info==mkcalc.fn_5 || info==mkcalc.fn_6 || info==mkcalc.fn_7 ||
      info==mkcalc.fn_8 || info==mkcalc.fn_9 || info==mkcalc.fn_A ||
      info==mkcalc.fn_B || info==mkcalc.fn_C || info==mkcalc.fn_D ||
      info==mkcalc.fn_E || info==mkcalc.fn_F)
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

/* ########## */
mkcalc.Entry=function(sstr,ioption,sinvstr) {
  this.str=mkuni.chkstr(sstr);
  this.option=parseInt(mkuni.chknumber(ioption,0),10);
  this.invstr=mkuni.chkstr(sinvstr);
};

/* ********** */
mkcalc.Entry.prototype.copy=function() {
  var ans=new this.constructor(this.str,this.option,this.invstr);
  return ans;
};

/* ********** */
mkcalc.Entry.prototype.invert=function() {
  if (this.invstr.length>0) {
    var tmp=this.str;
    this.str=this.invstr;
    this.invstr=tmp;
  }
};

/* ********** */
mkcalc.Entry.prototype.toJson=function(indent) {
  var nextindent='  ',xans='',ans='';
  if (typeof indent=='string')
    nextindent+=indent;
  ans+=aux.qdecl('str',nextindent);
  ans+=aux.qword(this.str);
  ans+=aux.qnextprop(1,indent);
  ans+=aux.qdecl('option',nextindent);
  ans+=aux.qword(this.option.toString());
  ans+=aux.qnextprop(1,indent);
  ans+=aux.qdecl('invstr',nextindent);
  ans+=aux.qword(this.invstr);
  xans+=aux.qattr(ans,1,indent);
  return xans;
};

/* ********** */
mkcalc.chkentry=function(entry) {
  if ((typeof entry!='undefined') && (entry instanceof mkcalc.Entry))
    return mkuni.good;
  return mkuni.bad;
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
  new mkcalc.Entry(mkcalc.fn_fac,mkcalc.fmt_unaction),
  new mkcalc.Entry(mkcalc.fn_reci,mkcalc.fmt_unaction),
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
  if (mkcalc.chkentry(entry)==mkuni.good) {
    var idx=mkuni.linsearch(entry,mkcalc.actionL,mkcalc.cmpentry);
    if (idx>=0)
      ans=mkcalc.actionL[idx].option;
  }
  return ans;
};
/* ########## */

/* ########## */
mkcalc.Calculator=function(resultcontext,infocontext) {
  this.numfmt=10;
  this.trigfmt=mkcalc.fmt_deg;
  this.opfmt=mkcalc.fmt_direct;
  this.input='';
  this.info='';
  this.output='';
  this.showinfo='';
  this.store=[];
  this.ops=[];
  this.paren=[];
  this.ctxresult=resultcontext;
  this.ctxinfo=infocontext;
};

/* ********** */
mkcalc.Calculator.prototype.setinput=function(inp) {
  this.input=mkuni.chkstr(inp);
  return this.input;
};

/* ********** */
mkcalc.Calculator.prototype.setinfo=function(inf) {
  this.info=mkuni.chkstr(inf);
  return this.info;
};

/* ********** */
mkcalc.Calculator.prototype.updateoutput=function(msg) {
  msg=mkuni.chkstr(msg);
  if (msg.length==0)
    msg=this.input;
  if (msg.length==0)
    msg=mkuni.str4code(mkuni.asciizero);
  this.output=msg;
  if (this.ctxresult && this.ctxresult instanceof HTMLTextAreaElement)
    this.ctxresult.value=this.output;
  else
    console.log(new Error().lineNumber+' ['+this.output+']');
  return mkuni.good;
};

/* ********** */
mkcalc.Calculator.prototype.doinfo=function(msg) {
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

/* ********** */
mkcalc.Calculator.prototype.updateinfo=function(msg) {
  msg=mkuni.chkstr(msg);
  if (msg.length==0)
    msg=this.info;
  if (this.ctxinfo && this.ctxinfo instanceof HTMLElement)
    this.ctxinfo.innerHTML=msg;
  else
    console.log(new Error().lineNumber+' ['+msg+']');
  return msg;
};

/* ********** */
mkcalc.Calculator.prototype.clear=function() {
  this.input='';
  this.updateoutput(this.input);
  return mkuni.good;
}

/* ********** */
mkcalc.Calculator.prototype.clean=function() {
  this.ops=[];
  this.paren=[];
  this.trigfmt=mkcalc.fmt_deg;
  this.opfmt=mkcalc.fmt_direct;
  this.input='';
  this.info='';
  this.updateoutput(this.input);
  this.updateinfo(this.info);
  return mkuni.good;
  //but.form.base.options.selectedIndex=8;
  //but.form.elements["b"].disabled=true;
};

/* ********** */
mkcalc.Calculator.prototype.setop=function(entry,idx) {
  var newentry;
  if (mkcalc.chkentry(entry)==mkuni.bad) {
    entry=mkuni.chkstr(entry);
    if (entry.length==0)
      return mkuni.bad;
    newentry=new mkcalc.Entry(entry,this.numfmt);
  }
  else
    newentry=entry.copy();
  idx=mkuni.chknumber(idx,this.ops.length);
  if (idx>=0 && idx<this.ops.length)
    this.ops[idx]=newentry;
  else
    this.ops.push(newentry);
  return mkuni.good;
};

/* ********** */
mkcalc.Calculator.prototype.getop=function(entry,idx) {
  idx=mkuni.chknumber(idx,this.ops.length);
  if (idx<0 || idx>=this.ops.length)
    return mkuni.bad;
  entry=this.ops[idx].copy();
  return mkuni.good;
};

/* ********** */
mkcalc.Calculator.prototype.errcomp=function(msg) {
  this.ops=[];
  this.paren=[];
  this.trigfmt=mkcalc.fmt_deg;
  this.opfmt=mkcalc.fmt_direct;
  this.input='';
  this.info='';
  msg=mkuni.chkstr(msg);
  if (msg.length==0)
    msg='Err';
  this.updateoutput(msg);
  this.updateinfo(this.info);
  return mkuni.good;
};

/* ********** */
mkcalc.Calculator.prototype.chgfmt=function(fmt) {
  var ff=this.numfmt;
  fmt=mkuni.chkbase(fmt,ff);
  if (fmt==ff)
    return ff;
  this.numfmt=fmt;
  var ii=0;
  for (ii=0;ii<this.ops.length;ii++)
    this.ops[ii].option=this.numfmt;
  //checkFnActive();
  return ff;
};

/* ********** */
mkcalc.Calculator.prototype.chktrigfmt=function(fmt) {
  fmt=mkuni.chkstr(fmt);
  if (fmt.length==0)
    return this.trigfmt;
  if (fmt==this.trigfmt)
    return this.trigfmt;
  var ff=this.trigfmt;
  if (fmt==mkcalc.fmt_deg || fmt==mkcalc.fmt_rad)
    this.trigfmt=fmt;
  return ff;
};

/* ********** */
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

/* ********** */
mkcalc.Calculator.prototype.dofmt=function(fmt) {
  fmt=mkint.chkbase(fmt,this.numfmt);
  if (fmt==this.numfmt)
    return this.numfmt;
  var strinput=this.input;
  var entry;
  if (this.input.length>0) {
    strinput=mkfixed.convertbase(this.input,this.numfmt,fmt);
    if (strinput==mkuni.strnan) {
      this.errcomp('err : cannot convert to integer');
      return this.numfmt;
    }
    this.setinput(strinput);
  }
  else if (this.getop(entry,this.ops.length-1)==mkuni.good) {
    var entryaction=mkcalc.isaction(entry);
    if ((entryaction&mkcalc.fmt_action)==0 && (entryaction&mkcalc.fmt_unaction)==0) {
      strinput=mkfixed.convertbase(entry.str,this.numfmt,fmt);
      if (strinput==mkuni.strnan) {
        this.errcomp('err : cannot convert to integer');
        return this.numfmt;
      }
      entry.str=strinput;
      entry.option=fmt;
      this.setop(entry,this.ops.length-1);
    }
  }
  this.numfmt=fmt;
  this.updateoutput(strinput);
  return this.numfmt;
};

/* ********** */
mkcalc.Calculator.prototype.validatehist=function() {
  var ecurr,enext;
  if (this.getop(ecurr,0)==mkuni.bad)
    return mkuni.bad;
  var numL=[];
  var opL=[];
  var ii=0,opscnt=this.ops.length;
  if (opscnt==1) {
    if ((mkcalc.isaction(ecurr)&mkcalc.fmt_action)>0)
      opL.push(ecurr);
    else
      numL.push(ecurr);
  }
  for (ii=0;ii<(opscnt-1);ii++) {
    this.getop(ecurr,ii);
    this.getop(enext,ii+1);
    if (ecurr.str.length>0) {
      if ((mkcalc.isaction(ecurr)&mkcalc.fmt_action)==0) {
        if ((mkcalc.isaction(enext)&mkcalc.fmt_action)==0) {
          numL=[];
          opL=[];
        }
        else
          numL.push(ecurr);
      }
      else {
        if (ii==0)
          numL.push(new mkcalc.Entry(mkuni.str4code(mkuni.asciizero),this.numfmt));
        if ((mkcalc.isaction(enext)&mkcalc.fmt_action)==0)
          opL.push(ecurr);
      }
    }
    if (ii==(opcnt-2) && enext.str.length>0) {
      if ((mkcalc.isaction(enext)&mkcalc.fmt_action)==0)
        numL.push(enext);
      else
        opL.push(enext);
    }
  }
  this.ops=[];
  if (numL.length==0 && opL.length==0)
    return mkuni.bad;
  if (numL.length==0 || opL.length==0) {
    ecurr=(numcnt==0 ? opL[0] : numL[0]);
    if (ecurr.str.length>0) {
      if (numL.length==0)
        this.setop(mkuni.asciizero);
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
    this.setOp(opL[2]);
  return mkuni.good;
};

/* ********** */
mkcalc.Calculator.prototype.calcunary=function(unop) {
  var ans=mkuni.bad;
  if ((mkcalc.isaction(unop)&mkcalc.fmt_unaction)==0)
    return ans;
  var einp=new mkcalc.Entry(this.input);
  if (einp.str.length==0) {
    this.validatehist();
    var opscnt=this.ops.length;
    while (opscnt>0) {
      this.getop(einp,--opscnt);
      if (einp.str.length>0 && (mkcalc.isaction(einp)&mkcalc.fmt_action)==0)
        break;
    }
    if (einp.str.length==0 || (mkcalc.isaction(einp)&mkcalc.fmt_action)>0)
      return ans;
  }
  var fp=new mkfixed.FixedPoint(einp.str);
  if (fp.unmix()==0)
    return ans;
  var domult=mkuni.str4code(mkuni.asciimult);
  var dodiv=mkuni.str4code(mkuni.asciidiv);
  var dosubs=mkuni.str4code(mkuni.asciiminus);
  var ieeecalc=NaN,prov=NaN;
  var strcalc='',strprov='';
  var trignum=(this.trigfmt==mkcalc.fmt_rad ? mkuni.str4code(mkuni.asciione) : mkfloat.rad);
  if (unop.str==mkcalc.fn_sqrt) {
    if (fp.sgn==mkuni.asciiminus) {
      this.errcomp('err : cannot compute transcendent number');
      return ans;
    }
    ieeecalc=mkfloat.roundpos(Math.sqrt(parseFloat(fp.toString())),mkfloat.maxieee);
  }
  else if (unop.str==mkcalc.fn_sqr) {
    strcalc=mkfixed.calculate(domult,fp.toString(),fp.toString());
  }
  else if (unop.str==mkcalc.fn_sin) {
    strprov=mkfixed.calculate(domult,fp.toString(),trignum);
    ieeecalc=mkfloat.roundpos(Math.sin(parseFloat(strprov)),mkfloat.maxieee);
  }
  else if (unop.str==mkcalc.fn_asin) {
    ieeecalc=Math.asin(parseFloat(fp.toString()));
    strcalc=mkfixed.calculate(dodiv,ieeecalc.toString(),trignum);
    var rdfp=new mkfixed.FixedPoint(strcalc);
    strcalc=rdfp.roundpos(mkfloat.maxieee).toString();
  }
  else if (unop.str==mkcalc.fn_cos) {
    strprov=mkfixed.calculate(domult,fp.toString(),trignum);
    ieeecalc=mkfloat.roundpos(Math.cos(parseFloat(strprov)),mkfloat.maxieee);
  }
  else if (unop.str==mkcalc.fn_acos) {
    ieeecalc=Math.acos(parseFloat(fp.toString()));
    strcalc=mkfixed.calculate(dodiv,ieeecalc.toString(),trignum);
    var rdfp=new mkfixed.FixedPoint(strcalc);
    strcalc=rdfp.roundpos(mkfloat.maxieee).toString();
  }
  else if (unop.str==mkcalc.fn_tan) {
    strprov=mkfixed.calculate(domult,fp.toString(),trignum);
    prov=Math.cos(parseFloat(strprov));
    if (mkfloat.cmp(prov,0,.0000000000000001)==0) {
      this.errcomp("err : cannot divide by \"0\"");
      return ans;
    }
    ieeecalc=mkfloat.roundpos(Math.tan(parseFloat(strprov)),mkfloat.maxieee);
  }
  else if (unop.str==mkcalc.fn_atan) {
    ieeecalc=Math.atan(parseFloat(fp.toString()));
    strcalc=mkfixed.calculate(dodiv,ieeecalc.toString(),trignum);
    var rdfp=new mkfixed.FixedPoint(strcalc);
    strcalc=rdfp.roundpos(mkfloat.maxieee).toString();
  }
  else if (unop.str==mkcalc.fn_ln) {
    if (fp.sgn==mkuni.asciiminus || fp.base==1) {
      errcomp("err : cannot compute transcendent number");
      return ans;
    }
    ieeecalc=mkfloat.roundpos(Math.log(parseFloat(fp.toString())),mkfloat.maxieee);
  }
  else if (unop.str==mkcalc.fn_powe) {
    if (fp.isInt()==0)
      ieeecalc=Math.exp(parseFloat(fp.toString()));
    else {
      var fpsgn=fp.sgn;
      fp.sgn=0;
      strprov=fp.toStringI();
      if (mkfixed.cmpstr10(strprov,mkuni.str4code(mkuni.asciizero))==0)
        strcalc=mkuni.str4code(mkuni.asciione);
      else {
        strcalc=mkfloat.euler;
        while (mkfixed.cmpstr10(strprov,mkuni.str4code(mkuni.asciizero))==1) {
          strcalc=mkfixed.calculate(domult,strcalc,mkfloat.euler);
          strprov=mkfixed.calculate(dosubs,strprov,mkuni.str4code(mkuni.asciione));
        }
      }
      if (fpsgn==mkuni.asciiminus)
        strcalc=mkfixed.calculate(dodiv,mkuni.str4code(mkuni.asciione),strcalc);
    }
  }
  else if (unop.str==mkcalc.fn_log) {
    if (fp.sgn==mkuni.asciiminus || fp.base==1) {
      errcomp("err : cannot compute transcendent number");
      return ans;
    }
    ieeecalc=Math.log(parseFloat(fp.toString()));
    strcalc=mkfixed.calculate(mkuni.str4code(mkuni.asciidiv),ieeecalc.toString(),mkfloat.ln10);
    var rdfp=new mkfixed.FixedPoint(strcalc);
    strcalc=rdfp.roundpos(mkfloat.maxieee).toString();
  }
  else if (unop.str==mkcalc.fn_pow10) {
    if (fp.isInt()==0)
      ieeecalc=Math.pow(10,parseFloat(fp.toString()));
    else {
      var fpsgn=fp.sgn;
      fp.sgn=0;
      strprov=fp.toStringI();
      if (mkfixed.cmpstr10(strprov,mkuni.str4code(mkuni.asciizero))==0)
        strcalc=mkuni.str4code(mkuni.asciione);
      else {
        strcalc=mkuni.strten;
        while (mkfixed.cmpstr10(strprov,mkuni.str4code(mkuni.asciizero))==1) {
          strcalc=mkfixed.calculate(domult,strcalc,mkuni.strten);
          strprov=mkfixed.calculate(dosubs,strprov,mkuni.str4code(mkuni.asciione));
        }
      }
      if (fpsgn==mkuni.asciiminus)
        strcalc=mkfixed.calculate(dodiv,mkuni.str4code(mkuni.asciione),strcalc);
    }
  }
  else if (unop.str==mkcalc.fn_fac) {
    if (fp.isInt()==0) {
      errcomp("err : input must be integer");
      return ans;
    }
    if (fp.sgn==mkuni.asciiminus || fp.base==1) {
      errcomp(mkuni.str4code(mkuni.asciizero));
      return ans;
    }
    strprov=fp.toStringI();
    strcalc=strprov;
    while (mkfixed.cmpstr10(strprov,mkuni.str4code(mkuni.asciione))==1) {
      strprov=mkfixed.calculate(dosubs,strprov,mkuni.str4code(mkuni.asciione));
      strcalc=mkfixed.calculate(domult,strcalc,strprov);
    }
  }
  else if (unop.str==mkcalc.fn_reci) {
    strprov=fp.toString();
    if (mkfixed.cmpstr10(strprov,mkuni.str4code(mkuni.asciizero))==0) {
      this.errcomp("err : cannot divide by \"0\"");
      return ans;
    }
    strcalc=mkfixed.calculate(dodiv,mkuni.str4code(mkuni.asciione),strprov);
  }
  else if (unop.str==mkcalc.fn_chsgn) {
    fp.sgn=(fp.sgn==0 ? mkuni.asciiminus : 0);
    strcalc=fp.toString();
  }
  if (!isNaN(ieeecalc)) {
    if (strcalc.length==0)
      strcalc=ieeecalc.toString();
    var info=this.doinfo('ieee');
    this.updateinfo(info);
  }
  fp=new mkfixed.FixedPoint(strcalc);
  if (fp.unmix()==0)
    return ans;
  ans=mkuni.good;
  strcalc=fp.toStringF(1000000);
  this.setop(new mkcalc.Entry(strcalc),this.ops.length-1);
  this.setinput(mkuni.str4code(mkuni.asciizero));
  this.updateoutput(strcalc);
  return ans;
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
  this.setinput(fp.toStringF(1000000));
  this.updateoutput(this.input);
  return mkuni.good;
};

/* ********** */
mkcalc.Calculator.prototype.pushstore=function() {
  var fmt=this.chkopfmt(mkcalc.fmt_direct);
  if (fmt==mkcalc.fmt_invert)
    return popstore();
  var str=this.input;
  var lastop;
  if (str.length==0 && this.getop(lastop,this.ops.length-1)==mkuni.good) {
    var entryaction=mkcalc.isaction(lastop);
    if ((entryaction&mkcalc.fmt_action)==0 && (entryaction&mkcalc.fmt_unaction)==0)
      str=lastop.str;
  }
  if (str.length>0)
    this.store.push(str);
  else
    this.store.pop();
  var ans=this.doinfo();
  this.updateinfo(ans);
  return mkuni.good;
};

/* ********** */
mkcalc.Calculator.prototype.popstore=function() {
  var fmt=this.chkopfmt(mkcalc.fmt_direct);
  if (mkcalc.fmt_invert)
    return this.pushstore();
  this.setinput(mkuni.str4code(mkuni.asciizero));
  var str=this.store.pop();
  setop(str);
  this.updateoutput(str);
  var ans=this.doinfo();
  this.updateinfo(ans);
  return mkuni.good;
};

/* ********** */
mkcalc.Calculator.prototype.pushparen=function() {
  var fmt=this.chkopfmt(mkcalc.fmt_direct);
  if (fmt==mkcalc.fmt_invert)
    return this.popparen();
  this.validatehist();
  var entry;
  if (this.getop(entry,this.ops.length-1)==mkuni.good) {
    var entryaction=mkcalc.isaction(entry);
    if ((entryaction&mkcalc.fmt_action)==0) {
      this.errcomp("err : prev must be operator");
      return mkuni.bad;
    }
  }
  this.paren.push(this.ops.slice());
  this.ops=[];
  this.setInput(mkuni.str4code(mkuni.asciizero));
  var ans=this.doinfo();
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
  //equals(true);
  var lastop;
  if (this.getop(lastop,this.ops.length-1)==mkuni.bad) {
    this.errcomp(mkuni.str4code(mkuni.asciizero));
    return mkuni.bad;
  }
  this.ops=this.paren.pop().slice();
  this.ops.push(lastop);
  var ans=this.doinfo();
  this.updateinfo(ans);
  return mkuni.good;
};

/* ********** */
mkcalc.Calculator.prototype.appendnumber=function(strNum) {
  strNum=mkuni.chkstr(strNum);
  if (strNum.length==0)
    return mkuni.good;
  if (strNum==mkcalc.fn_decsep)
    strNum=mkuni.str4code(mkuni.asciidec);
  this.chkopfmt(mkcalc.fmt_direct);
  var strInput=this.input;
  this.input=strInput+strNum;
  this.updateoutput(this.input);
  return mkuni.good;
};

/* ********** */
mkcalc.Calculator.prototype.appendoperator=function(strOp) {
  var entry=new mkcalc.Entry(strOp);
  var idx=mkuni.linsearch(entry,mkcalc.actionL,mkcalc.cmpentry);
  if (idx<0) {
    this.errcomp("err : unknown operator");
    return mkuni.bad;
  }
  entry=mkcalc.actionL[idx].copy();
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
    //this.updateaction();
    this.input='';
  }
  else {
    this.calcunary(entry);
  }
  return mkuni.good;
};

/* ********** */
mkcalc.Calculator.prototype.setextranumber=function(numtype) {
  numtype=mkuni.chkstr(numtype);
  if (numtype.length==0)
    return mkuni.bad;
  if (numtype==mkcalc.fn_exp)
    return this.appendnumber("e+000");
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
  else if (fid==mkcalc.fn_clearall)
    this.clean();
  else if (fid==mkcalc.fn_clear)
    this.clear();
  else if (fid==mkcalc.fn_chsgn)
    this.chgsgn();
  else
    ans=mkuni.bad;
  return ans;
};

/* ********** */
mkcalc.Calculator.prototype.updateaction=function() {
  //int validated=validateHist();

  /*if (validated<0)
    return -1;
  int numcnt=0,opcnt=0,opscnt=m_ops.count();
  if (opscnt<4)
    return -1;
  Entry num1=m_ops[0];
  numcnt++;
  Entry op1=m_ops[1];
  opcnt++;
  Entry num2=m_ops[2];
  numcnt++;
  Entry op2=m_ops[3];
  opcnt++;
  Entry num3;
  Entry op3;
  if (opscnt>4) {
    num3=m_ops[4];
    numcnt++;
  }
  if (opscnt>5) {
    op3=m_ops[5];
    opcnt++;
  }
  m_ops.clear();
  if (numcnt<3) {
    num1=compute(num1,num2,op1);
    if (num1.m_option!=m_numfmt)
      delayedChgFmt(num1.m_option);
    if (num1.m_str.len()==0) {
      errComp(0);
      return -1;
    }
    setOp(-1,num1);
    setOp(-1,op2);
    updateOutput(num1.m_str);
    return -1;
  }
  if (opcnt<3)
    return -1;
  num2=compute(num2,num3,op2);
  if (num2.m_option!=m_numfmt)
    delayedChgFmt(num2.m_option);
  if (num1.m_str.len()==0) {
    errComp(0);
    return -1;
  }
  if (op3==fn_mult || op3==fn_pow || op3==fn_invpow || op3==fn_div) {
    setOp(-1,num1);
    setOp(-1,op1);
    setOp(-1,num2);
    setOp(-1,op3);
    updateOutput(num2.m_str);
    return -1;
  }
  num1=compute(num1,num2,op1);
  if (num1.m_option!=m_numfmt)
    delayedChgFmt(num1.m_option);
  if (num1.m_str.len()) {
    errComp(0);
    return -1;
  }
  setOp(-1,num1);
  setOp(-1,op3);
  updateOutput(num1.m_str);*/

  return mkuni.good;
};

/*
Entry Calculator::compute(Entry num1_,Entry num2_,Entry action) {

  Entry res;
  if (num1_.m_str.len()==0 || num2_.m_str.len()==0 || action.m_str.len()==0)
    return res;
  if (isAction(action)==0)
    return res;
  Entry num1=num1_,num2=num2_;
  int fmt1=num1.m_option,fmt2=num2.m_option;
  int oki1=fmt1,oki2=fmt2,i=0,prec1=0,prec2=0,sgnr=1,sgni1=1,sgni2=1;
  aux::ulreal nl1=aux::a2ui((const char *)num1.m_str,&oki1,&sgni1),
                nl2=aux::a2ui((const char *)num2.m_str,&oki2,&sgni2);

  if (oki1<0 && (fmt1==2 || fmt1==8 || fmt1==16))
    return res;
  if (oki2<0 && (fmt2==2 || fmt2==8 || fmt2==16))
    return res;
  bool isintcalc=(oki1>0 && oki2>0);
  aux::ulreal rl=0;
  double nd1=.0,nd2=.0,rd=.0;
  if (oki1>0)
    nd1=(double)sgni1*double(nl1);
  else
    nd1=aux::a2d((const char *)num1.m_str,&prec1);
  if (oki2>0)
    nd2=(double)sgni2*double(nl2);
  else
    nd2=aux::a2d((const char *)num2.m_str,&prec2);
  if (aux::isNan(nd1)!=0 || aux::isNan(nd2)!=0)
    return res;
  if (action==fn_plus)
    rd=nd1+nd2;
  else if (action==fn_minus)
    rd=nd1-nd2;
  else if (action==fn_mult)
    rd=nd1*nd2;
  else if (action==fn_div)
    rd=nd1/nd2;
  else if (action==fn_pow)
    rd=pow(nd1,nd2);
  else if (action==fn_invpow)
    rd=pow(nd1,1.0/nd2);
  else
    return res;
  if (!aux::isFinite(rd))
    return res;
  int magrd=aux::mag(rd);
  if (isintcalc && (
    magrd>aux::ui64Mag ||
    (action==fn_pow && sgni2<0) ||
     action==fn_invpow ||
     (action==fn_div && (nl1<nl2 || (nl1%nl2)!=0))))
    isintcalc=false;
  if (isintcalc) {
    Entry iaction=action;
    int overflow=0;
    if (iaction==fn_plus && sgni1!=sgni2)
      iaction=fn_minus;
    else if (iaction==fn_minus && sgni1!=sgni2)
      iaction=fn_plus;
    if (iaction==fn_plus) {
      rl=aux::binAdd(nl1,nl2,&overflow);
      if (overflow&1)
        isintcalc=false;
      else if (sgni1<0)
        sgnr=-1;
    }
    else if (iaction==fn_minus) {
      if (nl1>nl2) {
        rl=nl1-nl2;
        if (sgni1<0)
          sgnr=-1;
      }
      else {
        rl=nl2-nl1;
        if (sgni1>0)
          sgnr=-1;
      }
    }
    else if (iaction==fn_mult) {
      rl=aux::binMult(nl1,nl2,&overflow);
      if (overflow&1)
        isintcalc=false;
      else if ((sgni1<0 && sgni2>0) || (sgni1>0 && sgni2<0))
        sgnr=-1;
    }
    else if (iaction==fn_div) {
      rl=nl1/nl2;
      if ((sgni1<0 && sgni2>0) || (sgni1>0 && sgni2<0))
        sgnr=-1;
    }
    else if (iaction==fn_pow) {
      rl=1;
      for (i=0;i<(int)nl2;i++) {
        rl=aux::binMult(rl,nl1,&overflow);
        if (overflow&1) {
          isintcalc=false;
          break;
        }
      }
      if (sgni1<0 && nl2!=0 && nl2%2)
        sgnr=-1;
    }
    if (isintcalc && rl>(aux::lreal)aux::i64Limit && sgnr<0)
      isintcalc=false;
  }
  aux::Asciistr buf;
  if (isintcalc) {
    if (sgnr<0)
      aux::i2a(sgnr*rl,&buf,fmt2);
    else
      aux::ui2a(rl,&buf,fmt2);
    res.m_str=buf;
    res.m_option=fmt2;
  }
  else {
    if (action==fn_plus || action==fn_minus)
      aux::d2a(rd,&buf,prec1>prec2 ? prec1 : prec2,-1);
    else
      aux::d2a(rd,&buf,-magrd+aux::dPrec,-1);
    res.m_str=buf;
    res.m_option=fmtDec;
    //doFmt(11);
  }

  return res;

}

int Calculator::equals(bool calcparen) {

  opFmt(fmtDirect);
  if (!calcparen && m_paren.count()>0) {
    errComp("err : unmatched paren");
    return -1;
  }
  if (m_input.len()>0)
    setOp(-1,Entry(m_input,m_numfmt));
  setInput(0);

  validateHist();
  int numcnt=0,opcnt=0,opscnt=m_ops.count();
  if (opscnt==0)
    return -1;
  Entry num1,num2,num3,op1,op2,op3;
  if (opscnt>0) {
    num1=m_ops[0];
    numcnt++;
  }
  if (opscnt>1) {
    op1=m_ops[1];
    opcnt++;
  }
  if (opscnt>2) {
    num2=m_ops[2];
    numcnt++;
  }
  if (opscnt>3) {
    op2=m_ops[3];
    opcnt++;
  }
  if (opscnt>4) {
    num3=m_ops[4];
    numcnt++;
  }
  if (opscnt>5) {
    op3=m_ops[5];
    opcnt++;
  }
  m_ops.clear();

  if (opcnt<1) {
    setOp(-1,num1);
    return -1;
  }

  if (numcnt<3 || opcnt<2) {
    num1=compute(num1,numcnt<2 ? num1 : num2,op1);
    if (num1.m_option!=m_numfmt)
      delayedChgFmt(num1.m_option);
    if (num1.m_str.len()==0) {
      errComp(0);
      return -1;
    }
    setOp(-1,num1);
    updateOutput(num1.m_str);
    return 0;
  }
  num2=compute(num2,num3,op2);
  if (num2.m_option!=m_numfmt)
    delayedChgFmt(num2.m_option);
  if (num2.m_str.len()==0) {
    errComp(0);
    return -1;
  }
  num1=compute(num1,num2,op1);
  if (num1.m_option!=m_numfmt)
    delayedChgFmt(num1.m_option);
  if (num1.m_str.len()==0) {
    errComp(0);
    return -1;
  }
  setOp(-1,num1);
  updateOutput(num1.m_str);

  //dbgOp(m_input,m_ops,__LINE__);

  return 0;

}

int Calculator::checkFnActive() {

  int ii=0,fnuifmt=((m_numfmt==2 || m_numfmt==8 || m_numfmt==16) ? fn_Inactive : fn_Active);

  for (ii=0;ii<numfn_operator;ii++) {
    if (fn_operator[ii]!=fn_div && fn_operator[ii]!=fn_minus &&
        fn_operator[ii]!=fn_mult && fn_operator[ii]!=fn_plus)
      fnActive(fn_operator[ii],fnuifmt);
  }
  fnActive(fn_exp,fnuifmt);
  fnActive(fn_chsgn,fnuifmt);
  fnActive(fn_euler,fnuifmt);
  fnActive(fn_pi,fnuifmt);
  fnActive(fn_decsep,fnuifmt);
  for (ii=1;ii<numfn_number;ii++)
    fnActive(fn_number[ii],ii>(int)m_numfmt ? fn_Inactive : fn_Active);
  fnActive(fn_parenr,m_paren.count()>0 ? fn_Active : fn_Inactive);
  fnActive(fn_memrcl,m_store.count()>0 ? fn_Active : fn_Inactive);

  return 0;

}

int Calculator::fnActive(const char *,int) {

  return 0;

}

static void key2calcstr(osix::xxEvent *keyInput,aux::Ucsstr *str) {

  if (keyInput->m_xxk==osix::xxk_comma)
    str->append(fn_decsep);
  else if (keyInput->m_xxk==osix::xxk_plus)
    str->append(fn_plus);
  else if (keyInput->m_xxk==osix::xxk_minus)
    str->append(fn_minus);
  else if (keyInput->m_xxk==osix::xxk_asterisk)
    str->append(fn_mult);
  else if (keyInput->m_xxk==osix::xxk_slash)
    str->append(fn_div);
  else if (keyInput->m_xxk>=osix::xxk_a && keyInput->m_xxk<=osix::xxk_f)
    str->append((unsigned short)toupper((int)keyInput->m_xxk));
  else
    str->append((unsigned short)keyInput->m_xxk);

}

// **********

int Calculator::updateOutput(aux::Asciistr msg) {

  if (msg.len() == 0)
    msg=m_input;
  if (msg.len() == 0)
    msg="0";
  m_output=msg;
  osUpdate(m_outputwin.m_w);
  return 0;

}

int Calculator::updateInfo(aux::Asciistr msg) {

  if (msg.len() == 0)
    msg=m_info;
  if (msg.len() == 0)
    msg=" ";
  m_showinfo=msg;
  osUpdate(m_showinfowin.m_w);
  return 0;

}

int Calculator::osEvent(void *disp,void *win,void *osev) {

  osix::xxEvent xxev;
  osix::xxtranslateEvent(osev,&xxev);
  osix::xxGC gc;
  aux::Ucsstr str;
  if (xxev.m_type==osix::xx_resize) {
    if (win==m_outputwin.m_w)
      m_outputwin.m_r=xxev.m_r;
    else if (win==m_showinfowin.m_w)
      m_showinfowin.m_r=xxev.m_r;
  }
  else if (xxev.m_type==osix::xx_paint) {
    if (win==m_outputwin.m_w) {
      gc.m_fg=m_outputfg;
      gc.m_bg=m_outputbg;
      gc.m_fnt=m_outputfnt;
      gc.m_r=m_outputwin.m_r;
      str=(const char *)m_output;
      osix::xxpredraw(disp,&m_outputwin,&gc);
      osix::xxfillRect(disp,&m_outputwin,&gc);
      osix::xxdrawText(disp,&m_outputwin,&gc,&str,osix::xx_alignright|osix::xx_alignvcenter);
      osix::xxpostdraw(disp,&m_outputwin,&gc);
    }
    else if (win==m_showinfowin.m_w) {
      gc.m_fg=m_infofg;
      gc.m_bg=m_infobg;
      gc.m_fnt=m_infofnt;
      gc.m_r=m_showinfowin.m_r;
      str=(const char *)m_showinfo;
      osix::xxpredraw(disp,&m_showinfowin,&gc);
      osix::xxfillRect(disp,&m_showinfowin,&gc);
      osix::xxdrawText(disp,&m_showinfowin,&gc,&str,osix::xx_alignright|osix::xx_alignvcenter);
      osix::xxpostdraw(disp,&m_showinfowin,&gc);
    }
    checkFnActive();
  }
  else if (osix::xxTypeInput(&xxev)>0)
    inputEvent(&xxev);

  return xxev.m_consumer;

}

int Calculator::inputEvent(osix::xxEvent *xxev) {

  if (osix::xxTypeMouse(xxev)>0)
    return xxev->m_consumer;

  if (xxev->m_type==osix::xx_keyPressed)
    xxev->m_consumer|=osix::xx_consumed;
  else if (xxev->m_type==osix::xx_keyReleased) {
    if ((m_xxlastinputev.m_consumer&osix::xx_consumed)>0)
      xxev->m_consumer|=osix::xx_consumed;
    if ((m_xxlastinputev.m_consumer&osix::xx_processed)==0)
      return xxev->m_consumer;
  }
  xxev->m_xxk=osix::lowerkey(xxev);
  int twkey=osix::typewriterkey(xxev);
  aux::Ucsstr str;
  key2calcstr(xxev,&str);
  if ((twkey&osix::xxk_ascii)>0) {
    if ((xxev->m_mods&osix::xx_modCtrl)>0) {
      if (xxev->m_xxk==osix::xxk_c) {
        xxev->m_consumer|=osix::xx_processed;
        if (xxev->m_type==osix::xx_keyReleased)
          doCopy();
      }
      if (xxev->m_xxk==osix::xxk_x) {
        xxev->m_consumer|=osix::xx_processed;
        if (xxev->m_type==osix::xx_keyReleased)
          doCut();
      }
      if (xxev->m_xxk==osix::xxk_v) {
        xxev->m_consumer|=osix::xx_processed;
        if (xxev->m_type==osix::xx_keyReleased)
          doPaste();
      }
    }
    else if (xxev->m_mods==0 || (xxev->m_mods&osix::xx_modShift)>0) {
      if ((xxev->m_xxk>=osix::xxk_0 && xxev->m_xxk<=osix::xxk_9) ||
          (xxev->m_xxk>=osix::xxk_a && xxev->m_xxk<=osix::xxk_f) ||
          xxev->m_xxk==osix::xxk_period) {
        if (xxev->m_type==osix::xx_keyPressed)
          xxev->m_consumer|=osix::xx_processed;
        else {
          if (
            (m_numfmt==fmtBin &&
              (xxev->m_xxk==osix::xxk_0 || xxev->m_xxk==osix::xxk_1))
            ||
            (m_numfmt==fmtOct &&
              (xxev->m_xxk>=osix::xxk_0 && xxev->m_xxk<=osix::xxk_8))
            ||
            (m_numfmt==fmtDec &&
              ((xxev->m_xxk>=osix::xxk_0 && xxev->m_xxk<=osix::xxk_9) ||
                xxev->m_xxk==osix::xxk_period))
            ||
            (m_numfmt==fmtHex &&
              ((xxev->m_xxk>=osix::xxk_0 && xxev->m_xxk<=osix::xxk_9) ||
                (xxev->m_xxk>=osix::xxk_a && xxev->m_xxk<=osix::xxk_f)))
          ) {
            xxev->m_consumer|=osix::xx_processed;
            appendNumber(str.asciistr());
          }
        }
      }
      else if (xxev->m_xxk==osix::xxk_asterisk || xxev->m_xxk==osix::xxk_plus ||
               xxev->m_xxk==osix::xxk_minus || xxev->m_xxk==osix::xxk_slash) {
        xxev->m_consumer|=osix::xx_processed;
        if (xxev->m_type==osix::xx_keyReleased)
          appendOperator(str.asciistr());
      }
      else if (xxev->m_xxk==osix::xxk_parenleft) {
        xxev->m_consumer|=osix::xx_processed;
        if (xxev->m_type==osix::xx_keyReleased)
          pushParen();
      }
      else if (xxev->m_xxk==osix::xxk_parenright) {
        xxev->m_consumer|=osix::xx_processed;
        if (xxev->m_type==osix::xx_keyReleased)
          popParen();
      }
      else if (xxev->m_xxk==osix::xxk_equal) {
        xxev->m_consumer|=osix::xx_processed;
        if (xxev->m_type==osix::xx_keyReleased)
          equals();
      }
    }
  }
  else if (twkey==0) {
    if (xxev->m_xxk==osix::xxk_Enter || xxev->m_xxk==osix::xxk_Return) {
      xxev->m_consumer|=osix::xx_processed;
      if (xxev->m_type==osix::xx_keyReleased)
        equals();
    }
    else if (xxev->m_xxk==osix::xxk_Delete) {
      xxev->m_consumer|=osix::xx_processed;
      if (xxev->m_type==osix::xx_keyReleased)
        clear();
    }
    else if (xxev->m_xxk==osix::xxk_BackSpace) {
      xxev->m_consumer|=osix::xx_processed;
      if (xxev->m_type==osix::xx_keyReleased) {
        opFmt(calculator::fmtDirect);
        if (m_input.len()>0) {
          m_input=m_input.substr(0,m_input.len()-1);
          updateOutput(m_input);
        }
        updateInfo(doInfo());
      }
    }
    else if (xxev->m_xxk==osix::xxk_Shift_L || xxev->m_xxk==osix::xxk_Super_L)
      xxev->m_consumer|=osix::xx_processed;
  }

  m_xxlastinputev=*xxev;

  return xxev->m_consumer;

}

int Calculator::doCopy() {

  aux::Ucsstr str;
  str=(const char *)m_output;
  osix::xxtoClipboard(&str);
  return 0;

}

int Calculator::doCut() {

  aux::Ucsstr str;
  str=(const char *)m_output;
  osix::xxtoClipboard(&str);
  int opscnt=m_ops.count();
  if (m_input.len() > 0) {
    setInput("");
  }
  else if (opscnt > 0) {
    Entry lastentry=m_ops[opscnt - 1];
    unsigned int isaction=isAction(lastentry);
    if ((isaction&fmtAction)==0 && (isaction&fmtUnaction) == 0)
      m_ops.remove(opscnt - 1);
  }
  updateOutput(m_input);
  return 0;

}

int Calculator::doPaste() {

  aux::Ucsstr str;
  osix::xxfromClipboard(&str);
  setInput(str.asciistr());
  updateOutput(m_input);
  return 0;

}

*/




