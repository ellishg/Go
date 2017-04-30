module Main where
import qualified Data.Map.Strict as M
import qualified Data.Set as S
import Data.Maybe
import Data.List
import Data.Char
import System.Random

-----------------------------------------------------
-- Taken from https://wiki.haskell.org/Random_shuffle
import Data.Array.ST
import Control.Monad
import Control.Monad.ST
import Data.STRef

-- | Randomly shuffle a list without the IO Monad
--   /O(N)/
shuffle' :: [a] -> StdGen -> ([a],StdGen)
shuffle' xs gen = runST (do
        g <- newSTRef gen
        let randomRST lohi = do
              (a,s') <- liftM (randomR lohi) (readSTRef g)
              writeSTRef g s'
              return a
        ar <- newArray n xs
        xs' <- forM [1..n] $ \i -> do
                j <- randomRST (i,n)
                vi <- readArray ar i
                vj <- readArray ar j
                writeArray ar j vi
                return vj
        gen' <- readSTRef g
        return (xs',gen'))
  where
    n = length xs
    newArray :: Int -> [a] -> ST s (STArray s Int a)
    newArray n xs =  newListArray (1,n) xs
-----------------------------------------------------

-----------------------------------------------------
-- Model
boardSize = 9

data Board = Board (M.Map Intersection Player)
  deriving (Eq, Ord)

instance Show Board where
  show (Board m) = foldr makeRow "" $ reverse [0..(pred boardSize)]
    where makeRow j str = str ++ "\n" ++ (foldr (makeTile j) "" $ reverse [0..(pred boardSize)])
          makeTile j i str = str ++ case M.lookup (Intersection i j) m of
                                      Just White -> "[o]"
                                      Just Black -> "[x]"
                                      Nothing    -> "[ ]"

data Stone = Stone Player Intersection
  deriving (Eq, Ord, Show)

data Group = Group Player (S.Set Intersection)
  deriving (Eq, Ord, Show)

data Player = White
            | Black
            | Neither
  deriving (Eq, Ord, Show)

data Intersection = Intersection Int Int
  deriving (Eq, Ord)

instance Show Intersection where
  show (Intersection u v) = "(" ++ (show u) ++  ", " ++ (show v) ++ ")"

opponentPlayer :: Player -> Player
opponentPlayer White = Black
opponentPlayer Black = White
opponentPlayer _     = Neither

validIntersection :: Intersection -> Bool
validIntersection (Intersection u v) =
  and [0 <= u, u < boardSize, 0 <= v, v < boardSize]

allIntersections :: [Intersection]
allIntersections = fmap aux [0..(boardSize * boardSize - 1)]
  where aux n = Intersection (n `quot` boardSize) (n `rem` boardSize)

cardinalDirections :: Intersection -> [Intersection]
cardinalDirections loc@(Intersection u v) =
  if validIntersection loc
    then filter validIntersection [ Intersection (succ u) v
                                  , Intersection (pred u) v
                                  , Intersection u (succ v)
                                  , Intersection u (pred v) ]
  else []

empty :: Board
empty = Board M.empty

fromList :: [Stone] -> Board
fromList xx = Board (M.fromList $ fmap (\(Stone p loc) -> (loc, p)) xx)

validMove :: Board -> Stone -> Bool
validMove board@(Board m) (Stone p loc) =
  and [ validIntersection loc
      , getOwner loc board == Neither
      , getOwner loc newBoard == p]
      where newM = M.insert loc p m
            groups = S.fromList $ fmap (\loc -> getGroup loc $ Board newM) (M.keys newM)
            deadGroups = S.filter (\g -> null $ getLiberties g $ Board newM) groups
            newBoard = foldr removeGroup (Board newM) deadGroups

-- Assume move is valid
applyMove :: Board -> Stone -> Board
applyMove  board@(Board m) (Stone p loc) = newBoard
  where newM = M.insert loc p m
        groups = S.fromList $ fmap (\loc -> getGroup loc $ Board newM) (M.keys newM)
        deadGroups = S.filter (\g -> null $ getLiberties g $ Board newM) groups
        newBoard = foldr removeGroup (Board newM) deadGroups

