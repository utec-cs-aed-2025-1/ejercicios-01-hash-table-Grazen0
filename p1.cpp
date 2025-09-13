#include "chainhash.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

vector<pair<string, string>> loadCSV(string file);

int main()
{
    vector<pair<string, string>> data = loadCSV("smalldata.csv");
    ChainHash<string, string> hash(13);

    for (size_t i = 0; i < data.size(); i++)
        hash.set(data[i].first, data[i].second);

    cout << "Size of the hash table:" << hash.bucket_count() << endl;

    for (int i = 0; i < hash.bucket_count(); i++) {
        cout << "Bucket #" << i << " contains " << hash.bucket_size(i)
             << " elements:";
        // usar el forward_list del stl
        for (auto it = hash.begin(i); it != hash.end(i); ++it)
            cout << "[" << (*it).key << ":" << (*it).value << "] ";
        cout << endl;
    }
    return 0;
}

vector<pair<string, string>> loadCSV(string file)
{
    vector<pair<string, string>> data;
    fstream fin;
    fin.open(file, ios::in);

    if (!fin.is_open()) {
        cerr << "Error: No se pudo abrir el archivo " << file << endl;
        return data;
    }

    string line;
    bool firstLine = true;

    while (getline(fin, line)) {
        if (firstLine) {
            firstLine = false;
            continue;
        }

        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        if (line.empty())
            continue;

        stringstream ss(line);
        string key, value;

        if (getline(ss, key, ';') && getline(ss, value, ';')) {
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);

            if (!key.empty() && !value.empty()) {
                data.push_back({key, value});
            }
        }
    }

    fin.close();
    return data;
}
