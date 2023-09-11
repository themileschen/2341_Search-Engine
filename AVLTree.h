// Class Owner: Miles

// Adapted from: https://github.com/mhahsler/CS2341/blob/main/Chapter4_Trees/AVLTree/AvlTree.h

#ifndef AVL_TREE_H
#define AVL_TREE_H

#define DEBUG

#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <queue>
#include <vector> 
#include <string> 
#include <cstdlib> 

#include "MapValue.h" 

using namespace std;

template <typename Object, typename keyType, typename valueType>
class AVLTree
{
    private:
        struct AVLNode
        {
            AVLNode* left;
            AVLNode* right;
            int height;

            keyType key;
            valueType values;

            AVLNode(const Object& newNode, AVLNode* lt, AVLNode* rt, int h = 0)
            {
                key = newNode.getKey();
                values = newNode.getValues();
                left = lt;
                right = rt;
                height = h;
            }
        };

        AVLNode* root;
        size_t size = 0;

    public:
        string result;

        AVLTree() 
        {
            root = nullptr;
        }

        AVLTree(const AVLTree &rhs) 
        {
            root = nullptr;
            root = clone(rhs.root); // internal function clone()
        }

        ~AVLTree()
        {
            makeEmpty();
        }

        AVLTree& operator=(const AVLTree& rhs)
        {
            makeEmpty();
            root = clone(rhs.root);
            return *this;
        }

        bool contains(const Object& x) const // if the node is exists in the tree
        {
            return contains(x, root);
        }

        bool containsKey(const string& s) const // if the key exists in the tree 
        {
            return containsKey(s, root);
        }

        bool isEmpty() const // if tree is empty 
        {
            return root == nullptr;
        }

        void printTreeSort(ostream& out = cout) const // prints tree in sorted order
        {
            printTreeSort(root, out);
        }

        string getSortedTree() // used for CATCH2 tests 
        {
            result = "";
            return getSortedTree(root);
        }

        void printTreeWithValues(ostream& out = cout) const // LNR (inorder) traversal 
        {
            printTreeWithValues(root, out);
        }

        void printTreeByLevel(ostream& out = cout) const // level-order traversal 
        {
            printTreeByLevel(root, out);
        }

        string getTreeByLevel() // used for CATCH2 tests 
        {
            result = "";
            return getTreeByLevel(root);
        }

        void prettyPrintTree() const // print the tree structure 
        {
            prettyPrintTree("", root, false);
        }

        string getPrettyTree() // used for CATCH2 tests 
        {
            result = "";
            return getPrettyTree("", root, false);
        }

        void printTreeForPersistence(ostream& out = cout) {
            printTreeForPersistence(root, out);
        }

        void makeEmpty()
        {
            makeEmpty(root);
        }

        void insert(const Object& x)
        {
            insert(x, root);
        }
            
        void remove(const Object& x)
        {
            remove(x, root);
        }

        valueType& returnValues(const string s) const // return the values for the given key 
        {
            return returnValues(s, root);
        }

        size_t getFreq(string s, string id) // when the value is an unordered_map: returns the value at key id for key s
        {
            return getFreq(s, id, root);
        }

        void insertAndPrint(const Object&x) // for testing only
        {
            insertAndPrint(x, root);
        }

        AVLNode* findMin()
        {
            return findMin(root);
        }

        int height()
        {
            return height(root);
        }

        void add_value_vector(string word, string value) // when the value is a vector: the word already exists in the key, so just add another doc (value) to the vector it maps to        
        {
            AVLNode* key = findKey(word, root);
            key->values.push_back(value);
        }        

        void add_value_map(string word, string value, size_t frequency) // when the value is an unordered_map: update the node 
        {
            AVLNode* key = findKey(word, root);
            add_value_map(value, frequency, key);
        }

        int max(int lhs, int rhs) const
        {
            if(lhs > rhs)
            {
                return lhs;
            }
            else
            {
                return rhs;
            }
        }

        AVLNode* findKey(const string s) const // returns the node for the given key 
        {
            return findKey(s, root);
        }

        bool isBalanced()
        {
            if(!root)
            {
                throw runtime_error("tree is empty!");
            }
            return (abs(height(root->left) - height(root->right)) <= 1);
        }

        size_t getSize()
        {
            return size;
        }
    
    private:

        void insert(const Object& x, AVLNode*& t)
        {
            string theKey = x.getKey();

            if(t == nullptr)
            {
                t = new AVLNode(x, nullptr, nullptr, 0);
                size++;
                return; // single node, always balanced
            }

            if(theKey < t->key) // go down left subtree
            {
                insert(x, t->left);
            }
            else if(t->key < theKey) // go down right subtree 
            {
                insert(x, t->right);
            }
            balance(t);
        }

        void insertAndPrint(const Object&x, AVLNode*& t) // for testing only
        {
            string theKey = x.getKey();

            if(t == nullptr)
            {
                t = new AVLNode(x, nullptr, nullptr, 0);
                return; // single node, always balanced
            }

            if(theKey < t->key)
            {
                insert(x, t->left);
            }
            else if(t->key < theKey)
            {
                insert(x, t->right);
            }

            balance(t);

            vector<string> temp = x.getValues();
            for(int i = 0; i < temp.size(); i++)
            {
                cout << temp.at(i) << " ";
            }
            cout << endl;
        }

