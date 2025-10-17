#include <iostream>
#include <string>
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

    void printTree(const string& prefix = "", bool isLast = true) const {
        cout << prefix;
        cout << (isLast ? "└─ " : "├─ ");
        cout << value << endl;

        for (size_t i = 0; i < children.size(); ++i) {
            children[i].printTree(prefix + (isLast ? "   " : "│  "), i == children.size() - 1);
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