#include <iostream>
#include <vector>
#include <algorithm>

class Rule {
public:
    std::string Key;    // Порождающая цепочка языка
    std::string Value;  // Порождаемая цепочка языка
    bool IsLooped;      // Введет ли правило зацикливанию.

    Rule(const std::string& k, const std::string& v, bool l = false) : Key(k), Value(v), IsLooped(l) {}
};

void PrintRules(const std::vector<Rule>& R) {
    std::cout << "Rules of language" << std::endl;
    for (const Rule& rule : R) {
        std::cout << "   \u2022" << rule.Key << "-->" << rule.Value << std::endl;
    }
}

class FormalLanguage {
private:
    std::vector<Rule> _rules;
    int MaxRepetitionsCount;

    bool CheckLoop(std::string& input, const Rule& rule, int count = 5) {
        for (int i = 0; i < count; i++) {
            size_t pos = input.find(rule.Key);

            if (pos != std::string::npos) {
                input.replace(pos, rule.Key.length(), rule.Value);
            }
            else {
                return false;
            }
        }
        return true;
    }

    void RefreshRules() {
        for (Rule& rule : _rules) {
            rule.IsLooped = false;
        }
    }

public:
    FormalLanguage(const std::vector<Rule>& rules, int count = 10000) : _rules(rules), MaxRepetitionsCount(count) {}

    std::string OutputLeft() {
        std::string result = "S";
        int count = 0;
        while (count < MaxRepetitionsCount) {
            size_t pos = std::string::npos;

            for (const Rule& rule : _rules) {
                size_t findPos = result.find(rule.Key);
                if ((pos > findPos || pos == std::string::npos) && findPos != std::string::npos) {
                    pos = findPos;
                    break;
                }
            }

            if (pos == std::string::npos) {
                break;
            }

            std::vector<Rule> rules;
            for (const Rule& rule : _rules) {
                if (pos == result.find(rule.Key)) {
                    rules.push_back(rule);
                }
            }

            int index = rand() % rules.size();
            Rule r = rules[index];

            size_t p = result.find(r.Key);
            result.replace(p, r.Key.length(), r.Value);

            count++;
        }

        return result;
    }

    std::string Translate(std::string text) {
        int count = 0;
        bool isEnd = false;
        while (count < MaxRepetitionsCount) {
            if (isEnd) break;

            count++;
            isEnd = true;

            for (Rule& rule : _rules) {
                if (!rule.IsLooped) {
                    size_t pos = text.find(rule.Key);

                    if (pos != std::string::npos) {
                        if (CheckLoop(text, rule)) {
                            rule.IsLooped = true;
                        }
                        else {
                            text.replace(pos, rule.Key.length(), rule.Value);
                            isEnd = false;
                            break;
                        }
                    }
                }
                else {
                    rule.IsLooped = !rule.IsLooped;
                }
            }
        }

        RefreshRules();
        return text;
    }
};

class Grammar {
public:
    std::vector<std::string> Nonterminal;   // Множество нетерминальных символов
    std::vector<std::string> Terminal;      // Множество терминальных символов
    std::vector<Rule> P;                    // Множество правил (продукций) грамматики
    std::string S;                          // Целевой (начальный) символ грамматики

    Grammar(const std::vector<std::string>& vn, const std::vector<std::string>& vt, const std::vector<Rule>& rules, const std::string& s = "S")
        : Nonterminal(vn), Terminal(vt), P(rules), S(s) {}

