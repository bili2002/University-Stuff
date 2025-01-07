#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <random>
#define LAPLACE_LAMBDA 0.5
#define LOG_OPT 1
#define NUM_FOLDS 10
 
using namespace std;
 
random_device rd;
mt19937 rng(rd());
 
enum Party { DEMOCRAT, REPUBLICAN };
 
struct Datapoint {
    Party party;
    vector<char> votes;
};
 
class PartyClassifier {
private:
    vector<Datapoint> training_data;
 
public:
    PartyClassifier(const vector<Datapoint>& training_data) : training_data(training_data) {}
 
    Party classify(const vector<char>& votes) {
        vector<pair<double, double>> vote_influences = get_vote_influences(votes);
        pair<double, double> party_probabilities = calculate_party_probabilities(vote_influences);
 
        return party_probabilities.first > party_probabilities.second ? DEMOCRAT : REPUBLICAN;
    }
 
private:
 
    pair<double, double> calculate_party_probabilities(const vector<pair<double, double>>& vote_influences) {
        pair<double, double> party_probabilities = calculate_party_probabilities();
 
        double democrat_probability = LOG_OPT ? party_probabilities.first : log(party_probabilities.first);
        for (const auto& vote_probability : vote_influences) {
            democrat_probability = LOG_OPT ?
                (vote_probability.first * democrat_probability)
                : (log(vote_probability.first) + democrat_probability);
        }
 
        double republican_probability = LOG_OPT ? party_probabilities.second : log(party_probabilities.second);
        for (const auto& vote_probability : vote_influences) {
            republican_probability = LOG_OPT ?
                (vote_probability.second * republican_probability)
                : (log(vote_probability.second) + republican_probability);
        }
 
        return {democrat_probability, republican_probability};
    }
 
    pair<double, double> calculate_party_probabilities() {
        double democrat_probability = 0.0;
        double republican_probability = 0.0;
        for (const auto& dp : training_data) {
            if (dp.party == DEMOCRAT) {
                ++democrat_probability;
            } else {
                ++republican_probability;
            }
        }
        democrat_probability /= training_data.size();
        republican_probability /= training_data.size();
 
        return {democrat_probability, republican_probability};
    }
 
    vector<pair<double, double>> get_vote_influences(const vector<char>& votes) {
        vector<pair<double, double>> result(votes.size());
        for (size_t vote_id = 0; vote_id < votes.size(); ++vote_id) {
            result[vote_id] = {
                calculate_vote_probability(DEMOCRAT, votes[vote_id], vote_id),
                calculate_vote_probability(REPUBLICAN, votes[vote_id], vote_id)
            };
        }
        return result;
    }
 
    double calculate_vote_probability(Party party, const char& vote, size_t vote_id) {
        size_t same_party = 0;
        for (const auto& dp : training_data) {
            if (dp.party == party) {
                ++same_party;
            }
        }
 
        size_t same_party_and_same_vote = 0;
        for (const auto& dp : training_data) {
            if (dp.party == party && dp.votes[vote_id] == vote) {
                ++same_party_and_same_vote;
            }
        }
        return static_cast<double>(same_party_and_same_vote + LAPLACE_LAMBDA) / (same_party + 2 * LAPLACE_LAMBDA);
    }
};
 
vector<Datapoint> parse_votes_dataset(const string& filename) {
    bool should_fix_data;
    cin >> should_fix_data;
 
    ifstream file(filename);
    vector<Datapoint> dataset;
    string line;
 
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        vector<char> votes;
        getline(ss, token, ',');
        Party party = (token == "democrat") ? DEMOCRAT : REPUBLICAN;
 
        while (getline(ss, token, ',')) {
            votes.push_back(token[0]);
        }
 
        dataset.push_back({party, votes});
    }
 
    if (should_fix_data) {
        for (size_t i = 0; i < dataset.size(); ++i) {
            for (size_t j = 0; j < dataset[i].votes.size(); ++j) {
                if (dataset[i].votes[j] == '?') {
                    size_t voted_yes = 0;
                    for (const auto& dp : dataset) {
                        if (dp.votes[j] == 'y') {
                            ++voted_yes;
                        }
                    }
                    dataset[i].votes[j] = (voted_yes >= dataset.size() / 2) ? 'y' : 'n';
                }
            }
        }
    }
 
    return dataset;
}
 
pair<vector<Datapoint>, vector<Datapoint>> split_by_party(const vector<Datapoint>& data) {
    vector<Datapoint> democrat_data;
    vector<Datapoint> republican_data;
 
    for (const auto& dp : data) {
        if (dp.party == DEMOCRAT) {
            democrat_data.push_back(dp);
        } else {
            republican_data.push_back(dp);
        }
    }
 
    return {democrat_data, republican_data};
}
 
