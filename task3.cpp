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
    eneeiknfeuohottyddy    expect: 654321    <- currently not working
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

std::string get_message_from_plain_text(std::string plain_text) {
    plain_text.erase(std::remove_if(plain_text.begin(), plain_text.end(), isspace), plain_text.end()); // Remove all spaces from plain text
    std::transform(plain_text.begin(), plain_text.end(), plain_text.begin(), ::toupper); // Convert plain text to uppercase
    return plain_text;
}

std::string get_key(std::string plain_text, std::string cipher_text) {
    auto message = get_message_from_plain_text(plain_text);
    auto cipher_message = get_message_from_plain_text(cipher_text);
    std::string solution;
    int key_length = 0;
    int MAX_KEY_LENGTH = 7;

    if (message.size() != cipher_message.size()) { return "Plaintext & Ciphertext have a different length."; }

    int message_position = 0;
    int cipher_message_position = 0;

    // finds the lenght of the key.
    while (message_position < message.size()) {
        if (message[message_position] == cipher_message[cipher_message_position]) { // found matching characters
            for (int i = 1; i <= MAX_KEY_LENGTH; i++) { // check length 1 to MAX_KEY_LENGTH for correct key length.
                int temp_message_pos = message_position;
                int temp_cipher_message_pos = cipher_message_position;
                key_length = i;
                while (temp_message_pos < message.size()) {
                    if (message[temp_message_pos] != cipher_message[temp_cipher_message_pos]) {
                        temp_message_pos = message_position;
                        temp_cipher_message_pos = cipher_message_position;
                        break;
                    }
                    temp_message_pos += i;
                    temp_cipher_message_pos++;
                }
                if (temp_message_pos != message_position) { // correct length found
                    break;
                }
            }
            if (cipher_message_position != 0) { // correct key_length found.
                break;
            }
            else {
                message_position++;
                cipher_message_position = 0;
            }
        }
        else if (cipher_message_position == MAX_KEY_LENGTH) {
            cipher_message_position = 0;
            message_position++;
        }
        else {
            cipher_message_position++;
        }
    }

    if (message_position == message.size()) { return "error in entry."; }

    message_position = 0;
    cipher_message_position = 0;

    std::vector<std::vector<char>> solution_vec;
    std::vector<int> order_vec;

    // initallize the the vector that will hold the order.
    for (int i = 0; i < key_length; i++) {
        std::vector<char> temp;
        solution_vec.push_back(temp);
        order_vec.push_back(0);
    }

    // 2D vector that holds the transposition.
    for (int i = 0; i < message.size(); i += key_length) {
        for (int j = 0; j < key_length; j++) {
            if (i + j == message.size()) { break; }
            solution_vec[j].push_back(static_cast<char>(message[i + j]));
        }
    }


    // find key order
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
            if (temp != cipher_message_position) { // break if new key has been found
                break;
            }
        }
    }

    for (int i = 0; i < order_vec.size(); i++) {
        solution += std::to_string(order_vec[i]);
    }
    return solution;
}