#include <iostream>
#include <string>
#include <fstream>
#include <vector>


using namespace std;

class tree {
public:
    string value;
    vector<tree> children;

    tree(string val) : value(val) {}

    void addChild(const tree& child) {
        children.push_back(child);
    }

    void printTree(int depth = 0) const {
        for (int i = 0; i < depth; i++) cout << "  ";
        cout << value << endl;
        for (const auto& child : children) {
            child.printTree(depth + 1);
        }
    }
};

int main() {
    tree root("Root");

    tree child1("Child1");
    tree child2("Child2");

    child1.addChild(tree("Grandchild1"));
    child1.addChild(tree("Grandchild2"));
    child2.addChild(tree("Grandchild3"));

    root.addChild(child1);
    root.addChild(child2);

    root.printTree();

    return 0;
}
