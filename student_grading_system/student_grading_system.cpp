#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Student {
    int number;
    char name[50];
    char middleName[50];
    char surname[50];
    float grades[5];
    float average;
    char letterGrade;
};

// Öðrencinin not ortalamasýný ve harf notunu hesaplayan fonksiyon
// Function to calculate the average and letter grade of a student based on their grades
void calculateGrades(struct Student *student) {
    float total = 0;

    // Öðrencinin notlarýný topla
    // Sum up the student's grades
    for (int i = 0; i < 5; i++) {
        total += student->grades[i];
    }

    // Ortalamayý hesapla
    // Calculate the average
    student->average = total / 5;

    // Harf notunu belirle
    // Determine the letter grade
    if (student->average >= 90) {
        student->letterGrade = 'A';
    } else if (student->average >= 80) {
        student->letterGrade = 'B';
    } else if (student->average >= 70) {
        student->letterGrade = 'C';
    } else if (student->average >= 60) {
        student->letterGrade = 'D';
    } else {
        student->letterGrade = 'F';
    }
}

// Yeni bir öðrenciyi dosyaya ekleyen fonksiyon
// Function to add a new student to the file
void addStudent(struct Student student) {
    FILE *file = fopen("student_info.txt", "a");

    // Dosyayý kontrol et
    // Check if the file is open
    if (file == NULL) {
        printf("Error opening file\n");
        exit(1);
    }

    // Öðrenci bilgilerini dosyaya yaz
    // Write student information to the file
    fprintf(file, "%d %s %s %s ", student.number, student.name, student.middleName, student.surname);

    for (int i = 0; i < 5; i++) {
        fprintf(file, "%.2f ", student.grades[i]);
    }

    fprintf(file, "%.2f %c\n", student.average, student.letterGrade);

    // Dosyayý kapat
    // Close the file
    fclose(file);
}

// Dosyadaki öðrenci bilgilerini ekrana yazdýran fonksiyon
// Function to print student information from the file to the screen
void listStudents() {
    FILE *file = fopen("student_info.txt", "r");

    // Dosyayý kontrol et
    // Check if the file is open
    if (file == NULL) {
        printf("Error opening file\n");
        exit(1);
    }

    char line[200];

    // Dosyadan okuyarak ekrana yaz
    // Read from the file and print to the screen
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);
    }

    // Dosyayý kapat
    // Close the file
    fclose(file);
}

// Bir öðrencinin notlarýný güncelleyen fonksiyon
// Function to update the grades of a student
void updateStudent(int number, struct Student *student) {
    FILE *file = fopen("student_info.txt", "r+");
    
    // Dosyayý kontrol et
    // Check if the file is open
    if (file == NULL) {
        printf("Error opening file\n");
        exit(1);
    }

    char line[200];
    long filePosition = 0;

    // Dosyadaki her satýrý kontrol et
    // Check each line in the file
    while (fgets(line, sizeof(line), file) != NULL) {
        sscanf(line, "%d %49[^\n] %49[^\n] %49[^\n] %f %f %f %f %f %f %f %c", 
               &student->number, 
               student->name, 
               student->middleName, 
               student->surname,
               &student->grades[0],
               &student->grades[1],
               &student->grades[2],
               &student->grades[3],
               &student->grades[4],
               &student->average,
               &student->letterGrade);

        // Eðer öðrenci bulunduysa pozisyonu kaydet ve döngüden çýk
        // If the student is found, save the position and exit the loop
        if (student->number == number) {
            filePosition = ftell(file) - strlen(line);
            break;
        }
    }

    // Eðer öðrenci bulunamadýysa hata mesajý yaz ve dosyayý kapat
    // If the student is not found, write an error message and close the file
    if (filePosition == 0) {
        printf("Student not found\n");
        fclose(file);
        return;
    }

    // Sadece notlarý güncelle
    // Only update the grades
    printf("New Course Grades (5 courses): ");
    for (int i = 0; i < 5; i++) {
        scanf("%f", &student->grades[i]);
    }

    // Ortalamayý ve harf notunu hesapla
    // Calculate the average and letter grade
    calculateGrades(student);

    // Dosyanýn konumunu güncelle
    // Update the file position
    fseek(file, filePosition, SEEK_SET);

    // Dosyaya yeni bilgileri yaz
    // Write the new information to the file
    fprintf(file, "%d %s %s %s %.2f %.2f %.2f %.2f %.2f %.2f %.2f %c\n", 
            student->number, 
            student->name, 
            student->middleName, 
            student->surname,
            student->grades[0],
            student->grades[1],
            student->grades[2],
            student->grades[3],
            student->grades[4],
            student->average,
            student->letterGrade);

    // Dosyayý kapat
    // Close the file
    fclose(file);
}

