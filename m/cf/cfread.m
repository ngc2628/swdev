
function cfL=cfread(fname)

%fname="cldata.txt";

cntcoor=4; % x,y,z,w
pp=[];
clL=struct("pp",pp,"descr",[]);
cfL=struct("cl",clL,"descr",[]);
ncf=0;
ncl=0;
nclL=[];
ii=1;
jj=1;
kk=1;
spp=[];

fp=fopen(fname,"r");
while (!feof(fp))
  line=fgetl(fp);
  strtrim(line);
  if (line==-1 || length(line)==0 || toascii(line(1))==35)
    continue;
  endif
  if (ncl==0)
    line=substr(line,index(line,"=")+1);
    [nclL]=strsplit(line,",");
    ncl=int32(length(nclL));
    ii=1;
    jj=1;
    ncf++;
    cf(ncf).descr=sprintf("cf #%d",ncf);
  else
    [spp]=strsplit(line,",");
    for kk=1:cntcoor
      cfL(ncf).clL(jj).pp(ii,kk)=str2double(spp{kk});
    endfor
    ii++;
    if (ii>int32(str2double(nclL{jj})))
      if (jj==ncl)
        cfL(ncf).clL(jj).descr=sprintf("cl #%d",jj);
        ncl=0;
        continue;
      endif
      cfL(ncf).clL(jj).descr=sprintf("cl #%d",jj);
      jj++;
      ii=1;
    endif
  endif
endwhile

% check input parser
#{
for ii=1:ncf
  printf("newcf [%s]\n",cfL(ii).descr);
  for jj=1:length(cfL(ii).clL)
    printf("newcl [%s]\n",cfL(ii).clL(jj).descr);
    for kk=1:length(cfL(ii).clL(jj).pp)
      cfL(ii).clL(jj).pp(kk,:)
    endfor
  endfor
endfor
#}

endfunction


