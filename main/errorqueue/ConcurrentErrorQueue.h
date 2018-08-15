#ifndef SORBET_CONCURRENT_ERRORQUEUE_H
#define SORBET_CONCURRENT_ERRORQUEUE_H
#include "common/concurrency/ConcurrentQueue.h"
#include "core/ErrorQueue.h"
#include "core/ErrorQueueMessage.h"
#include "core/lsp/QueryResponse.h"
#include "spdlog/spdlog.h"
#include <atomic>
#include <climits> // INT_MAX

namespace spd = spdlog;

namespace sorbet {
namespace realmain {
// This class has to be in a separate header to make us compile /facepalm:
//  - parser defines a constant TRUE in header generated by bison\regel
//  - ConcurrentBoundedQueue on mac includes some OS headers that "#define TRUE 1"
//

class ConcurrentErrorQueue : public core::ErrorQueue {
private:
    virtual void checkOwned() override;
    std::thread::id owner;
    UnorderedMap<core::FileRef, std::vector<core::ErrorQueueMessage>> collected;
    ConcurrentUnBoundedQueue<core::ErrorQueueMessage> queue;

    void collectForFile(core::FileRef whatFile, std::vector<std::unique_ptr<core::ErrorQueueMessage>> &out);
    virtual std::vector<std::unique_ptr<core::ErrorQueueMessage>> drainFlushed() override;
    virtual std::vector<std::unique_ptr<core::ErrorQueueMessage>> drainAll() override;

    void drainQueue();
    std::vector<core::ErrorQueueMessage> drainKind(core::ErrorQueueMessage::Kind kind);

public:
    ConcurrentErrorQueue(spd::logger &logger, spd::logger &tracer);
    ~ConcurrentErrorQueue();

    virtual void pushError(const core::GlobalState &gs, std::unique_ptr<core::BasicError> error) override;
    virtual void pushQueryResponse(std::unique_ptr<core::QueryResponse> error) override;
    virtual void markFileForFlushing(core::FileRef file) override;
};
} // namespace realmain
} // namespace sorbet

#endif // SORBET_ERRORQUEUE_H
