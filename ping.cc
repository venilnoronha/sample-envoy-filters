#include "ping.h"

#include <algorithm>
#include <string>

#include "envoy/buffer/buffer.h"
#include "envoy/network/connection.h"

#include "common/buffer/buffer_impl.h"
#include "common/common/assert.h"

namespace Envoy {
namespace Filter {

Network::FilterStatus Ping::onData(Buffer::Instance& data, bool) {
  std::string in_str = std::string(static_cast<char*>(data.linearize(data.length())), data.length());
  ENVOY_CONN_LOG(info, "ping: read a {} byte string {}", read_callbacks_->connection(), data.length(), in_str);

  std::string out_str = in_str;
  std::transform(out_str.begin(), out_str.end(), out_str.begin(), ::toupper);
  Buffer::OwnedImpl out_data(out_str);

  read_callbacks_->connection().write(out_data, false);
  ENVOY_CONN_LOG(info, "ping: wrote a {} byte string {}", read_callbacks_->connection(), data.length(), out_str);
  return Network::FilterStatus::StopIteration;
}

} // namespace Filter
} // namespace Envoy
