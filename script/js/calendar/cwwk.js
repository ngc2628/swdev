
var cwwk={

daysinmonth: Uint8Array([0,31,28,31,30,31,30,31,31,30,31,30,31]),

isLeapYear: function(yy) {

  var ans=new Uint8Array([0]);
  var idx=new Uint16Array([yy]);

  if (idx[0]>0 && (idx[0]%4)==0 && ((idx[0]%100)!=0 || (idx[0]%400)==0))
    ans[0]=1;

  return ans[0];

},

daysInMonth: function(yy,mm) {

  var ans=new Uint8Array([0]);
  var idx=new Uint16Array([yy,mm]);

  if (idx[1]>12)
    return ans[0];

  idx[0]=this.isLeapYear(idx[0]);
  ans[0]=this.daysinmonth[idx[1]];
  if (idx[0]==1 && idx[1]==2)
    ans[0]+=1;

  return ans[0];

},

wdjan1: function(yy) {



},

yd2md: function(yy,yd) {

  var ans=new Uint16Array([yd,this.isLeapYear(yy)==1 ? 366 : 365]);
  var ii=12;

  while (ans[1]>ans[0])
    ans[1]-=this.daysInMonth(yy,ii--);

  ans[0]=(ans[0]-ans[1]);
  ans[1]=ii+1;

  return ans;

},

cwwk2md: function(yy,cw,wk) {

  var chkdate=new Date(yy,0,1,0,0,0,0);
  var ans=new Uint16Array([0,0,0]);

  ans[0]=chkdate.getDay();
  if (ans[0]==5)
    ans[0]=10;
  else if (ans[0]==6)
    ans[0]=9;
  else
    ans[0]=8-ans[0];

  ans[1]=1;
  ans[2]=cw;
  while (ans[1]<ans[2]) {
    ans[0]+=7;
    ans[1]+=1;
  }

  ans[2]=ans[0];
  var md=this.yd2md(yy,ans[2]);
  ans[0]=md[0];
  ans[1]=md[1];

  return ans;

}

}


var ans=cwwk.cwwk2md(2015,33,1);
print (new Error().lineNumber+" ["+ans[0]+","+ans[1]+","+ans[2]+"]");


