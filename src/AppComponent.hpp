#ifndef AppComponent_hpp
#define AppComponent_hpp

#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/web/mime/ContentMappers.hpp"

#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/json/ObjectMapper.hpp"

#include "oatpp/macro/component.hpp"

#include "./auth/AuthConfig.hpp"
#include "./auth/JwtVerifier.hpp"
#include "./auth/AuthInterceptor.hpp"

/**
 *  Class which creates and holds Application components and registers components in oatpp::base::Environment
 *  Order of components initialization is from top to bottom
 */
class AppComponent {
public:
  
  /**
   *  Create ConnectionProvider component which listens on the port
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)([] {
    return oatpp::network::tcp::server::ConnectionProvider::createShared({"0.0.0.0", 8000, oatpp::network::Address::IP_4});
  }());
  
  /**
   *  Create Router component
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)([] {
    return oatpp::web::server::HttpRouter::createShared();
  }());

  // AuthConfig aus ENV (fail-fast, wenn Pflichtfelder fehlen)
  OATPP_CREATE_COMPONENT(std::shared_ptr<AuthConfig>, authConfig)([] {
    auto cfg = AuthConfig::fromEnv();
    if (cfg->issuer.empty() || cfg->jwksUrl.empty()) {
      OATPP_LOGe("AuthConfig", "Missing KEYCLOAK_ISSUER or KEYCLOAK_JWKS_URL");
      throw std::runtime_error("AuthConfig invalid");
    }
    return cfg;
  }());

  // JwtVerifier
  OATPP_CREATE_COMPONENT(std::shared_ptr<JwtVerifier>, jwtVerifier)([] {
    OATPP_COMPONENT(std::shared_ptr<AuthConfig>, cfg);
    return std::make_shared<JwtVerifier>(cfg);
  }());
  
  /**
   *  Create ConnectionHandler component which uses Router component to route requests
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, serverConnectionHandler)([] {
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router); // get Router component
    auto h = oatpp::web::server::HttpConnectionHandler::createShared(router);

    OATPP_COMPONENT(std::shared_ptr<JwtVerifier>, verifier);
    h->addRequestInterceptor(std::make_shared<AuthInterceptor>(verifier));
    return std::static_pointer_cast<oatpp::network::ConnectionHandler>(h);
  }());
  
  /**
   *  Create ObjectMapper component to serialize/deserialize DTOs in Contoller's API
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>, apiContentMappers)([] {

    auto json = std::make_shared<oatpp::json::ObjectMapper>();
    json->serializerConfig().json.useBeautifier = true;

    auto mappers = std::make_shared<oatpp::web::mime::ContentMappers>();
    mappers->putMapper(json);

    return mappers;

  }());

};

#endif /* AppComponent_hpp */
