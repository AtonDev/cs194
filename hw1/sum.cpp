#include <cstdlib>

int main(int argc, char  *argv[])
{
  //get the number of the user
  int n = atoi(argv[1]);

  //compute sum of first n intengers
  long long sum = 0;
  for (long long i = 1; i <= n; ++i) {
    sum += i;
  }
}
