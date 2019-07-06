module Hw2 where

data ASTResult = ASTError String | ASTJust (String, String, Int) deriving (Show, Read)
data ASTDatum = ASTSimpleDatum String | ASTLetDatum String deriving (Show, Read)
data AST = EmptyAST | ASTNode ASTDatum AST AST deriving (Show, Read)

isNumber :: String -> Bool
eagerEvaluation :: AST -> ASTResult
normalEvaluation :: AST -> ASTResult
-- DO NOT MODIFY OR DELETE THE LINES ABOVE -- 
-- IMPLEMENT isNumber, eagerEvaluation and normalEvaluation FUNCTIONS ACCORDING TO GIVEN SIGNATURES --

----------------------------------------------- isNumber -----------------------------------------------------------------------------------------------------------
isNumber ""  = False
isNumber "-" = False
isNumber a 
        | ((head a) == '-') = if (filter (`elem` ['0'..'9']) (tail a)) == (tail a) then True else False 
        | otherwise         = if (filter (`elem` ['0'..'9']) a) == a then True else False  
------------------------------------------------ Error Message -----------------------------------------------------------------------------------------------------
letTree EmptyAST = ""
letTree (ASTNode (ASTLetDatum p) l r) = letTree l
letTree (ASTNode (ASTSimpleDatum p) l EmptyAST) = (mytree l)
letTree (ASTNode (ASTSimpleDatum p) l r) = mytree (ASTNode (ASTSimpleDatum p) l r)      

mytree EmptyAST = ""
mytree (ASTNode (ASTLetDatum p) l r)
        | ((letTree l) == "") = (mytree r)
        | otherwise       = letTree l
mytree (ASTNode (ASTSimpleDatum p) EmptyAST EmptyAST) = p

mytree (ASTNode (ASTSimpleDatum p) l EmptyAST) 
        | (p == "num") && ((isNumber (mytree l)) == False) && ((length (mytree l)) /= 1) = "the value " ++ "'" ++ (mytree l) ++ "'" ++ " is not a number!"
        | (p == "len") && ((mytree l) == "num")                                          = "len operation is not defined on num!"
        | (p == "len") && (((mytree l) /= "num") || ((mytree l) /= "str"))               = (mytree l)
        | (p == "negate") && ((mytree l) == "str")                                       = "negate operation is not defined on str!"
        | (p == "negate") && (((mytree l) /= "num") || ((mytree l) /= "str"))            = (mytree l)
        | (p == "num") && ((isNumber (mytree l)) == True)                                = "num" 
        | (p == "str")                                                                   = "str"      
        | (p == "len") && ((mytree l) == "str")                                          = "num"   
        | (p == "negate") && ((mytree l) == "num")                                       = "num"
     
mytree (ASTNode (ASTSimpleDatum p) l r)
        | ((p == "plus") || (p == "times")) && ((mytree l) == "num") && ((mytree r) == "str") = p ++ " operation is not defined between num and str!" 
        | ((p == "plus") || (p == "times")) && ((mytree l) == "str") && ((mytree r) == "num") = p ++ " operation is not defined between str and num!" 
        | ((p == "plus") || (p == "times")) && ((mytree l) == "str") && ((mytree r) == "str") = p ++ " operation is not defined between str and str!" 
        | ((p == "plus") || (p == "times")) && ((mytree l) /= "str") && ((mytree l) /= "num") = (mytree l)
        | ((p == "plus") || (p == "times")) && (((mytree l) == "str") || ((mytree l) == "num")) && ((mytree r) /= "str") && ((mytree r) /= "num") = (mytree r)
        | ((p == "plus") || (p == "times")) && ((mytree l) == "num") && ((mytree r) == "num") = "num"
        | (p == "cat") && ((mytree l) == "num") && ((mytree r) == "str")                      =  "cat operation is not defined between num and str!" 
        | (p == "cat") && ((mytree l) == "str") && ((mytree r) == "num")                      =  "cat operation is not defined between str and num!"          
        | (p == "cat") && ((mytree l) == "num") && ((mytree r) == "num")                      =  "cat operation is not defined between num and num!" 
        | (p == "cat") && ((mytree l) /= "str") && ((mytree l) /= "num")                      = (mytree l)
        | (p == "cat") && (((mytree l) == "str") || ((mytree l) == "num")) && ((mytree r) /= "str") && ((mytree r) /= "num") = (mytree r)
        | (p == "cat") && ((mytree l) == "str") && ((mytree r) == "str")                      = "str"

