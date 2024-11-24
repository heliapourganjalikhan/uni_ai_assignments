#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <algorithm>

// Uninformed - Breadth First Search
using namespace std;

struct Statespace {
    int Puzzle[3][3];
    int Spare[2];
    int Eval;

    bool operator<(Statespace) const;
    int operator-(Statespace) const;
    bool operator==(Statespace) const;
    void operator=(Statespace);
};

class Equal {
    bool Greater;

public:
    Equal(const bool& Lesser = 1) {
        Greater = Lesser ? 0 : 1;
    }

    bool operator()(const Statespace& src, const Statespace& dest) const {
        return Greater ? (src < dest ? 0 : 1) : (src < dest);
    }
};

istream& operator>>(istream& input, Statespace& s) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            input >> s.Puzzle[i][j];
            if (s.Puzzle[i][j] == 0) {
                s.Spare[0] = i;
                s.Spare[1] = j;
            }
        }
    }
    return input;
}

ostream& operator<<(ostream& output, Statespace& s) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            output << s.Puzzle[i][j] << " ";
        }
        output << endl;
    }
    return output;
}

bool Statespace::operator<(Statespace s) const {
    return (Eval < s.Eval);
}

int Statespace::operator-(Statespace s) const {
    int count = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (Puzzle[i][j] != s.Puzzle[i][j] && Puzzle[i][j] != 0)
                count++;
        }
    }
    return count;
}

bool Statespace::operator==(Statespace s) const {
    return ((*this) - s == 0);
}

void Statespace::operator=(Statespace s) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            Puzzle[i][j] = s.Puzzle[i][j];
        }
    }
    Spare[0] = s.Spare[0];
    Spare[1] = s.Spare[1];
    Eval = s.Eval;
}

Statespace Initial, Goal, get();
int t = 0;
Statespace st;
ofstream ofs("out1.txt");
set<Statespace, Equal> Openlist;
vector<Statespace> Explored;

// Move Function to move blocks
inline int Move(char pos) {
    int p, q;
    int r, c;

    int s1 = st.Spare[0];
    int s2 = st.Spare[1];

    if (pos == 'R') {
        p = 0;
        q = 1;
    } else if (pos == 'L') {
        p = 0;
        q = -1;
    } else if (pos == 'U') {
        p = 1;
        q = 0;
    } else if (pos == 'D') {
        p = -1;
        q = 0;
    }

    r = s1 + p;
    c = s2 + q;

    swap(st.Puzzle[s1][s2], st.Puzzle[r][c]);

    if (find(Explored.begin(), Explored.end(), st) != Explored.end()) {
        st = Initial;
        return 0;
    }

    st.Spare[0] = r;
    st.Spare[1] = c;
    get();

    Openlist.insert(st);
    st = Initial;

    return 0;
}

Statespace get() {
    st.Eval = Goal - st;
    return st;
}

int BFS() {
    auto l = Openlist.begin();
    st = *l;

    if (st == Goal) {
        return t;
    }

    ofs << st << endl;
    cout << st << endl;

    Explored.push_back(st);
    Openlist.erase(l);

    Initial = st;

    if (st.Spare[1] < 2)
        Move('R');
    if (st.Spare[1] > 0)
        Move('L');
    if (st.Spare[0] < 2)
        Move('U');
    if (st.Spare[0] > 0)
        Move('D');

    t++;
    BFS();
    return -1;
}

int main() {
    ifstream inf("A2in1.txt");
    if (!inf.is_open()) {
        cerr << "Error: Unable to open input file 'A2in1.txt'." << endl;
        return 1;
    }

    inf >> Initial >> Goal;

    Openlist.insert(Initial);

    BFS();

    ofs << st << endl;
    cout << st << endl;

    // Process the results dynamically
    ifstream infile("out1.txt");
    ofstream ofile("BFS_out.txt");

    if (!infile.is_open() || !ofile.is_open()) {
        cerr << "Error: Unable to open result files." << endl;
        return 1;
    }

    vector<vector<int>> states;
    vector<int> zero_positions;

    // Read all states dynamically
    while (!infile.eof()) {
        vector<int> state(9);
        int zero_index = -1;
        for (int i = 0; i < 9; i++) {
            if (!(infile >> state[i]))
                break;
            if (state[i] == 0)
                zero_index = i;
        }
        if (!state.empty()) {
            states.push_back(state);
            zero_positions.push_back(zero_index);
        }
    }

    // Write movements
    for (size_t i = 1; i < states.size(); i++) {
        int prev_zero = zero_positions[i - 1];
        int curr_zero = zero_positions[i];
        const vector<int>& prev_state = states[i - 1];

        if (curr_zero == prev_zero + 1) {
            ofile << prev_state[curr_zero] << " left" << endl;
        } else if (curr_zero == prev_zero - 1) {
            ofile << prev_state[curr_zero] << " right" << endl;
        } else if (curr_zero == prev_zero + 3) {
            ofile << prev_state[curr_zero] << " up" << endl;
        } else if (curr_zero == prev_zero - 3) {
            ofile << prev_state[curr_zero] << " down" << endl;
        }
    }

    ofile << "Goal state found." << endl;
    cout << "Goal state found." << endl;

    return 0;
}
