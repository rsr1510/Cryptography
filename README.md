# Cryptographic Ciphers Implementation (All 13 algorithms implemented in a single code, chosen with menus)

This C++ program implements 13 different classical cryptographic ciphers with both encryption and decryption functionality. The program provides a user-friendly menu interface for selecting and using each cipher.

## Features

* Interactive menu system for cipher selection
* Support for both encryption and decryption


## Implemented Ciphers

1. **Caesar Cipher**
   * A substitution cipher where each letter is shifted by a fixed number of positions in the alphabet.
   * Parameters: Shift value (0-25)

2. **Atbash Cipher**
   * A substitution cipher that maps each letter to its reverse in the alphabet (A→Z, B→Y, etc.).
   * No additional parameters required.

3. **Augustus Cipher**
   * A combination of a keyword substitution and Caesar shift.
   * Parameters: Keyword, shift value (0-25)

4. **Affine Cipher**
   * Uses a mathematical function (ax + b) mod 26 for encryption.
   * Parameters: Coefficients 'a' (must be coprime with 26) and 'b' (0-25)

5. **Vigenere Cipher**
   * Uses a repeating keyword to determine variable shift values for each letter.
   * Parameters: Keyword (alphabetic)

6. **Gronsfeld Cipher**
   * Similar to Vigenere but uses numeric digits for the key.
   * Parameters: Numeric key (digits only)

7. **Beaufort Cipher**
   * A reciprocal cipher using a keyword to determine character substitutions.
   * Parameters: Keyword (alphabetic)

8. **Autokey/Running Key Cipher**
   * Uses the plaintext itself to extend the initial key.
   * Parameters: Initial key (alphabetic)

9. **N-gram Operations**
   * Performs operations on groups of n characters.
   * Parameters: Group size (n)
   * Note: Full decryption requires knowledge of original ordering.

10. **Hill Cipher**
    * Uses matrix multiplication for encryption/decryption.
    * Parameters: Keyword, block size (2 or 3)

11. **Rail Fence Cipher**
    * A transposition cipher that writes text in a zigzag pattern.
    * Parameters: Number of rails (fence lines)

12. **Route Cipher**
    * A transposition cipher that reads text along specific routes in a grid.
    * Parameters: Number of rows, number of columns, route type (spiral, zigzag, or columns)

13. **Myszkowski Cipher**
    * A columnar transposition cipher using a keyword with potentially repeated letters.
    * Parameters: Keyword (alphabetic)

## Compilation

Compile the program using a C++ compiler:

```bash
g++ -o rsalgo rsalgo.cpp
```

## Usage

Run the executable:

```bash
./rsalgo
```

Follow the menu prompts:
1. Select a cipher (1-13) or exit (0)
2. Choose encryption or decryption (except for ciphers like Atbash that are their own inverse)
3. Enter the required text and parameters
4. View the result

## Example Usage

```
==== Cryptographic Ciphers ====
1. Caesar Cipher
2. Atbash Cipher
3. Augustus Cipher
4. Affine Cipher
5. Vigenere Cipher
6. Gronsfeld Cipher
7. Beaufort Cipher
8. Autokey/Running Key Cipher
9. N-gram Operations
10. Hill Cipher
11. Rail Fence Cipher
12. Route Cipher
13. Myszkowski Cipher
0. Exit
Enter your choice: 1

Choose operation:
1. Encrypt
2. Decrypt
Enter your choice: 1

Enter the text: Hello World
Enter the shift value (0-25): 3
Encrypted text: Khoor Zruog
```

## Notes

- The implementation focuses on classical cryptographic methods and is intended for educational purposes.
- Modern cryptographic needs should be addressed with industry-standard libraries.
- Some ciphers have specific requirements:
  - Affine cipher requires coefficient 'a' to be coprime with 26.
  - Hill cipher requires a matrix that has an inverse mod 26.
  - N-gram decryption is not fully implemented due to its nature.

