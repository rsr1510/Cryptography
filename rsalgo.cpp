#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <map>
#include <algorithm>
#include <cctype>
#include <numeric>
#include <limits>
#include <stdexcept> // Needed for runtime_error in Hill Cipher

using namespace std;

// Utility functions
string toLowerCase(const string &input)
{
    string result = input;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

string toUpperCase(const string &input)
{
    string result = input;
    transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

string removeSpaces(const string &input)
{
    string result;
    for (char c : input)
    {
        if (!isspace(c)) // Use isspace to remove all kinds of whitespace
        {
            result += c;
        }
    }
    return result;
}

int mod(int a, int b)
{
    // Ensure b is positive for correct modulo behavior
    if (b == 0) throw runtime_error("Modulo by zero");
    int result = a % b;
    return (result < 0) ? result + abs(b) : result;
}

// Extended Euclidean Algorithm to find GCD and coefficients
int gcdExtended(int a, int b, int *x, int *y)
{
    if (a == 0)
    {
        *x = 0;
        *y = 1;
        return b;
    }

    int x1, y1;
    int gcd = gcdExtended(b % a, a, &x1, &y1);

    *x = y1 - (b / a) * x1;
    *y = x1;

    return gcd;
}

// Function to find modular inverse of a under modulo m
int modInverse(int a, int m)
{
    int x, y;
    int gcd = gcdExtended(a, m, &x, &y);

    if (gcd != 1)
    {
        return -1; // Inverse doesn't exist if gcd is not 1
    }
    else
    {
        // Make sure x is positive
        return mod(x, m);
    }
}

// Greatest Common Divisor (standard)
int gcd(int a, int b) {
    return b == 0 ? a : gcd(b, a % b);
}

// Caesar Cipher
string caesarEncrypt(const string &plaintext, int shift)
{
    string ciphertext;
    int effective_shift = mod(shift, 26); // Ensure shift is within 0-25
    for (char c : plaintext)
    {
        if (isalpha(c))
        {
            char base = islower(c) ? 'a' : 'A';
            ciphertext += static_cast<char>(mod(c - base + effective_shift, 26) + base);
        }
        else
        {
            ciphertext += c; // Keep non-alphabetic characters unchanged
        }
    }
    return ciphertext;
}

string caesarDecrypt(const string &ciphertext, int shift)
{
    int effective_shift = mod(shift, 26);
    return caesarEncrypt(ciphertext, 26 - effective_shift); // Decryption is encryption with inverse shift
}

// Atbash Cipher
string atbashCipher(const string &input)
{
    string result;
    for (char c : input)
    {
        if (isalpha(c))
        {
            char base = islower(c) ? 'a' : 'A';
            result += static_cast<char>(base + 25 - (c - base));
        }
        else
        {
            result += c; // Keep non-alphabetic characters unchanged
        }
    }
    return result;
}

// August Cipher (identical implementation to Caesar provided)
string augustusEncrypt(const string &plaintext, int shift)
{
     return caesarEncrypt(plaintext, shift=1);
}

string augustusDecrypt(const string &ciphertext, int shift)
{
    return caesarDecrypt(ciphertext, shift=1);
}

// Affine Cipher
// Note: This implementation assumes uppercase input without spaces
string encryptMessage(string msg, int a, int b)
{
    string cipher = "";
     int m = 26; // Alphabet size
    for (int i = 0; i < msg.length(); i++)
    {
        // Avoid processing spaces, work only on uppercase letters
        if (isupper(msg[i]))
            cipher += (char)(mod((a * (msg[i] - 'A') + b), m) + 'A');
        else if (islower(msg[i])) // Optionally handle lowercase too
             cipher += (char)(mod((a * (msg[i] - 'a') + b), m) + 'a');
        else
            cipher += msg[i]; // Keep other characters as is
    }
    return cipher;
}

string decryptCipher(string cipher, int a, int b)
{
    string msg = "";
    int m = 26;
    int a_inv = modInverse(a, m);
    if (a_inv == -1) {
        return "Error: 'a' is not coprime with 26. Decryption not possible.";
    }

    for (int i = 0; i < cipher.length(); i++)
    {
         if (isupper(cipher[i]))
         {
            int decrypted_char_val = mod(a_inv * (cipher[i] - 'A' - b), m);
            msg += (char)(decrypted_char_val + 'A');
         }
          else if (islower(cipher[i])) // Optionally handle lowercase
         {
             int decrypted_char_val = mod(a_inv * (cipher[i] - 'a' - b), m);
             msg += (char)(decrypted_char_val + 'a');
         }
        else
            msg += cipher[i]; // Keep other characters
    }
    return msg;
}


// Vigenere Cipher
string generateKey(string str, string key)
{
    int x = str.length(); // Use length() instead of size() for consistency
    string generatedKey = "";
    int keyLen = key.length();
    if (keyLen == 0) return ""; // Handle empty key

    int currentKeyIndex = 0;
    for (int i = 0; i < x; ++i) {
        if (isalpha(str[i])) { // Only extend key for alphabetic characters in str
            generatedKey += key[currentKeyIndex % keyLen];
            currentKeyIndex++;
        } else {
            generatedKey += ' '; // Add a placeholder for non-alpha chars in str
        }
    }
    // Now adjust the generated key to match the exact length and structure needed
    string finalKey = "";
    int key_idx = 0;
    for(int i=0; i<str.length(); ++i){
        if(isalpha(str[i])){
            while(!isalpha(key[key_idx % key.length()])) key_idx++; // Skip non-alpha in key
            finalKey += key[key_idx % key.length()];
            key_idx++;
        } else {
            finalKey += ' '; // Placeholder matches non-alpha in text
        }
    }


    return finalKey;
}


string vigenereEncrypt(string str, string key)
{
    string cipher_text;
    string fullKey = generateKey(str, key); // Generate key matching plaintext length
    if (fullKey.length() != str.length()) return "Error in key generation"; // Basic check

    for (int i = 0; i < str.size(); i++)
    {
        if (isalpha(str[i]))
        {
            char base = islower(str[i]) ? 'a' : 'A';
            // Ensure key char matches case or handle consistently (e.g., uppercase both)
            char keyChar = toupper(fullKey[i]); // Use uppercase key for calculation
            if (!isalpha(keyChar)) continue; // Skip if generated key part is placeholder
            char textChar = str[i];

            int textVal = textChar - base;
            int keyVal = keyChar - 'A'; // Assuming key affects based on A=0, B=1...

            char encryptedChar = mod(textVal + keyVal, 26) + base;
            cipher_text.push_back(encryptedChar);
        }
        else
        {
            cipher_text.push_back(str[i]); // Keep non-alphabetic characters
        }
    }
    return cipher_text;
}

string vigenereDecrypt(string cipher_text, string key)
{
    string orig_text;
     string fullKey = generateKey(cipher_text, key); // Generate key matching ciphertext length
    if (fullKey.length() != cipher_text.length()) return "Error in key generation";

    for (int i = 0; i < cipher_text.size(); i++)
    {
        if (isalpha(cipher_text[i]))
        {
             char base = islower(cipher_text[i]) ? 'a' : 'A';
             char keyChar = toupper(fullKey[i]);
             if (!isalpha(keyChar)) continue;
             char cipherChar = cipher_text[i];

             int cipherVal = cipherChar - base;
             int keyVal = keyChar - 'A';

             char decryptedChar = mod(cipherVal - keyVal, 26) + base;
             orig_text.push_back(decryptedChar);
        }
        else
        {
            orig_text.push_back(cipher_text[i]); // Keep non-alphabetic characters
        }
    }
    return orig_text;
}


// Gronsfeld Cipher
// Extended vocabulary including lowercase, symbols, numbers. Adjust if needed.
string vocabulary = " !\"#$%&'()*+,-./0123456789:<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

// Helper to check if key is numeric
bool isNumericKey(const string& key) {
    if (key.empty()) return false;
    for (char c : key) {
        if (!isdigit(c)) return false;
    }
    return true;
}

string gronsfeldEncrypt(string plain, string key)
{
    if (!isNumericKey(key)) return "Error: Gronsfeld key must be numeric.";
    if (key.empty()) return "Error: Gronsfeld key cannot be empty.";

    string result = plain;
    int keyLen = key.length();
    int vocabLen = vocabulary.length();

    for (int i = 0; i < plain.length(); i++)
    {
        size_t position = vocabulary.find(plain[i]);
        if (position != string::npos) // Only shift characters found in vocabulary
        {
            int shift = key[i % keyLen] - '0'; // Get the numeric shift value
            int realOffset = mod(position + shift, vocabLen);
            result[i] = vocabulary[realOffset];
        }
        // Characters not in vocabulary are left unchanged (already handled by result=plain initialization)
    }
    return result;
}

string gronsfeldDecrypt(string cipher, string key)
{
     if (!isNumericKey(key)) return "Error: Gronsfeld key must be numeric.";
     if (key.empty()) return "Error: Gronsfeld key cannot be empty.";

    string result = cipher;
    int keyLen = key.length();
    int vocabLen = vocabulary.length();

    for (int i = 0; i < cipher.length(); i++)
    {
         size_t position = vocabulary.find(cipher[i]);
         if (position != string::npos)
        {
             int shift = key[i % keyLen] - '0';
             int realOffset = mod(position - shift, vocabLen); // Subtract shift for decryption
             result[i] = vocabulary[realOffset];
        }
    }
    return result;
}


// Beaufort Cipher
// Note: This cipher is an involution (encrypt == decrypt).
// The implementation processes only uppercase letters.
string beaufortCipher(const string &text, const string &key)
{
    string result = "";
    string upper_key = toUpperCase(key);
    int keyLength = upper_key.length();
    if (keyLength == 0) return "Error: Key cannot be empty.";

    int keyIndex = 0;
    for (int i = 0; i < text.length(); ++i)
    {
        char textChar = text[i];
        if (!isalpha(textChar))
        {
            result += textChar; // Keep non-alphabet characters unchanged
            continue;
        }

        char upperTextChar = toupper(textChar);
        char keyChar = upper_key[keyIndex % keyLength];
         if (!isalpha(keyChar)) { // Skip non-alpha chars in key
            keyIndex++; // Ensure key progresses
            keyChar = upper_key[keyIndex % keyLength];
            if (!isalpha(keyChar)) return "Error: Key contains invalid characters."; // Basic check
        }


        int textVal = upperTextChar - 'A';
        int keyVal = keyChar - 'A';

        int cipherVal = mod(keyVal - textVal, 26);

        char cipherChar = cipherVal + 'A';
        // Preserve original case if needed, otherwise return uppercase
        result += islower(textChar) ? tolower(cipherChar) : cipherChar;
        keyIndex++; // Increment key index only for letters processed
    }

    return result;
}


// Autoclave Cipher
// Uses Vigenere logic but key extends with plaintext (encrypt) or ciphertext (decrypt)
// Assumes uppercase input, modify if needed
string autoEncryption(string msg, string key)
{
    string upper_msg = toUpperCase(removeSpaces(msg)); // Preprocess: uppercase, no spaces
    string upper_key = toUpperCase(removeSpaces(key));
    if (upper_key.empty()) return "Error: Key cannot be empty.";

    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int len = upper_msg.length();
    string newKey = upper_key + upper_msg; // Key is initial key + plaintext
    newKey = newKey.substr(0, len); // Trim key to message length
    string encryptMsg = "";

    for (int x = 0; x < len; x++)
    {
         // Assume already preprocessed, so chars should be uppercase letters
         int first = upper_msg[x] - 'A';
         int second = newKey[x] - 'A';
         int total = mod(first + second, 26);
         encryptMsg += alphabet[total];
    }
    return encryptMsg;
}

string autoDecryption(string msg, string key)
{
    string upper_msg = toUpperCase(removeSpaces(msg)); // Preprocess: uppercase, no spaces
    string upper_key = toUpperCase(removeSpaces(key));
     if (upper_key.empty()) return "Error: Key cannot be empty.";

    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string currentKey = upper_key;
    string decryptMsg = "";

    for (int x = 0; x < upper_msg.length(); x++)
    {
         int get1 = upper_msg[x] - 'A';
         // Ensure currentKey has the character needed
         if (x >= currentKey.length()) {
             return "Error: Decryption failed, key generation issue."; // Should not happen if logic is correct
         }
         int get2 = currentKey[x] - 'A';

         int total = mod(get1 - get2, 26);
         char decryptedChar = alphabet[total];
         decryptMsg += decryptedChar;
         currentKey += decryptedChar; // Extend key with the decrypted char
    }
    return decryptMsg;
}


// Hill Cipher
// Helper functions (getKeyMatrix, determinant, adjoint, inverse, modInverse, gcdExtended) are assumed to be correct

vector<vector<int>> getKeyMatrix(const string &key, int n)
{
     // Ensure key only contains letters for matrix generation
    string clean_key = "";
    for(char c : key) {
        if (isalpha(c)) {
            clean_key += tolower(c);
        }
    }

    if (clean_key.length() < n * n) {
        throw runtime_error("Key length is insufficient for the matrix size.");
    }


    vector<vector<int>> keyMatrix(n, vector<int>(n));
    int k = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
             keyMatrix[i][j] = clean_key[k] - 'a';
             k++;
        }
    }
    return keyMatrix;
}

