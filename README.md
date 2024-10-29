# Сборка

Сборка под ubuntu осуществляется с помощью докера. Он должен быть установлен и запущен.
Для запуска автоматической сборки и запуска на тестовом сценарии под windows можно использовать _build_and_run_sample.ps1
После сборки docker образа можно запустить его на всех сценариях, использованных при сборке. _run_on_test_data.ps1
Для удобства разработки в windows был добавлен проект vs (/vsproj). Он не используется при сборке docker образа.

# Цель

Цель задания — продемонстрировать навыки проектирования ПО.  
Не нужно усложнять задачу, беспокоиться о производительности, использовать потоки или подключать библиотеки.  
Мы не ограничиваем время выполнения задания, но рассчитываем, что оно займет не более 6-8 часов.

## Что мы хотим увидеть:
1. Максимальная простота и расширяемость кода.
2. Четкие зоны ответственности у каждой сущности в проекте.
3. Архитектура, позволяющая легко добавлять новые сущности и механики взаимодействия.
4. Читаемый и аккуратный код.

# Задача

Сделать пошаговую симуляцию битвы юнитов на карте для проведения экспериментов с механиками.

Можно создать карту размером W x H клеток. На карте можно разместить N юнитов.  
Каждый юнит занимает одну клетку. В одной клетке не может быть несколько юнитов одновременно.

Юниты могут быть разного типа с разными действиями и характеристиками.  
Каждый ход каждый юнит может выполнить только одно действие.
Юниты выполняют действие в ходу в порядке создания юнитов.

Юниты могут перемещаться между клетками карты. Если юнит не может достичь указанной клетки за один ход, он должен продолжать двигаться к ней в каждом следующем ходе, пока не достигнет цели.  
Юниты могут перемещаться только в том случае, если они не могут атаковать других юнитов (Attack-Move).

Симуляция заканчивается, когда нет ни одного юнита, способного совершить действие на следующем ходу, или на карте остается только один юнит.

## Юниты:
- Все юниты имеют уникальный идентификатор **Id** — целое число.
- Все юниты имеют характеристику **HP (Health points)**. Юнит остается живым и совершает действия только пока это значение больше 0.
- Все юниты за один ход могут переместиться в любую из восьми клеток вокруг себя.

### Воин
- Юнит ближнего боя.
- Атакует одного из юнитов в соседней клетке, нанося ему **Strength** единиц урона.
- **Характеристики: HP, Strength**

### Лучник
- Юнит дальнего боя.
- Стреляет, если в радиусе от двух клеток (включительно) до **Range** (включительно) есть другой юнит, нанося ему **Agility** единиц урона. Юнит может стрелять только если в соседних клетках нет других юнитов.
- Также, как и воин, атакует соседнего юнита ближней атакой со **Strength** уроном.
- **Характеристики: HP, Agility, Strength, Range**

# Приложение

При запуске приложение получает в аргументы путь к файлу со сценарием симуляции. В этом сценарии описаны **команды**, позволяющие создавать карту и юнитов, а также перемещать уже созданных юнитов по карте.  
Приложение должна выводить все происходящие события в **stdout**.

Так как на карте нет препятствий, не требуется использовать сложные алгоритмы поиска пути.

## Команды

- `CREATE_MAP W H` — Создает карту размером `W` на `H`. Эта команда начинает любой сценарий.
- `SPAWN_WARRIOR I X Y H S` — Создает воина с идентификатором `I` в точке на карте с координатами `X` и `Y`, с характеристиками здоровья `H` и силы `S`.
- `SPAWN_ARCHER I X Y H A S R` — Создает лучника с идентификатором `I` в точке на карте с координатами `X` и `Y`, с характеристиками здоровья `H`, ловкости `A`, силы `S `и дальности `R`.
- `MARCH I X Y` — Приказывает юниту с идентификатором `I` переместиться в точку с координатами `X` и `Y`.

## События

- `MAP_CREATED` — Создания карты.
- `MARCH_STARTED`, `MARCH_ENDED` — Начало и конец перемещения юнита.
- `UNIT_SPAWNED`, `UNIT_DIED` — Создание и смерть юнита.
- `UNIT_MOVED` — Перемещение юнита на клетку.
- `UNIT_ATTACKED` — Атаки одного юнита на другого.

Код парсера команд и код вывода событий в лог уже содержатся в проекте. Если необходимо, его можно менять. Сам формат ввода и вывода менять нельзя.  

Пример сценария находится в файле _commands_example.txt_.  
Пример лога событий находится в файле _main.cpp_.

# Планы

При разработке стоит учитывать планы на добавление новых юнитов, новых действий и новых характеристик в симуляцию.

### Башня
- Юнит дальнего боя.
- Не может передвигаться. 
- Не может атаковать в ближнем бою. 
- Стреляет в радиусе от двух до пяти клеток включительно.

### Ворон
- Летающий юнит ближнего боя.
- Может перемещаться на две клетки за ход.
- Может атаковать в ближнем бою любых других юнитов. 
- Воин и лучник не могут атаковать ворона в ближнем бою.
- Лучник может стрелять в ворона, даже если он находится в соседней клетке.

### Маг
- Юнит дальнего боя.
- Не может атаковать в ближнем бою. 
- Несколько раз за симуляцию может выстрелить огненным шаром, нанеся урон всем соседним клеткам цели.

### Разбойник
- Ближний бой.
- Несколько раз за симуляцию может парализовать противника на несколько ходов.
- Несколько раз за симуляцию может спрятаться, становясь невидимым для противника.


Эта функциональность не является частью задания, а лишь подсказывает, в каком направлении нужно двигаться.

# Требования

- ОС: **Ubuntu**
- Компилятор: **clang 15+**.
- Стандарт: **С++17** и новее.
- Система сборки: **cmake** (в проекте должен быть CMakeLists.txt).
- Задание необходимо опубликовать в любом публичном гит-репозитории (github, bitbucket, и т.д.).

_В процессе выполнения задания вы можете присылать любые вопросы._