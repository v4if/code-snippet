#include <bits/stdc++.h>
using namespace std;

void printArr(vector<int>& arr){
    for (int a : arr) cout << a << " ";
    cout << "\n";
}

int main()
{
    vector<int> arr{1,6,3,9,5};
    sort(arr.begin(), arr.end(), less<int>{});
    printArr(arr);
    sort(arr.begin(), arr.end(), greater<int>{});
    printArr(arr);
}


