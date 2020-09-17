//  Copyright (c) 2007-2008 Facebook
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//
// See accompanying file LICENSE or visit the Scribe site at:
// http://developers.facebook.com/scribe/
//
// @author Bobby Johnson

#ifndef SCRIBE_ENV
#define SCRIBE_ENV

#include "thrift/protocol/TBinaryProtocol.h"
#include "thrift/server/TNonblockingServer.h"
#include "thrift/concurrency/ThreadManager.h"
#include "thrift/concurrency/PosixThreadFactory.h"
#include "thrift/concurrency/Mutex.h"
#include "thrift/transport/TSocket.h"
#include "thrift/transport/TSocketPool.h"
#include "thrift/transport/TServerSocket.h"
#include "thrift/transport/TTransportUtils.h"
#include "thrift/transport/THttpClient.h"
#include "thrift/transport/TFileTransport.h"
#include "thrift/transport/TBufferTransports.h"
#include "thrift/transport/TSimpleFileTransport.h"

#include "fb303/FacebookBase.h"

#include "src/gen-cpp/scribe.h"
#include "src/gen-cpp/BucketStoreMapping.h"

typedef boost::shared_ptr<scribe::thrift::LogEntry> logentry_ptr_t;
typedef std::vector<logentry_ptr_t> logentry_vector_t;
typedef std::vector<std::pair<std::string, int> > server_vector_t;

// scribe version
const std::string scribeversion("2.2");
#define DEFAULT_CONF_FILE_LOCATION "/usr/local/scribe/scribe.conf"

/*
 * This file contains methods for handling tasks that depend
 * on the environment in which this process is running.
 * i.e. using scribe will not force the adoption of a particular
 * system for monitoring or configuring services over the network.
 */

/*
 * Debug logging
 */
#define LOG_OPER(format_string,...)                                     \
  {                                                                     \
    time_t now;                                                         \
    char dbgtime[26] ;                                                  \
    time(&now);                                                         \
    ctime_r(&now, dbgtime);                                             \
    dbgtime[24] = '\0';                                                 \
    fprintf(stderr,"[%s] " #format_string " \n", dbgtime,##__VA_ARGS__); \
  }


namespace scribe {

/*
 * Network based configuration and directory service
 */

namespace network_config {
  // gets a vector of machine/port pairs for a named service
  // returns true on success
  bool getService(const std::string& serviceName,
                         const std::string& options,
                         server_vector_t& _return);

} // !namespace scribe::network_config

/*
 * Concurrency mechanisms
 */

namespace concurrency {
  using apache::thrift::concurrency::ReadWriteMutex;

  // returns a new instance of read/write mutex.
  // you can choose different implementations based on your needs.
  boost::shared_ptr<ReadWriteMutex> createReadWriteMutex();

} // !namespace scribe::concurrency

/*
 * Time functions
 */

namespace clock {
  unsigned long nowInMsec();

} // !namespace scribe::clock

/*
 * Hash functions
 */

// You can probably find better hash functions than these
class integerhash {
 public:
  static uint32_t hash32(uint32_t key);
};

class strhash {
 public:
  static uint32_t hash32(const char *s);
};

/*
 * Starting a scribe server.
 */
void startServer();

/*
 * Stopping a scribe server.
 */
void stopServer();

} // !namespace scribe

#endif // SCRIBE_ENV
