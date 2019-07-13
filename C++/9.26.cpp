#include <iostream>
#include <vector>
#include <list>
using namespace std;
int main(){
    int ia[]={0,1,1,2,3,5,8,13,21,55,89};
    vector<int> vec(cbegin(ia),cend(ia));
    list<int> lst(cbegin(ia),cend(ia));
    for(auto &a:vec)
        cout<<a<<" ";
    cout<<endl;
    for(auto &b:lst)
        cout<<b<<" ";
    cout<<endl;

    auto vec_iter=vec.begin();
    auto lst_iter=lst.begin();
    /*典型错误
     *当while里面的判断语句执行后,vec_iter已经向后递增了一位;
     *导致程序没有处理vec的第一个元素.
    while(vec_iter++!=vec.end()){
        if(*vec_iter%2)
            continue;
        else
            vec.erase(vec_iter);
    }
    */

    //除此之外,还要注意循环中必须要有推进迭代器前进的语句.
    //正确做法:
    while(vec_iter!=vec.end()){
        if(*vec_iter%2){
            ++vec_iter;
            continue;
        }
        else
            vec_iter=vec.erase(vec_iter);
    }
    /*以下代码错误:
     *
     *while(lst_iter!=lst.end()){
        if(*lst_iter%2){         //原因在于:
            lst.erase(lst_iter); //执行erase操作后,lst_iter指向被删除了的元素,这个迭代器已经失效,不能再使用.
            ++lst_iter;
        }
        else
            ++lst_iter;
     }
     */
    while(lst_iter!=lst.end()){
        if(*lst_iter%2)
            lst_iter=lst.erase(lst_iter);
        else
            ++lst_iter;

    }
    for(auto &a:vec)
        cout<<a<<" ";
    cout<<endl;
    for(auto &b:lst)
        cout<<b<<" ";
    cout<<endl;
    return 0;
}
