function clchk()

format long

ii=1;
jj=1;

cf=cfread("cfdata.txt");
cl=cf(1).cl;

for ii=1:length(cl)
  printf("newcl [%s]\n",cl(ii).descr);
  for jj=1:length(cl(ii).pp)
    printf("p%d [%f,%f,%f,%f]\n",jj,cl(ii).pp(jj,1),cl(ii).pp(jj,2),cl(ii).pp(jj,3),cl(ii).pp(jj,4));
  endfor
endfor

printf("\n\n");

[clx,sbase,tbase]=clgrid(cl,"z");

printf("sbase :\n");
for ii=1:length(sbase)
  printf("%f ",sbase(ii));
  if (mod(ii,10)==0)
    printf("\n");
  endif
endfor
printf("\n\n");

printf("tbase :\n");
for ii=1:length(tbase)
  printf("%f ",tbase(ii));
  if (mod(ii,10)==0)
    printf("\n");
  endif
endfor
printf("\n\n");

for ii=1:length(clx)
  printf("newcl [%s]\n",clx(ii).descr);
  for jj=1:length(clx(ii).pp)
    printf("p%d [%f,%f,%f]\n",jj,clx(ii).pp(jj,1),clx(ii).pp(jj,2),clx(ii).pp(jj,3));
  endfor
endfor

endfunction
