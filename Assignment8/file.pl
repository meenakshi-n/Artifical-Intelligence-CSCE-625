rl :- consult('file.pl').


%*******************************************************

%base case
intersect([],B,[]).

%intersect([X|Y],B,[X|C]) :- member(X,B), intersect(Y,B,C).
intersect([X|Y],B,C) :- member(X,B), member(X,Y),intersect(Y,B,C).
intersect([X|Y],B,[X|C]) :- member(X,B), intersect(Y,B,C).
intersect([X|Y],B,C) :- intersect(Y,B,C).

%*************************************************************


% 5 queen


num(1).
num(2).
num(3).
num(4).
num(5).


queens(A,B,C,D,E) :- num(A), num(B),num(C),num(D),num(E),

		     A\=B, A\=C, A\=D, A\=E,  B\=C, B\=D, B\=E, C\=D, C\=E, D\=E, 

		     Z1 is B-A, Z1\=1, Z1\=(-1), Z2 is C-A, Z2\=2, Z2\=(-2), Z3 is D-A, Z3\=3,Z3\=(-3), Z4 is E-A, Z4\=4, Z4\=(-4),
		     Z5 is C-B,Z5\=1,Z5\=(-1), Z6 is D-B, Z6\=2,Z6\=(-2), Z7 is E-B, Z7\=3,Z7\=(-3), Z8 is D-C, Z8\=1,Z8\=(-1),
		     Z9 is E-C, Z9\=2,Z9\=(-2), Z10 is E-D, Z10\=1, Z10\=(-1).


%*********************************************************************************

%square root

%Initial(A,B) :- B is A/2+(1/2).

%sqrt(Input,Output) :- Initial(Input,Est),sqrtHelper(Input,Est,0.01,Output).

sqrt(Input,Output) :- Input>=0,Est is Input/2+0.5,sqrtHelper(Input,Est,0.0000000001,Output).

% if input is a negative number

sqrt(Input,Output) :- Input<0, Output is 0.

sqaure(A,B) :- B is A*A. 

sqrtHelper(In,Estimate,Tolerance, Out) :- sqaure(Estimate, EstimateSquared), Diff is EstimateSquared-In, Diff>Tolerance,
					  NewEstimate is (Estimate+In/Estimate)/2, sqrtHelper(In, NewEstimate, Tolerance, Out).

sqrtHelper(In,Estimate,Tolerance, Out) :- sqaure(Estimate, EstimateSquared), Diff is EstimateSquared-In, Diff<Tolerance, Out is Estimate.

%*********************************************************************************
% good meal

calories(water,0).
calories(hamburger,354).
calories(carrot,25).
calories(salad,100).
calories(banana,105).
calories(apple,95).
calories(peanuts,828).
calories(chicken_soup,87).
calories(lasagna,166). % traditional meat lasagna
calories(apple_pie,67).
calories(beans,41).
calories(peas,118).
calories(milk,8).
calories(orange_juice,39).
calories(coke,140).
calories(cookie,132).
calories(naan,78).
calories(potato_soup,149).

meat(hamburger).
vegetable(carrot).
fruit(apple).
fruit(banana).
drink(water).
drink(coke).
vegetable(salad).
fruit(apple).
protein(peanuts).
meat(chicken_soup).
meat(lasagna).
fruit(apple_pie).
protein(beans).
drink(milk).
protein(milk).
fruit(orange_juice).
drink(orange_juice).
drink(coke).
vegetable(potao_soup).


unique([]). 

unique([A|B]) :- \+ member(A,B), unique(B).


good_meal(M) :- nutrition(M), totalCalories(M,Value), Value>400 , Value<600,unique(M).

totalCalories([],Value) :- Value is 0.

totalCalories([A|B],Value) :-  totalCalories(B, Val),calories(A,Value_A), Value is Val+Value_A.

nutrition(M) :- contains_fruit_or_veg(M), contains_protein(M).

contains_fruit_or_veg([X|Y]) :- fruit(X).

contains_fruit_or_veg([X|Y]) :- vegetable(X).

contains_fruit_or_veg([X|Y]) :- contains_fruit_or_veg(Y).

contains_protein([I|J]) :-protein(I).

contains_protein([I|J]) :-meat(I).

contains_protein([I|J]) :-contains_protein(J).

vegetarian_meal(M,Calories) :- good_meal(M), totalCalories(M,Calories), no_meat(M). 

no_meat([]).

no_meat([A|B]) :- \+ meat(A), no_meat(B).


non_vegetarian_meal(M, Calories) :- has_meat(M), good_meal(M), totalCalories(M,Calories).


has_meat([A|B]) :- has_meat(B).

has_meat([A|B]) :- meat(A).














