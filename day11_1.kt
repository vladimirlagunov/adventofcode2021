import java.io.File

fun main() {
    val reader = File("day11_input.txt").bufferedReader()
    val map: List<IntArray> = reader.use {
        reader.lineSequence()
            .mapTo(mutableListOf()) { row -> row.map { "$it".toInt() }.toIntArray() }
    }

    var flashes = 0

    fun incOnFlash(x: Int, y: Int) {
        val row = map.getOrNull(y) ?: return
        if (x in row.indices) {
            if (map[y][x] > 0) {
                row[x] += 1
            }
        }
    }

    repeat(100) {
        for (y in map.indices) {
            for (x in map[0].indices) {
                map[y][x] += 1
            }
        }
        do {
            val flashesBefore = flashes
            for (y in map.indices) {
                for (x in map[0].indices) {
                    if (map[y][x] > 9) {
                        flashes += 1
                        map[y][x] = -1
                        incOnFlash(x - 1, y - 1)
                        incOnFlash(x, y - 1)
                        incOnFlash(x + 1, y - 1)
                        incOnFlash(x - 1, y)
                        incOnFlash(x + 1, y)
                        incOnFlash(x - 1, y + 1)
                        incOnFlash(x, y + 1)
                        incOnFlash(x + 1, y + 1)
                    }
                }
            }
        } while (flashesBefore < flashes)
        for (y in map.indices) {
            for (x in map[0].indices) {
                map[y][x] = map[y][x].coerceAtLeast(0)
            }
        }
    }

    println(flashes)
}