#include "../Lib/Rule.h"
#include "../Lib/Minimization.h"

#include <iostream>

int main()
{
    setlocale(LC_ALL, "RUS");

    // Знак перпендикулляра = +
    list<Rule> dict1 =
    {
         Rule("S", "aD"),
         Rule("D", "aG"),
         Rule("D", "1E"),
         Rule("G", "cE"),
         Rule("G", "bN"),
         Rule("N", "bG"),
         Rule("N", "cE"),
         Rule("E", "0C"),
         Rule("C", "dE"),
         Rule("S", "dF"),
         Rule("F", "aE"),
         Rule("F", "AN"),
         Rule("C", "1S"),
         Rule("B", "aS"),
         Rule("B", "bC"),
         Rule("A", "0S"),
         Rule("A", "1C"),
         
    };

    PrintRules(dict1);

    // Создаем формальный язык
    FormalLanguage language(dict1);

    // Выводим формальный язык до устранения недостижимых состояний
    std::cout << "Before elimination:" << std::endl;
    std::cout << language.OutputLeft() << std::endl;

    // Устраняем недостижимые состояния
    language.RemoveUnreachableStates();

    // Выводим формальный язык после устранения недостижимых состояний
    std::cout << "After elimination:" << std::endl;
    std::cout << language.OutputLeft() << std::endl;

    return 0;
}