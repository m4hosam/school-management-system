#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define datafile"data.bin"



struct Student{
    int studentNo;
    int classCode;
    int grade;
};

int afterNewlineNumber(int *ptr)
{
    int endIndex,studentNum;
    char check;
    FILE *file;
    if((file =fopen("index.txt","r"))==NULL)
    {
        printf("Error Opening in afterNewLineNumber()\n");
        return(1);
    }
    fseek(file, 0, SEEK_END);
    endIndex = ftell(file);

    if(*ptr < 0 || *ptr > endIndex || endIndex == 0)
    {
        return -1;
    }
    else
    {

        fseek(file, *ptr, SEEK_SET);
        if(*ptr > (endIndex-4) || *ptr == endIndex)
        {

            fseek(file, -3, SEEK_CUR);
        }
        else if(*ptr == 0)
        {

            fscanf(file, "%d", &studentNum);
            return studentNum;
        }
        check = fgetc(file);
        while(check!='\n')
        {
            fseek(file, -2, SEEK_CUR);
            *ptr = ftell(file);
            if(*ptr == 0)
            {
                break;
            }
            check = fgetc(file);
        }
        *ptr = ftell(file);
        fscanf(file, "%d", &studentNum);
        return studentNum;
    }

}

int createIndexFile(void)  //delete the previous one
{
    FILE *file;
    if((file =fopen(datafile,"r"))==NULL)
    {
        printf("Error Opening\n");
        return(1);
    }
    fseek(file, 0, SEEK_END);
    int endOfTheFile = ftell(file);
    int idx;
    if(endOfTheFile == 0)
    {
        printf("No records to copy from.");
    }

    int index;
    int studentNum;
    rewind(file);
    for(int i = 0 ;  ; i++)
    {
        struct Student s;
        index = ftell(file);
        size_t num = fread(&s, sizeof(struct Student), 1, file);
        studentNum = s.studentNo;
        //inserting in the index file
        int ptr = indexSort(studentNum, index);
        insetByPtr(ptr, studentNum, index);
        //breaking point
        idx = ftell(file);
        if(idx == endOfTheFile)
        {
            break;
        }
    }

    fclose(file);
}

int insetByPtr(int ptr, int number, int index) //Done
{
    FILE *file1 = fopen("index.txt","a+");
    if (file1 == NULL){
        printf("File not found");
        return 1;
    }
    FILE *file2 = fopen("index2.txt","w");
    if (file2 == NULL){
        printf("File not found");
        return 1;
    }
    char ch;
    //add to the first of the file
    // switch ()
    if(ptr == 0)
    {
        fprintf(file2, "%d %d\n",number,index);
        while((ch = fgetc(file1)) != EOF)
        {
            fputc(ch, file2);
        }

    }
    else if (ptr==1)
    {
        char buf[100];
        fgets(buf,100,file1);
        fprintf(file2, buf);
        fprintf(file2, "%d %d\n",number,index);
        while((ch = fgetc(file1)) != EOF)
        {
            fputc(ch, file2);
        }
    }
    else if(ptr == -1)
    {
        while((ch = fgetc(file1)) != EOF)
        {
            fputc(ch, file2);
        }
        fprintf(file2, "%d %d\n",number,index);
    }
    else if(ptr == -2)
    {
        fprintf(file2, "%d %d\n",number,index);
    }
    else
    {
        int indexf = ftell(file1);
        char ch;
        while(indexf <= ptr)
        {
            ch = fgetc(file1);
            fputc(ch, file2);
            indexf = ftell(file1);
        }

        while(ch!='\n')
        {
            ch = fgetc(file1);
            fputc(ch, file2);
        }
        //another line
        fprintf(file2, "%d %d\n",number,index);
        while((ch = fgetc(file1)) != EOF)
        {
            fputc(ch, file2);
        }

    }


    fclose(file1);
    fclose(file2);

    remove("index.txt");
    rename("index2.txt", "index.txt");
    return 0;

}

