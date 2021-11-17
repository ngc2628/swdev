function demo_interp14(cl)

format long
  
for ii=1:2
  figure(ii);
  clf("reset");
  set(gca(),'box','off');
  set(gca(),'Position',[0.05,0.05,0.9,0.9]);
  set(gca(),'LineWidth',0.3); 
  %axis off;
  hold on
endfor

% ----- statics
ninterspline=10;
nintersplineplot=50;
nintercloudplot=1250;
% pick point to remove;
remk = 6;
% ----- statics

ncl=length(cl);
[sbase,tbase]=demo_stgrid(cl);

% bad !
nmaxctrl=0;
for ii=1:ncl
  if (nmaxctrl<cl(ii).nctrl)
    nmaxctrl=cl(ii).nctrl;
  endif
endfor
% bad !

% ----- functional data to interpolate
for ii=1:ncl
  ff(ii).nctrl=cl(ii).nctrl;
  for jj=1:ff(ii).nctrl
    ff(ii).pp(jj).xx=sbase(jj);
    ff(ii).pp(jj).yy=tbase(ii);
    ff(ii).pp(jj).zz=cl(ii).pp(jj).xx*cl(ii).pp(jj).xx/4.0+...
              1.7*cl(ii).pp(jj).xx*cl(ii).pp(jj).yy+cl(ii).pp(jj).zz;
  endfor
endfor
% ----- functional data to interpolate

% ----- plot data points
minmaxx=[1.0e22,-1.0e22];
minmaxy=[1.0e22,-1.0e22];
minmaxz=[1.0e22,-1.0e22];
figure(1);
hold on;
for ii=1:ncl
  for jj=1:cl(ii).nctrl
    xx(jj)=cl(ii).pp(jj).xx;
    yy(jj)=cl(ii).pp(jj).yy;
    zz(jj)=cl(ii).pp(jj).zz;
  endfor
  if (min(xx)<minmaxx(1))
    minmaxx(1)=min(xx);
  endif
  if (min(yy)<minmaxy(1))
    minmaxy(1)=min(yy);
  endif
  if (min(zz)<minmaxz(1))
    minmaxz(1)=min(zz);
  endif
  if (max(xx)>minmaxx(2))
    minmaxx(2)=max(xx);
  endif
  if (max(yy)>minmaxy(2))
    minmaxy(2)=max(yy);
  endif
  if (max(zz)>minmaxz(2))
    minmaxz(2)=max(zz);
  endif
  plot3(xx,yy,zz,'linestyle','none','marker','o','color',[0.7,0.6,0.2]);
endfor
figure(2);
hold on;
for ii=1:ncl
  for jj=1:ff(ii).nctrl
    xx(jj)=ff(ii).pp(jj).xx;
    yy(jj)=ff(ii).pp(jj).yy;
    zz(jj)=ff(ii).pp(jj).zz;
  endfor
  plot3(xx,yy,zz,'linestyle','none','marker','o','color',[0.3,0.5,0.7]);
endfor
% ----- plot data points

% -----determine cl splines as a function of ss
smooth=0.0;
for ii=1:ncl
  for jj=1:cl(ii).nctrl
    weights(jj)=1.0;
    xx(jj)=sbase(jj);
    yy(jj)=cl(ii).pp(jj).xx;
  endfor
  [fail,spx(ii).nn,spx(ii).lamda,spx(ii).cc]=...
    Oe02be(cl(ii).nctrl,xx,yy,weights,smooth);
  if (length(fail)>0)
    printf ("%d, clx#%d %s",__LINE__,ii,fail);
  endif
  
  for jj=1:cl(ii).nctrl
    yy(jj)=cl(ii).pp(jj).yy;
  endfor
  [fail,spy(ii).nn,spy(ii).lamda,spy(ii).cc]=...
    Oe02be(cl(ii).nctrl,xx,yy,weights,smooth);
  if (length(fail)>0)
    printf ("%d, cly#%d %s",__LINE__,ii,fail);
  endif

  for jj=1:cl(ii).nctrl
    yy(jj)=cl(ii).pp(jj).zz;
  endfor
  [fail,spz(ii).nn,spz(ii).lamda,spz(ii).cc]=...
    Oe02be(cl(ii).nctrl,xx,yy,weights,smooth);
  if (length(fail)>0)
    printf ("%d, clz#%d %s",__LINE__,ii,fail);
  endif

  for jj=1:ff(ii).nctrl
    yy(jj)=ff(ii).pp(jj).zz;
  endfor
  [fail,spf(ii).nn,spf(ii).lamda,spf(ii).cc]=...
    Oe02be(cl(ii).nctrl,xx,yy,weights,smooth);
  if (length(fail)>0)
    printf ("%d, clf#%d %s",__LINE__,ii,fail);
  endif
