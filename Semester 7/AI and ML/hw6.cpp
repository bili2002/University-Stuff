#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <random>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
#include <unordered_map>

using namespace std;

static const int TREE_COUNT = 10;
static const int FOLD_COUNT = 10;
random_device rd;
mt19937 rng(rd());

enum Label { NO_RECURRENCE, RECURRENCE };

struct Record {
    vector<string> features;
    Label label;
};

struct Node {
    bool leaf;
    Label leaf_label;
    int split_attr;
    map<string, Node*> children;
};

int mode;
int max_depth = 5;

vector<Record> read_data(const string& filepath) {
    ifstream file(filepath);
    vector<Record> dataset;
    string lineStr;
    while (getline(file, lineStr)) {
        if (lineStr.empty() || lineStr[0] == '@') continue;
        stringstream ss(lineStr);
        vector<string> columns;
        while (ss.good()) {
            string tmp;
            getline(ss, tmp, ',');
            columns.push_back(tmp);
        }
        if (columns.size() < 2) continue;
        string lblStr = columns.back();
        columns.pop_back();
        Label lbl = (lblStr == "no") ? NO_RECURRENCE : RECURRENCE;
        dataset.push_back({columns, lbl});
    }
    return dataset;
}

pair<vector<Record>, vector<Record>> split_dataset(const vector<Record>& data, double ratio) {
    vector<Record> no_recurrence, recurrence;
    for (auto& x : data) {
        x.label == NO_RECURRENCE ? no_recurrence.push_back(x) : recurrence.push_back(x);
    }
    shuffle(no_recurrence.begin(), no_recurrence.end(), rng);
    shuffle(recurrence.begin(), recurrence.end(), rng);
    size_t s1 = (size_t)(ratio * no_recurrence.size());
    size_t s2 = (size_t)(ratio * recurrence.size());
    vector<Record> train_set, test_set;
    train_set.insert(train_set.end(), no_recurrence.begin(), no_recurrence.begin() + s1);
    train_set.insert(train_set.end(), recurrence.begin(), recurrence.begin() + s2);
    test_set.insert(test_set.end(), no_recurrence.begin() + s1, no_recurrence.end());
    test_set.insert(test_set.end(), recurrence.begin() + s2, recurrence.end());
    return {train_set, test_set};
}

vector<vector<Record>> create_folds(const vector<Record>& data) {
    vector<Record> no_recurrence, recurrence;
    for (auto& x : data) {
        x.label == NO_RECURRENCE ? no_recurrence.push_back(x) : recurrence.push_back(x);
    }
    shuffle(no_recurrence.begin(), no_recurrence.end(), rng);
    shuffle(recurrence.begin(), recurrence.end(), rng);
    vector<vector<Record>> folds(FOLD_COUNT);
    for (int i = 0; i < (int)no_recurrence.size(); i++) {
        folds[i % FOLD_COUNT].push_back(no_recurrence[i]);
    }
    for (int i = 0; i < (int)recurrence.size(); i++) {
        folds[i % FOLD_COUNT].push_back(recurrence[i]);
    }
    return folds;
}

double entropy(const vector<Record>& data) {
    if (data.empty()) return 0;
    double c1 = 0, c2 = 0;
    for (auto& x : data) x.label == NO_RECURRENCE ? c1++ : c2++;
    double p1 = c1 / data.size(), p2 = c2 / data.size();
    auto f = [&](double x) { return x < 1e-12 ? 0 : log2(x); };
    return -p1 * f(p1) - p2 * f(p2);
}

map<string, vector<Record>> split_attr_data(const vector<Record>& data, int idx) {
    map<string, vector<Record>> m;
    for (auto& x : data) {
        m[x.features[idx]].push_back(x);
    }
    return m;
}

double gain(const vector<Record>& data, int idx) {
    double e = entropy(data);
    auto s = split_attr_data(data, idx);
    double sum_val = 0;
    for (auto& pair : s) {
        double r = (double)pair.second.size() / data.size();
        sum_val += r * entropy(pair.second);
    }
    return e - sum_val;
}

int best_attribute(const vector<Record>& data, const set<int>& used) {
    double best_gain_val = -1;
    int best_index = -1;
    for (int i = 0; i < (int)data[0].features.size(); i++) {
        if (used.count(i)) continue;
        double g = gain(data, i);
        if (g > best_gain_val) {
            best_gain_val = g;
            best_index = i;
        }
    }
    return best_index;
}

Node* build_tree(vector<Record> data, set<int> used, int depth) {
    if (data.empty()) {
        Node* leaf = new Node{true, NO_RECURRENCE, -1, {}};
        return leaf;
    }
    bool all_no = true, all_yes = true;
    for (auto& x : data) {
        if (x.label == NO_RECURRENCE) all_yes = false;
        else all_no = false;
    }
    if (all_no || all_yes) {
        Node* leaf = new Node{true, all_no ? NO_RECURRENCE : RECURRENCE, -1, {}};
        return leaf;
    }
    if (mode == 0 && depth >= max_depth) {
        int c1 = 0, c2 = 0;
        for (auto& x : data) x.label == NO_RECURRENCE ? c1++ : c2++;
        Node* leaf = new Node{true, (c1 >= c2 ? NO_RECURRENCE : RECURRENCE), -1, {}};
        return leaf;
    }
    int best_attr_index = best_attribute(data, used);
    if (best_attr_index < 0) {
        int c1 = 0, c2 = 0;
        for (auto& x : data) x.label == NO_RECURRENCE ? c1++ : c2++;
        Node* leaf = new Node{true, (c1 >= c2 ? NO_RECURRENCE : RECURRENCE), -1, {}};
        return leaf;
    }
    Node* root = new Node{false, NO_RECURRENCE, best_attr_index, {}};
    used.insert(best_attr_index);
    auto branches = split_attr_data(data, best_attr_index);
    for (auto& pair : branches) {
        root->children[pair.first] = build_tree(pair.second, used, depth + 1);
    }
    return root;
}

