# SED

## Функциональность элементов:
0. *dialog.(ui|cpp|h), mainwindow - интерфейс ПО
1. Server.(cpp|h) - класс сервера, объект которого создается в интерфейсе и управляется через публичные методы, сигналы и слоты
  1.1 ServerUser.(cpp|h) - класс пользователя СЭД, то как его видит сервер, при подключении пользователя для него создается специальный объект данного класса, но только при успешной авторизации
  1.2 Listener.(cpp|h) - объект, который создается вместе с объектом сервера, для прослушивания порта и получения информации для авторизации пользователей
  1.3 UserConnection - объект обрабатывающий подключение пользователя, перенимает его у Listener и является его составной частью.
2. Client.(cpp|h) - класс клиента, объект которого создается в интерфейсе и управляется через публичные методы, сигналы и слоты
3. Общие объекты
  3.1 Crypto - функции шифрования, которые являются общими для клиента и сервера
  3.2 Doc - класс описывающий документ, поддерживает стандартные функции над документами, которые используются клиентом и сервером
  3.3 TcpMessager - оболочка над протоколом TCP, которая позволяется абстрагироваться от сложности его реализации
