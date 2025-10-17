#include <iostream>
#include <fstream>
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

    void printTree(ofstream& out, const string& prefix = "", bool isLast = true) const {
        out << prefix;
        out << (isLast ? "└─ " : "├─ ");
        out << value << endl;
        for (size_t i = 0; i < children.size(); ++i) {
            children[i].printTree(out, prefix + (isLast ? "   " : "│  "), i == children.size() - 1);
        }
    }
};

tree parseTree(ifstream& file) {
    string line;
    tree root("root");
    vector<tree*> stack;
    stack.push_back(&root);

    while (getline(file, line)) {
        string trimmed = "";
        for (int i = 0; i < line.size(); i++) {
            if (line[i] != '\r' && line[i] != '\n') trimmed += line[i];
        }
        if (trimmed.empty()) continue;

        if (trimmed.back() == '{') {
            string key = trimmed.substr(0, trimmed.size() - 1);
            tree newNode(key);
            stack.back()->addChild(newNode);
            stack.push_back(&stack.back()->children.back());
        }
        else if (trimmed == "}") {
            if (stack.size() > 1) stack.pop_back();
        }
        else {
            size_t colon = trimmed.find(':');
            if (colon != string::npos) {
                string key = trimmed.substr(0, colon);
                string val = trimmed.substr(colon + 1);
                tree nodeKey(key);
                tree nodeVal(val);
                nodeKey.addChild(nodeVal);
                stack.back()->addChild(nodeKey);
            }
            else {
                tree node(trimmed);
                stack.back()->addChild(node);
            }
        }
    }

    return root;
}

int main() {
    ifstream infile("data.txt");
    if (!infile) return 1;

    tree t = parseTree(infile);
    infile.close();

    ofstream outfile("tree_output.txt");
    if (!outfile) return 1;

    t.printTree(outfile);
    outfile.close();

    return 0;
}