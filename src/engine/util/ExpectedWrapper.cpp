#include "ExpectedWrapper.h"
#include <stdexcept>

bool ExpectedWrapper::isError() {
    return this->error_code < 0;
}


int ExpectedWrapper::expect(std::function<std::string(void)> error_fn) {
    if (this->isError()) {
        std::string error_message = error_fn();
        throw std::runtime_error(error_message);
    } else {
        return error_code;
    }
}

// template <typename T> bool ExpectedWrapperNull<T>::isError() {
//     return this->obj == nullptr;
// }

// template <typename T> T* ExpectedWrapperNull<T>::expect(std::function<std::string(void)> error_fn) {
//     if (this->isError()) {
//         std::string error_message = error_fn();
//         throw std::runtime_error(error_message);
//     } else {
//         return obj;
//     }
// }