/*
 * Copyright (C) 2021 hurricane <l@stdb.io>. All rights reserved.
 */
#include "game.hpp"
#include <gtest/gtest.h>



class game_test : public ::testing::Test {
 protected:
   game<4> g;
};

TEST_F(game_test, smoke_Test) {
  g.show_codes();
};
