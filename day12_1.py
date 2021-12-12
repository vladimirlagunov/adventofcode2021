import re
from typing import List, Tuple

edges: List[Tuple[str, str]] = []

with open('day12_input.txt') as f:
    for line in f:
        if m := re.match("([A-Za-z]+)-([A-Za-z]+)", line.strip()):
            edges.append((m.group(1), m.group(2)))
            edges.append((m.group(2), m.group(1)))


def iterate(current_vertex: str, small_visited: frozenset, path: List[str]) -> int:
    if current_vertex == 'end':
        return 1
    else:
        count = 0
        if current_vertex == current_vertex.lower():
            small_visited = small_visited | frozenset((current_vertex,))
        for from_vertex, to_vertex in edges:
            if from_vertex == current_vertex and to_vertex not in small_visited:
                new_path = path + [from_vertex]
                count += iterate(to_vertex, small_visited, new_path)
        return count


print(iterate('start', frozenset(), []))
