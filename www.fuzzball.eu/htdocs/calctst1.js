
function docalc() {
  var pi=3.14159265;
  var in1=document.getElementById("x1").value;
  var in2=document.getElementById("x2").value;
  var in3=document.getElementById("x3").value;
  var out=document.getElementById("x4");
  //var res=pi*parseFloat(in1)*parseFloat(in2);

  var res=parseFloat(in3)*(parseFloat(in1)+parseFloat(in2))/2.0;

  out.value=res;
}

function fxxx() {
  var in1="<label>a = </label><input type=\"text\" id=\"x1\">";
  var in2="<label>c = </label><input type=\"text\" id=\"x2\">";
  var in3="<label>h = </label><input type=\"text\" id=\"x3\">";
  var out="<label>area = </label><input type=\"text\" id=\"x4\">";
  var incalc="<input type=\"button\" value=\"rechne\" onclick=\"docalc()\">";
  
  var newtag="<form>";
  newtag=newtag+"<table>";
  newtag=newtag+"<tr><td>";
  newtag=newtag+in1;
  newtag=newtag+"</td></tr>";
  newtag=newtag+"<tr><td>";
  newtag=newtag+in2;
  newtag=newtag+"</td></tr>";
  newtag=newtag+"<tr><td>";
  newtag=newtag+in3;
  newtag=newtag+"</td></tr>";
  newtag=newtag+"<tr><td>";
  newtag=newtag+out;
  newtag=newtag+"</td></tr>";
  newtag=newtag+"<tr><td>";
  newtag=newtag+incalc;
  newtag=newtag+"</td></tr>";
  newtag=newtag+"<table>";
  newtag=newtag+"</form>";

  var id_xxx=document.getElementById("calctst1");
  id_xxx.innerHTML=newtag;
}