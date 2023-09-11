// FOR INFORMAL TESTING PURPOSES ONLY 

#include "AVLTree.h"
#include "MapValue.h"

int main()
{
    AVLTree<MapValue> testTree;
    string a = "Finance";
    vector<string> v = {"id100", "id101"};
    MapValue one = MapValue(a, v);

    vector<string> v2 = {"id102", "id103", "id100"};
    MapValue two = MapValue("Business", v2);
    MapValue three = MapValue("Accounting", v2);
    MapValue four = MapValue("Real Estate", v);
    MapValue five = MapValue("Marketing", v);
    testTree.insert(one);
    testTree.insert(two);
    testTree.insert(three);
    testTree.insert(four);
    testTree.insert(five);

    AVLTree<MapValue> test2 = testTree;
    AVLTree<MapValue> test3;
    MapValue newNode = MapValue("Economics", v);
    test2.insert(newNode);
    test3 = testTree;
    test3.insert(newNode);

    cout << testTree.contains(one) << endl;
    cout << testTree.isEmpty() << endl;
    test3.remove(newNode);
   // test2.removeMin();

    cout << testTree.findKey("Marketing")->key << endl;
    
    testTree.prettyPrintTree();

    vector<string> results = testTree.returnValues("Business");
    for(auto& a : results)
        cout << a << " ";
    cout << endl;

    cout << testTree.getSortedTree() << endl;
    cout << testTree.getTreeByLevel() << endl;
    cout << testTree.getPrettyTree() << endl;
    cout << testTree.isBalanced() << endl;


    
    return 0;
}