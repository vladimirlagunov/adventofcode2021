#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>

struct ArrayHasher {
    std::size_t operator()(const std::array<char, 2> &key) const {
        return key[0] * 31 + key[1];
    }
};

struct Data {
    std::array<size_t, 26> chars{0};
    std::unordered_map<std::array<char, 2>, size_t, ArrayHasher> twograms{};
    std::unordered_map<std::array<char, 2>, char, ArrayHasher> rules{};
};

template<typename K, typename H>
void increment(std::unordered_map<K, size_t, H> &map, const K &key, size_t value = 1) {
    auto it = map.find(key);
    if (it != map.cend()) {
        it->second += value;
    } else {
        map[key] = value;
    }
}

template<typename K>
size_t getOrZero(std::unordered_map<K, size_t> &map, const K &key) {
    auto it = map.find(key);
    if (it != map.cend()) {
        return it->second;
    } else {
        return 0;
    }
}

Data readData(std::istream &&inputStream) {
    Data result;
    std::string template_;
    inputStream >> template_;
    for (auto l = template_.cbegin(), r = ++template_.cbegin();
         l != template_.cend() && r != template_.cend();
         ++l, ++r) {
        increment(result.twograms, {*l, *r});
    }

    for (char c: template_) {
        ++result.chars[c - 'A'];
    }

    std::string left, arrow, right;
    while (!inputStream.eof()) {
        inputStream >> left >> arrow >> right;
        if (arrow != "->" || left.length() != 2 || right.length() != 1) {
            std::cerr << "Wrong input: " << left << arrow << right << std::endl;
            exit(1);
        }
        result.rules[{left[0], left[1]}] = right[0];
    }

    return result;
}

Data readData(std::fstream &&fileStream) {
    auto result = readData((std::istream &&) fileStream);
    fileStream.close();
    return result;
}

//void printData(const Data &data) {
//    std::cerr << "Chars:";
//    for (size_t i = 0; i < data.chars.size(); ++i) {
//        if (data.chars[i] != 0) {
//            std::cerr << " " << char('A' + i) << "=" << data.chars[i];
//        }
//    }
//    std::cerr << std::endl;
//
//    std::cerr << "Twograms:" << std::endl;
//    for (auto[k, v]: data.twograms) {
//        if (v > 0) {
//            auto[l, r] = k;
//            std::cerr << "  " << l << r << ": " << v << std::endl;
//        }
//    }
//}

void substituteData(Data &data) {
    std::stringstream newTemplate;
    decltype(data.twograms) oldTwogramsCopy(data.twograms);

    for (const auto[key, oldNumber]: oldTwogramsCopy) {
        if (0 == oldNumber) {
            continue;
        }

        auto rule = data.rules.find(key);
        if (rule == data.rules.cend()) {
            std::cerr << "No rule for " << key[0] << key[1] << std::endl;
            exit(1);
        }

        auto between = rule->second;
        auto[left, right] = rule->first;

//        std::cerr << "Apply " << left << right << " -> " << between << " " << oldNumber << " times; "
//                  << between << " grows from " << data.chars[between - 'A']
//                  << " to " << data.chars[between - 'A'] + oldNumber
//                  << std::endl;

        data.chars[between - 'A'] += oldNumber;
        increment(data.twograms, {left, between}, oldNumber);
        increment(data.twograms, {between, right}, oldNumber);
        increment(data.twograms, {left, right}, -oldNumber);
    }
}

int main() {
//    auto data = readData(std::stringstream() <<
//                                             "NNCB\n"
//                                             "\n"
//                                             "CH -> B\n"
//                                             "HH -> N\n"
//                                             "CB -> H\n"
//                                             "NH -> C\n"
//                                             "HB -> C\n"
//                                             "HC -> B\n"
//                                             "HN -> C\n"
//                                             "NN -> C\n"
//                                             "BH -> H\n"
//                                             "NC -> B\n"
//                                             "NB -> B\n"
//                                             "BN -> B\n"
//                                             "BB -> N\n"
//                                             "BC -> B\n"
//                                             "CC -> N\n"
//                                             "CN -> C");
    auto fstream = std::fstream();
    fstream.open("day14_input.txt", std::ios_base::in);
    auto data = readData(std::move(fstream));

    //printData(data);
    for (size_t i = 0; i < 40; ++i) {
        std::cerr << std::endl << "======= " << i << std::endl << std::endl;
        substituteData(data);
        std::cerr << std::endl;
        //printData(data);
    }

    std::vector<size_t> charCounts(data.chars.cbegin(), data.chars.cend());
    charCounts.erase(std::remove_if(charCounts.begin(), charCounts.end(), [](auto x) { return x == 0; }),
                     charCounts.end());
    auto[min, max] = std::minmax_element(charCounts.begin(), charCounts.end());

    std::cout << *max - *min << std::endl;

    return 0;
}
