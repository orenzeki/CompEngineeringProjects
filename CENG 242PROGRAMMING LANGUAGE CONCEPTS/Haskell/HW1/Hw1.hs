module Hw1 where

type Mapping = [(String, String, String)]
data AST = EmptyAST | ASTNode String AST AST deriving (Show, Read)

writeExpression :: (AST, Mapping) -> String
evaluateAST :: (AST, Mapping) -> (AST, String)
-- DO NOT MODIFY OR DELETE THE LINES ABOVE -- 
-- IMPLEMENT writeExpression and evaluateAST FUNCTION ACCORDING TO GIVEN SIGNATURES -- 

--------------------------------------- writeExpression --------------------------------------------------------------------------------------------------------------
writeExpression (x,y) = if (y == []) then mytreef
                                     else "let " ++ (mapf) ++ " in " ++ (mytreef)
                                     where mapf = placement (sFirst y) (sSecond y) (sThird y)
                                           mytreef = (elems (mytree x))      
--------------------------------------- elements of the tuple --------------------------------------------------------------------------------------------------------
exFirst  (a,_,_) = [a]
exSecond (_,a,_) = [a]
exThird  (_,_,a) = [a]
sFirst [] = []
sFirst (x:xs) = (exFirst x) ++ (sFirst xs)
sSecond [] = []
sSecond (x:xs) = (exSecond x) ++ (sSecond xs)
sThird [] = []
sThird (x:xs) = (exThird x) ++ (sThird xs)

---------------------------------------- adding paranthesis to the treatment ----------------------------------------------------------------------------------------
mytree EmptyAST = []
mytree (ASTNode p l EmptyAST) = if ((p == "negate") || (p == "len")) then ["("] ++ [p] ++ (mytree l) ++ [")"]
                                                                     else [p] ++ (mytree l)
mytree (ASTNode p EmptyAST r) =if ((p == "negate") || (p == "len")) then ["("] ++ [p] ++ (mytree r) ++ [")"]
                                                                     else [p] ++ (mytree r)
mytree (ASTNode p l r) = ["("] ++ (mytree l) ++ [p] ++ (mytree r) ++ [")"]

---------------------------------------- creating the treatment -----------------------------------------------------------------------------------------------------
elems [] = []
elems (x:xs) 
       | (x == "num")    = "" ++ (elems xs)
       | (x == "str")    = "\"" ++ (head xs) ++ "\"" ++ (elems (tail xs))
       | (x == "plus")   = "+" ++ (elems xs)
       | (x == "times")  = "*" ++ (elems xs)
       | (x == "cat")    = "++" ++ (elems xs)
       | (x == "negate") = "-" ++ (elems xs)
       | (x == "len")    = "length " ++ (elems xs) 
       | otherwise       = x ++ (elems xs)

---------------------------------------- place the elements to the right places -------------------------------------------------------------------------------------
placement [] [] [] = []
placement (x:[]) (y:[]) (z:[]) = if (y == "num") then (x ++ "=" ++ z)
                                                 else (x ++ "=\"" ++ z) ++ "\"" 
placement (x:xs) (y:ys) (z:zs) = if (y == "num") then (x ++ "=" ++ z) ++ ";" ++ (placement xs ys zs)
                                                 else (x ++ "=\"" ++ z) ++ "\";" ++ (placement xs ys zs)


