#include <iostream>
#include <queue>
#include <sstream>
#include <unordered_map>
#include <vector>
using namespace std;

bool cmp(pair<int, int>& m, pair<int, int>& n) { return m.second > n.second; }

vector<int> topKFrequent(vector<int>& nums, int k) {
  unordered_map<int, int> occurrences;
  for (auto& v : nums) {
    occurrences[v]++;
  }

  // pair 的第一个元素代表数组的值，第二个元素代表了该值出现的次数
  priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(&cmp)> q(cmp);
  for (auto& [num, count] : occurrences) {
    if (q.size() == k) {
      if (q.top().second < count) {
        q.pop();
        q.emplace(num, count);
      }
    } else {
      q.emplace(num, count);
    }
  }
  vector<int> ret;
  while (!q.empty()) {
    ret.emplace_back(q.top().first);
    q.pop();
  }
  return ret;
}

int main() {
  string input;
  getline(cin, input);
  bool val = false;
  vector<int> nums;
  string number;
  string m;
  for (auto item : input) {
    if (item == '[') {
      val = true;
      continue;
    }
    if (item == ']') {
      val = false;
      continue;
    }
    if (val) {
      if (item == ',') {
        // cout<<number<<endl;
        nums.emplace_back(atoi(number.c_str()));
        number = "";
      } else
        number = number + item;
    }
    if (!val) {
      m = m + item;
    }
  }
  nums.emplace_back(atoi(number.c_str()));
  int n = atoi(m.c_str());

  vector<int> ret(topKFrequent(nums, n));
  for (auto& v : ret) {
    cout << v << " ";
  }
}