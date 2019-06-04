/*
 * Copyright 2019 VMware, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
