//
//  main.cpp
//  Zadanie_2
//
//  Created by Anastasiya Charykova on 21.11.2024.
//

#include <chrono>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

const int numThreads = 5; // Количество потоков
const int duration = 10; // Длительность расчёта в секундах
const int barWidth = 50; // Ширина прогресс-бара

std::mutex outputMutex; // Мьютекс для синхронизации вывода в консоль

void progressBar(int threadId) {
    auto startTime = std::chrono::high_resolution_clock::now();

    for (int i = 0; i <= barWidth; ++i) {
        // Имитация работы
        std::this_thread::sleep_for(std::chrono::milliseconds(duration * 1000 / barWidth));

        // Вычисляем процент завершения
        float progress = static_cast<float>(i) / barWidth;

        // Синхронизация доступа к консоли
        {
            std::lock_guard<std::mutex> lock(outputMutex);
            // Устанавливаем курсор на нужную строку
            std::cout << "\033[" << (threadId + 3) << ";1H"; // Позиционирование курсора
            std::cout << std::setw(2) << threadId
                      << "   " << std::this_thread::get_id()
                      << "   [";
            int pos = barWidth * progress;
            for (int j = 0; j < barWidth; ++j) {
                if (j < pos)
                    std::cout << "="; // Заполненная часть
                else
                    std::cout << " "; // Пустая часть
            }
            std::cout << "] " << std::fixed << std::setprecision(2)
                      << (progress * 100) << "%";
            std::cout.flush(); // Обновляем вывод
        }
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = endTime - startTime;

    // Вывод времени выполнения после завершения потока
    {
        std::lock_guard<std::mutex> lock(outputMutex);
        std::cout << "            " << elapsed.count() << " сек.\n";
        std::cout << "\033[" << (threadId + 3) << ";1H"; // Позиционирование курсора
        // std::cout << std::setw(2) << threadId
        //           << "   " << std::this_thread::get_id()
        //           << "   Время выполнения: " << elapsed.count() << " сек." << std::endl;
       // std::cout << elapsed.count() << " сек.\n";
         
        
    }
}

int main() {
    std::vector<std::thread> threads;

    // Вывод заголовка таблицы
     std::cout<< std::setw(2) << "#" << std::setw(15) << "id" << std::setw(45) << "Progress bar"<< std::setw(40) << "time" <<std::endl;
    
  std::cout << std::endl;

    // Запуск потоков
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(progressBar, i);
    }

    // Ожидание завершения всех потоков
    for (auto &thread : threads) {
        thread.join();
    }

    return 0;
}
