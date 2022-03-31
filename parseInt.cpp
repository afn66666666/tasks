#include <string>
#include <iostream>
#include <vector>



/*In this kata we want to convert a string into an integer. The strings simply represent the numbers in words.

Examples:

"one" => 1
"twenty" => 20
"two hundred forty-six" => 246
"seven hundred eighty-three thousand nine hundred and nineteen" => 783919
Additional Notes:

The minimum number is "zero" (inclusively)
The maximum number, which must be supported is 1 million (inclusively)
The "and" in e.g. "one hundred and twenty-four" is optional, in some cases it's present and in others it's not
All tested numbers are valid, you don't need to validate them*/




long parse_int(std::string number);
int interpretWord(const std::string& str);
long makeCounts(std::vector<int>& ranks);
int main(){
    std::cout << parse_int("two hundred forty-six") << std::endl;

}

long parse_int(std::string number) {
    int i = 0;
    std::string rank = "";
    long result = 0;
    std::vector<int> ranks;
    std::vector<std::string> words;
    while (i != number.size()) {
        while (number[i] != ' ' and number[i]!='-' and number[i]!='\0') {
            rank += number[i];
            i++;
        }
        if (rank == "and") {
            rank.clear();
            i++;
            continue;
        }
        ranks.push_back(interpretWord(rank));
        rank.clear();
        if (i != number.size()) {
            i++;
        }
    }

    if (ranks.size() == 1) {
        return ranks[0];
    }
    else {
        result = makeCounts(ranks);
    }

    return result;
}


int interpretWord(const std::string& str) {
    if (str == "zero") {
        return 0;
    }
    if(str == "one"){ return 1; }
    if(str == "two"){ return 2; }
    if(str == "three"){ return 3; }
    if(str == "four"){ return 4; }
    if(str == "five"){ return 5; }
    if(str == "six"){ return 6; }
    if(str == "seven"){ return 7; }
    if(str == "eight"){ return 8; }
    if(str == "nine"){ return 9; }
    if (str == "ten") { return 10; }
    if(str == "eleven"){ return 11; }
    if(str == "twelve"){ return 12; }
    if(str == "thirteen"){ return 13; }
    if(str == "fourteen"){ return 14; }
    if (str == "fifteen") { return 15; }
    if(str == "sixteen"){ return 16; }
    if(str == "seventeen"){ return 17; }
    if(str == "eighteen"){ return 18; }
    if(str == "nineteen"){ return 19; }
    if(str == "twenty"){ return 20; }
    if(str == "thirty"){ return 30; }
    if(str == "forty"){ return 40; }
    if(str == "fifty"){ return 50; }
    if(str == "sixty"){ return 60; }
    if(str == "seventy"){ return 70; }
    if(str == "eighty"){ return 80; }
    if(str == "ninety"){ return 90; }
    if(str == "hundred"){ return 100; }
    if(str == "thousand"){ return 1000; }
    if(str == "million"){ return 1000000;}
}

long makeCounts(std::vector<int>& ranks)
{
    long result = 0;
    auto it = ranks.begin();
    long prev = 0;
    for (int i = 0; i < ranks.size() - 1;) {
        if (i+2 < ranks.size() and ranks[i+2] == 100) {
            ranks[i+1] *= ranks[i+2];
            auto spec = it + 2;
            ranks.erase(spec);
        }
        else {
            if (ranks[i] < ranks[i + 1]) {
                ranks[i + 1] *= ranks[i];
                
            }
            else {
                ranks[i + 1] += ranks[i];
            }
            ranks.erase(it);
            it = ranks.begin();
        }
    }
    return *it;
   


   
}
