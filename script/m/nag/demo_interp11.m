function demo_interp11(x,y,z,genspl,sbase,tbase)

format long
  
for jj=1:2
  figure(jj);
  clf("reset");
  set(gca(),'box','off');
  set(gca(),'Position',[0,0,1,1]);
  set(gca(),'LineWidth',0.1); 
  %axis off;
  hold on
end


ninter=20;
plotrand=1200;

% pick point to remove;
remk = 6;

% x=[4110.,4500.,7400.,9045.; 4750.,5400.,8850.,10900.; 5100.,6100.,10200.,12750.; ...
%   5900.,7300.,12400.,15650.; 6700.,9200.,14500.,18400.];
% y=[1.2,1.18,1.,0.87; 1.4,1.28,1.12,0.88; 1.5,1.47,1.23,0.95; ...
%   1.7,1.66,1.39,1.08; 1.9,1.85,1.5,0.91];
% z=[5000.,5000.,5000.,5000.; 6000.,6000.,6000.,6000.; 7000.,7000.,7000.,7000.; ...
%   8000.,8000.,8000.,8000.; 9500.,9500.,9500.,9500];

x=[4110.,4500.,7400.,9045.,4750.,5400.,8850.,10900.,5100.,6100.,10200.,12750.,...
   5900.,7300.,12400.,15650.,6700.,9200.,14500.,18400.];
y=[1.2,1.18,1.,0.87,1.4,1.35,1.12,0.88,1.5,1.47,1.23,0.95,...
   1.7,1.66,1.39,1.08, 1.9,1.85,1.5,1.15];
z=[5000.,5000.,5000.,5000.,6000.,6000.,6000.,6000.,7000.,7000.,7000.,7000.,...
   8000.,8000.,8000.,8000.,9200.,9200.,9200.,9200];

% map x,y,theta to x(st),y(s,t),theta(s,t);

if(nargin<5)
    %sbase = [0.;.333333;.666666;1.];
    sbase = [0.;.15;.6;1.];
    %tbase = [0.;0.25;0.5;0.75;1.];
    tbase = [0.;.25;.5;.75;1.];
end
% plot bounding lines and define s and t
figure(1);
hold on;
for j=1:5
    k = (j-1)*4 +(1:4);
    s(k) = sbase;
    t(k) = tbase(j);
    plot3(x(k),y(k),z(k),'ro');
end
for j=1:4
    k = (j-1)+1:4:20;
    plot3(x(k),y(k),z(k),'ko');
end

% functional data to interpolate
figure(2);
hold on;
F = (x.^1.7+2.*y.*x) + z;
plot3(s,t,F,'ro');

% generate spline along each box boundary.
% splines going horizontally are functions of s only, vertically are
% functions of t only
spx = [];
spy = [];
spt = [];
spf = [];
sps = [];
spt = [];
pspx = [];
pspy = [];
pspt = [];
pspf = [];
psps = [];
pspt = [];