endfor
% -----determine cl splines as a function of ss

% -----determine cross splines as a function of tt
for ii=1:nmaxctrl
  for jj=1:ncl
    weights(jj)=1.0;
    xx(jj)=tbase(jj);
    yy(jj)=cl(jj).pp(ii).xx;
  endfor
  [fail,tpx(ii).nn,tpx(ii).lamda,tpx(ii).cc]=...
    Oe02be(ncl,xx,yy,weights,smooth);
  if (length(fail)>0)
    printf ("%d, ctrlx#%d %s",__LINE__,ii,fail);
  endif

  for jj=1:ncl
    yy(jj)=cl(jj).pp(ii).yy;
  endfor
  [fail,tpy(ii).nn,tpy(ii).lamda,tpy(ii).cc]=...
    Oe02be(ncl,xx,yy,weights,smooth);
  if (length(fail)>0)
    printf ("%d, ctrlx#%d %s",__LINE__,ii,fail);
  endif  

  for jj=1:ncl
    yy(jj)=cl(jj).pp(ii).zz;
  endfor
  [fail,tpz(ii).nn,tpz(ii).lamda,tpz(ii).cc]=...
    Oe02be(ncl,xx,yy,weights,smooth);
  if (length(fail)>0)
    printf ("%d, ctrlx#%d %s",__LINE__,ii,fail);
  endif 

  for jj=1:ncl
    yy(jj)=ff(jj).pp(ii).zz;;
  endfor
  [fail,tpf(ii).nn,tpf(ii).lamda,tpf(ii).cc]=...
    Oe02be(ncl,xx,yy,weights,smooth);
  if (length(fail)>0)
    printf ("%d, ctrlx#%d %s",__LINE__,ii,fail);
  endif
endfor
% -----determine cross splines as a function of tt

% ----- interpolate ninter and ninterplot points along the splines
xxint=linspace(0,1-1.0e-8,ninterspline);
xxintplot=linspace(0,1-1.0e-8,nintersplineplot);
for ii=1:ncl
  for jj=1:nintersplineplot
    sps(ii).yyintplot(jj)=xxintplot(jj);
    spt(ii).yyintplot(jj)=tbase(ii);
    [fail,spx(ii).yyintplot(jj)]=...
      Oe02bb(spx(ii).nn,spx(ii).lamda,spx(ii).cc,xxintplot(jj));
    [fail,spy(ii).yyintplot(jj)]=...
      Oe02bb(spy(ii).nn,spy(ii).lamda,spy(ii).cc,xxintplot(jj));
    [fail,spz(ii).yyintplot(jj)]=...
      Oe02bb(spz(ii).nn,spz(ii).lamda,spz(ii).cc,xxintplot(jj));
    [fail,spf(ii).yyintplot(jj)]=...
      Oe02bb(spf(ii).nn,spf(ii).lamda,spf(ii).cc,xxintplot(jj));
    if (jj<=ninterspline)
      sps(ii).yyint(jj)=xxint(jj);
      spt(ii).yyint(jj)=tbase(ii);
      [fail,spx(ii).yyint(jj)]=...
        Oe02bb(spx(ii).nn,spx(ii).lamda,spx(ii).cc,xxint(jj));
      [fail,spy(ii).yyint(jj)]=...
        Oe02bb(spy(ii).nn,spy(ii).lamda,spy(ii).cc,xxint(jj));
      [fail,spz(ii).yyint(jj)]=...
        Oe02bb(spz(ii).nn,spz(ii).lamda,spz(ii).cc,xxint(jj));
      [fail,spf(ii).yyint(jj)]=...
        Oe02bb(spf(ii).nn,spf(ii).lamda,spf(ii).cc,xxint(jj));
    endif
  endfor
