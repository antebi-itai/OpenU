student name:	Itai Antebi
student ID:		204817498

Q01:
	I decided to use implement a "general search" function that performs a search from a start state
	to the goal state using a given stracture. 
	The stracture will hold nodes that will be a tuple (state, actions, cost), 
	where "state" is the state reached, "actions" is a list of actions leading to that state with the total cost of "cost". 
	Holding all this information in each node will ensure us that all stractures will have enough information 
	to choose which node to pop next. 
	Different stractures will manage their nodes differently, thus popping the nodes at a different order and causing a different search. 
	This is, obviously, a direct result of the hints given in the question. 
	
	DFS is implemented simply by calling "general search" where a Stack (implemented in util.py) is the stracture. 
	Stack will pop the last node pushed in and cause the search to be DFS. 
	Indeed, the solution found for mediumMaze is at total cost of 130. 
	
	This is NOT the best (cheapest) solution to the problem. 
	DFS does NOT ensure us an optimal solution, and as we will see in Q02, the best solution to Q02 is better than 130. 
	
Q02:
	Since we already implemented a "general search" function, 
	BFS is implemented simply by calling "general search" where a Queue (implemented in util.py) is the stracture. 
	
	BFS DOES find the best solution to a problem, since he scanes all states reached by a path of length n-1 before
	he scanes the states reached by a path of length n. 
	Thus, if a solution was found by a path of length n, there is no shorter path to the solution. 
	As we can see, as opposed to Q01 in which the solution to mediumMaze was found with a total cost of 130, 
	now a solution to mediumMaze is found with a total cose of 68. 

Q03:
	We will again call the "general search" function, but in order to ensure that the stracture will pop the node of least total cost first - 
	we will use the PriorityQueue stracture where the priority of a state will be the total cost it took to get to that state. 
	Thus, every time the PriorityQueue will pop the state of least cost first, as needed for UCS. 

Q04:
	We will again call the "general search" function, but in order to ensure that the stracture will pop the node of lowest combined cost and heuristic first - 
	we will use the PriorityQueue stracture where the priority of a state will be the combined cost and heuristic. 
	
	A* (using manhattenHeuristic) indeed finds the best solution to bigMaze (210), and a bit faster than UCS (538 nodes expanded vs 619)
	
	In openMaze, DFS finds a terrible solution of cost 210. 
	BFS and UCS are the same, since the cost of each step is 1. They both find the optimal solution of cost 54 after expanding 682 nodes. 
	A* (using manhattenHeuristic) also finds the optimal solution, but after expanding only 211 nodes (impressive!)

Q05:
	I chose to implement the problem where each state is a tuple: (position, corners_not_yet_visited). 
	This answers the requirements of the question: "define a state which does not encode more information than needed". 
	Pacman reaches its goal state when there are no corners left unvisited. 
	
	We notice that indeed BFS finds the best solution to tinyCorners in the cost of 28, 
	the imlementation of BFS solves mediumCorners by expanding less than 2000 nodes (1966)
	and using cornersHeuristic A* solves mediumCorners by expanding only 692 nodes (see Q06). 

Q06:
	I chose to implement a greedy heuristic that iterates over all permutations of the corners not yet visited, 
	and calculates the manhatten distance of the path from our location through all the corners. 
	It assumes there are no walls (i.e. the mazeDistance to a corner is equal to the manhattenDistance). 
	Thus, the cost of the optimal solution could be the value of the heuristic (in perfect circumstances of no walls) 
	or higher (if there are walls in such a way that interfere with our ability to reach a corner in the manhatten distance of it). 
	This is why my heuristic is addmissible, as requested. 
	Also, the number of nodes expanded in A* (using cornersHeuristic) is 741 (less than 800!)

Q07:
	I chose to implement a heuristic that calculates the mazeDistance of the farthest food. 
	The heuristic assumes that my path to the farthest food goes through all the other food. 
	This is obviously not always true, but the real distance to solve the problem is at least the value of the heuristic. 
	This is why my heuristic is addmissible. 
	Also, the number of nodes expanded in A* (using foodHeuristic) is 4123 (less than 7000!)
	There is no way a step of cost 1 will make the heuristic smaller than the previous heuristic value minus 1, 
	since a step of cost 1 (all steps are of cost 1) can bring the state closer to the farthest food by at most 1. 
	This is why my heuristic is consistent as well :)

Q08:
	I used bfs to find a closest solution to the AnyFoodSearchProblem. 
	To use this, I implemented missing parts of AnyFoodSearchProblem causing a solution to be True if pacman got to any food. 
	This enabled me to use bfs to find the closest food and go there. 
	Then, self.actions will have all actions needed to go to the closest food at any time. 
	My agent also found the solution in less than a second with cost of 350 :)