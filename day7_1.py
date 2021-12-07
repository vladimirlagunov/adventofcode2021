with open('day7_input.txt') as f:
    positions = [int(p) for p in next(f).split(',')]

print(min(
    sum(abs(p - center) for p in positions)
    for center in range(min(positions), max(positions) + 1)
))
