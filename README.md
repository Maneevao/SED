# SED

## Функциональность элементов:
0. *dialog.(ui|cpp|h), mainwindow - интерфейс ПО
1. Server.(cpp|h) - класс сервера, объект которого создается в интерфейсе и управляется через публичные методы, сигналы и слоты
  + ServerUser.(cpp|h) - класс пользователя СЭД, то как его видит сервер, при подключении пользователя для него создается специальный объект данного класса, но только при успешной авторизации
  + Listener.(cpp|h) - объект, который создается вместе с объектом сервера, для прослушивания порта и получения информации для авторизации пользователей
  + UserConnection - объект обрабатывающий подключение пользователя, перенимает его у Listener и является его составной частью.
2. Client.(cpp|h) - класс клиента, объект которого создается в интерфейсе и управляется через публичные методы, сигналы и слоты
3. Общие объекты
  + Crypto - функции шифрования, которые являются общими для клиента и сервера
  + Doc - класс описывающий документ, поддерживает стандартные функции над документами, которые используются клиентом и сервером
  + TcpMessager - оболочка над протоколом TCP, которая позволяется абстрагироваться от сложности его реализации
