#pragma once
#include <string>
#include <functional>
#include <stdexcept>

class ExpectedWrapper {
    private:
        int error_code;

        virtual bool isError();
    public:
        ExpectedWrapper(int error_code) :
            error_code(error_code)
        {};

        // return val, or throw error if is an error
        // error_fn -> function returns the error_message
        int expect(std::function<std::string(void)> error_fn);
};

template <typename T> class ExpectedWrapperNull {
    private:
        T* obj;

        virtual bool isError();
    public:
        ExpectedWrapperNull(T* obj) :
            obj(obj)
        {};

        // return val, or throw error if is an error
        // error_fn -> function returns the error_message
        T* expect(std::function<std::string(void)> error_fn);
};

template <typename T> bool ExpectedWrapperNull<T>::isError() {
    return this->obj == nullptr;
}

template <typename T> T* ExpectedWrapperNull<T>::expect(std::function<std::string(void)> error_fn) {
    if (this->isError()) {
        std::string error_message = error_fn();
        throw std::runtime_error(error_message);
    } else {
        return obj;
    }
}