
function tst_perm01()

mm=[39,74,14,85; 12,9,28,1; 27,20,53,7];

mm(1,:)

%size(mm,1)
%size(mm,2)
%[1:size(mm,1)]
%repmat([1,2,3],1,4)
repmat([1:size(mm,1)],1,size(mm,2))


mmm=find(all(diff(sort(nchoosek(repmat([1:size(mm,1)],1,size(mm,2)),size(mm,1)),2),1,2),2));
xx=nchoosek(mm(:)',size(mm,1));
xxx=xx(mmm,:);

[mmm]'
rows(mmm)
columns(mmm)

rows(xx)
columns(xx)

rows(xxx)
columns(xxx)

endfunction