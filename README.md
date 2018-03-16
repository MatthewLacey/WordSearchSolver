# WordSearchSolver
A program to find all words greater than a given length in a word search of arbitrary dimensions.

A word search puzzle is an N×M array of lower-case letters, and the objective is to locate and output all dictionary words of length L or more that appear in this grid horizontally, vertically, or diagonally and either forwards or backwards (but no wrap-arounds).

For example, given the following puzzle and a minimum length of six,

r d z i t p m f

t e k a n s t d

t i b b a r o o

k e l a h w a a

c j i e p n d k

s d e o e m z i

h z i y l a t x

i s h h e e l s

the output should be:

elephant from (7, 4) to (0, 4)
rabbit from (2, 5) to (2, 0)

This code:

   - handles puzzles of arbitrary dimensions

   - uses the dictionary words.txt 

   - use an unordered_set<std::string> to store the dictionary

   - sorts the output list of words of length L or more, along with their starting and ending positions alphabetically.
