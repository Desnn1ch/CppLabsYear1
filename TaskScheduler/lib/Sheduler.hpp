#pragma once
#include <memory>
#include <vector>

class ProxyTask {
 public:
    virtual const char* RunFunc() const = 0;
    virtual ~ProxyTask() = default;
};

template <typename Range, typename First, typename Second>
class Task : public ProxyTask {
 public:
    Task(Range&& range, First&& first, Second&& second)
        : range_(std::forward<Range>(range)), first_(std::forward<First>(first)), second_(std::forward<Second>(second)) {}

 private:
    Range range_;
    First first_;
    Second second_;

    const char* RunFunc() const override {
        auto value = range_(first_, second_);
        return reinterpret_cast<const char*&>(value);
    }
};

template <typename Range, typename First>
class Task<Range, First, void> : public ProxyTask {
 public:
    Task(Range&& range, First&& first)
        : range_(std::forward<Range>(range)), first_(std::forward<First>(first)) {}

 private:
    Range range_;
    First first_;

    const char* RunFunc() const override {
        auto value = range_(first_);
        return reinterpret_cast<const char*&>(value);
    }
};

template <typename Range>
class Task<Range, void, void> : public ProxyTask {
 public:
    Task(Range&& range) : range_(std::forward<Range>(range)) {}

 private:
    Range range_;

    const char* RunFunc() const override {
        auto value = range_();
        return reinterpret_cast<const char*&>(value);
    }
};

template <typename T>
class Any {
 public:
    explicit Any(std::shared_ptr<T> ptr) : ptr_(std::move(ptr)) {}
    Any(const Any& other) : ptr_(other.ptr_) {}

    const char* RunFunc() const {
        return ptr_->RunFunc();
    }

 private:
    std::shared_ptr<T> ptr_;
};

struct Index {
    size_t id_;
    Index(size_t id) : id_(id) {}
};

template <typename T>
class FutureResult {
 private:
    size_t index_;
    std::vector<const char*>& result_;
    const std::vector<Any<ProxyTask>>& container_;
 public:
    FutureResult(Index index, std::vector<const char*>& result, const std::vector<Any<ProxyTask>>& container)
        : index_(index.id_), result_(result), container_(container) {
        if (index.id_ >= container.size()) {
            throw std::runtime_error("Invalid index");
        }
    }

    operator T() const {
        if (index_ >= result_.size()) {
            throw std::runtime_error("Not calculated yet");
        }
        if (result_[index_] == nullptr) {
            result_[index_] = container_[index_].RunFunc();
        }
        return *reinterpret_cast<const T*>(&result_[index_]);
    }
};

class TaskScheduler {
 public:
    template <typename Range>
    Index Add(Range&& range) {
        auto temp = std::make_shared<Task<Range, void, void>>(std::forward<Range>(range));
        container_.emplace_back(std::move(temp));
        return {curr++};
    }

    template <typename Range, typename First>
    Index Add(Range&& range, First&& first) {
        auto temp = std::make_shared<Task<Range, First, void>>(std::forward<Range>(range), std::forward<First>(first));
        container_.emplace_back(std::move(temp));
        return {curr++};
    }

    template <typename Range, typename First, typename Second>
    Index Add(Range&& range, First&& first, Second&& second) {
        auto temp = std::make_shared<Task<Range, First, Second>>(
            std::forward<Range>(range), std::forward<First>(first), std::forward<Second>(second));
        container_.emplace_back(std::move(temp));
        return {curr++};
    }

    template <typename T>
    FutureResult<T> GetFutureResult(Index index) {
        if (curr < index.id_) {
            throw std::runtime_error("Invalid index");
        }
        return FutureResult<T>(index, result_, container_);
    }

    template <typename T>
    T GetResult(Index index) {
        if (index.id_ >= container_.size()) {
            throw std::runtime_error("Invalid index");
        }
        if (result_.size() < index.id_ + 1) {
            result_.resize(curr + 1, nullptr);
        }
        if (result_[index.id_] == nullptr) {
            result_[index.id_] = container_[index.id_].RunFunc();
        }
        return *reinterpret_cast<const T*>(&result_[index.id_]);
    }

    void ExecuteAll() {
        result_.resize(curr, nullptr);
        for (size_t i = 0; i < curr; ++i) {
            if (result_[i] == nullptr) {
                result_[i] = container_[i].RunFunc();
            }
        }
    }
 private:
    std::vector<Any<ProxyTask>> container_;
    std::vector<const char*> result_;
    size_t curr = 0;
};
