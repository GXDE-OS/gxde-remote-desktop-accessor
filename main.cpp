/*
 * 如果你想学习更多有关知识，可以访问以下网址：
 * DTK相关文档地址：https://linuxdeepin.github.io/dtk
 * Deepin项目地址：https://github.com/linuxdeepin
 * 社区项目投递地址：https://gitee.com/deepin-opensource
*/

#include "mainwindow.h"
#include <DApplication>

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication a(argc, argv);

    a.loadTranslator(); //加载翻译
    a.setAttribute(Qt::AA_UseHighDpiPixmaps);
    a.setOrganizationName("gfdgd xi"); //设置组织名称
    a.setApplicationVersion(DApplication::buildVersion("1.0beta1"));//设置程序版本
    a.setProductIcon(QIcon::fromTheme("/opt/durapps/spark-simple-remote-desktop-accessor/icon.svg"));  //设置程序Logo
    a.setProductName("远程桌面连接器"); //设置项目名称
    a.setApplicationDescription("一个简单的远程桌面连接器（基于rdesktop）");//设置程序介绍
    a.setApplicationName("远程桌面连接器"); //只有在这儿修改窗口标题才有效


    MainWindow w;
    w.show();

    return a.exec();
}
