#include <iostream>
#include <vector>
#include <string>
#include <cmath>
using namespace std;

string route_encrypt(const string& plain_text, int step_size) {
    int length = plain_text.length();
    int rows = ceil((double)length / step_size);
    vector<vector<char>> matrix(rows, vector<char>(step_size, '-'));
    int index = 0;

    for (int i = 0; i < rows && index < length; ++i) {
        for (int j = 0; j < step_size && index < length; ++j) {
            matrix[i][j] = plain_text[index++];
        }
    }

    string encrypted_text = "";
    int depth = min(step_size, rows) / 2;

    for (int i = 0; i < depth; ++i) {
        for (int j = i; j < rows - i - 1; ++j)
            encrypted_text += matrix[j][step_size - i - 1];

        for (int j = step_size - i - 1; j > i; --j)
            encrypted_text += matrix[rows - i - 1][j];

        for (int j = rows - i - 1; j > i; --j)
            encrypted_text += matrix[j][i];

        for (int j = i; j < step_size - i - 1; ++j)
            encrypted_text += matrix[i][j];
    }

    return encrypted_text;
}

string route_decrypt(const string& cipher_text, int step_size) {
    int length = cipher_text.length();
    int rows = ceil((double)length / step_size);
    vector<vector<char>> matrix(rows, vector<char>(step_size, 0));
    int index = 0;
    int depth = min(step_size, rows) / 2;

    for (int i = 0; i < depth && index < length; ++i) {
        for (int j = i; j < rows - i - 1 && index < length; ++j)
            matrix[j][step_size - i - 1] = cipher_text[index++];

        for (int j = step_size - i - 1; j > i && index < length; --j)
            matrix[rows - i - 1][j] = cipher_text[index++];

        for (int j = rows - i - 1; j > i && index < length; --j)
            matrix[j][i] = cipher_text[index++];

        for (int j = i; j < step_size - i - 1 && index < length; ++j)
            matrix[i][j] = cipher_text[index++];
    }

    string plain_text = "";
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < step_size; ++j)
            plain_text += matrix[i][j];

    return plain_text;
}

int main() {
    string input;
    int step;

    cout << "Enter plain text: ";
    getline(cin, input);

    cout << "Enter step size: ";
    cin >> step;

    string encrypted = route_encrypt(input, step);
    cout << "Encrypted text: " << encrypted << endl;

    string decrypted = route_decrypt(encrypted, step);
    cout << "Decrypted text: " << decrypted << endl;

    return 0;
}
