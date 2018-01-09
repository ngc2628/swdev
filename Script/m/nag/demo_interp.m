function demo_interp(x,y,z,genspl,sbase,tbase)
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
   1.7,1.66,1.39,1.08, 1.9,1.85,1.5,0.91];
z=[5000.,5000.,5000.,5000.,6000.,6000.,6000.,6000.,7000.,7000.,7000.,7000.,...
   8000.,8000.,8000.,8000.,9500.,9500.,9500.,9500];

% map x,y,theta to x(st),y(s,t),theta(s,t);
if(nargin<4)
    genspl = 0;
end
if(nargin<5)
    sbase = [0;1/3;2/3;1];
    tbase = [0;0.25;0.5;0.75;1];
end
% plot bounding lines and define s and t
clf;
subplot(1,2,1);
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
subplot(1,2,2);
hold on;
F = (x.^2+2*y.*x) + z;
plot3(s,t,F,'ro');

if(genspl)
% generate spline along each box boundary.
% splines going horizontally are functions of s only, vertically are
% functions of t only
start = 'c';
spx = [];
spy = [];
spt = [];
spf = [];
sps = [];
spt = [];

smooth = 1.0e-5;
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
    [nX, lamdaX, cX] =...
        e02be(start, s(k), x(k), w, smooth, n, lamda, wrk, iwrk);
     [nY, lamdaY, cY] =...
        e02be(start, s(k), y(k), w, smooth, n, lamda, wrk, iwrk);
     [nTh, lamdaTh, cTh] =...
        e02be(start, s(k), z(k), w, smooth, n, lamda, wrk, iwrk);
     [nF, lamdaF, cF] =...
        e02be(start, s(k), F(k), w, smooth, n, lamda, wrk, iwrk);
    % interpolate 20 points along the spline;
    sk = linspace(0,1-1.0e-8,20);
    tk = tbase(j);
    for k = 1:20
        sps(k+pos) = sk(k);
        spt(k+pos) = tk;
        [spX(pos+k)]=e02bb(lamdaX,cX,sk(k));
        [spY(pos+k)]=e02bb(lamdaY,cY,sk(k));
        [spTh(pos+k)]=e02bb(lamdaTh,cTh,sk(k));
        [spF(pos+k)]=e02bb(lamdaF,cF,sk(k));
    end
    % plot splined boundary
    subplot(1,2,1);
    hold on;
    plot3(spX(pos+(1:20)),spY(pos+(1:20)),spTh(pos+(1:20)),'r-x');
    subplot(1,2,2);
    hold on;
    % plot splined function
    plot3(sps(pos+(1:20)),spt(pos+(1:20)),spF(pos+(1:20)),'r-x');
    pos = pos+20;
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
        [nX, lamdaX, cX] =...
            e02be(start, tk, xk, w, smooth, n, lamda, wrk, iwrk);
        [nY, lamdaY, cY] =...
            e02be(start, tk, yk, w, smooth, n, lamda, wrk, iwrk);
        [nTh, lamdaTh, cTh] =...
            e02be(start, tk, thk, w, smooth, n, lamda, wrk, iwrk);
        [nF, lamdaF, cF] =...
            e02be(start, tk, Fk, w, smooth, n, lamda, wrk, iwrk);
         tk = linspace(0,1-1.0e-8,20);
         sk = sbase(j);
        for k = 1:20
            sps(k+pos) = sk;
            spt(k+pos) = tk(k);
            [spX(pos+k)]=e02bb(lamdaX,cX,tk(k));
            [spY(pos+k)]=e02bb(lamdaY,cY,tk(k));
            [spTh(pos+k)]=e02bb(lamdaTh,cTh,tk(k));
            [spF(pos+k)]=e02bb(lamdaF,cF,tk(k));
        end
        % plot splined boundary
        subplot(1,2,1);
        hold on;
        plot3(spX(pos+(1:20)),spY(pos+(1:20)),spTh(pos+(1:20)),'k-x');
        subplot(1,2,2);
        hold on;
        % plot splined function
        plot3(sps(pos+(1:20)),spt(pos+(1:20)),spF(pos+(1:20)),'k-x');
        pos = pos+20;
    end
end

