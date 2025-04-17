#include <iostream>
#include <string>
#include <vector>

class Running_key {
public:
    Running_key() {}

    std::string encode(std::string plain) {
        for (char &c : plain) {
            c = toupper(c);
        }
        
        if (plain.length() % 2 != 0) {
            plain += 'X';
        }

        int period = plain.length() / 2;
        std::string result = "";

        for (int i = 0; i < period; i++) {
            char k = plain[i];
            char p = plain[i + period];
            result += get_vigenere_char(k, p);
        }

        return result;
    }

    std::string decode(std::string cipher) {
        return ""; // Placeholder for decode function
    }

    bool key_need() {
        return false;
    }

    int get_key_num() {
        return 0;
    }

    std::vector<int> get_key_len() {
        return {}; // Placeholder for key length function
    }

    int process_id() {
        return 2;
    }

private:
    char get_vigenere_char(char k, char p) {
        return 'A'; // Placeholder for Vigenere cipher logic
    }
};

int main() {
    Running_key cipher;

    std::string input;
    std::cout << "Enter the plaintext: ";
    std::getline(std::cin, input);

    std::string encoded = cipher.encode(input);
    std::cout << "Encoded text: " << encoded << std::endl;

    return 0;
}
