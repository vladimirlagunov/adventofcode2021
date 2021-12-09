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

    val pools: List<IntArray> = map.indices.map { IntArray(map.first().size) { Int.MAX_VALUE } }
    var poolCounter = 0

    fun getPool(x: Int, y: Int) = pools.getOrNull(y)?.getOrNull(x) ?: Int.MAX_VALUE

    do {
        var anyFill = false
        for ((y, row) in map.withIndex()) {
            for ((x, v) in row.withIndex()) {
                if (v == 9) continue
                val poolId =
                    pools[y][x]
                        .coerceAtMost(getPool(x - 1, y))
                        .coerceAtMost(getPool(x + 1, y))
                        .coerceAtMost(getPool(x, y - 1))
                        .coerceAtMost(getPool(x, y + 1))
                        .takeIf { it != Int.MAX_VALUE }
                    ?: poolCounter
                        .also { poolCounter += 1 }
                if (pools[y][x] == poolId) continue
                anyFill = true
                pools[y][x] = poolId
            }
        }
    } while (anyFill)

    val poolSizes =
        pools.asSequence()
            .flatMap { poolRow -> poolRow.asSequence() }
            .filter { poolId -> poolId != Int.MAX_VALUE }
            .groupingBy { poolId -> poolId }
            .eachCount()

    println(poolSizes.values.sortedDescending().take(3).reduce { a, b -> a * b })
}