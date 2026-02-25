/*
 * device.cc
 *
 *  Created on: Nov 28, 2025
 *      Author: helen
 */
#include <omnetpp.h>
#include <string>
#include <iomanip>
#include <vector>
#include <sstream>
#include "Report.h"
#include "report_m.h"
#include <openssl/evp.h>
#include <openssl/bn.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/sha.h>

using namespace omnetpp;

class device : public cSimpleModule{
private:
    EVP_PKEY *evpPrivateKey = nullptr;
    BIGNUM *exponent = nullptr;
    int device_id;
    int clusterSize = 1;

public:
    RSA *publicKey;
protected:
    virtual void initialize() override{
        simtime_t initTime = uniform(0, 0.00001);  // uniform() 是 OMNeT++ 随机函数，需包含 omnetpp.h
                   // 步骤2：创建“触发发送报告”的事件消息（命名为 "TriggerSendReport"，便于识别）
        cMessage *triggerMsg = new cMessage("TriggerSendReport");

                   // 步骤3：调度事件——到 initTime 时刻，会触发 handleMessage() 函数
        scheduleAt(initTime, triggerMsg);
        generateKeyPair();
        device_id = par("device_id").intValue();
        clusterSize = par("clusterSize").intValue();

    }
    virtual void handleMessage(cMessage *msg) override {
        if (strcmp(msg->getName(), "TriggerSendReport") == 0) {

            for (int x = 0; x < clusterSize; ++x) {
                std::string senderID = "device_" + std::to_string(device_id) + "_" + std::to_string(x);
                std::string reportContent = "This is a trusted report of " + senderID;

                Report newreport(senderID, reportContent);
                EV << "Generated report sender=" << senderID
                   << " hash=" << newreport.getHash() << endl;

                std::vector<unsigned char> signature = signdata(newreport.getHash());
                newreport.signature = signature;

                std::string publickey = extractPublicKeyPEM();
                newreport.publickey = publickey;

                sendReport(newreport);
            }

            delete msg;   // ✅ self message 记得释放
            return;
        }

        delete msg; // 其它未知消息也释放，避免泄漏
    }

