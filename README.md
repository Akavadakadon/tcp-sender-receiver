# Тестовое задание
Копач А.А. для Imaqliq 

# Постановка задачи.
Необходимо реализовать клиент-серверное приложение под Linux. 
Клиент - программа, запускаемая из консоли. 
Сервер - демон, корректно завершающийся по сигналам SIGTERM и SIGHUP. 
Клиент должен передать содержимое текстового файла через TCP. 
Сервер должен принять и сохранить в файл.

# Решение.
`tcpSender` - Клиент, который запускается из консоли
Использование
```
Usage: tcpSender [1]server_ip [2]server_port [3]file_path
```

`tcpReceiver` - Сервер, который запускается как демон или как прога
Использование
```
Usage: tcpReceiver [1]mode:-d for daemon -e for executable [2]server_port [3]File path
```

# Как собрать с g++
Собираем
```
g++ tcpSender/tcpSender.cpp -o tcpSender.out
g++ tcpReceiver/tcpReceiver.cpp -o tcpReceiver.out
```

tcpSender требует файл для чтения.
tcpReceiver созданного файла не требует, файл будет создан при работе

Пример создания файла для отправки
```
echo $'Line1\nLine2' >> sendTxt
```

Пример запуска
```
./tcpReceiver.out -d 626269 recvTxt
./tcpSender.out 127.0.0.1 626269 sendTxt
```
вызов `cat recvTxt`
выведет 
```
Line1
Line2
```