#pragma once
#include <algorithm>

template<class ForwardIt, class T, class Compare>
ForwardIt binarySearch(ForwardIt first, ForwardIt last, const T& value, Compare comp)
{
    for(ForwardIt iter = first; iter != last; ++iter)
    {
        if(comp(value, *iter))
            return iter;
    }
    
    return last;
}
