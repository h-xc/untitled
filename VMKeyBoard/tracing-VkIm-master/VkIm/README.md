#VkIm
- 这是QT5的软键盘输入法实例，采用QT5的输入法框架实现
- 编译后应该有libvkimplatforminputcontextplugin.so，keyboard和Dialog三个程序，插件会自动放到qt安装目录的插件目录下
- 测试方法
 1. 使用qtcreator运行，先运行keyboard项目，注册服务和对象，再运行Dialog项目，点击编辑框输入法就能跳出来了
 2. 在终端运行，首先设置环境变量QT_IM_MODULE=vkim，然后运行keyboard, 再运行Dialog
- 设置好环境变量，运行输入法程序(keyboard)之后，任意一个有编辑框的程序都可以调出输入法
- 参考的文章及代码[http://www.kdab.com/qt-input-method-depth/](http://www.kdab.com/qt-input-method-depth/)
- 详细说明见[blog.csdn.net/tracing](http://blog.csdn.net/tracing/article/details/50617571)或者[cnblogs.com/tracing](http://www.cnblogs.com/tracing/p/5174773.html)

**Enjoy it!**
