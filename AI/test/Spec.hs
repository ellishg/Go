module Spec where
import Main hiding (main)
import qualified Data.Map.Strict as M
import qualified Data.Set as S
import Data.List
import Data.Maybe

main :: IO ()
main = do putStrLn ""
          putStrLn "Running Tests"
          putStrLn "============="
          mapM_ putStrLn $ fmap runTest allTests


runTest :: (Bool, String) -> String
runTest (True , s) = s ++ ": PASSED"
runTest (False, s) = s ++ ": FAILED"

allTests :: [(Bool, String)]
allTests = [ (test_validIntersection, "validIntersection")
           , (test_invalidIntersection, "invalidIntersection")
           , (test_cardinalDirections, "cardinalDirections")
           , (test_groups, "getGroup")
           , (test_liberties, "getLiberties")
           , (test_applyMoves, "applyMove")
           , (test_getOwners, "getOwner")
           ]

test_validIntersection = all validIntersection [ Intersection 0 0
                                               , Intersection 4 4 ]

test_invalidIntersection = not $ any validIntersection [ Intersection 5 5
                                                       , Intersection 0 (-1) ]

test_cardinalDirections =
  and [ null $ cardinalDirections (Intersection 0 18) \\ [Intersection 0 17, Intersection 1 18]
      , null $ cardinalDirections (Intersection 5 5) \\ [Intersection 4 5, Intersection 6 5, Intersection 5 4, Intersection 5 6]]

moves :: [[Stone]]
moves = [
    [Stone White (Intersection 2 2), Stone White (Intersection 2 3), Stone White (Intersection 1 2)]
  , [Stone Black (Intersection 1 0), Stone White (Intersection 1 1), Stone Black (Intersection 0 1)]
  ]

boards :: [Board]
boards = map fromList moves

intersections :: [Intersection]
intersections = [
    Intersection 2 2
  , Intersection 1 1
  ]

owners :: [Player]
owners = [
    White
  , White
  ]

groups :: [Group]
groups = [
    Group White (S.fromList [Intersection 2 2, Intersection 2 3, Intersection 1 2])
  , Group White (S.singleton $ Intersection 1 1)
  ]

liberties :: [S.Set Intersection]
liberties = [
      S.fromList [Intersection 2 4, Intersection 1 3, Intersection 0 2, Intersection 1 1, Intersection 2 1, Intersection 3 2, Intersection 3 3]
    , S.fromList [Intersection 2 1, Intersection 1 2]
  ]

test_getOwner (loc, board, owner) = getOwner loc board == owner
test_getOwners = all test_getOwner $ zip3 intersections boards owners

test_group (loc, board, group) = getGroup loc board == group
test_groups = all test_group $ zip3 intersections boards groups

test_liberty (group, board, libs) = getLiberties group board == libs
test_liberties = all test_liberty $ zip3 groups boards liberties

test_applyMove (stones, b) = foldl applyMove empty stones == b
test_applyMoves = all test_applyMove $ (zip moves boards)
