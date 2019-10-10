#include <list>
#include <forward_list>
#include <iostream>
using namespace std;
void list_del(list<int> &lst){
    auto iter=lst.begin();
    while(iter!=lst.end()){
        if(*iter%2){
            lst.insert(iter,*iter);
            ++iter;
        }
        else
            iter=lst.erase(iter);
    }
}
void forward_list_del(forward_list<int> &lst){
    auto pre=lst.before_begin();
    auto cur=lst.begin();
    while(cur!=lst.end()){
        if(*cur%2){
            /*注意以下错误代码;
             pre=lst.insert_after(pre,*cur);
             ++cur;
            */
            /*
             *此代码问题在于
             *pre=lst.insert_after(pre,*cur);
             *使得insert_after操作后,pre并非cur的直接前驱迭代器.
            */
            pre=lst.insert_after(pre,*cur);
            ++cur;
        }
        else
            cur=lst.erase_after(pre);
    }
}


int main(){
    list<int> lst1{0,1,2,3,4,5,6,7,8,9};
    list_del(lst1);
    for(auto &x:lst1)
        cout<<x<<" ";
    cout<<endl;
    forward_list<int> lst2{0,1,2,3,4,5,6,7,8,9};
    forward_list_del(lst2);
    for(auto &x:lst2)
        cout<<x<<" ";
    cout<<endl;
    return 0;
}
