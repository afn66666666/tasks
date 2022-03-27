//Given a list and a number, create a new list that contains each number
//of lst at most N times without reordering. For example if the input number
//is 2, and the input list is [1,2,3,1,2,1,2,3], you take [1,2,3,1,2], drop
//the next [1,2] since this would lead to 1 and 2 being in the result 3 times,
//and then take 3, which leads to [1,2,3,1,2,3]. With list [20,37,20,21] and number 1, the result would be [20,37,21].

#include <iostream>
#include <vector>
#include <unordered_map>

std::vector<int> deleteNth(std::vector<int> arr, int n);

int main()
{
    std::vector<int> result{ 20,37,20,21 };
    result = deleteNth(result, 1);
    result = { 1,1,3,3,7,2,2,2,2 };
    result = deleteNth(result, 3);
    
}

std::vector<int> deleteNth(std::vector<int> arr, int n)
{
    std::unordered_map<int, int> countsOfNum;
    std::unordered_map<int, int>::iterator it;
    std::vector<int>result;
    for (int i = 0; i < arr.size(); i++) {
        it = countsOfNum.find(arr[i]);
        if (it != countsOfNum.end()) {
            if (it->second < n) {
                it->second += 1;
                result.push_back(it->first);
            }
        }
        else {
            countsOfNum.insert(std::make_pair(arr[i], 1));
            result.push_back(arr[i]);
        }
    }

    return result;

}