/*
 * Copyright (C) 2021 hurricane <l@stdb.io>. All rights reserved.
 */
#include "game.hpp"
#include <gtest/gtest.h>


class game_test : public ::testing::Test {
 protected:
   void SetUp() override {
     g = new game<4>("game.yaml");
   }
   void TearDown() override {
    delete g;
   }
   //const std::string file = "game.yaml";
   game<4>* g;
};

TEST_F(game_test, smoke_Test) {
  g->show_codes();
};

TEST_F(game_test, next_Test) {
  auto next_games = g->next();
  ASSERT_EQ(next_games.size(), 24);
  for(auto& g : next_games)  {
    fmt::print("----------------\n");
    g->show_codes();
    fmt::print("----------------\n");
  }
}
