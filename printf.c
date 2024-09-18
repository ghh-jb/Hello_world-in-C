#include<stdio.h>
int main(){
  int num = 0;
  printf("enter number:");
  scanf("%i", &num);
  for(int i = 0; i <= num; i++){
    printf("Segment %i\n", i);
  }
  return 0;
}