// Bir öðrenciyi dosyadan silen fonksiyon
// Function to remove a student from the file
void removeStudent(int number) {
    FILE *file = fopen("student_info.txt", "r");
    
    // Dosyayý kontrol et
    // Check if the file is open
    if (file == NULL) {
        printf("Error opening file\n");
        exit(1);
    }

    FILE *tempFile = fopen("temp.txt", "w");

    // Geçici dosyayý kontrol et
    // Check if the temporary file is open
    if (tempFile == NULL) {
        printf("Error opening temporary file\n");
        fclose(file);
        exit(1);
    }

    char line[200];

    // Dosyadan okuyarak geçici dosyaya yaz
    // Read from the file and write to the temporary file
    while (fgets(line, sizeof(line), file) != NULL) {
        int currentNumber;
        sscanf(line, "%d", &currentNumber);

        // Silinecek öðrenciyi bulma
        // Find the student to be removed
        if (currentNumber != number) {
            fprintf(tempFile, "%s", line);
        }
    }

    // Dosyalarý kapat
    // Close the files
    fclose(file);
    fclose(tempFile);

    // Eski dosyayý sil, geçici dosyayý adlandýr
    // Remove the old file, rename the temporary file
    remove("student_info.txt");
    rename("temp.txt", "student_info.txt");
}

// Öðrenci ekleme iþlemini gerçekleþtiren fonksiyonu çaðýran fonksiyon
// Function to call the function that performs the student removal operation
void removeStudentCommand() {
    int studentNumber;
    printf("\nEnter Student Number (For Removal): ");
    scanf("%d", &studentNumber);
    removeStudent(studentNumber);
    printf("\nStudent Removed.\n");
}

// Ana fonksiyon
// Main function
int main() {
    struct Student newStudent;
    int choice;

    // Kullanýcýya menüyü göster
    // Show the menu to the user
    while (1) {
        printf("\n1. Add Student\n");
        printf("2. List Students\n");
        printf("3. Remove Student\n");
        printf("0. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        // Kullanýcýnýn seçimine göre iþlem yap
        // Perform operations based on user's choice
        switch (choice) {
            case 1:
                // Yeni öðrenci bilgilerini al ve dosyaya ekle
                // Get new student information and add to the file
                printf("Student Number: ");
                scanf("%d", &newStudent.number);

                printf("Student Name: ");
                scanf("%s", newStudent.name);

                printf("Student Middle Name (If you don't have a middle name, use dot(.)): ");
                scanf("%s", newStudent.middleName);

                printf("Student Surname: ");
                scanf("%s", newStudent.surname);

                printf("Course Grades (5 courses): ");
                for (int i = 0; i < 5; i++) {
                    scanf("%f", &newStudent.grades[i]);
                }

                // Ortalamayý ve harf notunu hesapla, öðrenciyi dosyaya ekle
                // Calculate the average and letter grade, add student to the file
                calculateGrades(&newStudent);
                addStudent(newStudent);
                break;

            case 2:
                // Dosyadaki öðrenci bilgilerini ekrana yazdýr
                // Print student information from the file to the screen
                printf("\nStudent Information:\n");
                listStudents();
                break;

            case 3:
                // Öðrenci silme iþlemi
                // Student removal operation
                removeStudentCommand();
                break;

            case 0:
                // Programdan çýkýþ
                // Exit the program
                printf("Exiting the program...\n");
                exit(0);
                break;

            default:
                // Geçersiz seçenek, tekrar deneyin
                // Invalid choice, try again
                printf("Invalid choice, please try again.\n");
                break;
        }
    }

    return 0;
}

