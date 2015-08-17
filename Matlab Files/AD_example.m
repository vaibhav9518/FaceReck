function [result] = MAD_example(Xl,Xu,Y,nofNn)
if(nofNn<0)
    error('No. of nearest neighbours should be positive')
end
un=(unique(Y));
if(un(1)~=1)
    error('The Labels should start with 1');
end
if(max(size(un))<2)
    error('The number of classes should be greater than or equal to 2');
end
vectors=[Xl Xu];
W=zeros(size(vectors,2));
for i=1:size(vectors,2)
    w=[];
    for j=1:size(vectors,2)
          w(:,j)=[cosine_sim(vectors(:,i),vectors(:,j)) j];
    end
    [d1,d2] = sort(w(1,:));
    w=w(:,d2);
    for l=1:nofNn%size(vectors,2):-1:size(vectors,2)-K
        W(i,w(2,l))=w(1,l);
        W(w(2,l),i)=w(1,l);
    end
end
size(W)
Label=[Y;ones(size(Xu,2),1)*-1]; 
result=AD(W,max(size(un)),Label);
end
