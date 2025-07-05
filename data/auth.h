#ifndef AUTH_H
#define AUTH_H

#include <string>

// Create a new user with salted hash and store it
bool signUp(const std::string& username, const std::string& password);

// Verify user credentials against stored values
bool login(const std::string& username, const std::string& password);

// Helpers
std::string generateSalt(size_t length = 16);
std::string sha256(const std::string& str);
std::string encryptAES(const std::string& plaintext, const std::string& key, std::string& ivOut);
std::string decryptAES(const std::string& ciphertext, const std::string& key, const std::string& iv);

bool saveCredentialsEncrypted(const std::string& username, const std::string& salt, const std::string& hash);
bool verifyCredentials(const std::string& username, const std::string& password);

#endif