    std::string GetTypeGrammar() {
        bool isTypeOne = true;
        bool isTypeTwo = true;
        bool isTypeThree = true;

        bool isEachTermPosBigger = true;
        bool isEachTermPosSmaller = true;
        for (const Rule& r : P) {
            isTypeOne &= r.Key.length() <= r.Value.length();

            for (const std::string& vt : Terminal) {
                isTypeTwo &= r.Key.find(vt) == std::string::npos;
            }

            if (isEachTermPosBigger || isEachTermPosSmaller) {
                std::vector<int> terminlPositions;
                std::vector<int> nonTerminlPositions;
                for (const std::string& vn : Nonterminal) {
                    int temp = r.Value.find(vn);
                    if (temp != -1) {
                        nonTerminlPositions.push_back(temp);
                    }
                }
                for (const std::string& vt : Terminal) {
                    int temp = r.Value.find(vt);
                    if (temp != -1) {
                        terminlPositions.push_back(temp);
                    }
                }
                for (int pos : terminlPositions) {
                    for (int posNonTerm : nonTerminlPositions) {
                        isEachTermPosBigger &= pos > posNonTerm;
                        isEachTermPosSmaller &= pos < posNonTerm;
                    }
                }
                if (!(isEachTermPosBigger || isEachTermPosSmaller)) {
                    isTypeThree = false;
                }
            }
        }

        std::cout << "Belongs to Chomsky types:";
        std::string res = "0";
        if (isTypeOne) res += " 1";
        if (isTypeTwo) res += " 2";
        if (isTypeThree) res += " 3";
        return res;
    }

    std::string MakeTree(std::string text) {
        int maxCount = 10000;
        int count = 0;
        std::vector<std::string> tree;
        tree.push_back(text);

        while (count < maxCount) {
            for (const Rule& rule : P) {
                size_t pos = text.rfind(rule.Value);

                if (pos != std::string::npos) {
                    text.replace(pos, rule.Value.length(), rule.Key);

                    std::string separator = "|";
                    for (size_t i = 0; i < pos; i++) {
                        separator = " " + separator;
                    }

                    tree.push_back(separator);
                    tree.push_back(text);
                }
            }
            count++;
        }

        std::reverse(tree.begin(), tree.end());

        for (const std::string& branch : tree) {
            std::cout << branch << std::endl;
        }

        return text;
    }
};


#include <ctime>  // Для использования time в srand

