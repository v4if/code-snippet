#include <bits/stdc++.h>
using namespace std;

void printArr(vector<int>& arr){
    for (int a : arr) cout << a << " ";
    cout << "\n";
}

bool cmp(int& l, int& r) {return l < r;}

struct Cmp{
    bool operator() (int& l, int& r) {return l < r;}
};

int main()
{
    vector<int> arr{1,6,3,9,5};
    sort(arr.begin(), arr.end(), less<int>{});
    printArr(arr);
    sort(arr.begin(), arr.end(), greater<int>{});
    printArr(arr);
    
    sort(arr.begin(), arr.end(), cmp);
    sort(arr.begin(), arr.end(), Cmp{});
}


