# 简易远程桌面连接工具 1.0
## 背景

> [@神末shenmo](/user/279750)在https://bbs.deepin.org/post/228885中发了一些在UOS上才有的应用，然后我发现远程协助这个应用应该只能连接其他Linux电脑（没实验过，不太清楚），Windows（rdp协议）是连接不了的，而商店里的 Remmina 虽然功能强大，但并非是中文版的，于是我制作了这个程序
> ![](https://storage.deepin.org/thread/202112071502382593_%E5%9B%BE%E7%89%87.png)

## 介绍

这是一款使用 `dtk` 写的一个简易远程连接工具，基于 `rdesktop`，适用于连接`Windows`等 `rdp` 协议的远程计算机（`xrdp`也是可以的，测试过了），虽然和`mstsc` 差的有点远
在我测试中，我发现系统默认版本的 `rdesktop（1.8.6）` 是无法连接 `Windows XP`（不知道是不是个例），而编译的 `rdesktop 1.9.0` 是没有问题的，所以我也把编译好的应用放进 `deb` 包并支持选择版本（我编译的这个版本在优麒麟上无法运行）
顺便提示一下，因为我能力上的问题，缺陷有很多，如下：

+ 如果 `rdesktop` 连接的日志太多（多过 `5000` 个字符），程序就会崩溃（当然不是 `rdesktop` 崩溃）
+ 不管是连接成功还是失败，都会弹出提示窗口显示日志（如果连接正常就不要理了，不正常就要好好看看了，暂时只有英语版的）
+ 标题不支持中文
+ 在窗口填写的信息无法保存，关闭后会全部丢失
+ 代码结构较为混乱，只有少量注释
+ ……
  
### 截图
  
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
