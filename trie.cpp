#include <iostream>
#include <vector>

using namespace std;

struct Node {
    Node* children[26];
    bool flag = false;
    int visited = 0;
};

struct queries {
    int op;
    string queryword;
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
            cout << "0" << endl;
            return;
        }

    vector<string> suggestion;
    printRecursive(word, current, suggestion);

    cout << suggestion.size() << endl;
    for (string str: suggestion)
        cout << str << endl;
}

void distanceRec(int index, string word, int r, Node* start, string str) {
    if (r<0 || index > word.length())
        return;

    if (index == word.length() && start->flag == true && start->visited == 0) {
        start->visited = true;
        cout << str << endl;
    }

    for (int i=0; i<26; i++) {
        if (start->children[i] != NULL) {
            char tmp = 'a'+i;
            if (index < word.length() && word[index] == tmp)
                distanceRec (index+1, word, r, start->children[i], str+tmp);
            distanceRec (index, word, r-1, start->children[i], str+tmp);
            distanceRec (index+1, word, r-1, start->children[i], str+tmp);
        }
    }
    distanceRec (index+1, word, r-1, start, str);
}

void operation3 (string word) {
    int startIndex = 0, distance = 3;
    string empty;
    distanceRec(startIndex, word, distance, root, empty);
}

void reinitialize(Node *item) {
    if (item == NULL) return;
    item->visited = 0;
    for (int i=0; i<26; i++)
        if (item->children[i] != 0) {
            reinitialize(item->children[i]);
        }
}

int main() {
    int wordcount, querycount;
    cin >> wordcount >> querycount;
    cin.ignore();

    vector<string> words;
    for (int i=0; i<wordcount; i++) {
        string tmp;
        getline(cin, tmp);
        words.push_back(tmp);
    }

    vector<queries> query_vector;
    for (int i=0; i<querycount; i++) {
        int num;
        string str;
        queries query;
        cin >> num >> str;
        cin.ignore();
        query.op = num;
        query.queryword = str;
        query_vector.push_back(query);
    }

    root = new Node;
    for (string str: words)
        insert(str);

    for (queries q: query_vector)
        if (q.op == 1)
            cout << operation1(q.queryword) << endl;
        else if (q.op == 2)
            operation2(q.queryword);
        else if (q.op == 3){
            operation3(q.queryword);
            reinitialize(root);
        }
        else
            cout << "invalid input" << endl;
    return 0;
}