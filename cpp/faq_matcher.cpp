#include <bits/stdc++.h>
using namespace std;

string lowerStr(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

string cleanText(string s) {
    string out;
    for (char c : s) {
        if (isalnum((unsigned char)c) || isspace((unsigned char)c)) out += tolower(c);
        else out += ' ';
    }
    return out;
}

vector<string> tokenize(const string& text) {
    unordered_set<string> stop = {
        "the","is","are","a","an","of","to","in","on","for","and","or","can","you","i",
        "any","there","does","what","when","how","pls","please","kya","kaise","it","me",
        "my","we","our","your","this","that","with","as","if","be","do","did","at","from"
    };

    vector<string> tokens;
    stringstream ss(cleanText(text));
    string w;
    while (ss >> w) {
        if (!stop.count(w)) tokens.push_back(w);
    }
    return tokens;
}

int overlapScore(const vector<string>& a, const vector<string>& b) {
    unordered_set<string> A(a.begin(), a.end()), B(b.begin(), b.end());
    int score = 0;
    for (const auto& x : A) if (B.count(x)) score += 2;
    return score;
}

struct FAQ {
    string question;
    string answer;
    vector<string> qTok;
    vector<string> aTok;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ifstream fin("data/nust_pro_dataset_1200.txt");
    if (!fin) {
        cerr << "Could not open dataset file.\n";
        return 1;
    }

    vector<FAQ> faqs;
    string line, q = "", a = "";

    while (getline(fin, line)) {
        smatch mq, ma;
        regex qre(R"(^Q\d+[a-z]*:\s*(.*)$)", regex_constants::icase);
        regex are(R"(^A\d+[a-z]*:\s*(.*)$)", regex_constants::icase);

        if (regex_match(line, mq, qre)) {
            q = mq[1];
        } else if (regex_match(line, ma, are) && !q.empty()) {
            a = ma[1];
            faqs.push_back({q, a, tokenize(q), tokenize(a)});
            q = "";
            a = "";
        }
    }

    cout << "Enter your question: ";
    string query;
    getline(cin, query);

    auto qTok = tokenize(query);

    int bestScore = -1;
    string bestAnswer = "Sorry, no confident match found.";

    for (const auto& f : faqs) {
        int score = overlapScore(qTok, f.qTok) + overlapScore(qTok, f.aTok);
        if (score > bestScore) {
            bestScore = score;
            bestAnswer = f.answer;
        }
    }

    cout << "\nBest Answer:\n" << bestAnswer << "\n";
    return 0;
}