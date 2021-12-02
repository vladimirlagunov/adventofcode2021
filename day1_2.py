count = 0

with open('day1_2_input.txt') as f:
    inp = [int(l.strip()) for l in f]

inp2 = iter(inp)
next(inp2)

inp3 = iter(inp)
next(inp3)
next(inp3)

inp = iter(inp)

count = 0
previous = next(inp) + next(inp2) + next(inp3)

for a, b, c in zip(inp, inp2, inp3):
    current = a + b + c
    if current > previous:
        count += 1
    previous = current

print(count)
