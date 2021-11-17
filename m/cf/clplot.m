function clplot()

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

% ----- locals
ninter=50;
minmax=[1.0e22,-1.0e22;1.0e22,-1.0e22;1.0e22,-1.0e22];
pp=[];
clL=struct("pp",pp,"descr",[]);
cfL=struct("cl",clL,"descr",[]);
clgridL=struct("pp",pp,"descr",[]);
ssL=[];
ttL=[];
inter=[];
interxL=[];
interyL=[];
intersL=struct("interx",interxL,"intery",interyL);
intertL=struct("interx",interxL,"intery",interyL);
der=[];
ii=0;
jj=0;
kk=0;
% ----- locals

cfL=cfread("cfdata.txt");
clL=cfL(1).clL;
[clgridL,ssL,ttL]=clgrid(clL);

% ----- grid bounds
for ii=1:length(clgridL)
  for jj=1:3
    if (min(clgridL(ii).pp(:,jj)')<minmax(jj,1))
      minmax(jj,1)=min(clgridL(ii).pp(:,jj)');
    endif
    if (max(clgridL(ii).pp(:,jj)')>minmax(jj,2))
      minmax(jj,2)=max(clgridL(ii).pp(:,jj)');
    endif
  endfor
endfor
for jj=1:3
  minmax(jj,1)-=realpow(10.0,log10(minmax(jj,2)-minmax(jj,1))-1.5);
  minmax(jj,2)+=realpow(10.0,log10(minmax(jj,2)-minmax(jj,1))-1.5);
endfor
% ----- grid bounds

% ----- interpol clL parametric along ss
for ii=1:length(clgridL)
  for jj=1:3
    der=xxspline(ssL,clgridL(ii).pp(:,jj)',"solve2nd","nat","");
    [intersL(ii).interxL(jj,:),intersL(ii).interyL(jj,:)]=...
      xxsplineinterpol(ninter,ssL,clgridL(ii).pp(:,jj)',der,"solve2nd","nat","");
  endfor
endfor
% ----- interpol clL parametric along ss

% ----- interpol cross-lines along tt
for ii=1:length(ssL)
  for jj=1:3
    for kk=1:length(clgridL)
      inter(jj,kk)=clgridL(kk).pp(ii,jj);
    endfor
  endfor
  for jj=1:3
    der=xxspline(ttL,inter(jj,:),"solve2nd","nat","");
    [intertL(ii).interxL(jj,:),intertL(ii).interyL(jj,:)]=...
      xxsplineinterpol(ninter,ttL,inter(jj,:),der,"solve2nd","nat","");
  endfor
endfor
% ----- interpol cross-lines along tt

% ----- plot gridpoints
figure(1);
xlim(minmax(1,:));
ylim(minmax(2,:));
zlim(minmax(3,:));
hold on;
for ii=1:length(clgridL)
  plot3(clgridL(ii).pp(:,1)',clgridL(ii).pp(:,2)',clgridL(ii).pp(:,3)',...
    'linestyle','none','marker','o','markersize',3,'color',[0.7,0.6,0.2]);
  plot3(clL(ii).pp(:,1)',clL(ii).pp(:,2)',clL(ii).pp(:,3)',...
    'linestyle','none','linewidth',2,'marker','+','markersize',5,'color',[0.9,0.1,0.4]);
endfor
% ----- plot gridpoints

% ----- plot interpol cl(ss)
figure(1);
hold on;
for ii=1:length(clgridL)
  plot3(intersL(ii).interyL(1,:),intersL(ii).interyL(2,:),intersL(ii).interyL(3,:),...
    'linestyle','none','linewidth',2,'marker','.','markersize',5,'color',[0.2,0.3,0.5]);
endfor
% ----- plot interpol cl(ss)

% ----- plot interpol cross(tt)
figure(1);
hold on;
for ii=1:length(ssL)
  plot3(intertL(ii).interyL(1,:),intertL(ii).interyL(2,:),intertL(ii).interyL(3,:),...
    'linestyle','none','linewidth',1,'marker','.','markersize',3,'color',[0.5,0.5,0.5]);
endfor
% ----- plot interpol cross(tt)













endfunction
