#pragma once

#include <vector>
#include <algorithm>

namespace mario::util{

template<typename T>
struct Generator{
  std::vector<T> items;
  int current_item_index = 0;

  Generator(){
    items.reserve(8);
    items.emplace_back();
    current_item_index = 0;
  };

  auto make_item_if_needed(){
    const auto available_items = std::find_if(items.begin(), items.end(), [](const auto& p){
      return !p.is_active;
    });

    if (available_items == items.end()){
      items.emplace_back();
      current_item_index = items.size() - 1;
      return;
    }

    current_item_index = available_items - items.begin();
 }

  auto& item(){
    return items[current_item_index];
  }

  const auto& item() const{
    return items[current_item_index];
  }
};

} //namespace mario::util
