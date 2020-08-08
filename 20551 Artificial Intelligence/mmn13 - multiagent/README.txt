name: 	Itai Antebi
ID:		204817498

Q01 - 	My algorythm heads toward the closest food with high penalties for very close unafraid ghosts. 
		In this manner, its chances of dying are very low and once the ghost is not near - it heads for food. 
		Thus, it easily wins in openClassic withoud dying and without walking back and forth in a stuck state. 
		
Q02 - 	My algorythm heads towards MAX value for pacman move and MIN value for every ghost on board. 
		A stage is one pacman move and then one ghost move for every ghost in the game. 
		The algorythm will continue searching untill reaches a win/lose state or depth-stages is reached. 
		Once depth-stages is reached in the search, I use the evaluationFunction to evaluate the state. 
		My minimax values for the start of the game in minimaxClassic are 9,8,7,-492 for depth-stages 1,2,3,4 (accordingly)
		- as required
		In trappedClassic the pacman heads towards suicide because he sees that if the ghosts are perfect players - 
		he is going to die in every scenario. Thus, the sooner he dies - the better score he will get (he is penalized for time). 
		
Q03 - 	This solution is similar to Q02, except it runs a bit faster because *I prun all choices that I know in advance are irrelevant* - 
		thanks to AlphaBeta prunning algorythm. 
		Since the solutions themselves are the same, in this question too my minimax values for the start of the game in 
		minimaxClassic are 9,8,7,-492 for depth-stages 1,2,3,4 (accordingly)- as required

Q04 - 	This soltution is similar to Q02, except it takes into consideration the fact that ghosts are somewhat random. 
		Thus, instead of assuming the ghosts are perfect players which choose the minimum value possible at all times, 
		it calculates the value of a ghost-turn node as an *average* of all values of possible choices of that ghost. 
		Indeed I see that in trappedClassic:
		AlphaBetaAgent loses 100% of times for reasons explained in Q02 - Is assumes ghosts are perfect players and commits suicide and 
		ExpectimaxAgent wins 50% of times because he takes into consideration the fact that he might wih (because ghosts are not perfect players)