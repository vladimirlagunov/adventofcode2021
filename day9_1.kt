import java.io.FileReader
import kotlin.streams.toList

fun main() {
    val reader = FileReader("day9_input.txt").buffered()

    val map: List<List<Int>> = reader.use {
        reader.lineSequence()
            .map { it.trim() }
            .filter { it.isNotEmpty() }
            .mapTo(mutableListOf()) { row ->
                row.chars()
                    .map { char -> char - '0'.code }
                    .toList()
            }
    }

    fun get(x: Int, y: Int) = map.getOrNull(y)?.getOrNull(x) ?: Int.MAX_VALUE

    println(map.indices.asSequence()
        .flatMap { y -> map.first().indices.map { x -> x to y } }
        .sumOf { (x, y) ->
            val minNeighbour = get(x - 1, y)
                .coerceAtMost(get(x + 1, y))
                .coerceAtMost(get(x, y - 1))
                .coerceAtMost(get(x, y + 1))
            map[y][x].takeIf { it < minNeighbour }?.plus(1) ?: 0
        })
}