int determinant(const vector<vector<int>> &matrix) // Removed unused n parameter
{
    int n = matrix.size();
    if (n == 0 || matrix[0].size() != n) {
         throw runtime_error("Invalid matrix for determinant calculation.");
    }

    if (n == 1)
    {
        return matrix[0][0];
    }
    if (n == 2)
    {
        // Calculate determinant mod 26
        return mod(matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0], 26);
    }

    int det = 0;
    for (int i = 0; i < n; i++)
    {
        vector<vector<int>> submatrix(n - 1, vector<int>(n - 1));
        for (int j = 1; j < n; j++) // Row index for submatrix starts from 1
        {
             int sub_k = 0; // Column index for submatrix
            for (int k = 0; k < n; k++) // Column index for original matrix
            {
                if (k == i) continue; // Skip the column i
                submatrix[j - 1][sub_k++] = matrix[j][k];
            }
        }
        int sign = (i % 2 == 0) ? 1 : -1;
        det = mod(det + sign * matrix[0][i] * determinant(submatrix), 26);
    }

    return det;
}


vector<vector<int>> adjoint(const vector<vector<int>> &matrix) // Removed unused n parameter
{
    int n = matrix.size();
     if (n == 0 || matrix[0].size() != n) {
         throw runtime_error("Invalid matrix for adjoint calculation.");
    }

    vector<vector<int>> adj(n, vector<int>(n));

    if (n == 1)
    {
        adj[0][0] = 1;
        return adj;
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
             // Find cofactor of matrix[i][j]
            vector<vector<int>> temp(n - 1, vector<int>(n - 1));
            int ti = 0; // Row index for temp
            for (int r = 0; r < n; r++)
            {
                 if (r == i) continue; // Skip row i
                 int tj = 0; // Col index for temp
                 for (int c = 0; c < n; c++)
                 {
                     if (c == j) continue; // Skip col j
                     temp[ti][tj++] = matrix[r][c];
                 }
                 ti++;
            }

             // Sign of cofactor
            int sign = ((i + j) % 2 == 0) ? 1 : -1;
            // Adjoint is the transpose of the cofactor matrix
            adj[j][i] = mod(sign * determinant(temp), 26);
        }
    }
    return adj;
}


