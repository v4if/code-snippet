#include <bits/stdc++.h>
using namespace std;

/*
lower_bound : 第一个大于等于val的iterator
upper_bound : 最后一个小于等于val的下一个iterator
 */
int main()
{
    vector<int> arr{1,3,5,7,9};
    vector<int>::iterator it = lower_bound(arr.begin(), arr.end(), 4);
    cout << *it << endl;
    cout << distance(arr.begin(), it) << endl;
    
    it = upper_bound(arr.begin(), arr.end(), 3);
    cout << *it << endl;
    
    cout << "Hello World!" << endl;
}