        void add_value_map(string value, size_t frequency, AVLNode* node)
        {
            if(node->values.find(value) == node->values.end()) // doc has not been added yet 
            {
                node->values.insert({value, frequency}); // add a new key-value pair 
            }
            else // doc already exists for this word
            {
                node->values.at(value) += frequency; // update the frequency 
            }
        }

        void remove(const Object& x, AVLNode*& t)
        {
            if(t == nullptr)
                return; // Item not found

            string theKey = x.getKey();

            if(theKey < t->key) // in left subtree
            {
                return remove(x, t->left);
            }
            else if(t->key < theKey) // in right subtree
            {
                return remove(x, t->right);
            }

            if(t->left == nullptr && t->right == nullptr) // case A: no children 
            {
                delete t;
                t = nullptr;
                size--;
                return;
            }
            else if(t->left != nullptr && t->right != nullptr) // case C: two children
            {
                AVLNode* replacement = removeMin(t->right);
                replacement->right = t->right;
                replacement->left = t->left;

                delete t;
                t = replacement;
                size--;
                return;
            }
            else // case B: one child
            {
                AVLNode* oldNode = t;
                if(t->left != nullptr)
                {
                    t = t->left;
                }
                else
                {
                    t = t->right;
                }
                delete oldNode;
                size--;
            }

            balance(t);
        }

        AVLNode* removeMin(AVLNode*& t)
        {
            if(t == nullptr) // no root node
            {
                return nullptr;
            }

            if(t->left == nullptr) // root is the minimum
            {
                AVLNode* min = t;
                t = t->right;
                return min;
            }

            if(t->left->left == nullptr) // traverse down left to the leaf
            {
                AVLNode* min = t->left;
                t->left = min->right;
                return min;
            }

            return removeMin(t->left);
        }

        AVLNode* findMin(AVLNode* t) const
        {
            if(t == nullptr)
            {
                return nullptr;
            }

            if(t->left == nullptr)
            {
                return t;
            }

            return findMin(t->left);
        }

        bool contains(const Object& x, AVLNode* t) const
        {
            string theKey = x.getKey();
            if(t == nullptr)
            {
                return false;
            }
            else if(theKey < t->key)
            {
                return contains(x, t->left);
            }
            else if(t->key < theKey)
            {
                return contains(x, t->right);
            }
            else
            {
                return true;
            }
        }

        bool containsKey(const string& s, AVLNode* t) const
        {
            if(t == nullptr)
            {
                return false;
            }
            else if(s < t->key)
            {
                return containsKey(s, t->left);
            }
            else if(t->key < s)
            {
                return containsKey(s, t->right);
            }
            else
            {
                return true;
            }
        }

        AVLNode* findKey(const string s, AVLNode* t) const
        {
            if(t == nullptr)
            {
                throw runtime_error("Key " + s + " was not found!");
            }
            else if(s < t->key)
            {
                return findKey(s, t->left);
            }
            else if(t->key < s)
            {
                return findKey(s, t->right);
            }
            else
            {
                return t;
            }
        }

        valueType& returnValues(const string s, AVLNode* t) const
        {
            AVLNode* key = findKey(s, t);
            return key->values;
        }

        size_t getFreq(string s, string id, AVLNode* t)
        {
            AVLNode* temp = findKey(s, t);
            return temp->values.at(id);
        }

        void makeEmpty(AVLNode*& t)
        {
            if(t == nullptr)
            {
                return;
            }

            makeEmpty(t->left);
            makeEmpty(t->right);
            delete t;
            t = nullptr;
        }

        AVLNode* clone(AVLNode* t) const
        {
            if(t == nullptr)
            {
                return nullptr;
            }

            MapValue<keyType, valueType> newValue = MapValue(t->key, t->values);

            return new AVLNode(newValue, clone(t->left), clone(t->right), t->height); 
        }

        void printTreeSort(AVLNode* t, ostream& out) const // print subtree with root t in sorted order using inorder (LNR) traversal
        {
            if(t == nullptr)
            {
                return;
            }

            printTreeSort(t->left, out);
            out << t->key << " ";
            printTreeSort(t->right, out);
        }

        string getSortedTree(AVLNode* t) // returns the tree as a string 
        {
            if(t == nullptr)
            {
                return result;
            }

            getSortedTree(t->left);
            result += t->key;
            result = result + " ";
            getSortedTree(t->right);
            return result;
        }

        void printTreeWithValues(AVLNode* t, ostream& out) const // prints the key-value pairs 
        {
            if(t == nullptr)
            {
                return;
            }

            printTreeWithValues(t->left, out);

            out << t->key << ":" << endl;
            for(auto const& pair: t->values) 
            {
                out << "    {" << pair.first << ": " << pair.second << "}" << endl; 
            }

            printTreeWithValues(t->right, out);
        }

