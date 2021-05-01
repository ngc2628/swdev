function tst14()
  
format long
  
for jj=1:7
  figure(jj);
  clf("reset");
  hold on
end
  
% pick point to remove;
remk = 6;

ncl=5;
nctrl=4;
ninter=10;

x=[4110.,4500.,7400.,9045.,4750.,5400.,8850.,10900.,5100.,6100.,10200.,12750.,...
   5900.,7300.,12400.,15650.,6700.,9200.,14500.,18400.];
y=[1.2,1.18,1.,0.87,1.4,1.35,1.12,0.88,1.5,1.47,1.23,0.95,...
   1.7,1.66,1.39,1.08, 1.9,1.85,1.5,0.91];
z=[5000.,5000.,5000.,5000.,6000.,6000.,6000.,6000.,7000.,7000.,7000.,7000.,...
   8000.,8000.,8000.,8000.,9500.,9500.,9500.,9500];

xx=[4110.,4500.,7400.,9045.;
   4750.,5400.,8850.,10900.;
   5100.,6100.,10200.,12750.;
   5900.,7300.,12400.,15650.;
   6700.,9200.,14500.,18400.];
yy=[1.2,1.18,1.,0.87;
   1.4,1.35,1.12,0.88;
   1.5,1.47,1.23,0.95;
   1.7,1.66,1.39,1.08;
   1.9,1.85,1.5,0.91];
zz=[5000.,5000.,5000.,5000.;
   6000.,6000.,6000.,6000.;
   7000.,7000.,7000.,7000.;
   8000.,8000.,8000.,8000.;
   9500.,9500.,9500.,9500];
sbase = [0.;.333333;.666666;1.];
tbase = [0.;0.25;0.5;0.75;1.];
ss=[];
tt=[];
ff=[];

% plot bounding lines and define s and t
figure(1);
hold on;
for jj=1:ncl
  for ii=1:nctrl
    ss(jj,ii) = sbase(ii);
    tt(jj,ii) = tbase(jj);
    % functional data to interpolate
    ff(jj,ii)=xx(jj,ii)*xx(jj,ii)+2.*xx(jj,ii)*yy(jj,ii)+zz(jj,ii);
  end
  plot3(xx(jj,:),yy(jj,:),zz(jj,:),'ro');
end

figure(2);
hold on
for jj=1:nctrl
  plot3(xx(:,jj),yy(:,jj),zz(:,jj),'ko');
end

figure(3);
hold on;
for jj=1:ncl
  plot3(ss(jj,:),tt(jj,:),ff(jj,:),'rx');
end

% generate spline along each box boundary.
% splines going
% horizontally are functions of s only,
% vertically are functions of t only
spx = [];
spy = [];
spz = [];
spf = [];
sps = [];
spt = [];
speval=0;

%nag spline parameter
nx=0;
ny=0;
nz=0;
nf=0;
lamdax=[];
lamday=[];
lamdaz=[];
lamdaf=[];
coeffx=[];
coeffy=[];
coeffz=[];
coefff=[];
weights=[];
smooth=0.00001;

for jj=1:ncl
  % determine splines as a function of s
  weights=ones(nctrl,1);
  [nx,lamdax,coeffx]=Oe02be(nctrl,ss(jj,:),xx(jj,:),weights,smooth);
  [ny,lamday,coeffy]=Oe02be(nctrl,ss(jj,:),yy(jj,:),weights,smooth);
  [nz,lamdaz,coeffz]=Oe02be(nctrl,ss(jj,:),zz(jj,:),weights,smooth);
  [nf,lamdaf,coefff]=Oe02be(nctrl,ss(jj,:),ff(jj,:),weights,smooth);
  % interpolate ninter points along the spline;
  for kk = 1:ninter
    speval=kk/ninter;
    spx(jj,kk)=Oe02bb(nx,lamdax,coeffx,speval);
    spy(jj,kk)=Oe02bb(ny,lamday,coeffy,speval);
    spz(jj,kk)=Oe02bb(nz,lamdaz,coeffz,speval);
    sps(jj,kk)=speval;
    spt(jj,kk)=tbase(jj);
    spf(jj,kk)=Oe02bb(nf,lamdaf,coefff,speval);
  end
  % plot splined boundary
  figure(4);
  hold on;
  plot3(spx(jj,:),spy(jj,:),spz(jj,:),'r-x');
  % plot splined function
  figure(5);
  hold on;
  plot3(sps(jj,:),spt(jj,:),spf(jj,:),'b-x');
  if(jj<4)
    spx=spy=spz=spf=sps=spt=lamdax=lamday=lamdaz=lamdaf=coeffx=coeffy=coeffz=coefff=weights=[];
    weights=ones(ncl,1);
    [nx,lamdax,coeffx]=Oe02be(ncl,tt(:,jj),xx(:,jj),weights,smooth);
    [ny,lamday,coeffy]=Oe02be(ncl,tt(:,jj),yy(:,jj),weights,smooth);
    [nz,lamdaz,coeffz]=Oe02be(ncl,tt(:,jj),zz(:,jj),weights,smooth);
fff=[1,2,3,4,5];
    [nf,lamdaf,coefff]=Oe02be(ncl,tt(:,jj),fff,weights,smooth);
    for kk=1:ninter
      speval=kk/ninter;
      spx(jj,kk)=Oe02bb(nx,lamdax,coeffx,speval);
      spy(jj,kk)=Oe02bb(ny,lamday,coeffy,speval);
      spz(jj,kk)=Oe02bb(nz,lamdaz,coeffz,speval);
      sps(jj,kk)=sbase(jj);
      spt(jj,kk)=speval;
      spf(jj,kk)=Oe02bb(nf,lamdaf,coefff,speval);
    end
    % plot splined boundary
    figure(4);
    hold on;
    plot3(spx(jj,:),spy(jj,:),spz(jj,:),'k-x');
    % plot splined function
    figure(7);
    hold on;
    plot3(sps(jj,:),spt(jj,:),spz(jj,:),'k-x');
    spx=spy=spz=spf=sps=spt=lamdax=lamday=lamdaz=lamdaf=coeffx=coeffy=coeffz=coefff=weights=[];
  end
end

spx=spy=spz=spf=sps=spt=lamdax=lamday=lamdaz=lamdaf=coeffx=coeffy=coeffz=coefff=weights=[];

end
