#include <unordered_map>
#include <vector>
#include <fstream>
#include <iostream>
#include <regex>
#include <unordered_map>
#include <string>

using namespace std;

class Point {
public:
    long x,y;

    Point():x(0), y(0) {}
    Point(long x, long y):x(x), y(y) {}
    Point(const Point& p):x(p.x), y(p.y) {}
};

class Equation {
public:
    Point A,B,Prize;

    Equation(): A(), B(), Prize(){}
    Equation(const Equation& eq): A(eq.A), B(eq.B), Prize(eq.Prize){}
};

vector<Equation> read_input()
{

    ifstream f("input.txt");

    if (!f.is_open())
    {
        cerr << "Error opening input file";
        exit(1);
    }

    string buffer;
    vector<pair<int,int>> raw_input;

    regex re(R"(X(?:\+|=)(\d+),\s*Y(?:\+|=)(\d+))");

    while (getline(f, buffer))
    {
        if (buffer.length() < 1)
            continue;

        smatch m;

        regex_search(buffer, m, re);

        raw_input.push_back(pair(atoi(m[1].str().c_str()), atoi(m[2].str().c_str())));

    }

    vector<Equation> input;
    for (size_t i=0 ; i<raw_input.size() ; i+=3) {
        Equation eq;
        eq.A = Point(raw_input[i].first, raw_input[i].second);
        eq.B = Point(raw_input[i+1].first, raw_input[i+1].second);
        eq.Prize = Point(raw_input[i+2].first, raw_input[i+2].second);
        input.push_back(eq);
    }

    return input;
}

void challenge1(vector<Equation> input) {
    size_t cost = 0;

    for (Equation& eq : input) {
        Equation copy(eq);

        copy.A.x *= eq.A.y;
        copy.B.x *= eq.A.y;
        copy.Prize.x *= eq.A.y;

        copy.A.y *= eq.A.x;
        copy.B.y *= eq.A.x;
        copy.Prize.y *= eq.A.x;

        copy.A.x -= copy.A.y;
        copy.B.x -= copy.B.y;
        copy.Prize.x -= copy.Prize.y;

        int B = copy.Prize.x / copy.B.x;
        int A = (eq.Prize.x - eq.B.x*B) / eq.A.x;


        if (copy.Prize.x % copy.B.x != 0 || (eq.Prize.x - eq.B.x*B) % eq.A.x != 0)
            continue;
        cost += A*3 + B;
    }

    cout << "Challenge1: " << cost << endl;
}

void challenge2(vector<Equation> input) {
    size_t cost = 0;

    for (Equation& eq : input) {
        eq.Prize.x += 10000000000000;
        eq.Prize.y += 10000000000000;

        Equation copy(eq);

        copy.A.x *= eq.A.y;
        copy.B.x *= eq.A.y;
        copy.Prize.x *= eq.A.y;

        copy.A.y *= eq.A.x;
        copy.B.y *= eq.A.x;
        copy.Prize.y *= eq.A.x;

        copy.A.x -= copy.A.y;
        copy.B.x -= copy.B.y;
        copy.Prize.x -= copy.Prize.y;

        long B = copy.Prize.x / copy.B.x;
        long A = (eq.Prize.x - eq.B.x*B) / eq.A.x;

        if (copy.Prize.x % copy.B.x != 0 || (eq.Prize.x - eq.B.x*B) % eq.A.x != 0)
            continue;
        cost += A*3 + B;
    }

    cout << "Challenge2: " << cost << endl;
}

int main () {
    vector<Equation> input = read_input();

    challenge1(input);
    challenge2(input);
    return 0;
}