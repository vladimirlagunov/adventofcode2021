import re

with open('day5_input.txt') as f:
    lines = []
    for line in f:
        if m := re.match(r"(\d+),(\d+) *-> *(\d+),(\d+)", line.strip()):
            lines.append((
                (int(m.group(1)), int(m.group(2))),
                (int(m.group(3)), int(m.group(4))),
            ))

# the_map[y][x]
the_map = [
    [0] * max(max(x1, x2) + 1 for (x1, _), (x2, _) in lines)
    for _ in range(max(max(y1, y2) + 1 for (_, y1), (_, y2) in lines))
]

for (x1, y1), (x2, y2) in lines:
    if x1 == x2:
        for y in range(min(y1, y2), max(y1, y2) + 1):
            the_map[y][x1] += 1
    elif y1 == y2:
        for x in range(min(x1, x2), max(x1, x2) + 1):
            the_map[y1][x] += 1

print(sum(
    cell > 1
    for row in the_map
    for cell in row
))
