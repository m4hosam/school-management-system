#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define datafile "data.bin"



struct Student{
    int studentNo;
    int classCode;
    int grade;
};

void deleteIndexFile(void)
{
    printf("\t\t-------Index File Silmek--------\n\n");
    int a = remove("index.txt");
    if(a==0)
        printf("Deleted");
    else
        printf("File Wasn't There");
}

int afterNewlineNumber(int *ptr)
{
    int endIndex,studentNum;
    char check;
    FILE *file;
    if((file =fopen("index.txt","r"))==NULL)
    {
        printf("Error Opening in afterNewLineNumber()\n");
        return(-10);
    }
    fseek(file, 0, SEEK_END);
    endIndex = ftell(file);

    if(*ptr < 0 || *ptr > endIndex || endIndex == 0)
    {
        fclose(file);
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
            fclose(file);
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
        fclose(file);
        return studentNum;
    }

}



int insetByPtr(int ptr, int number, int index)
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
    char buf[100];
    //add to the first of the file
    // switch ()
    if(ptr == 0)// add the Number then Copy
    {
        fprintf(file2, "%d %d\n",number,index);
        while((ch = fgetc(file1)) != EOF)
        {
            fputc(ch, file2);
        }

    }
    else if (ptr==1) // copy one line, add the Number then Copy to the end.
    {
        fgets(buf,100,file1);
        fprintf(file2, buf);
        fprintf(file2, "%d %d\n",number,index);
        while((ch = fgetc(file1)) != EOF)
        {
            fputc(ch, file2);
        }
    }
    else if(ptr == -1) // append to the end to the file
    {
        while((ch = fgetc(file1)) != EOF)
        {
            fputc(ch, file2);
        }
        fprintf(file2, "%d %d\n",number,index);
    }
    else if(ptr == -2) // Empty File add The Number
    {
        fprintf(file2, "%d %d\n",number,index);
    }
    else  //Reach the pointer of the insertion area, add the Number then copy to the end
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

int searchIndexes(int num, int *searchFlag,int *searchedptr) //searchFlag & searchedptr for searching only
{
    char check;
    int middle, lastNumber, firstNumber, secondNumber, endIndex, startIndex;
    FILE *file = fopen("index.txt", "a+");
    if (file == NULL)
    {
        printf("Error Opening The File");
    }

    startIndex = 0;
    fseek(file, 0, SEEK_END);
    endIndex = ftell(file);

    if (endIndex == 0) //-empty file-----1-append
    {
        fclose(file);
        return -2;
    }

    lastNumber = afterNewlineNumber(&endIndex);    //getting the last number
    firstNumber = afterNewlineNumber(&startIndex); //getting the first number

    rewind(file);
    check = fgetc(file);
    while (check != '\n')
    {
        fseek(file, 1, SEEK_CUR);
        check = fgetc(file);
    }
    fscanf(file, "%d", &secondNumber); //getting the second number

    rewind(file);
    char buf[100];
    int count_lines = 0;
    while ((fgets(buf, 100, file)) != NULL)
    {
        count_lines++; //if there is only one line in the file
    }
    //-                 -First Stage One line file-
    if (count_lines == 1 && num >= firstNumber) //-----2-append
    {
        if (num == firstNumber)
        {
            *searchFlag = *searchFlag + 1;
            *searchedptr = 0;
        }
        fclose(file);
        return -1;
    }
    else if (count_lines == 1 && num < firstNumber) //-----3-add then copy all the file
    {
        fclose(file);
        return 0;
    }
    //-             -Second Stage Multiple lines-
    if (num <= firstNumber) //-----4-
    {
        if (num == firstNumber)
        {
            *searchFlag = *searchFlag + 1;
            *searchedptr = 0;
        }
        fclose(file);
        return 0;
    }
    else if (num >= lastNumber) //---5
    {
        if (num == lastNumber)
        {
            *searchFlag = *searchFlag + 1;
            *searchedptr = endIndex;
        }
        fclose(file);
        return -1;
    }
    else if (num > firstNumber && num < secondNumber) //-----6
    {
        fclose(file);
        return 1;
    }
    else
    {
        //checking the numbers by binary search and detect the place to insert

        while (1)
        {
            middle = startIndex + ((endIndex - startIndex) / 2);

            int test = afterNewlineNumber(&middle);
            int idx = middle;
            if (num == test)
            {
                *searchFlag = *searchFlag + 1;
                startIndex = idx;
                *searchedptr = startIndex;
                fclose(file);
                return startIndex;
                break;
            }
            else if (num > test)
            {
                if (startIndex != idx)
                    startIndex = idx;
                else
                {
                    fclose(file);
                    return startIndex;
                    break;
                }
            }
            else
            {
                if (endIndex != idx)
                    endIndex = idx;
                else
                {
                    fclose(file);
                    return startIndex;
                    break;
                }
            }
        }

    }
}