% replace original with spline cor-ordinates
[s,I]=sort(sps);
t=spt(I);
x=spX(I);
y=spY(I);
z = spTh(I);
F=spF(I);
end

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
[nsx, lamdaX, ntx, muX, cX, fpx, rank, wrk, ifail] = ...
    e02dd('c', s, t, x, w, smooth, ns, lamdaX, nt, muX, wrk);

[nsy, lamdaY, nty, muY, cY, fpy, rank, wrk, ifail] = ...
    e02dd('c', s, t, y, w, smooth, ns, lamdaY, nt, muY, wrk);

[nsth, lamdaTh, ntth, muTh, cTh, fpth, rank, wrk, ifail] = ...
    e02dd('c', s, t, z, w, smooth, ns, lamdaTh, nt, muTh, wrk);

% interpolate F
smooth = 0.01;
[nsf, lamdaF, ntf, muF, cF, fpf, rank, wrk, ifail] = ...
    e02dd('c', s, t, F, w, smooth, ns, lamdaF, nt, muF, wrk);
    
 % generate some sample points;
% initialize random number generator;
[state, ifail] = g05kg(int64(2), int64(1));

% generate a vector of random numbers for s and t.
nr = int64(500);
[state, sr, ifail] = g05sa(nr, state);
[state, tr, ifail] = g05sa(nr, state);

% evaluate co-ordinates and function values at sample points;
[xr, ifail] = e02de(sr, tr, lamdaX, muX, cX,'px',nsx,'py',ntx);
[yr, ifail] = e02de(sr, tr, lamdaY, muY, cY,'px',nsy,'py',nty);
[thr, ifail] = e02de(sr, tr, lamdaTh, muTh, cTh,'px',nsth,'py',ntth);
[Fr, ifail] = e02de(sr, tr, lamdaF, muF, cF,'px',nsf,'py',ntf);

%plot interpolated surface
subplot(1,2,1);
plot3(xr,yr,thr,'x','markersize',1);
% plot interpolated function
subplot(1,2,2);
hold on;
plot3(sr,tr,Fr,'x','markersize',1);

% determine s,t co-ordinates of a point in x0

% store splines in user
user.lamdaX = lamdaX(1:nsx);
user.lamdaY = lamdaY(1:nsy);
user.lamdaTh = lamdaTh(1:nsth);
user.muX = muX(1:ntx);
user.muY = muY(1:nty);
user.muTh = muTh(1:ntth);
user.cX = cX;
user.cY = cY;
user.cTh = cTh;
user.x0 = [x(abs(remk));y(abs(remk));z(abs(remk))];
x0=user.x0;
m =int64(3);
n = int64(2);

% using unconstrained sum of squares minimization, providing derivatives
% rough guess of the position of [s0,t0] 
[s0t0] = [0.5 0.5];
lsfun2 = @(m,n,st,ldfjac,user) lsfun2_using_spline(m,n,st,ldfjac,user);
[s0t0_e04gz, fsumsq, user, ifail] = e04gz(m, lsfun2, s0t0, 'n', n, 'user', user);
s0t0 = s0t0_e04gz;
subplot(1,2,1);
hold on;
plot3(x0(1),x0(2),x0(3),'co');
% plot where the minimization believes the point is
[xr0, ifail] = e02de(s0t0(1), s0t0(2), lamdaX, muX, cX,'px',nsx,'py',ntx);
[yr0, ifail] = e02de(s0t0(1), s0t0(2), lamdaY, muY, cY,'px',nsy,'py',nty);
[thr0, ifail] = e02de(s0t0(1), s0t0(2), lamdaTh, muTh, cTh,'px',nsth,'py',ntth);
[Fr0, ifail] = e02de(s0t0(1), s0t0(2), lamdaF, muF, cF,'px',nsf,'py',ntf);

plot3(xr0,yr0,thr0,'k*');
subplot(1,2,2);
plot3(s0t0(1),s0t0(2),Fr0,'k*');

% using constrained minimisation using function values only
funct1=@(n,st,user) funct1_using_spline(n,st,user);
[bl, bu, s0t0_e04jy, f, iw, w, user, ifail] = ...
    e04jy(int64(3), funct1,[0;0], [1;1], [0.5;0.5],'user', user);
