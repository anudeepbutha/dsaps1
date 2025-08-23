#include <iostream>
#include <vector>

using namespace std;

struct Node {
    Node* children[26];
    bool flag = false;
};

Node* root;

void insert (string word) {
    Node* current = root;
    for (char ch: word) {
        if(current->children[ch-'a'] == NULL) {
            Node* new_node = new Node;
            current->children[ch-'a'] = new_node;
        }
        current = current->children[ch-'a'];
    }
    current->flag = true;
}

bool operation1 (string word) {
    Node* current = root;
    for (char ch: word) {
        if (current->children[ch-'a'] == NULL)
            return false;
    current = current->children[ch-'a'];
    }
    return current->flag;
}

void printRecursive (string word, Node* start, vector<string> &suggestion) {
    if (start->flag == true){
        suggestion.push_back(word);
    }
    for(int i=0; i<26; i++)
        if (start->children[i] != NULL) {
            char tmp = (char) (i+97);
            printRecursive(word+tmp, start->children[i], suggestion);
        }
}

void operation2 (string word) {
    Node *current = root;
    for (char ch: word)
        if (current->children[ch-'a'] != NULL)
            current = current->children[ch-'a'];
        else {
            cout << "404";
            return;
        }

    vector<string> suggestion;
    printRecursive(word, current, suggestion);

    for (string str: suggestion)
        cout << str << endl;
}

int main() {
    int wordcount, querycount;
    // cin >> wordcount >> querycount;
    // cin.ignore();
    // vector<string> words;
    // for (int i=0; i<wordcount; i++) {
    //     string tmp;
    //     getline(cin, tmp);
    //     words.push_back(tmp);
    // }
    root = new Node;
    // for (string str: words)
        insert("consider");
        insert("filters");
        insert("filers");
        insert("entitled");
        insert("tilers");
        insert("litter");
        insert("dames");
        insert("filling");
        insert("grasses");
        insert("fitter");
        operation2("fil");
    return 0;
}