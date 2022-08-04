# 简易远程桌面连接工具 1.2.0
## 背景

> [@神末shenmo](https://bbs.deepin.org/user/279750)在https://bbs.deepin.org/post/228885 中发了一些在UOS上才有的应用，然后我发现远程协助这个应用应该只能连接其他Linux电脑，Windows（rdp协议）是连接不了的，而商店里的 Remmina 虽然功能强大，但并非是中文版的，于是我制作了这个程序
> ![神末shenmo 提取的远程协助工具](https://storage.deepin.org/thread/202112071502382593_%E5%9B%BE%E7%89%87.png)

## 介绍

这是一款使用 `dtk` 写的一个简易远程连接工具，基于 `rdesktop`，适用于连接`Windows`等 `rdp` 协议的远程计算机（`xrdp`也是可以的，测试过了），虽然和`mstsc` 差的有点远
在我测试中，我发现系统默认版本的 `rdesktop（1.8.6）` 是无法连接 `Windows XP`（不知道是不是个例），而编译的 `rdesktop 1.9.0` 是没有问题的，所以我也把编译好的应用放进 `deb` 包并支持选择版本
  
### 截图
![截图录屏_simple-remote-desktop-accessor_20220529072709.png](https://storage.deepin.org/thread/202205290736044793_截图录屏_simple-remote-desktop-accessor_20220529072709.png)
![深度截图_选择区域_20220529071844.png](https://storage.deepin.org/thread/202205290736041866_深度截图_选择区域_20220529071844.png)
![深度截图_选择区域_20220529071602.png](https://storage.deepin.org/thread/202205290736049721_深度截图_选择区域_20220529071602.png)
![截图_选择区域_20220529073314.png](https://storage.deepin.org/thread/202205290736264141_截图_选择区域_20220529073314.png)
![截图_simple-remote-desktop-accessor_20220529073040.png](https://storage.deepin.org/thread/202205290736263677_截图_simple-remote-desktop-accessor_20220529073040.png)

## 更新内容
### 1.2.0-1（2022年05月29日）
※1、优化了在 Ubuntu 下界面的显示，不会显示双标题栏  
※2、补充了缺失依赖  
![截图录屏_simple-remote-desktop-accessor_20220529072709.png](https://storage.deepin.org/thread/202205290736044793_截图录屏_simple-remote-desktop-accessor_20220529072709.png)
![深度截图_选择区域_20220529071844.png](https://storage.deepin.org/thread/202205290736041866_深度截图_选择区域_20220529071844.png)
![深度截图_选择区域_20220529071602.png](https://storage.deepin.org/thread/202205290736049721_深度截图_选择区域_20220529071602.png)
![截图_选择区域_20220529073314.png](https://storage.deepin.org/thread/202205290736264141_截图_选择区域_20220529073314.png)
![截图_simple-remote-desktop-accessor_20220529073040.png](https://storage.deepin.org/thread/202205290736263677_截图_simple-remote-desktop-accessor_20220529073040.png)
### 1.2.0（2022年05月28日）
※1、添加了剪切板共享、目录共享、远程计算机声音播放的功能  
※2、把使用 `std::system()` 运行命令修改为用`QProcess`运行命令以及获取返回值和输出内容  
※3、修复了在 UOS/Ubuntu 下运行因为编译的 Qt 版本过高而无法运行的问题  
※4、修复了不管连接成功还是失败都会弹出提示窗口显示日志的问题  
5、修复了连接日志不超过 5000 个字符的问题  
6、修复了标题以及自定义参数不支持中文的问题  
7、优化了代码结构  
8、修复了如果能在 Ubuntu 下运行的情况下显示不良的问题   
9、修复了在显示日志以及“高级”选项卡里面的参数帮助乱码的问题
![截图录屏_simple-remote-desktop-accessor_20220528211936.png](https://storage.deepin.org/thread/202205282130114462_截图录屏_simple-remote-desktop-accessor_20220528211936.png)
![深度截图_选择区域_20220528210628.png](https://storage.deepin.org/thread/202205282130109227_深度截图_选择区域_20220528210628.png)
![深度截图_选择区域_20220528210552.png](https://storage.deepin.org/thread/202205282130108296_深度截图_选择区域_20220528210552.png)
![截图_simple-remote-desktop-accessor_20220528212950.png](https://storage.deepin.org/thread/202205282130264494_截图_simple-remote-desktop-accessor_20220528212950.png)
![截图_选择区域_20220528212932.png](https://storage.deepin.org/thread/202205282130265167_截图_选择区域_20220528212932.png)
### 1.0.0
缺陷有很多，如下：
1、如果 `rdesktop` 连接的日志太多（多过 `5000` 个字符），程序就会崩溃（当然不是 `rdesktop` 崩溃）  
2、不管是连接成功还是失败，都会弹出提示窗口显示日志（如果连接正常就不要理了，不正常就要好好看看了，暂时只有英语版的）  
3、标题不支持中文  
4、在窗口填写的信息无法保存，关闭后会全部丢失  
5、代码结构较为混乱，只有少量注释  
6、……  

![截图_simple-remote-desktop-accessor_20220118124956.png](https://storage.deepin.org/thread/202201181412141466_截图_simple-remote-desktop-accessor_20220118124956.png)
![截图_simple-remote-desktop-accessor_20220118124947.png](https://storage.deepin.org/thread/202201181412131509_截图_simple-remote-desktop-accessor_20220118124947.png)
![截图_simple-remote-desktop-accessor_20220118124938.png](https://storage.deepin.org/thread/202201181412127276_截图_simple-remote-desktop-accessor_20220118124938.png)
![截图_simple-remote-desktop-accessor_20220118124927.png](https://storage.deepin.org/thread/202201181412124405_截图_simple-remote-desktop-accessor_20220118124927.png)
![截图_选择区域_20220118125655.png](https://storage.deepin.org/thread/202201181412102120_截图_选择区域_20220118125655.png)
![截图_选择区域_20220118125224.png](https://storage.deepin.org/thread/202201181412088240_截图_选择区域_20220118125224.png)
![截图_选择区域_20220118143511.png](https://storage.deepin.org/thread/202201181436064267_截图_选择区域_20220118143511.png)

## 相关项目
|项目|链接|
|-|-|
| 简易远程桌面连接工具（Gitee） | https://gitee.com/gfdgd-xi/simple-remote-desktop-accessor |
| 简易远程桌面连接工具（Github） | https://github.com/gfdgd-xi/simple-remote-desktop-accessor |
| rdesktop | https://github.com/rdesktop/rdesktop |
