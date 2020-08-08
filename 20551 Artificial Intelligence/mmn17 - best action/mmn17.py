from decimal import Decimal

q_dict = {	"a" : 100, 
			"b" : -3, 
			"c" : 0,
			"d" : 3}
GAMMA = 0.99
ITERATION_NUM = 9999

def get_rewards(r):
	return 	[[r, -1, 10],
			[-1, -1, -1],
			[-1, -1, -1]]

STARTING_UTILITIES = \
[[0, 0, 10],
[0, 0, 0],
[0, 0, 0]]

ACTIONS = {	"up"	: (-1, 0),
			"down"	: (1, 0),
			"left"	: (0, -1),
			"right"	: (0, 1)}

MISGUIDED_ACTIONS = {	"up":		("left", "right"), 
						"down":		("left", "right"), 
						"right": 	("up", "down"), 
						"left": 	("up", "down")}

def pretty_print_array(arr):
	for item in arr:
		print item
	print

def next_location(location, action, height, width):
	new_location = (location[0] + ACTIONS[action][0], location[1] + ACTIONS[action][1])
	if 	(new_location[0] >= height) or \
		(new_location[0] < 0) or \
		(new_location[1] >= width) or \
		(new_location[1] < 0):
		new_location = location
	return new_location

def calc_next_utilities_table(utilities, rewards):
	height = len(utilities)
	width = len(utilities[0])
	next_utilies_table = [[Decimal('-Infinity') for j in xrange(width)] \
							for i in xrange(height)]
	best_action_table = [[None for j in xrange(width)] \
							for i in xrange(height)]
	for i in xrange(height):
		for j in xrange(width):
			# reached terminal state
			if i == 0 and j == width - 1:
				next_utilies_table[i][j] = rewards[i][j]
				best_action_table[i][j] = None
			else:
				best_action = ACTIONS.keys()[0]
				best_utility = Decimal('-Infinity')
				for action in ACTIONS:
					forward_location = next_location((i, j), action, height, width)
					mistake_1_location = next_location((i, j), MISGUIDED_ACTIONS[action][0], height, width)
					mistake_2_location = next_location((i, j), MISGUIDED_ACTIONS[action][1], height, width)					
					utility = rewards[i][j] + GAMMA * (0.8 * utilities[forward_location[0]][forward_location[1]] \
														+ 0.1 * utilities[mistake_1_location[0]][mistake_1_location[1]] \
														+ 0.1 * utilities[mistake_2_location[0]][mistake_2_location[1]])
					if utility > best_utility:
						best_utility = utility
						best_action = action
				next_utilies_table[i][j] = best_utility
				best_action_table[i][j] = best_action
	return next_utilies_table, best_action_table

def main():
	for q in sorted(q_dict.keys()):
		r = q_dict[q]
		print "question %s, r=%d" %(q, r)
		utilities = STARTING_UTILITIES
		rewards = get_rewards(r)
		pretty_print_array(rewards)
		for i in xrange(ITERATION_NUM):
			utilities, best_actions = calc_next_utilities_table(utilities, rewards)
		pretty_print_array(utilities)
		pretty_print_array(best_actions)
		print "_" * 50

main()