endfor
for ii=1:nmaxctrl
  for jj=1:nintersplineplot
    tps(ii).yyintplot(jj)=sbase(ii);
    tpt(ii).yyintplot(jj)=xxintplot(jj);
    [fail,tpx(ii).yyintplot(jj)]=...
      Oe02bb(tpx(ii).nn,tpx(ii).lamda,tpx(ii).cc,xxintplot(jj));
    [fail,tpy(ii).yyintplot(jj)]=...
      Oe02bb(tpy(ii).nn,tpy(ii).lamda,tpy(ii).cc,xxintplot(jj));
    [fail,tpz(ii).yyintplot(jj)]=...
      Oe02bb(tpz(ii).nn,tpz(ii).lamda,tpz(ii).cc,xxintplot(jj));
    [fail,tpf(ii).yyintplot(jj)]=...
      Oe02bb(tpf(ii).nn,tpf(ii).lamda,tpf(ii).cc,xxintplot(jj));
    if (jj<=ninterspline)
      tps(ii).yyint(jj)=sbase(ii);
      tpt(ii).yyint(jj)=xxint(jj);
      [fail,tpx(ii).yyint(jj)]=...
        Oe02bb(tpx(ii).nn,tpx(ii).lamda,tpx(ii).cc,xxint(jj));
      [fail,tpy(ii).yyint(jj)]=...
        Oe02bb(tpy(ii).nn,tpy(ii).lamda,tpy(ii).cc,xxint(jj));
      [fail,tpz(ii).yyint(jj)]=...
        Oe02bb(tpz(ii).nn,tpz(ii).lamda,tpz(ii).cc,xxint(jj));
      [fail,tpf(ii).yyint(jj)]=...
        Oe02bb(tpf(ii).nn,tpf(ii).lamda,tpf(ii).cc,xxint(jj));
    endif
  endfor
endfor
% ----- interpolate ninter points along the spline

% ----- plot splined boundaries
figure(1);
hold on;
xlim ([4000,19000]);
zlim ([4500,9500]);
for ii=1:ncl
  plot3(spx(ii).yyintplot,spy(ii).yyintplot,spz(ii).yyintplot,...
    'linestyle','--','marker','none','color',[0.7,0.6,0.2]);
endfor
for ii=1:nmaxctrl
  plot3(tpx(ii).yyintplot,tpy(ii).yyintplot,tpz(ii).yyintplot,...
    'linestyle','--','marker','none','color',[0.7,0.6,0.2]);
endfor
figure(2);
hold on;
for ii=1:ncl
  plot3(sps(ii).yyintplot,spt(ii).yyintplot,spf(ii).yyintplot,...
    'linestyle','--','marker','none','color',[0.3,0.5,0.7]);
endfor
for ii=1:nmaxctrl
  plot3(tps(ii).yyintplot,tpt(ii).yyintplot,tpf(ii).yyintplot,...
    'linestyle','--','marker','none','color',[0.3,0.5,0.7]);
endfor
% ----- plot splined boundaries

% sort ascending according to parameter ss and replace corrdinates
ss=[];
tt=[];
xx=[];
yy=[];
zz=[];
ff=[];
for ii=1:ncl
  ss=[ss,sps(ii).yyint];
  tt=[tt,spt(ii).yyint];
  xx=[xx,spx(ii).yyint];
  yy=[yy,spy(ii).yyint];
  zz=[zz,spz(ii).yyint];
  ff=[ff,spf(ii).yyint];
