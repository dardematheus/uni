#include <iostream>
#include <array>
#include <string>
#include <fstream>
#include <array>
#include <vector>
#include <set>
#include <ctime>

using namespace std;

char parse_file(ifstream &file, array<string, 26> &table, set<char> &check_set);
void print_table(array<string, 26> &table);
vector<char> expand(array<string, 26> &input_table, vector<char> &word, set<char> &check_set);

int
main(int argc, char *argv[])
{
    ifstream file;
    array<string, 26> table;
    char top;
    vector<char> word, expanded;
    set<char> check_set;
    clock_t start, end;

    if(argc != 2){
        cout << "error: Wrong usage. Must pass filename as argument.\n";
        return -1;
    }
    file.open(argv[1]);
    if(!file.is_open()){
        cout << "error: Could not open specified file.\n";
        return -1;
    }

    top = parse_file(file, table, check_set);
    word.push_back(top);
    
    start = clock();
    expanded = expand(table, word, check_set);
    end = clock();
    cout << expanded.size() << "\n" << "Ran for: " << (end - start) / CLOCKS_PER_SEC << " seconds\n";
    
    file.close();
    return 0;
}

vector<char>
expand(array<string, 26> &input_table, vector<char> &word, set<char> &check_set)
{
    vector<char> output;
    int len = word.size();
    char c;
    string val;

    for(char c : word){
        val = input_table[c - 'a'];
        for(char x : val){
            output.push_back(x);
        }
    }

    for(char c : output){
        if(check_set.count(c) == 0){
            return expand(input_table, output, check_set);
            break;
        } 
    }

    return output;

}

char
parse_file(ifstream &file, array<string, 26> &table, set<char> &check_set)
{
    string line, value;
    size_t strlen;
    char c, top;

    while(getline(file, line)){
        c = line[0];
        value.assign(line, 2, line.length() - 1);
        if(value.length() <= 2){
            check_set.insert(c);
            value = c;
        }
        table[c - 'a'] = value;
    }

    top = 'a';
    for(int i = 0; i < 26; i++){
        if(table[i].contains(top)){
            top = i + 'a';
            i = 0;
        }
    }

    cout << "Initial character: " << top << "\n";
    return top;
}
