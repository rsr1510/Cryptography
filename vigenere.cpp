#include <bits/stdc++.h>
using namespace std;

string generateKey(string str, string key) {
    int x = str.size();
    for (int i = 0;; i++) {
        if (x == i)
            i = 0;
        if (key.size() == str.size())
            break;
        key.push_back(key[i]);
    }
    return key;
}

string cipherText(string str, string key) {
    string cipher_text;
    for (int i = 0; i < str.size(); i++) {
        char x = (str[i] + key[i]) % 26;
        x += 'A';
        cipher_text.push_back(x);
    }
    return cipher_text;
}

string originalText(string cipher_text, string key) {
    string orig_text;
    for (int i = 0; i < cipher_text.size(); i++) {
        char x = (cipher_text[i] - key[i] + 26) % 26;
        x += 'A';
        orig_text.push_back(x);
    }
    return orig_text;
}

int main() {
    string str, keyword;
    cout << "Enter the message (alphabets only): ";
    getline(cin, str);
    cout << "Enter the keyword: ";
    getline(cin, keyword);

    transform(str.begin(), str.end(), str.begin(), ::toupper);
    transform(keyword.begin(), keyword.end(), keyword.begin(), ::toupper);

    string key = generateKey(str, keyword);
    string cipher_text = cipherText(str, key);
    string decrypted_text = originalText(cipher_text, key);

    cout << "Ciphertext: " << cipher_text << endl;
    cout << "Decrypted Text: " << decrypted_text << endl;

    return 0;
}
