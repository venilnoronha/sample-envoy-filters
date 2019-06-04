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

#include <algorithm>
#include <string>

#include "ping_filter.h"

#include "envoy/network/connection.h"

#include "common/buffer/buffer_impl.h"
#include "common/common/assert.h"

#include "absl/strings/match.h"

namespace Envoy {
namespace Extensions {
namespace NetworkFilters {
namespace Ping {

PingFilterConfig::PingFilterConfig(const bool convert_to_upper, Stats::Scope& scope)
    : convert_to_upper_(convert_to_upper), stats_(generateStats(scope)) {}

PingFilter::PingFilter(PingFilterConfigSharedPtr config) : config_(std::move(config)) {}

Network::FilterStatus PingFilter::onData(Buffer::Instance& data, bool) {
  const uint64_t len = data.length();
  std::string in_str = std::string(static_cast<char*>(data.linearize(len)), len);
  ENVOY_CONN_LOG(info, "ping: read a {} byte string on read path with value {}", read_callbacks_->connection(), len, in_str);

  if (absl::EqualsIgnoreCase(in_str, "ping\n")) {
    config_->stats_.total_pings_.inc();
  } else {
    config_->stats_.total_errors_.inc();
    data.drain(len);
    return Network::FilterStatus::StopIteration;
  }

  if (config_->convert_to_upper_) {
    std::string upper_str = in_str;
    std::transform(upper_str.begin(), upper_str.end(), upper_str.begin(), ::toupper);
    data.drain(len);
    data.add(upper_str);
  }

  return Network::FilterStatus::Continue;
}

Network::FilterStatus PingFilter::onWrite(Buffer::Instance& data, bool) {
  const uint64_t len = data.length();
  std::string out_str = std::string(static_cast<char*>(data.linearize(len)), len);
  ENVOY_CONN_LOG(info, "ping: read a {} byte string on write path with value {}", read_callbacks_->connection(), len, out_str);

  if (absl::EqualsIgnoreCase(out_str, "pong\n")) {
    config_->stats_.total_pongs_.inc();
  }
  return Network::FilterStatus::Continue;
}

} // namespace Ping
} // namespace NetworkFilters
} // namespace Extensions
} // namespace Envoy
