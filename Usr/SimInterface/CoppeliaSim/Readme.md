### 1. **项目环境配置**

1. 添加包含路径：我们需要把CoppeliaSim提供的include、common、remoteApi文件夹添加到包含目录，并把remoteApi里的文件（包含函数的实现）添加到项目中。在比较旧的版本中，remoteApi的实现被编译封装到remoteApi.lib中，这个文件需要单独下载。因为remoteApi里面的文件需要相互调用，所以这个包含路径需要添加到项目的包含目录中，当然也可以添加到系统的路径中和其他项目共享。
2. 配置宏定义：为了正确使能remoteApi，需要配置几个预编译宏，完全按照官网的[指导手册](http://www.coppeliarobotics.com/helpFiles/en/remoteApiClientSide.htm)来配置就可以。

注意：remoteApi库中本身是可跨平台运行的，因此移植库的时候无需关注平台。

#### 2. **CoppeliaSim.cpp**

封装CoppeliaSim软件模块的目标是方便开发者快速与服务器建立连接和配置客户端，所以模块里面使用了常用的配置，使用者只需调用对应的API即可进行通信，如果需要用到其他API，或者根据其他参数进行配置，可以看官方的[API框架及其子板块的介绍](http://www.coppeliarobotics.com/helpFiles/en/apisOverview.htm)，然后自行添加有关函数，不调用本模块封装的函数。使用前请先阅读[各种仿真模式的区别](http://www.coppeliarobotics.com/helpFiles/en/remoteApiModusOperandi.htm)（默认用非堵塞式模式）。对于仿真模式的选择其实非常重要，为了简单实现，我们选择用异步模式+数据流模式。

#### 3. **代码中的使用方法**

1. **数据类型**

   - `_simOP_Type`: 操作类型（`Operation Type`），可对某个对象进行不同类型的操作，包括修改或读取位置、姿态、四元数、图片、速度、力、信号等等。
   - `_simIO_Type`: 输入输出类型（`Input/Output Type`），有只读（`Read Only`），只写（`Write Only`）和读写（`Read and Write`）三种。在Coppeliasim中不同的对象有不同的属性，比如不能写入力传感器的值而只能读取它。同时Coppeliasim提供的通信API每次只能进行一种操作，所以为了尽可能减少通信的次数，我给出这三种可选属性。
   - `_simObj_Type`:对象类型（`Object Type`），模块里面暂时只提供了四种对象类型，分别是关节（Joint）、视觉传感器（Vision sensor）、力传感器（Force sensor）和其他对象（Other Object），对应Coppeliasim里面的不同对象类型。其他对象包含
   - `_simSignal_Type`：信号类型的对象，如要进阶使用可参考官方手册，或参考步兵项目中的使用方法。
   - `_simCollision_Type`：处理碰撞的对象，如要进阶使用可参考官方手册，或参考步兵项目中的使用方法。
   - `_simOPParam_Struct`:操作参数结构体，保存了某个对象要进行的操作类型和返回值，可用于调试。

2. **API的使用**

   - 模块中主要用到的有3个API

     ```c++
     bool Start(const char *p_IP, int32_t p_conection_port, int commThreadCycleInMs, int operationMode)
     ```

      用于启动通信服务和建立连接，建议不要中断尝试重连。

     ```c++
     _simObjectHandle_Type* Add_Object(std::string full_name, _simObj_Type type, std::initializer_list<simxInt> operation_ls)：
     ```

     用于添加对象到对象列表，第一个参数为对象在Coppeliasim中的全名，第二个参数为对象的类型，第三个参数是操作列表，操作可以有多个，用逗号隔开即可。返回值为对象的句柄（地址），可以通过句柄来操作数据。具体结构体中每个变量代表的数据可参考注释。

     ```c++
     bool ComWithServer();
     ```

     与Coppleliasim的主要操作封装在里面，会循环对象列表中每个对象的每个操作，然后调用对应的API。CoppeliaSim remote API的库自己有一个线程来处理API的请求，而且会有一个请求列表已存储的操作。具体实现可以阅读remote API的源码，但不影响这部分的使用。

3. 为了说明代码是如何使用的，请参考步兵的工程配置。