Label predict(Node* root, const Record& r) {
    if (!root) return NO_RECURRENCE;
    if (root->leaf) return root->leaf_label;
    string val = r.features[root->split_attr];
    if (!root->children.count(val)) return root->leaf_label;
    return predict(root->children[val], r);
}

double accuracy(Node* root, const vector<Record>& data) {
    if (data.empty()) return 0;
    int c = 0;
    for (auto& x : data) {
        if (predict(root, x) == x.label) c++;
    }
    return (double)c / data.size();
}

double forest_accuracy(const vector<Node*>& forest, const vector<Record>& data) {
    if (data.empty()) return 0;
    int c = 0;
    for (auto& x : data) {
        map<Label, int> freq;
        for (auto& t : forest) freq[predict(t, x)]++;
        Label best = (freq[NO_RECURRENCE] >= freq[RECURRENCE] ? NO_RECURRENCE : RECURRENCE);
        if (best == x.label) c++;
    }
    return (double)c / data.size();
}

void reduced_error_prune(Node*& root, Node* tree_root, const vector<Record>& val) {
    if (!root || root->leaf) return;
    for (auto& ch : root->children) reduced_error_prune(ch.second, tree_root, val);
    
    double original = accuracy(tree_root, val);
    Node old = *root;
    Node* tmp = new Node{true, NO_RECURRENCE, -1, {}};
    int c1 = 0, c2 = 0;

    for (auto& x : val) x.label == NO_RECURRENCE ? c1++ : c2++;
    tmp->leaf_label = c1 >= c2 ? NO_RECURRENCE : RECURRENCE;

    bool lf = root->leaf;
    auto chd = root->children;
    int sp = root->split_attr;

    root->leaf = true;
    root->leaf_label = tmp->leaf_label;
    root->children.clear();
    root->split_attr = -1;
    
    double pruned = accuracy(tree_root, val);
    if (pruned < original) {
        root->leaf = lf;
        root->split_attr = sp;
        root->children = chd;
        root->leaf_label = old.leaf_label;
    }
    delete tmp;
}

vector<Node*> build_forest(const vector<Record>& data, double ratio) {
    vector<Node*> forest;
    for (int i = 0; i < TREE_COUNT; i++) {
        vector<Record> bag;
        for (int j = 0; j < (int)data.size(); j++) {
            int idx = uniform_int_distribution<int>(0, data.size() - 1)(rng);
            bag.push_back(data[idx]);
        }
        set<int> used;
        Node* root = build_tree(bag, used, 0);
        if (mode == 1) reduced_error_prune(root, root, bag);
        forest.push_back(root);
    }
    return forest;
}

pair<double, double> cross_validate(const vector<Record>& data) {
    auto fs = create_folds(data);
    vector<double> vals;
    for (int i = 0; i < FOLD_COUNT; i++) {
        vector<Record> train_set;
        for (int j = 0; j < FOLD_COUNT; j++) {
            if (j == i) continue;
            train_set.insert(train_set.end(), fs[j].begin(), fs[j].end());
        }
        auto forest = build_forest(train_set, 0.8);
        double acc = forest_accuracy(forest, fs[i]);
        vals.push_back(acc);
        for (auto& t : forest) {
            delete t;
        }

        cout << "  Fold " << i + 1 << " Accuracy: " << acc * 100 << "%" << endl;
    }
    double s = 0;
    for (auto& x : vals) s += x;
    double avg = s / vals.size();
    double var = 0;
    for (auto& x : vals) var += (x - avg) * (x - avg);
    var /= vals.size();
    return {avg, sqrt(var)};
}

int main() {
    cin >> mode;
    string filepath = "breast-cancer.data";
    auto dataset = read_data(filepath);
    auto splitted = split_dataset(dataset, 0.8);

    auto training_data = splitted.first;
    auto testing_data = splitted.second;

    auto forest = build_forest(training_data, 0.8);
    double train_accuracy = forest_accuracy(forest, training_data);

    cout << fixed << setprecision(2);
    cout << "1. Train Set Accuracy:" << endl;
    cout << "  Accuracy: " << train_accuracy * 100 << "%" << endl << endl;

    cout << "10-Fold Cross-Validation Results:" << endl;
    auto cv = cross_validate(training_data);

    cout << "\n\n  Average Accuracy: " << cv.first * 100 << "%" << endl;
    cout << "  Standard Deviation: " << cv.second * 100 << "%" << endl << endl;

    double test_accuracy = forest_accuracy(forest, testing_data);
    cout << "2. Test Set Accuracy:" << endl;
    cout << "  Accuracy: " << test_accuracy * 100 << "%" << endl;
    
    return 0;
}
