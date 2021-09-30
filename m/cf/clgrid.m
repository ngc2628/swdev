
function [clgridL,ssL,ttL] = clgrid(clL,zorw)

ssL=[];
ttL=[];
pp=[];
clgridL=struct("pp",pp,"descr",[]);

% local variables
ii=1;
jj=1;
ll=1;
spL=struct("xxint",[],"yyint",[],"der",[]);
xL=[];
yL=[];
% local variables

for ii=1:length(clL)
  ll=length(clL(ii).pp);
  for jj=1:ll
    ssL=unique([ssL,(clL(ii).pp(jj,1)-clL(ii).pp(1,1))/(clL(ii).pp(ll,1)-clL(ii).pp(1,1))]);
    xL(ii,jj)=clL(ii).pp(jj,1);
    yL(ii,jj)=clL(ii).pp(jj,2);
  endfor
  spL(ii).der=xxspline(xL(ii,:),yL(ii,:),"solve2nd","nat","");
  ttL(ii)=(ii-1.)/(length(clL)-1.);
endfor

for ii=1:length(clL)
  clgridL(ii).descr=sprintf("cl #%d",ii);
  ll=length(clL(ii).pp);
  for jj=1:length(ssL)
    clgridL(ii).pp(jj,1)=...
      clL(ii).pp(1,1)+ssL(jj)*(clL(ii).pp(ll,1)-clL(ii).pp(1,1));
    clgridL(ii).pp(jj,2)=...
      xxsplineeval(clgridL(ii).pp(jj,1),xL(ii,:),yL(ii,:),spL(ii).der,"solve2nd","nat","");
    clgridL(ii).pp(jj,3)=clL(ii).pp(1,3);
  endfor
endfor

endfunction