errorMessage a = if (((mytree a) == "num") || ((mytree a) == "str")) then "" else (mytree a)
------------------------------------------------ Count of eagerEvaluation ------------------------------------------------------------------------------------------
eagerTree EmptyAST = []
eagerTree (ASTNode (ASTSimpleDatum p) EmptyAST EmptyAST) = [p]
eagerTree (ASTNode (ASTLetDatum p) l r) = (eagerTree l) ++ (eagerTree r)
eagerTree (ASTNode (ASTSimpleDatum p) l EmptyAST) = [p] ++ (eagerTree l)
eagerTree (ASTNode (ASTSimpleDatum p) EmptyAST r) = [p] ++ (eagerTree r)
eagerTree (ASTNode (ASTSimpleDatum p) l r) = [p] ++ (eagerTree l) ++ (eagerTree r)

eagerIf [] = []
eagerIf (x:xs) = if ((x == "plus") ||  (x == "cat") || (x == "times") || (x == "len") || (x == "negate")) then [x] ++ (eagerIf xs) else eagerIf xs 
eagerCount a = length (eagerIf (eagerTree a))

---------------------------------- Calculate the tree -------------------------------------------------------------------
tree' EmptyAST = []
tree' (ASTNode (ASTLetDatum p) l r) =  (tree' r)
tree' (ASTNode (ASTSimpleDatum p) EmptyAST EmptyAST) = p
tree' (ASTNode (ASTSimpleDatum p) l EmptyAST)
                | (p == "len")    = show (length (tree' l))
                | (p == "negate") = show (-(read (tree' l)::Int))
                | (p == "num")    = (tree' l)
                | (p == "str")    = (tree' l)
                | otherwise       = "error"

tree' (ASTNode (ASTSimpleDatum p) l r)
                | (p == "plus")   = show ((read (tree' l)::Int) + (read (tree' r)::Int))
                | (p == "times")  = show ((read (tree' l)::Int) * (read (tree' r)::Int))
                | (p == "cat")    = ((tree' l) ++ (tree' r))
                | otherwise       = "error"

----------------------------------------------- eagerEvaluation ---------------------------------------------------------------------------------------------------
eagerEvaluation x = eagerResult x where
                    eagerResult EmptyAST = ASTError ""
                    eagerResult (ASTNode (ASTLetDatum p) l r) = astjuste
                    eagerResult (ASTNode (ASTSimpleDatum p) EmptyAST EmptyAST)
                                 | ((errorMessage x) == "") = astjuste
                                 | otherwise                = ASTError (errorMessage x)
                    eagerResult (ASTNode (ASTSimpleDatum p) l EmptyAST)
                                 | ((errorMessage x) == "") = astjuste
                                 | otherwise                = ASTError (errorMessage x)
                    eagerResult (ASTNode (ASTSimpleDatum p) l r)
                                 | ((errorMessage x) == "") = astjuste
                                 | otherwise                = ASTError (errorMessage x)
                    
                    eagerTrees EmptyAST [] = EmptyAST
                    eagerTrees EmptyAST ((a,b):xs) = EmptyAST
                    eagerTrees (ASTNode (ASTLetDatum p) l r) [] = (ASTNode (ASTLetDatum p) l (eagerTrees r ([(p,(tree' (eagerTrees l [])))])))
                    eagerTrees (ASTNode (ASTLetDatum p) l r) ((a,b):xs) = (ASTNode (ASTLetDatum p) l (eagerTrees r ([(p,(tree' (eagerTrees l ((a,b):xs))))] ++ ((a,b):xs))))
                    eagerTrees (ASTNode (ASTSimpleDatum p) EmptyAST EmptyAST) [] = (ASTNode (ASTSimpleDatum p) EmptyAST EmptyAST)
                    eagerTrees (ASTNode (ASTSimpleDatum p) EmptyAST EmptyAST) ((a,b):xs)
                                 | (p == a)  = (ASTNode (ASTSimpleDatum b) EmptyAST EmptyAST)
                                 | otherwise = eagerTrees (ASTNode (ASTSimpleDatum p) EmptyAST EmptyAST) xs
                    eagerTrees (ASTNode (ASTSimpleDatum p) l EmptyAST) [] = (ASTNode (ASTSimpleDatum p) (eagerTrees l []) EmptyAST)
                    eagerTrees (ASTNode (ASTSimpleDatum p) l EmptyAST) ((a,b):xs) = (ASTNode (ASTSimpleDatum p) (eagerTrees l ((a,b):xs)) EmptyAST)
                    eagerTrees (ASTNode (ASTSimpleDatum p) l r) [] = (ASTNode (ASTSimpleDatum p) (eagerTrees l []) (eagerTrees r []))
                    eagerTrees (ASTNode (ASTSimpleDatum p) l r) ((a,b):xs) = (ASTNode (ASTSimpleDatum p) (eagerTrees l ((a,b):xs)) (eagerTrees r ((a,b):xs)))

                    e1 = tree' (eagerTrees x [])
                    e2 = if ((isNumber e1) == True) then "num" else "str"
                    e3 = eagerCount x
                    astjuste = ASTJust (e1, e2, e3)
-------------------------------------------------- normalEvaluation ------------------------------------------------------------------------------------------------
normalEvaluation y = normalResult y where
                     normalResult EmptyAST = ASTError ""
                     normalResult (ASTNode (ASTLetDatum p) l r) = astjustn
                     normalResult (ASTNode (ASTSimpleDatum p) EmptyAST EmptyAST)
                                  | ((errorMessage y) == "") = astjustn
                                  | otherwise                       = ASTError (errorMessage y)
                     normalResult (ASTNode (ASTSimpleDatum p) l EmptyAST)
                                  | ((errorMessage y) == "") = astjustn
                                  | otherwise                       = ASTError (errorMessage y)
                     normalResult (ASTNode (ASTSimpleDatum p) l r)
                                  | ((errorMessage y) == "") = astjustn
                                  | otherwise                       = ASTError (errorMessage y)
                     
                     normalTrees EmptyAST [] = EmptyAST
                     normalTrees EmptyAST ((a,b):xs) = EmptyAST
                     normalTrees (ASTNode (ASTLetDatum p) l r) [] = (normalTrees r ([(p,(normalTrees l []))]))
                     normalTrees (ASTNode (ASTLetDatum p) l r) ((a,b):xs) = (normalTrees r ([(p,(normalTrees l ((a,b):xs)))] ++ ((a,b):xs)))
                     normalTrees (ASTNode (ASTSimpleDatum p) EmptyAST EmptyAST) [] = (ASTNode (ASTSimpleDatum p) EmptyAST EmptyAST)
                     normalTrees (ASTNode (ASTSimpleDatum p) EmptyAST EmptyAST) ((a,b):xs)
                                  | (p == a)  = (b)
                                  | otherwise = normalTrees (ASTNode (ASTSimpleDatum p) EmptyAST EmptyAST) xs
                     normalTrees (ASTNode (ASTSimpleDatum p) l EmptyAST) [] = (ASTNode (ASTSimpleDatum p) l EmptyAST)
                     normalTrees (ASTNode (ASTSimpleDatum p) l EmptyAST) ((a,b):xs) = (ASTNode (ASTSimpleDatum p) l EmptyAST)
                     normalTrees (ASTNode (ASTSimpleDatum p) l r) [] = (ASTNode (ASTSimpleDatum p) (normalTrees l []) (normalTrees r []))
                     normalTrees (ASTNode (ASTSimpleDatum p) l r) ((a,b):xs) = (ASTNode (ASTSimpleDatum p) (normalTrees l ((a,b):xs)) (normalTrees r ((a,b):xs)))
                                          

                     treeNeeded EmptyAST = EmptyAST
                     treeNeeded (ASTNode (ASTLetDatum p) l r)
                                  | ((take 15 (show r)) == (take 15 "ASTNode (ASTLetDatum")) = (treeNeeded r)
                                  | otherwise                                                = r
                     treeNeeded (ASTNode (ASTSimpleDatum p) l r) = EmptyAST

                     el1 = normalTrees y []
                     el2 = eagerCount el1
                     el3 = tree' el1 
                     el4 = if ((isNumber el3) == True) then "num" else "str"
                     astjustn = ASTJust (el3, el4, el2)
