count = 0

with open('day1_1_input.txt') as inp:
    previous = int(next(inp).strip())
    for line in inp:
        current = int(line.strip())
        if current > previous:
            count += 1
        previous = current

print(count)
