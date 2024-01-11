:- use_module(library(clpfd)).

%moga li taka:
%sum(x,y,sm) :- x+y#=sm.

%ili trqbva neshto ot vida na:
sum(0,Y,Y).
sum(X,Y,Sm) :- X1#=X-1, Sm1#=Sm-1, sum(X1,Y,Sm1).

