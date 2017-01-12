#include "search.h"
using namespace std;

int main() {
  int bd[30] = {1, 2, 3, 4 ,5, 6, 1, 2, 3, 4 ,5, 6, 6, 1, 2, 3, 4 ,5, 1, 2, 3, 4 ,5, 6, 1, 2, 3, 4 ,5, 6};
  // Search* search = new Search(bd, 0, 0);
  Search* search = new Search("/home/mech-user/work/nekonote/pd/cpp/sample.dat", 0, 0);
  clock_t start = clock();    // スタート時間
  Board* best = search->beam_search(6);
  clock_t end = clock();     // 終了時間
  cout << "duration = " << (double)(end - start) / CLOCKS_PER_SEC << "sec.\n";
  // best->print_board();
  // best->print_simulate_board();
  // best->print_path();
  cout << best->score << endl;
  cout << best->combo_num << endl;
  // best->disp(false, 5000);
  best->disp_path(search->load_from_file("/home/mech-user/work/nekonote/pd/cpp/sample.dat"));
  int cnt = 1;
  while(cnt) {
    best->_mark_combo();
    best->disp(true, 500);
    cnt = best->_delete_drop();
    best->disp(true, 500);
    best->_fill_drop();
    best->disp(true, 500);
  }
}