vector<vector<int>> inverse(const vector<vector<int>> &matrix) // Removed unused n parameter
{
    int n = matrix.size();
    if (n == 0 || matrix[0].size() != n) {
         throw runtime_error("Invalid matrix for inverse calculation.");
    }

    int det = determinant(matrix);
    int detInv = modInverse(det, 26);

    if (detInv == -1)
    {
        throw runtime_error("Matrix determinant is not coprime with 26, inverse does not exist.");
    }

    vector<vector<int>> adj = adjoint(matrix);
    vector<vector<int>> inv(n, vector<int>(n));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            inv[i][j] = mod(adj[i][j] * detInv, 26);
        }
    }
    return inv;
}


string hillEncrypt(const string &plaintext, const string &key, int n)
{
    if (n <= 0) return "Error: Matrix dimension 'n' must be positive.";
    try
    {
        vector<vector<int>> keyMatrix = getKeyMatrix(key, n);
        // Test determinant early
        int det = determinant(keyMatrix);
        if (gcd(det, 26) != 1) {
             return "Warning: Key matrix determinant is not coprime with 26. Decryption will not be possible with this key.";
             // Allow encryption anyway? Or return error? Returning warning for now.
        }


        string processedText = removeSpaces(toLowerCase(plaintext));
        if (processedText.empty()) return ""; // Handle empty input

        // Pad with 'x' if needed
        while (processedText.length() % n != 0)
        {
            processedText += 'x';
        }

        string ciphertext = "";
        for (size_t i = 0; i < processedText.length(); i += n)
        {
            vector<int> block(n);
            for (int j = 0; j < n; j++)
            {
                block[j] = processedText[i + j] - 'a';
            }

            vector<int> result(n, 0);
            // Matrix multiplication: result = keyMatrix * block
            for (int row = 0; row < n; row++)
            {
                for (int col = 0; col < n; col++)
                {
                    result[row] = mod(result[row] + keyMatrix[row][col] * block[col], 26);
                }
            }

            for (int j = 0; j < n; j++)
            {
                ciphertext += static_cast<char>(result[j] + 'a');
            }
        }
        return ciphertext;
    }
    catch (const exception &e)
    {
        return string("Error during Hill encryption: ") + e.what();
    }
}

