import re
from typing import List, Tuple, Dict

edges: List[Tuple[str, str]] = []

with open('day12_input.txt') as f:
    for line in f:
        if m := re.match("([A-Za-z]+)-([A-Za-z]+)", line.strip()):
            edges.append((m.group(1), m.group(2)))
            if m.group(1) != 'start':
                edges.append((m.group(2), m.group(1)))


def iterate(current_vertex: str, small_visited: Dict[str, int], path: List[str]) -> int:
    if current_vertex == 'end':
        return 1
    else:
        count = 0
        if current_vertex == current_vertex.lower() and current_vertex != 'start':
            small_visited = dict(small_visited)
            small_visited.setdefault(current_vertex, 0)
            if small_visited[current_vertex] >= 2:
                return count
            small_visited[current_vertex] += 1
            if sum(v >= 2 for v in small_visited.values()) > 1:
                return count
        for from_vertex, to_vertex in edges:
            if from_vertex == current_vertex:
                new_path = path + [from_vertex]
                count += iterate(to_vertex, small_visited, new_path)
        return count


print(iterate('start', {}, []))