    void generateKeyPair() {
        ERR_load_crypto_strings();
        OpenSSL_add_all_algorithms();
        EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);
        if (!ctx) {
            EV_ERROR << "Failed to create EVP_PKEY context!\n";
            ERR_print_errors_fp(stderr);
            return;
        }
        // 初始化密钥生成参数（RSA 2048 位，公钥指数 65537）
        if (EVP_PKEY_keygen_init(ctx) <= 0 ||
            EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 2048) <= 0) {
            EV_ERROR << "Failed to initialize RSA key generation parameters!\n";
            ERR_print_errors_fp(stderr);
            EVP_PKEY_CTX_free(ctx);
            return;
         }
         // 生成 RSA 密钥对
         if (EVP_PKEY_keygen(ctx, &evpPrivateKey) <= 0) {
             EV_ERROR << "RSA key generation failed!\n";
             ERR_print_errors_fp(stderr);
             EVP_PKEY_CTX_free(ctx);
             return;
          }
          EVP_PKEY_CTX_free(ctx);
          EV << "RSA key pair generated successfully. Key size: " << EVP_PKEY_size(evpPrivateKey) << " bytes\n";
          // 示例：将公钥保存到文件（PEM 格式）
              FILE *pubKeyFile = fopen("device_public.pem", "w");
              if (pubKeyFile && PEM_write_PUBKEY(pubKeyFile, evpPrivateKey)) {
                  EV_INFO << "Public key saved to device_public.pem\n";
              } else {
                  EV_ERROR << "Failed to save public key!\n";
                  ERR_print_errors_fp(stderr);
              }
              if (pubKeyFile) fclose(pubKeyFile);

              // 示例：将私钥保存到文件（PEM 格式）
              FILE *privKeyFile = fopen("device_private.pem", "w");
              if (privKeyFile && PEM_write_PrivateKey(privKeyFile, evpPrivateKey, nullptr, nullptr, 0, nullptr, nullptr)) {
                  EV_INFO << "Private key saved to device_private.pem\n";
              } else {
                  EV_ERROR << "Failed to save private key!\n";
                  ERR_print_errors_fp(stderr);
              }
              if (privKeyFile) fclose(privKeyFile);
              //finish();
          }
    void finish() {
        // 释放资源
        if (evpPrivateKey) EVP_PKEY_free(evpPrivateKey);
            evpPrivateKey = nullptr;
            EVP_cleanup();
            ERR_free_strings();
    }
    std::vector<unsigned char> signdata(const std::string& hashValue){
        if (!evpPrivateKey || hashValue.empty()) {
                EV_ERROR << "SignData failed: invalid private key or empty hash value\n";
                return {};
            }
        // 检查 EVP_PKEY 是否为 RSA 类型
        if (EVP_PKEY_id(evpPrivateKey) != EVP_PKEY_RSA) {
            EV_ERROR << "SignData failed: private key is not an RSA key\n";
            return {};
        }
        RSA* rsaPrivateKey = EVP_PKEY_get1_RSA(evpPrivateKey);
           if (!rsaPrivateKey) {
               EV_ERROR << "SignData failed: could not extract RSA key from EVP_PKEY\n";
               ERR_print_errors_fp(stderr);
               return {};
           }
         unsigned int sigLen = RSA_size(rsaPrivateKey);
             std::vector<unsigned char> signature(sigLen);
             if (RSA_sign(NID_sha256,
                             (unsigned char*)hashValue.c_str(),
                             hashValue.length(),
                             signature.data(),
                             &sigLen,
                             rsaPrivateKey) != 1) {
                    EV_ERROR << "SignData failed: RSA_sign failed\n";
                    ERR_print_errors_fp(stderr);
                    RSA_free(rsaPrivateKey); // 释放 RSA 结构体
                    return {};
                }
         // 释放从 EVP_PKEY 中提取的 RSA 结构体
         RSA_free(rsaPrivateKey);
         // 调整签名向量的大小以匹配实际生成的签名长度
         signature.resize(sigLen);
         EV_INFO << "SignData: Successfully signed the hash value. Signature size: " << signature.size() << " bytes.\n";
         EV_INFO << "Generated Signature (hex): ";
         for (unsigned char c : signature) {
             EV_INFO << std::hex << std::setw(2) << std::setfill('0') << (int)c;
         }
         EV_INFO << std::dec << endl;
         return signature;
    }
    std::string extractPublicKeyPEM() {
            if (!evpPrivateKey) {
                EV_ERROR << "extractPublicKeyPEM: evpPrivateKey is nullptr. Key pair not generated.\n";
                return "";
            }

            // 使用 BIO (Basic I/O) 对象在内存中操作字符串
            BIO *bio = BIO_new(BIO_s_mem());
            if (!bio) {
                EV_ERROR << "extractPublicKeyPEM: Failed to create BIO object.\n";
                ERR_print_errors_fp(stderr);
                return "";
            }

            // 将公钥从 EVP_PKEY 对象写入到 BIO 中，格式为 PEM
            if (PEM_write_bio_PUBKEY(bio, evpPrivateKey) != 1) {
                EV_ERROR << "extractPublicKeyPEM: Failed to write public key to BIO.\n";
                ERR_print_errors_fp(stderr);
                BIO_free(bio); // 释放已分配的 BIO
                return "";
            }

            // 从 BIO 中获取内存中的字符串指针和长度
            BUF_MEM *bptr;
            BIO_get_mem_ptr(bio, &bptr);

            // 从指针和长度创建一个 std::string
            std::string publicKeyPEM(bptr->data, bptr->length);

            // 释放 BIO 对象。注意，BIO_free 会自动释放其内部的 BUF_MEM 结构
            BIO_free(bio);

            EV_INFO << "Successfully extracted public key in PEM format.\n";
            // EV_INFO << "Public Key PEM:\n" << publicKeyPEM << "\n"; // 如果想在日志中查看公钥

            return publicKeyPEM;
        }
    void sendReport(Report& reportToSend) {
        ReportMessage *msg = new ReportMessage("ReportMsg");
        msg->setSenderID(reportToSend.senderID.c_str());
        msg->setContent(reportToSend.content.c_str());
        msg->setHashValue(reportToSend.hashValue.c_str());
        msg->setTimestamp(reportToSend.timestamp);
        msg->setSenderpublickey(reportToSend.publickey.c_str());
        if (!reportToSend.signature.empty()) {
           // 将 std::vector<unsigned char> 转换为十六进制 std::string
           std::string signatureStr = binToHex(reportToSend.signature);
           // 将字符串签名设置到消息中
           msg->setSignature(signatureStr.c_str());
           EV_INFO << "Signature added to message as hex string. Original size: "
                   << reportToSend.signature.size() << " bytes, String size: " << signatureStr.size() << " bytes.\n";
         } else {
           EV_WARN << "Report signature is empty, not adding to message.\n";
           // 可以选择设置一个空字符串或不设置
           msg->setSignature("");
         }
        //cModule *peerNode = getModuleByPath("peer_1");
        EV << "Sending report from" << reportToSend.senderID << endl;
        send(msg, "out", 0);
        //sendDirect(msg, peerNode,"in");
    }
    std::string binToHex(const std::vector<unsigned char>& binData) {
            std::stringstream ss;
            ss << std::hex << std::setfill('0');
            for (unsigned char c : binData) {
                ss << std::setw(2) << static_cast<int>(c);
            }
            return ss.str();
        }
};
Define_Module(device);


