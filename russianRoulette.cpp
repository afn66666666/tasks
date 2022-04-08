#include <stdarg.h>
#include <iostream>
#include <vector>

using namespace std;

typedef long double ld;

ld Horners_Method(ld x, int n, ...);
vector < int> josephus(std::vector < int > items, int k);

int main()
{
    josephus({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }, 2);
}



vector < int> josephus(std::vector < int > items, int k) {
    int kill = 1;
    vector<int>result;
    vector<int>::iterator it = items.begin();

    while (!items.empty()) {
        if (kill == k) {
            if (it == items.end()) {
                it = items.begin();
            }
            result.push_back(*it);
            it = items.erase(it);
            kill = 1;
        }
        else {
            if (it == items.end()) {
                it = items.begin();
                continue;
            }
            kill++;
            it++;
        }
    }
    return result;

}


