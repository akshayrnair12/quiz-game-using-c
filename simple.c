#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_QUESTIONS 100
#define MAX_OPTION_LENGTH 100

typedef struct {
    char question[200];
    char options[4][MAX_OPTION_LENGTH];
    char correctOption;
} QuizQuestion;

void askQuestion(QuizQuestion q, int *score) {
    char answer;

    printf("\n%s\n", q.question);
    printf("A. %s\n", q.options[0]);
    printf("B. %s\n", q.options[1]);
    printf("C. %s\n", q.options[2]);
    printf("D. %s\n", q.options[3]);
    printf("Enter your answer (A/B/C/D): ");
    scanf(" %c", &answer);

    if (answer == q.correctOption || answer == q.correctOption + 32) {
        printf("Correct!\n");
        (*score)++;
    } else {
        printf("Wrong! Correct answer is: %c\n", q.correctOption);
    }
}

int loadQuestionsFromFile(const char *filename, QuizQuestion questions[]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open file: %s\n", filename);
        return 0;
    }

    int count = 0;
    while (fgets(questions[count].question, sizeof(questions[count].question), file)) {
        questions[count].question[strcspn(questions[count].question, "\n")] = 0;

        int i;
        for (i = 0; i < 4; i++) {
            if (!fgets(questions[count].options[i], sizeof(questions[count].options[i]), file)) {
                fclose(file);
                return count;
            }
            questions[count].options[i][strcspn(questions[count].options[i], "\n")] = 0;
        }

        char correct;
        if (fscanf(file, " %c\n", &correct) != 1) {
            break;
        }
        questions[count].correctOption = correct;

        count++;
        if (count >= MAX_QUESTIONS) break;
    }

    fclose(file);
    return count;
}

void saveScore(const char *username, int score, int total) {
    FILE *file = fopen("scores.txt", "a");
    if (file) {
        fprintf(file, "%s: %d/%d\n", username, score, total);
        fclose(file);
    }
}

int main() {
    QuizQuestion questions[MAX_QUESTIONS];
    int totalQuestions = loadQuestionsFromFile("quiz.txt", questions);

    if (totalQuestions == 0) {
        printf("No questions loaded. Exiting...\n");
        return 1;
    }

    char username[50];
    printf("===== Welcome to the Intermediate C Quiz App =====\n");
    printf("Enter your name: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;

    int score = 0;
    int i;
    for (i = 0; i < totalQuestions; i++) {
        askQuestion(questions[i], &score);
    }

    printf("\n===== Quiz Over! =====\n");
    printf("Name: %s\n", username);
    printf("Final Score: %d out of %d\n", score, totalQuestions);

    saveScore(username, score, totalQuestions);

    return 0;
}

