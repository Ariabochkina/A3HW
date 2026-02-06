Исследование алгоритма HyperLogLog: оценка числа уникальных объектов в потоке (частотный момент F_t^0) стандартным методом и улучшенной версией с bias-коррекцией.

Структура: src/ — исходники C++17 (RandomStreamGen, HashFuncGen, HyperLogLog, HyperLogLogImproved, main.cpp). scripts/ — визуализация plot_results.py. data/ — CSV с результатами. report/ — отчёт report.md.

Сборка без CMake:
mkdir -p build
g++ -std=c++17 -I src -o build/hll_experiment src/main.cpp src/random_stream_gen.cpp src/hash_func_gen.cpp src/hyperloglog.cpp src/hyperloglog_improved.cpp

С CMake: mkdir -p build && cd build && cmake .. && make

Запуск: ./build/hll_experiment — пишет в data/experiment_results.csv. Параметры: [файл_csv] [B] [число_прогонов]. Улучшенная версия: добавить в конец аргумент improved (по умолчанию пишет в data/experiment_results_improved.csv).

Графики: pip install -r requirements.txt, затем python3 scripts/plot_results.py data/experiment_results.csv -o data (для улучшенной — подставить data/experiment_results_improved.csv). Рисунки сохраняются в каталог -o.

Краткий анализ. При B=10 (m=1024) теоретическая стандартная ошибка σ ≈ 1.04/√m ≈ 3,25%; верхняя граница порядка 1,30/√m ≈ 4%. По экспериментам (потоки 10^4, 5·10^4, 10^5 элементов, шаги 5%–100%, 20 прогонов) относительная ошибка |N_t − F_t^0|/F_t^0 в основном укладывается в 2–6%, среднее около 3–4%, что согласуется с теорией. Дисперсия оценки σ_Nt стабильна по шагам t и по размеру потока. Улучшенная версия расширяет зону линейного подсчёта до 5m и вводит поправку на смещение в интервале (2.5m, 5m], что снижает завышение в переходной области малых кардинальностей при том же объёме памяти.

Воспроизведение: собрать, запустить эксперименты, выполнить скрипт графиков. Подробности и ссылки на статьи — в report/report.md.
