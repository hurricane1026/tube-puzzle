/*
 * Copyright (C) 2021 hurricane <l@stdb.io>. All rights reserved.
 */
#ifndef TUBE_PUZZLE_GAME_HPP_
#define TUBE_PUZZLE_GAME_HPP_

#include "tube.hpp"
#include "fmt/core.h"
#include "yaml-cpp/yaml.h"
#include <boost/lexical_cast.hpp>
#include <vector>

using boost::lexical_cast;
using boost::bad_lexical_cast;

template<std::uint8_t C>
class game {
 public:
  inline game():
    tubes_(),
    hashcodes_()
  {
    YAML::Node game_node = YAML::LoadFile("game.yaml");
    auto node = game_node["empty-tube"];
    std::string empty_tube = node.as<std::string>();

    auto tubes = game_node["tubes"];
    try {
      empty_tube_num_ = boost::lexical_cast<unsigned int>(empty_tube);
      fmt::print("free_empty_tube_num is {}", empty_tube_num_);


      uint16_t no = 0;
      for (auto iter = tubes.begin(); iter != tubes.end(); ++iter) {
        std::string content = iter->as<std::string>();
        tube<C>* temp_tube_ptr = new tube<C>(no++, content);
        tubes_.push_back(temp_tube_ptr);
        fmt::print(content);
      }

      for (unsigned int empty_no = 0; empty_no < empty_tube_num_; ++empty_no)
      {
        tubes_.push_back(new tube<C>(no++));
      }

      hash_record();

    }
    catch(bad_lexical_cast& e){
      fmt::print(e.what());
    }
  }

  inline game(const game& g):
    tubes_(),
    hashcodes_(g.hashcodes)
  {
    for (auto t : g.tubes) {
      tube<C>* new_t = new tube<C>(*t);
      tubes_.push_back(new_t);
    }
  }

  inline ~game() {
    for (auto t : tubes_) {
      delete t;
    }
  }

  inline std::vector<tube<C>> availables() const {
    std::vector<tube<C>*> avails;
    for (auto t : tubes_) {
      if (!t->is_pure()) {
        avails.push_back(t);
      }
    }
    return avails;
  }

  inline std::string genhash() const {
     std::string code;
    for (auto & c : hashcodes_) {
      code += "|";
      code += c;
    }
    return code;
  }

  inline bool hash_record() {
    std::string code = genhash();
    if (hashcodes_.contains(code)) {
      return false;
    } else {
      hashcodes_.insert(code);
      return true;
    }
  }

  inline void show_codes() const {
    for (auto & code : hashcodes_) {
      fmt::print("{}\n", code);
    }
  }

  inline decltype(auto) future_actions() {
    std::vector<std::pair<std::uint8_t, std::uint8_t>> actions;
    for (auto src : tubes_) {
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
    assert(tubes_[src]->get_no() == src);
    assert(tubes_[dst]->get_no() == dst);
    tubes_[src]->fill(*tubes_[dst]);
    return hash_record();
  }

  inline bool success() const {
    for (auto t : tubes_) {
      if (!t->pure() && !t->empty())
        return false;
    }
    return true;
  }

 private:
  unsigned int empty_tube_num_;
  std::vector<tube<C>*> tubes_;
  std::set<std::string> hashcodes_;
};





#endif  // TUBE_PUZZLE_GAME_HPP_
