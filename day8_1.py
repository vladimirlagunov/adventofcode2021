with open('day8_input.txt') as f:
    lines = [
        [s.split() for s in l.strip().split('|')]
        for l in f
        if l.strip()
    ]

digits = [
    frozenset('abcefg'),
    frozenset('cf'),
    frozenset('acdeg'),
    frozenset('acdfg'),
    frozenset('bcdf'),
    frozenset('abdfg'),
    frozenset('abdefg'),
    frozenset('acf'),
    frozenset('abcdefg'),
    frozenset('abcdfg'),
]

print(sum(
    len(p) == 2 or len(p) == 3 or len(p) == 4 or len(p) == 7
    for l in lines
    for p in l[1]
))
