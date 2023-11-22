

#include <iostream>
#include <algorithm> // for max and min

using namespace std;

// Splay tree:
class AVLTree {
public:
    struct Node {
        int key;
        Node* left;
        Node* right;
        Node* root;
        int height;

        Node(int k) : key(k), left(nullptr), right(nullptr), height(1) {}
    };

    Node* root;

    int getHeight(Node* node) {
        return (node) ? node->height : 0;
    }

    int getBalanceFactor(Node* node) {
        return getHeight(node->left) - getHeight(node->right);
    }

    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = 1 + max(getHeight(y->left), getHeight(y->right));
        x->height = 1 + max(getHeight(x->left), getHeight(x->right));

        return x;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = 1 + max(getHeight(x->left), getHeight(x->right));
        y->height = 1 + max(getHeight(y->left), getHeight(y->right));

        return y;
    }

    Node* insert(Node* node, int key) {
        if (!node)
            return new Node(key);

        if (key < node->key)
            node->left = insert(node->left, key);
        else if (key > node->key)
            node->right = insert(node->right, key);
        else
            return node; // Duplicate keys not allowed

        node->height = 1 + max(getHeight(node->left), getHeight(node->right));

        int balance = getBalanceFactor(node);

        // Left Heavy
        if (balance > 1) {
            if (key < node->left->key) {
                return rotateRight(node);
            }
            else {
                node->left = rotateLeft(node->left);
                return rotateRight(node);
            }
        }

        // Right Heavy
        if (balance < -1) {
            if (key > node->right->key) {
                return rotateLeft(node);
            }
            else {
                node->right = rotateRight(node->right);
                return rotateLeft(node);
            }
        }

        return node;
    }

public:
    AVLTree() : root(nullptr) {}

    void insert(int key) {
        root = insert(root, key);
    }

    void inorderTraversal(Node* node) {
        if (node) {
            inorderTraversal(node->left);
            cout << node->key << " ";
            inorderTraversal(node->right);
        }
    }

    void inorder() {
        inorderTraversal(root);
        cout << endl;
    }

    // Add more operations such as: search, delete, findMin, findMax:
//for search we know that we have to start searching from the root. if the root is bigger than the key then go to the right node 
//if the key is smaller then go to the left
    Node* getRoot(){
        return root;
    }
    Node* iterativeSearch(Node* root, int k){
        while(root != NULL && root ->key != k){
            if(root->key>k){
                root=root->left;
            }
            else{
                root=root->right;
            }
        }
        return root;
    }
    //deletion
    Node* deleteNode(Node* root, int k){
        if(root==NULL){
            return root;
        }
        if(k < root->key){
            root->left=deleteNode(root->left,k);
        } 
        else if(k>root->key){
            root->right=deleteNode(root->right,k);
        }
        else{
            if(root->left==NULL){
                Node* temp=root->right;
                delete root;
                cout<<"The key "<<root->key<<" is deleted \n";
                return temp;
            }
            else if(root->right==NULL){
                Node* temp = root->left;
                delete root;
                cout<<" The key "<<root->key<<" is deleted \n";
                return temp;
            }
            root->key=inOrderSuccessor(root->right);
            root->right=deleteNode(root->right,root->key);
        }
        return root;
    }
    int inOrderSuccessor(Node* root){
        int minimum= root->key;
        while(root->left !=NULL){
            minimum=root->left->key;
            root=root->left;
        }
        return minimum;
    }
    //minvalue
    Node* findMin(Node* root){
        if(root==NULL){
            return root;
        }
        Node* current=root;
        while(current->left !=NULL){
            current=current->left;
        }
        return current;
    }
    //maxvalue
    Node* findMax(Node* root){
        if(root==NULL){
            return root;
        }
        Node* current=root;
        while(current->right !=NULL){
            current=current->right;
        }
        return current;
    }
    //ispresent
    bool isPresent(Node* root, int k){
        if(root==NULL){
            return false;
        }
        if(root->key==k){
            return true;
        }
        else if(k<root->key){
            return isPresent(root->left,k);//if the key is smaller search the left side
        }
        else{
            return isPresent(root->right,k);//is the key is bigger then check the right side
        }
    }
    // Destructor to free memory (not optimal)
    ~AVLTree() {
        destroyTree(root);
    }

    void destroyTree(Node* node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }
};


