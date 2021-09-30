function demo_interp12(x,y,z,genspl,sbase,tbase)

format long
  
for ii=1:2
  figure(ii);
  clf("reset");
  set(gca(),'box','off');
  set(gca(),'Position',[0.05,0.05,0.9,0.9]);
  set(gca(),'LineWidth',0.1); 
  %axis off;
  hold on
end

% ----- statics
ncl=5;
nctrlL=[4,4,4,4,4];
nctrl=max(nctrlL);
ninterspline=10;
nintersplineplot=50;
nintercloudplot=1250;
% pick point to remove;
remk = 6;

xx=[4110.,4500.,7400.,9045.;
   4750.,5400.,8850.,10900.;
   5100.,6100.,10200.,12750.;
   5900.,7300.,12400.,15650.;
   6700.,9200.,14500.,18400.];
yy=[1.2,1.18,1.,0.87;
   1.4,1.35,1.12,0.88;
   1.5,1.47,1.23,0.95;
   1.7,1.66,1.39,1.08;
   1.9,1.85,1.5,1.15];
zz=[5000.,5000.,5000.,5000.;
   6000.,6000.,6000.,6000.;
   7000.,7000.,7000.,7000.;
   8000.,8000.,8000.,8000.;
   9200.,9200.,9200.,9200];
sbase = [0.,0.15,0.6,1.];
tbase = [0.;0.25;0.5;0.75;1.];
% ----- statics

% ----- functional data to interpolate
for ii=1:ncl
  for jj=1:nctrl
    ff(ii,jj)=xx(ii,jj)*xx(ii,jj)+1.7*xx(ii,jj)*yy(ii,jj)+zz(ii,jj);
  end
end
% ----- functional data to interpolate

% ----- plot data points
figure(1);
hold on;
for ii=1:ncl
  plot3(xx(ii,:),yy(ii,:),zz(ii,:),'bo');
end
figure(2);
hold on;
for ii=1:ncl
  for jj=1:nctrl
    tt(jj)=tbase(ii);
  end
  plot3(sbase,tt,ff(ii,:),'bo');
end
% ----- plot data points

% -----determine cl splines as a function of ss
weights=ones(nctrl,1);
smooth=0.0;
for ii=1:ncl
  [fail,spx(ii).nn,spx(ii).lamda,spx(ii).cc]=...
    Oe02be(nctrl,sbase,xx(ii,:),weights,smooth);
  if (length(fail)>0)
    printf ("%d, clx#%d %s",__LINE__,ii,fail);
  end
  [fail,spy(ii).nn,spy(ii).lamda,spy(ii).cc]=...
    Oe02be(nctrl,sbase,yy(ii,:),weights,smooth);
  if (length(fail)>0)
    printf ("%d, cly#%d %s",__LINE__,ii,fail);
  end
  [fail,spz(ii).nn,spz(ii).lamda,spz(ii).cc]=...
    Oe02be(nctrl,sbase,zz(ii,:),weights,smooth);
  if (length(fail)>0)
    printf ("%d, clz#%d %s",__LINE__,ii,fail);
  end
  [fail,spf(ii).nn,spf(ii).lamda,spf(ii).cc]=...
    Oe02be(nctrl,sbase,ff(ii,:),weights,smooth);
  if (length(fail)>0)
    printf ("%d, clf#%d %s",__LINE__,ii,fail);
  end
end
% -----determine cl splines as a function of ss

% -----determine cross splines as a function of tt
weights=ones(ncl,1);
smooth=0.0;
for ii=1:nctrl
  [fail,tpx(ii).nn,tpx(ii).lamda,tpx(ii).cc]=...
    Oe02be(ncl,tbase,xx(:,ii),weights,smooth);
  if (length(fail)>0)
    printf ("%d, ctrlx#%d %s",__LINE__,ii,fail);
  end
  [fail,tpy(ii).nn,tpy(ii).lamda,tpy(ii).cc]=...
    Oe02be(ncl,tbase,yy(:,ii),weights,smooth);
  if (length(fail)>0)
    printf ("%d, ctrly#%d %s",__LINE__,ii,fail);
  end
  [fail,tpz(ii).nn,tpz(ii).lamda,tpz(ii).cc]=...
    Oe02be(ncl,tbase,zz(:,ii),weights,smooth);
  if (length(fail)>0)
    printf ("%d, ctrlz#%d %s",__LINE__,ii,fail);
  end
  [fail,tpf(ii).nn,tpf(ii).lamda,tpf(ii).cc]=...
    Oe02be(ncl,tbase,ff(:,ii),weights,smooth);
  if (length(fail)>0)
    printf ("%d, ctrlf#%d %s",__LINE__,ii,fail);
  end
