#pragma once
#include <cstdlib>
#include <memory>
#include <string>
#include <vector>
#include <sstream>

/**
 * Zentrale Auth-Konfiguration (ENV-getrieben).
 * - issuer / jwksUrl: Pflicht (sonst fail-fast in AppComponent)
 * - audience: optional, aber empfehlenswert
 * - securePathPrefixes: Pfade, die Auth benötigen (Default: "/api/secure/")
 */
struct AuthConfig {
  std::string issuer;
  std::string jwksUrl;
  std::string audience; // optional
  int leewaySec = 60;
  int jwksCacheMinutes = 15;
  std::vector<std::string> securePathPrefixes;

  static std::shared_ptr<AuthConfig> fromEnv() {
    auto get = [](const char* k, const char* def = "") {
      const char* v = std::getenv(k);
      return std::string(v ? v : def);
    };
    auto geti = [](const char* k, int def) {
      const char* v = std::getenv(k);
      return v ? std::atoi(v) : def;
    };
    auto splitCsv = [](const std::string& csv) {
      std::vector<std::string> out;
      std::stringstream ss(csv);
      std::string item;
      while(std::getline(ss, item, ',')) {
        // trim
        size_t a = item.find_first_not_of(" \t");
        size_t b = item.find_last_not_of(" \t");
        if (a == std::string::npos) continue;
        out.push_back(item.substr(a, b - a + 1));
      }
      if (out.empty()) out.push_back("/api/secure/");
      return out;
    };

    auto c = std::make_shared<AuthConfig>();
    c->issuer           = get("KEYCLOAK_ISSUER");
    c->jwksUrl          = get("KEYCLOAK_JWKS_URL");
    c->audience         = get("KEYCLOAK_AUDIENCE");
    c->leewaySec        = geti("JWT_LEEWAY_SECONDS", 60);
    c->jwksCacheMinutes = geti("JWKS_CACHE_MINUTES", 15);
    c->securePathPrefixes = splitCsv(get("SECURE_PATH_PREFIXES", "/api/secure/"));
    return c;
  }
};
