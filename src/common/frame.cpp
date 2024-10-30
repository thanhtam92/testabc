#include "common/frame.h"


namespace onevu {

FrameNotificationQueue::FrameNotificationQueue(){

}

FrameNotificationQueue::~FrameNotificationQueue(){

}

void FrameNotificationQueue::enqueueNotification(Poco::Notification::Ptr pNotification) {
    if(_ntfQueue.size() < _maxSize){
        _ntfQueue.enqueueNotification(pNotification);
    }
}

Poco::Notification* FrameNotificationQueue::waitDequeueNotification() {
    return _ntfQueue.waitDequeueNotification();
}
Poco::Notification* FrameNotificationQueue::waitDequeueNotification(long milliseconds) {
    return _ntfQueue.waitDequeueNotification(milliseconds);
}

Poco::Notification* FrameNotificationQueue::dequeueNotification() {
    return _ntfQueue.dequeueNotification();
}

void FrameNotificationQueue::wakeUpAll() {
    _ntfQueue.wakeUpAll();
}

size_t FrameNotificationQueue::size() {
    return _ntfQueue.size();
}

void FrameNotificationQueue::clear() {
    _ntfQueue.clear();
}

} // namespace