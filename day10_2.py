def correct(line):
    stack = []
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
                return []
    return stack[::-1]


def main():
    cost = {
        ')': 1,
        ']': 2,
        '}': 3,
        '>': 4,
    }

    points = []
    with open('day10_input.txt') as f:
        for line in f:
            remains = correct(line.strip())
            line_score = 0
            for c in remains:
                line_score = line_score * 5 + cost[c]
            if line_score:
                points.append(line_score)

    points.sort()
    print(points[len(points) // 2])


if __name__ == '__main__':
    main()