----------------------------------------- evaluateAST ---------------------------------------------------------------------------------------------------------------
evaluateAST (x,y) = ((mytree2 x y), (mytree' (mytree2 x y)))

----------------------------------------- generate the tree by using the tuple --------------------------------------------------------------------------------------
mytree2 EmptyAST [] = EmptyAST
mytree2 EmptyAST ((x,y,z):xs) = EmptyAST
mytree2 (ASTNode p EmptyAST EmptyAST) [] = (ASTNode p EmptyAST EmptyAST)
mytree2 (ASTNode p l EmptyAST) [] = (ASTNode p (mytree2 l []) EmptyAST)
mytree2 (ASTNode p EmptyAST r) [] = (ASTNode p EmptyAST (mytree2 r []))
mytree2 (ASTNode p l r) [] = (ASTNode p (mytree2 l []) (mytree2 r []))
mytree2 (ASTNode p EmptyAST EmptyAST) ((x,y,z):[])
                | (p == x) && (y == "num") = (ASTNode "num" (ASTNode (z) EmptyAST EmptyAST) EmptyAST)
                | (p == x) && (y == "str") = (ASTNode "str" (ASTNode (z) EmptyAST EmptyAST) EmptyAST)
                | otherwise                                     = (ASTNode p EmptyAST EmptyAST)
mytree2 (ASTNode p l EmptyAST) ((x,y,z):[])
                | (p == x) && (y == "num")  = (ASTNode "num" (ASTNode (z) (mytree2 l [(x,y,z)]) EmptyAST) EmptyAST)
                | (p == x) && (y == "str")  = (ASTNode "str" (ASTNode (z) (mytree2 l [(x,y,z)]) EmptyAST) EmptyAST)
                | otherwise                                      = (ASTNode p (mytree2 l [(x,y,z)]) EmptyAST)
mytree2 (ASTNode p EmptyAST r) ((x,y,z):[])
                | (p == x) && (y == "num") = (ASTNode "num" (ASTNode (z) EmptyAST (mytree2 r [(x,y,z)])) EmptyAST)
                | (p == x) && (y == "str") = (ASTNode "str" (ASTNode (z) EmptyAST (mytree2 r [(x,y,z)])) EmptyAST)
                | otherwise                                     = (ASTNode p (mytree2 r [(x,y,z)]) EmptyAST)
mytree2 (ASTNode p l r) ((x,y,z):[])
                | (p == x) && (y == "num") = (ASTNode "num" (ASTNode (z) (mytree2 l [(x,y,z)]) (mytree2 r [(x,y,z)])) EmptyAST)
                | (p == x) && (y == "str") = (ASTNode "str" (ASTNode (z) (mytree2 l [(x,y,z)]) (mytree2 r [(x,y,z)])) EmptyAST)
                | otherwise                                     = (ASTNode p (mytree2 l [(x,y,z)]) (mytree2 r [(x,y,z)]))
mytree2 (ASTNode p l EmptyAST) ((x,y,z):xs)
                | (p == x) && (y == "num") = (ASTNode "num" (ASTNode (z) (mytree2 l ([(x,y,z)] ++ xs)) EmptyAST) EmptyAST)
                | (p == x) && (y == "str") = (ASTNode "str" (ASTNode (z) (mytree2 l ([(x,y,z)] ++ xs)) EmptyAST) EmptyAST)
                | otherwise                                     = mytree2 (ASTNode p (mytree2 l ([(x,y,z)] ++ xs)) EmptyAST) xs
mytree2 (ASTNode p EmptyAST r) ((x,y,z):xs)
                | (p == x) && (y == "num") = (ASTNode "num" (ASTNode (z) EmptyAST (mytree2 r ([(x,y,z)] ++ xs))) EmptyAST)
                | (p == x) && (y == "str") = (ASTNode "str" (ASTNode (z) EmptyAST (mytree2 r ([(x,y,z)] ++ xs))) EmptyAST)
                | otherwise                                     = mytree2 (ASTNode p EmptyAST (mytree2 r ([(x,y,z)] ++ xs))) xs
mytree2 (ASTNode p l r) ((x,y,z):xs)
                | (p == x) && (y == "num") = (ASTNode "num" (ASTNode (z) (mytree2 l ([(x,y,z)] ++ xs)) (mytree2 r ([(x,y,z)] ++ xs))) EmptyAST)
                | (p == x) && (y == "str") = (ASTNode "str" (ASTNode (z) (mytree2 l ([(x,y,z)] ++ xs)) (mytree2 r ([(x,y,z)] ++ xs))) EmptyAST)
                | otherwise                                     = mytree2 (ASTNode p (mytree2 l ([(x,y,z)] ++ xs)) (mytree2 r ([(x,y,z)] ++ xs))) xs

---------------------------------------------- calculate the tree -----------------------------------------------------------------------------------------------------
mytree' EmptyAST = []
mytree' (ASTNode p EmptyAST EmptyAST) = p
mytree' (ASTNode p l EmptyAST)
                | (p == "len")    = show (length (mytree' l))
                | (p == "negate") = show (-(read (mytree' l)::Int))
                | (p == "num")    = (mytree' l)
                | (p == "str")    = (mytree' l)
                | otherwise       = "error"
mytree' (ASTNode p EmptyAST r)
                | (p == "len")    = show (length (mytree' r)) 
                | (p == "negate") = show (-(read (mytree' r)::Int))
                | (p == "num")    = (mytree' r)
                | (p == "str")    = (mytree' r)
                | otherwise       = "error"
mytree' (ASTNode p l r)
                | (p == "plus")   = show ((read (mytree' l)::Int) + (read (mytree' r)::Int))
                | (p == "times")  = show ((read (mytree' l)::Int) * (read (mytree' r)::Int))
                | (p == "cat")    = ((mytree' l) ++ (mytree' r))
                | otherwise       = "error"

 


