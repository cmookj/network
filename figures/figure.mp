prologues := 3;
% outputtemplate := "%j-%c.mps";
% outputformat := "eps";

input boxes 

def edge(suffix a,b) expr p = 
    drawarrow p cutbefore bpath.a cutafter bpath.b;
enddef;

def dedge(suffix a,b) expr p = 
    drawarrow p cutbefore bpath.a cutafter bpath.b dashed evenly;
enddef;

vardef loop@# expr p =
    edge(@#,@#) @#.c{curl0}..@#.c+p..{curl0}@#.c enddef; 

vardef dloop@# expr p =
    dedge(@#,@#) @#.c{curl0}..@#.c+p..{curl0}@#.c enddef; 

% Figure 1: Directed Graph
beginfig(1)

circleit.O(btex O etex);
circleit.N(btex N etex);
circleit.J(btex J etex);
circleit.B(btex B etex);

numeric hsep;
N.c - O.c = J.c - N.c = B.c - J.c = (hsep,0);
xpart(B.c - O.c) = 6cm;

circleit.K(btex K etex);
circleit.F(btex F etex);
circleit.L(btex L etex);
circleit.E(btex E etex);
E.c - L.c = L.c - F.c = F.c - K.c = (hsep,0);
O.c - K.c = (0,.8hsep);

circleit.C(btex C etex);
circleit.I(btex I etex);
F.c - C.c = (0,.8hsep);
E.c - I.c = (0,.8hsep);

circleit.A(btex A etex);
circleit.M(btex M etex); 
circleit.D(btex D etex);
M.c - A.c = (.5hsep,-hsep*sqrt(3)/2);
M.c - D.c = (hsep,0);
O.c - M.c = (.4hsep,-.8hsep);

circleit.G(btex G etex);
circleit.H(btex H etex);
K.c - G.c = (hsep,0);
K.c - H.c = (.5hsep,hsep*sqrt(3)/2);

drawboxed(A,M,D);
drawboxed(O,N,J,B);
drawboxed(K,F,L,E);
drawboxed(C,I);
drawboxed(G,H);

% Labels
% label.lft(btex A etex, A.c);
% label.urt(btex M etex, M.c);
% label.ulft(btex D etex, D.c);
%
% label.lft(btex O etex, O.c);
% label.urt(btex N etex, N.c);
% label.top(btex J etex, J.c);
% label.urt(btex B etex, B.c);
%
% label.lft(btex G etex, G.c);
% label.ulft(btex K etex, K.c);
% label.lft(btex H etex, H.c);
%
% label.urt(btex F etex, F.c);
% label.lrt(btex C etex, C.c);
%
% label.bot(btex L etex, L.c);
% label.urt(btex E etex, E.c);
% label.urt(btex I etex, I.c);

% Edges
edge(A,M) A.c{dir-40}..M.c;
edge(M,A) M.c{dir145}..A.c;

edge(D,M) D.c{dir30}..M.c;
edge(M,D) M.c{dir-155}..D.c;

edge(M,O) M.c..O.c;
edge(O,N) O.c..N.c;
edge(N,J) N.c..J.c;
edge(J,B) J.c..B.c;
edge(O,B) O.c{dir60}..B.c;

edge(O,F) O.c..F.c;
edge(F,L) F.c..L.c;
edge(L,E) L.c..E.c;

edge(E,I) E.c..I.c;

edge(G,K) G.c..K.c;
edge(K,H) K.c..H.c;
edge(H,G) H.c..G.c;
edge(K,N) K.c..N.c;

edge(H,C) H.c..C.c;

edge(J,L) J.c{dir-45}..L.c;
edge(L,J) L.c{dir135}..J.c;

edge(F,C) F.c{dir-45}..C.c;
edge(C,F) C.c{dir135}..F.c;

loop.I(0,-20pt);

% Strongly connected components
numeric u;
u = hsep;

draw fullcircle scaled .5u shifted I.c dashed evenly;
draw fullcircle scaled .5u shifted O.c dashed evenly;
draw fullcircle scaled .5u shifted N.c dashed evenly;
draw fullcircle scaled .5u shifted B.c dashed evenly;
draw fullcircle scaled .5u shifted E.c dashed evenly;

draw fullcircle xscaled .6u yscaled 1.4u shifted .5[J.c,L.c] dashed evenly;
draw fullcircle xscaled .6u yscaled 1.4u shifted .5[F.c,C.c] dashed evenly;

draw fullcircle xscaled 1.5u yscaled 1.6u shifted .3[.5[A.c,M.c],D.c] dashed evenly;
draw fullcircle xscaled 1.5u yscaled 1.6u shifted .3[.5[G.c,K.c],H.c] dashed evenly;

endfig;

% Figure 2: Depth First Forest
beginfig(2)

%% Tree #1
circleit.O(btex O etex);
circleit.N(btex N etex);
circleit.J(btex J etex);
circleit.L(btex L etex);
circleit.E(btex E etex);
circleit.I(btex I etex);
circleit.B(btex B etex);

numeric vsep;
O.c - N.c = N.c - J.c = J.c - L.c = L.c - E.c = E.c - I.c = (0,vsep);
ypart(O.c - I.c) = 6cm;
L.c - B.c = (vsep,0);

drawboxed(O,N,J,L,E,I,B);

circleit.F(btex F etex);
circleit.C(btex C etex);

F.c - C.c = (0,vsep);
F.c - O.c = (vsep,-vsep);

drawboxed(F,C);

%% Tree #2 
circleit.K(btex K etex);
circleit.H(btex H etex);
circleit.G(btex G etex);

K.c - H.c = H.c - G.c = (0,vsep);
H.c - F.c = (2vsep,0);

drawboxed(K,H,G);

%% Tree #3 
circleit.M(btex M etex);
circleit.A(btex A etex);
circleit.D(btex D etex);

M.c - A.c = (.4vsep,vsep);
M.c - D.c = (-.4vsep,vsep);
A.c - H.c = (2vsep,0);

drawboxed(M,A,D);

%% Arcs 
edge(O,N) O.c..N.c;
edge(N,J) N.c..J.c;
edge(J,L) J.c..L.c;
edge(L,E) L.c..E.c;
edge(E,I) E.c..I.c;
edge(J,B) J.c..B.c;

edge(F,C) F.c..C.c;

edge(K,H) K.c..H.c;
edge(H,G) H.c..G.c;

edge(M,A) M.c..A.c;
edge(M,D) M.c..D.c;

edge(O,F) O.c..F.c;

dedge(O,B) O.c{dir-135}..B.c;
dedge(L,J) L.c{dir45}..J.c;
dloop.I(0,-.5vsep);

dedge(F,L) F.c{dir-135}..{dir180}L.c;
dedge(C,F) C.c{dir45}..F.c;

dedge(G,K) G.c{dir45}..K.c;
dedge(H,C) H.c{dir-135}..{dir180}C.c;

dedge(A,M) A.c{dir100}..M.c;
dedge(D,M) D.c{dir80}..M.c;
dedge(M,O) M.c{dir155}..O.c;
endfig;

end
