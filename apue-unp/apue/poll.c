// 阻塞I/O
// 非阻塞I/O，将两个输入描述符都设置为非阻塞的，对第一个描述符发一个read。如果该输入上有数据，则读数据并处理它；如果无数据可读，则read立即返回。然后对第二个描述符作同样的处理。在此之后，等待若干秒，然后再读第一个描述符。不足之处是浪费cpu时间。
// 异步I/O，基本思想是进程告诉内核，当一个描述符已准备好可以进行I/O时，用一个信号通知它。
// I/O多路转接(I/O multiplexing)，先构造一张有关描述符的列表，然后调用一个函数，直到这些描述符中的一个已经准备好进行I/O时，该函数才返回。在返回时，它告诉进程哪些描述符已准备好进行I/O。