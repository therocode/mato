#pragma once
#include <vector>

template <typename T>
void emplaceOptional(th::Optional<T> data, std::vector<T>& storage)
{
    if(data)
        storage.emplace_back(std::move(*data));
}
