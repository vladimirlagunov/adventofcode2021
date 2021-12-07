with open('day7_input.txt') as f:
    positions = [int(p) for p in next(f).split(',')]

print(min(
    sum((c + 1) * c // 2 for p in positions for c in [abs(p - center)])
    for center in range(min(positions), max(positions) + 1)
))
