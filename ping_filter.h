#pragma once

#include "envoy/network/filter.h"
#include "envoy/stats/scope.h"
#include "envoy/stats/stats.h"
#include "envoy/stats/stats_macros.h"

#include "common/common/logger.h"

namespace Envoy {
namespace Extensions {
namespace NetworkFilters {
namespace Ping {

/**
 * All ping filter stats. @see stats_macros.h
 */
// clang-format off
#define ALL_PING_FILTER_STATS(COUNTER)                                           \
  COUNTER(total_pings)                                                           \
  COUNTER(total_pongs)                                                           \
  COUNTER(total_errors)                                                          \
// clang-format on

/**
 * Struct definition for all ping filter stats. @see stats_macros.h
 */
struct PingFilterStats {
  ALL_PING_FILTER_STATS(GENERATE_COUNTER_STRUCT)
};

/**
 * Configuration for the Ping filter.
 */
class PingFilterConfig {
public:
  PingFilterConfig(const bool convert_to_upper, Stats::Scope& scope);
  const PingFilterStats& stats() { return stats_; }

  const bool convert_to_upper_;
  PingFilterStats stats_;

private:
  PingFilterStats generateStats(Stats::Scope& scope) {
    return PingFilterStats{ALL_PING_FILTER_STATS(POOL_COUNTER_PREFIX(scope, "ping."))};
  }
};

using PingFilterConfigSharedPtr = std::shared_ptr<PingFilterConfig>;

/**
 * Implementation of a basic ping filter.
 */
class PingFilter : public Network::Filter, Logger::Loggable<Logger::Id::filter> {
public:
  PingFilter(PingFilterConfigSharedPtr config);
  ~PingFilter() override = default;

  // Network::ReadFilter
  Network::FilterStatus onData(Buffer::Instance& data, bool end_stream) override;
  Network::FilterStatus onNewConnection() override { return Network::FilterStatus::Continue; }
  void initializeReadFilterCallbacks(Network::ReadFilterCallbacks& callbacks) override { read_callbacks_ = &callbacks; }

  // Network::WriteFilter
  Network::FilterStatus onWrite(Buffer::Instance& data, bool end_stream) override;

private:
  PingFilterConfigSharedPtr config_;
  Network::ReadFilterCallbacks* read_callbacks_{};
};

} // namespace Ping
} // namespace NetworkFilters
} // namespace Extensions
} // namespace Envoy
