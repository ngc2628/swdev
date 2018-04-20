
function [sbase,tbase] = demo_stgrid(cl)

sbase=[];
tbase=[];

ncl=length(cl);
nctrlmax=0;
for ii=1:ncl
  if (cl(ii).nctrl>nctrlmax)
    nctrlmax=cl(ii).nctrl;
  endif
endfor

ssbase=[];
for ii=1:ncl
  ssbase(ii,1)=0.0;
  for jj=2:nctrlmax-1
    tmp=cl(ii).pp(1).xx;
    ssbase(ii,jj)=(cl(ii).pp(jj).xx-tmp)/(cl(ii).pp(cl(ii).nctrl).xx-tmp);
  endfor
  ssbase(ii,cl(ii).nctrl)=1.0;
endfor

for ii=1:nctrlmax
  sbase(ii)=mean(ssbase(:,ii));
endfor

ttbase=[];
for ii=1:nctrlmax
  for jj=1:ncl
    if (ii<=cl(ncl).nctrl)
      tmp=cl(1).pp(ii).yy;
      ttbase(ii,jj)=(cl(jj).pp(ii).yy-tmp)/(cl(ncl).pp(ii).yy-tmp);
    else
      
    endif
  endfor
endfor

for ii=1:ncl
  tbase(ii)=mean(ttbase(:,ii));
endfor

endfunction
    
