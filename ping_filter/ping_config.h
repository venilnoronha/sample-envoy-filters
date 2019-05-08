#pragma once

#include "ping_filter.h"
#include "ping_filter/ping_filter.pb.h"
#include "ping_filter/ping_filter.pb.validate.h"

#include "extensions/filters/network/common/factory_base.h"
#include "extensions/filters/network/well_known_names.h"

namespace Envoy {
namespace Extensions {
namespace NetworkFilters {
namespace Ping {

/**
 * Config registration for the Ping filter.
 */
class PingConfigFactory : public Envoy::Extensions::NetworkFilters::Common::FactoryBase<config::PingFilter> {
public:
  PingConfigFactory() : FactoryBase("envoy.filters.network.ping") {}

private:
  Network::FilterFactoryCb createFilterFactoryFromProtoTyped(
      const config::PingFilter& proto_config,
      Server::Configuration::FactoryContext& context) override;
};

}  // namespace Ping
}  // namespace NetworkFilters
}  // namespace Extensions
} // namespace Envoy
