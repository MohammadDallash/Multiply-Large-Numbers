#include <stdio.h>
#include <math.h>
#include <string.h>

void my();

int main() {
   
        my();

}


void my()
{
    char inputOne[250];
    char inputTwo[250];
    
    scanf("%s", inputOne);
    scanf("%s", inputTwo);

    int totalSize = strlen(inputOne) +strlen(inputTwo);

    int shifting, numO, numT;

    int output[totalSize];
    for (int i = 0; i < totalSize; i++)
    {
        output[i]=0;
    }
      
    for(int i = 0 ; i <strlen(inputOne) ; i++)
     {
        for (int j= 0 ; j < strlen(inputTwo); j++)
         {
            shifting = (strlen(inputOne) -1 - i) +(strlen(inputTwo) -1 - j);
            numO = ((inputOne[i]-48) * (inputTwo[j]-48)) % 10;
            numT = floor((inputOne[i]-48) * (inputTwo[j]-48)/10);

            if (output[0+shifting] + numO > 9)
            {
                output[1+shifting] += floor((numO + output[0+shifting]) / 10);
                output[0+shifting] = (output[0+shifting] + numO)%10;
            }
            else
            {
                output[0+shifting] += numO;
            }
            
            if (output[1+shifting] + numT > 9)
            {
                output[2+shifting] += floor((numT + output[1+shifting]) / 10);
                output[1+shifting] = (output[1+shifting] + numT)%10;
            }
            else
            {
                output[1+shifting] += numT;
            }
         }
     }

    if (output[totalSize-1] != 0) printf("%i", output[totalSize - 1]);
    
    for (int i = totalSize - 2; i > -1  ; i--)
    {
        printf("%i", output[i]);
        
    }
    printf("\n");

}