
mkcalc.win=void(0);
mkcalc.pc=void(0);

/* ##### find inputs #####

/* ********** */
mkcalc.findElementById=function(eid) {
  eid=mkuni.chkstr(eid);
  if (eid.length>0 && mkcalc.win)
    return mkcalc.win.document.getElementById(eid);
  return void(0);
};

/* ********** */
mkcalc.findButtonById=function(eid) {
  var but=mkcalc.findElementById(eid);
  if (typeof but!='undefined' &&
       (but instanceof HTMLInputElement) &&
       but.type.toLowerCase()=='button')
    return but;
  return void(0);
};

/* ********** */
mkcalc.findRadioById=function(eid) {
  var trig=mkcalc.findElementById(eid);
  if (typeof trig!='undefined' &&
       (trig instanceof HTMLInputElement) &&
       trig.type.toLowerCase()=='radio')
    return trig;
  return void(0);
};

/* ********** */
mkcalc.findComboById=function(eid) {
  var combo=mkcalc.findElementById(eid);
  if (typeof combo!='undefined' && (combo instanceof HTMLSelectElement))
    return combo;
  return void(0);
};

/* ********** */
mkcalc.findLineeditById=function(eid) {
  var fpdig=mkcalc.findElementById(eid);
  if (typeof fpdig!='undefined' &&
       (fpdig instanceof HTMLInputElement) &&
       fpdig.type.toLowerCase()=='text')
    return fpdig;
  return void(0);
};

/* ********** */
mkcalc.findTextareaById=function(eid) {
  var multiline=mkcalc.findElementById(eid);
  if (typeof multiline!='undefined' && (multiline instanceof HTMLTextAreaElement))
    return multiline;
  return void(0);
};

/* ********** */
mkcalc.findTagById=function(eid) {
  var tag=mkcalc.findElementById(eid);
  if (typeof tag!='undefined' && (tag instanceof HTMLElement))
    return tag;
  return void(0);
};

/* ##### interactive #####

/* ********** */
mkcalc.domaxlen=function(evt,inp) {
  var lineedit=mkcalc.findLineeditById(inp.id);
  if (lineedit) {
    var oldmax=mkcalc.pc.chkscale();
    if (mkcalc.pc.chkscale(mkint.int4str(lineedit.value))==oldmax)
      lineedit.value=mkint.int2str(oldmax);
  }
};

/* ********** */
mkcalc.doroundpos=function(evt,inp) {
  var lineedit=mkcalc.findLineeditById(inp.id);
  if (lineedit) {
    var oldrd=mkcalc.pc.chkrdpos();
    if (mkcalc.pc.chkrdpos(mkint.int4str(lineedit.value))==oldrd)
      lineedit.value=mkint.int2str(oldrd);
  }
};

/* ********** */
mkcalc.dosci=function(evt,inp) {
  var lineedit=mkcalc.findLineeditById(inp.id);
  if (lineedit) {
    var oldscifmt=mkcalc.pc.chkscifmt();
    if (mkcalc.pc.chkscifmt(mkint.int4str(lineedit.value))==oldscifmt)
      lineedit.value=mkint.int2str(oldscifmt);
  }
};

/* ********** */
mkcalc.dobase=function(evt,inp) {
  var combo=mkcalc.findComboById(inp.id);
  if (combo)
    mkcalc.pc.chknumfmt(combo.options.selectedIndex+2);
};

/* ********** */
mkcalc.doinput=function(inpid) {
  var ans=mkuni.bad;
  inpid=mkuni.chkstr(inpid);
  if (inpid.length==0)
    return ans;
  if (mkcalc.isfnnumber(inpid))
    ans=mkcalc.pc.appendnumber(inpid);
  else if (mkcalc.isfnextranumber(inpid))
    ans=mkcalc.pc.setextranumber(inpid);
  else if (mkcalc.isfnoperator(inpid))
    ans=mkcalc.pc.appendoperator(inpid);
  else
    ans=mkcalc.pc.extrafunction(inpid);
  return ans;
};

/* ********** */
mkcalc.dobutinput=function(but) {
  var butid=but.id;
  return mkcalc.doinput(butid);
};

