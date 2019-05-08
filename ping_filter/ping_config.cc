#include <string>

#include "ping_config.h"

#include "envoy/registry/registry.h"
#include "envoy/server/filter_config.h"

namespace Envoy {
namespace Extensions {
namespace NetworkFilters {
namespace Ping {

/**
 * Config registration for the Ping filter. @see NamedNetworkFilterConfigFactory.
 */
Network::FilterFactoryCb
PingConfigFactory::createFilterFactoryFromProtoTyped(
    const config::PingFilter& proto_config,
    Server::Configuration::FactoryContext& context) {
  PingFilterConfigSharedPtr filter_config(
      std::make_shared<PingFilterConfig>(proto_config.convert_to_upper(), context.scope()));
  return [filter_config](Network::FilterManager& filter_manager) -> void {
    filter_manager.addFilter(std::make_shared<PingFilter>(filter_config));
  };
}

/**
 * Static registration for the Ping filter. @see RegisterFactory.
 */
REGISTER_FACTORY(PingConfigFactory, Server::Configuration::NamedNetworkFilterConfigFactory);

} // namespace Ping
} // namespace NetworkFilters
} // namespace Extensions
} // namespace Envoy