endfor
for ii=1:nmaxctrl
  ss=[ss,tps(ii).yyint];
  tt=[tt,tpt(ii).yyint];
  xx=[xx,tpx(ii).yyint];
  yy=[yy,tpy(ii).yyint];
  zz=[zz,tpz(ii).yyint];
  ff=[ff,tpf(ii).yyint];
endfor
[ss,I]=sort(ss);
tt=tt(I);
xx=xx(I);
yy=yy(I);
zz=zz(I);
ff=ff(I);
% sort ascending according to parameter s and replace corrdinates

% -----determine parametric bicubic splines as functions from ss,tt
nctrlbc=(ncl+nmaxctrl)*ninterspline;
weights=ones(nctrlbc,1);
smooth = 5.0e3;
[fail,nsx,lamdax,ntx,mux,cx,fpx] = Oe02dd(nctrlbc,ss,tt,xx,weights,smooth);
if (length(fail)>0)
  printf ("%d, [nx=%d,ny=%d] %s",__LINE__,nsx,ntx,fail);
endif
[fail,nsy,lamday,nty,muy,cy,fpy] = Oe02dd(nctrlbc,ss,tt,yy,weights,smooth);
if (length(fail)>0)
  printf ("%d, [nx=%d,ny=%d] %s",__LINE__,nsy,nty,fail);
endif
[fail,nsz,lamdaz,ntz,muz,cz,fpz] = Oe02dd(nctrlbc,ss,tt,zz,weights,smooth);
if (length(fail)>0)
  printf ("%d, [nx=%d,ny=%d] %s",__LINE__,nsz,ntz,fail);
endif
smooth = 1.0e11;
[fail,nsf,lamdaf,ntf,muf,cf,fpf] = Oe02dd(nctrlbc,ss,tt,ff,weights,smooth);
if (length(fail)>0)
  printf ("%d, [nx=%d,ny=%d] %s",__LINE__,nsf,ntf,fail);
endif
% -----determine parametric bicubic splines as function from ss,tt

% ----- interpolate nintercloudplot random points along the bicubic splines
[fail,state]=Og05kg(2,1);
[fail,cloudst]=Og05sa(2*nintercloudplot,state);
for ii=1:nintercloudplot
  clouds(ii)=cloudst(ii);
  cloudt(ii)=cloudst(2*nintercloudplot-ii+1);
endfor
[fail,xbc]=Oe02de(nintercloudplot,clouds,cloudt,lamdax,mux,cx,nsx,ntx);
if (length(fail)>0)
  printf ("%d, %s",__LINE__,fail);
endif
[fail,ybc] = Oe02de(nintercloudplot,clouds,cloudt,lamday,muy,cy,nsy,nty);
if (length(fail)>0)
  printf ("%d, %s",__LINE__,fail);
endif
[fail,zbc] = Oe02de(nintercloudplot,clouds,cloudt,lamdaz,muz,cz,nsz,ntz);
if (length(fail)>0)
  printf ("%d, %s",__LINE__,fail);
endif
[fail,fbc] = Oe02de(nintercloudplot,clouds,cloudt,lamdaf,muf,cf,nsf,ntf);
if (length(fail)>0)
  printf ("%d, %s",__LINE__,fail);
endif
% ----- interpolate nintercloudplot random points along the bicubic splines

% ----- plot surface as cloud of points
figure(1);
minmaxx(1)-=realpow(10.0,log10(minmaxx(2)-minmaxx(1))-1.5);
minmaxy(1)-=realpow(10.0,log10(minmaxy(2)-minmaxy(1))-1.5);
minmaxz(1)-=realpow(10.0,log10(minmaxz(2)-minmaxz(1))-1.5);
minmaxx(2)+=realpow(10.0,log10(minmaxx(2)-minmaxx(1))-1.5);
minmaxy(2)+=realpow(10.0,log10(minmaxy(2)-minmaxy(1))-1.5);
minmaxz(2)+=realpow(10.0,log10(minmaxz(2)-minmaxz(1))-1.5);
xlim(minmaxx);
ylim(minmaxy);
zlim(minmaxz);
hold on;
plot3(xbc,ybc,zbc,...
  'linestyle','none','marker','*','markersize',3,'color',[0.5,0.6,0.5]);
