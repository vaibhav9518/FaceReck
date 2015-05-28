clear all;
clc;
vectors=[];
endsum=0;
for times=1:2
start=times;
for i=start:start+1
 imagefiles = dir(strcat('yaleB0',num2str(i),'/*.pgm'));      
 nfiles = length(imagefiles);    % Number of files found
 nfiles=min(65,nfiles);
 for ii=1:nfiles
   currentfilename = imagefiles(ii).name;
   currentimage = imread(strcat('yaleB0',num2str(i),'/',currentfilename));
   vectors(:,65*(i-start)+ii)=currentimage(:);
   %imwrite(currentimage,strcat('file',num2str(ii),'.jpg'));
 end
end
%'Vectors done'
%[d1,d2] = sort(a(1,:));
%a(:,d2)
MAxii=-2;
for K=2:2
 W=zeros(size(vectors,2));
 for i=1:size(vectors,2)
    w=[];
    for j=1:size(vectors,2)
        if(i~=j)
          w(:,j)=[cosine_sim(vectors(:,i),vectors(:,j)) j];
        else
          w(:,j)=[realmin('double') j];
        end
    end
    [d1,d2] = sort(w(1,:));
    w=w(:,d2);
    for l=1:K%size(vectors,2):-1:size(vectors,2)-K
        W(i,w(2,l))=w(1,l);
        W(w(2,l),i)=w(1,l);
    end
 end
 %'W done'
 size(W)
 Label=[ones(20,1);ones(90,1)*-1;ones(20,1)*2];
 RES_acc=[ones(1,65)*1 ones(1,65)*2];
 result=AD(W,2,Label);
 MAxii=max(sum(result==RES_acc),MAxii)
end
endsum=endsum+MAxii;
end
(endsum/2-40)/90*100
