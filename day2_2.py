import re

x = 0
y = 0
aim = 0

for line in open('day2_2_input.txt'):
    if not (m := re.match('(forward|down|up) (\d+)', line.strip())):
        continue
    elif m.group(1) == 'forward':
        x += int(m.group(2))
        y += int(m.group(2)) * aim
    elif m.group(1) == 'down':
        aim += int(m.group(2))
    else:
        aim -= int(m.group(2))

print(x * y)