figure(2);
hold on;
plot3(clouds,cloudt,fbc,...
  'linestyle','none','marker','*','markersize',1,'color',[0.3,0.8,0.2]);
% ----- plot surface as cloud of points

% ----- using unconstrained sum of squares minimization, providing derivatives
% ----- rough guess of the position of [s0,t0] 

x0a=[cl(2).pp(3).xx+100,cl(2).pp(3).yy+0.5,0.0];
figure(1);
hold on;
plot3(x0a(1),x0a(2),x0a(3),...
  'linestyle','none','marker','o','markersize',7,'markerfacecolor',[0.1,0.3,0.5],'color',[0.1,0.3,0.5]);
ststart=[0.5,0.5];
[fail,sta]=Oe04gb(x0a,ststart,nsx,ntx,lamdax,mux,cx,nsy,nty,lamday,muy,cy,nsz,ntz,lamdaz,muz,cz);
if (length(fail)>0)
  printf ("%d, %s",__LINE__,fail);
endif

% ----- plot where the minimization believes the point is
[fail,xxmin] = Oe02de(1,sta(1),sta(2),lamdax,mux,cx,nsx,ntx);
if (length(fail)>0)
  printf ("%d, %s",__LINE__,fail);
endif
[fail,yymin] = Oe02de(1,sta(1),sta(2),lamday,muy,cy,nsy,nty);
if (length(fail)>0)
  printf ("%d, %s",__LINE__,fail);
endif
[fail,zzmin] = Oe02de(1,sta(1),sta(2),lamdaz,muz,cz,nsz,ntz);
if (length(fail)>0)
  printf ("%d, %s",__LINE__,fail);
endif
xmin=[xxmin(1),yymin(1),zzmin(1)];

plot3(xmin(1),xmin(2),xmin(3),...
  'linestyle','none','marker','o','markersize',7,'markerfacecolor',[0.7,0.1,0.1],'color',[0.7,0.5,0.3]);


x0b=[cl(3).pp(2).xx+100.0,cl(3).pp(2).yy+1.0,cl(3).pp(2).zz+1.0];
figure(1);
hold on;
%plot3(x0b(1),x0b(2),x0b(3),...
%  'linestyle','none','marker','o','markersize',7,'markerfacecolor',[0.1,0.3,0.5],'color',[0.1,0.3,0.5]);
ststart=[0.5,0.5];
[fail,stb]=Oe04gba(x0b,ststart,nsx,ntx,lamdax,mux,cx,nsy,nty,lamday,muy,cy,nsz,ntz,lamdaz,muz,cz);
if (length(fail)>0)
  printf ("%d, %s",__LINE__,fail);
endif

% ----- plot where the minimization believes the point is
[fail,xxmin] = Oe02de(1,stb(1),stb(2),lamdax,mux,cx,nsx,ntx);
if (length(fail)>0)
  printf ("%d, %s",__LINE__,fail);
endif
[fail,yymin] = Oe02de(1,stb(1),stb(2),lamday,muy,cy,nsy,nty);
if (length(fail)>0)
  printf ("%d, %s",__LINE__,fail);
endif
[fail,zzmin] = Oe02de(1,stb(1),stb(2),lamdaz,muz,cz,nsz,ntz);
if (length(fail)>0)
  printf ("%d, %s",__LINE__,fail);
endif
xmin=[xxmin(1),yymin(1),zzmin(1)];

% plot3(xmin(1),xmin(2),xmin(3),...
%  'linestyle','none','marker','o','markersize',7,'markerfacecolor',[0.7,0.1,0.1],'color',[0.7,0.5,0.3]);
xmin


endfunction