int indexSort(int num, int indx) //Redesign
{
    FILE *file = fopen("index.txt","a+");
    if (file == NULL){
        printf("File not found");
        return -10;
    }

    char check;
    int middle,orta,lastNumber,firstNumber,secondNumber;



    fseek(file,0,SEEK_END);
    int endIndex = ftell(file);
    if(endIndex == 0)  //checking if the file is empty
    {
        fclose(file);
        return -2;
    }




    int startIndex = 0;
    check = fgetc(file);
    while(check!='\n')
    {
        fseek(file, -2, SEEK_CUR);
        check = fgetc(file);

    }
    fscanf(file, "%d", &lastNumber); //getting the last number

    rewind(file);
    char buf[100];
    int count_lines=0;
    while((fgets(buf,100,file)) != NULL)
    {
        count_lines++;
    }

    //checking the first item of the file
    rewind(file);
    fscanf(file, "%d", &firstNumber); //getting the first number

    if(count_lines == 1 && num >= firstNumber)
    {
        fclose(file);
        return -1;
    }
    else if(count_lines == 1 && num < firstNumber)
    {
        fclose(file);
        return 0;
    }



    check = fgetc(file);
    while(check!='\n')
    {
        fseek(file, 1, SEEK_CUR);
        check = fgetc(file);
    }
    fscanf(file, "%d", &secondNumber); //getting the second number




    if(num <= firstNumber)
    {
        fclose(file);
        return 0;
    }
    else if(num >= lastNumber)
    {
        fclose(file);
        return -1;
    }
    else if(num > firstNumber && num < secondNumber)
    {
        fclose(file);
        return 1;
    }
    else
    {
        //checking the numbers by binary search and detect the place to insert

        while(startIndex!=endIndex || endIndex>startIndex)
        {
            orta= (endIndex-startIndex)/2;
            middle = startIndex + orta;
            fseek(file, middle, SEEK_SET);
            check = fgetc(file);

            while(check!='\n')
            {
                fseek(file, -2, SEEK_CUR);
                check = fgetc(file);
            }

            int idx = ftell(file);

            int k;
            fscanf(file, "%d", &k);

            if(num > k )
            {
                if(startIndex != idx)
                {
                    startIndex = idx;
                }
                else
                {
                    fclose(file);
                    return startIndex;
                    break;
                }

            }
            else if(num < k)
            {
                if(endIndex != idx)
                {
                    endIndex = idx;
                }
                else
                {
                    fclose(file);
                    return startIndex;
                    break;
                }

            }
            else
            {
                startIndex = idx;
                fclose(file);
                return startIndex;
                break;
            }


        }


    }

}

