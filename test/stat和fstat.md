fstat和stat都是用于获取文件状态信息的函数，但是它们的参数不同，具体区别如下：

stat函数的参数是文件名，而fstat函数的参数是文件描述符。

stat函数返回指定文件的状态信息，而fstat函数返回由文件描述符指定的文件的状态信息。

stat函数可以用于任何类型的文件，而fstat函数只能用于已经打开的文件。

stat函数返回的是文件的状态信息，而fstat函数返回的是文件描述符所指向的文件的状态信息。

总之，fstat和stat函数都是用于获取文件状态信息的函数，但是它们的参数和使用场景有所不同。