pair<vector<Datapoint>, vector<Datapoint>> split_data(const vector<Datapoint>& data, double training_ratio) {
    vector<Datapoint> shuffled_data = data;
    shuffle(shuffled_data.begin(), shuffled_data.end(), rng);
    auto [democrat_data, republican_data] = split_by_party(shuffled_data);
 
    size_t democrat_training_size = static_cast<size_t>(training_ratio * democrat_data.size());
    size_t republican_training_size = static_cast<size_t>(training_ratio * republican_data.size());
 
    vector<Datapoint> training_data;
    vector<Datapoint> test_data;
 
    for (size_t i = 0; i < democrat_data.size(); ++i) {
        if (i < democrat_training_size) {
            training_data.push_back(democrat_data[i]);
        } else {
            test_data.push_back(democrat_data[i]);
        }
    }
 
    for (size_t i = 0; i < republican_data.size(); ++i) {
        if (i < republican_training_size) {
            training_data.push_back(republican_data[i]);
        } else {
            test_data.push_back(republican_data[i]);
        }
    }
 
    return {training_data, test_data};
}
 
vector<vector<Datapoint>> get_folds(const vector<Datapoint>& data) {
    vector<Datapoint> shuffled_data = data;
    shuffle(shuffled_data.begin(), shuffled_data.end(), rng);
    auto [democrat_data, republican_data] = split_by_party(shuffled_data);
 
    vector<vector<Datapoint>> folds(NUM_FOLDS);
    for (size_t i = 0; i < democrat_data.size(); ++i) {
        folds[i % NUM_FOLDS].push_back(democrat_data[i]);
    }
    for (size_t i = 0; i < republican_data.size(); ++i) {
        folds[i % NUM_FOLDS].push_back(republican_data[i]);
    }
 
    return folds;
}
 
double calculate_accuracy(PartyClassifier& classifier, const vector<Datapoint>& test_data) {
    size_t correct = 0;
    for (const auto& dp : test_data) {
        if (classifier.classify(dp.votes) == dp.party) {
            ++correct;
        }
    }
    return static_cast<double>(correct) / test_data.size();
}
 
double calculate_average(const vector<double>& values) {
    double sum = 0.0;
    for (const auto& value : values) {
        sum += value;
    }
    return sum / values.size();
}
 
double calculate_standard_deviation(const vector<double>& values, double average) {
    double variance = 0.0;
    for (const auto& value : values) {
        variance += pow(value - average, 2);
    }
    return sqrt(variance / values.size());
}
 
int main() {
    string dataset_path = "house-votes-84.data";
    vector<Datapoint> dataset = parse_votes_dataset(dataset_path);
    auto data_split = split_data(dataset, 0.8);
 
    PartyClassifier classifier(data_split.first);
    double train_accuracy = calculate_accuracy(classifier, data_split.first);
    cout << fixed << setprecision(2);
    cout << "1. Train Set Accuracy:" << endl;
    cout << "  Accuracy: " << train_accuracy * 100 << "%" << endl;
    cout << endl;
 
    vector<vector<Datapoint>> folds = get_folds(dataset);
    vector<double> fold_accuracies;
 
    cout << "10-Fold Cross-Validation Results:" << endl;
    for (size_t i = 0; i < NUM_FOLDS; ++i) {
        vector<Datapoint> training_data;
        for (size_t j = 0; j < NUM_FOLDS; ++j) {
            if (j != i) {
                training_data.insert(training_data.end(), folds[j].begin(), folds[j].end());
            }
        }
        PartyClassifier fold_classifier(training_data);
        fold_accuracies.push_back(calculate_accuracy(fold_classifier, folds[i]));
        cout << "  Fold " << i + 1 << " Accuracy: " << fold_accuracies.back() * 100 << "%" << endl;
    }
 
    cout << endl;
    double average = calculate_average(fold_accuracies);
    cout << "  Average Accuracy: " << average * 100 << "%" << endl;
 
    double standard_deviation = calculate_standard_deviation(fold_accuracies, average);
    cout << "  Standard Deviation: " << standard_deviation * 100 << "%" << endl;
    cout << endl;
 
    double test_accuracy = calculate_accuracy(classifier, data_split.second);
    cout << "2. Test Set Accuracy:" << endl;
    cout << "  Accuracy: " << test_accuracy * 100 << "%" << endl;
    return 0;
}
 