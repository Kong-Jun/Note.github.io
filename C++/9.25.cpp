#include <iostream>
#include <list>
#include <vector>
using namespace std;
int main(){
    int ia[]={0,1,1,2,3,5,8,13,21,55,89};
    vector<int> vec(begin(ia),end(ia));
    list<int> lst(begin(ia),end(ia));
    vector<int>::iterator iter1=vec.begin();
    list<int>::iterator iter2=lst.begin();
    while(iter1++!=vec.end())
        if((*iter1+1)%2) vec.erase(iter1);
    while(iter2++!=lst.end())
        if(*iter2%2)
            lst.erase(iter2);
    for(auto &a:vec)
        cout<<a<<" ";
    cout<<endl;
    for(auto &b:lst)
        cout<<b<<" ";
    cout<<endl;
    return 0;
}
