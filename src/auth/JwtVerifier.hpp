#pragma once
#include "AuthConfig.hpp"
#include "JwksCache.hpp"
#include <jwt-cpp/jwt.h>

/**
 * JwtVerifier
 * - RS256 Validierung gegen JWKS (n,e) und iss/aud/exp/nbf/iat + leeway
 * - Erwartet jwt-cpp >= 0.7.x (rs256-ctor mit n,e (base64url))
 */
class JwtVerifier {
  std::shared_ptr<AuthConfig> cfg_;
  JwksCache jwks_;

public:
  explicit JwtVerifier(std::shared_ptr<AuthConfig> cfg)
    : cfg_(std::move(cfg)), jwks_(cfg_->jwksUrl) {}

  jwt::decoded_jwt<> verify(const std::string& token) {
    auto decoded = jwt::decode(token);

    const auto kid = decoded.get_key_id().value_or("");
    if (kid.empty()) throw std::runtime_error("missing kid");

    const auto [n_b64u, e_b64u] = jwks_.getNE(kid, cfg_->jwksCacheMinutes);

    // RS256 mit (n,e) – jwt-cpp baut intern den Public Key
    const auto alg = jwt::algorithm::rs256(n_b64u, e_b64u);
    auto v = jwt::verify()
      .allow_algorithm(alg)
      .leeway(cfg_->leewaySec)
      .with_issuer(cfg_->issuer);

    if (!cfg_->audience.empty()) {
      v.with_audience(cfg_->audience);
    }

    v.verify(decoded); // prüft exp/nbf/iat

    return decoded;
  }

  const std::shared_ptr<AuthConfig>& config() const noexcept { return cfg_; }
};
