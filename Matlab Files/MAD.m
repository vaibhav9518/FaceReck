function [result] = MAD(W,classes,Label)
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here
Y=zeros(size(W,1),classes+1);
for i=1:size(W,1)
    for j=1:classes+1
        if(Label(i)==j)
            Y(i,Label(i))=1;
        else
            Y(i,j)=0;
        end
    end
end
R=zeros(1,classes+1);
R(1,classes+1)=1;
Y_cap=Y;
Beta=2;
Meu1=4;
Meu2=4;
Meu3=2;
[V,E]=size(W);
Pr=zeros(V,V);
for i=1:V
    sum=0;
    for j=1:V
        if(W(i,j)~=0)
         sum=sum+W(i,j);
        end
    end
    for j=1:V
        Pr(j,i)=W(i,j)/sum;
    end
end
H=zeros(V,1);
Pr;
for i=1:V
    sum=0;
    for j=1:V
        if(W(i,j)~=0)
          if(Pr(i,j)>0)
            sum=sum+-1*Pr(i,j)*log(Pr(i,j));
          end
        end
    end
    H(i,1)=sum;
end
C=zeros(V,1);
for i=1:V
    C(i,1)=log(Beta)/log(Beta+exp(H(i,1)));
end
H;
C;
D=zeros(V,1);
Z=zeros(V,1);
for i=1:V
    if(Label(i,1)>0)
      D(i,1)=(1-C(i,1))*(H(i,1)^0.5);
    else
      D(i,1)=0;  
    end
    Z(i,1)=max(D(i,1)+C(i,1),1);
end
D
C
P_inj=zeros(V,1);
P_cont=zeros(V,1);
P_abnd=zeros(V,1);
for i=1:V
    P_inj(i,1)=D(i,1)/Z(i,1);
    P_cont(i,1)=C(i,1)/Z(i,1);
    P_abnd(i,1)=1-P_inj(i,1)-P_cont(i,1);
end
P_cont
P_inj
P_abnd
M=zeros(V,1);
W;
for i=1:V
    sum=0;
    for j=1:V
        if(i~=j)
           sum=sum+P_cont(i,1)*W(i,j) + P_cont(j,1)*W(j,i);
        end
    end
    M(i,1)=Meu1*P_inj(i,1)+Meu2*sum+Meu3;
end
M
for k=1:500
   D=zeros(V,classes+1); 
   Y_old=Y_cap;
   Y_cap;
   for i=1:V
    for j=1:V
        if(W(i,j)>0)
           D(i,:)=D(i,:)+(P_cont(j,1)*W(j,i)+P_cont(i,1)*W(i,j))*Y_cap(j,:);
        end
    end
   end
   D;
   for i=1:V
       Y_cap(i,:)=(Meu1*P_inj(i,1)*Y(i,:)+Meu2*D(i,:)+Meu3*P_abnd(i,1)*R)/M(i,1);
   end
   Y_cap
   if(k==28)
       norm(Y_cap)
       norm(Y_old)
   end
   if((abs(norm(Y_cap)^2-norm(Y_old)^2))<=(.001))
       break
   end
end
Y_cap
result=[];
for i=1:V
    maxi=-1;
    index=-1;
    for j=1:classes
        if(Y_cap(i,j)>maxi)
            maxi=Y_cap(i,j);
            index=j; 
        end
    end
    result=[result index];
end
result;
end

