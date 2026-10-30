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
int lengths[MAX_LEN];
int num_words = 0;
int result_len = 0;
int num_letters = 0;
bool is_leading[26] = { 0 };

bool solution(int deep, char operators[]) {
    if (deep == num_letters) {
        double nums[MAX_WORDS];
        for (int i = 0; i < num_words; i++) {
            long long num = 0;
            int len = lengths[i];
            for (int j = 0; j < len; j++) {
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
        for (int i = 0; i < num_words - 1 && ops[i] != '\0'; i++) {
            if (ops[i] == '+') left_part += nums[i + 1];
            else if (ops[i] == '-') left_part -= nums[i + 1];
        }
        long long right_res = 0;
        for (int j = 0; j < result_len; j++) {
            right_res = right_res * 10 + value_letters[result_word[j] - 'A'];
        }
        return fabs(left_part - (double)right_res) < 1e-9;
    }
    int current = letters[deep] - 'A';
    for (int digit = 0; digit <= 9; digit++) {
        if (!taken_nums[digit]) {
            if (digit == 0 && is_leading[letters[deep] - 'A']) {
                continue;
            }
            value_letters[current] = digit;
            taken_nums[digit] = true;
            if (solution(deep + 1, operators)) return true;
            value_letters[current] = -1;
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
            int len = strlen(wd);
            if (in_result) result_len = len;
            else {
                lengths[widx] = len;
                num_words++;
            }
            for (int i = 0; wd[i] != '\0'; i++) {
                if (wd[i] >= 'A' && wd[i] <= 'Z') uniq[wd[i] - 'A'] = true;
            }
            if (len > 1) {
                is_leading[wd[0] - 'A'] = true;
            }
            if (!in_result) widx++;
        }
        token = strtok(NULL, " ");
    }
    for (int i = 0; i < 26; i++) {
        if (uniq[i]) letters[num_letters++] = (char)('A' + i);
    }
    clock_t start = clock();
    if (solution(0, operators)) {
        printf("Решение: ");
        for (int i = 0; i < num_words; i++) {
            for (int j = 0; j < lengths[i]; j++) {
                printf("%d", value_letters[words[i][j] - 'A']);
            }
            if (i < num_words - 1) printf(" %c ", operators[i]);
        }
        printf(" = ");
        for (int j = 0; j < result_len; j++) {
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