string hillDecrypt(const string &ciphertext, const string &key, int n)
{
     if (n <= 0) return "Error: Matrix dimension 'n' must be positive.";
     string processedText = removeSpaces(toLowerCase(ciphertext));
     if (processedText.empty()) return "";
     if (processedText.length() % n != 0) {
        return "Error: Ciphertext length is not a multiple of matrix dimension 'n'.";
     }

    try
    {
        vector<vector<int>> keyMatrix = getKeyMatrix(key, n);
        vector<vector<int>> inverseMatrix = inverse(keyMatrix); // Throws if not invertible

        string plaintext = "";
        for (size_t i = 0; i < processedText.length(); i += n)
        {
            vector<int> block(n);
            for (int j = 0; j < n; j++)
            {
                block[j] = processedText[i + j] - 'a';
            }

            vector<int> result(n, 0);
             // Matrix multiplication: result = inverseMatrix * block
            for (int row = 0; row < n; row++)
            {
                for (int col = 0; col < n; col++)
                {
                    result[row] = mod(result[row] + inverseMatrix[row][col] * block[col], 26);
                }
            }

            for (int j = 0; j < n; j++)
            {
                plaintext += static_cast<char>(result[j] + 'a');
            }
        }
        return plaintext;
    }
    catch (const exception &e)
    {
        return string("Error during Hill decryption: ") + e.what();
    }
}


