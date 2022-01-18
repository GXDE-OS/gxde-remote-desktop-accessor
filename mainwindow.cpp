#include "mainwindow.h"
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <DWidgetUtil> //加入此头文件方可使用moveToCenter
#include <DSearchEdit>
#include <DTitlebar>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <DLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <DPushButton>
#include <DFileDialog>
#include <iostream>
#include <QByteArray>
#include <DCheckBox>
#include <DMessageBox>
#include <DFrame>
#include <DTabWidget>
#include <DRadioButton>
#include <QButtonGroup>
#include <QGridLayout>
#include <QSlider>
#include <DGroupBox>
#include <DTextBrowser>
#include <fstream>
#define MAXPATH 1000

MainWindow::MainWindow(DMainWindow *parent)
    : DMainWindow(parent)
{
    // 获取程序目录
    getcwd(programPath, 100);
    std::clog << "Path: " << programPath << std::endl;
    //初始化主窗口 Initialize the main window
    setCentralWidget(w);//将w作为窗口的用户部分（整个窗口除了标题栏的部分）
    moveToCenter(this); //把窗口移动到屏幕中间
    w->setWindowTitle(tr("远程桌面连接器"));
    this->setWindowFlags(windowFlags() &~ Qt::WindowMaximizeButtonHint);
    //w->setWindowFlags(w->windowFlags() &~Qt::WindowMaximizeButtonHint);
    MainWindow::resize(500, 100); //改变窗口大小应当改变MainWindow的大小

    /*DLabel *ipTips = new DLabel("IP地址：");
    ipTips->setParent(w);
    ipTips->show();*/

    ip = new DLineEdit;
    ip->setParent(w);
    ip->setToolTip(tr("请在这里输入IP地址"));
    ip->setPlaceholderText(tr("请在这里输入IP地址"));

    ip->show();

    DPushButton *connectButton = new DPushButton;
    connectButton->setText(tr("连接"));
    connectButton->setParent(w);
    connect(connectButton, &DPushButton::clicked, this, &MainWindow::ConnectIp);
    connectButton->show();

    /*DLabel *userName = new DLabel("用户名：本地登录用户名");
    userName->setParent(w);
    userName->show();*/

    moreSetting = new DCheckBox("更多选项");
    moreSetting->setParent(w);
    moreSetting->setCheckable(true);
    moreSetting->show();
    //moreSetting->adjustSize();
    connect(moreSetting, &DCheckBox::stateChanged, this, &MainWindow::ShowMoreSetting);

    QHBoxLayout *moreSettingLayout = new QHBoxLayout;
    moreSettingLayout->setParent(moreSettingFrame);

    moreSettingFrame = new DFrame;
    moreSettingFrame->setParent(w);
    moreSettingFrame->setEnabled(false);
    moreSettingFrame->setLayout(moreSettingLayout);

    DWidget *showTab = new DWidget;
    showTab->setParent(moreSettingFrame);
    showTab->show();
    DWidget *userPasswordTab = new DWidget();
    userPasswordTab->setParent(moreSettingFrame);
    userPasswordTab->show();

    user = new DLineEdit();
    user->setParent(userPasswordTab);
    user->setPlaceholderText("在这里输入用户名（不输入则以当前电脑登录用户的用户名）");
    user->show();

    password = new DPasswordEdit();
    password->show();

    showScreen = new QButtonGroup();
    DRadioButton *fullScreen = new DRadioButton("全屏访问");
    fullScreen->show();
    fullScreen->setParent(showTab);
    //QVBoxLayout *sizeScreenLayout = new QVBoxLayout();
    DRadioButton *sizeScreen = new DRadioButton("指定分辨率访问");
    sizeScreen->setChecked(true);
    sizeScreen->setParent(showTab);
    sizeScreen->show();

    showScreen->addButton(fullScreen, 0);
    showScreen->addButton(sizeScreen, 1);
    connect(moreSetting, &DCheckBox::stateChanged, this, &MainWindow::ShowMoreSetting);
    connect(fullScreen, &DRadioButton::clicked, this, &MainWindow::ShowScreenEnabled);
    connect(sizeScreen, &DRadioButton::clicked, this, &MainWindow::ShowScreenEnabled);


    sizeScreenWidth = new DLineEdit();
    sizeScreenWidth->setParent(showTab);
    sizeScreenWidth->setText("800");
    sizeScreenWidth->show();
    sizeScreenHeight = new DLineEdit();
    DLabel *cheng = new DLabel(tr("×"));
    cheng->setParent(showTab);
    cheng->show();
    sizeScreenHeight->setParent(showTab);
    sizeScreenHeight->setText("600");
    sizeScreenWidth->show();

    DLabel *showTitleTips = new DLabel("标题：");
    showTitleTips->setParent(showTab);
    showTitleTips->show();

    showTitle = new DLineEdit();
    showTitle->setParent(showTab);
    showTitle->show();

    DLabel *colorTips = new DLabel(tr("色彩位数："));
    colorTips->setParent(showTab);
    colorTips->show();

    QHBoxLayout *colorLayout = new QHBoxLayout();

    color = new DSlider();
    color->setMaximum(3);
    color->setMinimum(0);
    color->setValue(3);
    color->setParent(showTab);
    color->show();
    connect(color, &DSlider::valueChanged, this, [&](){
        QString number = QString::number(MainWindow::color->value() * 8 + 8);
        number.append("位");
        colorShow->setText(number);
    });

    colorShow = new DLabel("32位");
    colorShow->setParent(showTab);
    colorShow->show();

    colorLayout->addWidget(color);
    colorLayout->addWidget(colorShow);

    QGridLayout *showTabLayout = new QGridLayout;
    showTabLayout->setParent(showTab);
    showTabLayout->addWidget(fullScreen, 0, 0);
    showTabLayout->addWidget(sizeScreen, 1, 0);
    showTabLayout->addWidget(sizeScreenWidth, 1, 1);
    showTabLayout->addWidget(cheng, 1, 2);
    showTabLayout->addWidget(sizeScreenHeight, 1, 3);
    showTabLayout->addWidget(showTitleTips, 2, 0);
    showTabLayout->addWidget(showTitle, 2, 1, 1, 3);
    showTabLayout->addWidget(colorTips, 3, 0);
    //showTabLayout->addWidget(color, 3, 1, 1, 2);
    //showTabLayout->addWidget(colorShow, 3, 3);
    showTabLayout->addLayout(colorLayout, 3, 1, 1, 3);
    showTab->setLayout(showTabLayout);

    QGridLayout *userPasswordLayout = new QGridLayout();
    userPasswordLayout->addWidget(new DLabel(tr("用户名：")), 0, 0);
    userPasswordLayout->addWidget(user, 0, 1);
    userPasswordLayout->addWidget(new DLabel(tr("密码：")), 1, 0);
    userPasswordLayout->addWidget(password, 1, 1);
    userPasswordTab->setLayout(userPasswordLayout);
    //QHBoxLayout
    QVBoxLayout *connectLayout = new QVBoxLayout();
    QVBoxLayout *rdpVersionLayout = new QVBoxLayout();
    QVBoxLayout *rdesktopVersionLayout = new QVBoxLayout();

    DWidget *connectTab = new DWidget();
    connectTab->setParent(moreSettingFrame);
    connectTab->setLayout(connectLayout);
    connectTab->show();

    DGroupBox *rdpVersionBox = new DGroupBox();
    rdpVersionBox->setParent(connectTab);
    rdpVersionBox->show();
    rdpVersionBox->setTitle("rdp 版本");
    rdpVersionBox->setLayout(rdpVersionLayout);
    connectLayout->addWidget(rdpVersionBox);

    DRadioButton *rdp4 = new DRadioButton();
    rdp4->setParent(rdpVersionBox);
    rdp4->setText("设置为rdp4协议连接");
    rdp4->show();

    DRadioButton *rdp5 = new DRadioButton();
    rdp5->setParent(rdpVersionBox);
    rdp5->setText("设置为rdp5协议连接（推荐）");
    rdp5->setChecked(true);
    rdp5->show();

    rdpVersion = new QButtonGroup();
    rdpVersion->addButton(rdp4, 0);
    rdpVersion->addButton(rdp5, 1);

    rdpVersionLayout->addWidget(rdp4);
    rdpVersionLayout->addWidget(rdp5);

    DGroupBox *rdesktopVersionBox = new DGroupBox();
    rdesktopVersionBox->setParent(connectTab);
    rdesktopVersionBox->show();
    rdesktopVersionBox->setTitle("rdesktop 版本");
    rdesktopVersionBox->setLayout(rdesktopVersionLayout);
    connectLayout->addWidget(rdesktopVersionBox);

    DRadioButton *systemRdesktop = new DRadioButton();
    systemRdesktop->setParent(rdesktopVersionBox);
    systemRdesktop->setText("设置为系统版本的rdesktop（可能会不支持连接Windows XP）");
    systemRdesktop->show();

    DRadioButton *rdesktop190 = new DRadioButton();
    rdesktop190->setParent(rdesktopVersionBox);
    rdesktop190->setText("设置为rdesktop 1.9.0（测试支持连接Windows XP）");
    rdesktop190->setChecked(true);
    rdesktop190->show();

    rdesktopVersion = new QButtonGroup();
    rdesktopVersion->addButton(systemRdesktop, 0);
    rdesktopVersion->addButton(rdesktop190, 1);

    rdesktopVersionLayout->addWidget(systemRdesktop);
    rdesktopVersionLayout->addWidget(rdesktop190);

    DWidget *highSetting = new DWidget();
    highSetting->setParent(moreSettingFrame);
    highSetting->show();

    QVBoxLayout *highSettingLayout = new QVBoxLayout();
    highSettingLayout->setParent(highSetting);
    highSetting->setLayout(highSettingLayout);

    DTextBrowser *help = new DTextBrowser();
    help->setParent(highSetting);
    std::ifstream file;
    file.open("/home/gfdgd_xi/temp.txt");
    char allHelp[10000];
    char things[100];
    //file>>things;
    while (file.getline(things, 100))
    {
        strcat(allHelp, "\n");
        strcat(allHelp, things);
    }
    help->setText(allHelp);
    std::clog << "Read from file: " << allHelp;
    help->show();
    highSettingLayout->addWidget(help);

    command = new DLineEdit();
    command->setPlaceholderText("请在这里输入需要的参数（上面是参数示例）");
    command->setParent(highSetting);
    command->show();
    highSettingLayout->addWidget(command);

    DTabWidget *moreSettingTab = new DTabWidget;
    moreSettingTab->setParent(moreSettingFrame);
    moreSettingTab->addTab(showTab, tr("显示"));
    moreSettingTab->addTab(userPasswordTab, tr("用户"));
    moreSettingTab->addTab(connectTab, tr("连接"));
    moreSettingTab->addTab(highSetting, tr("高级"));
    moreSettingTab->show();

    moreSettingLayout->addWidget(moreSettingTab);

    QVBoxLayout *AllWidget = new QVBoxLayout;
    QHBoxLayout *firstLineWidget = new QHBoxLayout;
    firstLineWidget->addWidget(new DLabel("IP地址："));
    firstLineWidget->addWidget(ip);
    firstLineWidget->addWidget(connectButton);

    AllWidget->addLayout(firstLineWidget);
    AllWidget->setStretchFactor(firstLineWidget, 1);
    //AllWidget->addWidget(new DLabel("用户名：本地登录用户名"));
    //AllWidget->setStretchFactor(new DLabel("用户名：本地登录用户名"), 1);
    //AllWidget->addWidget(fullScreen);
    AllWidget->addWidget(moreSetting);
    AllWidget->setStretchFactor(moreSetting, 1);
    AllWidget->addWidget(moreSettingFrame);
    AllWidget->setStretchFactor(moreSettingFrame, 6);

    w->setLayout(AllWidget);

    //在标题栏上添加一个菜单/菜单项 Add a menu/menu item to the title bar
    QMenu *menu=new QMenu;
    QAction *giteeAction=new QAction("Gitee 网站");
    menu->addAction(giteeAction);
    titlebar()->setMenu(menu);
    connect(giteeAction, &QAction::triggered, this, [&](){std::system("xdg-open https://gitee.com/gfdgd-xi/simple-remote-desktop-accessor");});


    QAction *githubAction=new QAction("Github 网站");
    menu->addAction(githubAction);
    titlebar()->setMenu(menu);
    connect(githubAction, &QAction::triggered, this, [&](){std::system("xdg-open https://github.com/gfdgd-xi/simple-remote-desktop-accessor");});

    //设置标题栏上的图标 Set the icon on the title bar
    titlebar()->setIcon(QIcon::fromTheme("/opt/durapps/spark-simple-remote-desktop-accessor/icon.svg"));

    //让程序适配浅色模式和深色模式 Adapt the program to light and dark model
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,this,&MainWindow::setTheme);
}
void MainWindow::ShowScreenEnabled(){
    MainWindow::sizeScreenWidth->setEnabled(MainWindow::showScreen->checkedId());
    MainWindow::sizeScreenHeight->setEnabled(MainWindow::showScreen->checkedId());
}
void MainWindow::ShowMoreSetting(){
    if (MainWindow::moreSetting->checkState()){
        moreSettingFrame->setEnabled(true);
        return;
    }
    moreSettingFrame->setEnabled(false);
}
char * MainWindow::ReadFile(char *path){
    std::ifstream file;
    file.open(path);
    char things[100];
    file >> things;
    return things;
}

