#pragma once

#include <vector>
#include <iterator>
#include <math.h>
#include <random>

namespace sort_algorithms {

template <typename _DataType>
void _counting_argsort_call(_DataType begin, _DataType end, _DataType result,
                           std::iter_value_t<_DataType> max) {
  auto size = std::distance(begin, end);

  std::vector<int> counts_array(max + 1, 0);
  for (auto iter = begin; iter != end; ++iter) {
    ++counts_array[*iter];
  }

  for (int i = 1; i < max + 1; ++i) {
    counts_array[i] += counts_array[i - 1];
  }

  for (int i = size - 1; i >= 0; --i) {
    auto original_number = *(begin + i);
    auto pos = counts_array[original_number] - 1;
    result[pos] = i;
    --counts_array[original_number];
  }
}

template <typename _DataType, typename Comparator>
void _insertion_argsort_call(_DataType begin, _DataType end, _DataType result,
                            Comparator comp) {
  for (auto iter = begin; iter != end; ++iter) {
    for (auto cur_element = iter; cur_element != begin; --cur_element) {
      if (comp(*cur_element, *(cur_element - 1))) {
        std::iter_swap(cur_element, cur_element - 1);
        auto i = std::distance(begin, cur_element);
        auto j = std::distance(begin, cur_element - 1);
        std::iter_swap(result + i, result + j);
      } else {
        break;
      }
    }
  }
}

template <typename _DataType>
std::pair<_DataType, _DataType> partition(_DataType begin, _DataType end,
                                          _DataType result, int random) {
  auto pivot = *(begin + random);
  auto left = begin;
  auto right = begin;
  auto upper_bound = begin + std::distance(begin, end) - 1;

  while (std::distance(begin, right) <= std::distance(begin, upper_bound)) {
    if (*right < pivot) {
      std::iter_swap(left, right);
      std::iter_swap(result + std::distance(begin, left),
                     result + std::distance(begin, right));
      ++left;
      ++right;
    } else if (*right > pivot) {
      std::iter_swap(right, upper_bound);
      std::iter_swap(result + std::distance(begin, right),
                     result + std::distance(begin, upper_bound));
      --upper_bound;
    } else {
      ++right;
    }
  }

  return {left, right};
}

template <typename _DataType>
void _quick_argsort_rec(_DataType begin, _DataType end, _DataType result,
                        std::mt19937 &generator) {
  for (auto size = std::distance(begin, end); size >= Limit;
       size = std::distance(begin, end)) {
    std::uniform_int_distribution<int> distribution(0, size);
    auto random = distribution(generator);
    auto [l, r] = partition(begin, end, result, random);
    if (std::distance(begin, l) <= std::distance(r, end)) {
      _quick_argsort_rec(begin, l, result, generator);
      result += std::distance(begin, r);
      begin = r;
    } else {
      _quick_argsort_rec(r, end, result + std::distance(begin, r), generator);
      end = l;
    }
  }
  _insertion_argsort_call(begin, end, result, std::less());
}

template <typename _DataType>
void _radix_argsort_call(_DataType begin, _DataType end, _DataType result) {
  if (std::distance(begin, end) < 2) {
    return;
  }

  auto bits_count = sizeof(std::iter_value_t<_DataType>) * 8;
  auto bits_per_loop = 0;

  switch (bits_count) {
  case 8:
  case 16:
    bits_per_loop = bits_count;
    break;
  case 32:
    bits_per_loop = bits_count / 2;
    break;
  case 64:
    bits_per_loop = bits_count / 4;
    break;
  }
  auto loops = bits_count / bits_per_loop;
  auto mask = (1 << bits_per_loop) - 1;
  std::vector<size_t> counts(1 << bits_per_loop);
  std::vector<std::iter_value_t<_DataType>> temp(std::distance(begin, end));
  std::vector<std::iter_value_t<_DataType>> temp_indices(
      std::distance(begin, end));
  for (int p = 0; p < loops; ++p) {
    std::fill(counts.begin(), counts.end(), 0);
    std::copy(result, result + std::distance(begin, end), temp_indices.begin());

    for (auto iter = begin; iter != end; ++iter) {
      auto cur_bits = ((*iter) >> (bits_per_loop * p)) & mask;
      ++counts[cur_bits];
    }

    for (int i = 1; i < counts.size(); ++i) {
      counts[i] += counts[i - 1];
    }
    for (int i = std::distance(begin, end) - 1; i >= 0; --i) {
      auto num = *(begin + i);
      auto cur_bits = (num >> (bits_per_loop * p)) & mask;
      auto pos = counts[cur_bits] - 1;
      temp[pos] = num;
      *(result + pos) = temp_indices[i];
      --counts[cur_bits];
    }
    std::copy(temp.begin(), temp.end(), begin);
  }
}

template <typename _DataType>
void backend_counting_argsort(
    _DataType begin, _DataType end, _DataType result){
  auto size = std::distance(begin, end);
  if (size < 2) {
    return;
  }
  std::vector tmp(begin, end);
  auto max = *(std::max_element(tmp.begin(), tmp.end()));
  _counting_argsort_call(tmp.begin(), tmp.end(), result, max);
}

template <typename _DataType, typename Comparator>
void backend_insertion_argsort(_DataType begin, _DataType end, _DataType result,
                       Comparator comp) requires std::random_access_iterator<_DataType>{
  auto size = std::distance(begin, end);
  if (size < 2) {
    return;
  }
  std::vector tmp(begin, end);
  std::iota(result, result + std::distance(begin, end), 0);
  _insertion_argsort_call(tmp.begin(), tmp.end(), result, comp);
}

template <typename _DataType>
void backend_radix_argsort(_DataType begin, _DataType result, size_t size){
  _DataType end = begin + size;
  std::vector tmp(begin, end);
  std::iota(result, result + std::distance(begin, end), 0);
  _radix_argsort_call(tmp.begin(), tmp.end(), result);
}

template <typename _DataType>
void backend_bucket_argsort(
    _DataType begin, _DataType result, size_t size) 
   requires std::random_access_iterator<_DataType> {
  // auto size = std::distance(begin, end);
  _DataType end = begin + size;

  if (size < 2) {
    return;
  }
  auto num_buckets = std::distance(begin, end);
  auto max = *(std::max_element(begin, end));
  auto bucket_range = (uint64_t)ceil(((double)max) / num_buckets);
  using value_t = std::iter_value_t<_DataType>;
  using index_t = std::iter_value_t<_DataType>;
  using val_index_t = std::pair<value_t, index_t>;
  std::vector<std::vector<val_index_t>> buckets(num_buckets + 1,
                                           std::vector<val_index_t>());
  for (auto iter = begin; iter != end; ++iter) {
    if (*iter == 0) {
      buckets[0].emplace_back(*iter, std::distance(begin, iter));
    } else {
      auto index = (int)floor((*iter / bucket_range));
      buckets[index].emplace_back(*iter, std::distance(begin, iter));
    }
  }
  for (auto &bucket : buckets) {
    _insertion_argsort_call(bucket.begin(), bucket.end(), std::less<val_index_t>());
  }
  for (auto &bucket : buckets) {
    for (auto &pair : bucket) {
      *result = pair.second;
      ++result;
    }
  }
}

template <typename _DataType>
void backend_quick_argsort(_DataType begin, _DataType result, size_t size) 
    requires std::random_access_iterator<_DataType> {
  static thread_local std::mt19937 generator(std::random_device{}());
  _DataType end = begin + size;
  std::vector tmp(begin, end);
  std::iota(result, result + std::distance(begin, end), 0);
  _quick_argsort_rec<decltype(tmp.begin())>(
      tmp.begin(), tmp.end(), result, generator);
}
} // namespace sort_algorithms