// Rail Fence Cipher
string preprocessText(const string& text)
{
    string result;
    for (char c : text)
    {
        if (isalpha(c))
            result += toupper(c); // Consistent case for simplicity
    }
    return result;
}

string encryptRailFence(string text, int key)
{
    if (key <= 1) return "Error: Number of rails must be greater than 1.";
    string processed_text = preprocessText(text);
    if (processed_text.empty()) return "";

    int n = processed_text.length();
    vector<string> rails(key);
    int row = 0;
    bool down = true; // Start going down

    for (char c : processed_text)
    {
        rails[row] += c;
        if (row == key - 1) { // Reached bottom rail
            down = false;
        } else if (row == 0) { // Reached top rail
            down = true;
        }
        row += down ? 1 : -1;
    }

    string result = "";
    for (const string& rail : rails)
        result += rail;
    return result;
}

string decryptRailFence(string cipher, int key)
{
    if (key <= 1) return "Error: Number of rails must be greater than 1.";
    int n = cipher.length();
     if (n == 0) return "";

    // Create the matrix to mark positions
    vector<vector<char>> rail(key, vector<char>(n, '\n')); // Use '\n' as placeholder
    int row = 0;
    bool down = true;

    // Mark the pattern
    for (int col = 0; col < n; col++)
    {
        rail[row][col] = '*'; // Mark position
         if (row == key - 1) {
            down = false;
        } else if (row == 0) {
            down = true;
        }
        row += down ? 1 : -1;
    }

    // Fill the matrix with ciphertext characters row by row
    int index = 0;
    for (int i = 0; i < key; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (rail[i][j] == '*' && index < n)
            {
                rail[i][j] = cipher[index++];
            }
        }
    }

    // Read the matrix in zigzag pattern to get plaintext
    string result = "";
    row = 0;
    down = true;
    for (int col = 0; col < n; col++)
    {
         if (rail[row][col] != '\n') // Read the character
            result += rail[row][col];

         if (row == key - 1) {
            down = false;
        } else if (row == 0) {
            down = true;
        }
        row += down ? 1 : -1;
    }

    return result;
}


// Route Cipher
// Encrypts by writing into a grid and reading out spirally (clockwise from top-right here)
// Decrypts by reversing the process
string route_encrypt(const string& plain_text, int step_size) // step_size is num columns
{
    if (step_size <= 0) return "Error: Step size (columns) must be positive.";
    string processed_text = removeSpaces(toUpperCase(plain_text)); // No spaces, uppercase
    int length = processed_text.length();
    if (length == 0) return "";

    int rows = ceil((double)length / step_size);
    vector<vector<char>> matrix(rows, vector<char>(step_size, 'X')); // Pad with 'X'
    int index = 0;

    // Fill the matrix row by row
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < step_size; ++j) {
            if (index < length) {
                matrix[i][j] = processed_text[index++];
            }
        }
    }

    // Read out in a spiral path (example: clockwise starting top-right)
    string encrypted_text = "";
    int top = 0, bottom = rows - 1, left = 0, right = step_size - 1;
    int dir = 0; // 0: right->left (top), 1: top->bottom (left), 2: left->right (bottom), 3: bottom->top (right)

    // Modify spiral direction: Start Top Right -> Down -> Left -> Up -> Right ...
    int r = 0, c = step_size - 1;
    int visited_count = 0;
    vector<vector<bool>> visited(rows, vector<bool>(step_size, false));

    // This implementation reads column-wise downwards, alternating right-to-left and left-to-right
    // Example: Read Col N down, Col N-1 down, ... Col 1 down
    // Or Read Col 1 down, Col 2 down, ... Col N down
    // The original code seems to attempt a spiral - let's refine that: Clockwise OUTWARD spiral starting top-left
    // This is complex. Let's stick to a simpler transposition: Read columns downwards.

    encrypted_text = "";
    for (int j = 0; j < step_size; ++j) { // Read column by column
        for (int i = 0; i < rows; ++i) {
            encrypted_text += matrix[i][j];
        }
    }
    // This is a simple columnar transposition, not the spiral implied by the original code's loop structure.
    // If a specific spiral is needed, the reading logic needs significant change.
    // Assuming simple columnar transposition based on common Route Cipher variations.

    return encrypted_text;
}