void MainWindow::ConnectIp(){
    if(MainWindow::ip->text() == ""){
        DMessageBox::information(w, tr("提示"), tr("没有输入IP地址，无法继续"));
        return;
    }
    QByteArray ip = MainWindow::ip->text().toLatin1();
    char command[500] = "";
    if (moreSetting->isChecked()){
        switch (MainWindow::rdesktopVersion->checkedId()) {
            case 0:
                strcat(command, "rdesktop ");
            break;
        case 1:

            //strcat(command, programPath);
            strcat(command, "/opt/durapps/spark-simple-remote-desktop-accessor/rdesktop ");
            break;
        }

        strcat(command, ip.data());
    std::cout<<MainWindow::showScreen->checkedId();
    switch (MainWindow::showScreen->checkedId()){
    case 0:
        strcat(command, " -f");
        break;
    case 1:
        strcat(command, " -g '");
        QByteArray screenWidth = MainWindow::sizeScreenWidth->text().toLatin1();
        QByteArray screenHeight = MainWindow::sizeScreenHeight->text().toLatin1();
        strcat(command, screenWidth.data());
        strcat(command, "x");
        strcat(command, screenHeight.data());
        strcat(command, "'");
        break;
    }
    QByteArray title = MainWindow::showTitle->text().toLatin1();
    QByteArray users = MainWindow::user->text().toLatin1();
    QByteArray passwords = MainWindow::password->text().toLatin1();
    strcat(command, " -T '");
    strcat(command, title.data());
    strcat(command, "'");
    if(MainWindow::user->text() != ""){
    strcat(command, " -u '");
    strcat(command, users);
    strcat(command, "'");
    }
    if(MainWindow::password->text()!=""){
    strcat(command, " -p '");
    strcat(command, passwords);
    strcat(command, "'");
    }
    strcat(command, " -a ");
    char colorNUmber[5];
    sprintf(colorNUmber, "%d", MainWindow::color->value() * 8 + 8);
    strcat(command, colorNUmber);
    switch (MainWindow::rdpVersion->checkedId()) {
        case 0:
            strcat(command, " -4 ");
            break;
        case 1:
            strcat(command, " -5 ");
            break;
    }
    strcat(command, " ");
    QByteArray commands = MainWindow::command->text().toLatin1();
    strcat(command, commands.data());
    }
    else {
        strcat(command, "rdesktop ");
        strcat(command, ip.data());
        strcat(command, " -T '");
        strcat(command, ip.data());
        strcat(command, "' ");
    }
    w->setEnabled(false);
    strcat(command, " 2> /tmp/rdesktop.txt");
    //char aaa[500] = "/home/gfdgd_xi/Documents/git/build-DtkDemo-unknown-Debug/rdesktop 120.25.153.145 -g '800x600' -T '' -a 32 -5   2> /tmp/rdesktop.txt";
       int returnValue = std::system(command);
    //int returnValue = std::system("/home/gfdgd_xi/Documents/git/build-DtkDemo-unknown-Debug/rdesktop 120.25.153.145 -g '800x600' -T '' -a 32 -5   2> /tmp/rdesktop.txt");
        std::cout<<command<<std::endl;
        std::cout<<"rdesktop 运行返回值："<<returnValue<<std::endl;
        w->setEnabled(true);
        std::ifstream file;
        file.open("/tmp/rdesktop.txt");
        char allHelp[5000];
        char things[300];
        //file>>things;
        while (file.getline(things, 300))
        {
            strcat(allHelp, things);
            strcat(allHelp, "\n");
        }
        DMessageBox::information(this, tr("提示"), tr(allHelp));
        std::clog << "Read from file: " << allHelp;


}
MainWindow::~MainWindow()
{
    delete w;
}

void MainWindow::setTheme(DGuiApplicationHelper::ColorType theme)
{
    //这个函数在程序运行之初就会被调用一次，而后每次更换主题颜色时被调用。 This function is called at the beginning of the program and then every time the theme color is changed.
    if(theme==DGuiApplicationHelper::LightType){ //在这个位置设置两种不同的样式 Set two different styles at this location
        //浅色模式 Light Mode
        qDebug()<<"Lighe";
    }else {
        //深色模式 Dark Mode
        qDebug()<<"Dark";
    }
}
