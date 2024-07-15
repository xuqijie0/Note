#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

namespace foo {
enum Video { H264 = 1, H265 };
string ToString(const Video &value) {
  switch (value) {
    case H264:
      return "H264";
    case H265:
      return "H265";
  }
}
}  // namespace foo

string ToString(int value) {
  switch (value) {
    case 0:
      return "0";
    case 1:
      return "1";
    case 2:
      return "2";
  }
}

int main(int argc, char **argv) {
  cout << ::ToString(foo::Video::H264) << endl;

  return 0;
}