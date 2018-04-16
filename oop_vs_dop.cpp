#include <vector>

using std::vector;

namespace ba_foo {

  //-----------------------------------------------------------------------------
  class Behavior {
  public:
    Behavior();
    ~Behavior();

    void mark_behavior();
    double behavior();

  private:
    // lots of data items here, for now we'll keep it simple
    double data;
  };

  Behavior::Behavior() {}

  Behavior::~Behavior() {}

  void Behavior::mark_behavior() {
    data = 1.;  // in reality a big complex, algorithm ofc
  }

  double Behavior::behavior() { return data; }

  //-----------------------------------------------------------------------------
  class State_data {
  public:
    State_data();
    ~State_data();

    void save_state(const double state_dat);
    double state();

  private:
    // lots of data items here, for now we'll keep it simple
    double data;
  };

  State_data::State_data() {}

  State_data::~State_data() {}

  void State_data::save_state(const double state_dat) { data = state_dat; }

  double State_data::state() { return data; }

  // Object-oriented
  //-----------------------------------------------------------------------------
  struct OOP {
  public:
    OOP();
    ~OOP();

    void oop_update();

  private:
    Behavior behavior;
    State_data state;
  };

  OOP::OOP() {
    // initialize private data
  }

  OOP::~OOP() {}

  void OOP::oop_update() {
    // manage behavior and state for each individual struct (record) separately

    ;  // do behavior analytics here
    ;  // track analytics results here
  }

  // Data-oriented
  //-----------------------------------------------------------------------------
  struct DOP {
  public:
    DOP();
    DOP(const int cnt);
    ~DOP();

    void dop_update();

  private:
    vector<Behavior> behaviors;
    vector<State_data> states;
  };

  DOP::DOP() {}

  DOP::DOP(const int cnt) : behaviors(cnt), states(cnt) {
    // initialize all system data
  }

  DOP::~DOP() {}

  void DOP::dop_update() {
    // manage behavior and state for all structs (records) sequentially, broken up
    // into distinct phases

    for (auto& e : behaviors)
      ;  // do behavior analytics here
    for (auto& e : states)
      ;  // track analytics results here

    // OPTIMIZATION: For large datasets, manage active/inactive items in separate
    // DOP private structures with runtime move semantics. Use the same approach
    // for hot/cold splitting and sparse datasets. This is also very likely the
    // right time to keep data sorted and/or indexed.
    // -Remember the goal is to keep as many actionable data items in the cache
    // line as possible at all times.
  }

}  // namespace ba_foo

// NOTES:
// -By sequentially processing long contiguous, compacted memory layout
// structures of data (the optimized 'struct of arrays' approach) during runtime
// -- rather than stepping through different records each with their own private
// state (the traditional 'array of structs' approach) -- we optimize
// runtime performance by removing a layer of memory-access indirection with
// each and every record access of the latter approach. This allows the
// prefetcher to do a better job of keeping the cache line full during runtime
// processing.
//
// -The DOP struct utilizes this technique, becoming in effect a
// memory-disjointed container for all the data items in the system, instead of
// keeping data items local to each record.
//
// -In this trivial example of just a couple of doubles contained in each record
// the benefit is minimal. In a large real-world system with complex (typically
// sparse) data memory layout requirements for each record, the perf benefits
// will become obvious and usage semantics can usually also be simplified in the
// caller by hiding complexity behind the DOP interfaces, as in this example.
//
// -I believe this approach also makes maintenance easier in general because it
// helps keep the state invariants management localized to a single (or small
// number of) struct(s) instead of it potentially being spread across many.
//
int main() {
  //bool processing{true};
  //const int count{10'000'000};    // more realistic case
  const int count{1'000};           // example case

  vector<ba_foo::OOP> oops(count);  // array of structs
  ba_foo::DOP dop{count};           // struct of arrays

  //while (processing) {
    // manage network, disk, etc. activity

    for (auto& e : oops) e.oop_update();
    dop.dop_update();
  //}
}
