#pragma once
#include <oatpp/web/server/interceptor/RequestInterceptor.hpp>
#include <oatpp/web/protocol/http/Http.hpp>
#include <oatpp/web/protocol/http/outgoing/ResponseFactory.hpp>
#include "JwtVerifier.hpp"

/**
 * AuthInterceptor
 * - schützt Pfade (ENV: SECURE_PATH_PREFIXES) mit Bearer Token
 * - 401 bei fehlendem/ungültigem Token (WWW-Authenticate gesetzt)
 * - Claims können optional ins Request-Bundle gelegt werden
 */
class AuthInterceptor : public oatpp::web::server::interceptor::RequestInterceptor {
  std::shared_ptr<JwtVerifier> verifier_;

  static bool matchesAnyPrefix(const std::string& path, const std::vector<std::string>& prefixes) {
    for (const auto& p : prefixes) {
      if (path.rfind(p, 0) == 0) return true; // startsWith
    }
    return false;
  }
  static std::string getBearer(const oatpp::String& header) {
    if (!header) return {};
    const auto s = header->std_str();
    const auto sp = s.find(' ');
    if (sp == std::string::npos) return {};
    auto scheme = s.substr(0, sp);
    for (auto& c : scheme) c = (char)tolower(c);
    if (scheme != "bearer") return {};
    return s.substr(sp + 1);
  }

public:
  explicit AuthInterceptor(std::shared_ptr<JwtVerifier> v) : verifier_(std::move(v)) {}

  std::shared_ptr<oatpp::web::protocol::http::outgoing::Response>
  intercept(const std::shared_ptr<oatpp::web::protocol::http::incoming::Request>& req) override {
    const auto path = req->getStartingLine().path.toString();
    const auto& cfg = *verifier_->config();
    if (!matchesAnyPrefix(path, cfg.securePathPrefixes)) {
      return nullptr; // nicht geschützt → weiterreichen
    }

    const auto tok = getBearer(req->getHeader("authorization"));
    if (tok.empty()) {
      auto r = oatpp::web::protocol::http::outgoing::ResponseFactory::createResponse(
        oatpp::web::protocol::http::Status::CODE_401, "Missing or invalid Authorization header");
      r->putHeader("WWW-Authenticate", "Bearer");
      return r;
    }

    try {
      const auto decoded = verifier_->verify(tok);

      // Optional: Ausgewählte Claims ins Bundle legen (sparsam!)
      // if (auto sub = decoded.get_subject()) {
      //   req->putBundleData("jwt.sub", oatpp::String(*sub));
      // }
      // if (decoded.has_payload_claim("preferred_username")) {
      //   req->putBundleData("jwt.username", oatpp::String(decoded.get_payload_claim("preferred_username").as_string()));
      // }

      return nullptr; // OK → weiterreichen
    } catch (const std::exception& e) {
      // Sicherheitsbewusst: keine Token-Inhalte loggen.
      auto r = oatpp::web::protocol::http::outgoing::ResponseFactory::createResponse(
        oatpp::web::protocol::http::Status::CODE_401, "Unauthorized");
      r->putHeader("WWW-Authenticate", "Bearer error=\"invalid_token\"");
      return r;
    }
  }
};
