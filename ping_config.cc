#include <string>

#include "ping.h"

#include "envoy/registry/registry.h"
#include "envoy/server/filter_config.h"

namespace Envoy {
namespace Server {
namespace Configuration {

/**
 * Config registration for the ping filter. @see NamedNetworkFilterConfigFactory.
 */
class PingConfigFactory : public NamedNetworkFilterConfigFactory {
public:
  Network::FilterFactoryCb createFilterFactoryFromProto(const Protobuf::Message&,
                                                        FactoryContext&) override {
    return [](Network::FilterManager& filter_manager) -> void {
      filter_manager.addReadFilter(Network::ReadFilterSharedPtr{new Filter::Ping()});
    };
  }

  ProtobufTypes::MessagePtr createEmptyConfigProto() override {
    return ProtobufTypes::MessagePtr{new Envoy::ProtobufWkt::Empty()};
  }

  std::string name() override { return "ping"; }

  Network::FilterFactoryCb createFilterFactory(const Json::Object&, FactoryContext&) override {
    NOT_IMPLEMENTED_GCOVR_EXCL_LINE;
  }
};

/**
 * Static registration for the ping filter. @see RegisterFactory.
 */
static Registry::RegisterFactory<PingConfigFactory, NamedNetworkFilterConfigFactory> registered_;

} // namespace Configuration
} // namespace Server
} // namespace Envoy