        void printTreeByLevel(AVLNode* t, ostream&out) const // level-order traversal
        {
            if(t == nullptr)
            {
                return;
            }

            AVLNode* current;
            queue<AVLNode*> q;
            q.push(t); // start with root node

            while(!q.empty())
            {
                current = q.front();
                q.pop();
                out << current->key << " ";

                if(current->left != nullptr)
                {
                    q.push(current->left);
                }

                if(current->right != nullptr)
                {
                    q.push(current->right);
                }
            }
        }

        string getTreeByLevel(AVLNode* t)
        {
            if(t == nullptr)
            {
                return result;
            }

            AVLNode* current;
            queue<AVLNode*> q;
            q.push(t); // start with root node

            while(!q.empty())
            {
                current = q.front();
                q.pop();
                result += current->key;
                result += " ";

                if(current->left != nullptr)
                {
                    q.push(current->left);
                }

                if(current->right != nullptr)
                {
                    q.push(current->right);
                }
            }
            return result;
        }

        void prettyPrintTree(const string& prefix, const AVLNode* node, bool isRight) const
        {
            if(node == nullptr)
            {
                return;
            }

            cout << prefix;
            cout << (isRight ? "├──" : "└──");
            cout << node->key << endl;

            prettyPrintTree(prefix + (isRight ? "│   " : "    "), node->right, true);
            prettyPrintTree(prefix + (isRight ? "│   " : "    "), node->left, false);
        }

        string getPrettyTree(const string& prefix, const AVLNode* node, bool isRight)
        {
            if(node == nullptr)
            {
                return result;
            }

            result += prefix;;
            if(isRight)
            {
                result += "├──";
            }
            else
            {
                result += "└──";
            }
            result += node->key;
            result += "\n";

            getPrettyTree(prefix + (isRight ? "│   " : "    "), node->right, true);
            getPrettyTree(prefix + (isRight ? "│   " : "    "), node->left, false);
            return result;
        }
        void printTreeForPersistence(AVLNode* t, ostream& out) {
            if(t == nullptr)
            {
                return;
            }

            printTreeForPersistence(t->left, out);

            out << t->key << "\n";
	
            for(auto const& pair: t->values) 
            {
                out << pair.first << "\n" << pair.second << "\n";
            }
            out << "}\n";

            printTreeForPersistence(t->right, out);

        }

        int height(AVLNode* t) const
        {
            if(t == nullptr)
            {
                return -1;
            }
            else
            {
                return t->height;
            }
        }

        static const int ALLOWED_IMBALANCE = 1;

        void balance(AVLNode*& t)
        {
            if(t == nullptr)
            {
                return;
            }

            if(height(t->left) - height(t->right) > ALLOWED_IMBALANCE)
            {
                if(height(t->left->left) >= height(t->left->right))
                {
                    rotateWithLeftChild(t); // outside case
                }
                else
                {
                    doubleWithLeftChild(t); // inside case
                }
            }
            else if(height(t->right) - height(t->left) > ALLOWED_IMBALANCE)
            {
                if(height(t->right->right) >= height(t->right->left))
                {
                    rotateWithRightChild(t); // outside case
                }
                else
                {
                    doubleWithRightChild(t); // inside case
                }
            }

            t->height = max(height(t->left), height(t->right)) + 1;
        }

        void rotateWithLeftChild(AVLNode*& k2)
        {
                    // #ifdef DEBUG
                    // cout << "Need to rotateWithLeftChild for node " << k2->key << endl;
                    // cout << "Before:" << endl;
                    // prettyPrintTree();
                    // #endif

            AVLNode* k1 = k2->left;
            k2->left = k1->right;
            k1->right = k2;
            k2->height = max(height(k2->left), height(k2->right)) + 1;
            k1->height = max(height(k1->left), k2->height) + 1;
            k2 = k1;

                    // #ifdef DEBUG
                    // cout << "After:" << endl;
                    // prettyPrintTree();
                    // #endif
        }

        void rotateWithRightChild(AVLNode*& k1)
        {
                    // #ifdef DEBUG
                    // cout << "Need to rotateWithRightChild for node " << k1->key << endl;
                    // cout << "Before:" << endl;
                    // prettyPrintTree();
                    // #endif

            AVLNode* k2 = k1->right;
            k1->right = k2->left;
            k2->left = k1;
            k1->height = max(height(k1->left), height(k1->right)) + 1;
            k2->height = max(height(k2->right), k1->height) + 1;
            k1 = k2;

                    // #ifdef DEBUG
                    // cout << "After:" << endl;
                    // prettyPrintTree();
                    // #endif
        }

        void doubleWithLeftChild(AVLNode*& k3)
        {
                    // #ifdef DEBUG
                    // cout << "doubleWithLeftChild" << endl;
                    // #endif

            rotateWithRightChild(k3->left);
            rotateWithLeftChild(k3);
        }

        void doubleWithRightChild(AVLNode*& k1)
        {
                    // #ifdef DEBUG
                    // cout << "doubleWithRightChild" << endl;
                    // #endif 

            rotateWithLeftChild(k1->right);
            rotateWithRightChild(k1);
        }
};

#endif 
