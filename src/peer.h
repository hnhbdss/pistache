/* peer.h
   Mathieu Stefani, 12 August 2015
   
  A class representing a TCP Peer
*/

#pragma once

#include "net.h"
#include "os.h"
#include "async.h"
#include <string>
#include <iostream>
#include <memory>
#include <unordered_map>

namespace Net {

namespace Tcp {

class IoWorker;

class Peer {
public:
    friend class IoWorker;

    Peer();
    Peer(const Address& addr);

    Address address() const;
    std::string hostname() const;

    void associateFd(Fd fd);
    Fd fd() const;

    void putData(std::string name, std::shared_ptr<void> data);

    std::shared_ptr<void> getData(std::string name) const;
    template<typename T>
    std::shared_ptr<T> getData(std::string name) const {
        return std::static_pointer_cast<T>(getData(std::move(name)));
    }

    std::shared_ptr<void> tryGetData(std::string name) const;
    template<typename T>
    std::shared_ptr<T> tryGetData(std::string name) const {
        auto data = tryGetData(std::move(name));
        if (data == nullptr) return nullptr;

        return std::static_pointer_cast<T>(data);
    }

    Async::Promise<ssize_t> send(const void* buf, size_t len);

private:
    IoWorker* io_;

    Address addr;
    std::string hostname_;
    Fd fd_;
    std::unordered_map<std::string, std::shared_ptr<void>> data_;
};

std::ostream& operator<<(std::ostream& os, const Peer& peer);

} // namespace Tcp

} // namespace Net
