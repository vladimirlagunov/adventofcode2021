with open('day8_input.txt') as f:
    lines = [
        [s.split() for s in l.strip().split('|')]
        for l in f
        if l.strip()
    ]


#   0:      1:      2:      3:      4:
#  aaaa    ....    aaaa    aaaa    ....
# b    c  .    c  .    c  .    c  b    c
# b    c  .    c  .    c  .    c  b    c
#  ....    ....    dddd    dddd    dddd
# e    f  .    f  e    .  .    f  .    f
# e    f  .    f  e    .  .    f  .    f
#  gggg    ....    gggg    gggg    ....
#
#   5:      6:      7:      8:      9:
#  aaaa    aaaa    aaaa    aaaa    aaaa
# b    .  b    .  .    c  b    c  b    c
# b    .  b    .  .    c  b    c  b    c
#  dddd    dddd    ....    dddd    dddd
# .    f  e    f  .    f  e    f  .    f
# .    f  e    f  .    f  e    f  .    f
#  gggg    gggg    ....    gggg    gggg

def single(i):
    result = next(i)
    if next(i, None) is not None:
        raise Exception
    return result


def guess_digits(ten_examples):
    ten_examples = [frozenset(p) for p in ten_examples]
    segments_of_1 = single(p for p in ten_examples if len(p) == 2)
    segments_of_7 = single(p for p in ten_examples if len(p) == 3)
    segments_of_4 = single(p for p in ten_examples if len(p) == 4)

    segments_of_3 = single(p for p in ten_examples if len(p) == 5 and p & segments_of_1 == segments_of_1)

    segments_of_6 = single(p for p in ten_examples if len(p) == 6 and segments_of_1 & p != segments_of_1)
    segments_of_9 = single(p for p in ten_examples if len(p) == 6 and segments_of_3 & p == segments_of_3)
    segments_of_0 = single(p for p in ten_examples if len(p) == 6 and p != segments_of_6 and p != segments_of_9)

    segment_c = single(iter(segments_of_9 - segments_of_6))
    segment_e = single(iter(segments_of_6 - segments_of_9))

    segments_of_5 = single(p for p in ten_examples if len(p) == 5 and segment_c not in p and segment_e not in p)
    segments_of_2 = single(p for p in ten_examples if len(p) == 5 and segment_c in p and segment_e in p)


    return [
        ''.join(sorted(segments_of_0)),
        ''.join(sorted(segments_of_1)),
        ''.join(sorted(segments_of_2)),
        ''.join(sorted(segments_of_3)),
        ''.join(sorted(segments_of_4)),
        ''.join(sorted(segments_of_5)),
        ''.join(sorted(segments_of_6)),
        ''.join(sorted(segments_of_7)),
        'abcdefg',
        ''.join(sorted(segments_of_9)),
    ]


result = 0
for ten_examples, four_digits in lines:
    try:
        digits = guess_digits(ten_examples)
        decoded = [digits.index(''.join(sorted(d))) for d in four_digits]
        n = 1000 * decoded[0] + 100 * decoded[1] + 10 * decoded[2] + decoded[3]
        print(n)
        result += n
    except:
        print(f'Error in {ten_examples} | {four_digits}')
        raise

print(result)