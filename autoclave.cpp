#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

using namespace std;

string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

string toUpper(string text) {
    transform(text.begin(), text.end(), text.begin(), ::toupper);
    return text;
}

string autoEncryption(string msg, string key) {
    msg = toUpper(msg);
    key = toUpper(key);
    int len = msg.length();
    string newKey = key + msg;
    newKey = newKey.substr(0, newKey.length() - key.length());
    string encryptMsg = "";

    for (int x = 0; x < len; x++) {
        int first = alphabet.find(msg[x]);
        int second = alphabet.find(newKey[x]);
        int total = (first + second) % 26;
        encryptMsg += alphabet[total];
    }
    return encryptMsg;
}

string autoDecryption(string msg, string key) {
    msg = toUpper(msg);
    key = toUpper(key);
    string currentKey = key;
    string decryptMsg = "";

    for (int x = 0; x < msg.length(); x++) {
        int get1 = alphabet.find(msg[x]);
        int get2 = alphabet.find(currentKey[x]);
        int total = (get1 - get2) % 26;
        total = (total < 0) ? total + 26 : total;
        decryptMsg += alphabet[total];
        currentKey += alphabet[total];
    }
    return decryptMsg;
}

int main() {
    string msg, key;

    cout << "Enter the plaintext: ";
    cin >> msg;

    cout << "Enter the key (single letter): ";
    cin >> key;

    string enc = autoEncryption(msg, key);

    cout << "\nPlaintext : " << toUpper(msg) << endl;
    cout << "Encrypted : " << enc << endl;
    cout << "Decrypted : " << autoDecryption(enc, key) << endl;

    return 0;
}
