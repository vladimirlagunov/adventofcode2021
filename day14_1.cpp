#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

struct Rule {
    char left;
    char right;
    char putBetween;

    Rule() = default;

    Rule(char left, char right, char putBetween)
            : left{left}
            , right{right}
            , putBetween{putBetween} {}
};

struct Data {
    std::string template_;
    std::vector<Rule> rules;

    Data() = default;
};

Data readData(std::istream &&inputStream) {
    Data result;
    inputStream >> result.template_;
    std::string left, arrow, right;
    while (!inputStream.eof()) {
        inputStream >> left >> arrow >> right;
        if (arrow != "->" || left.length() != 2 || right.length() != 1) {
            std::cerr << "Wrong input: " << left << arrow << right << std::endl;
            exit(1);
        }
        result.rules.emplace_back(Rule(left[0], left[1], right[0]));
    }

    return result;
}

Data readData(std::fstream &&fileStream) {
    auto result = readData((std::istream &&) fileStream);
    fileStream.close();
    return result;
}

void substituteData(Data &data) {
    std::stringstream newTemplate;
    auto left = data.template_.cbegin();
    newTemplate << *left;
    auto right = data.template_.cbegin();
    ++right;
    for (; left != data.template_.cend() && right != data.template_.cend(); ++left, ++right) {
        bool ok = false;
        for (const auto &rule: data.rules) {
            if (rule.left == *left && rule.right == *right) {
                newTemplate << rule.putBetween << rule.right;
                ok = true;
                break;
            }
        }
        if (!ok) {
            throw std::logic_error((std::stringstream() << "Failed to find rule for " << *left << *right).str());
        }
    }
    data.template_ = newTemplate.str();
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

    for (size_t i = 0; i < 10; ++i) {
        substituteData(data);
    }

    std::vector<size_t> charCounts(26, 0);
    for (char c: data.template_) {
        ++charCounts[c - 'A'];
    }
    charCounts.erase(std::remove_if(charCounts.begin(), charCounts.end(), [](auto x) { return x == 0; }),
                     charCounts.end());
    auto[min, max] = std::minmax_element(charCounts.begin(), charCounts.end());
    std::cout << *max - *min << std::endl;

    return 0;
}
