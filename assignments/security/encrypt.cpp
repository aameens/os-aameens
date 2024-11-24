#include <iostream>
#include <fstream>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <cstring>
#include <openssl/bio.h>
#include <openssl/buffer.h>

/* To run:
g++ encrypt.cpp -o encrypt -lssl -lcrypto
g++ decrypt.cpp -o decrypt -lssl -lcrypto
./encrypt
./decrypt
*/

void handleErrors() {
    std::cerr << "An error occurred!" << std::endl;
    exit(1);
}

// Function to encode binary data into Base64
std::string base64Encode(const unsigned char *data, size_t length) {
    BIO *bio, *b64;
    BUF_MEM *bufferPtr;

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    // Encode to Base64
    BIO_write(bio, data, length);
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &bufferPtr);

    std::string base64String(bufferPtr->data, bufferPtr->length - 1); // Exclude the trailing null byte
    BIO_free_all(bio);

    return base64String;
}

void encryptMessage(const std::string &message, const std::string &key, const std::string &outputFile) {
    unsigned char iv[16];
    if (!RAND_bytes(iv, sizeof(iv))) {
        handleErrors();
    }

    unsigned char derivedKey[32];
    SHA256(reinterpret_cast<const unsigned char *>(key.c_str()), key.size(), derivedKey);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        handleErrors();
    }

    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, derivedKey, iv) != 1) {
        handleErrors();
    }

    unsigned char ciphertext[1024];
    int len, ciphertext_len;

    if (EVP_EncryptUpdate(ctx, ciphertext, &len, reinterpret_cast<const unsigned char *>(message.c_str()), message.size()) != 1) {
        handleErrors();
    }
    ciphertext_len = len;

    if (EVP_EncryptFinal_ex(ctx, ciphertext + len, &len) != 1) {
        handleErrors();
    }
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    // Base64 encode the IV and ciphertext
    std::string ivBase64 = base64Encode(iv, sizeof(iv));
    std::string ciphertextBase64 = base64Encode(ciphertext, ciphertext_len);

    std::ofstream file(outputFile);
    file << ivBase64 << "\n" << ciphertextBase64 << std::endl;
    file.close();

    std::cout << "Message encrypted and saved to " << outputFile << std::endl;
}

int main() {
    std::string message = "secret message";
    std::string key = "password1234";

    encryptMessage(message, key, "encrypted.txt");
    return 0;
}
