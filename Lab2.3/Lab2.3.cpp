#include <iostream>
#include "../Lib/NFA_to_DFA.h"
#include "../Lib/FormalLanguage.h"
#include "../Lib/Rule.h"

using namespace std;

//СОСТОЯНИЯ  0  1  2   3   4
enum State { H, S, AS, BS, ER};// ER состояние ошибки

void Analizator(const string& text) {
    State now = H;
    int count = 0;
    string res = "";
    bool reachedFinalState = false;//флаг проверки последнего состояния
    do {
        // Проверка выхода за пределы индексации
        if (count < text.length()) {
            switch (now) {
            case H: {
                if (text[count] == '0')
                    now = S;
                else if (text[count] == '1')
                    now = AS;
                else
                    now = ER;
                break;
            }
            case AS: {
                if (text[count] == '0')
                    now = BS;
                else if (text[count] == '1')
                    now = AS;
                else
                    now = ER;
                break;
            }
            case BS: {
                if (text[count] == '1')
                    now = AS;
                else
                    now = ER;
                break;
            }
            default:
                break;
            }

            res += to_string(now) + " ";
            count++;
        }
        else
            break;
    }// while (now != S && text[count] != '\u00A7');
    while (now != ER && text[count] != '\u00A7'); //now != State.S &&
    cout << res << endl;

    //проверяем было ли достугнуто конечное состояние 
  /*  reachedFinalState = (now == S && count == text.length());
    if (reachedFinalState)
        cout << "Цепочка принадлежит данному языку." << std::endl;
    else
        cout << "Цепочка не принадлежит данному языку." << std::endl;*/
}

int main()
{
    setlocale(LC_ALL, "RU");

    cout << "Задание 2.3" << endl;
    list<Rule> dict =
    {
        Rule("S", "A0"),
        Rule("S", "A1"),
        Rule("S", "B1"),
        Rule("S", "0"),
        Rule("S", "1"),
        Rule("A", "A1"),
        Rule("A", "B1"),
        Rule("A", "1"),
        Rule("B", "A0")

    };

    PrintRules(dict);
    FormalLanguage fl(dict);

    //take_input();
    take_input_static();
    print_output();
    create_state_transitions(start_state);
    print_dfa();



    cout << "Анализаторы цепочек " << endl;
    cout << "Состояния: H-0, S-1, AS-2, BS-3, ER-4" << endl;
    cout << "Цепочка 1011    \t:";
    Analizator("1011");
    cout << "Цепочка 0       \t:";
    Analizator("0");
    cout << "Цепочка 101011  \t:";
    Analizator("101011 ");
    cout << endl;

    cout << "G ({0,1}, {H,AS,BS,S},P,H)" << endl;

    return 0;
}
