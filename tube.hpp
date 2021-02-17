/*
 * Copyright (C) 2021 hurricane <l@stdb.io>. All rights reserved.
*/


#ifndef TUBE_PUZZLE_TUBE_HPP_
#define TUBE_PUZZLE_TUBE_HPP_

#include <vector>
#include <cstdint>
#include <string>
#include <tuple>
#include <cassert>
#include <set>

inline bool color_comp(char dst, char src) {
  assert(src != 0);
  if (dst == 0) return true;
  else return dst == src;
}

template<std::uint8_t C>
class tube;

template<std::uint8_t C>
bool operator == (const tube<C>& t1, const tube<C>& t2);

template<std::uint8_t C>
class tube {
 using CV = std::vector<char>;
 public:
  inline tube(int16_t no, const std::string& input):
    no(no),
    content(input.begin(), input.end())
  { }
  inline tube(int16_t no, const CV& input):
    no(no),
    content(input)
  { }

  inline tube(int16_t no):
    no(no),
    content()
  {
    content.reserve(C);
  }

  inline tube(const tube& t):
    no(t.no),
    content(t.content)
  { }

  inline std::string show() const {
    return std::string(content);
  }

  inline bool pure() const {
    if (content.size() == C) {
      auto c = content.front();
      for (auto cc : content) {
        if (cc != c) return false;
      }
      return true;
    }
    return false;
  }

  inline bool empty() const {
    return content.empty();
  }

  inline std::tuple<std::uint8_t, char> top_slots() const {
    if (empty()) {
      return std::make_tuple(C, 0);
    } else {
      return std::make_tuple(C - content.size(), content.back());
    }
  }

  // if the tube is full, and the rest color will still be color
  inline bool push(CV& color) {
    auto [top_slot_num, top_color] = top_slots();
    if (top_slot_num == 0)
      return false;
    else if (!color_comp(top_color, color.front())) {
      return false;
    }
    else {
      std::uint16_t push_len;
      std::uint16_t rest_of_color_len;
      if (top_slot_num <= color.size()) {
        push_len = top_slot_num;
        rest_of_color_len = color.size() - top_slot_num;
      } else {
        push_len = color.size();
        rest_of_color_len = 0UL;
      }

      for (auto i = 0; i < push_len; ++i) {
        content.push_back(color.front());
      }
      color.resize(rest_of_color_len);
      return true;
    }
  }

  inline CV pop() {
    if (content.empty()) {
      return CV();
    } else {
      char top_color = content.back();
      std::uint16_t pop_size = 0;
      for (auto i = content.rbegin(); i != content.rend(); ++i) {
        if (*i == top_color) {
          pop_size++;
        } else break;
      }
      content.resize(content.size() - pop_size);
      // this is for RVO
      return CV(pop_size, top_color);
    }
  }

  inline bool fill(tube& dst) {
    if (this == &dst) {
      return false;
    }
    auto [dst_slot_num, dst_top_color] = dst.top_slots();
    if (dst_slot_num == 0)
      return false;
    auto [src_slot_num, src_top_color] = this->top_slots();
    if (src_slot_num == C) return false;
    if (!color_comp(dst_top_color, src_top_color))
      return false;
    auto water = this->pop();
    bool done = dst.push(water);
    if (!water.empty()) {
      this->push(water);
    }
    return done;
  }

  inline std::uint16_t get_no() const {
    return no;
  }

 private:
  std::int16_t no;
  std::vector<char> content;

  friend bool operator ==<> (const tube<C>&, const tube<C>&);
};

template<std::uint8_t C>
bool operator == (const tube<C>& t1, const tube<C>& t2) {
  return t1.content == t2.content;
}



#endif  // TUBE_PUZZLE_TUBE_HPP_
