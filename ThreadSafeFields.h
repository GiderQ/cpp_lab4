#ifndef THREADSAFEFIELDS_H
#define THREADSAFEFIELDS_H

#include <array>
#include <format>
#include <mutex>
#include <shared_mutex>
#include <string>

template <typename FieldType>
class ThreadSafeFields {
    static constexpr int kFieldCount = 3;

    std::array<FieldType, kFieldCount> fields_;
    std::array<std::shared_mutex, kFieldCount> mutexes_;

    static bool IsFieldNumberValid(const int fieldNumber) {
        return fieldNumber >= 0 && fieldNumber < kFieldCount;
    }

public:
    explicit operator std::string() {
        std::shared_lock lock1(mutexes_[0], std::defer_lock);
        std::shared_lock lock2(mutexes_[1], std::defer_lock);
        std::shared_lock lock3(mutexes_[2], std::defer_lock);
        std::lock(lock1, lock2, lock3);

        return std::format("field 0 = '{}', field 1 = '{}', field 2 = '{}'",
            fields_[0], fields_[1], fields_[2]);
    }

    void Write(const int fieldNumber, FieldType value) {
        if (!IsFieldNumberValid(fieldNumber)) return;

        std::unique_lock lock(mutexes_[fieldNumber]);
        fields_[fieldNumber] = value;
    }

    FieldType Read(const int fieldNumber) {
        if (!IsFieldNumberValid(fieldNumber)) return FieldType{};
        std::shared_lock lock(mutexes_[fieldNumber]);
        return fields_[fieldNumber];
    }
};

#endif  // THREADSAFEFIELDS_H
