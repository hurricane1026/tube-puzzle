/*
 * Copyright (C) 2021 hurricane <l@stdb.io>. All rights reserved.
 */

#include "concurrentqueue/concurrentqueue.h"

#include "game.hpp"


const unsigned int QueueLength = 16<<10;
const unsigned int Threads = 40;

int main() {
  moodycamel::ConcurrentQueue<game<4>*> q(QueueLength);
  game<4>* init_g = new game<4>("game.yaml");
  q.enqueue(init_g);

  //game<4>* winner = nullptr;

  auto worker = [&]() {
    game<4>* winner = nullptr;

    while (true) {
      game<4>* temp;
      if (q.try_dequeue(temp)) {
        if (temp->success()) {
          winner = temp;
          break;
        } else {
          // gen the next games.
          auto nexts = temp->next();
          for (auto g : nexts) {
            q.enqueue(g);
          }
          delete temp;
        }
      }
    }
    winner->show_codes();
    abort();
   };

  std::vector<std::thread> threads;
  for (unsigned int i = 0; i < Threads; ++i) {
      //std::thread t(worker);
      threads.emplace_back(worker);
      fmt::print("start a thread\n");
  }

  for (auto& t : threads) {
      t.join();
      fmt::print("join a thread\n");
  }
  return 0;
}
