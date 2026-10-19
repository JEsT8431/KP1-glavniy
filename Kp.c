#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <math.h>

double get_q(double x)
{
    if (x < 100) return 2.0 * x + 10;
    if (x < 200) return 1.5 * x + 60;
    return 0.8 * x + 200;
}

int main()
{
    setlocale(LC_ALL, "RUS");

    double x;
    double q0 = 350.0;    // Целевое значение
    double smax = 1500.0; // Максимальное отклонение

    double Q[1000];       // Массив для хранения всех значений Q
    int n = 0;            // Счетчик измерений

    double s_current = 0; // Текущее накопленное отклонение
    int alarm_found = 0;  // Флаг первого превышения

    printf("Введите показания датчика X (любой символ для выхода):\n");

    while (scanf("%lf", &x) == 1)
    {
        Q[n] = get_q(x); // Считаем Q и сохраняем в массив
        s_current += fabs(Q[n] - q0); // Сразу считаем S

        printf("X=%.1f -> Q=%.1f ", x, Q[n]);

        if (s_current > smax && alarm_found == 0)
        {
            alarm_found = 1;
            printf("[ТРЕВОГА!] Первое превышение Smax!\n");
            printf("  -> Значение Q в этот момент: %.2f\n", Q[n]);
            printf("  -> Накопленное отклонение: %.2f\n", s_current);
            printf("  -> Обработано измерений: %d\n", n + 1);
        }
        else
        {
            printf("[S=%.1f, норма]\n", s_current);
        }

        n++; 
    }

    if (n == 0)
    {
        printf("Нет данных для обработки.\n");
        return 0;
    }

    double sum = 0;
    double min_q = Q[0], max_q = Q[0];

    for (int i = 0; i < n; i++)
    {
        sum += Q[i];
        if (Q[i] < min_q) min_q = Q[i];
        if (Q[i] > max_q) max_q = Q[i];
    }
    double mean = sum / n;

    double var = 0;
    for (int i = 0; i < n; i++)
    {
        var += (Q[i] - mean) * (Q[i] - mean);
    }
    var = (n > 1) ? var / (n - 1) : 0; // Выборочная дисперсия
    double std = sqrt(var);

    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - 1 - i; j++)
        {
            if (Q[j] > Q[j + 1])
            {
                double temp = Q[j];
                Q[j] = Q[j + 1];
                Q[j + 1] = temp;
            }
        }
    }

    double med;
    if (n % 2 != 0)
    {
        med = Q[n / 2]; // Нечетное количество
    }
    else
    {
        med = (Q[n / 2 - 1] + Q[n / 2]) / 2.0; // Четное
    }

    printf("\n--- Статистика ---\n");
    printf("Среднее:             %.2f\n", mean);
    printf("Медиана:             %.2f\n", med);
    printf("Дисперсия:           %.2f\n", var);
    printf("Станд. отклонение:   %.2f\n", std);
    printf("Минимум:             %.2f\n", min_q);
    printf("Максимум:            %.2f\n", max_q);
    printf("Диапазон [mean-std, mean+std]: [%.2f, %.2f]\n", mean - std, mean + std);

    return 0;
}