// Splay tree:
class SplayTree {
private:
    struct Node {
        int key;
        Node* left;
        Node* right;
        Node(int k) : key(k), left(nullptr), right(nullptr) {}
    };

    Node* root;

    Node* splay(int key, Node* node) {
        if (!node) return nullptr;

        if (key < node->key) {
            if (!node->left) return node;
            if (key < node->left->key) {
                node->left->left = splay(key, node->left->left);
                node = rotateRight(node);
            }
            else if (key > node->left->key) {
                node->left->right = splay(key, node->left->right);
                if (node->left->right)
                    node->left = rotateLeft(node->left);
            }

            return (node->left) ? rotateRight(node) : node;
        }
        else if (key > node->key) {
            if (!node->right) return node;
            if (key < node->right->key) {
                node->right->left = splay(key, node->right->left);
                if (node->right->left)
                    node->right = rotateRight(node->right);
            }
            else if (key > node->right->key) {
                node->right->right = splay(key, node->right->right);
                node = rotateLeft(node);
            }

            return (node->right) ? rotateLeft(node) : node;
        }
        else {
            return node;
        }
    }

    Node* rotateRight(Node* node) {
        Node* newRoot = node->left;
        node->left = newRoot->right;
        newRoot->right = node;
        return newRoot;
    }

    Node* rotateLeft(Node* node) {
        Node* newRoot = node->right;
        node->right = newRoot->left;
        newRoot->left = node;
        return newRoot;
    }

public:
    SplayTree() : root(nullptr) {}

    void insert(int key) {
        root = splay(key, root);
        if (!root || root->key != key) {
            Node* newNode = new Node(key);
            if (!root) {
                root = newNode;
            }
            else if (key < root->key) {
                newNode->left = root->left;
                newNode->right = root;
                root->left = nullptr;
                root = newNode;
            }
            else {
                newNode->right = root->right;
                newNode->left = root;
                root->right = nullptr;
                root = newNode;
            }
        }
    }

    // Add more operations such as: search, delete, findMin, findMax:

    void inorderTraversal(Node* node) {
        if (node) {
            inorderTraversal(node->left);
            cout << node->key << " ";
            inorderTraversal(node->right);
        }
    }

    void inorder() {
        inorderTraversal(root);
        cout << endl;
    }

    // Destructor to free memory (not optimal)
    ~SplayTree() {
        destroyTree(root);
    }

    void destroyTree(Node* node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }
};


int main() 
{
    // AVL tree and a Splay tree
    AVLTree   avlTree;
    SplayTree splayTree;

    // Insert some values into the AVL tree
    int Keys[] = { 50, 30, 70, 20, 40, 60, 80 };

    for (int key : Keys) {
        avlTree.insert(key);
        splayTree.insert(key);
    }


    // Display AVL tree
    cout << "Inorder traversal of the AVL tree: ";
    avlTree.inorder();
    cout<<"\n";
    // Display Splay tree
    cout << "Inorder traversal of the Splay tree: ";
    splayTree.inorder();
    cout<<"\n";

    int keyToSearch = 70;
    int ispresentKey=40;
    AVLTree::Node* root = avlTree.getRoot();
    AVLTree::Node* searchResult = avlTree.iterativeSearch(root, keyToSearch);
    AVLTree::Node* minNode = avlTree.findMin(avlTree.getRoot());
    AVLTree::Node* maxNode = avlTree.findMax(avlTree.getRoot());

    //searching key
    if (searchResult != nullptr) {
        cout << "Key " << keyToSearch << " found in AVL tree." << endl;
    } else {
        cout << "Key " << keyToSearch << " not found in AVL tree." << endl;
    }

    //delete key
    cout<<"\n";
    avlTree.deleteNode(root,80);
    cout<<"\n";

    //min
    if (minNode != nullptr) {
        cout << "Minimum key in the AVL tree is: " << minNode->key << endl;
    } else {
        cout << "The tree is empty." << endl;
    }

    //max
    cout<<"\n";
    if (maxNode != nullptr) {
        cout << "Maximum key in the AVL tree is: " << maxNode->key << endl;
    } else {
        cout << "The tree is empty." << endl;
    }

    //ispresent
    cout<<"\n";
    bool found = avlTree.isPresent(avlTree.getRoot(),ispresentKey);
    if(found){
        cout<<"Key "<<ispresentKey<<" is present in the AVL tree";
    }
    else{
        cout<<"Key "<<ispresentKey<<"is not present in the AVL tree";
    }

    return 0;
}
