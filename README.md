# Chat-Room-Programming
### Create a chat room by using TCP protocol with Thread and Select()

以basic來說，透過執行緒(thread)來傳遞訊息，在server端，前面socket()建立、bind()、listen()及accept()的部分基本上和HW1一樣使用三項式交握。之後create一個thread，當程式執行到此時thread會呼叫create的第三個引數的function，並在此function內執行receive及send的動作。分辨client誰是誰是利用宣告一個陣列並存放每一個接收的client，藉此來分辨此訊息是從哪一位client發送過來。在client的部分，一樣透過建立thread來接收從server送過來的訊息。以advanced來說，透過select()來做同步多工(I/O)，透過select()來同時監控多個socket，來告訴哪些socket可以讀取或寫入等操作。