s0t0 = s0t0_e04jy;
[xr0, ifail] = e02de(s0t0(1), s0t0(2), lamdaX, muX, cX,'px',nsx,'py',ntx);
[yr0, ifail] = e02de(s0t0(1), s0t0(2), lamdaY, muY, cY,'px',nsy,'py',nty);
[thr0, ifail] = e02de(s0t0(1), s0t0(2), lamdaTh, muTh, cTh,'px',nsth,'py',ntth);
[Fr0, ifail] = e02de(s0t0(1), s0t0(2), lamdaF, muF, cF,'px',nsf,'py',ntf);
subplot(1,2,1);
plot3(xr0,yr0,thr0,'r+');
subplot(1,2,2);
plot3(s0t0(1),s0t0(2),Fr0,'r+');


% using interpolation of s and t as functions of x,y,theta
if(remk>0)
    k = [1:(remk-1) (remk+1):length(x)];
else
    k=1:length(x);
end
  
[iqs, rqs, ifail] = e01tg(x(k), y(k), z(k), s(k), int64(0), int64(0));
[iqt, rqt, ifail] = e01tg(x(k), y(k), z(k), t(k), int64(0),int64(0));
[s0t0_e01t(1)] = e01th(x(k), y(k), z(k), s(k), iqs, rqs, x0(1), x0(2), x0(3));
[s0t0_e01t(2)] = e01th(x(k), y(k), z(k), t(k), iqt, rqt, x0(1), x0(2), x0(3));
s0t0 = s0t0_e01t;
[xr0, ifail] = e02de(s0t0(1), s0t0(2), lamdaX, muX, cX,'px',nsx,'py',ntx);
[yr0, ifail] = e02de(s0t0(1), s0t0(2), lamdaY, muY, cY,'px',nsy,'py',nty);
[thr0, ifail] = e02de(s0t0(1), s0t0(2), lamdaTh, muTh, cTh,'px',nsth,'py',ntth);
[Fr0, ifail] = e02de(s0t0(1), s0t0(2), lamdaF, muF, cF,'px',nsf,'py',ntf);
subplot(1,2,1);
plot3(xr0,yr0,thr0,'m+');
subplot(1,2,2);
plot3(s0t0(1),s0t0(2),Fr0,'m+');
[s0t0_e04gz(:) s0t0_e04jy(:) s0t0_e01t(:) [s(abs(remk));t(abs(remk))]]

end

function [fvec,fjac,user] = lsfun2_using_spline(m,n,st,ldfjac,user)

s=st(1);
t=st(2);
% calculate derivatives of spline with respect to each dimension
nus = int64(1);
nut=int64(1);
[dxds, ifail] = e02dh(s, t, user.lamdaX, user.muX, user.cX, nus, int64(0));
[dyds, ifail] = e02dh(s, t, user.lamdaY, user.muY, user.cY, nus, int64(0));
[dthds, ifail] = e02dh(s, t, user.lamdaTh, user.muTh, user.cTh, nus, int64(0));
[dxdt, ifail] = e02dh(s, t, user.lamdaX, user.muX, user.cX, int64(0), nut);
[dydt, ifail] = e02dh(s, t, user.lamdaY, user.muY, user.cY, int64(0), nut);
[dthdt, ifail] = e02dh(s, t, user.lamdaTh, user.muTh, user.cTh, int64(0), nut);

fjac(1,1:2) = [dxds dxdt];
fjac(2,1:2) = [dyds dydt];
fjac(3,1:2) = [dthds dthdt];
% calculate function values
[fvec(1), ifail] = e02de(s, t, user.lamdaX, user.muX, user.cX);
[fvec(2), ifail] = e02de(s, t, user.lamdaY, user.muY, user.cY);
[fvec(3), ifail] = e02de(s, t, user.lamdaTh, user.muTh, user.cTh);
fvec = fvec(:)-user.x0(:);
end

function [fc,user]=funct1_using_spline(n,st,user)
s=st(1);
t=st(2);
[fvec(1), ifail] = e02de(s, t, user.lamdaX, user.muX, user.cX);
[fvec(2), ifail] = e02de(s, t, user.lamdaY, user.muY, user.cY);
[fvec(3), ifail] = e02de(s, t, user.lamdaTh, user.muTh, user.cTh);
fvec = fvec(:)-user.x0(:);
fc=sqrt(sum( fvec(:).^2));
end