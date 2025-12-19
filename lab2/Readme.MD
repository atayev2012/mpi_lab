# Лабораторная работа №2
## Задача №3
Создать свою реализацию типа rwlock и функций rdlock,
wrlock блокировки чтения-записи. Апробировать эту реализацию
на примере односвязного списка. См. файл pth_ll_rwl.c. Сравнить производительность своей реализации с библиотечной.

### Реализация
Файлы:
`./include/rwl.h`
`./task_3/rwl.c`

### Сравнение
|  **pthreads_rwl**          |  **custom rwl** |
:-------------------------:|:-------------------------:
1 thread | 1 thread
<img src="img/orig_rwl_threads_1.png" alt="orig_rwl_1" width="300px"> | <img src="img/custom_rwl_threads_1.png" width="300px" alt="custom_rwl_1">
4 threads | 4 threads
<img src="img/orig_rwl_threads_4.png" alt="orig_rwl_4" width="300px"> | <img src="img/custom_rwl_threads_4.png" width="300px" alt="custom_rwl_4">
8 threads | 8 threads
<img src="img/orig_rwl_threads_8.png" alt="orig_rwl_8" width="300px"> | <img src="img/custom_rwl_threads_8.png" width="300px" alt="custom_rwl_8">
