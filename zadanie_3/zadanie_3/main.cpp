//
//  main.cpp
//  zadanie_3
//
//  Created by Anastasiya Charykova on 26.11.2024.
//

#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
#include <utility> // Для std::move

class Data {
public:
    int value; // Скалярное значение
    mutable std::mutex mtx; // Мьютекс для защиты данных

    Data(int v) : value(v) {}
};

// Вариант 1: Использование std::lock
void swapLock(Data& a, Data& b) {
    std::lock(a.mtx, b.mtx); // Захватываем мьютексы
    std::lock_guard<std::mutex> lockA(a.mtx, std::adopt_lock); // Привязываем мьютекс a
    std::lock_guard<std::mutex> lockB(b.mtx, std::adopt_lock); // Привязываем мьютекс b

    std::swap(a.value, b.value); // Обмен значениями
}

// Вариант 2: Использование std::scoped_lock
void swapScopedLock(Data& a, Data& b) {
    std::scoped_lock lock(a.mtx, b.mtx); // Захватываем мьютексы

    std::swap(a.value, b.value); // Обмен значениями
}

// Вариант 3: Использование std::unique_lock
void swapUniqueLock(Data& a, Data& b) {
    std::unique_lock<std::mutex> lockA(a.mtx); // Захватываем мьютекс a
    std::unique_lock<std::mutex> lockB(b.mtx); // Захватываем мьютекс b

    std::swap(a.value, b.value); // Обмен значениями
}

int main() {
    Data data1(1);
    Data data2(2);

    std::cout << "Before swap: data1 = " << data1.value << ", data2 = " << data2.value << std::endl;

    // Пример использования swap с std::lock
    swapLock(data1, data2);
    std::cout << "After swap with std::lock: data1 = " << data1.value << ", data2 = " << data2.value << std::endl;

    // Сброс значений для следующего примера
    data1.value = 1;
    data2.value = 2;

    // Пример использования swap с std::scoped_lock
    swapScopedLock(data1, data2);
    std::cout << "After swap with std::scoped_lock: data1 = " << data1.value << ", data2 = " << data2.value << std::endl;

    // Сброс значений для следующего примера
    data1.value = 1;
    data2.value = 2;

    // Пример использования swap с std::unique_lock
    swapUniqueLock(data1, data2);
    std::cout << "After swap with std::unique_lock: data1 = " << data1.value << ", data2 = " << data2.value << std::endl;

    return 0;
}

/*
 
 
 Класс Data:
     Содержит целочисленное значение value и мьютекс mtx для защиты доступа к этому значению.

 Функции swap:
     swapLock: Использует std::lock для захвата мьютексов обоих объектов. Затем создаются std::lock_guard с флагом std::adopt_lock, чтобы избежать повторного захвата.
     swapScopedLock: Использует std::scoped_lock, который автоматически захватывает оба мьютекса и обеспечивает безопасный обмен значениями.
     swapUniqueLock: Использует std::unique_lock, который позволяет захватывать мьютексы по отдельности.

  main:
     Создает два объекта Data и выводит их значения перед и после обмена для каждого из трёх вариантов.

Этот код демонстрирует, как можно безопасно обменивать данные между объектами с использованием различных механизмов блокировки.
 */
