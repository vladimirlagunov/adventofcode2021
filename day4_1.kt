import java.io.File

fun main() {
    val reader = File("day4_1_input.txt").reader()

    val numbers: List<Int>
    val boards: List<List<MutableList<Int>>>
    val boardSize = 5

    reader.use {
        val lines = reader.buffered().lines().iterator()

        numbers = lines.next().split(",").map { it.trim().toInt() }
        boards = lines.asSequence()
            .map { it.trim() }
            .filter { it.isNotEmpty() }
            .windowed(size = boardSize, step = boardSize) { rowsStr ->
                rowsStr
                    .map { rowStr ->
                        rowStr.split(Regex(" +"))
                            .mapTo(mutableListOf()) { it.trim().toInt() }
                            .also { assert(it.size == boardSize) }
                    }
            }
            .toList()
    }

    for (number in numbers) {
        for (board in boards) {
            for (row in board) {
                for ((i, v) in row.withIndex()) {
                    if (v == number) {
                        row[i] = Int.MIN_VALUE
                    }
                }
            }
        }

        val result = boards.firstOrNull { board ->
            (0 until boardSize).any { rowNumber -> board[rowNumber].all { it == Int.MIN_VALUE } } ||
                    (0 until boardSize).any { columnNumber -> board.all { it[columnNumber] == Int.MIN_VALUE } }
        }

        if (result != null) {
            val unmarkedSum = result.flatten().sumOf { it.coerceAtLeast(0) }
            println(unmarkedSum * number)
            break
        }
    }
}
