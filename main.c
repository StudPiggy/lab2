#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <locale.h>
#include <windows.h>
#include <math.h>
#include <time.h>
#define MAX_WORDS 7
#define MAX_LEN 25

char words[MAX_WORDS][MAX_LEN];
char result_word[MAX_LEN];
int value_letters[26];
bool taken_nums[10] = { 0 };
char letters[10];

int letter_weight(char c) {
    int weight = 0, len;
    for (int i = 0; i < MAX_WORDS && words[i][0] != '\0'; i++) {
        len = strlen(words[i]);
        for (int j = len - 1; j >= 0; j--) {
            if (words[i][j] == c) {
                weight += (len - j);
                break;
            }
        }
    }
    len = strlen(result_word);
    for (int j = len - 1; j >= 0; j--) {
        if (result_word[j] == c) {
            weight += (len - j);
            break;
        }
    }
    return weight;
}

bool solution(int deep, char operators[]) {
    if (letters[deep] == '\0') {
        double nums[MAX_WORDS];
        for (int i = 0; i < MAX_WORDS && words[i][0] != '\0'; i++) {
            long long num = 0;
            for (int j = 0; words[i][j] != '\0'; j++) {
                num = num * 10 + value_letters[words[i][j] - 'A'];
            }
            nums[i] = num;
        }
        char ops[MAX_WORDS];
        strcpy(ops, operators);
        for (int i = 0; i < MAX_WORDS - 1 && ops[i] != '\0'; i++) {
            if (ops[i] == '*') {
                nums[i + 1] = nums[i] * nums[i + 1];
                nums[i] = 0;
                ops[i] = '+';
            }
            else if (ops[i] == '/') {
                if (nums[i + 1] == 0) return false;
                nums[i + 1] = nums[i] / nums[i + 1];
                nums[i] = 0;
                ops[i] = '+';
            }
        }
        double left_part = nums[0];
        for (int i = 0; i < MAX_WORDS - 1 && ops[i] != '\0'; i++) {
            if (ops[i] == '+') left_part += nums[i + 1];
            else if (ops[i] == '-') left_part -= nums[i + 1];
        }
        long long right_res = 0;
        for (int j = 0; result_word[j] != '\0'; j++) {
            right_res = right_res * 10 + value_letters[result_word[j] - 'A'];
        }
        return fabs(left_part - (double)right_res) < 1e-9;
    }
    for (int digit = 0; digit <= 9; digit++) {
        if (!taken_nums[digit]) {
            bool first = false;
            for (int i = 0; i < MAX_WORDS && words[i][0] != '\0'; i++) {
                if (words[i][0] == letters[deep]) first = true;
            }
            if (result_word[0] == letters[deep]) first = true;
            if (digit == 0 && first) continue;
            value_letters[letters[deep] - 'A'] = digit;
            taken_nums[digit] = true;
            if (solution(deep + 1, operators)) return true;
            value_letters[letters[deep] - 'A'] = -1;
            taken_nums[digit] = false;
        }
    }
    return false;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "rus");
    char rebus[256];
    printf("Числовой ребус: ");
    if (!fgets(rebus, sizeof(rebus), stdin)) return 1;
    rebus[strcspn(rebus, "\n")] = 0;
    for (int i = 0; i < 26; i++) value_letters[i] = -1;
    bool uniq[26] = { 0 };
    bool in_result = false;
    memset(words, 0, sizeof(words));
    memset(result_word, 0, sizeof(result_word));
    memset(letters, 0, sizeof(letters));
    char operators[MAX_WORDS] = { 0 };
    int widx = 0, opidx = 0;
    char* token = strtok(rebus, " ");
    while (token != NULL) {
        if (strcmp(token, "=") == 0) in_result = true;
        else if (strcmp(token, "+") == 0 || strcmp(token, "-") == 0 ||
            strcmp(token, "*") == 0 || strcmp(token, "/") == 0) {
            operators[opidx++] = token[0];
        }
        else {
            char* wd = in_result ? result_word : words[widx];
            strcpy(wd, token);
            for (int i = 0; wd[i] != '\0'; i++) {
                if (wd[i] >= 'A' && wd[i] <= 'Z') uniq[wd[i] - 'A'] = true;
            }
            if (!in_result) widx++;
        }
        token = strtok(NULL, " ");
    }
    int lidx = 0;
    for (int i = 0; i < 26; i++) {
        if (uniq[i]) letters[lidx++] = (char)('A' + i);
    }
    int n = lidx;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (letter_weight(letters[j]) < letter_weight(letters[j + 1])) {
                char temp = letters[j];
                letters[j] = letters[j + 1];
                letters[j + 1] = temp;
            }
        }
    }
    clock_t start = clock();
    if (solution(0, operators)) {
        printf("Решение: ");
        for (int i = 0; i < MAX_WORDS && words[i][0] != '\0'; i++) {
            for (int j = 0; words[i][j] != '\0'; j++) {
                printf("%d", value_letters[words[i][j] - 'A']);
            }
            if (words[i + 1][0] != '\0') printf(" %c ", operators[i]);
        }
        printf(" = ");
        for (int j = 0; result_word[j] != '\0'; j++) {
            printf("%d", value_letters[result_word[j] - 'A']);
        }
        printf("\n");
    }
    else {
        printf("Решения нет.\n");
    }
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Время решения: %.4f сек.", time_spent);
    return 0;
}
