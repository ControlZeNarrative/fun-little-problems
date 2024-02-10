"""
Solution  for the Zombie Problem.
"""
from heapq import heappush, heappop
from copy import deepcopy

class Zombie_problem(object):
    def __init__(self):
        self.cross_Time = {
            "U": 1,
            "G": 2,
            "PD": 5,
            "PF": 10
        }
        self.start_State = [{"U", "PD", "G", "PF"}, set(), 0, 0]
        self.goal_State = [set(), {"U", "G", "PD", "PF"}]

    
   
    def start_node(self):
        """returns start node"""
        return self.start_State
    
    def is_goal(self, node):
        """is True if `node` is a goal"""
        if node[0:2] == self.goal_State:
            return True
        return False

    def get_combinations(self, node):
        array = list(node[0])
        length = len(array)
        combinations = []

        for i in range(length):
            for j in range(i+1, length):
                combinations.append({array[i], array[j]})

        return combinations
    
    def neighbors(self, node):
        """returns a list of the arcs for the neighbors of `node`"""
        neighbors = []
        if node[3] == 0:
            combinations = self.get_combinations(node)
            for combination in combinations:
                left = node[0].copy()
                left.difference_update(combination)
                right = node[1].copy()
                right.update(combination)
                neighbors.append([node, [left, right, max(self.cross_Time[person] for person in combination), 1]])
        else:
            for person in node[1]:
                left = node[0].copy()
                left.add(person)
                right = node[1].copy()
                right.remove(person)
                neighbors.append([node, [left, right, self.cross_Time[person], 0]])
        

        return neighbors
    
    def neighbors2(self, path):
        """returns a list of the arcs for the neighbors of `node`"""
        neighbors = []
        node = path[-1]
        if node[3] == 0:
            combinations = self.get_combinations(node)
            for combination in combinations:
                return_path = deepcopy(path)
                left = node[0].copy()
                left.difference_update(combination)
                right = node[1].copy()
                right.update(combination)
                return_path.append([left, right, max(self.cross_Time[person] for person in combination), 1])
                neighbors.append(return_path)
        else:
            for person in node[1]:
                return_path = deepcopy(path)
                left = node[0].copy()
                left.add(person)
                right = node[1].copy()
                right.remove(person)
                return_path.append([left, right, self.cross_Time[person], 0])
                neighbors.append(return_path)
        

        return neighbors
        

    def arc_cost(self, arc):
        """Returns the cost of `arc`"""
        return arc[-1][2]
    
    def arc_cost2(self, arc):
        """Returns the cost of `arc`"""
        return arc[2]

    def cost(self, path):
        """Returns the cost of `path`"""
        return sum( self.arc_cost2(arc) for arc in path )

    def heuristic(self, node):
        """Returns the heuristic value of `node`"""
        if len(node[0]) == 0:
            return 0
        h_value = max(self.cross_Time[person] for person in node[0])
        return h_value - (h_value // 6)

    def search(self):
        """Return a solution path"""
        frontier = Frontier()
        frontier.add(self.start_node(), 1)

        while not frontier.is_empty():
            current = frontier.remove()
            if type(current[-1]) == int:
                current = [current]
            if self.is_goal(current[-1]):
                return current
            
            neighbors = self.neighbors2(current)
            for neighbor in neighbors:
                cost = self.arc_cost(neighbor)
                heuristic_value = self.heuristic(neighbor[-1])
                frontier.add(neighbor, cost + heuristic_value)
        
        return None
        

class Frontier(object):
    """
    Convenience wrapper for a priority queue usable as a frontier
    implementation.
    """
    def __init__(self):
        self.heap = []

    def add(self, path, priority):
        """Add `path` to the frontier with `priority`"""
        # Push a ``(priority, item)`` tuple onto the heap so that `heappush`
        # and `heappop` will order them properly
        heappush(self.heap, (priority, path))

    def remove(self):
        """Remove and return the smallest-priority path from the frontier"""
        priority,path = heappop(self.heap)
        return path

    def is_empty(self):
        return len(self.heap) == 0


def unit_tests():
    """
    Some trivial tests to check that the implementation even runs.
    """
    print("testing...")
    p = Zombie_problem()
    assert p.start_node() is not None
    assert not p.is_goal(p.start_node())
    assert p.heuristic(p.start_node()) >= 0

    ns = p.neighbors(p.start_node())
    assert len(ns) > 0

    soln = p.search()
    assert p.cost(soln) > 0
    print("tests ok")

def main():
    unit_tests()
    p = Zombie_problem()
    soln = p.search()
    if soln:
        print("Solution found (cost=%s)\n%s" % (p.cost(soln), soln))
    else:
        raise RuntimeError("Empty solution")

if __name__ == '__main__':
    main()
