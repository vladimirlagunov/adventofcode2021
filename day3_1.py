total_count = 0
ones = ()

for line in open('day3_1_input.txt'):
    line = line.strip()
    total_count += 1
    if not ones:
        ones = (0, ) * len(line)
    ones = tuple(ones[i] + (v == '1') for i, v in enumerate(line))

print(ones)

gamma = int(''.join('1' if v > total_count / 2.0 else '0' for v in ones), 2)
sigma = (2 ** len(ones) - 1) ^ gamma

print(gamma, sigma)

print(gamma * sigma)