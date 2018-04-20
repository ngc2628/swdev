function tst11()

% x=[4110.,4500.,7400.,9045.; 4750.,5400.,8850.,10900.; 5100.,6100.,10200.,12750.; ...
%   5900.,7300.,12400.,15650.; 6700.,9200.,14500.,18400.];
% y=[1.2,1.18,1.,0.87; 1.4,1.28,1.12,0.88; 1.5,1.47,1.23,0.95; ...
%   1.7,1.66,1.39,1.08; 1.9,1.85,1.5,0.91];
% z=[5000.,5000.,5000.,5000.; 6000.,6000.,6000.,6000.; 7000.,7000.,7000.,7000.; ...
%   8000.,8000.,8000.,8000.; 9500.,9500.,9500.,9500];

ncl=5;
nctrl=4;
ninter=50;

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
ww=[1.,1.,1.,1.];
fx=[];
fy=[];
fz=[];
smooth=0.0;

sbase = [0;1/3;2/3;1];
tbase = [0;0.25;0.5;0.75;1];

clf;
figure(1);
hold on;
for jj=1:ncl
  ss(jj,1:nctrl)=sbase;
  tt(jj,1:nctrl)=tbase(jj);
  [smmy(jj).mm,slamday(jj).lamda,sccy(jj).cc]=Oe02be(nctrl,xx(jj,:),yy(jj,:),ww,smooth);
  plot3(xx(jj,:),yy(jj,:),zz(jj,:),'+r');
end

hold on;
for jj=1:ncl
  for ii=1:ninter
    fx(ii)=xx(jj,1)+ii*(xx(jj,nctrl)-xx(jj,1))/ninter;
    fy(ii)=Oe02bb(smmy(jj).mm,slamday(jj).lamda,sccy(jj).cc,fx(ii));
    fz(ii)=zz(jj,1);
  end
  if (jj==1 || jj==ncl)
    plot3(fx,fy,fz,'-b');
  endif
end

clear smmy;
clear slamday;
clear sccy;
ww(ncl)=1.;

%figure(2);
hold on;
for jj=1:nctrl
  [smmy(jj).mm,slamday(jj).lamda,sccy(jj).cc]=Oe02be(ncl,xx(:,jj),yy(:,jj),ww,smooth);
  [smmz(jj).mm,slamdaz(jj).lamda,sccz(jj).cc]=Oe02be(ncl,xx(:,jj),zz(:,jj),ww,smooth);
  %plot3(xx(:,jj),yy(:,jj),zz(:,jj),'+');
end

hold on;
fx=[];
fy=[];
fz=[];
for jj=1:nctrl
  for ii=1:ninter
    fx(ii)=xx(1,jj)+ii*(xx(ncl,jj)-xx(1,jj))/ninter;
    fy(ii)=Oe02bb(smmy(jj).mm,slamday(jj).lamda,sccy(jj).cc,fx(ii));
    fz(ii)=Oe02bb(smmz(jj).mm,slamdaz(jj).lamda,sccz(jj).cc,fx(ii));
  end
  if (jj==1 || jj==nctrl)
    plot3(fx,fy,fz,'-b');
  endif
end


%hold on;
%for jj=1:nctrl
%  plot3(xx(:,jj),yy(:,jj),zz(:,jj),'--ko');
%end

% functional data to interpolate
%figure(2);
%hold on;
%FF = (xx.^2+2*yy.*xx) + zz;
%plot3(ss',tt',FF','-go');





end