int createIndexFile(void)
{
    FILE *file;
    if((file =fopen(datafile,"r"))==NULL)
    {
        printf("Error Opening\n");
        return(1);
    }
    remove("index.txt");
    fseek(file, 0, SEEK_END);
    int endOfTheFile = ftell(file);
    int idx;
    if(endOfTheFile == 0)
    {
        printf("\nNo records in data file\n");
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
        int searchedtmp = 0,tmp;
        int ptr = searchIndexes(studentNum, &searchedtmp, &tmp);
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

int listSearchedIndex(int num, int *records, int *indexes)
{
    int searchFlag = 0;
    int ptr,test;
    int flag = 0;
    int num1, num2, recordsNum = 0;
    FILE *file = fopen("index.txt", "r");
    if (file == NULL)
    {
        printf("File not found");
        return -1;
    }
    fseek(file, 0, SEEK_END);
    int endOfTheFile = ftell(file);

    searchIndexes(num, &searchFlag, &ptr);
    if (searchFlag == 1) //Number  was found
    {
        if (ptr == 0)
            flag = 1;//first Number in the File
        else
        {
            while (ptr != 0)
            {
                ptr = ptr - 3;
                test = afterNewlineNumber(&ptr);
                if (num != test)
                {
                    break;
                }
            }
            if (ptr == 0 && num == test)//first Number in the File
                flag = 1;

        }
        // -------------------------------------Getting the first record-------------------

        //Adding the records in the arrays
        fseek(file, ptr, SEEK_SET);
        if (flag == 0)//skip a line to the first Record
        {
            char buf[100];
            fgets(buf, 100, file);
        }
        for (int i = 0;; i++)
        {

            fscanf(file, "%d %d", &num1, &num2);
            if (num == num1)
            {
                //add to array
                records[i] = num1;
                indexes[i] = num2;
                recordsNum = recordsNum + 1;
                if ((ftell(file)) + 2 == endOfTheFile)
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

        fclose(file);
        return recordsNum;
    }
    else
    {
        printf("Student Number Not Found.\n");
        fclose(file);
        return -1;
    }
}


void showIndexfile(void)
{
    printf("\t\t-------Index Dosyasi Gostermek--------\n\n");
    FILE *file = fopen("index.txt","r");
    if (file == NULL){
        printf("File not found");
    }
    fseek(file, 0, SEEK_END);
    int endFile = ftell(file);
    rewind(file);
    if(endFile == 0)
        printf("File is Empty.\n");
    else
    {
        char buf[100];
        int count_lines=0;
        while((fgets(buf,100,file)) != NULL)
        {
            count_lines++;
            printf("%d--> %s",count_lines,buf);
        }
        printf("\n\n-------------------------\n\n");

    }

    fclose(file);
}

int kayitEkle(void)
{
    printf("\t\t-------Kayit Eklemek--------\n\n");
    struct Student newStudent;
    printf("Input Student Number(orenci No): ");
    scanf("%d", &newStudent.studentNo);
    printf("Input Class Code(ders kodu): ");
    scanf("%d", &newStudent.classCode);
    printf("Input Student Score(Puan): ");
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
    int tmp;
    int searchtmp = 0;
    int ptr = searchIndexes(num, &searchtmp, &tmp);
    insetByPtr(ptr, num, end);

    printf("\n-------Saved-------\n");
    return 0;

}


int listAllRecords(void)
{
    printf("\t\t-------Tum Kayitlar--------\n\n");
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

        printf("Student Number(orenci No): %d \nClass Code(Ders Kodu): %d\nStudent Score(Puan): %d ", s.studentNo,s.classCode, s.grade );
        printf("\n--------------\n");
        idx = ftell(file);
        if(idx == endOfTheFile)
        {
            break;
        }
    }

    fclose(file);

}

int listByIndex(int *indexes, int size)
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

int kayitBul(void)
{
    printf("\t\t-------Kayit Bulmak--------\n\n");
    int studentNumber;
    int records[20], indexes[20];
    printf("Enter Student Number(ogrenci no): ");
    scanf("%d", &studentNumber);

    int size = listSearchedIndex(studentNumber, records, indexes);

    listByIndex(indexes, size);
}


int updateByIndex(int index, int updateFlag, int newGrade) //update if updateFlag = 1 else delete
{
    FILE *file;
    if ((file = fopen("data.bin", "r")) == NULL)
    {
        printf("Error Opening\n");
        return (1);
    }
    FILE *file2;
    if ((file2 = fopen("tmp.bin", "w")) == NULL)
    {
        printf("Error Opening\n");
        return (1);
    }

    fseek(file, 0, SEEK_END);
    int endOfTheFile = ftell(file);
    rewind(file);
    struct Student s;

    int ptr;
    if (index == 0)
    {

        fread(&s, sizeof(struct Student), 1, file);
        if (updateFlag == 1)
        {
            s.grade = newGrade;
            fwrite(&s, sizeof(struct Student), 1, file2);
        }
        while ((fread(&s, sizeof(struct Student), 1, file)) != NULL)
        {
            fwrite(&s, sizeof(struct Student), 1, file2);
        }
        printf("\ndeleted\n");
    }
    else
    {

        fread(&s, sizeof(struct Student), 1, file);
        fwrite(&s, sizeof(struct Student), 1, file2);
        while (1)
        {
            ptr = ftell(file);
            if (ptr == endOfTheFile)
            {
                break;
            }
            else if (ptr == index)
            {
                fread(&s, sizeof(struct Student), 1, file);
                if (updateFlag == 1)
                {
                    s.grade = newGrade;
                    fwrite(&s, sizeof(struct Student), 1, file2);
                }
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

int kayitSil(void) //Depends on updateByIndex(-, 1, -)
{
    printf("\t\t-------Kayit Silmek--------\n\n");
    int choice;
    int studentNumber;
    int records[10], indexes[10];
    printf("Enter Student Number(ogrenci no): ");
    scanf("%d", &studentNumber);

    int size = listSearchedIndex(studentNumber, records, indexes);
    if(size!= -1)
    {
        listByIndex(indexes, size);

        printf("Which one to delete?[1:%d] ", size);
        scanf("%d", &choice);

        for(int i = 0 ; i < size ; i++)
        {
            if(choice == (i+1))
            {
                updateByIndex(indexes[i], 0, 0);
                createIndexFile();
                printf("Deleted.\n");
            }
        }
        if(choice <1 || choice > size)
        {
            printf("invalid input....\n");
        }
    }
}

int kayitGuncel(void) //Depends on updateByIndex(-,0 ,)
{
    printf("\t\t-------Kayit Guncel--------\n\n");
    int choice, newGrade,studentNumber;
    int records[10], indexes[10];
    printf("Enter Student Number(ogrenci no): ");
    scanf("%d", &studentNumber);

    int size = listSearchedIndex(studentNumber, records, indexes);
    if(size!= -1)
    {
        listByIndex(indexes, size);
        printf("Which one to update?[1:%d] ", size);
        scanf("%d", &choice);
        printf("New Score(Yeni Puan): ");
        scanf("%d", &newGrade);
        for(int i = 0 ; i < size ; i++)
        {
            if(choice == (i+1))
            {
                updateByIndex(indexes[i], 1, newGrade);
                printf("Updated\n");
            }
        }
    }
}


int main()
{
    printf("\t\t-------Ana Menu--------\n\n");
    int choice = 1;
    while (choice >0 && choice < 9)
    {
        printf("1->Kayit Eklemek icin -> 1\n\n");
        printf("2->Kayit Bulmek icin -> 2\n\n");
        printf("3->Kayit Silmek icin -> 3\n\n");
        printf("4->Kayit Guncellemek icin -> 4\n\n");
        printf("5->Tum Kayitlar Listle -> 5\n\n");
        printf("6->Index Dosyasi Gostermek -> 6\n\n");
        printf("7->Index Dosyasi Silmek -> 7\n\n");
        printf("8->Index Dosyasi Olusturmak -> 8\n\n");
        printf("0->Cikis icin -> 0 basininz:");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            system("cls");
            kayitEkle();
            printf("\n\nCikis icin 0'e basin.\nAna menu donmek icin 1'e basin:");
            scanf("%d", &choice);
            system("cls");
            break;
        case 2:
            system("cls");
            kayitBul();
            printf("\n\nCikis icin 0'e basin.\nAna menu donmek icin 1'e basin:");
            scanf("%d", &choice);
            system("cls");
            break;
        case 3:
            system("cls");
            kayitSil();
            printf("\n\nCikis icin 0'e basin.\nAna menu donmek icin 1'e basin:");
            scanf("%d", &choice);
            system("cls");
            break;
        case 4:
            system("cls");
            kayitGuncel();
            printf("\n\nCikis icin 0'e basin.\nAna menu donmek icin 1'e basin:");
            scanf("%d", &choice);
            system("cls");
            break;
        case 5:
            system("cls");
            listAllRecords();
            printf("\n\nCikis icin 0'e basin.\nAna menu donmek icin 1'e basin:");
            scanf("%d", &choice);
            system("cls");
            break;
        case 6:
            system("cls");
            showIndexfile();
            printf("\n\nCikis icin 0'e basin.\nAna menu donmek icin 1'e basin:");
            scanf("%d", &choice);
            system("cls");
            break;
        case 7:
            system("cls");
            deleteIndexFile();
            printf("\n\nCikis icin 0'e basin.\nAna menu donmek icin 1'e basin:");
            scanf("%d", &choice);
            system("cls");
            break;
        case 8:
            system("cls");
            printf("\t\t-------Index File Olustur--------\n\n");
            createIndexFile();
            printf("\nIndex File olusturuldu\n");
            printf("\n\nCikis icin 0'e basin.\nAna menu donmek icin 1'e basin:");
            scanf("%d", &choice);
            system("cls");
            break;
        default :
            break;
        }
    }
    return 0;
}
