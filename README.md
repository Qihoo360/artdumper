# artdumper
android5.0之后，从oat文件中dump出来dex的工具

##工具的由来：
因为平时工作中经常需要从framwork中dump出一些系统dex，并且分析其行为，但在5.0之后没有类似的工具，所以自己开发了这个工具。

##工具的实现原理：
从android5.0之后，虚拟机变更为art虚拟机，对应的系统文件也变更为oat格式，oat只是对于linux的elf文件的一个扩展，在内部包含一个完整的dex文件，根据这个思路实现了这个dump工具。

##具体方法：
1、从elf的标准文件头找到具体的rodata段，并且获取其段的开始地址和大小；
2、从rodata段开始才是一个完整的oat文件，解析具体的oat头，获取oat头的信息；
3、跳过oat文件的头和对应的key-value存储值；
4、获取dex的个数后（因为一个oat中可以包含多个dex文件），依次循环获取dex的文件名和具体dex的offset和size；
5、将具体的dex文件dump到指定目录；

其中elf解析参考和引用了elfloader和google对于elf文件解析的代码，oat的解析参考了android相关art的实现。

##工具如何使用：
 Usage: OATLoaderDumper [options]
 --oat-file：要dump的oat文件。
 --out-path：dump的输出目录。
 
 具体例子： OATLoaderDumper --oat-file=F:\androidL\framework\arm\wm.odex --out-path=F:\androidL\outdex\

##系统：
目前只是支持windows，后续会陆续支持linux和mac
