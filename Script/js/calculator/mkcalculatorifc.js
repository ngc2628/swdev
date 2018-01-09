
mkcalc.win=void(0);
mkcalc.pc=void(0);

/* ********** */
mkcalc.findElementById=function(eid) {
  eid=mkuni.chkstr(eid);
  if (eid.length>0 && mkcalc.win)
    return mkcalc.win.document.getElementById(eid);
  return void(0);
};

/* ********** */
mkcalc.doInput=function(but) {
  var butid=but.id;
  if (mkcalc.isfnnumber(butid))
    mkcalc.pc.appendnumber(butid);
  else if (mkcalc.isfnextranumber(butid))
    mkcalc.pc.setextranumber(butid);
  else if (mkcalc.isfnoperator(butid))
    mkcalc.pc.appendoperator(butid);
  else
    mkcalc.pc.extrafunction(butid);
};

/* ********** */
mkcalc.newcalculator=function() {
  mkcalc.win=window;
  mkcalc.pc=
    new mkcalc.Calculator(mkcalc.findElementById('result'),mkcalc.findElementById('info'));

};
 