# arrange-sentences
An algorithm for arranging sentences within a story in alphabetical order. This algorithm consists of two sentence sorting stages: “rough sorting” and “fine sorting”.  

The rough sorting part of the algorithm employs hashing and sorts sentences by the first letter of a sentence. Running time of this part is O(n), where n is the number of sentences.  

The fine sorting part of the algorithm cross-compare sentences as strings taking each character into account. Running time of this part is O(n^2l), where n is the number of sentences and l is the max number of characters in a sentence. 

Dependently on the end use of this application, fine sorting might be turned of for speeding up the computation process.  

There are four files in this repository: "info_page_w_complexity.c", "Makefile", "info_page.pdf" and “TheLastQuestion.txt”.  "arrangeSentences.c", "Makefile" are the program itself, "info_page.pdf" is a short explanation and complexity analysis of the solution as well as some of my thoughts on it. “TheLastQuestion.txt” is a test story.
