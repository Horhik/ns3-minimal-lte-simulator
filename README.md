# Основное задание 1
1. Установить NS-3 и скомпилировать. 

2. С помощью документации NS-3 сделать минимальный LTE сценарий. 

   a. Есть eNB и два абонента. 
   b. Траффик Full Buffer (пакеты идут в обе стороны бесконечно). 
   c. В LTE модуле сконфигурирован планировщик пакетов pf-ff-mac-scheduler. 
   d. В LTE модуле сконфигурирован вывод ключевых характеристик с Rlc и MAC  уровня. 

3. Запустить сценарий и получить вывод ключевых характеристик.  

## Решение

Решение находится в файле `minimal-lte.cc`

Что бы собрать и запустить симуляцию, можно воспользоватся скриптом `compute.sh`

```sh
bash compute.sh /your/path/to/ns-3/folder
```

Ключевые характеристики будут находится в файлах: `DlMacStats.txt`,  `DlRlcStats.txt`,  `UlMacStats.txt`,  `UlRlcStats.txt`

Также можно изменить некоторые параметры симуляции, о которых можно узнать с помощью флага `--help`


# Основное задание 2
1. Написать скрипт, который по полученному выводу ключевых характеристик с Rlc уровня  посчитает Throughput в DL и в UL для каждого пользователя отдельно и выведет его на  экран. 

## Решение

Решение находится в файле `throughput-compute.py` и требует установленную библиотеку `pandas`

```sh
pip -m venv .venv
source .venv/bin/activate
pip install pandas
python throughput-compute.py
```

Скрипт выводит пропускную способность и IMSI каждого пользователя:

```bash
(.venv) ➜ python throughput-compute.py

DL Throughput per user (Mbps):
   IMSI  Throughput_DL
0     1       8.694903
1     2       8.693986

UL Throughput per user (Mbps):
   IMSI  Throughput_UL
0     1       8.694903
1     2       8.693986

```