int main() {
    //задание 1
    std::cout << "№1" << std::endl;
    // Создаем правила для формального языка (1-й вариант)
    std::vector<Rule> rules1 = {
        {"S", "T", true},
        {"S", "T+S", true},
        {"S", "T-S", true},
        {"T", "F", true},
        {"T", "F*T", true},
        {"F", "a", true},
        {"F", "b", true},
    };

    // Печатаем правила (1-й вариант)
    PrintRules(rules1);

    // Создаем объект формального языка (1-й вариант)
    FormalLanguage formalLanguage1(rules1);

    // Создаем грамматику (1-й вариант)
    std::vector<std::string> nonterminals1 = { "S", "A", "B" };
    std::vector<std::string> terminals1 = { "a", "b", "+", "-", "*" };
    Grammar grammar1(nonterminals1, terminals1, rules1);

    // Задаем сид для генератора случайных чисел (1-й вариант)
    srand(time(0));

    // Целевая цепочка для 1-го варианта
    std::string targetChain1 = "a-b*a+b";

    // Генерируем и трансформируем цепочку в цикле (1-й вариант)
    std::string generatedChain1;
    do {
        // Генерируем случайную цепочку
        generatedChain1 = formalLanguage1.OutputLeft();
        //std::cout << "Generated chain:  " << generatedChain1 << std::endl;

        // Трансформируем цепочку на формальный язык
        std::string transformedChain1 = formalLanguage1.Translate(generatedChain1);
        //std::cout << "Transformed chain: " << transformedChain1 << std::endl;

        // Сравниваем с целевой цепочкой
        if (transformedChain1 == targetChain1) {
            //std::cout << "Target chain achieved: " << transformedChain1 << std::endl;
            break; // Выход из цикла при достижении целевой цепочки
        }

    } while (true);

    // Строим дерево вывода (1-й вариант)
    std::cout << "Parse tree (1st variant):" << std::endl;
    std::string treeResult1 = grammar1.MakeTree(generatedChain1);

    // Создаем правила для формального языка (2-й вариант)
    std::vector<Rule> rules2 = {
        {"S", "aSBC", true},
        {"S", "abC", true},
        {"CB", "BC", true},
        {"bB", "bb", true},
        {"bC", "bc", true},
        {"cC", "cc", true},
    };

    // Печатаем правила (2-й вариант)
    PrintRules(rules2);

    // Создаем объект формального языка (2-й вариант)
    FormalLanguage formalLanguage2(rules2);

    // Создаем грамматику (2-й вариант)
    std::vector<std::string> nonterminals2 = { "S", "B", "C" };
    std::vector<std::string> terminals2 = { "a", "b", "c" };
    Grammar grammar2(nonterminals2, terminals2, rules2);

    // Задаем сид для генератора случайных чисел (2-й вариант)
    srand(time(0));

    // Целевая цепочка для 2-го варианта
    std::string targetChain2 = "aaabbbccc";

    // Генерируем и трансформируем цепочку в цикле (2-й вариант)
    std::string generatedChain2;
    do {
        // Генерируем случайную цепочку
        generatedChain2 = formalLanguage2.OutputLeft();
        //std::cout << "Generated chain:  " << generatedChain2 << std::endl;

        // Трансформируем цепочку на формальный язык
        std::string transformedChain2 = formalLanguage2.Translate(generatedChain2);
        //std::cout << "Transformed chain: " << transformedChain2 << std::endl;

        // Сравниваем с целевой цепочкой
        if (transformedChain2 == targetChain2) {
            //std::cout << "Target chain achieved: " << transformedChain2 << std::endl;
            break; // Выход из цикла при достижении целевой цепочки
        }

    } while (true);

    // Строим дерево вывода (2-й вариант)
    std::cout << "Parse tree (2nd variant):" << std::endl;
    std::string treeResult2 = grammar2.MakeTree(generatedChain2);

    //задания 2
    
    // Подпункт a)
    std::cout << "№2:" << std::endl;
    std::cout << "a):" << std::endl;

    std::vector<Rule> rulesA = {
        {"S", "aaCFD", true},
        {"AD", "D", true},
        {"F", "AFB", true},
        {"F", "AB", true},
        {"Cb", "bC", true},
        {"AB", "bBA", true},
        {"CB", "C", true},
        {"Ab", "bA", true},
        {"bCD", "\u03B5"},  // epsilon
    };

    PrintRules(rulesA);

    FormalLanguage flA(rulesA);

    std::cout << "Chain: " << flA.Translate("S") << std::endl;
    std::cout << "Language: L = { a^n | n > 0 }" << std::endl;
    std::cout << std::endl;

    // Подпункт б)
    std::cout << "b):" << std::endl;

    std::vector<Rule> rulesB = {
        {"S", "A\u00A7"},   // \u27C2 - значок перпендикуляра
        {"S", "B\u00A7"},
        {"A", "a", true},
        {"A", "Ba", true},
        {"b", "b", true},
        {"b", "Bb", true},
        {"b", "Ab", true},
    };

    PrintRules(rulesB);

    FormalLanguage flB(rulesB);

    std::cout << "Chain: " << flB.Translate("S") << std::endl;
    std::cout << "Language: L = { (a\u00A7)^n | n > 0 }" << std::endl;


    std::cout << "№3" << std::endl;
    // Подпункт a)
    std::cout << "a):" << std::endl;

    std::vector<Rule> rulesA1 = {
        {"S", "aaB", true},
        {"B", "bCCCC", true},
        {"B", "b", true},
        {"C", "Cc", true},
        {"C", "c", true},
    };

    PrintRules(rulesA1);

    FormalLanguage flA1(rulesA);

    std::cout << "Chain: " << flA1.Translate("S") << std::endl;
    std::cout << std::endl;

    // Подпункт б)
    std::cout << "b):" << std::endl;

    std::vector<Rule> rulesB1 = {
        {"S", "0AB", true},
        {"A", "000", true},
        {"B", "1010", true},
    };

    PrintRules(rulesB1);

    FormalLanguage flB1(rulesB1);

    std::cout << "Chain: " << flB1.Translate("S") << std::endl;
    std::cout << std::endl;

    // Подпункт в)
    std::cout << "C):" << std::endl;

    std::vector<Rule> rulesC = {
        {"S", "AB", true},
        {"A", "1001010", true},
        {"B", "0101001", true},
    };

    PrintRules(rulesC);

    FormalLanguage flC(rulesC);

    std::cout << "Chain: " << flC.Translate("S") << std::endl;
    std::cout << std::endl;
    return 0;
       
}











