# Lift test task by Kirill Scherba application

## 1. Description

Предлагаем вам решить тестовое задание — написать программу «симулятор лифта». 

Программа запускается из командной строки, в качестве параметров задается:

- кол-во этажей в подъезде — N (от 5 до 20);
- высота одного этажа;
- скорость лифта при движении в метрах в секунду (ускорением пренебрегаем, считаем, 
  что когда лифт едет — он сразу едет с определенной скоростью);
- время между открытием и закрытием дверей.

После запуска программа должна постоянно ожидать ввода от пользователя и выводить 
действия лифта в реальном времени. События, которые нужно выводить:

- лифт проезжает некоторый этаж;
- лифт открыл двери;
- лифт закрыл двери.

Возможный ввод пользователя:

- вызов лифта на этаж из подъезда;
- нажать на кнопку этажа внутри лифта.

Считаем, что пользователь не может помешать лифту закрыть двери.

Все данные, которых не хватает в задаче, можно выбрать на свое усмотрение.

Решение можно прислать в виде ссылки на любой публичный git-репозиторий: GitHub, Bitbucket, GitLab и т.п.

Желаем успехов!


## 2. Generate your application sources (first time when got sources from repository)

    ./autogen.sh


## 3. Make your application 

    make

## 4. Run 
    
    src/kslift kslift