/* ********** */
mkcalc.dokeyinput=function(evt,inp) {
  var ans=mkuni.bad;
  var multiline=mkcalc.findElementById(inp.id);
  if (typeof multiline=='undefined' || !(multiline instanceof HTMLTextAreaElement))
    return ans;
  var kevt=(evt || window.event);
  var keycode=(kevt.keyCode || kevt.which);
  var fncalc='';
  if (keycode==mkuni.asciimult)
    fncalc=mkcalc.fn_mult;
  else if (keycode==mkuni.asciiplus)
    fncalc=mkcalc.fn_plus;
  else if (keycode==mkuni.asciiminus)
    fncalc=mkcalc.fn_minus;
  else if (keycode==mkuni.asciidiv)
    fncalc=mkcalc.fn_div;
  else if (keycode==mkuni.asciiequals || keycode==mkuni.asciicr)
    fncalc=mkcalc.fn_equals;
  else if (keycode==mkuni.asciidot)
    fncalc=mkcalc.fn_decsep;
  else
    fncalc=mkcalc.keycode2fnnumber(keycode);
  if (keycode!=mkuni.asciileft && keycode!=mkuni.asciiright &&
      keycode!=mkuni.asciibackspace && keycode!=mkuni.asciidel) {
    kevt.returnValue=false;
    kevt.preventDefault();
  }
  ans=mkcalc.doinput(fncalc);
  return ans;
};

/* ********** */
mkcalc.dokeyup=function(evt,inp) {
  var ans=mkuni.bad;
  var kevt=(evt || window.event);
  var keycode=(kevt.keyCode || kevt.which);
  if (keycode==mkuni.asciibackspace || keycode==mkuni.asciidel) {
    var txtres=mkcalc.findTextareaById('result');
    if (txtres) {
      var str=txtres.value;
      kevt.returnValue=false;
      kevt.preventDefault();
      mkcalc.pc.replaceoutput(str);
    }
  }
  return ans;
};

/* ##### callbacks #####

/* ********** */
mkcalc.ctxout=function(msg) {
  var txtres=mkcalc.findTextareaById('result');
  if (txtres)
    txtres.value=msg;
};

/* ********** */
mkcalc.ctxinfo=function(msg) {
  var txtinfo=mkcalc.findTagById('info');
  if (txtinfo)
    txtinfo.innerHTML=msg;
};

/* ********** */
mkcalc.ctxbuttons=function(numfmt) {
  numfmt=mkint.chkbase(numfmt);
  var but=void(0);
  var ii=0,icode=0;
  for (ii=0;ii<mkint.maxbase;ii++) {
    icode=mkint.basecharcode[ii];
    but=mkcalc.findButtonById(mkuni.str4code(icode));
    if (but)
      but.disabled=(ii<numfmt ? false : true);
  }
  var combo=mkcalc.findComboById('base');
  if (combo) {
    var combofmt=combo.options.selectedIndex+2;
    if (numfmt!=combofmt)
      combo.options.selectedIndex=numfmt-2;
  }
};

/* ********** */
mkcalc.ctxtrig=function(trigfmt) {
  trigfmt=mkuni.chkstr(trigfmt);
  if (trigfmt.length==0)
    return;
  var radio=mkcalc.findRadioById(trigfmt);
  if (radio)
    radio.checked=true;
};

/* ********** */
mkcalc.ctxrd=function(dig) {
  dig=mkuni.chknumber(dig,mkfixed.defrd);
  var lineedit=mkcalc.findLineeditById('lerd');
  if (lineedit)
    lineedit.value=mkint.int2str(dig);
};

/* ********** */
mkcalc.ctxsci=function(sci) {
  sci=mkuni.chknumber(sci,mkcalc.fmt_defsci);
  var lineedit=mkcalc.findLineeditById('lesci');
  if (lineedit)
    lineedit.value=mkint.int2str(sci);
};

/* ********** */
mkcalc.ctxscale=function(maxdig) {
  maxdig=mkuni.chknumber(maxdig,mkfixed.maxlen);
  var lineedit=mkcalc.findLineeditById('lesc');
  if (lineedit)
    lineedit.value=mkint.int2str(maxdig);
};

/* ********** */
mkcalc.newcalculator=function() {
  mkcalc.win=window;
  mkcalc.pc=new mkcalc.Calculator(
    mkcalc.ctxout,mkcalc.ctxinfo,mkcalc.ctxbuttons,mkcalc.ctxtrig,
    mkcalc.ctxrd,mkcalc.ctxsci,mkcalc.ctxscale);
};


 