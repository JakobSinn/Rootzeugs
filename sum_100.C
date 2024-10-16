#include <cstdio>
#include <iostream>
using namespace std;
#include <stdio.h>
#include <stdlib.h>

int summe(int limit);

void sum_100(){
  int limit;
  printf("Please enter the limit you want: ");
  scanf("%d",&limit);
  printf("Limit is %d\n",limit);
  printf("Summation from 1 to %d (inclusive) yields %d \n",limit,summe(limit));
}

int summe(int limit){
  int sum = 0;
  int i;
  for (i = 1; i <= limit; i++){
    sum += i;
  }
  return sum;
}