smooth = .0;
pos = 0;
for j=1:5
    k = (j-1)*4+(1:4);
    n = int64(0);
    m = int64(4);
    nest = m+4;
    wrk = zeros(4*m+16*nest+41,1);
    iwrk = int64(zeros(nest,1));
    w = ones(m,1);
    lamda = zeros(nest,1);
    % determine splines as a function of s
    [fail,nX, lamdaX, cX] = Oe02be(m,s(k), x(k), w, smooth);
    if (length(fail)>0)
      printf ("%d, j=%d %s",__LINE__,j,fail);
    end
    [fail,nY, lamdaY, cY] = Oe02be(m, s(k), y(k), w, smooth);
    if (length(fail)>0)
      printf ("%d, j=%d %s",__LINE__,j,fail);
    end
    [fail,nTh, lamdaTh, cTh] = Oe02be(m, s(k), z(k), w, smooth);
    if (length(fail)>0)
      printf ("%d, j=%d %s",__LINE__,j,fail);
    end
    [fail,nF, lamdaF, cF] = Oe02be(m, s(k), F(k), w, smooth);
    if (length(fail)>0)
      printf ("%d, j=%d %s",__LINE__,j,fail);
    end
    % interpolate 20 points along the spline;
    sk = linspace(0,1-1.0e-8,ninter);
    tk = tbase(j);
    for k = 1:ninter
        sps(k+pos) = sk(k);
        spt(k+pos) = tk;
        [fail,spX(pos+k)]=Oe02bb(nX,lamdaX,cX,sk(k));
        if (length(fail)>0)
          printf ("%d, j=%d %s",__LINE__,j,fail);
        end
        [fail,spY(pos+k)]=Oe02bb(nY,lamdaY,cY,sk(k));
        if (length(fail)>0)
          printf ("%d, j=%d %s",__LINE__,j,fail);
        end
        [fail,spTh(pos+k)]=Oe02bb(nTh,lamdaTh,cTh,sk(k));
        if (length(fail)>0)
          printf ("%d, j=%d %s",__LINE__,j,fail);
        end
        [fail,spF(pos+k)]=Oe02bb(nF,lamdaF,cF,sk(k));
        if (length(fail)>0)
          printf ("%d, j=%d %s",__LINE__,j,fail);
        end
    end
    % plot splined boundary
    figure(1);
    hold on;
    plot3(spX(pos+(1:20)),spY(pos+(1:20)),spTh(pos+(1:20)),'g-');
    figure(2);
    hold on;
    % plot splined function
    plot3(sps(pos+(1:ninter)),spt(pos+(1:ninter)),spF(pos+(1:ninter)),'r-');
    pos = pos+ninter;

    if(j<5)
        k = (j-1)+1:4:(20+(j-1));
        tk = t(k);
        tk = tk(:);
        xk=x(k);
        yk=y(k);
        thk=z(k);
        Fk=F(k);
        m = int64(5);
        nest = m+4;
        wrk = zeros(4*m+16*nest+41,1);
        iwrk = int64(zeros(nest,1));
        lamda = zeros(nest,1);
        w = ones(m,1);
        [fail,nX, lamdaX, cX] = Oe02be(m , tk, xk, w, smooth);
        if (length(fail)>0)
          printf ("%d, j=%d %s",__LINE__,j,fail);
        end
        [fail,nY, lamdaY, cY] = Oe02be(m , tk, yk, w, smooth);
        if (length(fail)>0)
          printf ("%d, j=%d %s",__LINE__,j,fail);
        end
        [fail,nTh, lamdaTh, cTh] = Oe02be(m , tk, thk, w, smooth);
        if (length(fail)>0)
          printf ("%d, j=%d %s",__LINE__,j,fail);
        end
        [fail,nF, lamdaF, cF] = Oe02be(m , tk, Fk, w, smooth);
        if (length(fail)>0)
          printf ("%d, j=%d %s",__LINE__,j,fail);
        end
        tk = linspace(0,1-1.0e-8,ninter);
        sk = sbase(j);
        for k = 1:ninter
            sps(k+pos) = sk;
            spt(k+pos) = tk(k);
            [fail,spX(pos+k)]=Oe02bb(nX,lamdaX,cX,tk(k));
            if (length(fail)>0)
              printf ("%d, j=%d %s",__LINE__,j,fail);
            end
            [fail,spY(pos+k)]=Oe02bb(nY,lamdaY,cY,tk(k));
            if (length(fail)>0)
              printf ("%d, j=%d %s",__LINE__,j,fail);
            end
            [fail,spTh(pos+k)]=Oe02bb(nTh,lamdaTh,cTh,tk(k));
            if (length(fail)>0)
              printf ("%d, j=%d %s",__LINE__,j,fail);
            end
            [fail,spF(pos+k)]=Oe02bb(nF,lamdaF,cF,tk(k));
            if (length(fail)>0)
              printf ("%d, j=%d %s",__LINE__,j,fail);
            end
        end
        % plot splined boundary
        figure(1);
        hold on;
        plot3(spX(pos+(1:ninter)),spY(pos+(1:ninter)),spTh(pos+(1:ninter)),'g-');
        figure(2);
        hold on;
        % plot splined function
        plot3(sps(pos+(1:ninter)),spt(pos+(1:ninter)),spF(pos+(1:ninter)),'k-');
        pos = pos+ninter;
    end
