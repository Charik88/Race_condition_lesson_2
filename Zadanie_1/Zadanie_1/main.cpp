/*Задание 1
Атомарная очередь клиентов
Нужно модифицировать задание 1 к первому уроку так, чтобы счётчик клиентов был атомарным.
Все операции со счётчиков должны быть атомарными.
Проверьте работу различными способами упорядочения доступа к памяти.
 #include <iostream>
 #include <thread>
 #include <atomic>
 #include <chrono>

 // Атомическая переменная для счётчика клиентов
 std::atomic<int> clientCounter(0);
 const int maxClients = 10; // Максимальное количество клиентов

 // Функция-поток для клиента
 void client() {
     for (int i = 0; i < maxClients; i++) {
         clientCounter++;
         std::cout << "Client: " << clientCounter << std::endl;
         std::this_thread::sleep_for(std::chrono::seconds(1));
     }
 }

 // Функция-поток для оператора
 void operatorService() {
     while (clientCounter > 0) {
         clientCounter--;
         std::cout << "Operator: " << clientCounter << std::endl;
         std::this_thread::sleep_for(std::chrono::seconds(2));
     }
 }

 int main() {
     std::thread clientThread(client);
     std::thread operatorThread(operatorService);

     // Ждём окончания работы потоков
     clientThread.join();
     operatorThread.join();

     return 0;
 }
*/



#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

// Атомическая переменная для счётчика клиентов
std::atomic<int> clientCounter(0);
const int maxClients = 10; // Максимальное количество клиентов

// Функция-поток для клиента
void client() {
    for (int i = 0; i < maxClients; i++) {
        // Используем memory_order_relaxed для увеличения
        clientCounter.fetch_add(1, std::memory_order_relaxed);
        std::cout << "Клиент: " << clientCounter.load(std::memory_order_relaxed) << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

// Функция-поток для оператора
void operatorService() {
    while (true) {
        // Используем memory_order_acquire для чтения
        int currentCount = clientCounter.load(std::memory_order_acquire);
        if (currentCount == 0) {
            break; // Выходим из цикла, если клиентов нет
        }
        // Используем memory_order_release для уменьшения
        clientCounter.fetch_sub(1, std::memory_order_release);
        std::cout << "Оператор: " << clientCounter.load(std::memory_order_relaxed) << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

int main() {
    std::thread clientThread(client);
    std::thread operatorThread(operatorService);

    // Ждём окончания работы потоков
    clientThread.join();
    operatorThread.join();

    return 0;
}

// fetch_add и fetch_sub: Эти функции обеспечивают атомарное увеличение и уменьшение счётчика. Они также возвращают предыдущее значение счётчика, если это нужно
// memory_order_acquire и memory_order_release используются для гарантирования правильного порядка операций чтения и записи. Это помогает избежать проблем с видимостью изменений между потоками.
// В функции operatorService добавлена проверка на ноль после загрузки текущего значения счётчика. Если клиентов больше нет, оператор завершает работу.
