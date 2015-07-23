function [X] = cosine_sim(A,B)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
X=norm(A-B);%dot(A,B)/(norm(A)*norm(B));
end

