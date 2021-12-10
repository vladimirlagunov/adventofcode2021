def parse(line):
    stack = []
    errors = []
    for i in range(len(line)):
        c = line[i]
        if stack and c == stack[-1]:
            stack.pop()
        else:
            for p in '()', '{}', '[]', '<>':
                if c == p[0]:
                    stack.append(p[1])
                    break
            else:
                errors.append(c)
                break
    return errors


def main():
    cost = {
        ')': 3,
        ']': 57,
        '}': 1197,
        '>': 25137,
    }

    points = 0
    with open('day10_input.txt') as f:
        for line in f:
            errors = parse(line.strip())
            points += sum(cost[c] for c in errors)

    print(points)


if __name__ == '__main__':
    main()
