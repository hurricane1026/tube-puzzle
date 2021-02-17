/*
 * Copyright (C) 2021 hurricane <l@stdb.io>. All rights reserved.
 */


#include <gtest/gtest.h>

#include <tube.hpp>

using CV = std::vector<char>;

class TubeTest : public ::testing::Test {
 protected:
  tube<4> t1 {1, "1234"};
  tube<4> t2 {2, "1111"};
  tube<4> t3 {3};
  tube<4> t4 {4, "11"};
  tube<4> t5 {5, "11"};
  tube<4> t6 {6, "1234"};
  tube<4> t7 {7, "123"};
  tube<4> t8 {8, "24"};

};

TEST_F(TubeTest, EQ_Test) {
  ASSERT_EQ(t4, t5);
  ASSERT_TRUE(t4==t5);
  ASSERT_EQ(t1, t6);
  ASSERT_NE(t2, t4);
};

TEST_F(TubeTest, CstrTest) {

  ASSERT_NE(t1, t2);
  ASSERT_NE(t1, t3);
  auto t = t1;
  ASSERT_EQ(t1, t);
};

TEST_F(TubeTest, PureTest) {
  ASSERT_FALSE(t1.pure());
  ASSERT_TRUE(t2.pure());
  ASSERT_FALSE(t3.pure());
  ASSERT_FALSE(t4.pure());
};

TEST_F(TubeTest, EmptyTest) {
  ASSERT_FALSE(t1.empty());
  ASSERT_FALSE(t2.empty());
  ASSERT_TRUE(t3.empty());
  ASSERT_FALSE(t4.empty());
};

TEST_F(TubeTest, top_slots_TEST) {
  auto [num1, top_color1] = t1.top_slots();
  ASSERT_EQ(num1, 0);
  ASSERT_EQ(top_color1, '4');

  auto [num2, top_color2] = t3.top_slots();
  ASSERT_EQ(num2, 4);
  ASSERT_EQ(top_color2, 0);

  auto [num3, top_color3] = t4.top_slots();
  ASSERT_EQ(num3, 2);
  ASSERT_EQ(top_color3, '1');
};

TEST_F(TubeTest, push_fail_Test) {

  CV water {'1', '1'};
  auto ok = t1.push(water);
  ASSERT_FALSE(ok);
  ASSERT_EQ(water.size(), 2);

  ok = t2.push(water);
  ASSERT_FALSE(ok);
  ASSERT_EQ(water.size(), 2);


};

TEST_F(TubeTest, push_empty_Test) {
  CV water {'1', '1'};
  auto ok = t3.push(water);
  ASSERT_TRUE(ok);
  ASSERT_EQ(water.size(), 0);
  auto [slots, color] = t3.top_slots();
  ASSERT_EQ(slots, 2);
  ASSERT_EQ(color, '1');
};

TEST_F(TubeTest, push_ok_norest_Test) {
  CV water {'1', '1'};
  auto ok = t4.push(water);
  ASSERT_TRUE(ok);
  ASSERT_EQ(water.size(), 0);

  CV water2 {'1', '1'};
  ok = t3.push(water);
  ASSERT_TRUE(ok);
  ASSERT_EQ(water.size(), 0);
};

TEST_F(TubeTest, push_ok_rest_Test) {
  CV water {'3', '3'};
  auto ok = t7.push(water);
  ASSERT_TRUE(ok);
  ASSERT_EQ(water.size(), 1);

  CV water1 {'1', '1', '1'};
  ok = t4.push(water1);
  ASSERT_TRUE(ok);
  ASSERT_EQ(water1.size(), 1);
};

TEST_F(TubeTest, pop_Test) {
  auto water1 = t1.pop();
  std::vector<char> temp_water_4 {'4'};
  ASSERT_EQ(water1, temp_water_4);
  auto water2 = t2.pop();
  std::vector<char> temp_water_1 {'1', '1', '1', '1'};
  ASSERT_EQ(water2, temp_water_1);
  auto water3 = t3.pop();
  ASSERT_EQ(water3.size(), 0);

  auto water4 = t4.pop();
  std::vector<char> temp_water_11 {'1', '1'};
  ASSERT_EQ(water4, temp_water_11);

  auto water7 = t7.pop();
  std::vector<char> temp_water_3 {'3'};
  ASSERT_EQ(water7, temp_water_3);

};

TEST_F(TubeTest, fill_fail_Test) {
  auto done = t4.fill(t2);
  ASSERT_FALSE(done);
  auto [slot, top_c] = t2.top_slots();
  ASSERT_EQ(slot, 0);
  ASSERT_EQ(top_c, '1');

  done = t4.fill(t1);
  ASSERT_FALSE(done);
  auto [slot1_, top_c1_] = t1.top_slots();
  ASSERT_EQ(slot1_, 0);
  ASSERT_EQ(top_c1_, '4');


  done = t1.fill(t6);
  ASSERT_FALSE(done);
  auto [slot1, top_c1] = t1.top_slots();
  auto [slot6, top_c6] = t6.top_slots();
  ASSERT_EQ(slot1, 0);
  ASSERT_EQ(top_c1, '4');
  ASSERT_EQ(slot6, 0);
  ASSERT_EQ(top_c6, '4');
};

TEST_F(TubeTest, fill_from_empty_Test) {
  auto done = t3.fill(t5);
  ASSERT_FALSE(done);
};

TEST_F(TubeTest, fill_from_self_Test) {
  auto done = t3.fill(t3);
  ASSERT_FALSE(done);
};


TEST_F(TubeTest, fill_ok_Test1) {
  auto done = t4.fill(t5);
  ASSERT_TRUE(done);

  auto [slot4, tc4] = t4.top_slots();
  auto [slot5, tc5] = t5.top_slots();

  ASSERT_EQ(slot4, 4);
  ASSERT_EQ(slot5, 0);
  ASSERT_EQ(tc4, 0);
  ASSERT_EQ(tc5, '1');
};


TEST_F(TubeTest, fill_ok_Test2) {
  auto done = t2.fill(t4);
  ASSERT_TRUE(done);

  auto [slot2, tc2] = t2.top_slots();
  auto [slot4, tc4] = t4.top_slots();

  ASSERT_EQ(slot2, 2);
  ASSERT_EQ(slot4, 0);
  ASSERT_EQ(tc2, '1');
  ASSERT_EQ(tc4, '1');

  done = t6.fill(t8);
  ASSERT_TRUE(done);
  auto [slot8, tc8] = t8.top_slots();
  ASSERT_EQ(slot8, 1);
  ASSERT_EQ(tc8, '4');

  ASSERT_EQ(t6, t7);
};

TEST_F(TubeTest, fill_ok_Test3) {
  auto done = t8.fill(t3);
  ASSERT_TRUE(done);

  auto [slot8, tc8] = t8.top_slots();
  ASSERT_EQ(slot8, 3);
  ASSERT_EQ(tc8, '2');

  auto [slot3, tc3] = t3.top_slots();
  ASSERT_EQ(slot3, 3);
  ASSERT_EQ(tc3, '4');
  
};


