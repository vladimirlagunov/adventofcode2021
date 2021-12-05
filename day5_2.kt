import java.io.File
import kotlin.math.abs
import kotlin.math.max

fun main() {
    val reader = File("day5_input.txt").bufferedReader()
    data class Line(val x1: Int, val y1: Int, val x2: Int, val y2: Int)

    val lines: List<Line> = reader.use {
        reader.lineSequence().mapNotNullTo(mutableListOf()) { rawLine ->
            Regex("""(\d+),(\d+) *-> *(\d+),(\d+)""").matchEntire(rawLine)?.let { match ->
                val (_, x1, y1, x2, y2) = match.groupValues
                Line(x1 = x1.toInt(), y1 = y1.toInt(), x2 = x2.toInt(), y2 = y2.toInt())
            }
        }
    }

    // map[y][x]
    val map: List<IntArray> =
        (0..lines.maxOf { max(it.y1, it.y2) }).map {
            IntArray(lines.maxOf { max(it.x1, it.x2) + 1 }) { 0 }
        }

    fun autoSeq(a: Int, b: Int): Sequence<Int> =
        if (a < b) (a..b).asSequence()
        else (b..a).reversed().asSequence()

    for (line in lines) {
        val (xIter: Sequence<Int>, ySeq: Sequence<Int>) = line.run {
            when {
                x1 == x2 ->
                    generateSequence { x1 } to autoSeq(y1, y2)
                y1 == y2 ->
                    autoSeq(x1, x2) to generateSequence { y1 }
                abs(x1 - x2) == abs(y1 - y2) ->
                    autoSeq(x1, x2) to autoSeq(y1, y2)
                else ->
                    emptySequence<Int>() to emptySequence()
            }
        }

        for ((x, y) in xIter.zip(ySeq)) {
            map[y][x] += 1
        }
    }

    println(map.sumOf { row -> row.sumOf { if (it > 1) 1L else 0L } })
}