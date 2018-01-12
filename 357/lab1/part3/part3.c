#include<stdio.h>
#include"part3.h"
   int sum(int *arrayToSum, int arraySize){
      int i=0;
      int sumOfArray=0;
      for(i=0;i<arraySize;i++){
         sumOfArray=sumOfArray+*arrayToSum;
         arrayToSum++;
      }
      return sumOfArray;
}
