#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
using namespace std;

string remove_spaces_and_upper(const string& input) {
    string cleaned;
    for (char c : input) {
        if (!isspace(c))
            cleaned += toupper(c);
    }
    return cleaned;
}

vector<int> generate_order(const string& key) {
    string key_u = key;
    transform(key_u.begin(), key_u.end(), key_u.begin(), ::toupper);
    vector<char> k_char(key_u.begin(), key_u.end());
    sort(k_char.begin(), k_char.end());

    vector<int> result(key.length(), 0);
    int cur_index = 1;
    int from_index = 0;

    for (int i = 0; i < key.length(); ++i) {
        if (i == 0 || k_char[i] == k_char[i - 1]) {
            size_t pos = key_u.find(k_char[i], from_index);
            from_index = pos + 1;
            result[pos] = cur_index;
        } else {
            cur_index++;
            from_index = 0;
            size_t pos = key_u.find(k_char[i], from_index);
            from_index = pos + 1;
            result[pos] = cur_index;
        }
    }
    return result;
}

vector<vector<char>> build_block(const string& text, int rows, int cols) {
    if (rows == -1)
        rows = (text.length() + cols - 1) / cols;

    vector<vector<char>> block(rows, vector<char>(cols, '\0'));
    int index = 0;

    for (int i = 0; i < rows && index < text.length(); ++i) {
        for (int j = 0; j < cols && index < text.length(); ++j) {
            block[i][j] = text[index++];
        }
    }

    return block;
}

vector<vector<char>> transp_block(const vector<vector<char>>& block, const vector<int>& num) {
    int row = block.size();
    int col = block[0].size();
    vector<vector<char>> result(row + 1, vector<char>(col, '\0'));

    int cur_num = 1, filled = 0, find_pos = 0;

    while (filled < col) {
        int pos = -1;
        for (int i = find_pos; i < num.size(); ++i) {
            if (num[i] == cur_num) {
                pos = i;
                break;
            }
        }

        if (pos == -1) {
            cur_num++;
            find_pos = 0;
            continue;
        } else {
            result[0][filled] = '0' + num[pos];
            for (int i = 0; i < row; ++i) {
                result[i + 1][filled] = block[i][pos];
            }
            filled++;
            find_pos = pos + 1;
        }
    }

    return result;
}

string read_col(const vector<vector<char>>& trans_block, int start_pos, int end_pos) {
    if (start_pos > end_pos) return "";
    string result;
    int row = trans_block.size();
    for (int i = 1; i < row; ++i) {
        for (int j = start_pos; j <= end_pos; ++j) {
            if (trans_block[i][j] != '\0') {
                result += trans_block[i][j];
            }
        }
    }
    return result;
}

string read_block(const vector<vector<char>>& trans_block) {
    int col = trans_block[0].size();
    int start_pos = 0, end_pos = 0;
    string result;

    while (end_pos < col) {
        if (trans_block[0][end_pos] == trans_block[0][start_pos]) {
            end_pos++;
        } else {
            result += read_col(trans_block, start_pos, end_pos - 1);
            start_pos = end_pos;
        }
    }

    result += read_col(trans_block, start_pos, end_pos - 1);
    return result;
}

string encode(string plain, string key) {
    plain = remove_spaces_and_upper(plain);
    key = remove_spaces_and_upper(key);

    vector<int> num_in = generate_order(key);
    auto block = build_block(plain, -1, key.length());
    auto trans_block = transp_block(block, num_in);
    string cipher_text = read_block(trans_block);
    return cipher_text;
}

int main() {
    string text, key;

    cout << "Enter the plaintext: ";
    getline(cin, text);

    cout << "Enter the key: ";
    getline(cin, key);

    string cipher = encode(text, key);
    cout << "Encoded text: " << cipher << endl;

    return 0;
}
