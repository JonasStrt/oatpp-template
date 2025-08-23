#pragma once
#include <string>
#include <unordered_map>
#include <chrono>
#include <mutex>
#include <stdexcept>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

/**
 * Thread-sicherer JWKS-Cache (kid -> (n,e)), TTL-basiert.
 * - Fetch via libcurl (5s Timeout, Follow-Redirects)
 * - Fail-closed: schlägt Fetch/Re-Load fehl → wirft Exception → 401 oben
 */
class JwksCache {
  const std::string url_;
  std::chrono::steady_clock::time_point expireAt_{};
  std::unordered_map<std::string, std::pair<std::string,std::string>> kidToNE_;
  std::mutex m_;

  static size_t writeCb(void* ptr, size_t size, size_t nmemb, void* data) {
    auto* s = static_cast<std::string*>(data);
    s->append(static_cast<char*>(ptr), size * nmemb);
    return size * nmemb;
  }
  static std::string fetchUrl(const std::string& url) {
    CURL* curl = curl_easy_init();
    if (!curl) throw std::runtime_error("curl init failed");
    std::string out;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
    auto rc = curl_easy_perform(curl);
    long code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
    curl_easy_cleanup(curl);
    if (rc != CURLE_OK || code / 100 != 2) {
      throw std::runtime_error("fetch JWKS failed");
    }
    return out;
  }
  void reloadLocked(int ttlMin) {
    const auto body = fetchUrl(url_);
    auto j = nlohmann::json::parse(body, /*cb=*/nullptr, /*allow_exceptions=*/true);
    std::unordered_map<std::string, std::pair<std::string,std::string>> newMap;
    for (auto& k : j["keys"]) {
      if (k.value("kty","") != "RSA") continue;
      const auto kid = k.value("kid", "");
      const auto n   = k.value("n", "");
      const auto e   = k.value("e", "");
      if (!kid.empty() && !n.empty() && !e.empty()) {
        newMap.emplace(kid, std::make_pair(n, e)); // base64url kodiert
      }
    }
    if (newMap.empty()) throw std::runtime_error("JWKS empty");
    kidToNE_.swap(newMap);
    expireAt_ = std::chrono::steady_clock::now() + std::chrono::minutes(ttlMin);
  }

public:
  explicit JwksCache(std::string url) : url_(std::move(url)) {
    // optional: curl_global_init(CURL_GLOBAL_ALL); // libcurl ist idempotent genug pro easy handle
  }

  std::pair<std::string,std::string> getNE(const std::string& kid, int ttlMin) {
    std::scoped_lock lk(m_);
    const auto now = std::chrono::steady_clock::now();
    if (now >= expireAt_) {
      reloadLocked(ttlMin);
    }
    auto it = kidToNE_.find(kid);
    if (it == kidToNE_.end()) {
      // mögliche Rotation → hart neu laden
      reloadLocked(ttlMin);
      it = kidToNE_.find(kid);
      if (it == kidToNE_.end()) {
        throw std::runtime_error("kid not found in JWKS");
      }
    }
    return it->second;
  }
};
