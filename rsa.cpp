#include <cmath>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

// finds gcd of the two given numbers
long gcd(long x, long y);   
// finds decryption key given 2 prime numbers  
long find_d(long p, long q);  
// decrypts given message from file using calculated key and n value and writes message to file
void decrypt(
        string input,
        string output,
        long d,
        long n); 
// encrypts inputted message using calculated n value and writes result to file 
void encrypt(
        string filename,
        long n,
        string message); 
// returns log base 100 of the given number                
long log_100(long x);             
// returns M^e % n       
long modExpAlg(long M, long e, long n);  
// returns a string representing the given number in binary
string toBinary(long num);               

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Incorrect number of arguments given." << endl;
        return 1;
    }

    // reads in user input and calculates decryption key
    long p = stol(argv[1]);
    long q = stol(argv[2]);
    long d = find_d(p, q);

    string command, full_command;
    while (true) {
        cout << "Enter a command." << endl;
        getline(cin, full_command);
        stringstream s(full_command);
        s >> command;
        if (command == "EXIT") {
            break;
        } else if (command == "DECRYPT") {
            string input, output;
            s >> input;
            s >> output;
            decrypt(input, output, d, (p * q));
        } else if (command == "ENCRYPT") {
            string filename, message, m;
            long n;
            message = "";
            s >> filename;
            s >> n;
            while (s >> m) {
                message += m;
                message += ' ';
            }
            message = message.substr(0, message.length() - 1);
            encrypt(filename, n, message);
        } else {
            cout << "Command invalid." << endl;
        }
    }
}

long gcd(long x, long y)  // using standard euclidian algorithm
{
    long r = x;
    long old_r = y;
    while (r != 0) {
        long quotient = old_r / r;
        long temp = r;
        r = old_r - quotient * r;
        old_r = temp;
    }
    return old_r;
}

long find_d(long p, long q)  // using extended euclidian algorithm
{
    long L = (p - 1) * (q - 1) / gcd(p - 1, q - 1);
    long e = 65537;
    if (L <= e) {
        cout << "L value too small." << endl;
        throw exception();
    }
    long t = 1;
    long old_t = 0;
    long r = e;
    long old_r = L;
    while (r != 0) {
        long quotient = old_r / r;
        long temp = r;
        r = old_r - quotient * r;
        old_r = temp;
        temp = t;
        t = old_t - quotient * t;
        old_t = temp;
    }
    if (gcd(e, L) != 1) {
        cout << "gcd of e and L is not 1" << endl;
        throw exception();
    }
    if (old_t < 0) {
        old_t += (p - 1) * (q - 1);
    }
    return old_t;
}

void decrypt(string input, string output, long d, long n) {
    ifstream in_file;
    in_file.open(input);
    ofstream out_file;
    out_file.open(output);
    long C;
    while (in_file >> C) {
        long M = modExpAlg(C, d, n);  // decrypt with M = C^d % n
        string message_num = to_string(M);
        while (message_num.length() != 0) {
            int val = 0;                        // val becomes value that will correspond to letters and spaces
            if (message_num.length() % 2 == 0)  // seperate each integer
            {
                val += message_num[0] - 48;
                message_num = message_num.substr(1);
                val *= 10;
            }
            val += message_num[0] - 48;
            message_num = message_num.substr(1);
            char message_char;
            if (val == 0)  // convert to letters and spaces
            {
                message_char = ' ';
            } else {
                message_char = (char)(val + 96);
            }
            out_file << message_char;
        }
    }
    in_file.close();
    out_file.close();
}

void encrypt(string filename, long n, string message) {
    ofstream file;
    file.open(filename);
    long e = 65537;
    long x = 1 + log_100(n / 27);
    if (n < 27) {
        cout << "n value too small" << endl;
        throw exception();
    }
    while (message.length() != 0) {
        string message_num = "";
        int val;
        for (long i = 0; i < x; i++)  // convert message to integers
        {
            if ((size_t)i >= message.length() || message[i] == ' ') {
                val = 0;
            } else {
                val = (int)(message[i]) - 96;
            }
            if (val >= 0 && val <= 9) {
                message_num += '0';
            }
            message_num += to_string(val);
        }
        if (message.length() >= (unsigned long)x) {
            message.erase(0, x);
        } else {
            message = "";
        }
        long M = stol(message_num);
        long C = modExpAlg(M, e, n);  // encrypt with C = M^e % n
        file << C;
        if (message.length() != 0) {
            file << " ";
        }
    }
    file.close();
}

long log_100(long x) {
    return ((log(x)) / (log(100)));  // uses change of base rule to find log base 100 of a given number
}

long modExpAlg(long M, long e, long n) {
    string binary_e = toBinary(e);
    long C = 1;
    long power = M % n;
    for (int i = binary_e.length() - 1; i >= 0; i--) {
        if (binary_e[i] == '1') {
            C = (C * power) % n;
        }
        power = (power * power) % n;
    }
    return C;
}

string toBinary(long num) {
    string result = "";
    string flipped = "";
    while (num > 0) {
        if (num % 2 == 1) {
            result += '1';
        } else {
            result += '0';
        }
        num /= 2;
    }
    for (int i = result.length() - 1; i >= 0; i--) {
        flipped += result[i];
    }
    return flipped;
}