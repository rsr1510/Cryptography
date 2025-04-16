#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

string beaufortCipher(const string& text, const string& key) {
    string result = "";
    int keyLength = key.length();

    for (int i = 0; i < text.length(); ++i) {
        char textChar = toupper(text[i]);
        char keyChar = toupper(key[i % keyLength]);

        if (!isalpha(textChar)) {
            result += textChar; // Keep non-alphabet characters unchanged
            continue;
        }

        int textVal = textChar - 'A';
        int keyVal = keyChar - 'A';

        int cipherVal = (keyVal - textVal + 26) % 26;

        result += (cipherVal + 'A');
    }

    return result;
}

int main() {
    string text, key;
    int choice;

    cout << "===== Beaufort Cipher =====\n";
    cout << "1. Encrypt\n2. Decrypt\nChoose an option (1 or 2): ";
    cin >> choice;
    cin.ignore(); // Clear newline from input buffer

    cout << "Enter the text: ";
    getline(cin, text);
    cout << "Enter the key: ";
    cin >> key;

    // Convert to uppercase to ensure consistency
    transform(text.begin(), text.end(), text.begin(), ::toupper);
    transform(key.begin(), key.end(), key.begin(), ::toupper);

    string result = beaufortCipher(text, key);

    if (choice == 1)
        cout << "Encrypted Text: " << result << endl;
    else if (choice == 2)
        cout << "Decrypted Text: " << result << endl;
    else
        cout << "Invalid option." << endl;

    return 0;
}
