#include <algorithm>
#include <string>

#include "ping_filter.h"

#include "envoy/buffer/buffer.h"
#include "envoy/network/connection.h"

#include "common/buffer/buffer_impl.h"
#include "common/common/assert.h"

namespace Envoy {
namespace Extensions {
namespace NetworkFilters {
namespace Ping {

PingFilterConfig::PingFilterConfig(const bool convert_to_upper, Stats::Scope& scope)
    : convert_to_upper_(convert_to_upper), stats_(generateStats(scope)) {}

PingFilter::PingFilter(PingFilterConfigSharedPtr config) : config_(std::move(config)) {}

Network::FilterStatus PingFilter::onData(Buffer::Instance& data, bool) {
  std::string in_str = std::string(static_cast<char*>(data.linearize(data.length())), data.length());
  ENVOY_CONN_LOG(info, "ping: read a {} byte string on read path with value {}", read_callbacks_->connection(), data.length(), in_str);

  std::string out_str = in_str;
  std::transform(out_str.begin(), out_str.end(), out_str.begin(), ::toupper);
  Buffer::OwnedImpl out_data(out_str);

  read_callbacks_->connection().write(out_data, false);
  ENVOY_CONN_LOG(info, "ping: wrote a {} byte string on read path with value {}", read_callbacks_->connection(), data.length(), out_str);
  return Network::FilterStatus::StopIteration;
}

Network::FilterStatus PingFilter::onWrite(Buffer::Instance& data, bool) {
  std::string in_str = std::string(static_cast<char*>(data.linearize(data.length())), data.length());
  ENVOY_CONN_LOG(info, "ping: read a {} byte string on write path with value {}", read_callbacks_->connection(), data.length(), in_str);
  return Network::FilterStatus::Continue;
}

} // namespace Ping
} // namespace NetworkFilters
} // namespace Extensions
} // namespace Envoy
