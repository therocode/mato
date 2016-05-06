#pragma once
#include <thero/optional.hpp>
#include <vector>

template <typename T>
void emplaceOptional(th::Optional<T> data, std::vector<T>& storage)
{
    if(data)
        storage.emplace_back(std::move(*data));
}

template <typename T, typename Func>
T* findIf(std::vector<T>& storage, Func func)
{
    auto result = std::find_if(storage.begin(), storage.end(), func);

    if(result != storage.end())
        return &*result;
    else
        return nullptr;
}

template <typename T, typename Func>
const T* findIf(const std::vector<T>& storage, Func func)
{
    auto result = std::find_if(storage.begin(), storage.end(), func);

    if(result != storage.end())
        return &*result;
    else
        return nullptr;
}
