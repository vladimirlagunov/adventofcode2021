import java.io.BufferedReader;
import java.io.FileReader;
import java.io.StringReader;
import java.util.HashMap;
import java.util.Map;
import java.util.stream.Collectors;

public class Day6 {
    public static void main(String[] args) throws Exception {
        Map<Integer, Long> lanternFishesByAge = new HashMap<>();
        try (var reader = new BufferedReader(new FileReader("day6_input.txt"))) {
            for (String token : reader.readLine().split(",")) {
                lanternFishesByAge.merge(Integer.parseInt(token), 1L, Long::sum);
            }
        }

        for (int day = 1; day <= 256; ++day) {
            var oldValues = new HashMap<>(lanternFishesByAge);
            lanternFishesByAge = new HashMap<>();
            Long lastDayCount = oldValues.remove(0);

            for (var oldValue : oldValues.entrySet()) {
                lanternFishesByAge.put(oldValue.getKey() - 1, oldValue.getValue());
            }

            if (lastDayCount != null) {
                lanternFishesByAge.merge(8, lastDayCount, Long::sum);
                lanternFishesByAge.merge(6, lastDayCount, Long::sum);
            }

            if (day == 18 || day == 80 || day == 256) {
                System.out.println(
                        day + ": " +
                        lanternFishesByAge.values().stream().collect(Collectors.summarizingLong(x -> x)).getSum());
            }
        }
    }
}
