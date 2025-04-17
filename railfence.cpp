#include <iostream>
#include <string>
#include <vector>
#include <cctype>
using namespace std;

string preprocessText(const string& text) {
    string result;
    for (char c : text) {
        if (isalpha(c))
            result += toupper(c);
    }
    return result;
}

string encryptRailFence(string text, int key) {
    text = preprocessText(text);
    if (key <= 1 || text.empty()) return text;

    vector<string> rails(key);
    int row = 0;
    bool down = false;

    for (char c : text) {
        rails[row] += c;
        if (row == 0 || row == key - 1) down = !down;
        row += down ? 1 : -1;
    }

    string result;
    for (const string& rail : rails)
        result += rail;
    return result;
}

string decryptRailFence(string cipher, int key) {
    if (key <= 1 || cipher.empty()) return cipher;

    vector<vector<char>> rail(key, vector<char>(cipher.length(), '\n'));
    bool down = false;
    int row = 0;

    // Mark the rail matrix with '*'
    for (int col = 0; col < cipher.length(); col++) {
        rail[row][col] = '*';
        if (row == 0 || row == key - 1) down = !down;
        row += down ? 1 : -1;
    }

    // Fill the rail matrix with cipher text
    int index = 0;
    for (int i = 0; i < key; i++) {
        for (int j = 0; j < cipher.length(); j++) {
            if (rail[i][j] == '*' && index < cipher.length()) {
                rail[i][j] = cipher[index++];
            }
        }
    }

    // Read the matrix in zigzag order
    string result;
    row = 0;
    down = false;
    for (int col = 0; col < cipher.length(); col++) {
        result += rail[row][col];
        if (row == 0 || row == key - 1) down = !down;
        row += down ? 1 : -1;
    }

    return result;
}

int main() {
    string text;
    int key;

    cout << "Enter the plaintext: ";
    getline(cin, text);

    cout << "Enter the number of rails: ";
    cin >> key;

    string encrypted = encryptRailFence(text, key);
    cout << "\nEncrypted text (Rail-wise):\n";

    // Display rail-wise output like in the image
    string processed = preprocessText(text);
    vector<string> rails(key);
    int row = 0;
    bool down = false;
    for (char c : processed) {
        rails[row] += c;
        if (row == 0 || row == key - 1) down = !down;
        row += down ? 1 : -1;
    }
    for (const string& rail : rails)
        cout << rail << endl;

    cout << "\nFinal Encrypted Message: " << encrypted << endl;

    string decrypted = decryptRailFence(encrypted, key);
    cout << "Decrypted Message: " << decrypted << endl;

    return 0;
}
