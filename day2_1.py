import re

x = 0
y = 0

for line in open('day2_1_input.txt'):
    if not (m := re.match('(forward|down|up) (\d+)', line.strip())):
        continue
    elif m.group(1) == 'forward':
        x += int(m.group(2))
    elif m.group(1) == 'down':
        y += int(m.group(2))
    else:
        y -= int(m.group(2))

print(x * y)