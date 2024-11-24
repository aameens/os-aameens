#include <iostream>
#include <fstream>
#include <vector>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <cstring>

void handleErrors() {
    std::cerr << "An error occurred!" << std::endl;
    exit(1);
}

// Function to decode Base64 into binary data
std::vector<unsigned char> base64Decode(const std::string &base64Data) {
    BIO *bio, *b64;
    int decodeLen = base64Data.size();
    std::vector<unsigned char> decodedData(decodeLen);

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new_mem_buf(base64Data.data(), base64Data.size());
    bio = BIO_push(b64, bio);

    // Disable newline decoding for Base64 (default adds newline handling)
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);

    decodeLen = BIO_read(bio, decodedData.data(), base64Data.size());
    if (decodeLen < 0) {
        BIO_free_all(bio);
        handleErrors();
    }

    decodedData.resize(decodeLen); // Resize to the actual data length
    BIO_free_all(bio);

    return decodedData;
}

void decryptMessage(const std::string &key, const std::string &inputFile, const std::string &outputFile) {
    std::ifstream file(inputFile);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << inputFile << std::endl;
        exit(1);
    }

    // Read Base64-encoded IV and ciphertext
    std::string ivBase64, ciphertextBase64;
    std::getline(file, ivBase64);
    std::getline(file, ciphertextBase64);
    file.close();

    // Decode Base64 strings
    std::vector<unsigned char> iv = base64Decode(ivBase64);
    std::vector<unsigned char> ciphertext = base64Decode(ciphertextBase64);

    if (iv.size() != 16) {
        std::cerr << "Invalid IV size!" << std::endl;
        exit(1);
    }

    // Derive key using SHA-256
    unsigned char derivedKey[32];
    SHA256(reinterpret_cast<const unsigned char *>(key.c_str()), key.size(), derivedKey);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        handleErrors();
    }

    // Initialize decryption context
    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, derivedKey, iv.data()) != 1) {
        handleErrors();
    }

    unsigned char plaintext[1024];
    int len = 0, plaintext_len = 0;

    // Decrypt the ciphertext
    if (EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext.data(), ciphertext.size()) != 1) {
        handleErrors();
    }
    plaintext_len = len;

    // Finalize decryption
    if (EVP_DecryptFinal_ex(ctx, plaintext + len, &len) != 1) {
        std::cerr << "Decryption failed. Invalid key or corrupted data." << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        exit(1);
    }
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    // Write plaintext to the output file
    std::ofstream outFile(outputFile);
    outFile.write(reinterpret_cast<char *>(plaintext), plaintext_len);
    outFile.close();

    std::cout << "Message decrypted and saved to " << outputFile << std::endl;
}

int main() {
    std::string key = "password1234";

    decryptMessage(key, "encrypted.txt", "decrypted.txt");
    return 0;
}