end
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
    end
  end
end
for ii=1:nctrl
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
    end
  end
end
% ----- interpolate ninter points along the spline

% ----- plot splined boundaries
figure(1);
hold on;
xlim ([4000,19000]);
zlim ([4500,9500]);
for ii=1:ncl
  plot3(spx(ii).yyintplot,spy(ii).yyintplot,spz(ii).yyintplot,'g-');
end
for ii=1:nctrl
  plot3(tpx(ii).yyintplot,tpy(ii).yyintplot,tpz(ii).yyintplot,'g-');
end
figure(2);
hold on;
for ii=1:ncl
  plot3(sps(ii).yyintplot,spt(ii).yyintplot,spf(ii).yyintplot,'m-');
end
for ii=1:nctrl
  plot3(tps(ii).yyintplot,tpt(ii).yyintplot,tpf(ii).yyintplot,'m-');
end
% ----- plot splined boundaries

% sort ascending according to parameter s and replace corrdinates
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
end
for ii=1:nctrl
  ss=[ss,tps(ii).yyint];
  tt=[tt,tpt(ii).yyint];
  xx=[xx,tpx(ii).yyint];
  yy=[yy,tpy(ii).yyint];
  zz=[zz,tpz(ii).yyint];
  ff=[ff,tpf(ii).yyint];
end
[ss,I]=sort(ss);
tt=tt(I);
xx=xx(I);
yy=yy(I);
zz=zz(I);
ff=ff(I);
% sort ascending according to parameter s and replace corrdinates

% -----determine parametric bicubic splines as functions from ss,tt
nctrlbc=(ncl+nctrl)*ninterspline;
weights=ones(nctrlbc,1);
smooth = 0.1;
[fail,nsx,lamdax,ntx,mux,cx,fpx] = Oe02dd(nctrlbc,ss,tt,xx,weights,smooth);
if (length(fail)>0)
  printf ("%d, %s",__LINE__,fail);
end
[fail,nsy,lamday,nty,muy,cy,fpy] = Oe02dd(nctrlbc,ss,tt,yy,weights,smooth);
if (length(fail)>0)
  printf ("%d, %s",__LINE__,fail);
end
[fail,nsz,lamdaz,ntz,muz,cz,fpz] = Oe02dd(nctrlbc,ss,tt,zz,weights,smooth);
if (length(fail)>0)
  printf ("%d, %s",__LINE__,fail);
end
smooth = 1.5;
[fail,nsf,lamdaf,ntf,muf,cf,fpf] = Oe02dd(nctrlbc,ss,tt,ff,weights,smooth);
if (length(fail)>0)
  printf ("%d, %s",__LINE__,fail);
end
% -----determine parametric bicubic splines as function from ss,tt

% ----- interpolate nintercloudplot random points along the bicubic splines
state=Og05kg(2,1);
cloudst=Og05sa(2*nintercloudplot,state);
for ii=1:nintercloudplot
  clouds(ii)=cloudst(ii);
  cloudt(ii)=cloudst(2*nintercloudplot-ii+1);
end
[fail,xbc]=Oe02de(nintercloudplot,clouds,cloudt,lamdax,mux,cx,nsx,ntx);
if (length(fail)>0)
  printf ("%d, %s",__LINE__,fail);
end
[fail,ybc] = Oe02de(nintercloudplot,clouds,cloudt,lamday,muy,cy,nsy,nty);
if (length(fail)>0)
  printf ("%d, %s",__LINE__,fail);
end
[fail,zbc] = Oe02de(nintercloudplot,clouds,cloudt,lamdaz,muz,cz,nsz,ntz);
if (length(fail)>0)
  printf ("%d, %s",__LINE__,fail);
end
[fail,fbc] = Oe02de(nintercloudplot,clouds,cloudt,lamdaf,muf,cf,nsf,ntf);
if (length(fail)>0)
  printf ("%d, %s",__LINE__,fail);
end
% ----- interpolate nintercloudplot random points along the bicubic splines

% ----- plot surface as cloud of points
figure(1);
xlim([4000,19000]);
zlim([4500,9500]);
hold on;
plot3(xbc,ybc,zbc,'*k','markersize',1);
figure(2);
hold on;
plot3(clouds,cloudt,fbc,'*','markersize',1);
% ----- plot surface as cloud of points

end

