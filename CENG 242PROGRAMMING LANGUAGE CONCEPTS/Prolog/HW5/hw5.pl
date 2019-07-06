:-module(hw5, [catomic_number/2, ion/2, molecule/2]).
:-[catoms].

sumList([],0).
sumList([H|T],R):-sumList(T,NR),R is H+NR.
catomic_number(X,Y):-catom(X,_,_,L),sumList(L,Y).

lastElement([N],N).
lastElement([_|T],R):-lastElement(T,R).
ion(X,Y):-catom(X,_,_,L),lastElement(L,T),(T=<4-> Y is T ; Y is T-8).

sumCatoms([],0).
sumCatoms([H],S):-catomic_number(H,S1),S1 == S.
sumCatoms([H,H1|T],S):-catomic_number(H,S1),catomic_number(H1,S2),S1=<S2,B is S1+S2,B=<S,A is S-S1,sumCatoms([H1|T],A).

sumIons([],0).
sumIons([H|T],S):-ion(H,S1),sumIons(T,S2),S is S1+S2.

molecule(CATOM_LIST,TOTAL_CATOMIC_NUMBER):-sumCatoms(CATOM_LIST,TOTAL_CATOMIC_NUMBER),sumIons(CATOM_LIST,0).
