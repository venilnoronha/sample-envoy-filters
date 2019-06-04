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