end

% replace original with spline cor-ordinates
[s,I]=sort(sps);
t=spt(I);
x=spX(I);
y=spY(I);
z = spTh(I);
F=spF(I);

m = int64(length(x));
w = ones(m,1);
if(remk>0)
    w(remk) = 1.0;
end
nsest = int64(120);
ntest = int64(120);
u = double(nsest-4);
v = double(ntest-4);
lwrk = (7*u*v+25*max(u,v))*(max(u,v)+1)+2*(u+v+4*double(m))+23*max(u,v)+56;
lwrk = 10*lwrk;
muX = zeros(ntest,1);
lamdaX = zeros(nsest,1);
muY=muX;
lamdaY=lamdaX;
muTh = muX;
lamdaTh=lamdaX;
lamdaF=lamdaX;
muF=muX;
wrk = zeros(lwrk,1);
% construct co-ordinate mapping
ns = int64(0);
nt = int64(0);
smooth = 1.0e-3;
[fail,nsx, lamdaX, ntx, muX, cX, fpx] = Oe02dd(m, s, t, x, w, smooth);
if (length(fail)>0)
  printf ("%d, %s",__LINE__,fail);
end
[fail,nsy, lamdaY, nty, muY, cY, fpy] = Oe02dd(m, s, t, y, w, smooth);
if (length(fail)>0)
  printf ("%d, %s",__LINE__,fail);
end
[fail,nsth, lamdaTh, ntth, muTh, cTh, fpth] = Oe02dd(m, s, t, z, w, smooth);
if (length(fail)>0)
  printf ("%d, %s",__LINE__,fail);
end
% interpolate F
smooth = 0.01;
[fail,nsf, lamdaF, ntf, muF, cF, fpf] = Oe02dd(m, s, t, F, w, smooth);
if (length(fail)>0)
  printf ("%d, %s",__LINE__,fail);
end

 % generate some sample points;
% initialize random number generator;
state = Og05kg(int64(2), int64(1));

% generate a vector of random numbers for s and t.
%nr = int64(500);
nr=int64(plotrand);
sr = Og05sa(nr, state);
state = Og05kg(int64(2), int64(1));
tr = Og05sa(nr, state);

state = Og05kg(int64(2), int64(1));
xr = Og05sa(nr, state);
state = Og05kg(int64(2), int64(1));
yr = Og05sa(nr, state);
state = Og05kg(int64(2), int64(1));
thr = Og05sa(nr, state);
state = Og05kg(int64(2), int64(1));
Fr = Og05sa(nr, state);

% evaluate co-ordinates and function values at sample points;
[fail,xr] = Oe02de(nr,sr, tr, lamdaX, muX, cX,nsx,ntx);
if (length(fail)>0)
  printf ("%d, %s",__LINE__,fail);
end
[fail,yr] = Oe02de(nr,sr, tr, lamdaY, muY, cY,nsy,nty);
if (length(fail)>0)
  printf ("%d, %s",__LINE__,fail);
end
[fail,thr] = Oe02de(nr,sr, tr, lamdaTh, muTh, cTh,nsth,ntth);
if (length(fail)>0)
  printf ("%d, %s",__LINE__,fail);
end

%plot interpolated surface
figure(1);
hold on;
xlim ([4000,19000]);
zlim ([4500,9500]);
%plot3(xr,yr,thr,'*k','markersize',1);

[fail,Fr] = Oe02de(nr,sr, tr, lamdaF, muF, cF,nsf,ntf);
if (length(fail)>0)
  printf ("%d, %s",__LINE__,fail);
end
% plot interpolated function
figure(2);
hold on;
plot3(sr,tr,Fr,'*','markersize',1);

end