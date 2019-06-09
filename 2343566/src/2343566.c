/*
 ============================================================================
 Name        : spisok.c
 Author      : D.Nest
 Version     : 1.0
 Description :
----------------------------------------------
Входной файл :
+ s 4 4
- s 8 4
* s 2 4
/ s 4 2
^ s 5 5
! s 3
+ v 2 1 3 5 4
- v 2 7 2 5 1
* v 2 3 2 4 5
------------------------------------------------
Выходной файл :
Результат сложения: 8.00
Результат вычитания: 4.00
Результат умножения: 8.00
Результат деления: 2.00
Результат возведения в степень: 80.00
Факториал: 6.00
{ 1.00  3.00 } + { 5.00  4.00 } = { 6.00  7.00 }
{ 7.00  2.00 } - { 5.00  1.00 } = { 2.00  1.00 }
{ 3.00  2.00 } * { 4.00  5.00 } =  22.00

 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct first_list {
    char operation; //Операция
    char type; // Тип операции ( векторная или числовая)
    int size; // Размерность вектора
    float* value1; // Число 1
    float* value2; // Число 2
    struct first_list* next; // Переход к следующему элементу
} elem;

typedef struct final_list {
    float* result; // Хранение результата
    struct final_list* next_final; // Переход к последнему элементу
} result;
//Функция для вычисления результата числовых операций.
float* number_calc(char operation, float* value1, float* value2)
{
    float* result_elem; //Возвращаем указатель
    result_elem = malloc(1 * sizeof(float)); //Выделяем память для результата
    float res; //Локальная переменная для факториала
    if (operation == '+') {
        result_elem[0] = value1[0] + value2[0];
    }
    if (operation == '-') {
        result_elem[0] = value1[0] - value2[0];
    }
    if (operation == '*') {
        result_elem[0] = value1[0] * value2[0];
    }
    if (operation == '/') {
        if (value2[0] == 0)
            result_elem[0] = 0;
        else
            result_elem[0] = value1[0] / value2[0];
    }
    if (operation == '^') {
        result_elem[0] = value1[0];
        if (value2[0] != 0) {
            for (int i = 1; i < value2[0]; i++) {
                value1[0] = value1[0] + result_elem[0];
                result_elem[0] = value1[0];
            }
        }
        else {
            result_elem[0] = 1;
        }
    }
    if (operation == '!') {
        res = 1;
        for (int i = 1; i <= value1[0]; i++) {
            res = res * i;
        }
        result_elem[0] = res;
    }
    return result_elem;
}
//Функции для опредление результата векторных операций.
float* vector_calc(char operation, int size, float* vector1, float* vector2)
{
    float* vector; //Массив результата операции с векторами.
    if (operation != '*') {
        vector = malloc(size * sizeof(float)); //Выделяем память для длинны вектора
        if (operation == '+') {
            for (int i = 0; i < size; i++) {
                vector[i] = vector1[i] + vector2[i];
            }
        }
        if (operation == '-') {
            for (int i = 0; i < size; i++) {
                vector[i] = vector1[i] - vector2[i];
            }
        }
    }
    else {
        vector = malloc(1 * sizeof(float));
        vector[0] = 0;
        for (int i = 0; i < size; i++) {
            vector[0] = vector[0] + (vector1[i] * vector2[i]);
        }
    }
    return vector;
}

//Функция для записи первого и второго числа
float* addnumber(FILE* input, int size)
{
    float* number;
    number = malloc(size * sizeof(float));
    for (int i = 0; i < size; i++) {
        fscanf(input, "%f", &number[i]);
    }
    return number;
}
//Функция для добавления элемента в лист
void addelement(elem* now_elem, FILE* input)
{
    elem* tmp = malloc(1 * sizeof(elem));
    //Добавление в структуру.
    fscanf(input, " %c %c", &tmp->operation, &tmp->type);
    if (tmp->type == 'v') {
        fscanf(input, " %i", &tmp->size);
    }
    else {
        tmp->size = 1;
    }
    if (tmp->operation != '!') {
        tmp->value1 = addnumber(input, tmp->size);
        tmp->value2 = addnumber(input, tmp->size);
    }
    else {
        tmp->value1 = addnumber(input, tmp->size);
        tmp->value2 = NULL;
    }
    tmp->next = NULL;
    now_elem->next = tmp;
}
//Функция для добавления результата
void addelement_result(result* now_elem_res, elem* now_elem)
{
    result* tmp_res = malloc(1 * sizeof(result));
    if (now_elem->type == 'v') {
        tmp_res->result = vector_calc(now_elem->operation, now_elem->size, now_elem->value1, now_elem->value2);
    }
    else {
        tmp_res->result = number_calc(now_elem->operation, now_elem->value1, now_elem->value2);
    }
    tmp_res->next_final = NULL;
    now_elem_res->next_final = tmp_res;
}

int main()
{
    FILE *input, *output; //Файлы на ввод и вывод
    elem *head, *now_elem; //Указатели для списка с данными. На начало и на текущий элемент
    result *head_res, *now_elem_res; //Указатели для списка ответов на начало и текущий элемент
    input = fopen("input.txt", "r"); //Открываем файл входящих данных
    if (!feof(input)) {
        head = malloc(1 * sizeof(elem)); //Выделяем память
        fscanf(input, " %c %c", &head->operation, &head->type);
        //Записываем данные для первого элемента
        if (head->type == 'v') {
            fscanf(input, " %i", &head->size);
        }
        else {
            head->size = 1;
        }
        if (head->operation != '!') {
            head->value1 = addnumber(input, head->size);
            head->value2 = addnumber(input, head->size);
        }
        else {
            head->value1 = addnumber(input, head->size);
            head->value2 = NULL;
        }
        now_elem = head;
        //Копируем указатели и добавляем элементы до последней строчки
        while (!feof(input)) {
            addelement(now_elem, input);
            now_elem = now_elem->next;
        }
        head_res = malloc(sizeof(result));
        now_elem = head;
        if (now_elem->type == 'v') {
            head_res->result = vector_calc(now_elem->operation, now_elem->size, now_elem->value1, now_elem->value2);
        }
        else {
            head_res->result = number_calc(now_elem->operation, now_elem->value1, now_elem->value2);
        }
        head_res->next_final = NULL;
        now_elem = now_elem->next;
        now_elem_res = head_res;
        while (now_elem != NULL) { // Пока не закончились элементы
            addelement_result(now_elem_res, now_elem); //Добавляем их
            now_elem = now_elem->next;
            now_elem_res = now_elem_res->next_final;
        }
        now_elem = head;
        now_elem_res = head_res;
        fclose(input); //Закрываем файл на ввод
        output = fopen("output.txt", "w"); // Открываем файл на вывод
        while (now_elem != NULL) {
            if (now_elem->type == 'v') {
                fprintf(output, "{");
                //Вывод данных из value1,value2 и result
                for (int i = 0; i < now_elem->size; i++) {
                    fprintf(output, " %.2f ", now_elem->value1[i]);
                }
                fprintf(output, "} %c {", now_elem->operation);
                for (int i = 0; i < now_elem->size; i++) {
                    fprintf(output, " %.2f ", now_elem->value2[i]);
                }
                fprintf(output, "} = ");
                if (now_elem->operation != '*') {
                    fprintf(output, "{");
                    for (int i = 0; i < now_elem->size; i++) {
                        fprintf(output, " %.2f ", now_elem_res->result[i]);
                    }
                    fprintf(output, "}\n");
                }
                else {
                    fprintf(output, " %.2f\n", now_elem_res->result[0]);
                }
            }
            else {
                switch (now_elem->operation) {
                case '+':
                    fprintf(output, "Результат сложения: %.2f\n", now_elem_res->result[0]);
                    break;
                case '-':
                    fprintf(output, "Результат вычитания: %.2f\n", now_elem_res->result[0]);
                    break;
                case '*':
                    fprintf(output, "Результат умножения: %.2f\n", now_elem_res->result[0]);
                    break;
                case '/':
                    fprintf(output, "Результат деления: %.2f\n", now_elem_res->result[0]);
                    break;
                case '!':
                    fprintf(output, "Факториал: %.2f\n", now_elem_res->result[0]);
                    break;
                case '^':
                    fprintf(output, "Результат возведения в степень: %.2f\n", now_elem_res->result[0]);
                    break;
                }
            }
            now_elem = now_elem->next;
            now_elem_res = now_elem_res->next_final;
        }
        fclose(output);
    }
    return 0;
}
