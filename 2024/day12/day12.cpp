#include <fstream>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <set>
#include <algorithm>

using namespace std;

typedef vector<char> Row;
typedef vector<Row> Map;
typedef vector<pair<size_t, size_t>> Region;

#define GET_IDX(i, j, map) i *map[0].size() + j
#define GET_IDX2(pair, map) pair.first *map[0].size() + pair.second
#define GET_IDX3(pair, map) pair.second *map.size() + pair.first

Map read_input()
{

    ifstream f("input.txt");

    if (!f.is_open())
    {
        cerr << "Error opening input file";
        exit(1);
    }

    string buffer;
    Map input;

    while (getline(f, buffer))
    {
        if (buffer.length() <= 1)
            break;
        Row line;
        for (const char *c = buffer.c_str(); *c; c++)
            line.push_back(*c);
        input.push_back(line);
    }

    return input;
}

void print_map(Map map)
{
    for (Row row : map)
    {
        for (char c : row)
            cout << c;
        cout << '\n';
    }
    cout << endl;
}

Region search_region(size_t i, size_t j, char crop, Map &map, unordered_map<size_t,bool> &visited)
{
    Region region;

    if (visited.find(GET_IDX(i,j,map)) != visited.end() || map[i][j] != crop) return region;

    visited[GET_IDX(i,j,map)] = true;

    Region up, down, left, right;
    region.push_back(pair(i,j));
    
    if (i > 0)
        up = search_region(i-1, j, crop, map, visited);
    if (i+1 < map.size())
        down = search_region(i+1, j, crop, map, visited);
    if (j > 0)
        left = search_region(i, j-1, crop, map, visited);
    if (j+1 < map[0].size())
        right = search_region(i, j+1, crop, map, visited);

    for (Region r: {up, down, left, right})
        for (auto& p: r)
            region.push_back(p);

    return region;
}

bool comp (size_t a, size_t b) {
    return a<=b;
}

pair<int, int> calculate_region_cost(Region &region, Map &map) {
    int perimeter = 0;
    char crop = map[region[0].first][region[0].second];

    set<size_t> above, under, left, right;

    for (auto& pair : region) {

        // see if it has edge above
        if (pair.first == 0 || map[pair.first-1][pair.second] != crop) {
            perimeter++;
            above.insert(GET_IDX2(pair, map));
        }

        // see if it has edge under
        if (pair.first == map.size()-1 || map[pair.first+1][pair.second] != crop) {
            perimeter++;
            under.insert(GET_IDX2(pair, map));
        }

        // see if it as edge on left
        if (pair.second == 0 || map[pair.first][pair.second-1] != crop) { 
            perimeter++;
            left.insert(GET_IDX3(pair, map));
        }

        // see if it as edge on right
        if (pair.second == map[0].size()-1 || map[pair.first][pair.second+1] != crop) { 
            perimeter++;
            right.insert(GET_IDX3(pair, map));
        }
    }

    vector<size_t> above_vec (above.begin(), above.end()), 
                    under_vec(under.begin(), under.end()),
                    left_vec(left.begin(), left.end()),
                    right_vec(right.begin(), right.end());

    sort(above_vec.begin(), above_vec.end(), comp);
    sort(under_vec.begin(), under_vec.end(), comp);
    sort(left_vec.begin(), left_vec.end(), comp);
    sort(right_vec.begin(), right_vec.end(), comp);

    int num_edges = 0;

    for (size_t i=1 ; i<under_vec.size() ; i++)
        if (under_vec[i] - under_vec[i-1] > 1) num_edges++;
    num_edges++;

    for (size_t i=1 ; i<above_vec.size() ; i++)
        if (above_vec[i] - above_vec[i-1] > 1) num_edges++;
    num_edges++;

    for (size_t i=1 ; i<left_vec.size() ; i++)
        if (left_vec[i] - left_vec[i-1] > 1) num_edges++;
    num_edges++;

    for (size_t i=1 ; i<right_vec.size() ; i++)
        if (right_vec[i] - right_vec[i-1] > 1) num_edges++;
    num_edges++;

    return pair(perimeter * region.size(), num_edges * region.size());
}

void solve(Map input) {
    vector<Region> regions;
    unordered_map<size_t, size_t> pos_to_region; // map position to its region ID -> position in regions vector

    for (size_t i = 0; i < input.size(); i++)
    {
        Row row = input[i];

        for (size_t j = 0; j < row.size(); j++)
        {
            size_t idx = GET_IDX(i, j, input);

            if (pos_to_region.find(idx) == pos_to_region.end())
            {
                unordered_map<size_t,bool> visited;
                char crop = input[i][j];
                Region r = search_region(i, j, crop, input, visited);
                size_t region_id = regions.size();
                regions.push_back(r);

                for (auto &pair : r)
                    pos_to_region[GET_IDX(pair.first, pair.second, input)] = region_id;
            }
        }
    }

    int result = 0, result2 = 0;
    for (Region r : regions) {
        pair<int,int> res = calculate_region_cost(r, input);
        
        result += res.first;
        result2 += res.second;
    }

    cout << "Challenge1: " << result << endl;
    cout << "Challenge2: " << result2 << endl;
}

int main()
{
    Map input = read_input();

    solve(input);
    
    return 0;
}