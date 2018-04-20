function bound()

format long

% local variables
ii=0;
jj=0;
ncl=0;
nctrl=0;
ninter=20;
sbase=[];
tbase=[];
ss=[];
tt=[];
ff=[];
xx=[];
yy=[];
zz=[];
ww=[];
spx=struct("xxint",[],"yyint",[],"der",[]);
spy=struct("xxint",[],"yyint",[],"der",[]);
spz=struct("xxint",[],"yyint",[],"der",[]);
sps=struct("xxint",[],"yyint",[],"der",[]);
spf=struct("xxint",[],"yyint",[],"der",[]);
% local variables

for ii=1:2
  figure(ii);
  clf("reset");
  hold on
end


cfL=cfread("/data/versions/swdev/m/cf/cfdata.txt");
clL=cfL(1).cl;
ncl=length(clL);
for ii=1:ncl
  nctrl=length(clL(ii).pp);
  for jj=1:nctrl
    xx(ii,jj)=clL(ii).pp(jj,1);
    yy(ii,jj)=clL(ii).pp(jj,2);
    zz(ii,jj)=clL(ii).pp(jj,3);
    ww(ii,jj)=clL(ii).pp(jj,4);
  endfor
endfor
[sbase,tbase]=clngrid(clL);

% plot bounding lines and define s and t
%figure(1);
%hold on;
for jj=1:ncl
  for ii=1:nctrl
    ss(jj,ii) = sbase(ii);
    tt(jj,ii) = tbase(jj);
    % functional data to interpolate
    ff(jj,ii)=xx(jj,ii)*xx(jj,ii)+2.*xx(jj,ii)*yy(jj,ii)+zz(jj,ii);
  endfor
%  plot3(xx(jj,:),yy(jj,:),zz(jj,:),'ro');
endfor

%figure(2);
%hold on
for jj=1:nctrl
%  plot3(xx(:,jj),yy(:,jj),zz(:,jj),'ko');
endfor

%figure(3);
%hold on;
for jj=1:ncl
%  plot3(ss(jj,:),tt(jj,:),ff(jj,:),'rx');
endfor

% generate spline along each box boundary.
% splines going
% horizontally are functions of s only,
% vertically are functions of t only

for jj=1:ncl
  % determine splines as a function of s
  % and interpolate ninter points along the spline;
  spx(jj).der=xxspline(ss(jj,:),xx(jj,:),"solve2nd","nat","");
  [spx(jj).xxint,spx(jj).yyint]=...
    xxsplineinterpol(ninter,ss(jj,:),xx(jj,:),spx(jj).der,"solve2nd","nat","");
  spy(jj).der=xxspline(ss(jj,:),yy(jj,:),"solve2nd","nat","");
  [spy(jj).xxint,spy(jj).yyint]=...
    xxsplineinterpol(ninter,ss(jj,:),yy(jj,:),spy(jj).der,"solve2nd","nat","");
  spz(jj).der=xxspline(ss(jj,:),zz(jj,:),"solve2nd","nat","");
  [spz(jj).xxint,spz(jj).yyint]=...
    xxsplineinterpol(ninter,ss(jj,:),zz(jj,:),spz(jj).der,"solve2nd","nat","");
  sps(jj).xxint=sps(jj).yyint=spx(jj).xxint;
  sps(jj).yyint(1,:)=tbase(jj);
  spf(jj).der=xxspline(ss(jj,:),ff(jj,:),"solve2nd","nat","");
  [spf(jj).xxint,spf(jj).yyint]=...
    xxsplineinterpol(ninter,ss(jj,:),ff(jj,:),spf(jj).der,"solve2nd","nat","");
  % plot splined boundary
  figure(1);
  hold on;
  plot3(spx(jj).yyint,spy(jj).yyint,spz(jj).yyint,...
    'linestyle','none','linewidth',2,'marker','.','color',[0.8,0.3,0.1]);
  % plot splined function
  figure(2);
  hold on;
  plot3(sps(jj).xxint,sps(jj).yyint,spf(jj).yyint,'b-');
endfor

for jj=1:nctrl
  % determine splines as a function of t
  % and interpolate ninter points along the spline;
  spx(jj).der=xxspline(tt(:,jj),xx(:,jj),"solve2nd","nat","");
  [spx(jj).xxint,spx(jj).yyint]=...
    xxsplineinterpol(ninter,tt(:,jj),xx(:,jj),spx(jj).der,"solve2nd","nat","");

  spy(jj).der=xxspline(tt(:,jj),yy(:,jj),"solve2nd","nat","");
  [spy(jj).xxint,spy(jj).yyint]=...
    xxsplineinterpol(ninter,tt(:,jj),yy(:,jj),spy(jj).der,"solve2nd","nat","");
  spz(jj).der=xxspline(tt(:,jj),zz(:,jj),"solve2nd","nat","");
  [spz(jj).xxint,spz(jj).yyint]=...
    xxsplineinterpol(ninter,tt(:,jj),zz(:,jj),spz(jj).der,"solve2nd","nat","");
  sps(jj).xxint=sps(jj).yyint=spx(jj).xxint;
  sps(jj).xxint(:,1)=sbase(jj);
  spf(jj).der=xxspline(tt(:,jj),ff(:,jj),"solve2nd","nat","");
  [spf(jj).xxint,spf(jj).yyint]=...
    xxsplineinterpol(ninter,tt(:,jj),ff(:,jj),spf(jj).der,"solve2nd","nat","");
  % plot splined boundary
  figure(1);
  hold on;
  plot3(spx(jj).yyint,spy(jj).yyint,spz(jj).yyint,...
    'linestyle','-','linewidth',2,'marker','.','color',[0.2,0.3,0.5]);
  % plot splined function
  figure(2);
  hold on;
  plot3(sps(jj).xxint,sps(jj).yyint,spf(jj).yyint,'k-');
endfor



endfunction