int indexSearch(int num, int *records, int *indexes)  //Redesign
{
    FILE *file = fopen("index.txt","r");
    if (file == NULL)
    {
        printf("File not found");
        return -1;
    }
    char check;
    int middle,orta;

    fseek(file,0,SEEK_END);
    int endIndex = ftell(file);
    int endOfTheFile = ftell(file);
    int startIndex = 0;
    check = fgetc(file);


    while(startIndex < endIndex)
    {
        orta= (endIndex-startIndex)/2;
        middle = startIndex + orta;
        fseek(file, middle, SEEK_SET);
        check = fgetc(file);
        int fileStart, test2;

        while(check!='\n')
        {
            fseek(file, -2, SEEK_CUR);
            fileStart = ftell(file);
            if(fileStart == 0)
            {
                break;
            }
            check = fgetc(file);
        }

        int idx = ftell(file);

        int test,flag;
        flag = 0;
        fscanf(file, "%d", &test);
        printf("Numbers111 \n");
        if(num == test)
        {
            //getting all the records with the same number
            do
            {
                // Getting the first record
                printf("Numbers222\n");
                idx = idx - 3;
                fseek(file, idx,SEEK_SET);
                check = fgetc(file);
                while(check!='\n')
                {
                    fseek(file, -2, SEEK_CUR);
                    fileStart = ftell(file);
                    if(fileStart == 0)
                    {
                        break;
                    }
                    check = fgetc(file);
                }

                idx = ftell(file);
                if(fileStart == 0)
                {
                    flag=1;
                    break;
                }
                fscanf(file, "%d", &test2);

            }
            while(num == test2);


            //Adding the records in the arrays
            int num1,num2,recordsNum = 0;

            if(flag == 0)
            {
                char buf[100];
                fgets(buf,100,file);
                printf("Numbers333 \n");
                for(int i = 0 ;  ; i++)
                {
                    fscanf(file, "%d %d",&num1,&num2);
                    if(num == num1)
                    {
                        //add to array

                        records[i] = num1;
                        indexes[i] = num2;
                        recordsNum++;
                        //printf("Numbers are :%d %d\n", num1,num2);
                        if((ftell(file))+2 == endOfTheFile)
                        {
                            // if i am getting the last number in index file
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }


            }
            else
            {
                for(int i = 0 ;  ; i++)
                {
                    fscanf(file, "%d %d",&num1,&num2);
                    printf("Numbers444 \n");
                    if(num == num1)
                    {
                        //add to array
                        records[i] = num1;
                        indexes[i] = num2;
                        recordsNum++;
                        //printf("Numbers are :%d %d\n", num1,num2);
                    }
                    else
                    {
                        break;
                    }
                }
            }


            fclose(file);
            return recordsNum;
        }
        else if(num < test)
        {
            if(endIndex != idx)
            {
                endIndex = idx;
            }
            else
            {
                fclose(file);
                printf("Not found\n");
                return -1;
                break;
            }
        }
        else
        {
            if(startIndex != idx)
            {
                startIndex = idx;
            }
            else
            {
                fclose(file);
                printf("Not found\n");
                return -1;
                break;
            }
        }

    }

    fclose(file);
    printf("Not found\n");
    return -1;


}

void showIndex(void)  //Done
{
    FILE *file = fopen("index.txt","r");
    if (file == NULL){
        printf("File not found");
        return 1;
    }



    char buf[100];
    int count_lines=0;
    while((fgets(buf,100,file)) != NULL)
    {
        count_lines++;
        printf("%d--> %s",count_lines,buf);
    }
    printf("\n\n-------------------------\n\n");
    fclose(file);
}

int inputStore(void) //Done
{
    struct Student newStudent;
    printf("Input Student Number(orenci No): ");
    scanf("%d", &newStudent.studentNo);
    printf("Input Class Code(ders kodu): ");
    scanf("%d", &newStudent.classCode);
    printf("Input Student Grade(Puan): ");
    scanf("%d", &newStudent.grade);

    int num = newStudent.studentNo;

    FILE *file;
    if((file =fopen(datafile,"a"))==NULL)
    {
        printf("Error Opening\n");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    int end = ftell(file);
    fseek(file, 0, SEEK_SET);

    fwrite(&newStudent, sizeof(struct Student), 1, file);


    fclose(file);

    int ptr = indexSort(num, end);
    insetByPtr(ptr, num, end);

    printf("\n-------Saved-------\n");
    return 0;

}


int listAllRecords(void) //Done
{
    FILE *file;
    if((file =fopen(datafile,"r"))==NULL)
    {
        printf("Error Opening\n");
        return(1);
    }
    fseek(file, 0, SEEK_END);
    int endOfTheFile = ftell(file);
    int idx;
    if(endOfTheFile == 0)
    {
        printf("No records");
    }

    rewind(file);
    for(int i = 0 ;  ; i++)
    {
        struct Student s;
        size_t num = fread(&s, sizeof(struct Student), 1, file);

        printf("Student Number: %d \nClass Code: %d\nStudent Grade: %d ", s.studentNo,s.classCode, s.grade );
        printf("\n--------------\n");
        idx = ftell(file);
        if(idx == endOfTheFile)
        {
            break;
        }
    }

    fclose(file);

}

int listByIndex(int *indexes, int size) //Done
{
    FILE *file;
    if((file =fopen(datafile,"r"))==NULL)
    {
        printf("Error Opening\n");
        return(1);
    }

    for(int i = 0 ; i < size ; i++)
    {
        fseek(file, indexes[i], SEEK_SET);
        struct Student s;
        size_t num = fread(&s, sizeof(struct Student), 1, file);
        printf("[%d]\n", i+1);
        printf("Student Number: %d \nClass Code: %d\nStudent Grade: %d ", s.studentNo,s.classCode, s.grade );
        printf("\n--------------\n");
    }

    fclose(file);


}

int kayitBul(void)  //Done
{
    int studentNumber;
    int records[10], indexes[10];
    printf("Enter Student Number(ogrenci no): ");
    scanf("%d", &studentNumber);

    int size = indexSearch(studentNumber, records, indexes);

    listByIndex(indexes, size);
}

int deleteByIndex(int index)
{
    FILE *file;
    if((file =fopen("data.bin","r"))==NULL)
    {
        printf("Error Opening\n");
        return(1);
    }
    FILE *file2;
    if((file2 =fopen("tmp.bin","w"))==NULL)
    {
        printf("Error Opening\n");
        return(1);
    }


    struct Student s;
    printf("\nindex: %d\n", index);

    int ptr = ftell(file);
    printf("ptr %d, index:%d\n", ptr, index);
    if(ptr == index)
    {

        fread(&s, sizeof(struct Student), 1, file);
        while ((fread(&s, sizeof(struct Student), 1, file)) != NULL)
        {
            fwrite(&s, sizeof(struct Student), 1, file2);
        }
        printf("deleted");

    }
    else
    {
        fseek(file, 0,SEEK_END);
        int endOfTheFile = ftell(file);
        rewind(file);
        fread(&s, sizeof(struct Student), 1, file);
        fwrite(&s, sizeof(struct Student), 1, file2);
        while(1)
        {
            ptr = ftell(file);
            if(ptr == endOfTheFile)
            {
                break;
            }
            else if (ptr == index)
            {
                printf("A record with requested name found and deleted.\n\n");
                fread(&s, sizeof(struct Student), 1, file);
            }
            else
            {
                fread(&s, sizeof(struct Student), 1, file);
                fwrite(&s, sizeof(struct Student), 1, file2);
            }
        }

    }

    fclose(file);
    fclose(file2);

    remove("data.bin");
    rename("tmp.bin", "data.bin");

}

int kayitSil(void)
{
    int choice;
    int studentNumber;
    int records[10], indexes[10];
    printf("Enter Student Number(ogrenci no): ");
    scanf("%d", &studentNumber);

    int size = indexSearch(studentNumber, records, indexes);
    if(size!= -1)
    {
        listByIndex(indexes, size);

        printf("Which one to delete?[1:%d] ", size);
        scanf("%d", &choice);

        for(int i = 0 ; i < size ; i++)
        {
            if(choice == (i+1))
            {
                deleteByIndex(indexes[i]);
                remove("index.txt");
                createIndexFile();
            }
        }
        if(choice <1 || choice > size)
        {
            printf("invalid input....\n");
        }
    }
}

int updateByIndex(int index, int newGrade)
{
    FILE *file;
    if((file =fopen(datafile,"r"))==NULL)
    {
        printf("Error Opening\n");
        return(1);
    }

    FILE *file2;
    if((file2 =fopen("tmp.bin","w"))==NULL)
    {
        printf("Error Opening\n");
        return(1);
    }

    struct Student s;
    printf("\nindex: %d\n", index);

    int ptr = ftell(file);
    printf("ptr %d, index:%d\n", ptr, index);
    if(ptr == index)
    {

        fread(&s, sizeof(struct Student), 1, file);
        s.grade = newGrade; //-----
        fwrite(&s, sizeof(struct Student), 1, file2); //----
        while ((fread(&s, sizeof(struct Student), 1, file)) != NULL)
        {
            fwrite(&s, sizeof(struct Student), 1, file2);
        }
        printf("deleted");

    }
    else
    {
        fseek(file, 0,SEEK_END);
        int endOfTheFile = ftell(file);
        rewind(file);
        fread(&s, sizeof(struct Student), 1, file);
        fwrite(&s, sizeof(struct Student), 1, file2);
        while(1)
        {
            ptr = ftell(file);
            if(ptr == endOfTheFile)
            {
                break;
            }
            else if (ptr == index)
            {
                fread(&s, sizeof(struct Student), 1, file);
                s.grade = newGrade; //-----
                fwrite(&s, sizeof(struct Student), 1, file2); //----
            }
            else
            {
                fread(&s, sizeof(struct Student), 1, file);
                fwrite(&s, sizeof(struct Student), 1, file2);
            }
        }

    }

    fclose(file);
    fclose(file2);

    remove("data.bin");
    rename("tmp.bin", "data.bin");

}

int kayitGuncel(void)
{
    int choice, newGrade,studentNumber;
    int records[10], indexes[10];
    printf("Enter Student Number(ogrenci no): ");
    scanf("%d", &studentNumber);

    int size = indexSearch(studentNumber, records, indexes);
    if(size!= -1)
    {
        listByIndex(indexes, size);
        printf("Which one to update?[1:%d] ", size);
        scanf("%d", &choice);
        printf("New Grade: ");
        scanf("%d", &newGrade);
        for(int i = 0 ; i < size ; i++)
        {
            if(choice == (i+1))
            {
                updateByIndex(indexes[i], newGrade);
                printf("Updated");
            }
        }
    }
}


int main()
{
    //int a = indexSort(6,24);
    //insetByPtr(a, 6, 24);
    //printf("\n---index: %d-----\n",a);

    //inputStore();

    int a = 3;
    showIndex();
    printf("\n\n-----------------------------------------------------------------------\n\n");
    int k = afterNewlineNumber(&a);
    printf("return:%d\n",k);



  //listAllRecords();

   //kayitGuncel();

   // kayitBul();
   //deleteByIndex(37);
   //kayitSil();
   //updateByIndex(24, 0);
   //createIndexFile();
  //printf("\n\n-----------------------------------------------------------------------\n\n");




   //listAllRecords();

    return 0;
}
