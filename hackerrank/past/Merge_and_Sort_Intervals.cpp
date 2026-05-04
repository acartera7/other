#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);



/*
 * Complete the 'mergeHighDefinitionIntervals' function below.
 *
 * The function is expected to return a 2D_INTEGER_ARRAY.
 * The function accepts 2D_INTEGER_ARRAY intervals as parameter.
 */

vector<vector<int>> mergeHighDefinitionIntervals(vector<vector<int>> intervals) {
    vector<vector<int>> output;
    if (intervals.empty()) return output;
    
    //std::cout << "Unsorted\n";
    //for (auto interval : intervals) {
    //    std::cout << interval[0] << " " << interval[1] << "\n";
    //}
    //std::cout << "\n\n";
    
    //Sort intervals by first number
    std::sort(intervals.begin(), intervals.end(), [](vector<int> x1, vector<int> x2){
        return x1.begin().base()[0] < x2.begin().base()[0];
    });
    
    //std::cout << "Sorted\n";
    //for (auto interval : intervals) {
    //    std::cout << interval[0] << " " << interval[1] << "\n";
    //}
    //std::cout << "\n\n";
    
    vector<vector<int>>::iterator last_merged;
    
    
    
    for (auto curr_interval : intervals) {
        
        // if output array is empty, just emplace current interval and grow from there
        if (output.empty()) {
            output.push_back(curr_interval);
            last_merged = output.begin();
            continue;
        }
        
        // split interval into points
        int curr_p1 = curr_interval[0];
        int curr_p2 = curr_interval[1];
        
        //get last merged
        int& out_p1 = (*last_merged)[0];
        int& out_p2 = (*last_merged)[1];
        
        if (out_p2 < curr_p1 || out_p1 > curr_p2) { 
            // no overlap
            output.push_back(curr_interval);
            last_merged = output.end()-1;
            continue;
            
        } else if (out_p1 <= curr_p1 && out_p2 >= curr_p2) { 
            // fully enveloped current inside last_merged
            continue;
        } else {
            // partial overlap full overlap
            if(curr_p1 < out_p1)
                out_p1 = curr_p1;
            if(curr_p2 > out_p2)
                out_p2 = curr_p2;
        }
        
    }
    return output;
}

int main()
{
    string intervals_rows_temp;
    getline(cin, intervals_rows_temp);

    int intervals_rows = stoi(ltrim(rtrim(intervals_rows_temp)));

    string intervals_columns_temp;
    getline(cin, intervals_columns_temp);

    int intervals_columns = stoi(ltrim(rtrim(intervals_columns_temp)));

    vector<vector<int>> intervals(intervals_rows);

    for (int i = 0; i < intervals_rows; i++) {
        intervals[i].resize(intervals_columns);

        string intervals_row_temp_temp;
        getline(cin, intervals_row_temp_temp);

        vector<string> intervals_row_temp = split(rtrim(intervals_row_temp_temp));

        for (int j = 0; j < intervals_columns; j++) {
            int intervals_row_item = stoi(intervals_row_temp[j]);

            intervals[i][j] = intervals_row_item;
        }
    }

    vector<vector<int>> result = mergeHighDefinitionIntervals(intervals);

    for (size_t i = 0; i < result.size(); i++) {
        for (size_t j = 0; j < result[i].size(); j++) {
            cout << result[i][j];

            if (j != result[i].size() - 1) {
                cout << " ";
            }
        }

        if (i != result.size() - 1) {
            cout << "\n";
        }
    }

    cout << "\n";

    return 0;
}

string ltrim(const string &str) {
    string s(str);

    s.erase(
        s.begin(),
        find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace)))
    );

    return s;
}

string rtrim(const string &str) {
    string s(str);

    s.erase(
        find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
        s.end()
    );

    return s;
}

vector<string> split(const string &str) {
    vector<string> tokens;

    string::size_type start = 0;
    string::size_type end = 0;

    while ((end = str.find(" ", start)) != string::npos) {
        tokens.push_back(str.substr(start, end - start));

        start = end + 1;
    }

    tokens.push_back(str.substr(start));

    return tokens;
}
