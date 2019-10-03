#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

/*
    Sample inputs:

    Plaintext: you need to find the key

    Ciphertext:
    onetfnteeyuedoidhky    expect: 21
    ntneudihyyeodkoefte    expect: 3421
    odnkyeieutdenotyefh    expect: 21345
    eneeiknfeuohottyddy    expect: 654321
    ooedtenyedufkythnie    expect: 6157342
*/

std::string get_message_from_plain_text(std::string);
std::string get_key(std::string, std::string);

int main() {
    std::string plain_text;
    std::string cipher_text;

    std::cout << "Enter Plaintext: ";
    std::getline(std::cin, plain_text);
    std::cout << "Enter Ciphertext: ";
    std::getline(std::cin, cipher_text);

    std::string solution = get_key(plain_text, cipher_text);
    if (solution.size() < 8) {
        std::cout << "The Key is: " << solution << std::endl;
    }
    else {
        std::cout << solution << std::endl;
    }
    return 0;
}

// this removes spaces from a string and converts the characters to all caps
std::string get_message_from_plain_text(std::string plain_text) {
    plain_text.erase(std::remove_if(plain_text.begin(), plain_text.end(), isspace), plain_text.end()); // Remove all spaces from plain text
    std::transform(plain_text.begin(), plain_text.end(), plain_text.begin(), ::toupper); // Convert plain text to uppercase
    return plain_text;
}

// finds the correct key and returns the order represented as integers.
// eg "sure" = "3421", "testing" = "6157342"
// duplicate characters read left to right.
std::string get_key(std::string plain_text, std::string cipher_text) {
    auto message = get_message_from_plain_text(plain_text);
    auto cipher_message = get_message_from_plain_text(cipher_text);
    std::string solution;
    int key_length = 0;
    int MAX_KEY_LENGTH = 7;

    // check for valid input lengths.
    if (message.size() != cipher_message.size()) { return "Plaintext & Ciphertext have a different length."; }

    int message_position = 0;
    int cipher_message_position = 0;

    // finds the length of the key.
    // STEPS:
    // 1. Find matching characters 
    // 2. Brute force check key lengths from 1 to MAX_KEY_LENGTH
    //      ->    incrementing the message position by the length you are checking
    //      ->    incrementing the cipher_message position by 1.
    //      ->    increment accross entire message until you reach the end
    //      ->    once you have reached the end you have found the key_length.
    // 3. Check for valid key_length
    //      ->    if message_position reaches the end of the message, 
    //      ->    no key has been found, error in input or key_length out of range.
    while (message_position < message.size()) {
        // 1. Find matching characters
        if (message[message_position] == cipher_message[cipher_message_position]) { // found matching characters
            bool found = false;
            // 2. Brute force check key lengths
            for (int i = 1; i <= MAX_KEY_LENGTH; i++) {
                int temp_message_pos = message_position;
                int temp_cipher_message_pos = cipher_message_position;
                key_length = i;
                while (temp_message_pos < message.size()) {
                    // if next characters do not match, check next key_length
                    if (message[temp_message_pos] != cipher_message[temp_cipher_message_pos]) {
                        temp_message_pos = message_position;
                        temp_cipher_message_pos = cipher_message_position;
                        found = false;
                        break;
                    }
                    temp_message_pos += i;
                    temp_cipher_message_pos++;
                    found = true;
                }
                // correct key_length found break for loop
                if (temp_message_pos != message_position) {
                    break;
                }
            }
            // correct key_length found break while loop
            if (cipher_message_position != 0 && found) {
                break;
            }
            // increment to next character to begin finding next matching characters.
            else {
                message_position++;
                cipher_message_position = 0;
            }
        }
        // if we have reached to MAX_KEY_LENGTH find new matching characters.
        else if (cipher_message_position == MAX_KEY_LENGTH) {
            cipher_message_position = 0;
            message_position++;
        }
        // check next character.
        else {
            cipher_message_position++;
        }
    }

    // if now key is found, error in input or invalid key_length.
    if (message_position == message.size()) { return "error in entry."; }

    message_position = 0;
    cipher_message_position = 0;

    std::vector<std::vector<char>> solution_vec;
    std::vector<int> order_vec;

    // initallize the the vector that will hold then correct ordered key.
    for (int i = 0; i < key_length; i++) {
        std::vector<char> temp;
        solution_vec.push_back(temp);
        order_vec.push_back(0);
    }

    // 2D vector that holds the columnar transposition.
    for (int i = 0; i < message.size(); i += key_length) {
        for (int j = 0; j < key_length; j++) {
            if (i + j == message.size()) { break; }
            solution_vec[j].push_back(static_cast<char>(message[i + j]));
        }
    }


    // find key order
    //    ->    go over each column and match it up with the Ciphertext
    for (int i = 1; i <= key_length; i++) {
        for (int row = 0; row < solution_vec.size(); row++) {
            int temp = cipher_message_position;
            for (int col = 0; col < solution_vec[row].size(); col++) {
                if (solution_vec[row][col] != cipher_message[cipher_message_position]) {
                    cipher_message_position -= col;
                    break;
                }
                else {
                    cipher_message_position++;
                }
                if (col == solution_vec[row].size() - 1) {
                    order_vec[row] = i;
                }
            }
            // incorrect key. Column matched.
            if (temp != cipher_message_position) {
                std::cout << "error in program: incorrect key found." << std::endl;
                break;
            }
        }
    }
    // convert ordered solution to string.
    for (int i = 0; i < order_vec.size(); i++) {
        solution += std::to_string(order_vec[i]);
    }
    return solution;
}