getOwner :: Intersection -> Board -> Player
getOwner loc (Board m) = case M.lookup loc m of
                          Just s  -> s
                          Nothing -> Neither

clearIntersection :: Intersection -> Board -> Board
clearIntersection loc (Board m) = Board $ M.delete loc m

getGroup :: Intersection -> Board -> Group
getGroup loc board = Group owner $ aux loc S.empty owner
  where owner = getOwner loc board
        aux :: Intersection -> S.Set Intersection -> Player -> S.Set Intersection
        aux loc visited p | S.member loc visited    = S.empty
                          | getOwner loc board == p = S.insert loc $ S.unions newSets
                          | otherwise               = S.empty
          where newSets = fmap (\l -> aux l nVisited p) $ cardinalDirections loc
                nVisited = S.insert loc visited

removeGroup :: Group -> Board -> Board
removeGroup (Group _ locs) board = foldr clearIntersection board locs

getLiberties :: Group -> Board -> S.Set Intersection
getLiberties (Group _ locs) board = S.filter (\l -> getOwner l board == Neither) adjacent
  where adjacent = S.fromList $ concatMap cardinalDirections (S.toList locs)

-----------------------------------------------------
-- AI
pointDifference :: Player -> Board -> Int
pointDifference p b = (scoreForPlayer p b) - (scoreForPlayer (opponentPlayer p) b)
  where scoreForPlayer p (Board m) = length . filter (== p) $ M.elems m

scoreBoard :: StdGen -> Player -> Int -> Int -> Board -> Int
scoreBoard g p depth breadth board = aux g breadth []
  where aux :: StdGen -> Int -> [Int] -> Int
        aux _ 0       acc = sum acc `quot` length acc
        aux g breadth acc = aux g1 (pred breadth) ((pointDifference p newBoard):acc)
          where newBoard = simulateGame g2 p depth board
                (g1, g2) = split g

getNodeChildren :: Player -> Board -> [(Stone, Board)]
getNodeChildren p board = zip stones newNodes
  where newNodes = fmap (applyMove board) stones
        stones = filter (validMove board) $ fmap (\l -> Stone p l) allIntersections

simulateGame :: StdGen -> Player -> Int -> Board -> Board
simulateGame _ _ 0     board = board
simulateGame g p depth board | null newStones = board
                             | otherwise      = simulateGame g1 (opponentPlayer p) (pred depth) newBoard
  where (newMoves, _) = shuffle' allIntersections g2
        newStones = filter (validMove board) $ fmap (\l -> Stone p l) newMoves
        newBoard = applyMove board $ head newStones
        (g1, g2) = split g

findBestMove :: StdGen -> Player -> Int -> Int -> Board -> Stone
findBestMove g p depth breadth board = (\(stone, _) -> stone) $ maximumBy (\(_, a) (_, b) -> compare a b) scores
  where childBoards = getNodeChildren p board
        scores = aux g childBoards []
          where aux :: StdGen -> [(Stone, Board)] -> [(Stone, Int)] -> [(Stone, Int)]
                aux g []              acc = acc
                aux g ((stone, b):xs) acc = aux g1 xs ((stone, score):acc)
                  where score = scoreBoard g2 p depth breadth b
                        (g1, g2) = split g

makeMove g board = do
  inputXA <- getChar
  inputYA <- getChar
  _ <- getLine
  let moveA = Intersection (digitToInt inputXA) (digitToInt inputYA)
  let boardA = applyMove board (Stone Black moveA)
  print boardA

  let (g1, g2) = split g
  let moveB = findBestMove g1 White 10 100 boardA
  let boardB = applyMove boardA moveB
  print boardB
  makeMove g2 boardB


computerVSComputer g board = do
  let (g1, g2) = split g
  let (g3, g4) = split g1
  let moveA = findBestMove g2 Black 5 30 board
  let boardA = applyMove board moveA
  print boardA
  let moveB = findBestMove g3 White 15 100 boardA
  let boardB = applyMove boardA moveB
  print boardB
  computerVSComputer g4 boardB

main :: IO()
main = do
  g <- getStdGen
  --makeMove g empty
  computerVSComputer g empty
