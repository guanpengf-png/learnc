#include <stdio.h>

int main(void)
{
    int row=3;
    int col=2;
    int scores[row][col];
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            scanf("%d",&(*(*scores+i*col+j)));
            printf("scores[%d][%d]:",&scores[i][j]);
            //printf("scores[%d][%d]: %d\n",i,j,*(*scores+i*col+j));
         }

    }
    double class_scores;
    double all_class_scores=0;
    for(int i=0;i<row;i++){
        class_scores=0;
        for(int j=0;j<col;j++){
            class_scores+=scores[i][j];

        }
        all_class_scores+=class_scores;
        printf("averge of class: %lf:",class_scores/col);

    }
    printf("averge of all class: %lf:",all_class_scores/row);
    return 0;
}
