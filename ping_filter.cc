#include <algorithm>
#include <string>

#include "ping_filter.h"

#include "envoy/buffer/buffer.h"
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
  data.drain(len);

  if (absl::StartsWithIgnoreCase(in_str, "ping")) {
    config_->stats_.total_pings_.inc();
  } else {
    config_->stats_.total_errors_.inc();
  }

  std::string out_str = in_str;
  std::transform(out_str.begin(), out_str.end(), out_str.begin(), ::toupper);
  Buffer::OwnedImpl out_data(out_str);

  read_callbacks_->connection().write(out_data, false);
  ENVOY_CONN_LOG(info, "ping: wrote a {} byte string on read path with value {}", read_callbacks_->connection(), len, out_str);
  return Network::FilterStatus::StopIteration;
}

Network::FilterStatus PingFilter::onWrite(Buffer::Instance& data, bool) {
  const uint64_t len = data.length();
  std::string in_str = std::string(static_cast<char*>(data.linearize(len)), len);
  ENVOY_CONN_LOG(info, "ping: read a {} byte string on write path with value {}", read_callbacks_->connection(), len, in_str);
  return Network::FilterStatus::Continue;
}

} // namespace Ping
} // namespace NetworkFilters
} // namespace Extensions
} // namespace Envoy
