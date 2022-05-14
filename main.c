#include <stdio.h>
#include <math.h>
#include <string.h>

#define MAX 250

void solve();

int main() {
   
        solve();
}

int checkValidNumber (char* text)
{

       for(int i = 0 ; i <strlen(text) ; i++)
       {
           if (text[i] < 48 || text[i] > 57)
           {
               printf("Please Enter your input again:\n");
               scanf("%s", text);
               return checkValidNumber(text);
           }
       }
       return 0;
   
}



void solve()
{
    char first_number[MAX];
    char second_number[MAX];
    printf("Please Enter your first number:\n");
    scanf("%s", first_number);
    
    checkValidNumber(first_number);
    
    
    printf("Please Enter your second number:\n");
    scanf("%s", second_number);
    checkValidNumber(second_number);
    
    int output_size = strlen(first_number) +strlen(second_number);

    int shifting, numO, numT;

    int output[output_size];
    for (int i = 0; i < output_size; i++)
    {
        output[i]=0;
    }
      
    for(int i = 0 ; i <strlen(first_number) ; i++)
     {
        for (int j= 0 ; j < strlen(second_number); j++)
         {
            shifting = (strlen(first_number) -1 - i) +(strlen(second_number) -1 - j);
            numO = ((first_number[i]-48) * (second_number[j]-48)) % 10;
            numT = floor((first_number[i]-48) * (second_number[j]-48)/10);

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

    if (output[output_size-1] != 0) printf("%i", output[output_size - 1]);
    
    printf("The answer is:\n");
    for (int i = output_size - 2; i > -1  ; i--)
    {
        printf("%i", output[i]);
        
    }
    printf("\n");

}