/*
 * Copyright (C) 2021 hurricane <l@stdb.io>. All rights reserved.
 */
#ifndef TUBE_PUZZLE_GAME_HPP_
#define TUBE_PUZZLE_GAME_HPP_

#include "tube.hpp"
#include "fmt/core.h"

template<std::uint16_t N, std::uint8_t C>
class game {
 public:
  inline game(const std::string& path) {

  }

  inline game(const game& g):
    tubes(),
    hashcodes(g.hashcodes)
  {
    for (auto t : g.tubes) {
      tube<C>* new_t = new tube<C>(*t);
      tubes.push_back(new_t);
    }
  }

  inline ~game() {
    for (auto t : tubes) {
      delete t;
    }
  }

  inline std::vector<tube<C>> availables() const {
    std::vector<tube<C>*> avails;
    for (auto t : tubes) {
      if (!t->is_pure()) {
        avails.push_back(t);
      }
    }
    return avails;
  }

  inline std::string genhash() const {
     std::string code;
    for (auto & c : hashcodes) {
      code += "|";
      code += c;
    }
    return code;
  }

  inline bool hash_record() {
    std::string code = genhash();
    if (hashcodes.contains(code)) {
      return false;
    } else {
      hashcodes.insert(code);
      return true;
    }
  }

  inline void show_codes() const {
    for (auto & code : hashcodes) {
      fmt::print("{}\n", code);
    }
  }

  inline decltype(auto) future_actions() {
    std::vector<std::pair<std::uint8_t, std::uint8_t>> actions;
    for (auto src : tubes) {
      auto [src_slot, src_color] = src->top_slots();
      if (src_color == 0) continue;
      //std::uint16_t curr_no = src->get_no();
      for (auto dst : availables()) {
        if (dst == src) continue;
        auto [dst_slot, dst_color] = dst->top_slots();
        if (dst_slot == 0) continue;
        if (color_comp(dst_color, src_color)) {
          actions.push_back(std::make_pair(src->get_no(), dst->get_no()));
        }
      }
    }
    return actions;
  }

  inline bool do_fill(std::pair<std::uint8_t, std::uint8_t>& action) {
    auto [src, dst] = action;
    assert(tubes[src]->get_no() == src);
    assert(tubes[dst]->get_no() == dst);
    tubes[src]->fill(*tubes[dst]);
    return hash_record();
  }

  inline bool success() const {
    for (auto t : tubes) {
      if (!t->pure() && !t->empty())
        return false;
    }
    return true;
  }

 private:
  std::vector<tube<C>*> tubes;
  std::set<std::string> hashcodes;
};





#endif  // TUBE_PUZZLE_GAME_HPP_