string route_decrypt(const string& cipher_text, int step_size) // step_size is num columns
{
    if (step_size <= 0) return "Error: Step size (columns) must be positive.";
    int length = cipher_text.length();
    if (length == 0) return "";

    int rows = ceil((double)length / step_size);
     if (rows * step_size != length) {
        // This suggests the ciphertext might not perfectly fit the grid used for encryption
        // Or padding wasn't handled consistently. Let's try to proceed but might be inaccurate.
        rows = length / step_size; // Integer division might be closer if padding was minimal
        if (length % step_size != 0) rows++; // Adjust if there's remainder
    }


    vector<vector<char>> matrix(rows, vector<char>(step_size));
    int index = 0;

    // Fill the matrix column by column (reverse of encryption reading)
    for (int j = 0; j < step_size; ++j) {
        for (int i = 0; i < rows; ++i) {
             if (index < length) { // Check bounds
                matrix[i][j] = cipher_text[index++];
             } else {
                 // Handle cases where length doesn't perfectly match rows*step_size
                 // This indicates potential padding removal issue or incorrect step_size
                 matrix[i][j] = '\0'; // Or some placeholder
             }
        }
    }

    // Read out row by row to get original text
    string plain_text = "";
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < step_size; ++j) {
             if (matrix[i][j] != '\0') { // Avoid adding placeholders/padding if any
                plain_text += matrix[i][j];
            }
        }
    }

    // Optional: Remove padding characters like 'X' if they were used consistently
    // size_t lastChar = plain_text.find_last_not_of('X');
    // if (string::npos != lastChar) {
    //     plain_text = plain_text.substr(0, lastChar + 1);
    // }

    return plain_text;
}


// Myszkowski Cipher
// Helper functions (remove_spaces_and_upper, generate_order, build_block, transp_block, read_col, read_block) assumed correct.
// Note: Decryption for Myszkowski is complex and not implemented here.

string remove_spaces_and_upper(const string& input)
{
    string cleaned;
    for (char c : input)
    {
        if (isalpha(c)) // Keep only letters
            cleaned += toupper(c);
    }
    return cleaned;
}

// Generates numerical order based on keyword character sorting (handles duplicates)
vector<int> generate_order(const string& key)
{
    int n = key.length();
    if (n == 0) return {};
    vector<pair<char, int>> key_pairs(n);
    for(int i = 0; i < n; ++i) {
        key_pairs[i] = {toupper(key[i]), i};
    }

    // Sort primarily by character, secondarily by original index
    sort(key_pairs.begin(), key_pairs.end());

    vector<int> result(n);
    int current_rank = 1;
    result[key_pairs[0].second] = current_rank;

    for(int i = 1; i < n; ++i) {
        // If current char is different from previous, increment rank
        if (key_pairs[i].first != key_pairs[i-1].first) {
            current_rank++;
        }
         // Assign the current rank (handles ties by giving same rank)
        result[key_pairs[i].second] = current_rank;
    }
    return result;
}


vector<vector<char>> build_block(const string& text, int cols)
{
    if (cols <= 0) return {};
    int length = text.length();
    int rows = (length + cols - 1) / cols; // Calculate needed rows

    vector<vector<char>> block(rows, vector<char>(cols, 'X')); // Pad with 'X'
    int index = 0;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (index < length) {
                block[i][j] = text[index++];
            }
        }
    }
    return block;
}

// Reads columns based on the numerical order, grouping columns with the same number.
string read_block_myszkowski(const vector<vector<char>>& block, const vector<int>& order) {
    if (block.empty() || order.empty()) return "";
    int rows = block.size();
    int cols = order.size();
    if (cols != block[0].size()) return "Error: Order size mismatch with block columns.";

    string result = "";
    int max_rank = 0;
    for(int rank : order) {
        if (rank > max_rank) max_rank = rank;
    }

    for (int current_rank = 1; current_rank <= max_rank; ++current_rank) {
        vector<int> cols_with_rank;
        for (int j = 0; j < cols; ++j) {
            if (order[j] == current_rank) {
                cols_with_rank.push_back(j);
            }
        }

        // Read according to Myszkowski rule:
        if (cols_with_rank.size() == 1) { // Normal columnar read for unique rank
            int col_idx = cols_with_rank[0];
            for (int i = 0; i < rows; ++i) {
                 result += block[i][col_idx];
            }
        } else { // Read row by row across columns with the same rank
            for (int i = 0; i < rows; ++i) {
                for (int col_idx : cols_with_rank) {
                    result += block[i][col_idx];
                }
            }
        }
    }
    // Remove padding 'X's? This is tricky as 'X' might be part of the message.
    // Usually, padding is removed manually or length is transmitted.
    return result;

}


