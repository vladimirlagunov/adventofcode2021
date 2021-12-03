data = [line.strip() for line in
        open('day3_2_input.txt')
        if line.strip()
        ]

oxy = list(data)
for i in range(len(data[0])):
    ones = (0,) * len(data[0])
    for line in oxy:
        ones = tuple(ones[i] + (v == '1') for i, v in enumerate(line))

    if len(oxy) < 2:
        break
    oxy = [line for line in oxy if int(line[i]) == (ones[i] >= len(oxy) - ones[i])]
    print(i, len(oxy), ones[i], oxy)

co2 = list(data)
for i in range(len(data[0])):
    ones = (0,) * len(data[0])
    for line in co2:
        ones = tuple(ones[i] + (v == '1') for i, v in enumerate(line))

    if len(co2) < 2:
        break
    co2 = [line for line in co2 if int(line[i]) != (ones[i] >= len(co2) - ones[i])]
    print(i, len(co2), ones[i], co2)

print(oxy, co2)

print(int(oxy[0], 2) * int(co2[0], 2))
