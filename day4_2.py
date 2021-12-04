from itertools import groupby

board_size = 5

with open("day4_1_input.txt") as f:
    source = iter(f)
    numbers = [int(n.strip()) for n in next(source).split(',')]
    boards = [
        [
            [int(n) for n in line.split()]
            for _, line in board_raw
        ]
        for _, board_raw in groupby(
            enumerate(
                line.strip()
                for line in source
                if line.strip()
            ),
            lambda n: n[0] // board_size
        )
    ]


def get_winners():
    return [
        any(all(n == '*' for n in board[row_number]) for row_number in range(board_size))
        or any(all(r[column_number] == '*' for r in board) for column_number in range(board_size))
        for board in boards
    ]


for number in numbers:
    was_winner = get_winners()

    for board in boards:
        for row in board:
            for i, v in enumerate(row):
                if v == number:
                    row[i] = '*'

    is_winner = get_winners()

    if all(is_winner):
        board_without_bingo = boards[was_winner.index(False)]
        unmarked_sum = sum(n if n != '*' else 0 for row in board_without_bingo for n in row)
        print(unmarked_sum * number)
        break