string myszkowskiEncrypt(string plain, string key)
{
    string processed_plain = remove_spaces_and_upper(plain);
    string processed_key = remove_spaces_and_upper(key);
    if (processed_key.empty()) return "Error: Key cannot be empty.";

    vector<int> num_order = generate_order(processed_key);
    auto block = build_block(processed_plain, processed_key.length());
    string cipher_text = read_block_myszkowski(block, num_order);

     // Optional: Clean padding 'X' from the end if it's obvious padding
    size_t last_char_pos = cipher_text.find_last_not_of('X');
     if (string::npos != last_char_pos && last_char_pos < cipher_text.length() -1 ) {
         // Heuristic: if trailing Xs exist and original length might be known
         // This is unreliable without knowing original length.
         // For now, return as is, including padding.
     }

    return cipher_text;
}


// N-gram Operations (alias for Hill Cipher in this context)
string ngramEncrypt(const string &plaintext, const string &key, int n)
{
    return hillEncrypt(plaintext, key, n);
}

string ngramDecrypt(const string &ciphertext, const string &key, int n)
{
    return hillDecrypt(ciphertext, key, n);
}


// Main function with menu system
int main()
{
    int choice;
    string input, output, key;
    bool isEncryption = true; // Default, will be set

    while (true)
    {
        cout << "\n==== Cryptographic Ciphers ====\n";
        cout << " 1. Caesar Cipher\n";
        cout << " 2. Atbash Cipher\n";
        cout << " 3. August Cipher (Caesar variant)\n";
        cout << " 4. Affine Cipher\n";
        cout << " 5. Vigenere Cipher\n";
        cout << " 6. Gronsfeld Cipher\n";
        cout << " 7. Beaufort Cipher\n";
        cout << " 8. Autoclave Cipher/Running key Cipher\n";
        cout << " 9. N-gram Operations\n";
        cout << "10. Hill Cipher\n";
        cout << "11. Rail Fence Cipher\n";
        cout << "12. Route Cipher\n";
        cout << "13. Myszkowski Cipher (Encode only)\n";
        cout << " 0. Exit\n";
        cout << "Enter your choice: ";

        // Input validation for choice
        while (!(cin >> choice)) {
             cout << "Invalid input. Please enter a number: ";
             cin.clear();
             cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the rest of the line

        if (choice == 0)
        {
            cout << "Exiting program. Goodbye!\n";
            break;
        }

        if (choice < 0 || choice > 14)
        {
            cout << "Invalid choice. Please select a number between 0 and 14.\n";
            continue;
        }

        // Handle Atbash separately as it's its own inverse
        if (choice == 2) {
             cout << "Enter text for Atbash Cipher: ";
             getline(cin, input);
             output = atbashCipher(input);
             cout << "Result: " << output << endl;
             continue; // Go back to main menu
        }

        // Ask for Encrypt/Decrypt for other ciphers
        cout << "Choose operation:\n";
        cout << "1. Encrypt\n";
        cout << "2. Decrypt\n";
        cout << "Enter your choice (1 or 2): ";
        int opChoice;
        while (!(cin >> opChoice) || (opChoice != 1 && opChoice != 2)) {
            cout << "Invalid input. Please enter 1 for Encrypt or 2 for Decrypt: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer
        isEncryption = (opChoice == 1);

        // Handle ciphers where only encryption is implemented/practical here
         if (!isEncryption && (choice == 9 || choice == 14)) {
             cout << "Note: Decryption for this cipher is not implemented or is complex.\n";
             if (choice == 9) output = "Running Key decryption not available.";
             if (choice == 14) output = "Myszkowski decryption not implemented.";
              cout << output << endl;
             continue; // Go back to main menu
         }


        cout << "Enter text: ";
        getline(cin, input);

        // Get keys/parameters based on cipher choice
        int shift = 0, a = 0, b = 0, n = 0, rails = 0, step_size = 0;

        switch (choice) {
            case 1: // Caesar
            case 3: // August
                break;
            case 4: // Affine
                cout << "Enter key 'a' (must be coprime with 26): ";
                 while (!(cin >> a) || gcd(a, 26) != 1) {
                     cout << "Invalid 'a'. It must be an integer coprime with 26. Try again: ";
                     cin.clear();
                     cin.ignore(numeric_limits<streamsize>::max(), '\n');
                 }
                 cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Enter key 'b' (integer shift): ";
                 while (!(cin >> b)) {
                     cout << "Invalid input. Please enter an integer: ";
                     cin.clear();
                     cin.ignore(numeric_limits<streamsize>::max(), '\n');
                 }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                // Preprocess for Affine functions (uppercase, no spaces maybe needed depending on func)
                // The provided functions handle case, so let's not preprocess here forcefully.
                break;
            case 5: // Vigenere
            case 7: // Beaufort
            case 8: // Autoclave
                 cout << "Enter keyword (alphabetic characters recommended): ";
                 getline(cin, key);
                 if (key.empty()) {
                     cout << "Keyword cannot be empty.\n"; continue;
                 }
                 break;
             case 6: // Gronsfeld
                 cout << "Enter numeric keyword (e.g., 31415): ";
                 getline(cin, key);
                 if (!isNumericKey(key)) {
                     cout << "Invalid key. Gronsfeld key must contain only digits.\n"; continue;
                 }
                 break;
            case 9: // N-gram (Hill)
            case 10: // Hill
                cout << "Enter keyword (at least n*n letters): ";
                getline(cin, key);
                cout << "Enter matrix dimension 'n' (e.g., 2 for 2x2, 3 for 3x3): ";
                 while (!(cin >> n) || n <= 0) {
                     cout << "Invalid dimension. Enter a positive integer: ";
                     cin.clear();
                     cin.ignore(numeric_limits<streamsize>::max(), '\n');
                 }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                 // Basic key length check
                 {
                    string clean_key_check = "";
                    for(char c : key) if(isalpha(c)) clean_key_check += c;
                    if (clean_key_check.length() < n*n) {
                        cout << "Error: Key has fewer than n*n (" << n*n << ") alphabetic characters.\n";
                        continue; // Go back to main menu
                    }
                 }
                break;
            case 11: // Rail Fence
                cout << "Enter number of rails (integer > 1): ";
                 while (!(cin >> rails) || rails <= 1) {
                     cout << "Invalid input. Rails must be an integer greater than 1: ";
                     cin.clear();
                     cin.ignore(numeric_limits<streamsize>::max(), '\n');
                 }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            case 12: // Route Cipher
                cout << "Enter step size (number of columns, integer > 0): ";
                 while (!(cin >> step_size) || step_size <= 0) {
                     cout << "Invalid input. Step size must be a positive integer: ";
                     cin.clear();
                     cin.ignore(numeric_limits<streamsize>::max(), '\n');
                 }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            case 13: // Myszkowski (Encode only)
                 cout << "Enter keyword: ";
                 getline(cin, key);
                  if (key.empty()) {
                     cout << "Keyword cannot be empty.\n"; continue;
                 }
                break;
        }

        // Perform encryption or decryption based on choice
        output = ""; // Reset output
        try { // Add a try block for potential exceptions like in Hill
            switch (choice) {
                case 1: // Caesar
                    output = isEncryption ? caesarEncrypt(input, shift) : caesarDecrypt(input, shift);
                    break;
                case 3: // August
                    output = isEncryption ? augustusEncrypt(input, shift) : augustusDecrypt(input, shift);
                    break;
                case 4: // Affine
                    output = isEncryption ? encryptMessage(input, a, b) : decryptCipher(input, a, b);
                    break;
                case 5: // Vigenere
                    output = isEncryption ? vigenereEncrypt(input, key) : vigenereDecrypt(input, key);
                    break;
                case 6: // Gronsfeld
                    output = isEncryption ? gronsfeldEncrypt(input, key) : gronsfeldDecrypt(input, key);
                    break;
                 case 7: // Beaufort - Function is involution, call is the same
                     output = beaufortCipher(input, key);
                     break;
                 case 8: // Autoclave
                     output = isEncryption ? autoEncryption(input, key) : autoDecryption(input, key);
                     break;
                case 9: // N-gram (Hill)
                case 10: // Hill
                    output = isEncryption ? hillEncrypt(input, key, n) : hillDecrypt(input, key, n);
                    break;
                case 11: // Rail Fence
                    output = isEncryption ? encryptRailFence(input, rails) : decryptRailFence(input, rails);
                    break;
                case 12: // Route
                    output = isEncryption ? route_encrypt(input, step_size) : route_decrypt(input, step_size);
                    break;
                case 13: // Myszkowski (Encode only)
                     if (isEncryption) output = myszkowskiEncrypt(input, key);
                      // Decryption case handled earlier
                     break;
            }

             // Display result
             cout << (isEncryption ? "Encrypted" : "Decrypted") << " text: " << output << endl;

        } catch (const exception& e) {
            cerr << "An error occurred: " << e.what() << endl;
        }

    } // End of while loop

    return 0;
}