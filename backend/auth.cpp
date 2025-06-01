#include "auth.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/aes.h>
#include <openssl/sha.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cstring>

std::string generateSalt(size_t length) {
    std::string salt(length, 0);
    RAND_bytes(reinterpret_cast<unsigned char*>(&salt[0]), length);
    for (size_t i = 0; i < length; ++i) {
        salt[i] = 'A' + (salt[i] % 26);  // Simple printable salt
    }
    return salt;
}

std::string sha256(const std::string& str) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(str.c_str()), str.size(), hash);
    std::ostringstream oss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    return oss.str();
}

bool saveCredentialsEncrypted(const std::string& username, const std::string& salt, const std::string& hash) {
    std::ofstream out("../backend/encrypted/credentials.dat", std::ios::app);

    if (!out) {
        std::cerr << "❌ Failed to open credentials.dat for writing\n";
        return false;
    }

    out << username << ":" << salt << ":" << hash << "\n";
    return true;
}

bool verifyCredentials(const std::string& username, const std::string& password) {
    std::ifstream in("../backend/encrypted/credentials.dat", std::ios::binary);
    if (!in) return false;

    std::string key = sha256("secretkey000000000000000000000000");

    std::string line;
    while (getline(in, line)) {
        if (line.size() < 16) continue; // too short
        std::string iv = line.substr(0, 16);
        std::string ciphertext = line.substr(16);

        std::string decrypted = decryptAES(ciphertext, key, iv);
        std::cerr << "🧪 Decrypted: ";
        for (unsigned char c : decrypted) {
            if (isprint(c))
                std::cerr << c;
            else
                std::cerr << "\\x" << std::hex << std::setw(2) << std::setfill('0') << (int)c;
        }
        std::cerr << std::endl;

        std::istringstream iss(decrypted);
        std::string storedUser, storedSalt, storedHash;

        if (getline(iss, storedUser, ':') &&
            getline(iss, storedSalt, ':') &&
            getline(iss, storedHash)) {

            if (storedUser == username) {
                std::string inputHash = sha256(storedSalt + password);
                return inputHash == storedHash;
            }
        }
    }

    return false;
}

bool signUp(const std::string& username, const std::string& password) {
    std::string salt = generateSalt();
    std::string hash = sha256(salt + password);
    std::string plain = username + ":" + salt + ":" + hash;
    std::string key = sha256("secretkey000000000000000000000000");
    std::string iv;
    std::string encrypted = encryptAES(plain, key, iv);

    std::ofstream out("../backend/encrypted/credentials.dat", std::ios::app);
    if (!out) {
        std::cerr << "❌ Failed to open credentials.dat for writing\n";
        return false;
    }
    out << iv << encrypted << "\n";
    return true;
}

bool login(const std::string& username, const std::string& password) {
    return verifyCredentials(username, password);
}

std::string encryptAES(const std::string& plaintext, const std::string& key, std::string& ivOut) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    unsigned char iv[16];
    RAND_bytes(iv, sizeof(iv));
    ivOut = std::string(reinterpret_cast<char*>(iv), 16);

    unsigned char ciphertext[1024];
    int len, ciphertext_len;

    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL,
                       reinterpret_cast<const unsigned char*>(key.c_str()),
                       iv);

    EVP_EncryptUpdate(ctx, ciphertext, &len,
                      reinterpret_cast<const unsigned char*>(plaintext.c_str()),
                      plaintext.length());
    ciphertext_len = len;

    EVP_EncryptFinal_ex(ctx, ciphertext + len, &len);
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return std::string(reinterpret_cast<char*>(ciphertext), ciphertext_len);
}

std::string decryptAES(const std::string& ciphertext, const std::string& key, const std::string& iv) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    unsigned char decryptedtext[1024];
    int len, decrypted_len;

    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL,
                       reinterpret_cast<const unsigned char*>(key.c_str()),
                       reinterpret_cast<const unsigned char*>(iv.c_str()));

    EVP_DecryptUpdate(ctx, decryptedtext, &len,
                      reinterpret_cast<const unsigned char*>(ciphertext.c_str()),
                      ciphertext.length());
    decrypted_len = len;

    EVP_DecryptFinal_ex(ctx, decryptedtext + len, &len);
    decrypted_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return std::string(reinterpret_cast<char*>(decryptedtext), decrypted_len);
}
