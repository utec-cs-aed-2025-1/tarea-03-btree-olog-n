#include "btree.h"
#include <iostream>
#include <vector>

using namespace std;

int main(){
    BTree<int> bee(3); 
    cout <<"Paso 1 \n"; 
    bee.insert2(4);
    cout<<"Paso 2 \n";
    bee.insert2(10);
    bee.insert2(11);

    string s = bee.toString(",");
    cout << "Paso 3 \n"; 
    cout << s << "\n";
    return 0;
}